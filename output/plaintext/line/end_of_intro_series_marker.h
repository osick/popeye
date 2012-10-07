#if !defined(OUTPUT_PLAINTEXT_LINE_END_OF_INTRO_SERIES_MARKER_H)
#define OUTPUT_PLAINTEXT_LINE_END_OF_INTRO_SERIES_MARKER_H

#include "solving/solve.h"

/* This module provides the STOutputPlaintextLineEndOfIntroSeriesMarker
 * slice type. Slices of this type remember the end of an intro series.
 */

/* Number of move inversions up to the current move.
 * Exposed for read-only access only
 */
extern boolean is_end_of_intro_series[maxply+1];


/* Allocate a STOutputPlaintextLineEndOfIntroSeriesMarker slice.
 * @return index of allocated slice
 */
slice_index alloc_output_plaintext_line_end_of_intro_series_marker_slice(void);

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            slack_length-2 the move just played or being played is illegal
 *            <=n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type
output_plaintext_line_end_of_intro_series_marker_solve(slice_index si,
                                                        stip_length_type n);

#endif
