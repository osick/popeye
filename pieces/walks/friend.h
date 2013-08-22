#if !defined(PIECES_WALKS_FRIEND_H)
#define PIECES_WALKS_FRIEND_H

/* This module implements the Friend */

#include "position/board.h"
#include "position/position.h"
#include "utilities/boolean.h"
#include "solving/observation.h"

void locate_observees(PieNam walk, square pos_observees[]);
void isolate_observee(PieNam walk, square const pos_observees[], unsigned int isolated_observee);
void restore_observees(PieNam walk, square const pos_observees[]);

boolean find_next_friend_in_chain(square sq_target,
                                  PieNam friend_observer,
                                  evalfunction_t *evaluate);

/* Generate moves for a Friend
 */
void friend_generate_moves(void);

boolean friend_check(evalfunction_t *evaluate);

#endif