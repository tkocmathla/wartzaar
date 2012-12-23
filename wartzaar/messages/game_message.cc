#include "wartzaar/messages/game_message.h"

#include <iostream>
#include <stdexcept>

namespace wartzaar { namespace messages {

GameMessage::GameMessage(const std::string &text, TypeCode type_code)
    : text_(text),
      type_code_(type_code) {
  type_name_ = ExtractTypeName(text_);
  payload_ = ExtractPayload(text_);

  ParsePayload();
}

GameMessage::~GameMessage() {}

std::string GameMessage::ExtractTypeName(const std::string &text) {
  size_t pos = text.find('{');

  if (pos == std::string::npos)
    throw std::runtime_error("Can't extract message type name: invalid message format.");

  return text.substr(0, pos);
}

std::string GameMessage::ExtractPayload(const std::string &text) {
  if (text.length() == 0) return std::string();

  size_t pos_start = text.find('{') + 1;
  size_t pos_end = text.rfind('}');

  if (pos_start == std::string::npos || pos_end == std::string::npos)
    throw std::runtime_error("Can't extract message payload: invalid message format.");

  return text.substr(pos_start, pos_end - pos_start);
}

void GameMessage::ParsePayload() {}

std::string GameMessage::text() {
  return text_;
}

void GameMessage::set_text(const std::string &text) {
  text_ = text;
}

std::string GameMessage::type_name() {
  return type_name_;
}

void GameMessage::set_type_name(const std::string &type_name) {
  type_name_ = type_name;
}

GameMessage::TypeCode GameMessage::type_code() {
  return type_code_;
}

void GameMessage::set_type_code(TypeCode type_code) {
  type_code_ = type_code;
}

std::string GameMessage::payload() {
  return payload_;
}

void GameMessage::set_payload(const std::string &data) {
  payload_ = data;
}

}} // namespace wartzaar::messages
