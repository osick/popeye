#if !defined(STIPULATION_BOOLEAN_NOT_H)
#define STIPULATION_BOOLEAN_NOT_H

#include "py.h"
#include "solving/solve.h"
#include "utilities/boolean.h"

/* This module provides functionality dealing with STNot stipulation
 * slices.
 */

/* Allocate a not slice.
 * @return index of allocated slice
 */
slice_index alloc_not_slice(void);

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            slack_length-2 the move just played or being played is illegal
 *            <=n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type not_solve(slice_index si, stip_length_type n);

#endif
