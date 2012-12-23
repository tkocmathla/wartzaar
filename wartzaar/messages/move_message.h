#ifndef WARTZAAR_MESSAGES_MOVE_MESSAGE_H_
#define WARTZAAR_MESSAGES_MOVE_MESSAGE_H_

#include "wartzaar/messages/game_message.h"

namespace wartzaar { namespace messages {

///-----------------------------------------------------------------------------
/// This class represents a "Move" message as defined by the Daedalus Game
/// Manager. It can be used to parse message strings from the server, or to
/// construct new outgoing messages.
///-----------------------------------------------------------------------------
class MoveMessage : public GameMessage {
 public:
  /// Constructor creates a "pass" move message.
  MoveMessage();

  /// Constructor creates a move message based on the given text.
  MoveMessage(const std::string &message);

  /// Constructor creates a move message based on the given coordinates.
  MoveMessage(int from_column, int from_row, int to_column, int to_row);

  void ParsePayload();
  void CreatePayload();
  void CreateText();

  bool pass() const;
  void set_pass(bool pass);

  int from_column() const;
  void set_from_column(int from_column);

  int from_row() const;
  void set_from_row(int from_row);

  int to_column() const;
  void set_to_column(int to_column);

  int to_row() const;
  void set_to_row(int to_row);

 private:
  bool pass_;
  int from_column_;
  int from_row_;
  int to_column_;
  int to_row_;
};

}} // namespace wartzaar::messages

#endif // WARTZAAR_MESSAGES_MOVE_MESSAGE_H_
