#ifndef WARTZAAR_GAME_BOARD_H_
#define WARTZAAR_GAME_BOARD_H_

#include <vector>
#include "wartzaar/game_board_position.h"
#include "wartzaar/types/color.h"
#include "wartzaar/types/piece_type.h"

namespace wartzaar {

///-----------------------------------------------------------------------------
///
///-----------------------------------------------------------------------------
class GameBoard {
 public:
  /// Constructor calls Init.
  GameBoard();

  /// Copy constructor calls Init and replaces each game board position on the
  /// existing board with the contents of the new board.
  GameBoard(const GameBoard &that);

  /// Assignment operator
  GameBoard& operator= (const GameBoard &that);

  /// Adds a single piece to the game board at the specified cell.
  void AddPiece(wartzaar::types::color::Color color,
      wartzaar::types::piecetype::PieceType type, int cell);

  /// Adds a single piece to the game board at the specified column and row.
  void AddPiece(wartzaar::types::color::Color color,
      wartzaar::types::piecetype::PieceType type, int col, int row);

  /// Adds a stack of pieces to the game board at the specified cell.
  void AddPieces(wartzaar::types::color::Color color,
      wartzaar::types::piecetype::PieceType type, int stack_height, int cell);

  /// Adds a stack of pieces to the game board at the specified column and row.
  void AddPieces(wartzaar::types::color::Color color,
      wartzaar::types::piecetype::PieceType type, int stack_height, int col,
      int row);

  GameBoardPosition* PositionAt(int col, int row) const;

  /// Calculates the cell number (0-59) for a given column and row.
  int CalculateCell(int col, int row) const;

  /// Calculates the column number for a given cell number (0-59).
  int CalculateCol(int cell) const;

  /// Calculates the row number for a given cell number (0-59).
  int CalculateRow(int cell) const;

  /// Returns a (const) iterator to the first element of the board vector.
  std::vector< std::vector<GameBoardPosition> >::iterator Begin();
  std::vector< std::vector<GameBoardPosition> >::const_iterator BeginConst() const;

  /// Returns a (const) iterator to the end of the board vector.
  std::vector< std::vector<GameBoardPosition> >::iterator End();
  std::vector< std::vector<GameBoardPosition> >::const_iterator EndConst() const;

  /// Returns true if the given game board is equal to this game board.
  bool Equals(const GameBoard &that) const;

 private:
  /// Initializes the board positions.
  void Init();

  /// Initializes the links between board positions.
  void InitLinks();

  /// A 2-dimensional vector of GameBoardPositions representing the Tzaar board.
  ///
  /// The second dimension varies in size based on the number of positions in
  /// that column on the board (a jagged vector).
  ///
  std::vector< std::vector<GameBoardPosition> > board_;
};

} // namespace wartzaar

#endif // WARTZAAR_GAME_BOARD_H_
