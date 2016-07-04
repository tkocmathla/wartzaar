#include "wartzaar/messages/your_player_number_message.h"
#include "wartzaar/types/player_number.h"

namespace wtpn = wartzaar::types::playernumber;

namespace wartzaar { namespace messages {

YourPlayerNumberMessage::YourPlayerNumberMessage(const std::string &message)
    : GameMessage(message, GameMessage::kYourPlayerNumber) {
  ParsePayload();
}

void YourPlayerNumberMessage::ParsePayload() {
  if (payload_ == "One") {
    player_number_ = wtpn::kPlayerOne;
  }
  else if (payload_ == "Two") {
    player_number_ = wtpn::kPlayerTwo;
  }
}

wtpn::PlayerNumber YourPlayerNumberMessage::player_number() {
  return player_number_;
}

void YourPlayerNumberMessage::set_player_number(wtpn::PlayerNumber player_number) {
  player_number_ = player_number;
}

}} // namespace wartzaar::messages
