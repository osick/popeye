#include "conditions/forced_squares.h"
#include "debugging/trace.h"
#include "pydata.h"

#include <assert.h>

/* Determine the length of a move in the presence of forced squares; the higher
 * the value the more likely the move is going to be played.
 * @param sq_departure departure square
 * @param sq_arrival arrival square
 * @param sq_capture capture square
 * @return a value expressing the precedence of this move
 */
int forced_squares_measure_length(square sq_departure,
                                  square sq_arrival,
                                  square sq_capture)
{
  SquareFlags const flag = trait[nbply]==White ? WhForcedSq : BlForcedSq;
  int const result = TSTFLAG(sq_spec[sq_arrival],flag);

  TraceFunctionEntry(__func__);
  TraceSquare(sq_departure);
  TraceSquare(sq_arrival);
  TraceSquare(sq_capture);
  TraceFunctionParamListEnd();

  TraceValue("%x\n",sq_spec[sq_arrival]);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}
