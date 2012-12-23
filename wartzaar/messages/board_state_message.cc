#include "wartzaar/messages/board_state_message.h"

#include <sstream>

namespace wartzaar { namespace messages {

BoardStateMessage::BoardStateMessage(const std::string &message)
    : GameMessage::GameMessage(message, GameMessage::kBoardState),
      board_state_(GameBoard()) {
  ParsePayload();
}

void BoardStateMessage::ParsePayload() {
  size_t pos_start = payload_.find('{') + 1;
  size_t pos_end   = payload_.find('}');
  int cell = 0;

  do {
    std::string item = payload_.substr(pos_start, pos_end - pos_start);
    if (item != "") {
      int stack_height = 1;
      std::string type;
      std::string top_type;
      std::string color = item.substr(0, item.find(','));
      std::stringstream ss(item.substr(item.find(',') + 1));

      // Get the top-of-stack piece
      getline(ss, top_type, ',');

      // Count up the remaining pieces
      while (getline(ss, type, ',')) ++stack_height;

      // Add the stack to the game board
      board_state_.AddPieces(GameBoardPosition::ParseColor(color),
          GameBoardPosition::ParseType(top_type), stack_height, cell);
    }

    pos_start = payload_.find('{', pos_start) + 1;
    pos_end   = payload_.find('}', pos_end + 1);

    ++cell;

  } while (pos_start != std::string::npos && pos_end != std::string::npos);
}

GameBoard BoardStateMessage::board_state() {
  return board_state_;
}

}} // namespace wartzaar
