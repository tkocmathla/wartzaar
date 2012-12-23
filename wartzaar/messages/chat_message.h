#ifndef WARTZAAR_MESSAGES_CHAT_MESSAGE_H_
#define WARTZAAR_MESSAGES_CHAT_MESSAGE_H_

#include "wartzaar/messages/game_message.h"

namespace wartzaar { namespace messages {

class ChatMessage : public GameMessage {
 public:
  ChatMessage(const std::string&);
};

}} // namespace wartzaar::messages

#endif // WARTZAAR_MESSAGES_CHAT_MESSAGE_H_
