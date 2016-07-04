#include "wartzaar/game_state.h"

#include <iostream>
#include <limits>
#include <sstream>

namespace wtc = wartzaar::types::color;
namespace wtpt = wartzaar::types::piecetype;

namespace wartzaar {

GameState::GameState(const GameBoard &board)
    : board_(board),
      heuristic_value_(-std::numeric_limits<float>::max()),
      last_move_from_(0),
      last_move_to_(0) {
  Init();
}

GameState::GameState(const GameState &that)
    : board_(that.board_),
      heuristic_value_(that.heuristic_value_),
      last_move_from_(0),
      last_move_to_(0),
      stack_height_(that.stack_height_),
      piece_count_(that.piece_count_) {
  if (that.last_move_from_ != 0) {
    last_move_from_ = board_.PositionAt(
      that.last_move_from_->col(),
      that.last_move_from_->row()
    );
  }

  if (that.last_move_to_ != 0) {
    last_move_to_ = board_.PositionAt(
      that.last_move_to_->col(),
      that.last_move_to_->row()
    );
  }
}

GameState &GameState::operator=(const GameState &that) {
  board_ = that.board_;
  heuristic_value_ = that.heuristic_value_;
  stack_height_ = that.stack_height_;
  piece_count_ = that.piece_count_;

  if (that.last_move_from_ != 0) {
    last_move_from_ = board_.PositionAt(
      that.last_move_from_->col(),
      that.last_move_from_->row()
    );
  }

  if (that.last_move_to_ != 0) {
    last_move_to_ = board_.PositionAt(
      that.last_move_to_->col(),
      that.last_move_to_->row()
    );
  }

  return *this;
}

bool GameState::operator==(const GameState &that) const {
  return board_.Equals(that.board_);
}

bool GameState::operator<(const GameState &that) const {
  return heuristic_value_ < that.heuristic_value_;
}

bool GameState::operator>(const GameState &that) const {
  return heuristic_value_ > that.heuristic_value_;
}

void GameState::Init() {
  piece_count_[wtc::kWhite][wtpt::kTzaar]  = 0;
  piece_count_[wtc::kWhite][wtpt::kTzarra] = 0;
  piece_count_[wtc::kWhite][wtpt::kTott]   = 0;
  piece_count_[wtc::kBlack][wtpt::kTzaar]  = 0;
  piece_count_[wtc::kBlack][wtpt::kTzarra] = 0;
  piece_count_[wtc::kBlack][wtpt::kTott]   = 0;

  stack_height_[wtc::kWhite][wtpt::kTzaar]  = 0;
  stack_height_[wtc::kWhite][wtpt::kTzarra] = 0;
  stack_height_[wtc::kWhite][wtpt::kTott]   = 0;
  stack_height_[wtc::kBlack][wtpt::kTzaar]  = 0;
  stack_height_[wtc::kBlack][wtpt::kTzarra] = 0;
  stack_height_[wtc::kBlack][wtpt::kTott]   = 0;

  std::vector< std::vector<GameBoardPosition> >::iterator col_itr;
  for (col_itr = board_.Begin(); col_itr != board_.End(); ++col_itr) {
    std::vector<GameBoardPosition>::iterator row_itr;
    for (row_itr = col_itr->begin(); row_itr != col_itr->end(); ++row_itr) {
      if (row_itr->stack_height() == 0)
        continue;

      wtc::Color color = *row_itr->color();
      wtpt::PieceType type = *row_itr->type();

      piece_count_ [color][type]++;
      stack_height_[color][type] += row_itr->stack_height() - 1;
    }
  }
}

void GameState::MakeMove(int from_column, int from_row, int to_column, int to_row) {
  GameBoardPosition *from_ptr = board_.PositionAt(from_column, from_row);
  GameBoardPosition *to_ptr   = board_.PositionAt(to_column, to_row);

  wtc::Color from_color = *from_ptr->color();
  wtc::Color to_color = *to_ptr->color();

  wtpt::PieceType to_type = *to_ptr->type();
  wtpt::PieceType from_type = *from_ptr->type();

  int to_height = to_ptr->stack_height();
  int from_height = from_ptr->stack_height();

  // Make a stacking move if the "from" and "to" colors are equal
  if (from_color == to_color) {
    board_.AddPieces(*from_ptr->color(), *from_ptr->type(),
        from_ptr->stack_height(), to_column, to_row);
    from_ptr->ClearPieces();

    // Adjust the stack height counters
    stack_height_[from_color][from_type]
        += (board_.PositionAt(to_column, to_row)->stack_height() - 1) // combined height
         - (from_height - 1);                                         // source height

    stack_height_[to_color][to_type] -= (to_height - 1);
  }
  // Otherwise, make a capturing move
  else {
    // Adjust the stack height counter
    stack_height_[to_color][to_type] -= (to_height - 1);

    to_ptr->ClearPieces();
    to_ptr->AddPieces(*from_ptr->color(), *from_ptr->type(), from_ptr->stack_height());
    from_ptr->ClearPieces();
  }

  // Adjust the piece counters
  piece_count_[to_color][to_type]--;

  // Update the last move pointers
  last_move_from_ = from_ptr;
  last_move_to_ = to_ptr;
}

int GameState::GetStackHeight(const wtc::Color &color, const wtpt::PieceType &type) const {
  return stack_height_.find(color)->second.find(type)->second;
}

int GameState::GetPieceCount(const wtc::Color &color, const wtpt::PieceType &type) const {
  return piece_count_.find(color)->second.find(type)->second;
}

std::vector< std::vector<GameBoardPosition> >::iterator GameState::Begin() {
  return board_.Begin();
}

std::vector< std::vector<GameBoardPosition> >::const_iterator GameState::BeginConst() const {
  return board_.BeginConst();
}

std::vector< std::vector<GameBoardPosition> >::iterator GameState::End() {
  return board_.End();
}

std::vector< std::vector<GameBoardPosition> >::const_iterator GameState::EndConst() const {
  return board_.EndConst();
}

bool GameState::Equals(const GameState &that) {
  return board_.Equals(that.board_);
}

std::string GameState::ToString() {
  if (last_move_from_ == 0 || last_move_to_ == 0)
    return std::string();

  std::stringstream s;
  s << last_move_from_->col() << "," << last_move_from_->row()
    << " -> "
    << last_move_to_->col()   << "," << last_move_to_->row();

  return s.str();
}

float GameState::heuristic_value() const {
  return heuristic_value_;
}

void GameState::set_heuristic_value(float heuristic_value) {
  heuristic_value_ = heuristic_value;
}

GameBoardPosition* GameState::last_move_from() {
  return last_move_from_;
}

void GameState::set_last_move_from(const GameBoardPosition &position) {
  last_move_from_ = const_cast<GameBoardPosition*>(&position);
}

GameBoardPosition* GameState::last_move_to() {
  return last_move_to_;
}

void GameState::set_last_move_to(const GameBoardPosition &position) {
  last_move_to_ = const_cast<GameBoardPosition*>(&position);
}

} // namespace wartzaar
