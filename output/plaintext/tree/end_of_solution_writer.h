#if !defined(OUTPUT_PLAINTEXT_END_OF_SOLUTION_WRITER_H)
#define OUTPUT_PLAINTEXT_END_OF_SOLUTION_WRITER_H

#include "solving/solve.h"

/* Allocate a STEndOfSolutionWriter slice.
 * @return index of allocated slice
 */
slice_index alloc_end_of_solution_writer_slice(void);

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            slack_length-2 the move just played or being played is illegal
 *            <=n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type end_of_solution_writer_solve(slice_index si,
                                               stip_length_type n);

#endif
