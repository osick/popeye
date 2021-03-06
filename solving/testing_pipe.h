#if !defined(SOLVING_TESTING_PIPE_H)
#define SOLVING_TESTING_PIPE_H

/* Testing pipes
 * Delegate solving along the pipe only if testing the condition referred
 * to by .next2 is successful.
 * .next2 is a shortcut to a slice owned by somebody else. Deallocating a
 * testing pipe will not deallocate .next2 and the slices attached to it.
 */

#include "solving/machinery/solve.h"

/* Solve the next2 part of a fork
 * @param si identifies the fork slice
 * @param n maximum number of moves (typically slack_length or
 *         length_unspecified)
 * @return length of solution found and written, i.e.:
 *            previous_move_is_illegal the move just played is illegal
 *            this_move_is_illegal     the move being played is illegal
 *            immobility_on_next_move  the moves just played led to an
 *                                     unintended immobility on the next move
 *            <=n+1 length of shortest solution found (n+1 only if in next
 *                                     branch)
 *            n+2 no solution found in this branch
 *            n+3 no solution found in next branch
 */
stip_length_type testing_pipe_solve_delegate(slice_index si, stip_length_type n);

#endif
