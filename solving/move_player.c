#include "solving/move_player.h"
#include "pieces/walks/pawns/en_passant.h"
#include "position/position.h"
#include "solving/move_effect_journal.h"
#include "solving/move_generator.h"
#include "solving/has_solution_type.h"
#include "solving/pipe.h"
#include "debugging/trace.h"
#include "debugging/assert.h"

static void play_move(Side side)
{
  numecoup const curr = CURRMOVE_OF_PLY(nbply);
  move_generation_elmt const * const move_gen_top = move_generation_stack+curr;
  square const sq_capture = move_gen_top->capture;
  square const sq_departure = move_gen_top->departure;
  square const sq_arrival = move_gen_top->arrival;

  if (en_passant_is_ep_capture(sq_capture))
    move_effect_journal_do_capture_move(sq_departure,
                                        sq_arrival,
                                        sq_capture-offset_en_passant_capture,
                                        move_effect_reason_ep_capture);
  else if (is_square_empty(sq_capture))
  {
    move_effect_journal_do_no_piece_removal();
    move_effect_journal_do_piece_movement(move_effect_reason_moving_piece_movement,
                                          sq_departure,sq_arrival);
  }
  else
    move_effect_journal_do_capture_move(sq_departure,
                                        sq_arrival,
                                        sq_capture,
                                        move_effect_reason_regular_capture);
}

/* Try to solve in solve_nr_remaining half-moves.
 * @param si slice index
 * @note assigns solve_result the length of solution found and written, i.e.:
 *            previous_move_is_illegal the move just played is illegal
 *            this_move_is_illegal     the move being played is illegal
 *            immobility_on_next_move  the moves just played led to an
 *                                     unintended immobility on the next move
 *            <=n+1 length of shortest solution found (n+1 only if in next
 *                                     branch)
 *            n+2 no solution found in this branch
 *            n+3 no solution found in next branch
 *            (with n denominating solve_nr_remaining)
 */
void move_player_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  play_move(SLICE_STARTER(si));
  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
