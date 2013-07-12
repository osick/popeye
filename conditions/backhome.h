#if !defined(CONDITIONS_RETOUR_H)
#define CONDITIONS_RETOUR_H

/* This module implements the fairy condition "Back-Home" */

#include "py.h"

int len_backhome(square sq_departure, square sq_arrival, square sq_capture);

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            previous_move_is_illegal the move just played (or being played)
 *                                     is illegal
 *            immobility_on_next_move  the moves just played led to an
 *                                     unintended immobility on the next move
 *            <=n+1 length of shortest solution found (n+1 only if in next
 *                                     branch)
 *            n+2 no solution found in this branch
 *            n+3 no solution found in next branch
 */
stip_length_type back_home_moves_only_solve(slice_index si, stip_length_type n);

/* Initialise solving in Back-Home
 */
void backhome_initialise_solving(void);

#endif