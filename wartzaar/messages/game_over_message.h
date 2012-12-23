#ifndef WARTZAAR_MESSAGES_GAME_OVER_MESSAGE_H_
#define WARTZAAR_MESSAGES_GAME_OVER_MESSAGE_H_

#include "wartzaar/messages/game_message.h"

namespace wartzaar { namespace messages {

class GameOverMessage : public GameMessage {
 public:
  enum Condition {
    kNone,
    kYouWin,
    kYouLose,
    kOtherPlayerForfeits,
    kDraw
  };

  GameOverMessage(const std::string&);

  void ParsePayload();

  Condition condition();
  void set_condition(Condition);

 private:
  Condition condition_;
};

}} // namespace wartzaar::messages

#endif // WARTZAAR_MESSAGES_GAME_OVER_MESSAGE_H_
