#include "position/move_diff_code.h"

#include <limits.h>

move_diff_type const move_diff_code[square_h8 - square_a1 + 1]=
{
  /* left/right   */        0,   1,   4,   9,  16,  25,  36,  49,
  /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
  /* 1 left  up   */            50,  37,  26,  17,  10,   5,   2,
  /* 1 right up   */        1,   2,   5,  10,  17,  26,  37,  50,
  /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
  /* 2 left  up   */            53,  40,  29,  20,  13,   8,   5,
  /* 2 right up   */        4,   5,   8,  13,  20,  29,  40,  53,
  /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
  /* 3 left  up   */            58,  45,  34,  25,  18,  13,  10,
  /* 3 right up   */        9,  10,  13,  18,  25,  34,  45,  58,
  /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
  /* 4 left  up   */            65,  52,  41,  32,  25,  20,  17,
  /* 4 right up   */       16,  17,  20,  25,  32,  41,  52,  65,
  /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
  /* 5 left  up   */            74,  61,  50,  41,  34,  29,  26,
  /* 5 right up   */       25,  26,  29,  34,  41,  50,  61,  74,
  /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
  /* 6 left  up   */            85,  72,  61,  52,  45,  40,  37,
  /* 6 right up   */       36,  37,  40,  45,  52,  61,  72,  85,
  /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
  /* 7 left  up   */            98,  85,  74,  65,  58,  53,  50,
  /* 7 right up   */       49,  50,  53,  58,  65,  74,  85,  98
};
