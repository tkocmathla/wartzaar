#ifndef WARTZAAR_MESSAGES_VERSION_MESSAGE_H_
#define WARTZAAR_MESSAGES_VERSION_MESSAGE_H_

#include "wartzaar/messages/game_message.h"

namespace wartzaar { namespace messages {

class VersionMessage : public GameMessage {
 public:
  VersionMessage(const std::string&);

  void ParsePayload();

  std::string manager_version();
  void set_manager_version(const std::string&);

  std::string game_version();
  void set_game_version(const std::string&);

 private:
  std::string manager_version_;
  std::string game_version_;
};

}} // namespace wartzaar::messages

#endif // WARTZAAR_MESSAGES_VERSION_MESSAGE_H_
