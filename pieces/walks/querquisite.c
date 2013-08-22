#include "pieces/walks/querquisite.h"
#include "pieces/walks/leapers.h"
#include "pieces/walks/riders.h"
#include "solving/move_generator.h"
#include "debugging/trace.h"
#include "pydata.h"

/* Generate moves for an Querquisite
 */
void querquisite_generate_moves(void)
{
  switch (curr_generation->departure%onerow - nr_of_slack_files_left_of_board)
  {
    case file_rook_queenside:
    case file_rook_kingside:
      rider_generate_moves(vec_rook_start,vec_rook_end);
      break;
    case file_bishop_queenside:
    case file_bishop_kingside:
      rider_generate_moves(vec_bishop_start,vec_bishop_end);
      break;
    case file_queen:
      rider_generate_moves(vec_queen_start,vec_queen_end);
      break;
    case file_knight_queenside:
    case file_knight_kingside:
      leaper_generate_moves(vec_knight_start,vec_knight_end);
      break;
    case file_king:
      leaper_generate_moves(vec_queen_start,vec_queen_end);
      break;
  }
}

boolean querquisite_check(evalfunction_t *evaluate)
{
  square const sq_target = move_generation_stack[current_move[nbply]-1].capture;
  vec_index_type k;

  for (k = vec_rook_start; k<=vec_rook_end; k++)
  {
    square const sq_departure = find_end_of_line(sq_target,vec[k]);
    int const file_departure= sq_departure%onerow - nr_of_slack_files_left_of_board;
    if ((file_departure==file_rook_queenside
         || file_departure==file_queen
         || file_departure==file_rook_kingside)
        && INVOKE_EVAL(evaluate,sq_departure,sq_target))
      return true;
  }

  for (k= vec_bishop_start; k<=vec_bishop_end; k++)
  {
    square sq_departure = find_end_of_line(sq_target,vec[k]);
    int const file_departure= sq_departure%onerow - nr_of_slack_files_left_of_board;
    if ((file_departure==file_bishop_queenside
         || file_departure==file_queen
         || file_departure==file_bishop_kingside)
        && INVOKE_EVAL(evaluate,sq_departure,sq_target))
      return true;
  }

  for (k= vec_knight_start; k<=vec_knight_end; k++)
  {
    square const sq_departure= sq_target+vec[k];
    int const file_departure= sq_departure%onerow - nr_of_slack_files_left_of_board;
    if ((file_departure==file_knight_queenside
         || file_departure==file_knight_kingside)
        && INVOKE_EVAL(evaluate,sq_departure,sq_target))
      return true;
  }

  for (k= vec_queen_start; k<=vec_queen_end; k++)
  {
    square const sq_departure= sq_target+vec[k];
    int const file_departure= sq_departure%onerow - nr_of_slack_files_left_of_board;
    if (file_departure==file_king
        && INVOKE_EVAL(evaluate,sq_departure,sq_target))
      return true;
  }

  return false;
}