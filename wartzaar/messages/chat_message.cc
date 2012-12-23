#include "wartzaar/messages/chat_message.h"

namespace wartzaar { namespace messages {

ChatMessage::ChatMessage(const std::string &message)
    : GameMessage::GameMessage(message, GameMessage::kChat) {}

}} // namespace wartzaar::messages
