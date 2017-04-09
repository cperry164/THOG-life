/*
 * Copyright (c) 2006-2010 by Tensilica Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the.
 * copyrighted works and confidential proprietary information of Tensilica Inc..
 * They may not be modified, copied, reproduced, distributed, or disclosed to.
 * third parties in any manner, medium, or form, in whole or in part, without.
 * the prior written consent of Tensilica Inc..
 */

/* Do not modify. This is automatically generated.*/

#ifndef CSTUB_first_tie_HEADER
#define CSTUB_first_tie_HEADER

#include <string.h>

#if defined(__GNUC__)
#define CSTUB_MSC_ALIGN(x) 
#define CSTUB_GCC_ALIGN(x) __attribute__((aligned(x))) 
#define CSTUB_EXPLICIT explicit
#define CSTUB_MAY_ALIAS __attribute__((__may_alias__)) 
#define CSTUB_EXTERN extern
#elif defined(_MSC_VER)
#define CSTUB_MSC_ALIGN(x) _declspec(align(x)) 
#define CSTUB_GCC_ALIGN(x) 
#define CSTUB_EXPLICIT explicit
#define CSTUB_MAY_ALIAS 
#if defined(CSTUB_DLLEXPORT)
#define CSTUB_EXTERN __declspec(dllexport)
#elif defined(CSTUB_DLLIMPORT)
#define CSTUB_EXTERN __declspec(dllimport)
#else
#define CSTUB_EXTERN extern
#endif
#else 
#error "Error: Only GCC/G++ and Visual C++ are supported"
#endif

#define CSTUB_(X) cstub_Xm_tie_dev1_##X

/* Ctypes convertion tables */

#define CSTUB_NUM_CTYPES   18

/* ========================================================= */
/* ctype_loadi (PtoV, Addr, Offset) :
     -- PtoV is a pointer to register-layout value stored in memory.
     -- Addr + Offset provaides an address of memory-layout value.

   The semantics is *PtoV = *(Addr + Offset).
*/

typedef void (*cstub_ftype_loadi)(unsigned int *, unsigned int, unsigned int);

extern cstub_ftype_loadi cstub_loadi_function (unsigned int);
/* ========================================================= */

/* For STOREI, arguments vary depending on the CTYPE length :

   sizeof(CTYPE) <= 32 : args (Value to reg from, Addr, Offset),
   sizeof(CTYPE) >  32 : args (pointer to the Value to read from, Addr, Offset).

   Value is a register-layout value stored in memory or passed directly.
   the semantics is *(Addr + Offset) = Value and *(Addr + Offset) = *pValue;
*/

/* sizeof(CTYPE) <= 32.  */
typedef void (*cstub_one_word_storei)(unsigned int, unsigned int, unsigned int);

/* sizeof(CTYPE) > 32.  */
typedef void (*cstub_multi_word_storei)(unsigned int*, unsigned int, unsigned int);

typedef union {
  cstub_one_word_storei   f_one;
  cstub_multi_word_storei f_many;
} cstub_ftype_storei;

extern cstub_ftype_storei cstub_storei_function (unsigned int);
/* ========================================================== */

/* Arguments for RTOR CTYPE functions:
   ( address of register-layout value in memory to write to,
     address of register-layout value in memory to read from, in case
     CTYPE takes more than one word, and the value otherwise ).  */

/* When CTYPE to convert from takes one word use this profile.  */
typedef void (*cstub_one_word_rtor)(unsigned int *, unsigned int);

/* When CTYPE to convert from takes multi-words use this profile.  */
typedef void (*cstub_multi_word_rtor)(unsigned int *, unsigned int *);

typedef union {
  cstub_one_word_rtor   f_one;
  cstub_multi_word_rtor f_many;
} cstub_ftype_rtor;

/* For RTOR functions, we won't have a table. Instead, we will provide
   a function with Two arguments, which are CTYPEs IDs (numbers).  */

/* Returns a pointer to RTOR function for Two CTYPEs, or XTENSA_UNDEFINED.  */
extern cstub_ftype_rtor cstub_rtor_function (unsigned int, unsigned int);
/* ========================================================== */

/* cstub_ctype_size_function (ctype_id) > 0, if CTYPE is a multi-word type.  */ 

extern unsigned char cstub_ctype_size_function (unsigned int);
/* ========================================================== */

/* Initializes CSTUB CTYPEs tables. Must be called after loading library.  */
extern void cstub_ctypes_init ();


#endif /* !CSTUB_first_tie_HEADER */
