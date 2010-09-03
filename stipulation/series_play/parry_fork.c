#include "stipulation/series_play/parry_fork.h"
#include "pydata.h"
#include "pyproc.h"
#include "stipulation/series_play/play.h"
#include "stipulation/branch.h"
#include "stipulation/proxy.h"
#include "pypipe.h"
#include "trace.h"

#include <assert.h>

/* Determine and write the solution(s)
 * @param si slice index of slice being solved
 * @param n exact number of half moves until end state has to be reached
 * @return length of solution found, i.e.:
 *         n+2 the move leading to the current position has turned out
 *             to be illegal
 *         n+1 no solution found
 *         n   solution found
 */
stip_length_type parry_fork_solve_in_n(slice_index si, stip_length_type n)
{
  stip_length_type result;
  Side const side_at_move = slices[si].starter;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParamListEnd();

  if (echecc(nbply,side_at_move))
    result = series_solve_in_n(slices[si].u.parry_fork.parrying,n+1)-1;
  else
    result = series_solve_in_n(slices[si].u.parry_fork.next,n);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Determine whether the slice has a solution in n half moves.
 * @param si slice index of slice being solved
 * @param n number of half moves until end state has to be reached
 * @return length of solution found, i.e.:
 *         n+2 the move leading to the current position has turned out
 *             to be illegal
 *         n+1 no solution found
 *         n   solution found
 */
stip_length_type parry_fork_has_solution_in_n(slice_index si,
                                              stip_length_type n)
{
  stip_length_type result;
  Side const side_at_move = slices[si].starter;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParamListEnd();

  if (echecc(nbply,side_at_move))
    result = series_has_solution_in_n(slices[si].u.parry_fork.parrying,n+1)-1;
  else
    result = series_has_solution_in_n(slices[si].u.parry_fork.next,n);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Allocate a STParryFork slice.
 * @param parrying identifies slice responsible for parrying
 * @return allocated slice
 */
static slice_index alloc_parry_fork(stip_length_type length,
                                    stip_length_type min_length,
                                    slice_index parrying)
{
  slice_index result;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",length);
  TraceFunctionParam("%u",min_length);
  TraceFunctionParam("%u",parrying);
  TraceFunctionParamListEnd();

  result = alloc_branch(STParryFork,length,min_length); 
  slices[result].u.parry_fork.parrying = parrying;

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Convert a series branch to a parry series branch
 * @param si identifies first slice of the series branch
 * @param parrying identifies slice responsible for parrying
 */
void convert_to_parry_series_branch(slice_index si, slice_index parrying)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",parrying);
  TraceFunctionParamListEnd();

  TraceStipulation(si);

  {
    slice_index const inverter = branch_find_slice(STMoveInverterSeriesFilter,
                                                   si);
    slice_index const dealt = branch_find_slice(STSeriesMoveDealtWith,
                                                inverter);
    stip_length_type const length = slices[dealt].u.branch.length;
    stip_length_type const min_length = slices[dealt].u.branch.min_length;
    slice_index const parry_fork = alloc_parry_fork(length,min_length,
                                                    parrying);

    assert(inverter!=no_slice);
    assert(dealt!=no_slice);

    pipe_append(slices[inverter].prev,parry_fork);
    pipe_link(inverter,dealt);
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Traverse a subtree
 * @param branch root slice of subtree
 * @param st address of structure defining traversal
 */
void stip_traverse_structure_parry_fork(slice_index branch,
                                        stip_structure_traversal *st)
{
  slice_index const parrying = slices[branch].u.parry_fork.parrying;
  stip_traverse_structure_pipe(branch,st);
  stip_traverse_structure(parrying,st);
}
