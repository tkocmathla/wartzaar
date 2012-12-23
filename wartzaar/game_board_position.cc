#include "wartzaar/game_board_position.h"

#include <stdexcept>

namespace wtd = wartzaar::types::direction;
namespace wtc = wartzaar::types::color;
namespace wtpt = wartzaar::types::piecetype;

namespace wartzaar {

GameBoardPosition::GameBoardPosition()
    : col_(),
      row_(),
      stack_height_(0),
      color_(0),
      type_(0),
      n_(0),
      ne_(0),
      se_(0),
      s_(0),
      sw_(0),
      nw_(0) {}

GameBoardPosition::GameBoardPosition(int col, int row)
    : col_(col),
      row_(row),
      stack_height_(0),
      color_(0),
      type_(0),
      n_(0),
      ne_(0),
      se_(0),
      s_(0),
      sw_(0),
      nw_(0) {}

GameBoardPosition::GameBoardPosition(int col, int row, wtc::Color color,
    wtpt::PieceType type)
    : col_(col),
      row_(row),
      stack_height_(1),
      color_(new wtc::Color(color)),
      type_(new wtpt::PieceType(type)),
      n_(0),
      ne_(0),
      se_(0),
      s_(0),
      sw_(0),
      nw_(0) {}

GameBoardPosition::GameBoardPosition(int col, int row, wartzaar::types::color::Color color,
    wartzaar::types::piecetype::PieceType type, int stack_height)
    : col_(col),
      row_(row),
      stack_height_(stack_height),
      color_(new wtc::Color(color)),
      type_(new wtpt::PieceType(type)),
      n_(0),
      ne_(0),
      se_(0),
      s_(0),
      sw_(0),
      nw_(0) {}

GameBoardPosition::GameBoardPosition(const GameBoardPosition &that)
    : col_(that.col_),
      row_(that.row_),
      stack_height_(that.stack_height_),
      color_(0),
      type_(0),
      n_(that.n_),
      ne_(that.ne_),
      se_(that.se_),
      s_(that.s_),
      sw_(that.sw_),
      nw_(that.nw_) {
  if (that.color_ != 0)
    color_ = new wtc::Color(*(that.color_));
  if (that.type_ != 0)
    type_ = new wtpt::PieceType(*(that.type_));
}

GameBoardPosition::~GameBoardPosition() {
  ClearPieces();
}

void GameBoardPosition::AddPieces(wartzaar::types::color::Color color,
    wartzaar::types::piecetype::PieceType type, int stack_height) {
  int to_height = stack_height_;
  ClearPieces();
  color_ = new wtc::Color(color);
  type_ = new wtpt::PieceType(type);
  stack_height_ = to_height + stack_height;
}

void GameBoardPosition::ClearPieces() {
  if (color_ != 0) {
    delete color_;
    color_ = 0;
  }
  if (type_ != 0) {
    delete type_;
    type_ = 0;
  }

  stack_height_ = 0;
}

/// Returns a null pointer if no path to another stack exists in the given
/// direction.
///
GameBoardPosition* GameBoardPosition::SearchPath(const wtd::Direction &dir) const {
  GameBoardPosition *pos = DirLink(dir);

  while (pos != 0) {
    // Skip empty positions
    if (pos->stack_height() == 0) {
      pos = pos->DirLink(dir);
      continue;
    }

    return pos;
  }

  return 0;
}

GameBoardPosition* GameBoardPosition::DirLink(const wtd::Direction &dir) const {
  switch (dir) {
    case wtd::kNorth:
      return n_;
      break;
    case wtd::kNortheast:
      return ne_;
      break;
    case wtd::kSoutheast:
      return se_;
      break;
    case wtd::kSouth:
      return s_;
      break;
    case wtd::kSouthwest:
      return sw_;
      break;
    case wtd::kNorthwest:
      return nw_;
      break;
  }

  return 0;
}

bool GameBoardPosition::Equals(const GameBoardPosition &that) const {
  if (col_ == that.col_ && row_ == that.row_ && stack_height_ == that.stack_height_)
    if ((color_ == 0 && type_ == 0 && that.color_ == 0 && that.type_ == 0)
        || (*color_ == *(that.color_) && *type_ == *(that.type_)))
      return true;

  return false;
}

wtc::Color GameBoardPosition::ParseColor(const std::string &color_string) {
  wtc::Color color;

  if (color_string == "WHITE")
    color = wtc::kWhite;
  else if (color_string == "BLACK")
    color = wtc::kBlack;
  else
    throw std::runtime_error("Unknown piece color: " + color_string);

  return color;
}

wtpt::PieceType GameBoardPosition::ParseType(const std::string &type_string) {
  wtpt::PieceType type;

  if (type_string == "Tzaar") {
    type = wtpt::kTzaar;
  }
  else if (type_string == "Tzarra") {
    type = wtpt::kTzarra;
  }
  else if (type_string == "Tott") {
    type = wtpt::kTott;
  }
  else {
    throw std::runtime_error("Unknown piece type: " + type_string);
  }

  return type;
}

std::string GameBoardPosition::ColorString() {
  std::string color_string;
  if (color_ == 0) return color_string;

  switch (*color_) {
    case wtc::kWhite:
      color_string = "White";
      break;
    case wtc::kBlack:
      color_string = "Black";
      break;
  }

  return color_string;
}

std::string GameBoardPosition::TypeString() {
  std::string type_string;
  if (type_ == 0) return type_string;

  switch (*type_) {
    case wtpt::kTott:
      type_string = "Tott";
      break;
    case wtpt::kTzarra:
      type_string = "Tzarra";
      break;
    case wtpt::kTzaar:
      type_string = "Tzaar";
      break;
  }

  return type_string;
}

int GameBoardPosition::col() const {
  return col_;
}

void GameBoardPosition::set_col(int col) {
  col_ = col;
}

void GameBoardPosition::set_row(int row) {
  row_ = row;
}

int GameBoardPosition::row() const {
  return row_;
}

int GameBoardPosition::stack_height() const {
  return stack_height_;
}

void GameBoardPosition::set_stack_height(int stack_height) {
  stack_height_ = stack_height;
}

wtc::Color* GameBoardPosition::color() const {
  return color_;
}

void GameBoardPosition::set_color(wtc::Color *color) {
  delete color_;
  color_ = 0;
  if (color != 0)
    color_ = new wtc::Color(*color);
}

wtpt::PieceType* GameBoardPosition::type() const {
  return type_;
}

void GameBoardPosition::set_type(wtpt::PieceType *type) {
  delete type_;
  type_ = 0;
  if (type != 0)
    type_ = new wtpt::PieceType(*type);
}

GameBoardPosition* GameBoardPosition::n() {
  return n_;
}

void GameBoardPosition::n(const GameBoardPosition *n) {
  n_ = const_cast<GameBoardPosition*>(n);
}

GameBoardPosition* GameBoardPosition::ne() {
  return ne_;
}

void GameBoardPosition::ne(const GameBoardPosition *ne) {
  ne_ = const_cast<GameBoardPosition*>(ne);
}

GameBoardPosition* GameBoardPosition::se() {
  return se_;
}


void GameBoardPosition::se(const GameBoardPosition *se) {
  se_ = const_cast<GameBoardPosition*>(se);
}

GameBoardPosition* GameBoardPosition::s() {
  return s_;
}

void GameBoardPosition::s(const GameBoardPosition *s) {
  s_ = const_cast<GameBoardPosition*>(s);
}

GameBoardPosition* GameBoardPosition::sw() {
  return sw_;
}

void GameBoardPosition::sw(const GameBoardPosition *sw) {
  sw_ = const_cast<GameBoardPosition*>(sw);
}

GameBoardPosition* GameBoardPosition::nw() {
  return nw_;
}

void GameBoardPosition::nw(const GameBoardPosition *nw) {
  nw_ = const_cast<GameBoardPosition*>(nw);
}

} // namespace wartzaar
