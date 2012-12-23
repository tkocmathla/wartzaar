#ifndef WARTZAAR_MESSAGES_BOARD_STATE_MESSAGE_H_
#define WARTZAAR_MESSAGES_BOARD_STATE_MESSAGE_H_

#include <vector>
#include "wartzaar/game_board.h"
#include "wartzaar/messages/game_message.h"

namespace wartzaar { namespace messages {

class BoardStateMessage : public GameMessage {
 public:
  BoardStateMessage(const std::string&);

  void ParsePayload();
  GameBoard board_state();

 private:
  GameBoard board_state_;
};

}} // namespace wartzaar::messages

#endif // WARTZAAR_MESSAGES_BOARD_STATE_MESSAGE_H_
