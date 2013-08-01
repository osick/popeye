#if !defined(CONDITIONS_DISPARATE_H)
#define CONDITIONS_DISPARATE_H

#include "utilities/boolean.h"
#include "py.h"

/* Implementation of condition Disparate Chess
 */

/* Validate an observation according to Disparate Chess
 * @return true iff the observation is valid
 */
boolean disparate_validate_observation(slice_index si);

/* Generate moves for a single piece
 * @param identifies generator slice
 * @param sq_departure departure square of generated moves
 * @param p walk to be used for generating
 */
void disparate_generate_moves_for_piece(slice_index si,
                                        square sq_departure,
                                        PieNam p);

/* Inialise the solving machinery with Disparate chess
 * @param si identifies root slice of solving machinery
 */
void disparate_initialise_solving(slice_index si);

#endif
