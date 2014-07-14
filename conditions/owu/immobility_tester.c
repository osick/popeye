#include "conditions/owu/immobility_tester.h"
#include "stipulation/stipulation.h"
#include "solving/has_solution_type.h"
#include "stipulation/proxy.h"
#include "stipulation/branch.h"
#include "stipulation/slice_insertion.h"
#include "stipulation/boolean/and.h"
#include "solving/king_move_generator.h"
#include "solving/non_king_move_generator.h"
#include "solving/legal_move_counter.h"
#include "solving/capture_counter.h"
#include "debugging/trace.h"

#include "debugging/assert.h"

/* This module provides functionality dealing with slices that detect
 * whether a side is immobile
 */

static void substitute_owu_specific_testers(slice_index si,
                                            stip_structure_traversal *st)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  stip_traverse_structure_children_pipe(si,st);

  {
    slice_index const proxy_king = alloc_proxy_slice();
    slice_index const proxy_non_king = alloc_proxy_slice();
    slice_index const king_branch = slices[si].next1;
    slice_index const non_king_branch = stip_deep_copy(king_branch);
    slice_index const king_tester = alloc_pipe(STOWUImmobilityTesterKing);
    slice_index const non_king_tester = alloc_pipe(STImmobilityTester);

    pipe_link(si,alloc_and_slice(proxy_king,proxy_non_king));

    pipe_link(proxy_king,king_tester);
    link_to_branch(king_tester,king_branch);

    pipe_link(proxy_non_king,non_king_tester);
    link_to_branch(non_king_tester,non_king_branch);

    {
      slice_index const prototype = alloc_legal_defense_counter_slice();
      slice_insertion_insert(king_branch,&prototype,1);
    }

    {
      slice_index const generator = branch_find_slice(STMoveGenerator,
                                                      king_branch,
                                                      stip_traversal_context_intro);
      slice_index const prototype = alloc_capture_counter_slice();

      assert(generator!=no_slice);
      pipe_substitute(generator,alloc_king_move_generator_slice());

      slice_insertion_insert(king_branch,&prototype,1);
    }

    {
      slice_index const generator = branch_find_slice(STMoveGenerator,
                                                      non_king_branch,
                                                      stip_traversal_context_intro);
      assert(generator!=no_slice);
      pipe_substitute(generator,alloc_non_king_move_generator_slice());
    }

    pipe_remove(si);
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Substitute OWU specific immobility testers
 * @param si where to start (entry slice into stipulation)
 */
void owu_replace_immobility_testers(slice_index si)
{
  stip_structure_traversal st;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  stip_structure_traversal_init(&st,0);
  stip_structure_traversal_override_single(&st,
                                           STImmobilityTester,
                                           &substitute_owu_specific_testers);
  stip_traverse_structure(si,&st);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Try to solve in solve_nr_remaining half-moves.
 * @param si slice index
 * @note assigns solve_result the length of solution found and written, i.e.:
 *            previous_move_is_illegal the move just played is illegal
 *            this_move_is_illegal     the move being played is illegal
 *            immobility_on_next_move  the moves just played led to an
 *                                     unintended immobility on the next move
 *            <=n+1 length of shortest solution found (n+1 only if in next
 *                                     branch)
 *            n+2 no solution found in this branch
 *            n+3 no solution found in next branch
 *            (with n denominating solve_nr_remaining)
 */
void owu_immobility_tester_king_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  /* stop counting once we have >0 legal king moves */
  legal_move_count_init(0);

  /* avoid concurrent counts */
  assert(capture_counter_count==0);

  /* stop counting once we have >1 legal king captures */
  capture_counter_interesting = 1;

  solve(slices[si].next1);

  solve_result = (legal_move_counter_count[nbply]==0 && capture_counter_count==1
                  ? previous_move_has_solved
                  : previous_move_has_not_solved);

  /* clean up after ourselves */
  capture_counter_count = 0;

  legal_move_count_fini();

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
