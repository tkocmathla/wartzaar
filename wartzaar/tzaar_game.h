#ifndef WARTZAAR_TZAAR_GAME_H_
#define WARTZAAR_TZAAR_GAME_H_

#include <time.h>  // for clock, CLOCKS_PER_SEC

#include <map>
#include <queue>
#include <vector>

#include "wartzaar/game_state.h"
#include "wartzaar/messages/move_message.h"
#include "wartzaar/priority_vector.h"
#include "wartzaar/types/color.h"
#include "wartzaar/types/direction.h"
#include "wartzaar/types/player_number.h"

namespace wartzaar {

///-----------------------------------------------------------------------------
///
///-----------------------------------------------------------------------------
class TzaarGame {
 public:
  /// Constructor.
  TzaarGame(int turn_time, int max_depth, int beam_size, int tzaar_coefficient,
      int tzarra_coefficient, int tott_coefficient, int stack_coefficient);

  /// Copy constructor and assignment operator are not supported.
  TzaarGame(const TzaarGame&);
  void operator=(const TzaarGame&);

  /// This is the public interface for the AI search algorithm.
  wartzaar::messages::MoveMessage GetNextMove(bool capture_only);

  /// Makes the given move on the current state.
  void MakeMove(int from_column, int from_row, int to_column, int to_row);

  wartzaar::types::color::Color OppositeColor(wartzaar::types::color::Color color) const;

  void set_current_state(const GameState &current_state);

  wartzaar::types::color::Color player_color();
  void set_player_color(const std::string &player_color_string);

  wartzaar::types::playernumber::PlayerNumber player_number();
  void set_player_number(const std::string &player_number_string);

  /// Accessor for turn_count_ member.
  int turn_count();
  void set_turn_count(int turn_count);

  /// Accessor for move_count_ member.
  int turn_move_count();
  void set_turn_move_count(int turn_move_count);

 private:
  void Init();

  /// Executes the recursive minimax search on the game state tree.
  float Minimax(GameState &state, int depth, float alpha, float beta,
      wartzaar::types::color::Color color, bool capture_only);

  /// Returns the number of possible game states based on the current state.
  int CountSuccessors(const GameState &state,
      wartzaar::types::color::Color color, bool capture_only) const;

  /// Finds the next possible game states based on the current state.
  std::vector<GameState> FindSuccessors(const GameState &state,
      wartzaar::types::color::Color color, bool capture_only) const;

  ///
  float EvaluateHeuristic(GameState state, wartzaar::types::color::Color color,
      bool capture_only);

  /// Pre-computed values for ln(n - 0.9), from 1 through 15.
  float ln_[16];

  /// The duration of each turn, in seconds.
  /// This is used to limit the time spent trying to find the best next move.
  int turn_time_;

  /// The total number of plies to search down the state tree.
  int max_depth_;

  /// The depth of the current iteration of the DFID/minimax search.
  int local_depth_;

  /// The number of states to search at each ply.
  int beam_size_;

  int tzaar_coefficient_;
  int tzarra_coefficient_;
  int tott_coefficient_;
  int stack_coefficient_;

  std::vector<wartzaar::types::direction::Direction> directions_;
  GameState current_state_;
  GameState best_move_;

  wartzaar::types::color::Color player_color_;
  wartzaar::types::playernumber::PlayerNumber player_number_;
  int turn_count_;
  int turn_move_count_;
  clock_t turn_move_timeout_;

//  std::map<std::string, float> memoized_states_;
};

} // namespace wartzaar

#endif // WARTZAAR_TZAAR_GAME_H_
