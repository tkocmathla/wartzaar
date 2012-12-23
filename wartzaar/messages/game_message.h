#ifndef WARTZAAR_MESSAGES_GAME_MESSAGE_H_
#define WARTZAAR_MESSAGES_GAME_MESSAGE_H_

#include <string>

namespace wartzaar { namespace messages {

class GameMessage {
 public:
  enum TypeCode {
    kBoardState,
    kChat,
    kControl,
    kGameOver,
    kMove,
    kVersion,
    kYourPlayerNumber,
    kYourTurn
  };

  GameMessage(const std::string&, TypeCode);
  ~GameMessage();

  std::string ExtractTypeName(const std::string&);
  std::string ExtractPayload(const std::string&);
  void ParsePayload();

  std::string text();
  void set_text(const std::string&);

  std::string type_name();
  void set_type_name(const std::string&);

  TypeCode type_code();
  void set_type_code(TypeCode);

  std::string payload();
  void set_payload(const std::string&);

 protected:
  std::string text_;
  std::string type_name_;
  TypeCode type_code_;
  std::string payload_;
};

}} // namespace wartzaar::messages

#endif // WARTZAAR_MESSAGES_GAME_MESSAGE_H_
