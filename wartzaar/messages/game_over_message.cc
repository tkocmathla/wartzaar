#include "wartzaar/messages/game_over_message.h"

namespace wartzaar { namespace messages {

GameOverMessage::GameOverMessage(const std::string &message)
    : GameMessage(message, GameMessage::kYourPlayerNumber) {
  ParsePayload();
}

void GameOverMessage::ParsePayload() {
  if (payload_ == "None") {
    condition_ = GameOverMessage::kNone;
  }
  else if (payload_ == "YouWin") {
    condition_ = GameOverMessage::kYouWin;
  }
  else if (payload_ == "YouLose") {
    condition_ = GameOverMessage::kYouLose;
  }
  else if (payload_ == "OtherPlayerForfeits") {
    condition_ = GameOverMessage::kOtherPlayerForfeits;
  }
  else if (payload_ == "Draw") {
    condition_ = GameOverMessage::kDraw;
  }
}

GameOverMessage::Condition GameOverMessage::condition() {
  return condition_;
}

void GameOverMessage::set_condition(Condition condition) {
  condition_ = condition;
}

}} // namespace wartzaar::messages
