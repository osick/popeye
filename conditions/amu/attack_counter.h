#if !defined(CONDITIONS_AMU_ATTACK_COUNTER_H)
#define CONDITIONS_AMU_ATTACK_COUNTER_H

#include "solving/solve.h"

/* This module implements the condition Traitor Chess */

extern boolean amu_attacked_exactly_once[maxply+1];

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            slack_length-2 the move just played or being played is illegal
 *            <=n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type amu_attack_counter_solve(slice_index si, stip_length_type n);

/* Instrument slices with move tracers
 */
void stip_insert_amu_attack_counter(slice_index si);

#endif
