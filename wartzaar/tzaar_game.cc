#include "wartzaar/tzaar_game.h"

#include <float.h>  // for FLT_MAX
#include <math.h>   // for log

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace wm   = wartzaar::messages;
namespace wtc  = wartzaar::types::color;
namespace wtd  = wartzaar::types::direction;
namespace wtpn = wartzaar::types::playernumber;
namespace wtpt = wartzaar::types::piecetype;

namespace wartzaar {

TzaarGame::TzaarGame(int turn_time, int search_depth, int beam_size,
    int tzaar_coefficient, int tzarra_coefficient, int tott_coefficient,
    int stack_coefficient)
    : turn_time_(turn_time),
      max_depth_(search_depth),
      local_depth_(0),
      beam_size_(beam_size),
      tzaar_coefficient_(tzaar_coefficient),
      tzarra_coefficient_(tzarra_coefficient),
      tott_coefficient_(tott_coefficient),
      stack_coefficient_(stack_coefficient),
      current_state_(GameState(GameBoard())),
      best_move_(GameState(GameBoard())),
      player_color_(),
      player_number_(),
      turn_count_(0),
      turn_move_count_(0),
      turn_move_timeout_(0) {
  Init();
}

void TzaarGame::Init() {
  directions_.push_back(wtd::kNorth);
  directions_.push_back(wtd::kNortheast);
  directions_.push_back(wtd::kSoutheast);
  directions_.push_back(wtd::kSouth);
  directions_.push_back(wtd::kSouthwest);
  directions_.push_back(wtd::kNorthwest);

  ln_[15] = 2.6461748f;
  ln_[14] = 2.57261223f;
  ln_[13] = 2.49320545f;
  ln_[12] = 2.40694511f;
  ln_[11] = 2.31253542f;
  ln_[10] = 2.20827441f;
  ln_[9]  = 2.09186406f;
  ln_[8]  = 1.96009478f;
  ln_[7]  = 1.80828877f;
  ln_[6]  = 1.62924054f;
  ln_[5]  = 1.41098697f;
  ln_[4]  = 1.13140211f;
  ln_[3]  = 0.741937345f;
  ln_[2]  = 0.0953101798f;
  ln_[1]  = -2.30258509f;
  ln_[0]  = 0.0f;
}

wm::MoveMessage TzaarGame::GetNextMove(bool capture_only) {
  // Initialize the best move
  best_move_ = GameState(GameBoard());

  // Wind up the alarm clock
  turn_move_timeout_ = clock() + turn_time_ * CLOCKS_PER_SEC;

  // Execute the minimax search using depth-first iterative deepening (DFID)
  for (local_depth_ = 1; local_depth_ <= max_depth_ && clock() < turn_move_timeout_; local_depth_++) {
    Minimax(current_state_, local_depth_, -FLT_MAX, FLT_MAX, player_color_, capture_only);

    std::cout << "TzaarGame::GetNextMove: Completed minimax search for ply = " << local_depth_
              << "; current best move = " << best_move_.ToString()
              << "; hval = " << best_move_.heuristic_value()
              << std::endl;

    // Stop searching for moves if a win state was found
    if (best_move_.heuristic_value() == FLT_MAX)
      break;
  }

  current_state_ = best_move_;

  std::cout << "Clock: " << clock() << ", Timeout: " << turn_move_timeout_ << std::endl;
  std::cout << "Best move piece counts: " << best_move_.ToString()
            << ": W("  << best_move_.GetPieceCount(wtc::kWhite, wtpt::kTzaar)
            << ", "    << best_move_.GetPieceCount(wtc::kWhite, wtpt::kTzarra)
            << ", "    << best_move_.GetPieceCount(wtc::kWhite, wtpt::kTott)
            << "), B(" << best_move_.GetPieceCount(wtc::kBlack, wtpt::kTzaar)
            << ", "    << best_move_.GetPieceCount(wtc::kBlack, wtpt::kTzarra)
            << ", "    << best_move_.GetPieceCount(wtc::kBlack, wtpt::kTott)
            << ")"     << std::endl;

  return wm::MoveMessage(
    best_move_.last_move_from()->col(),
    best_move_.last_move_from()->row(),
    best_move_.last_move_to()->col(),
    best_move_.last_move_to()->row()
  );
}

float TzaarGame::Minimax(GameState &state, int depth, float alpha, float beta,
    wtc::Color color, bool capture_only) {
  // Bail out if we're at the depth limit
  if (depth == 0 || clock() >= turn_move_timeout_)
    return EvaluateHeuristic(state, color, capture_only);

  // Get successor states
  std::vector<GameState> successors = FindSuccessors(state, color, capture_only);

//  std::cout << "Have " << successors.size() << " successors for "
//            << (capture_only ? "capture_only" : "!capture_only") << " move"
//            << std::endl;

  // Bail out if this is a terminal (leaf) state
  if (successors.size() == 0)
    return EvaluateHeuristic(state, color, capture_only);

  // Initialize the best move
  if (depth == local_depth_) {
    successors[0].set_heuristic_value(EvaluateHeuristic(successors[0], color,
        capture_only));

    if (best_move_.heuristic_value() == -FLT_MAX
        || successors[0].heuristic_value() > best_move_.heuristic_value())
      best_move_ = successors[0];
  }

  // Maximizing player's turn (ours)
  int successors_passed = 0;
  if (player_color_ == color) {
    std::vector<GameState>::iterator successor_itr = successors.begin();
    while (successor_itr != successors.end() && clock() < turn_move_timeout_ && successors_passed <= beam_size_) {

//      std::cout << "Evaluating child " << successors_passed + 1 << "/" << successors.size()
//                << " (" << successor_itr->ToString() << ")" << std::endl;

      float value = -FLT_MAX;

      // For the two moves of our turn, return +infinity directly for any
      // winning state.
      //
      // If it's the first move of the game, or this player has sent both moves,
      // the turn is over, and we need to call minimax for the opposite player
      // with capture_only = true.
      //
      // Otherwise, it's the first move of the turn, and we need to call minimax
      // for the same player with capture_only = false.
      //
      if (depth >= local_depth_ - 1)
        value = EvaluateHeuristic(*successor_itr, color, capture_only);

      if (value < FLT_MAX) {
        if ((turn_count_ == 0 && depth == local_depth_) || !capture_only) {
  //        std::cout << "MAX player calling Minimax(" << successor_itr->ToString() << ", " << depth - 1 << ", opponent, capture_only)" << std::endl;
          value = Minimax(*successor_itr, depth - 1, alpha, beta, OppositeColor(color), true);
        }
        else {
  //        std::cout << "MAX player calling Minimax(" << successor_itr->ToString() << ", " << depth - 1 << ", self, !capture_only)" << std::endl;
          value = Minimax(*successor_itr, depth - 1, alpha, beta, color, false);
        }
      }

      if (value > alpha || (depth >= local_depth_ - 1 && value == FLT_MAX)) {
        alpha = value;

        if (depth >= local_depth_ - 1 && value == FLT_MAX)
          successor_itr->set_heuristic_value(FLT_MAX);
        else
          successor_itr->set_heuristic_value(EvaluateHeuristic(*successor_itr,
              color, capture_only));

        if (depth == local_depth_
            && successor_itr->heuristic_value() > best_move_.heuristic_value())
          best_move_ = *successor_itr;
      }

      if (alpha >= beta) break;  // beta cutoff

      ++successor_itr;
      ++successors_passed;
    }

    return alpha;
  }
  // Minimizing player's turn (opponent's)
  else {
    std::vector<GameState>::iterator successor_itr = successors.begin();
    while (successor_itr != successors.end() && clock() < turn_move_timeout_ && successors_passed <= beam_size_) {
      // If it's the first move of the turn, call minimax for the same player
      // with capture_only = false.
      //
      // Otherwise, this player has sent both moves, and we need to call minimax
      // for the opposite player with capture_only = true.
      //
      float value = -FLT_MAX;
      if (capture_only) {
//        std::cout << "MIN player calling Minimax(" << successor_itr->ToString() << ", " << depth - 1 << ", self, !capture_only)" << std::endl;
        value = Minimax(*successor_itr, depth - 1, alpha, beta, color, false);
      }
      else {
//        std::cout << "MIN player calling Minimax(" << successor_itr->ToString() << ", " << depth - 1 << ", opponent, capture_only)" << std::endl;
        value = Minimax(*successor_itr, depth - 1, alpha, beta, OppositeColor(color), true);
      }

//      std::cout << "...value = " << value << std::endl;

      if (value < beta) beta = value;
      if (alpha >= beta) break;  // alpha cutoff

      ++successor_itr;
      ++successors_passed;
    }

    return beta;
  }
}

int TzaarGame::CountSuccessors(const GameState &from_state,
    wtc::Color color, bool capture_only) const {
  int count = 0;
  std::vector< std::vector<GameBoardPosition> >::const_iterator col_itr;

  for (col_itr = from_state.BeginConst(); col_itr != from_state.EndConst(); ++col_itr) {
    std::vector<GameBoardPosition>::const_iterator row_itr;

    for (row_itr = col_itr->begin(); row_itr != col_itr->end(); ++row_itr) {
      // Skip empty positions and opponent positions
      if (row_itr->stack_height() == 0
          || (row_itr->color() != 0 && *(row_itr->color()) != color))
        continue;

      // Search for possible moves in each direction
      for (size_t i = 0; i < directions_.size(); ++i) {
        GameBoardPosition *pos = row_itr->SearchPath(directions_[i]);

        // Check for capturing move
        if (pos != 0 && color != *(pos->color())
            && row_itr->stack_height() >= pos->stack_height())
          ++count;

        // Check for stacking move
        if (pos != 0 && *(pos->color()) == color && capture_only == false
            && from_state.GetPieceCount(color, *(pos->type())) > 1)
          ++count;
      }
    }
  }

  return count;

}

/// A capturing move can be made in a given direction if:
///   - The next link is not null
///   - The next cell has one or more pieces in it
///   - The stack in the next cell belongs to the opponent
///   - The size of the stack in the next cell is equal to or smaller than
///     ours
///
/// A stacking move can be made in a given direction if:
///   - The next link is not null
///   - The next cell has one or more pieces in it
///   - The stack in the next cell belongs to us
///   - There are at least 2 of the destination piece type left on the board
///
std::vector<GameState> TzaarGame::FindSuccessors(const GameState &from_state,
    wtc::Color color, bool capture_only) const {
  std::vector<GameState> successors;
  std::vector< std::vector<GameBoardPosition> >::const_iterator col_itr;

  for (col_itr = from_state.BeginConst(); col_itr != from_state.EndConst(); ++col_itr) {
    std::vector<GameBoardPosition>::const_iterator row_itr;

    for (row_itr = col_itr->begin(); row_itr != col_itr->end(); ++row_itr) {
      // Skip empty positions and opponent positions
      if (row_itr->stack_height() == 0
          || (row_itr->color() != 0 && *(row_itr->color()) != color))
        continue;

      // Search for possible moves in each direction
      for (size_t i = 0; i < directions_.size(); ++i) {
        GameBoardPosition *pos = row_itr->SearchPath(directions_[i]);

        // Check for capturing move
        if (pos != 0 && color != *(pos->color())
            && row_itr->stack_height() >= pos->stack_height()) {
          successors.push_back(GameState(from_state));
          successors.back().MakeMove(row_itr->col(), row_itr->row(), pos->col(), pos->row());
        }

        // Check for stacking move
        if (pos != 0 && *(pos->color()) == color && capture_only == false
            && from_state.GetPieceCount(color, *(pos->type())) > 1) {
          successors.push_back(GameState(from_state));
          successors.back().MakeMove(row_itr->col(), row_itr->row(), pos->col(), pos->row());
        }
      }
    }
  }

  return successors;
}

/// ----------------------------------------------------------------------------
/// Piece count heuristic: A different weight is given to each piece type,
/// depending on starting count.
///
/// The weight factor is multiplied by the natural log of the specific piece's
/// count, less 0.9. The log function ensures that small piece counts cause the
/// heuristic value to drop sharply, while relatively large piece counts don't
/// have a dramatic effect. The piece count is reduced by 0.9 so a value of 1
/// actually results in a negative effect on the heuristic value, while values
/// between 2 and 15 have increasingly positive effects.
///
/// Starting state:
///   ln(6 - 0.9)  = 1.62924054  // tzaars
///   ln(9 - 0.9)  = 2.09186406  // tzarras
///   ln(15 - 0.9) = 2.6461748   // totts
///
/// A very bad state:
///   ln(1 - 0.9) = -2.30258509
///
/// ----------------------------------------------------------------------------
/// Stack height heuristic: Higher stacks are more powerful and protect the top
/// pieces better than if they were on their own.
///
/// This is an important tactic for preventing the opponent from capturing all
/// of one piece type. The stack height is adjusted so that single pieces do not
/// skew the heuristic, thus the stack height at game start is zero.
///
float TzaarGame::EvaluateHeuristic(GameState state, wtc::Color color,
    bool capture_only) {
  float hval = 0.0f; // heuristic value

  int white_tzaars  = state.GetPieceCount(wtc::kWhite, wtpt::kTzaar);
  int white_tzarras = state.GetPieceCount(wtc::kWhite, wtpt::kTzarra);
  int white_totts   = state.GetPieceCount(wtc::kWhite, wtpt::kTott);
  int black_tzaars  = state.GetPieceCount(wtc::kBlack, wtpt::kTzaar);
  int black_tzarras = state.GetPieceCount(wtc::kBlack, wtpt::kTzarra);
  int black_totts   = state.GetPieceCount(wtc::kBlack, wtpt::kTott);

  // Calculate a combined weighted piece count value
  float white_count = tzaar_coefficient_  * log(white_tzaars)
                    + tzarra_coefficient_ * log(white_tzarras)
                    + tott_coefficient_   * log(white_totts);
  float black_count = tzaar_coefficient_  * log(black_tzaars)
                    + tzarra_coefficient_ * log(black_tzarras)
                    + tott_coefficient_   * log(black_totts);

  //    white_count = tzaar_coefficient_  * ln_[white_tzaars]
  //                + tzarra_coefficient_ * ln_[white_tzarras]
  //                + tott_coefficient_   * ln_[white_totts];
  //    black_count = tzaar_coefficient_  * ln_[black_tzaars]
  //                + tzarra_coefficient_ * ln_[black_tzarras]
  //                + tott_coefficient_   * ln_[black_totts];

  // Check for end-of-game scenarios
  if (color == wtc::kWhite) {
    if (black_tzaars < 1 || black_tzarras < 1 || black_totts < 1)
      return FLT_MAX;
    else if (white_tzaars < 1 || white_tzarras < 1 || white_totts < 1)
      return -FLT_MAX;
    else
      hval += white_count - black_count;
  }
  else if (color == wtc::kBlack){
    if (white_tzaars < 1 || white_tzarras < 1 || white_totts < 1)
      return FLT_MAX;
    else if (black_tzaars < 1 || black_tzarras < 1 || black_totts < 1)
      return -FLT_MAX;
    else
      hval += black_count - white_count;
  }

  int white_tzaar_height  = state.GetStackHeight(wtc::kWhite, wtpt::kTzaar);
  int white_tzarra_height = state.GetStackHeight(wtc::kWhite, wtpt::kTzarra);
  int white_tott_height   = state.GetStackHeight(wtc::kWhite, wtpt::kTott);
  int black_tzaar_height  = state.GetStackHeight(wtc::kBlack, wtpt::kTzaar);
  int black_tzarra_height = state.GetStackHeight(wtc::kBlack, wtpt::kTzarra);
  int black_tott_height   = state.GetStackHeight(wtc::kBlack, wtpt::kTott);

  float white_height = tzaar_coefficient_  * log(white_tzaar_height  + 1.0f)
                     + tzarra_coefficient_ * log(white_tzarra_height + 1.0f)
                     + tott_coefficient_   * log(white_tott_height   + 1.0f);
  float black_height = tzaar_coefficient_  * log(black_tzaar_height  + 1.0f)
                     + tzarra_coefficient_ * log(black_tzarra_height + 1.0f)
                     + tott_coefficient_   * log(black_tott_height   + 1.0f);

//  float white_height = tzaar_coefficient_  * white_tzaar_height
//                     + tzarra_coefficient_ * white_tzarra_height
//                     + tott_coefficient_   * white_tott_height;
//  float black_height = tzaar_coefficient_  * black_tzaar_height
//                     + tzarra_coefficient_ * black_tzarra_height
//                     + tott_coefficient_   * black_tott_height;

  if (color == wtc::kWhite) {
    hval += white_height - black_height;
    hval *= white_count / white_height;
  }
  else if (color == wtc::kBlack){
    hval += black_height - white_height;
    hval *= black_count / black_height;
  }

  // Capturing moves available?
  int moves = CountSuccessors(state, color, true);
  if (moves == 0) return -FLT_MAX;

  int oppmoves = CountSuccessors(state, OppositeColor(color), true);
  if (oppmoves == 0) return FLT_MAX;

  return hval;
}

void TzaarGame::MakeMove(int from_column, int from_row, int to_column, int to_row) {
  ++turn_count_;
  current_state_.MakeMove(from_column, from_row, to_column, to_row);
}

wtc::Color TzaarGame::OppositeColor(wtc::Color color) const {
  if (color == wtc::kWhite)
    return wtc::kBlack;
  else if (color == wtc::kBlack)
    return wtc::kWhite;
  else
    throw std::runtime_error("Can't find opposite color: invalid color!");
}

wtc::Color TzaarGame::player_color() {
  return player_color_;
}

wtpn::PlayerNumber TzaarGame::player_number() {
  return player_number_;
}

void TzaarGame::set_player_number(const std::string &player_number_string) {
  if (player_number_string == "One") {
    player_number_ = wtpn::kPlayerOne;
    player_color_ = wtc::kWhite;
  }
  else if (player_number_string == "Two") {
    player_number_ = wtpn::kPlayerTwo;
    player_color_ = wtc::kBlack;
  }
  else {
    throw std::runtime_error("Can't set player number: invalid player number!");
  }
}

void TzaarGame::set_current_state(const GameState &state) {
  current_state_ = state;
}

int TzaarGame::turn_count() {
  return turn_count_;
}

void TzaarGame::set_turn_count(int turn_count) {
  turn_count_ = turn_count;
}

int TzaarGame::turn_move_count() {
  return turn_move_count_;
}

void TzaarGame::set_turn_move_count(int turn_move_count) {
  turn_move_count_ = turn_move_count;
}

} // namespace wartzaar
