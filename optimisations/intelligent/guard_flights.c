#include "optimisations/intelligent/guard_flights.h"
#include "pydata.h"
#include "pyint.h"
#include "optimisations/intelligent/count_nr_of_moves.h"
#include "optimisations/intelligent/block_flights.h"
#include "optimisations/intelligent/place_white_king.h"
#include "optimisations/intelligent/intercept_check_from_guard.h"
#include "options/maxsolutions/maxsolutions.h"
#include "platform/maxtime.h"
#include "trace.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* index of guarding piece currently being placed */
unsigned int index_of_guarding_piece;

guard_dir_struct GuardDir[5][maxsquare+4];

static void init_guard_dirs_leaper(PieNam guarder,
                                   square target,
                                   unsigned int start, unsigned int end,
                                   numvec value)
{
  unsigned int i;
  for (i = start; i <= end; ++i)
    GuardDir[guarder-Pawn][target+vec[i]].dir = value;
}

static void init_guard_dirs_rider(PieNam guarder,
                                  square flight,
                                  numvec dir)
{
  square const start = flight+dir;
  if (move_diff_code[abs(king_square[Black]-start)]<=2)
  {
    /* start is a flight, too.
     * GuardDir will be initialised from start in this dir */
  }
  else
  {
    square s;
    for (s = start; e[s]==vide; s += dir)
    {
      GuardDir[guarder-Pawn][s].dir = -dir;
      GuardDir[guarder-Pawn][s].target = flight;
    }
  }
}

static void init_guard_dirs_queen(square black_king_pos)
{
  unsigned int i;

  for (i = vec_queen_start; i<=vec_queen_end; ++i)
  {
    square const flight = black_king_pos+vec[i];
    if (e[flight]==vide)
    {
      unsigned int j;
      for (j = vec_queen_start; j<=vec_queen_end; ++j)
        if (vec[i]!=-vec[j])
          init_guard_dirs_rider(Queen,flight,vec[j]);
    }
  }

  for (i = vec_queen_start; i<=vec_queen_end; ++i)
  {
    square const flight = black_king_pos+vec[i];
    if (e[flight]==vide)
      init_guard_dirs_leaper(Queen,
                             flight,
                             vec_queen_start,vec_queen_end,
                             guard_dir_guard_uninterceptable);
  }

  init_guard_dirs_leaper(Queen,
                         black_king_pos,
                         vec_queen_start,vec_queen_end,
                         guard_dir_check_uninterceptable);
}

static void init_guard_dirs_rook(square black_king_pos)
{
  unsigned int i;

  for (i = vec_queen_start; i<=vec_queen_end; ++i)
  {
    square const flight = black_king_pos+vec[i];
    if (e[flight]==vide)
    {
      unsigned int j;
      for (j = vec_rook_start; j<=vec_rook_end; ++j)
        if (vec[i]!=-vec[j])
          init_guard_dirs_rider(Rook,flight,vec[j]);
    }
  }

  for (i = vec_queen_start; i<=vec_queen_end; ++i)
  {
    square const flight = black_king_pos+vec[i];
    if (e[flight]==vide)
      init_guard_dirs_leaper(Rook,
                             flight,
                             vec_rook_start,vec_rook_end,
                             guard_dir_guard_uninterceptable);
  }

  init_guard_dirs_leaper(Rook,
                         black_king_pos,
                         vec_rook_start,vec_rook_end,
                         guard_dir_check_uninterceptable);
}

static void init_guard_dirs_bishop(square black_king_pos)
{
  unsigned int i;

  for (i = vec_queen_start; i<=vec_queen_end; ++i)
  {
    square const flight = black_king_pos+vec[i];
    if (e[flight]==vide)
    {
      unsigned int j;
      for (j = vec_bishop_start; j<=vec_bishop_end; ++j)
        if (vec[i]!=-vec[j])
          init_guard_dirs_rider(Bishop,flight,vec[j]);
    }
  }

  for (i = vec_queen_start; i<=vec_queen_end; ++i)
  {
    square const flight = black_king_pos+vec[i];
    if (e[flight]==vide)
      init_guard_dirs_leaper(Bishop,
                             flight,
                             vec_bishop_start,vec_bishop_end,
                             guard_dir_guard_uninterceptable);
  }

  init_guard_dirs_leaper(Bishop,
                         black_king_pos,
                         vec_bishop_start,vec_bishop_end,
                         guard_dir_check_uninterceptable);
}

static void init_guard_dirs_knight(square black_king_pos)
{
  unsigned int i;

  for (i = vec_queen_start; i<=vec_queen_end; ++i)
  {
    square const flight = black_king_pos+vec[i];
    if (e[flight]==vide)
      init_guard_dirs_leaper(Knight,
                             flight,
                             vec_knight_start,vec_knight_end,
                             guard_dir_guard_uninterceptable);
  }

  /* only now - check trumps guard */
  init_guard_dirs_leaper(Knight,
                         black_king_pos,
                         vec_knight_start,vec_knight_end,
                         guard_dir_check_uninterceptable);
}

static void init_guard_dir_pawn(square flight, numvec dir)
{
  GuardDir[Pawn-Pawn][flight+dir_down+dir_left].dir = dir;
  GuardDir[Pawn-Pawn][flight+dir_down+dir_left].target = flight;
  GuardDir[Pawn-Pawn][flight+dir_down+dir_right].dir = dir;
  GuardDir[Pawn-Pawn][flight+dir_down+dir_right].target = flight;
}

static void init_guard_dirs_pawn(square black_king_pos)
{
  unsigned int i;
  for (i = vec_queen_start; i<=vec_queen_end; ++i)
  {
    square const flight = black_king_pos+vec[i];
    if (e[flight]==vide)
      init_guard_dir_pawn(flight,guard_dir_guard_uninterceptable);
  }

  init_guard_dir_pawn(black_king_pos,guard_dir_check_uninterceptable);
}

/* Initialise the internal structures for fast detection of guards by newly
 * placed white pieces
 * @param black_king_pos position of black king
 */
void init_guard_dirs(square black_king_pos)
{
  memset(GuardDir, 0, sizeof GuardDir);
  init_guard_dirs_queen(black_king_pos);
  init_guard_dirs_rook(black_king_pos);
  init_guard_dirs_bishop(black_king_pos);
  init_guard_dirs_knight(black_king_pos);
  init_guard_dirs_pawn(black_king_pos);
}

/* Does the white king guard a flight
 * @param from where might the king guard?
 * @return true iff the king guards a flight
 */
static boolean white_king_guards_flight(square from)
{
  int const diff = move_diff_code[abs(king_square[Black]-from)];
  boolean const result = diff>3 && diff<=8;

  TraceFunctionEntry(__func__);
  TraceSquare(from);
  TraceFunctionParamListEnd();

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* find out whether to remember to keep a guard line open, and do it
 * @param from start of line
 * @param to end of line
 * @param type type of rider
 * @param delta [+-1] remember resp. forget to keep the line open
 */
static void remember_to_keep_guard_line_open(square from, square to,
                                             int delta)
{
  int const dir = CheckDir[Queen][to-from];
  square s;

  TraceFunctionEntry(__func__);
  TraceSquare(from);
  TraceSquare(to);
  TraceFunctionParam("%d",delta);
  TraceFunctionParamListEnd();

  TraceValue("%d\n",dir);

  /* the guard line only needs to be kept open up to the flight closest to
   * from; e.g. reset to to c1 with from:a1 to:e1 king_square[Black]:d2
   */
  for (s = to-dir; s!=from && move_diff_code[abs(king_square[Black]-s)]<=2; s -= dir)
    to = s;

  remember_to_keep_rider_line_open(from,to,dir,delta);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* fix the white king on its diagram square
 */
static void fix_white_king_on_diagram_square(void)
{
  square const king_diagram_square = white[index_of_king].diagram_square;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  if (e[king_diagram_square]==vide
      && nr_reasons_for_staying_empty[king_diagram_square]==0)
  {
    white[index_of_king].usage = piece_is_fixed_to_diagram_square;
    intelligent_place_white_king(king_diagram_square,
                                 &intelligent_find_and_block_flights);
    white[index_of_king].usage = piece_is_unused;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* continue after guarding
 */
static void guarding_done(void)
{
  unsigned int const save_index_of_guarding_piece = index_of_guarding_piece;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  index_of_guarding_piece = UINT_MAX;

  if (white[index_of_king].usage==piece_is_unused
      && white[index_of_king].diagram_square!=square_e1
      && intelligent_get_nr_remaining_moves(White)==0)
    fix_white_king_on_diagram_square();
  else
    intelligent_find_and_block_flights();

  index_of_guarding_piece = save_index_of_guarding_piece;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* place a (promoted or original) queen in opposition to the black king
 * @param guard_from from what square should the queen guard
 */
static void place_queen_opposition(square guard_from)
{
  square const to_be_intercepted = (king_square[Black]+guard_from)/2;

  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  SetPiece(db,guard_from,white[index_of_guarding_piece].flags);
  if (e[to_be_intercepted]==vide)
  {
    assert(nr_reasons_for_staying_empty[to_be_intercepted]==0);
    intercept_check_on_guarded_square(to_be_intercepted);
    intelligent_intercept_orthogonal_check_by_pin(to_be_intercepted);
    e[to_be_intercepted]= vide;
    spec[to_be_intercepted]= EmptySpec;
  }
  else
    /* there is already a guard between queen and king */
    intelligent_continue_guarding_flights();

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* place a (promoted or original) rider
 * @param rider_type type of rider
 * @param guard_from from what square should the rider guard
 */
static void place_rider(piece rider_type, square guard_from)
{
  TraceFunctionEntry(__func__);
  TracePiece(rider_type);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  {
    int const dir = GuardDir[rider_type-Pawn][guard_from].dir;
    switch (dir)
    {
      case guard_dir_check_uninterceptable:
      case 0:
        break;

      case guard_dir_guard_uninterceptable:
      {
        square const guarded = GuardDir[rider_type-Pawn][guard_from].target;
        if (e[guarded]>=vide)
        {
          SetPiece(rider_type,guard_from,white[index_of_guarding_piece].flags);
          if (CheckDir[rider_type][king_square[Black]-guard_from]!=0
              && e[guarded]==vide)
          {
            assert(nr_reasons_for_staying_empty[guarded]==0);
            intercept_check_on_guarded_square(guarded);
          }
          else
            intelligent_continue_guarding_flights();
        }
        break;
      }

      default:
      {
        square const guarded = GuardDir[rider_type-Pawn][guard_from].target;
        if (e[guarded]>=vide && is_line_empty(guard_from,guarded,dir))
        {
          SetPiece(rider_type,guard_from,white[index_of_guarding_piece].flags);
          remember_to_keep_guard_line_open(guard_from,guarded,+1);
          if (CheckDir[rider_type][king_square[Black]-guard_from]!=0
              && e[guarded]==vide)
          {
            assert(nr_reasons_for_staying_empty[guarded]==0);
            intercept_check_on_guarded_square(guarded);
          }
          else
            intelligent_continue_guarding_flights();
          remember_to_keep_guard_line_open(guard_from,guarded,-1);
        }
        break;
      }
    }
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* place a (promoted or original) knight
 * @param guard_from from what square should the knight guard
 */
static void place_knight(square guard_from)
{
  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  if (GuardDir[Knight-Pawn][guard_from].dir==guard_dir_guard_uninterceptable)
  {
    SetPiece(cb,guard_from,white[index_of_guarding_piece].flags);
    intelligent_continue_guarding_flights();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard king flights with an unpromoted pawn
 * @param guard_from from what square should the pawn guard
 */
static void unpromoted_pawn(square guard_from)
{
  Flags const pawn_flags = white[index_of_guarding_piece].flags;
  square const starts_from = white[index_of_guarding_piece].diagram_square;

  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  if (square_a2<=guard_from && guard_from<=square_h7
      && GuardDir[Pawn-Pawn][guard_from].dir==guard_dir_guard_uninterceptable
      && intelligent_reserve_white_pawn_moves_from_to_no_promotion(starts_from,
                                                                   guard_from))
  {
    SetPiece(pb,guard_from,pawn_flags);
    intelligent_continue_guarding_flights();
    e[guard_from] = vide;
    spec[guard_from] = EmptySpec;
    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard king flights with a promoted queen
 * @param guard_from from what square should the queen guard
 */
static void promoted_queen(square guard_from)
{
  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  switch (move_diff_code[abs(king_square[Black]-guard_from)])
  {
    case 1+0: /* e.g. Ka2 Qb2 */
    case 1+1: /* e.g. Ka2 Qb3 */
      /* uninterceptable check */
      break;

    case 4+0: /* e.g. Ka2 Qc2 */
      if (intelligent_reserve_promoting_white_pawn_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                                 db,
                                                                 guard_from))
      {
        place_queen_opposition(guard_from);
        intelligent_unreserve();
      }
      break;

    case 4+1: /* e.g. Ka2 Qc3 - no lines to be kept open */
    case 9+1: /* e.g. Ka2 Qd3 - 2 guard lines, only 1 needs to remain open */
      if (intelligent_reserve_promoting_white_pawn_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                                 db,
                                                                 guard_from))
      {
        SetPiece(db,guard_from,white[index_of_guarding_piece].flags);
        intelligent_continue_guarding_flights();
        intelligent_unreserve();
      }
      break;

    default:
      if (intelligent_reserve_promoting_white_pawn_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                                 db,
                                                                 guard_from))
      {
        place_rider(db,guard_from);
        intelligent_unreserve();
      }
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard king flights with a rook
 * @param guard_from from what square should the rook guard
 */
static void promoted_rook(square guard_from)
{
  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  switch (move_diff_code[abs(king_square[Black]-guard_from)])
  {
    case 1+0: /* e.g. Ka2 Rb2 */
      /* uninterceptable check */
      break;

    case 1+1: /* e.g. Kc2 Rd3 - 2 guard lines, only 1 needs to remain open */
      if (intelligent_reserve_promoting_white_pawn_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                                 tb,
                                                                 guard_from))
      {
        SetPiece(tb,guard_from,white[index_of_guarding_piece].flags);
        intelligent_continue_guarding_flights();
        intelligent_unreserve();
      }
      break;

    default:
      if (intelligent_reserve_promoting_white_pawn_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                                 tb,
                                                                 guard_from))
      {
        place_rider(tb,guard_from);
        intelligent_unreserve();
      }
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard king flights with a promoted bishop
 * @param guard_from from what square should the bishop guard
 */
static void promoted_bishop(square guard_from)
{
  int const diff = move_diff_code[abs(king_square[Black]-guard_from)];

  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  if (diff==1+1) /* e.g. Ka2 Bb3 */
  {
    /* uninterceptable check */
  }
  else if (intelligent_reserve_promoting_white_pawn_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                                  fb,
                                                                  guard_from))
  {
    if (diff<=2+2)
    {
      SetPiece(fb,guard_from,white[index_of_guarding_piece].flags);
      intelligent_continue_guarding_flights();
    }
    else
      place_rider(fb,guard_from);

    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard king flights with a promoted knight
 * @param guard_from from what square should the knight guard
 */
static void promoted_knight(square guard_from)
{
  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  if (CheckDir[Knight][king_square[Black]-guard_from]==0
      && intelligent_reserve_promoting_white_pawn_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                                cb,
                                                                guard_from))
  {
    place_knight(guard_from);
    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard king flights with a promoted pawn
 * @param guard_from from what square should the promotee guard
 */
static void promoted_pawn(square guard_from)
{
  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  if (intelligent_can_promoted_white_pawn_theoretically_move_to(index_of_guarding_piece,
                                                                guard_from))
  {
    piece pp;
    for (pp = getprompiece[vide]; pp!=vide; pp = getprompiece[pp])
      switch (pp)
      {
        case db:
          promoted_queen(guard_from);
          break;

        case tb:
          promoted_rook(guard_from);
          break;

        case fb:
          promoted_bishop(guard_from);
          break;

        case cb:
          promoted_knight(guard_from);
          break;

        default:
          assert(0);
          break;
      }
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard king flights with a queen
 * @param guard_from from what square should the queen guard
 */
static void queen(square guard_from)
{
  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  switch (move_diff_code[abs(king_square[Black]-guard_from)])
  {
    case 1+0: /* e.g. Ka2 Qb2 */
    case 1+1: /* e.g. Ka2 Qb3 */
      /* uninterceptable check */
      break;

    case 4+0: /* e.g. Ka2 Qc2 */
      if (intelligent_reserve_officer_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                    db,
                                                    guard_from))
      {
        place_queen_opposition(guard_from);
        intelligent_unreserve();
      }
      break;

    case 4+1: /* e.g. Ka2 Qc3 - no lines to be kept open */
    case 9+1: /* e.g. Ka2 Qd3 - 2 guard lines, only 1 needs to remain open */
      if (intelligent_reserve_officer_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                    db,
                                                    guard_from))
      {
        SetPiece(db,guard_from,white[index_of_guarding_piece].flags);
        intelligent_continue_guarding_flights();
        intelligent_unreserve();
      }
      break;

    default:
      if (intelligent_reserve_officer_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                    db,
                                                    guard_from))
      {
        place_rider(db,guard_from);
        intelligent_unreserve();
      }
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard king flights with a rook
 * @param guard_from from what square should the rook guard
 */
static void rook(square guard_from)
{
  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  switch (move_diff_code[abs(king_square[Black]-guard_from)])
  {
    case 1+0: /* e.g. Ka2 Rb2 */
      /* uninterceptable check */
      break;

    case 1+1: /* e.g. Kc2 Rd3 - 2 guard lines, only 1 needs to remain open */
      if (intelligent_reserve_officer_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                    tb,
                                                    guard_from))
      {
        SetPiece(tb,guard_from,white[index_of_guarding_piece].flags);
        intelligent_continue_guarding_flights();
        intelligent_unreserve();
      }
      break;

    default:
      if (intelligent_reserve_officer_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                    tb,
                                                    guard_from))
      {
        place_rider(tb,guard_from);
        intelligent_unreserve();
      }
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard king flights with a bishop
 * @param guard_from from what square should the bishop guard
 */
static void bishop(square guard_from)
{
  int const diff = move_diff_code[abs(king_square[Black]-guard_from)];

  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  if (diff==1+1) /* e.g. Ka2 Bb3 */
  {
    /* uninterceptable check */
  }
  else if (intelligent_reserve_officer_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                     fb,
                                                     guard_from))
  {
    if (diff<=2+2)
    {
      SetPiece(fb,guard_from,white[index_of_guarding_piece].flags);
      intelligent_continue_guarding_flights();
    }
    else
      place_rider(fb,guard_from);

    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard king flights with a white knight
 * @param guard_from from what square should the knight guard
 */
static void knight(square guard_from)
{
  TraceFunctionEntry(__func__);
  TraceSquare(guard_from);
  TraceFunctionParamListEnd();

  if (CheckDir[Knight][king_square[Black]-guard_from]==0
      && intelligent_reserve_officer_moves_from_to(white[index_of_guarding_piece].diagram_square,
                                                   cb,
                                                   guard_from))
  {
    place_knight(guard_from);
    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void guard_next_flight(void)
{
  square const *bnp;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  for (bnp = boardnum; *bnp!=initsquare; bnp++)
    if (e[*bnp]==vide && nr_reasons_for_staying_empty[*bnp]==0)
    {
      switch (white[index_of_guarding_piece].type)
      {
        case db:
          queen(*bnp);
          break;

        case tb:
          rook(*bnp);
          break;

        case fb:
          bishop(*bnp);
          break;

        case cb:
          knight(*bnp);
          break;

        case pb:
          unpromoted_pawn(*bnp);
          promoted_pawn(*bnp);
          break;

        default:
          assert(0);
          break;
      }

      e[*bnp] = vide;
      spec[*bnp] = EmptySpec;
    }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard more king flights */
void intelligent_continue_guarding_flights(void)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  if (!max_nr_solutions_found_in_phase() && !hasMaxtimeElapsed())
  {
    if (intelligent_reserve_masses(White,1))
    {
      unsigned int const save_index_guarder = index_of_guarding_piece;

      for (++index_of_guarding_piece; index_of_guarding_piece<MaxPiece[White]; ++index_of_guarding_piece)
        if (white[index_of_guarding_piece].usage==piece_is_unused)
        {
          white[index_of_guarding_piece].usage = piece_guards;
          guard_next_flight();
          white[index_of_guarding_piece].usage = piece_is_unused;
        }

      index_of_guarding_piece = save_index_guarder;

      intelligent_unreserve();
    }

    guarding_done();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard king flights with the white king */
static void king()
{
  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  if (white[index_of_king].usage==piece_is_unused)
  {
    square const comes_from = white[index_of_king].diagram_square;
    square const *bnp;

    white[index_of_king].usage = piece_guards;

    for (bnp = boardnum; *bnp!=initsquare; ++bnp)
      if (e[*bnp]==vide
          && nr_reasons_for_staying_empty[*bnp]==0
          && white_king_guards_flight(*bnp)
          && intelligent_reserve_white_king_moves_from_to(comes_from,*bnp))
      {
        king_square[White]= *bnp;
        SetPiece(roib,*bnp,white[index_of_king].flags);
        intelligent_continue_guarding_flights();
        e[*bnp] = vide;
        spec[*bnp] = EmptySpec;
        intelligent_unreserve();
      }

    king_square[White] = initsquare;
    white[index_of_king].usage = piece_is_unused;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* guard the king flights */
void intelligent_guard_flights(void)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  if (!max_nr_solutions_found_in_phase()
      && !hasMaxtimeElapsed())
  {
    king();

    TraceText("try not using white king for guarding\n");
    intelligent_continue_guarding_flights();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}