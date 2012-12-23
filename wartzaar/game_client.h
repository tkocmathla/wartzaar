#ifndef WARTZAAR_GAME_CLIENT_H_
#define WARTZAAR_GAME_CLIENT_H_

#include <winsock.h>
#include <string>

#include "wartzaar/messages/board_state_message.h"
#include "wartzaar/messages/game_message.h"
#include "wartzaar/messages/your_turn_message.h"

namespace wartzaar {

///-----------------------------------------------------------------------------
/// The GameClient class handles communication with the Daedalus Game Manager.
///
/// TODO Add support for Linux sockets
///-----------------------------------------------------------------------------
class GameClient {
 public:
  /// Constructor initializes the Winsock DLL.
  ///
  /// @param string host address of the game manager
  /// @param int port on which the game manager is listening
  ///
  GameClient(const std::string &host, int port);

  /// Destructor calls Disconnect but suppresses exceptions.
  ~GameClient();

  /// Establishes a socket connection with the host.
  boolean Connect();

  /// Destroys the socket and cleans up Winsock.
  void Disconnect();

  ///
  void SendGameMessage(const wartzaar::messages::GameMessage &message);

  ///
  std::string ReceiveGameMessage();

  std::string game_version();
  void set_game_version(const std::string &game_version);

  std::string manager_version();
  void set_manager_version(const std::string &manager_version);

 private:
  void Init();

  std::string host_;
  int port_;
  SOCKET sock_;
  sockaddr_in target_;
  WSADATA wsadata_;
  std::string game_version_;
  std::string manager_version_;
};

} // namespace wartzaar

#endif // WARTZAAR_GAME_CLIENT_H_
