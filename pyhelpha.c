#include "pyhelpha.h"
#include "pybrah.h"
#include "pyhash.h"
#include "trace.h"

#include <assert.h>

/* Allocate a STHelpHashed slice for a STBranchHelp slice and insert
 * it at the STBranchHelp slice's position. 
 * The STHelpHashed takes the place of the STBranchHelp slice.
 * @param si identifies STBranchHelp slice
 */
void insert_help_hashed_slice(slice_index si)
{
  slice_index copy;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  assert(slices[si].type!=STHelpHashed);
  TraceValue("%u\n",slices[si].type);

  copy = copy_slice(si);

  slices[si].type = STHelpHashed;
  slices[si].u.pipe.next = copy;
  
  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Solve in a number of half-moves
 * @param si identifies slice
 * @param n number of half moves until end state has to be reached
 * @param side_at_move side at the move
 * @return true iff >=1 solution was found
 */
boolean help_hashed_solve_in_n(slice_index si,
                               stip_length_type n,
                               Side side_at_move)
{
  boolean result;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParam("%u",side_at_move);
  TraceFunctionParamListEnd();

  assert(n>slack_length_help);

  if (inhash(si,HelpNoSucc,n/2))
    result = false;
  else if (help_solve_in_n(slices[si].u.pipe.next,n,side_at_move))
    result = true;
  else
  {
    result = false;
    addtohash(si,HelpNoSucc,n/2);
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Determine whether there is a solution in n half moves.
 * @param si slice index of slice being solved
 * @param n number of half moves until end state has to be reached
 * @param side_at_move side at move
 * @return true iff >= 1 solution has been found
 */
boolean help_hashed_has_solution_in_n(slice_index si,
                                      stip_length_type n,
                                      Side side_at_move)
{
  boolean result;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",side_at_move);
  TraceFunctionParam("%u",n);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  assert(n>slack_length_help);

  if (inhash(si,HelpNoSucc,n/2))
    result = false;
  else
  {
    if (help_has_solution_in_n(slices[si].u.pipe.next,n,side_at_move))
      result = true;
    else
    {
      addtohash(si,HelpNoSucc,n/2);
      result = false;
    }
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Determine and write solution(s): add first moves to table (as
 * threats for the parent slice. First consult hash table.
 * @param continuations table where to add first moves
 * @param si slice index of slice being solved
 * @param n number of half moves until end state has to be reached
 * @param side_at_move side at move
 */
void help_hashed_solve_continuations_in_n(table continuations,
                                          slice_index si,
                                          stip_length_type n,
                                          Side side_at_move)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParam("%u",side_at_move);
  TraceFunctionParamListEnd();

  assert(n>slack_length_help);

  if (!inhash(si,HelpNoSucc,n/2))
  {
    slice_index const next = slices[si].u.pipe.next;
    help_solve_continuations_in_n(continuations,next,n,side_at_move);
    if (table_length(continuations)==0)
      addtohash(si,HelpNoSucc,n/2);
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Is there no chance left for reaching the solution?
 * E.g. did the help side just allow a mate in 1 in a hr#N?
 * Tests may rely on the current position being hash-encoded.
 * @param si slice index
 * @param just_moved side that has just moved
 * @return true iff no chance is left
 */
boolean help_hashed_must_starter_resign_hashed(slice_index si, Side just_moved)
{
  boolean result;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",just_moved);
  TraceFunctionParamListEnd();

  result = slice_must_starter_resign_hashed(slices[si].u.pipe.next,
                                            advers(just_moved));

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}
