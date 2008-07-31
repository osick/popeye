#if !defined(PYSEQUEN_H)
#define PYSEQUEN_H

#include "py.h"
#include "pyhash.h"
#include "pycompos.h"
#include "boolean.h"

/* This module provides functionality dealing with sequence
 * (i.e. simply concatenated) stipulation slices.
 */

/* Determine and write continuations at end of sequence slice
 * @param attacker attacking side
 * @param continuations table where to store continuing moves
 *                      (e.g. threats)
 * @param si index of sequence slice
 */
void d_sequence_end_solve_continuations(Side attacker,
                                        int continuations,
                                        slice_index si);

/* Find and write defender's set play
 * @param defender defending side
 * @param si slice index
 */
void d_sequence_end_solve_setplay(Side defender, slice_index si);

/* Find and write defender's set play in self/reflex play if every
 * set move leads to end
 * @param defender defending side
 * @param si slice index
 * @return true iff every defender's move leads to end
 */
boolean d_sequence_end_solve_complete_set(Side defender, slice_index si);

/* Continue solving at the end of a sequence slice
 * Unsolvability (e.g. because of a forced reflex move) has already
 * been dealt with.
 * @param attacker attacking side
 * @param restartenabled true iff the written solution should only
 *                       start at the Nth legal move of attacker
 *                       (determined by user input)
 * @param si slice index 
 */
void d_sequence_end_solve(Side attacker,
                          boolean restartenabled,
                          slice_index si);

/* Write the key just played, then continue solving at end of sequence
 * slice to find and write the post key play (threats, variations) and
 * write the refutations (if any)
 * @param attacker attacking side (has just played key)
 * @param refutations table containing the refutations (if any)
 * @param si slice index
 * @param is_try true iff what we are solving is a try
 */
void d_sequence_end_write_key_solve_postkey(Side attacker,
                                            int refutations,
                                            slice_index si,
                                            boolean is_try);

/* Continue solving help play at the end of a sequence slice
 * @param side_at_move side at the move
 * @param no_succ_hash_category hash category for storing failures
 * @param restartenabled true iff option movenum is activated
 * @param si slice index
 * @return true iff >=1 solution was found
 */
boolean h_sequence_end_solve(Side side_at_move,
                             hashwhat no_succ_hash_category,
                             boolean restartenabled,
                             slice_index si);

/* Continue solving series play at the end of a sequence slice
 * @param series_side side playing the series
 * @param restartenabled true iff option movenum is activated
 * @param si slice index
 * @return true iff >=1 solution was found
 */
boolean ser_sequence_end_solve(Side series_side,
                               boolean restartenabled,
                               slice_index si);

/* Determine whether the attacking side wins at the end of a sequence slice
 * @param attacker attacking side
 * @param si slice identifier
 * @return true iff attacker wins
 */
boolean d_sequence_end_does_attacker_win(Side attacker, slice_index si);

/* Find and write variations starting at end of sequence slice
 * @param attacker attacking side
 * @param len_threat length of threat (shorter variations are suppressed)
 * @param threats table containing threats (variations not defending
 *                against all threats are suppressed)
 * @param refutations table containing refutations (written at end)
 * @param si slice index
 */
void d_sequence_end_solve_variations(Side attacker,
                                     int len_threat,
                                     int threats,
                                     int refutations,
                                     slice_index si);

/* Determine whether the defending side wins at the end of a sequence slice
 * @param defender defending side
 * @param si slice identifier
 * @return "how much or few" the defending side wins
 */
d_composite_win_type d_sequence_end_does_defender_win(Side defender,
                                                      slice_index si);

/* Determine whether the defender has directly lost in direct play
 * with his move just played.
 * Assumes that there is no short win for the defending side.
 * @param attacker attacking side
 * @param si slice identifier
 * @return true iff the defending side has directly lost
 */
boolean d_sequence_end_has_defender_lost(Side attacker, slice_index si);

/* Attempt to deremine which side is at the move
 * at the start of a slice.
 * @param si identifies slice
 * @param is_duplex is this for duplex?
 * @return one of blanc, noir, no_side (the latter if we can't
 *         determine which side is at the move)
 */
Side sequence_who_starts(slice_index si, boolean is_duplex);

#endif
