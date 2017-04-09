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
extern unsigned _TIE_first_tie_RUR_cordic_x(void);
extern void _TIE_first_tie_WUR_cordic_x(unsigned v);
extern unsigned _TIE_first_tie_RUR_cordic_y(void);
extern void _TIE_first_tie_WUR_cordic_y(unsigned v);
extern unsigned _TIE_first_tie_RUR_cordic_z(void);
extern void _TIE_first_tie_WUR_cordic_z(unsigned v);
extern unsigned _TIE_first_tie_RUR_cordic_k(void);
extern void _TIE_first_tie_WUR_cordic_k(unsigned v);
extern unsigned _TIE_first_tie_RUR_mask_out(void);
extern void _TIE_first_tie_WUR_mask_out(unsigned v);
extern unsigned _TIE_first_tie_RUR_ydiv2_out(void);
extern void _TIE_first_tie_WUR_ydiv2_out(unsigned v);
extern unsigned _TIE_first_tie_RUR_xdiv2_out(void);
extern void _TIE_first_tie_WUR_xdiv2_out(unsigned v);
extern unsigned _TIE_first_tie_tie_getbin(unsigned a, unsigned n);
extern void _TIE_first_tie_tie_add(unsigned in1, unsigned in2, unsigned in3, unsigned in4);
extern void _TIE_first_tie_tie_cordic_init(unsigned x, unsigned y);
extern void _TIE_first_tie_tie_cordic_iterate(void);
#define RUR_reg_A _TIE_first_tie_RUR_reg_A
#define Rreg_A _TIE_first_tie_RUR_reg_A
#define RUR0 _TIE_first_tie_RUR_reg_A
#define WUR_reg_A _TIE_first_tie_WUR_reg_A
#define Wreg_A _TIE_first_tie_WUR_reg_A
#define WUR0 _TIE_first_tie_WUR_reg_A
#define RUR_cordic_x _TIE_first_tie_RUR_cordic_x
#define Rcordic_x _TIE_first_tie_RUR_cordic_x
#define RUR1 _TIE_first_tie_RUR_cordic_x
#define WUR_cordic_x _TIE_first_tie_WUR_cordic_x
#define Wcordic_x _TIE_first_tie_WUR_cordic_x
#define WUR1 _TIE_first_tie_WUR_cordic_x
#define RUR_cordic_y _TIE_first_tie_RUR_cordic_y
#define Rcordic_y _TIE_first_tie_RUR_cordic_y
#define RUR2 _TIE_first_tie_RUR_cordic_y
#define WUR_cordic_y _TIE_first_tie_WUR_cordic_y
#define Wcordic_y _TIE_first_tie_WUR_cordic_y
#define WUR2 _TIE_first_tie_WUR_cordic_y
#define RUR_cordic_z _TIE_first_tie_RUR_cordic_z
#define Rcordic_z _TIE_first_tie_RUR_cordic_z
#define RUR3 _TIE_first_tie_RUR_cordic_z
#define WUR_cordic_z _TIE_first_tie_WUR_cordic_z
#define Wcordic_z _TIE_first_tie_WUR_cordic_z
#define WUR3 _TIE_first_tie_WUR_cordic_z
#define RUR_cordic_k _TIE_first_tie_RUR_cordic_k
#define Rcordic_k _TIE_first_tie_RUR_cordic_k
#define RUR4 _TIE_first_tie_RUR_cordic_k
#define WUR_cordic_k _TIE_first_tie_WUR_cordic_k
#define Wcordic_k _TIE_first_tie_WUR_cordic_k
#define WUR4 _TIE_first_tie_WUR_cordic_k
#define RUR_mask_out _TIE_first_tie_RUR_mask_out
#define Rmask_out _TIE_first_tie_RUR_mask_out
#define RUR5 _TIE_first_tie_RUR_mask_out
#define WUR_mask_out _TIE_first_tie_WUR_mask_out
#define Wmask_out _TIE_first_tie_WUR_mask_out
#define WUR5 _TIE_first_tie_WUR_mask_out
#define RUR_ydiv2_out _TIE_first_tie_RUR_ydiv2_out
#define Rydiv2_out _TIE_first_tie_RUR_ydiv2_out
#define RUR6 _TIE_first_tie_RUR_ydiv2_out
#define WUR_ydiv2_out _TIE_first_tie_WUR_ydiv2_out
#define Wydiv2_out _TIE_first_tie_WUR_ydiv2_out
#define WUR6 _TIE_first_tie_WUR_ydiv2_out
#define RUR_xdiv2_out _TIE_first_tie_RUR_xdiv2_out
#define Rxdiv2_out _TIE_first_tie_RUR_xdiv2_out
#define RUR7 _TIE_first_tie_RUR_xdiv2_out
#define WUR_xdiv2_out _TIE_first_tie_WUR_xdiv2_out
#define Wxdiv2_out _TIE_first_tie_WUR_xdiv2_out
#define WUR7 _TIE_first_tie_WUR_xdiv2_out
#define tie_getbin _TIE_first_tie_tie_getbin
#define tie_add _TIE_first_tie_tie_add
#define tie_cordic_init _TIE_first_tie_tie_cordic_init
#define tie_cordic_iterate _TIE_first_tie_tie_cordic_iterate

#ifndef RUR
#define RUR(NUM) RUR##NUM()
#endif

#ifndef WUR
#define WUR(VAL, NUM) WUR##NUM(VAL)
#endif

#endif /* __XCC__ */

#endif /* __XTENSA__ */

#endif /* !_XTENSA_first_tie_HEADER */
