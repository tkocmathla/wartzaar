#include "wartzaar/game_board.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "wartzaar/game_board_position.h"

namespace wtc = wartzaar::types::color;

namespace wartzaar {

GameBoard::GameBoard() {
  Init();
}

/// Copy constructor copies the stack of pieces from each cell in the source
/// board to the same cell in the new board.
///
GameBoard::GameBoard(const GameBoard &that) {
  Init();

  for (size_t col = 0; col < board_.size(); ++col) {
    for (size_t row = 0; row < board_[col].size(); ++row) {
      board_[col][row].set_stack_height(that.board_[col][row].stack_height());
      board_[col][row].set_color(that.board_[col][row].color());
      board_[col][row].set_type(that.board_[col][row].type());
    }
  }
}

/// Assignment operator copies the stack of pieces from each cell in the source
/// board to the same cell in the new board.
///
GameBoard &GameBoard::operator=(const GameBoard &that) {
  for (size_t col = 0; col < board_.size(); ++col) {
    for (size_t row = 0; row < board_[col].size(); ++row) {
      board_[col][row].set_stack_height(that.board_[col][row].stack_height());
      board_[col][row].set_color(that.board_[col][row].color());
      board_[col][row].set_type(that.board_[col][row].type());
    }
  }

  return *this;
}

void GameBoard::Init() {
  board_.clear();
  board_.push_back(std::vector<GameBoardPosition>(5));
  board_.push_back(std::vector<GameBoardPosition>(6));
  board_.push_back(std::vector<GameBoardPosition>(7));
  board_.push_back(std::vector<GameBoardPosition>(8));
  board_.push_back(std::vector<GameBoardPosition>(8));
  board_.push_back(std::vector<GameBoardPosition>(8));
  board_.push_back(std::vector<GameBoardPosition>(7));
  board_.push_back(std::vector<GameBoardPosition>(6));
  board_.push_back(std::vector<GameBoardPosition>(5));

  for (size_t col = 0; col < board_.size(); ++col) {
    for (size_t row = 0; row < board_[col].size(); ++row) {
      board_[col][row].set_col(col);
      board_[col][row].set_row(row);
    }
  }

  InitLinks();
}

void GameBoard::InitLinks() {
  for (size_t col = 0; col < board_.size(); ++col) {
    for (size_t row = 0; row < board_[col].size(); ++row) {
      // Set up north links.
      //
      // A position has a north link UNLESS it is:
      //  - the top piece of a column (row == max)
      //  - directly south of the center position
      //
      if (row < board_[col].size() - 1 && !(col == 4 && row == 3))
        board_[col][row].n(&board_[col][row + 1]);

      // Set up northeast links.
      //
      // A position has a northeast link UNLESS it is:
      //  - the top piece of a center or right-hand-side column (row == max)
      //  - in the right-most column
      //  - directly southwest of the center position
      //
      if (!(col > 3 && row == board_[col].size() - 1) &&
          !(col == 8) &&
          !(col == 3 && row == 3)) {
        if (col < 3 || (col == 3 && row < 3) || (col == 4 && row > 3))
          board_[col][row].ne(&board_[col + 1][row + 1]);
        else
          board_[col][row].ne(&board_[col + 1][row]);
      }

      // Set up southeast links.
      //
      // A position has a southeast link UNLESS it is:
      //  - the bottom piece of a center or right-hand-side column (row == 0)
      //  - in the right-most column
      //  - directly northwest of the center position
      //
      if (!(col > 3 && row == 0) &&
          !(col == 8) &&
          !(col == 3 && row == 4)) {
        if (col > 4 || (col == 3 && row > 4) || (col == 4 && row < 4))
          board_[col][row].se(&board_[col + 1][row - 1]);
        else
          board_[col][row].se(&board_[col + 1][row]);
      }

      // Set up south links.
      //
      // A position has a south link UNLESS it is:
      //  - the bottom piece of a column (row == 0)
      //  - directly north of the center position
      //
      if (row > 0 && !(col == 4 && row == 4))
        board_[col][row].s(&board_[col][row - 1]);

      // Set up southwest links.
      //
      // A position has a southwest link UNLESS it is:
      //  - the bottom piece of a center or left-hand-side column (row == 0)
      //  - in the left-most column
      //  - directly northeast of the center position
      //
      if (!(col < 5 && row == 0) &&
          !(col == 0) &&
          !(col == 5 && row == 4)) {
        if (col < 4 || (col == 4 && row < 4) || (col == 5 && row > 4))
          board_[col][row].sw(&board_[col - 1][row - 1]);
        else
          board_[col][row].sw(&board_[col - 1][row]);
      }

      // Set up northwest links.
      //
      // A position has a northwest link UNLESS it is:
      //  - the top piece of a center or left-hand-side column (row == max)
      //  - in the left-most column
      //  - directly southeast of the center position
      //
      if (!(col < 5 && row == board_[col].size() - 1) &&
          !(col == 0) &&
          !(col == 5 && row == 3)) {
        if (col > 5 || (col == 5 && row < 3) || (col == 4 && row > 3))
          board_[col][row].nw(&board_[col - 1][row + 1]);
        else
          board_[col][row].nw(&board_[col - 1][row]);
      }
    }
  }
}

void GameBoard::AddPiece(wartzaar::types::color::Color color,
    wartzaar::types::piecetype::PieceType type, int cell) {
  board_[CalculateCol(cell)][CalculateRow(cell)].AddPieces(color, type, 1);
}

void GameBoard::AddPieces(wartzaar::types::color::Color color,
    wartzaar::types::piecetype::PieceType type, int stack_height, int cell) {
  board_[CalculateCol(cell)][CalculateRow(cell)].AddPieces(color, type, stack_height);
}

void GameBoard::AddPiece(wartzaar::types::color::Color color,
    wartzaar::types::piecetype::PieceType type, int col, int row) {
  board_[col][row].AddPieces(color, type, 1);
}

void GameBoard::AddPieces(wartzaar::types::color::Color color,
    wartzaar::types::piecetype::PieceType type, int stack_height, int col, int row) {
  board_[col][row].AddPieces(color, type, stack_height);
}

GameBoardPosition* GameBoard::PositionAt(int col, int row) const {
  return const_cast<GameBoardPosition*>(&board_[col][row]);
}

int GameBoard::CalculateCell(int col, int row) const {
  int cell;

  switch (col) {
    case 0:
      cell = row;
      break;
    case 1:
      cell = row + 5;
      break;
    case 2:
      cell = row + 11;
      break;
    case 3:
      cell = row + 18;
      break;
    case 4:
      cell = row + 26;
      break;
    case 5:
      cell = row + 36;
      break;
    case 6:
      cell = row + 42;
      break;
    case 7:
      cell = row + 49;
      break;
    case 8:
      cell = row + 55;
      break;
    default:
      throw std::runtime_error("Invalid column value");
      break;
  }

  return cell;
}

int GameBoard::CalculateCol(int cell) const {
  int column;

  if (cell < 5)
    column = 0;
  else if (cell < 11)
    column = 1;
  else if (cell < 18)
    column = 2;
  else if (cell < 26)
    column = 3;
  else if (cell < 34)
    column = 4;
  else if (cell < 42)
    column = 5;
  else if (cell < 49)
    column = 6;
  else if (cell < 55)
    column = 7;
  else if (cell < 60)
    column = 8;
  else
    throw std::runtime_error("Cell number must be between 0 and 59");

  return column;
}

int GameBoard::CalculateRow(int cell) const {
  int row;
  int col = CalculateCol(cell);

  switch (col) {
    case 0:
      row = cell;
      break;
    case 1:
      row = cell - 5;
      break;
    case 2:
      row = cell - 11;
      break;
    case 3:
      row = cell - 18;
      break;
    case 4:
      row = cell - 26;
      break;
    case 5:
      row = cell - 34;
      break;
    case 6:
      row = cell - 42;
      break;
    case 7:
      row = cell - 49;
      break;
    case 8:
      row = cell - 55;
      break;
    default:
      throw std::runtime_error("Cell number must be between 0 and 59");
      break;
  }

  return row;
}

std::vector< std::vector<GameBoardPosition> >::iterator GameBoard::Begin() {
  return board_.begin();
}

std::vector< std::vector<GameBoardPosition> >::const_iterator GameBoard::BeginConst() const {
  return board_.begin();
}

std::vector< std::vector<GameBoardPosition> >::iterator GameBoard::End() {
  return board_.end();
}

std::vector< std::vector<GameBoardPosition> >::const_iterator GameBoard::EndConst() const {
  return board_.end();
}

bool GameBoard::Equals(const GameBoard &that) const {
  for (size_t col = 0; col < board_.size(); ++col)
    for (size_t row = 0; row < board_[col].size(); ++row)
      if (!board_[col][row].Equals(that.board_[col][row]))
        return false;

  return true;
}

} // namespace wartzaar
