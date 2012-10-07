#if !defined(CONDITIONS_PHANTOM_H)
#define CONDITIONS_PHANTOM_H

#include "solving/solve.h"

/* This module provides implements the condition Phantom Chess
 */

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            slack_length-2 the move just played or being played is illegal
 *            <=n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type phantom_en_passant_adjuster_solve(slice_index si,
                                                    stip_length_type n);

/* Instrument slices with promotee markers
 */
void stip_insert_phantom_en_passant_adjusters(slice_index si);

#endif
