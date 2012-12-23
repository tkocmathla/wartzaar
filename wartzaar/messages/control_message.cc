#include "wartzaar/messages/control_message.h"

namespace wartzaar { namespace messages {

ControlMessage::ControlMessage(const std::string &message)
    : GameMessage::GameMessage(message, GameMessage::kControl) {}

}} // namespace wartzaar::messages
