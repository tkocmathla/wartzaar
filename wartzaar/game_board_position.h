#ifndef WARTZAAR_GAME_BOARD_POSITION_H_
#define WARTZAAR_GAME_BOARD_POSITION_H_

#include <string>

#include "wartzaar/types/color.h"
#include "wartzaar/types/direction.h"
#include "wartzaar/types/piece_type.h"

namespace wtc = wartzaar::types::color;
namespace wtpt = wartzaar::types::piecetype;

namespace wartzaar {

///-----------------------------------------------------------------------------
///
///-----------------------------------------------------------------------------
class GameBoardPosition {
 public:
  /// Default constructor.
  GameBoardPosition();

  /// Destructor.
  ~GameBoardPosition();

  /// Constructor initializes a position with a column and row.
  GameBoardPosition(int col, int row);

  /// Constructor initializes a position with a column and row and a single
  /// game piece.
  GameBoardPosition(int col, int row, wartzaar::types::color::Color color,
      wartzaar::types::piecetype::PieceType type);

  /// Constructor initializes a position with a column and row and a stack of
  /// game pieces.
  GameBoardPosition(int col, int row, wartzaar::types::color::Color color,
      wartzaar::types::piecetype::PieceType type, int stack_height);

  /// Copy constructor
  GameBoardPosition(const GameBoardPosition &that);

  void AddPieces(wartzaar::types::color::Color color,
      wartzaar::types::piecetype::PieceType type, int stack_height);

  void ClearPieces();

  GameBoardPosition* SearchPath(const wartzaar::types::direction::Direction &dir) const;

  /// Returns the pointer for the position in the given direction.
  GameBoardPosition* DirLink(const wartzaar::types::direction::Direction &dir) const;

  /// Returns true if the given game board position is equal to this one.
  bool Equals(const GameBoardPosition &that) const;

  static wtc::Color ParseColor(const std::string &color_string);
  static wtpt::PieceType ParseType(const std::string &type_string);
  std::string ColorString();
  std::string TypeString();

  int col() const;
  void set_col(int col);

  int row() const;
  void set_row(int row);

  int stack_height() const;
  void set_stack_height(int stack_height);

  wartzaar::types::color::Color* color() const;
  void set_color(wartzaar::types::color::Color *color);

  wartzaar::types::piecetype::PieceType* type() const;
  void set_type(wartzaar::types::piecetype::PieceType *type);

  GameBoardPosition* n();
  void n(const GameBoardPosition *n);

  GameBoardPosition* ne();
  void ne(const GameBoardPosition *ne);

  GameBoardPosition* se();
  void se(const GameBoardPosition *se);

  GameBoardPosition* s();
  void s(const GameBoardPosition *s);

  GameBoardPosition* sw();
  void sw(const GameBoardPosition *sw);

  GameBoardPosition* nw();
  void nw(const GameBoardPosition *nw);

 private:
  unsigned int col_;
  unsigned int row_;
  unsigned int stack_height_;
  wartzaar::types::color::Color *color_;
  wartzaar::types::piecetype::PieceType *type_;
  GameBoardPosition *n_;
  GameBoardPosition *ne_;
  GameBoardPosition *se_;
  GameBoardPosition *s_;
  GameBoardPosition *sw_;
  GameBoardPosition *nw_;
};

} // namespace wartzaar

#endif // WARTZAAR_GAME_BOARD_POSITION_H_
