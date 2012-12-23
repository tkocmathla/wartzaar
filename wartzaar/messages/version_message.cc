#include "wartzaar/messages/version_message.h"

#include <sstream>
#include <vector>

namespace wartzaar { namespace messages {

VersionMessage::VersionMessage(const std::string &message)
    : GameMessage::GameMessage(message, GameMessage::kVersion) {
  ParsePayload();
}

// Splits the payload on semicolon and stores the versions for the manager and
// game in member variables.
//
void VersionMessage::ParsePayload() {
  std::vector<std::string> elements;
  std::stringstream ss(payload_);
  std::string item;

  while (std::getline(ss, item, ';'))
    elements.push_back(item);

  manager_version_ = elements[0];
  game_version_ = elements[2];
}

std::string VersionMessage::manager_version() {
  return manager_version_;
}

void VersionMessage::set_manager_version(const std::string &manager_version) {
  manager_version_ = manager_version;
}

std::string VersionMessage::game_version() {
  return game_version_;
}

void VersionMessage::set_game_version(const std::string &game_version) {
  game_version_ = game_version;
}

}} // namespace wartzaar::messages
