/* Definitions for the first_tie TIE package */

/*
 * Copyright (c) 2004-2010 by Tensilica Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of Tensilica Inc.
 * They may not be modified, copied, reproduced, distributed, or disclosed to
 * third parties in any manner, medium, or form, in whole or in part, without
 * the prior written consent of Tensilica Inc.
 */

/* Do not modify. This is automatically generated.*/

#ifndef _XTENSA_first_tie_HEADER
#define _XTENSA_first_tie_HEADER

#ifdef __XTENSA__
#ifdef __XCC__

#include <xtensa/tie/xt_core.h>

/*
 * The following prototypes describe intrinsic functions
 * corresponding to TIE instructions.  Some TIE instructions
 * may produce multiple results (designated as "out" operands
 * in the iclass section) or may have operands used as both
 * inputs and outputs (designated as "inout").  However, the C
 * and C++ languages do not provide syntax that can express
 * the in/out/inout constraints of TIE intrinsics.
 * Nevertheless, the compiler understands these constraints
 * and will check that the intrinsic functions are used
 * correctly.  To improve the readability of these prototypes,
 * the "out" and "inout" parameters are marked accordingly
 * with comments.
 */

extern unsigned _TIE_first_tie_RUR_reg_A(void);
extern void _TIE_first_tie_WUR_reg_A(unsigned v);
extern void _TIE_first_tie_tie_add(unsigned in1, unsigned in2, unsigned in3, unsigned in4);
#define RUR_reg_A _TIE_first_tie_RUR_reg_A
#define Rreg_A _TIE_first_tie_RUR_reg_A
#define RUR0 _TIE_first_tie_RUR_reg_A
#define WUR_reg_A _TIE_first_tie_WUR_reg_A
#define Wreg_A _TIE_first_tie_WUR_reg_A
#define WUR0 _TIE_first_tie_WUR_reg_A
#define tie_add _TIE_first_tie_tie_add

#ifndef RUR
#define RUR(NUM) RUR##NUM()
#endif

#ifndef WUR
#define WUR(VAL, NUM) WUR##NUM(VAL)
#endif

#endif /* __XCC__ */

#endif /* __XTENSA__ */

#endif /* !_XTENSA_first_tie_HEADER */
