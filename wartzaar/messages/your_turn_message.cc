#include "wartzaar/messages/your_turn_message.h"

namespace wartzaar { namespace messages {

YourTurnMessage::YourTurnMessage(const std::string &message)
    : GameMessage(message, GameMessage::kYourTurn) {}

}} // namespace wartzaar::messages
