//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "boost/program_options.hpp"

#include "wartzaar/game_client.h"
#include "wartzaar/messages/game_messages_common.h"
#include "wartzaar/tzaar_game.h"

namespace po = boost::program_options;
namespace wm = wartzaar::messages;

int main(int argc, char *argv[]) {
  //----------------------------------------------------------------------------
  // Parse the command line options.
  //----------------------------------------------------------------------------
  po::options_description desc("Usage");
  desc.add_options()
      ("help", "Output this usage information.")

      ("host", po::value<std::string>()->default_value("127.0.0.1"),
          "Address of the game manager server.")
      ("port", po::value<int>()->default_value(2525),
          "TCP/IP port on which the game manager is listening.")

      ("turn-time", po::value<int>()->default_value(20),
          "Allowed time for each turn move, in seconds.")
      ("search-depth", po::value<int>()->default_value((std::numeric_limits<int>::max)()),
          "Maximum depth of the minimax search.")
      ("beam-size", po::value<int>()->default_value((std::numeric_limits<int>::max)()),
          "Number of states to search at each ply.")

      ("tzaar-coefficient", po::value<int>()->default_value(64),
          "Weight of tzaar pieces in heuristic evaluation.")
      ("tzarra-coefficient", po::value<int>()->default_value(8),
          "Weight of tzarra pieces in heuristic evaluation.")
      ("tott-coefficient", po::value<int>()->default_value(1),
          "Weight of tott pieces in heuristic evaluation.")
      ("stack-coefficient", po::value<int>()->default_value(10),
          "Factor to use to normalize stack height values.");

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  }
  catch (const boost::program_options::error& e) {
    std::cerr << "WarTzaar: " << e.what() << std::endl;
    std::cout << desc << std::endl;
    return 1;
  }

  // Output the usage message and exit
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  // Output the value of each option
  if (vm.count("host"))
    std::cout << "Host: " << vm["host"].as<std::string>() << std::endl;

  if (vm.count("port"))
    std::cout << "Port: " << vm["port"].as<int>() << std::endl;

  if (vm.count("turn-time"))
    std::cout << "Turn time: " << vm["turn-time"].as<int>() << std::endl;

  if (vm.count("search-depth"))
    std::cout << "Search depth: " << vm["search-depth"].as<int>() << std::endl;

  if (vm.count("beam-size"))
    std::cout << "Beam size: " << vm["beam-size"].as<int>() << std::endl;

  if (vm.count("tzaar-coefficient"))
    std::cout << "Tzaar coefficient: " << vm["tzaar-coefficient"].as<int>() << std::endl;

  if (vm.count("tzarra-coefficient"))
    std::cout << "Tzarra coefficient: " << vm["tzarra-coefficient"].as<int>() << std::endl;

  if (vm.count("tott-coefficient"))
    std::cout << "Tott coefficient: " << vm["tott-coefficient"].as<int>() << std::endl;

  if (vm.count("stack-coefficient"))
    std::cout << "Stack coefficient: " << vm["stack-coefficient"].as<int>() << std::endl;

  //----------------------------------------------------------------------------
  // Create the game client and establish a connection with the game manager.
  //----------------------------------------------------------------------------
  wartzaar::GameClient tzaar_client(vm["host"].as<std::string>(), vm["port"].as<int>());

  try {
    tzaar_client.Connect();
  }
  catch (std::runtime_error &e) {
    std::cerr << "Failed to connect to game manager: " << e.what() << std::endl;
    return 1;
  }

  std::cout << "Connected to game manager!" << std::endl;

  //----------------------------------------------------------------------------
  // Create the game AI.
  //----------------------------------------------------------------------------
  wartzaar::TzaarGame tzaar_game(
      vm["turn-time"].as<int>(),
      vm["search-depth"].as<int>(),
      vm["beam-size"].as<int>(),
      vm["tzaar-coefficient"].as<int>(),
      vm["tzarra-coefficient"].as<int>(),
      vm["tott-coefficient"].as<int>(),
      vm["stack-coefficient"].as<int>()
  );

  //----------------------------------------------------------------------------
  // Enter the main program loop.
  //
  // Wait for the game manager to send a message, then respond based on the type
  // of message. Do this until a GameOver message is received.
  //----------------------------------------------------------------------------
  while (1) {
    try {
      // TODO Use select() before sending and receiving!
      std::string text = tzaar_client.ReceiveGameMessage();
      std::cout << "Message received from game manager: " << text << std::endl;

      //------------------------------------------------------------------------
      // BoardState message.
      //
      // Spawn a new GameState based on the given board and update the TzaarGame
      // with this state.
      //------------------------------------------------------------------------
      if (text.substr(0, strlen("BoardState")) == "BoardState") {
        tzaar_game.set_current_state(wartzaar::GameState(
          wm::BoardStateMessage(text).board_state()
        ));
      }
      //------------------------------------------------------------------------
      // Chat message.
      //
      // Simply print the chat message to the console.
      //------------------------------------------------------------------------
      else if (text.substr(0, strlen("Chat")) == "Chat") {
        std::cout << "Chat: "
                  << wm::ChatMessage(text).payload()
                  << std::endl;
      }
      //------------------------------------------------------------------------
      // Control message.
      //
      // This is currently a noop. Simply print the message to the console.
      //------------------------------------------------------------------------
      else if (text.substr(0, strlen("Control")) == "Control") {
        std::cout << "Control: "
                  << wm::ControlMessage(text).payload()
                  << std::endl;
      }
      //------------------------------------------------------------------------
      // GameOver message.
      //------------------------------------------------------------------------
      else if (text.substr(0, strlen("GameOver")) == "GameOver") {
        wm::GameOverMessage message(text);

        tzaar_client.Disconnect();
        break;
      }
      //------------------------------------------------------------------------
      // Move message.
      //
      // Make the opponent's move on our board.
      //------------------------------------------------------------------------
      else if (text.substr(0, strlen("Move")) == "Move") {
        wm::MoveMessage message(text);
        tzaar_game.MakeMove(message.from_column(), message.from_row(),
                            message.to_column(), message.to_row());

        tzaar_game.set_turn_move_count(0);

        std::cout << "Move: " << message.from_column() << ", "
                              << message.from_row()    << " -> "
                              << message.to_column()   << ", "
                              << message.to_row()      << std::endl;
      }
      //------------------------------------------------------------------------
      // Version message.
      //
      // Store the game and manager versions in the GameClient.
      //------------------------------------------------------------------------
      else if (text.substr(0, strlen("Version")) == "Version") {
        wm::VersionMessage message(text);
        tzaar_client.set_game_version(message.game_version());
        tzaar_client.set_manager_version(message.manager_version());

        std::cout << "Manager Version: " << message.manager_version() << std::endl;
        std::cout << "Game Version: " << message.game_version() << std::endl;
      }
      //------------------------------------------------------------------------
      // YourPlayerNumber message.
      //------------------------------------------------------------------------
      else if (text.substr(0, strlen("YourPlayerNumber")) == "YourPlayerNumber") {
        wm::YourPlayerNumberMessage message(text);
        tzaar_game.set_player_number(message.payload());
      }
      //------------------------------------------------------------------------
      // YourTurn message.
      //
      // If this is the first turn of the game, make a single capturing move.
      // Otherwise, make a capturing then a capturing-or-stacking move.
      //------------------------------------------------------------------------
      else if (text.substr(0, strlen("YourTurn")) == "YourTurn") {
        if (tzaar_game.turn_count() == 0) {
          tzaar_client.SendGameMessage(tzaar_game.GetNextMove(true));
        }
        else {
          if (tzaar_game.turn_move_count() == 0)
            tzaar_client.SendGameMessage(tzaar_game.GetNextMove(true));
          else
            tzaar_client.SendGameMessage(tzaar_game.GetNextMove(false));
        }

        tzaar_game.set_turn_move_count(tzaar_game.turn_move_count() + 1);
      }
    }
    catch (std::runtime_error &e) {
      std::cerr << "Runtime error in main: " << e.what() << std::endl;
      return 1;
    }
  }

  std::cout << "Exiting game...";
  return 0;
}
