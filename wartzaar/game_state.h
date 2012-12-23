#ifndef WARTZAAR_GAME_STATE_H_
#define WARTZAAR_GAME_STATE_H_

#include <map>
#include <string>
#include <vector>

#include "wartzaar/game_board.h"
#include "wartzaar/game_state.h"

namespace wartzaar {

///-----------------------------------------------------------------------------
///
///-----------------------------------------------------------------------------
class GameState {
 public:
  GameState(const GameBoard &board);

  /// Copy constructor
  GameState(const GameState &that);

  /// Assignment operator
  GameState& operator=(const GameState &that);

  /// Equals operator
  bool operator==(const GameState &that) const;

  /// Less than operator
  bool operator<(const GameState &that) const;

  /// Greater than operator
  bool operator>(const GameState &that) const;

  void MakeMove(int from_column, int from_row, int to_column, int to_row);

  int GetStackHeight(const wartzaar::types::color::Color &color,
      const wartzaar::types::piecetype::PieceType &type) const;

  int GetPieceCount(const wartzaar::types::color::Color &color,
      const wartzaar::types::piecetype::PieceType &type) const;

  /// Returns a (const) iterator to the first element of the board vector.
  std::vector< std::vector<GameBoardPosition> >::iterator Begin();
  std::vector< std::vector<GameBoardPosition> >::const_iterator BeginConst() const;

  /// Returns a (const) iterator to the end of the board vector.
  std::vector< std::vector<GameBoardPosition> >::iterator End();
  std::vector< std::vector<GameBoardPosition> >::const_iterator EndConst() const;

  /// Returns true if the given game state is equal to this game state.
  ///
  /// Two states are considered equal if the underlying game board is the same.
  /// That is, all pieces are identical in position and stack height.
  ///
  bool Equals(const GameState &that);

  /// Prints the move that generated this state.
  std::string ToString();

  float heuristic_value() const;
  void set_heuristic_value(float heuristic_value);

  GameBoardPosition* last_move_from();
  void set_last_move_from(const GameBoardPosition &last_move_from);

  GameBoardPosition* last_move_to();
  void set_last_move_to(const GameBoardPosition &last_move_to);

 private:
  void Init();

  /// The game board representing the game state.
  GameBoard board_;

  /// The heuristic estimate of this state.
  float heuristic_value_;

  GameBoardPosition *last_move_from_;
  GameBoardPosition *last_move_to_;

  /// The adjusted stack height for each piece of each color. Only stacks of 2
  /// or more pieces are considered in this value. The first dimension is the
  /// player color, and the second dimension is the piece type.
  ///
  /// height = map[color][type]
  ///
  std::map<wartzaar::types::color::Color,
      std::map<wartzaar::types::piecetype::PieceType, int> > stack_height_;

  /// A 2-dimensional map representing the number of pieces of each type for
  /// each player. The first dimension is the player color, and the second
  /// dimension is the piece type.
  ///
  /// count = map[color][type]
  ///
  std::map<wartzaar::types::color::Color,
    std::map<wartzaar::types::piecetype::PieceType, int> > piece_count_;
};

} // namespace wartzaar

#endif // WARTZAAR_GAME_STATE_H_
