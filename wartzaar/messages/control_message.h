#ifndef WARTZAAR_MESSAGES_CONTROL_MESSAGE_H_
#define WARTZAAR_MESSAGES_CONTROL_MESSAGE_H_

#include "wartzaar/messages/game_message.h"

namespace wartzaar { namespace messages {

class ControlMessage : public GameMessage {
 public:
  ControlMessage(const std::string&);
};

}} // namespace wartzaar::messages

#endif // WARTZAAR_MESSAGES_CONTROL_MESSAGE_H_
