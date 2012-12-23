#ifndef WARTZAAR_MESSAGES_YOUR_PLAYER_NUMBER_MESSAGE_H_
#define WARTZAAR_MESSAGES_YOUR_PLAYER_NUMBER_MESSAGE_H_

#include "wartzaar/messages/game_message.h"
#include "wartzaar/types/player_number.h"

namespace wartzaar { namespace messages {

class YourPlayerNumberMessage : public GameMessage {
 public:
  YourPlayerNumberMessage(const std::string&);

  void ParsePayload();

  wartzaar::types::playernumber::PlayerNumber player_number();
  void set_player_number(wartzaar::types::playernumber::PlayerNumber);

 private:
  wartzaar::types::playernumber::PlayerNumber player_number_;
};

}} // namespace wartzaar::messages

#endif // WARTZAAR_MESSAGES_YOUR_PLAYER_NUMBER_MESSAGE_H_
