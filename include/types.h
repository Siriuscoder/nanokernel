/*  This file is part of simple kernel.
 Project NanoKernel (for study purposes only)
 Copyright (C) 2013  Sirius (Vdov Nikita)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TYPES_H
#define TYPES_H

/* 7.8.1.1 Exact-width integer types */

#ifdef __INT8_TYPE__
typedef __INT8_TYPE__ int8_t;
#else
typedef char int8_t;
#endif

#ifdef __INT16_TYPE__
typedef __INT16_TYPE__ int16_t;
#else
typedef short int int16_t;
#endif

#ifdef __INT32_TYPE__
typedef __INT32_TYPE__ int32_t;
#else
typedef int int32_t;
#endif

#ifdef __INT64_TYPE__
typedef __INT64_TYPE__ int64_t;
#else
typedef long long int64_t;
#endif

#ifdef __UINT8_TYPE__
typedef __UINT8_TYPE__ uint8_t;
#else
typedef unsigned char uint8_t;
#endif

#ifdef __UINT16_TYPE__
typedef __UINT16_TYPE__ uint16_t;
#else
typedef unsigned short int uint16_t;
#endif

#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#else
typedef unsigned int uint32_t;
#endif

#ifdef __UINT64_TYPE__
typedef __UINT64_TYPE__ uint64_t;
#else
typedef unsigned long long uint64_t;
#endif

#ifdef __SIZE_TYPE__
typedef __SIZE_TYPE__ size_t;
#else
typedef unsigned long int size_t;
#endif

#ifdef __INT_LEAST8_TYPE__
/* 7.8.1.2 Minimum-width integer types */
typedef __INT_LEAST8_TYPE__ int_least8_t;
typedef __INT_LEAST16_TYPE__ int_least16_t;
typedef __INT_LEAST32_TYPE__ int_least32_t;
typedef __INT_LEAST64_TYPE__ int_least64_t;
typedef __UINT_LEAST8_TYPE__ uint_least8_t;
typedef __UINT_LEAST16_TYPE__ uint_least16_t;
typedef __UINT_LEAST32_TYPE__ uint_least32_t;
typedef __UINT_LEAST64_TYPE__ uint_least64_t;
#else
typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;
typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;
#endif

#ifdef __INT_FAST8_TYPE__
/* 7.8.1.3 Fastest minimum-width integer types */
typedef __INT_FAST8_TYPE__ int_fast8_t;
typedef __INT_FAST16_TYPE__ int_fast16_t;
typedef __INT_FAST32_TYPE__ int_fast32_t;
typedef __INT_FAST64_TYPE__ int_fast64_t;
typedef __UINT_FAST8_TYPE__ uint_fast8_t;
typedef __UINT_FAST16_TYPE__ uint_fast16_t;
typedef __UINT_FAST32_TYPE__ uint_fast32_t;
typedef __UINT_FAST64_TYPE__ uint_fast64_t;
#else
typedef int8_t int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;
typedef uint8_t uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;
#endif

/* 7.8.1.4 Integer types capable of holding object pointers */

#ifdef __INTPTR_TYPE__
typedef __INTPTR_TYPE__ intptr_t;
#else
typedef int *intptr_t;
#endif

#ifdef __UINTPTR_TYPE__
typedef __UINTPTR_TYPE__ uintptr_t;
#else
typedef unsigned int *uintptr_t;
#endif

typedef void *ptr_t;

/* Type to use for unaligned operations.  */
typedef uint8_t byte;

#define NULL_PTR		((ptr_t)0x0)
#define NULL			NULL_PTR
/* 7.8.1.5 Greatest-width integer types */

typedef __INTMAX_TYPE__ intmax_t;
typedef __UINTMAX_TYPE__ uintmax_t;

#if !defined __cplusplus || defined __STDC_LIMIT_MACROS

/* 7.18.2 Limits of specified-width integer types */

#undef INT8_MAX
#ifdef __INT8_MAX__
# define INT8_MAX __INT8_MAX__
#else
# define INT8_MAX 0x7f
#endif

#undef INT8_MIN
#define INT8_MIN (-INT8_MAX - 1)


#undef UINT8_MAX
#ifdef __UINT8_MAX__
# define UINT8_MAX __UINT8_MAX__
#endif

#undef INT16_MAX
#ifdef __INT16_MAX__
# define INT16_MAX __INT16_MAX__
#else
# define INT16_MAX 0x7fff
#endif

#undef INT16_MIN
#define INT16_MIN (-INT16_MAX - 1)

#undef UINT16_MAX
#ifdef __UINT16_MAX__
# define UINT16_MAX __UINT16_MAX__
#endif

#undef INT32_MAX
#ifdef __INT32_MAX__
# define INT32_MAX __INT32_MAX__
#else
# define INT32_MAX 0x7fffffff
#endif

#undef INT32_MIN
#define INT32_MIN (-INT32_MAX - 1)

#undef UINT32_MAX
#ifdef __UINT32_MAX__
# define UINT32_MAX __UINT32_MAX__
#endif

#undef INT64_MAX
#ifdef __INT64_MAX__
# define INT64_MAX __INT64_MAX__
#else
# define INT64_MAX 0x7fffffffffffffff
#endif

#undef INT64_MIN
#define INT64_MIN (-INT64_MAX - 1)

#undef UINT64_MAX
#ifdef __UINT64_MAX__
# define UINT64_MAX __UINT64_MAX__
#endif


#undef INT_LEAST8_MAX
#ifdef __INT_LEAST8_MAX__
# define INT_LEAST8_MAX __INT_LEAST8_MAX__
#else
# define INT_LEAST8_MAX INT8_MAX
#endif

#undef INT_LEAST8_MIN
#define INT_LEAST8_MIN (-INT_LEAST8_MAX - 1)

#undef UINT_LEAST8_MAX
#ifdef __UINT_LEAST8_MAX__
# define UINT_LEAST8_MAX __UINT_LEAST8_MAX__
#else
# define UINT_LEAST8_MAX UINT8_MAX
#endif

#undef INT_LEAST16_MAX
#ifdef __INT_LEAST16_MAX__
# define INT_LEAST16_MAX __INT_LEAST16_MAX__
#else
# define INT_LEAST16_MAX INT16_MAX
#endif

#undef INT_LEAST16_MIN
#define INT_LEAST16_MIN (-INT_LEAST16_MAX - 1)

#undef UINT_LEAST16_MAX
#ifdef __UINT_LEAST16_MAX__
# define UINT_LEAST16_MAX __UINT_LEAST16_MAX__
#else
# define UINT_LEAST16_MAX UINT16_MAX
#endif

#undef INT_LEAST32_MAX
#ifdef __INT_LEAST32_MAX__
# define INT_LEAST32_MAX __INT_LEAST32_MAX__
#else
# define INT_LEAST32_MAX INT32_MAX
#endif

#undef INT_LEAST32_MIN
#define INT_LEAST32_MIN (-INT_LEAST32_MAX - 1)

#undef UINT_LEAST32_MAX
#ifdef __UINT_LEAST32_MAX__
# define UINT_LEAST32_MAX __UINT_LEAST32_MAX__
#else
# define UINT_LEAST32_MAX UINT32_MAX
#endif

#undef INT_LEAST64_MAX
#ifdef __INT_LEAST64_MAX__
# define INT_LEAST64_MAX __INT_LEAST64_MAX__
#else
# define INT_LEAST64_MAX INT64_MAX
#endif

#undef INT_LEAST64_MIN
#define INT_LEAST64_MIN (-INT_LEAST64_MAX - 1)

#undef UINT_LEAST64_MAX
#ifdef __UINT_LEAST64_MAX__
# define UINT_LEAST64_MAX __UINT_LEAST64_MAX__
#else
# define UINT_LEAST64_MAX UINT64_MAX
#endif

#undef INT_FAST8_MAX
#ifdef __INT_FAST8_MAX__
# define INT_FAST8_MAX __INT_FAST8_MAX__
#else
# define INT_FAST8_MAX INT8_MAX
#endif

#undef INT_FAST8_MIN
#define INT_FAST8_MIN (-INT_FAST8_MAX - 1)

#undef UINT_FAST8_MAX
#ifdef __UINT_FAST8_MAX__
# define UINT_FAST8_MAX __UINT_FAST8_MAX__
#else
# define UINT_FAST8_MAX UINT8_MAX
#endif

#undef INT_FAST16_MAX
#ifdef __INT_FAST16_MAX__
# define INT_FAST16_MAX __INT_FAST16_MAX__
#else
# define INT_FAST16_MAX INT16_MAX
#endif

#undef INT_FAST16_MIN
#define INT_FAST16_MIN (-INT_FAST16_MAX - 1)

#undef UINT_FAST16_MAX
#ifdef __UINT_FAST16_MAX__
# define UINT_FAST16_MAX __UINT_FAST16_MAX__
#else
# define UINT_FAST16_MAX UINT16_MAX
#endif

#undef INT_FAST32_MAX
#ifdef __INT_FAST32_MAX__
# define INT_FAST32_MAX __INT_FAST32_MAX__
#else
# define INT_FAST32_MAX INT8_MAX
#endif

#undef INT_FAST32_MIN
#define INT_FAST32_MIN (-INT_FAST32_MAX - 1)

#undef UINT_FAST32_MAX
#ifdef __UINT_FAST32_MAX__
# define UINT_FAST32_MAX __UINT_FAST32_MAX__
#else
# define UINT_FAST32_MAX UINT32_MAX
#endif

#undef INT_FAST64_MAX
#ifdef __INT_FAST64_MAX__
# define INT_FAST64_MAX __INT_FAST64_MAX__
#else
# define INT_FAST64_MAX INT64_MAX
#endif

#undef INT_FAST64_MIN
#define INT_FAST64_MIN (-INT_FAST64_MAX - 1)

#undef UINT_FAST64_MAX
#ifdef __UINT_FAST64_MAX__
# define UINT_FAST64_MAX __UINT_FAST64_MAX__
#else
# define UINT_FAST64_MAX UINT64_MAX
#endif

#ifdef __INTPTR_MAX__
# undef INTPTR_MAX
# define INTPTR_MAX __INTPTR_MAX__
# undef INTPTR_MIN
# define INTPTR_MIN (-INTPTR_MAX - 1)
#endif
#ifdef __UINTPTR_MAX__
# undef UINTPTR_MAX
# define UINTPTR_MAX __UINTPTR_MAX__
#endif

#undef INTMAX_MAX
#define INTMAX_MAX __INTMAX_MAX__
#undef INTMAX_MIN
#define INTMAX_MIN (-INTMAX_MAX - 1)
#undef UINTMAX_MAX
#define UINTMAX_MAX __UINTMAX_MAX__

/* 7.18.3 Limits of other integer types */

#undef PTRDIFF_MAX
#define PTRDIFF_MAX __PTRDIFF_MAX__
#undef PTRDIFF_MIN
#define PTRDIFF_MIN (-PTRDIFF_MAX - 1)

#undef SIG_ATOMIC_MAX
#define SIG_ATOMIC_MAX __SIG_ATOMIC_MAX__
#undef SIG_ATOMIC_MIN
#define SIG_ATOMIC_MIN __SIG_ATOMIC_MIN__

#undef SIZE_MAX
#define SIZE_MAX __SIZE_MAX__

#undef WCHAR_MAX
#define WCHAR_MAX __WCHAR_MAX__
#undef WCHAR_MIN
#define WCHAR_MIN __WCHAR_MIN__

#undef WINT_MAX
#define WINT_MAX __WINT_MAX__
#undef WINT_MIN
#define WINT_MIN __WINT_MIN__

#endif /* !defined __cplusplus || defined __STDC_LIMIT_MACROS */

#if !defined __cplusplus || defined __STDC_CONSTANT_MACROS

#undef INT8_C
#define INT8_C(c) __INT8_C(c)
#undef INT16_C
#define INT16_C(c) __INT16_C(c)
#undef INT32_C
#define INT32_C(c) __INT32_C(c)
#undef INT64_C
#define INT64_C(c) __INT64_C(c)
#undef UINT8_C
#define UINT8_C(c) __UINT8_C(c)
#undef UINT16_C
#define UINT16_C(c) __UINT16_C(c)
#undef UINT32_C
#define UINT32_C(c) __UINT32_C(c)
#undef UINT64_C
#define UINT64_C(c) __UINT64_C(c)
#undef INTMAX_C
#define INTMAX_C(c) __INTMAX_C(c)
#undef UINTMAX_C
#define UINTMAX_C(c) __UINTMAX_C(c)

#endif /* !defined __cplusplus || defined __STDC_CONSTANT_MACROS */

#if 0
typedef _Bool bool;
#else
/**
 * The is no reason to follow strict ABI in kernel code and to define boolean
 * type as 1-byte _Bool. Make it native word sized.
 *
 * May be I'm not right and it would be better to introduce something like
 * bool_t to avoid confusion with bool defined by C99 standard.
 */
typedef int bool;
#endif

#define false 	0
#define true  	1

/* include args */
/* Allows functions to accept an indefinite number of arguments. */
#include "std/stdarg.h"

#endif
