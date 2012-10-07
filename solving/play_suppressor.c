#include "solving/play_suppressor.h"
#include "stipulation/pipe.h"
#include "stipulation/battle_play/branch.h"
#include "debugging/trace.h"

/* Allocate a STPlaySuppressor defender slice.
 * @return index of allocated slice
 */
slice_index alloc_play_suppressor_slice(void)
{
  slice_index result;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  result = alloc_pipe(STPlaySuppressor);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            slack_length-2 the move just played or being played is illegal
 *            <=n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type play_suppressor_solve(slice_index si, stip_length_type n)
{
  stip_length_type result;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParamListEnd();

  result = n;

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

static void filter_output_mode(slice_index si, stip_structure_traversal *st)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  if (slices[si].u.output_mode_selector.mode==output_mode_tree)
    stip_traverse_structure_children_pipe(si,st);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void insert_play_suppressors(slice_index si,
                                    stip_structure_traversal *st)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  if (st->context==stip_traversal_context_defense)
  {
    slice_index const prototype = alloc_play_suppressor_slice();
    defense_branch_insert_slices(si,&prototype,1);
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static structure_traversers_visitor const play_suppressors_inserters[] =
{
  { STOutputModeSelector, &filter_output_mode      },
  { STNotEndOfBranch,     &insert_play_suppressors }
};

enum
{
  nr_play_suppressors_inserters = sizeof play_suppressors_inserters / sizeof play_suppressors_inserters[0]
};

/* Instrument the stipulation representation so that postkey play is suppressed
 * @param si identifies slice where to start
 */
void stip_insert_play_suppressors(slice_index si)
{
  stip_structure_traversal st;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  stip_structure_traversal_init(&st,0);
  stip_structure_traversal_override_by_function(&st,
                                                slice_function_binary,
                                                &stip_traverse_structure_children_pipe);
  stip_structure_traversal_override(&st,
                                    play_suppressors_inserters,
                                    nr_play_suppressors_inserters);
  stip_traverse_structure(si,&st);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
