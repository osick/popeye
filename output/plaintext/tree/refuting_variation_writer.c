#include "output/plaintext/tree/refuting_variation_writer.h"
#include "stipulation/stipulation.h"
#include "pydata.h"
#include "pymsg.h"
#include "stipulation/pipe.h"
#include "stipulation/branch.h"
#include "solving/solve.h"
#include "output/plaintext/move_inversion_counter.h"
#include "debugging/trace.h"

/* Allocate a STRefutingVariationWriter slice.
 * @return index of allocated slice
 */
slice_index alloc_refuting_variation_writer_slice(void)
{
  slice_index result;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  result = alloc_pipe(STRefutingVariationWriter);

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
stip_length_type
refuting_variation_writer_solve(slice_index si, stip_length_type n)
{
  stip_length_type result;
  slice_index const next = slices[si].next1;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParamListEnd();

  result = solve(next,n);

  if (result>n)
  {
    unsigned int const move_depth = nbply+output_plaintext_nr_move_inversions;
    Message(NewLine);
    sprintf(GlobalStr,"%*c",4*move_depth-4,blank);
    StdString(GlobalStr);
    Message(Refutation);
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}
