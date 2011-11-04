#include "optimisations/intelligent/mate/pin_black_piece.h"
#include "pyint.h"
#include "pydata.h"
#include "optimisations/intelligent/count_nr_of_moves.h"
#include "optimisations/intelligent/mate/finish.h"
#include "trace.h"

#include <assert.h>

static void by_officer(piece pinner_type,
                       Flags pinner_flags,
                       square pinner_comes_from,
                       square pin_from)
{
  TraceFunctionEntry(__func__);
  TracePiece(pinner_type);
  TraceSquare(pinner_comes_from);
  TraceSquare(pin_from);
  TraceFunctionParamListEnd();

  if (intelligent_reserve_officer_moves_from_to(pinner_comes_from,
                                                pinner_type,
                                                pin_from))
  {
    SetPiece(pinner_type,pin_from,pinner_flags);
    intelligent_mate_test_target_position();
    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void by_promoted_pawn(Flags pinner_flags,
                             square pinner_comes_from,
                             square pin_from,
                             boolean diagonal)
{
  piece const minor_pinner_type = diagonal ? fb : tb;

  TraceFunctionEntry(__func__);
  TraceSquare(pinner_comes_from);
  TraceSquare(pin_from);
  TraceFunctionParam("%u",diagonal);
  TraceFunctionParamListEnd();

  if (intelligent_reserve_promoting_white_pawn_moves_from_to(pinner_comes_from,
                                                             minor_pinner_type,
                                                             pin_from))
  {
    SetPiece(minor_pinner_type,pin_from,pinner_flags);
    intelligent_mate_test_target_position();
    intelligent_unreserve();
  }

  if (intelligent_reserve_promoting_white_pawn_moves_from_to(pinner_comes_from,
                                                             db,
                                                             pin_from))
  {
    SetPiece(db,pin_from,pinner_flags);
    intelligent_mate_test_target_position();
    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void one_pin(square sq_to_be_pinned,
                    square pin_on,
                    unsigned int pinner_index,
                    boolean diagonal)
{
  piece const pinner_type = white[pinner_index].type;
  Flags const pinner_flags = white[pinner_index].flags;
  square const pinner_comes_from = white[pinner_index].diagram_square;

  TraceFunctionEntry(__func__);
  TraceSquare(sq_to_be_pinned);
  TraceSquare(pin_on);
  TraceFunctionParam("%u",pinner_index);
  TraceFunctionParam("%u",diagonal);
  TraceFunctionParamListEnd();

  switch (pinner_type)
  {
    case db:
      by_officer(db,pinner_flags,pinner_comes_from,pin_on);
      break;

    case tb:
      if (!diagonal)
        by_officer(tb,pinner_flags,pinner_comes_from,pin_on);
      break;

    case fb:
      if (diagonal)
        by_officer(fb,pinner_flags,pinner_comes_from,pin_on);
      break;

    case cb:
      break;

    case pb:
      by_promoted_pawn(pinner_flags,pinner_comes_from,pin_on,diagonal);
      break;

    default:
      assert(0);
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

void intelligent_mate_pin_black_piece(square sq_to_be_pinned)
{
  TraceFunctionEntry(__func__);
  TraceSquare(sq_to_be_pinned);
  TraceFunctionParamListEnd();

  if (intelligent_reserve_masses(White,1))
  {
    int const dir = sq_to_be_pinned-king_square[Black];
    boolean const diagonal = SquareCol(king_square[Black]+dir)==SquareCol(king_square[Black]);

    square pin_on;
    for (pin_on = sq_to_be_pinned+dir; e[pin_on]==vide && nr_reasons_for_staying_empty[pin_on]==0; pin_on += dir)
    {
      if (nr_reasons_for_staying_empty[pin_on]==0)
      {
        unsigned int pinner_index;
        for (pinner_index = 1; pinner_index<MaxPiece[White]; ++pinner_index)
          if (white[pinner_index].usage==piece_is_unused)
          {
            white[pinner_index].usage = piece_pins;
            one_pin(sq_to_be_pinned,pin_on,pinner_index,diagonal);
            white[pinner_index].usage = piece_is_unused;
          }

        e[pin_on] = vide;
        spec[pin_on] = EmptySpec;
      }

      ++nr_reasons_for_staying_empty[pin_on];
    }

    for (pin_on -= dir; pin_on!=sq_to_be_pinned; pin_on -= dir)
      --nr_reasons_for_staying_empty[pin_on];

    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
