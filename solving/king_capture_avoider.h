#if !defined(SOLVING_KING_CAPTURE_AVOIDER_H)
#define SOLVING_KING_CAPTURE_AVOIDER_H

#include "solving/solve.h"

/* This module provides functionality to avoid king capture in immobility
 * testing for double (and counter) mate goals.
 * According to current knowledge, this can only happen if the king to be
 * mated is neutral.
 */

/* Reset king capture avoiders
 */
void king_capture_avoiders_reset(void);

/* Make stip_insert_king_capture_avoiders() insert slices that prevent moves
 * that leave the moving side without king
 */
void king_capture_avoiders_avoid_own(void);

/* Make stip_insert_king_capture_avoiders() insert slices that prevent moves
 * that leave the moving side's opponent without king
 */
void king_capture_avoiders_avoid_opponent(void);

/* Instrument a stipulation
 * @param si identifies root slice of stipulation
 */
void stip_insert_king_capture_avoiders(slice_index si);

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            slack_length-2 the move just played or being played is illegal
 *            <=n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type own_king_capture_avoider_solve(slice_index si,
                                             stip_length_type n);

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            slack_length-2 the move just played or being played is illegal
 *            <=n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type opponent_king_capture_avoider_solve(slice_index si,
                                                     stip_length_type n);

#endif