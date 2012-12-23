#ifndef WARTZAAR_MESSAGES_YOUR_TURN_MESSAGE_H_
#define WARTZAAR_MESSAGES_YOUR_TURN_MESSAGE_H_

#include "wartzaar/messages/game_message.h"

namespace wartzaar { namespace messages {

class YourTurnMessage : public GameMessage {
 public:
  YourTurnMessage(const std::string&);
};

}} // namespace wartzaar::messages

#endif // WARTZAAR_MESSAGES_YOUR_TURN_MESSAGE_H_
