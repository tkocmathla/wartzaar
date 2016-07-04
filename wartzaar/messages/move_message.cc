#include "wartzaar/messages/move_message.h"

#include <sstream>

namespace wartzaar { namespace messages {

MoveMessage::MoveMessage()
    : GameMessage("Move{}", GameMessage::kMove),
      pass_(true),
      from_column_(-1),
      from_row_(-1),
      to_column_(-1),
      to_row_(-1) {}

MoveMessage::MoveMessage(const std::string &message)
    : GameMessage(message, GameMessage::kMove),
      pass_(false),
      from_column_(-1),
      from_row_(-1),
      to_column_(-1),
      to_row_(-1) {
  ParsePayload();
}

MoveMessage::MoveMessage(int from_column, int from_row, int to_column, int to_row)
    : GameMessage("Move{}", GameMessage::kMove),
      pass_(false),
      from_column_(from_column),
      from_row_(from_row),
      to_column_(to_column),
      to_row_(to_row) {
  CreateText();
}

// Extract the character for each move coordinate and convert it to an int.
//
void MoveMessage::ParsePayload() {
  std::stringstream ss(payload_);

  ss >> from_column_;
  ss.ignore(1, ',');

  ss >> from_row_;
  ss.ignore(1, ',');

  ss >> to_column_;
  ss.ignore(1, ',');

  ss >> to_row_;
}

void MoveMessage::CreatePayload() {
  if (pass_) {
    payload_ = std::string();
  }
  else {
    std::stringstream ss;
    ss << from_column_ << ","
       << from_row_    << ","
       << to_column_   << ","
       << to_row_;

    payload_ = ss.str();
  }
}

void MoveMessage::CreateText() {
  CreatePayload();
  text_ = "Move{" + payload_ + "}\r\n";
}

int MoveMessage::from_column() const {
  return from_column_;
}

void MoveMessage::set_from_column(int from_column) {
  from_column_ = from_column;
}

int MoveMessage::from_row() const {
  return from_row_;
}

void MoveMessage::set_from_row(int from_row) {
  from_row_ = from_row;
}

int MoveMessage::to_column() const {
  return to_column_;
}

void MoveMessage::set_to_column(int to_column) {
  to_column_ = to_column;
}

int MoveMessage::to_row() const {
  return to_row_;
}

void MoveMessage::set_to_row(int to_row) {
  to_row_ = to_row;
}

}} // namespace wartzaar::messages
