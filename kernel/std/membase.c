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

#include <std/membase.h>

/* The macros defined in this file are:

 BYTE_COPY_FWD(dst_beg_ptr, src_beg_ptr, nbytes_to_copy)

 BYTE_COPY_BWD(dst_end_ptr, src_end_ptr, nbytes_to_copy)

 WORD_COPY_FWD(dst_beg_ptr, src_beg_ptr, nbytes_remaining, nbytes_to_copy)

 WORD_COPY_BWD(dst_end_ptr, src_end_ptr, nbytes_remaining, nbytes_to_copy)

 MERGE(old_word, sh_1, new_word, sh_2)
 [I fail to understand.  I feel stupid.  --roland]
 */

/* Type to use for aligned memory operations.
 This should normally be the biggest type supported by a single load
 and store.  */
#define	op_t		unsigned long int
#define OPSIZ		(sizeof(op_t))

/* Optimal type for storing bytes in registers.  */
#define	reg_char	char

#define MERGE(w0, sh_1, w1, sh_2) (((w0) >> (sh_1)) | ((w1) << (sh_2)))
/* Copy exactly NBYTES bytes from SRC_BP to DST_BP,
 without any assumptions about alignment of the pointers.  */
#define BYTE_COPY_FWD(dst_bp, src_bp, nbytes)				      \
  do									      \
    {									      \
      size_t __nbytes = (nbytes);					      \
      while (__nbytes > 0)						      \
	{								      \
	  byte __x = ((byte *) src_bp)[0];				      \
	  src_bp += 1;							      \
	  __nbytes -= 1;						      \
	  ((byte *) dst_bp)[0] = __x;					      \
	  dst_bp += 1;							      \
	}								      \
    } while (0)

/* Copy exactly NBYTES_TO_COPY bytes from SRC_END_PTR to DST_END_PTR,
 beginning at the bytes right before the pointers and continuing towards
 smaller addresses.  Don't assume anything about alignment of the
 pointers.  */
#define BYTE_COPY_BWD(dst_ep, src_ep, nbytes)				      \
  do									      \
    {									      \
      size_t __nbytes = (nbytes);					      \
      while (__nbytes > 0)						      \
	{								      \
	  byte __x;							      \
	  src_ep -= 1;							      \
	  __x = ((byte *) src_ep)[0];					      \
	  dst_ep -= 1;							      \
	  __nbytes -= 1;						      \
	  ((byte *) dst_ep)[0] = __x;					      \
	}								      \
    } while (0)

/* Copy *up to* NBYTES bytes from SRC_BP to DST_BP, with
 the assumption that DST_BP is aligned on an OPSIZ multiple.  If
 not all bytes could be easily copied, store remaining number of bytes
 in NBYTES_LEFT, otherwise store 0.  */
static void
_wordcopy_fwd_aligned(long int, long int, size_t);
static void
_wordcopy_fwd_dest_aligned(long int, long int, size_t);
#define WORD_COPY_FWD(dst_bp, src_bp, nbytes_left, nbytes)		      \
  do									      \
    {									      \
      if (src_bp % OPSIZ == 0)						      \
	_wordcopy_fwd_aligned (dst_bp, src_bp, (nbytes) / OPSIZ);	      \
      else								      \
	_wordcopy_fwd_dest_aligned (dst_bp, src_bp, (nbytes) / OPSIZ);	      \
      src_bp += (nbytes) & -OPSIZ;					      \
      dst_bp += (nbytes) & -OPSIZ;					      \
      (nbytes_left) = (nbytes) % OPSIZ;					      \
    } while (0)

/* Copy *up to* NBYTES_TO_COPY bytes from SRC_END_PTR to DST_END_PTR,
 beginning at the words (of type op_t) right before the pointers and
 continuing towards smaller addresses.  May take advantage of that
 DST_END_PTR is aligned on an OPSIZ multiple.  If not all bytes could be
 easily copied, store remaining number of bytes in NBYTES_REMAINING,
 otherwise store 0.  */
static void
_wordcopy_bwd_aligned(long int, long int, size_t);
static void
_wordcopy_bwd_dest_aligned(long int, long int, size_t);
#define WORD_COPY_BWD(dst_ep, src_ep, nbytes_left, nbytes)		      \
  do									      \
    {									      \
      if (src_ep % OPSIZ == 0)						      \
	_wordcopy_bwd_aligned (dst_ep, src_ep, (nbytes) / OPSIZ);	      \
      else								      \
	_wordcopy_bwd_dest_aligned (dst_ep, src_ep, (nbytes) / OPSIZ);	      \
      src_ep -= (nbytes) & -OPSIZ;					      \
      dst_ep -= (nbytes) & -OPSIZ;					      \
      (nbytes_left) = (nbytes) % OPSIZ;					      \
    } while (0)

/* Threshold value for when to enter the unrolled loops.  */
#define	OP_T_THRES	16

#define CMP_LT_OR_GT(a, b) memcmp_bytes ((a), (b))

/* _wordcopy_fwd_aligned -- Copy block beginning at SRCP to
 block beginning at DSTP with LEN `op_t' words (not LEN bytes!).
 Both SRCP and DSTP should be aligned for memory operations on `op_t's.  */

static void
_wordcopy_fwd_aligned(long int dstp, long int srcp, size_t len)
{
	op_t a0, a1;

	switch (len % 8)
	{
	case 2:
		a0 = ((op_t *) srcp)[0];
		srcp -= 6 * OPSIZ;
		dstp -= 7 * OPSIZ;
		len += 6;
		goto do1;
	case 3:
		a1 = ((op_t *) srcp)[0];
		srcp -= 5 * OPSIZ;
		dstp -= 6 * OPSIZ;
		len += 5;
		goto do2;
	case 4:
		a0 = ((op_t *) srcp)[0];
		srcp -= 4 * OPSIZ;
		dstp -= 5 * OPSIZ;
		len += 4;
		goto do3;
	case 5:
		a1 = ((op_t *) srcp)[0];
		srcp -= 3 * OPSIZ;
		dstp -= 4 * OPSIZ;
		len += 3;
		goto do4;
	case 6:
		a0 = ((op_t *) srcp)[0];
		srcp -= 2 * OPSIZ;
		dstp -= 3 * OPSIZ;
		len += 2;
		goto do5;
	case 7:
		a1 = ((op_t *) srcp)[0];
		srcp -= 1 * OPSIZ;
		dstp -= 2 * OPSIZ;
		len += 1;
		goto do6;

	case 0:
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			return;
		a0 = ((op_t *) srcp)[0];
		srcp -= 0 * OPSIZ;
		dstp -= 1 * OPSIZ;
		goto do7;
	case 1:
		a1 = ((op_t *) srcp)[0];
		srcp -= -1 * OPSIZ;
		dstp -= 0 * OPSIZ;
		len -= 1;
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
		goto do8;
		/* No-op.  */
	}

	do
	{
		do8: a0 = ((op_t *) srcp)[0];
		((op_t *) dstp)[0] = a1;
		do7: a1 = ((op_t *) srcp)[1];
		((op_t *) dstp)[1] = a0;
		do6: a0 = ((op_t *) srcp)[2];
		((op_t *) dstp)[2] = a1;
		do5: a1 = ((op_t *) srcp)[3];
		((op_t *) dstp)[3] = a0;
		do4: a0 = ((op_t *) srcp)[4];
		((op_t *) dstp)[4] = a1;
		do3: a1 = ((op_t *) srcp)[5];
		((op_t *) dstp)[5] = a0;
		do2: a0 = ((op_t *) srcp)[6];
		((op_t *) dstp)[6] = a1;
		do1: a1 = ((op_t *) srcp)[7];
		((op_t *) dstp)[7] = a0;

		srcp += 8 * OPSIZ;
		dstp += 8 * OPSIZ;
		len -= 8;
	}
	while (len != 0);

	/* This is the right position for do0.  Please don't move
	 it into the loop.  */
	do0: ((op_t *) dstp)[0] = a1;
}

/* _wordcopy_fwd_dest_aligned -- Copy block beginning at SRCP to
 block beginning at DSTP with LEN `op_t' words (not LEN bytes!).
 DSTP should be aligned for memory operations on `op_t's, but SRCP must
 *not* be aligned.  */

static void
_wordcopy_fwd_dest_aligned(long int dstp, long int srcp, size_t len)
{
	op_t a0, a1, a2, a3;
	int sh_1, sh_2;

	/* Calculate how to shift a word read at the memory operation
	 aligned srcp to make it aligned for copy.  */

	sh_1 = 8 * (srcp % OPSIZ);
	sh_2 = 8 * OPSIZ - sh_1;

	/* Make SRCP aligned by rounding it down to the beginning of the `op_t'
	 it points in the middle of.  */
	srcp &= -OPSIZ;

	switch (len % 4)
	{
	case 2:
		a1 = ((op_t *) srcp)[0];
		a2 = ((op_t *) srcp)[1];
		srcp -= 1 * OPSIZ;
		dstp -= 3 * OPSIZ;
		len += 2;
		goto do1;
	case 3:
		a0 = ((op_t *) srcp)[0];
		a1 = ((op_t *) srcp)[1];
		srcp -= 0 * OPSIZ;
		dstp -= 2 * OPSIZ;
		len += 1;
		goto do2;
	case 0:
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			return;
		a3 = ((op_t *) srcp)[0];
		a0 = ((op_t *) srcp)[1];
		srcp -= -1 * OPSIZ;
		dstp -= 1 * OPSIZ;
		len += 0;
		goto do3;
	case 1:
		a2 = ((op_t *) srcp)[0];
		a3 = ((op_t *) srcp)[1];
		srcp -= -2 * OPSIZ;
		dstp -= 0 * OPSIZ;
		len -= 1;
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
		goto do4;
		/* No-op.  */
	}

	do
	{
		do4: a0 = ((op_t *) srcp)[0];
		((op_t *) dstp)[0] = MERGE(a2, sh_1, a3, sh_2);
		do3: a1 = ((op_t *) srcp)[1];
		((op_t *) dstp)[1] = MERGE(a3, sh_1, a0, sh_2);
		do2: a2 = ((op_t *) srcp)[2];
		((op_t *) dstp)[2] = MERGE(a0, sh_1, a1, sh_2);
		do1: a3 = ((op_t *) srcp)[3];
		((op_t *) dstp)[3] = MERGE(a1, sh_1, a2, sh_2);

		srcp += 4 * OPSIZ;
		dstp += 4 * OPSIZ;
		len -= 4;
	}
	while (len != 0);

	/* This is the right position for do0.  Please don't move
	 it into the loop.  */
	do0: ((op_t *) dstp)[0] = MERGE(a2, sh_1, a3, sh_2);
}

/* _wordcopy_bwd_aligned -- Copy block finishing right before
 SRCP to block finishing right before DSTP with LEN `op_t' words
 (not LEN bytes!).  Both SRCP and DSTP should be aligned for memory
 operations on `op_t's.  */

static void
_wordcopy_bwd_aligned(long int dstp, long int srcp, size_t len)
{
	op_t a0, a1;

	switch (len % 8)
	{
	case 2:
		srcp -= 2 * OPSIZ;
		dstp -= 1 * OPSIZ;
		a0 = ((op_t *) srcp)[1];
		len += 6;
		goto do1;
	case 3:
		srcp -= 3 * OPSIZ;
		dstp -= 2 * OPSIZ;
		a1 = ((op_t *) srcp)[2];
		len += 5;
		goto do2;
	case 4:
		srcp -= 4 * OPSIZ;
		dstp -= 3 * OPSIZ;
		a0 = ((op_t *) srcp)[3];
		len += 4;
		goto do3;
	case 5:
		srcp -= 5 * OPSIZ;
		dstp -= 4 * OPSIZ;
		a1 = ((op_t *) srcp)[4];
		len += 3;
		goto do4;
	case 6:
		srcp -= 6 * OPSIZ;
		dstp -= 5 * OPSIZ;
		a0 = ((op_t *) srcp)[5];
		len += 2;
		goto do5;
	case 7:
		srcp -= 7 * OPSIZ;
		dstp -= 6 * OPSIZ;
		a1 = ((op_t *) srcp)[6];
		len += 1;
		goto do6;

	case 0:
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			return;
		srcp -= 8 * OPSIZ;
		dstp -= 7 * OPSIZ;
		a0 = ((op_t *) srcp)[7];
		goto do7;
	case 1:
		srcp -= 9 * OPSIZ;
		dstp -= 8 * OPSIZ;
		a1 = ((op_t *) srcp)[8];
		len -= 1;
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
		goto do8;
		/* No-op.  */
	}

	do
	{
		do8: a0 = ((op_t *) srcp)[7];
		((op_t *) dstp)[7] = a1;
		do7: a1 = ((op_t *) srcp)[6];
		((op_t *) dstp)[6] = a0;
		do6: a0 = ((op_t *) srcp)[5];
		((op_t *) dstp)[5] = a1;
		do5: a1 = ((op_t *) srcp)[4];
		((op_t *) dstp)[4] = a0;
		do4: a0 = ((op_t *) srcp)[3];
		((op_t *) dstp)[3] = a1;
		do3: a1 = ((op_t *) srcp)[2];
		((op_t *) dstp)[2] = a0;
		do2: a0 = ((op_t *) srcp)[1];
		((op_t *) dstp)[1] = a1;
		do1: a1 = ((op_t *) srcp)[0];
		((op_t *) dstp)[0] = a0;

		srcp -= 8 * OPSIZ;
		dstp -= 8 * OPSIZ;
		len -= 8;
	}
	while (len != 0);

	/* This is the right position for do0.  Please don't move
	 it into the loop.  */
	do0: ((op_t *) dstp)[7] = a1;
}

/* _wordcopy_bwd_dest_aligned -- Copy block finishing right
 before SRCP to block finishing right before DSTP with LEN `op_t'
 words (not LEN bytes!).  DSTP should be aligned for memory
 operations on `op_t', but SRCP must *not* be aligned.  */

static void
_wordcopy_bwd_dest_aligned(long int dstp, long int srcp, size_t len)
{
	op_t a0, a1, a2, a3;
	int sh_1, sh_2;

	/* Calculate how to shift a word read at the memory operation
	 aligned srcp to make it aligned for copy.  */

	sh_1 = 8 * (srcp % OPSIZ);
	sh_2 = 8 * OPSIZ - sh_1;

	/* Make srcp aligned by rounding it down to the beginning of the op_t
	 it points in the middle of.  */
	srcp &= -OPSIZ;
	srcp += OPSIZ;

	switch (len % 4)
	{
	case 2:
		srcp -= 3 * OPSIZ;
		dstp -= 1 * OPSIZ;
		a2 = ((op_t *) srcp)[2];
		a1 = ((op_t *) srcp)[1];
		len += 2;
		goto do1;
	case 3:
		srcp -= 4 * OPSIZ;
		dstp -= 2 * OPSIZ;
		a3 = ((op_t *) srcp)[3];
		a2 = ((op_t *) srcp)[2];
		len += 1;
		goto do2;
	case 0:
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			return;
		srcp -= 5 * OPSIZ;
		dstp -= 3 * OPSIZ;
		a0 = ((op_t *) srcp)[4];
		a3 = ((op_t *) srcp)[3];
		goto do3;
	case 1:
		srcp -= 6 * OPSIZ;
		dstp -= 4 * OPSIZ;
		a1 = ((op_t *) srcp)[5];
		a0 = ((op_t *) srcp)[4];
		len -= 1;
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
		goto do4;
		/* No-op.  */
	}

	do
	{
		do4: a3 = ((op_t *) srcp)[3];
		((op_t *) dstp)[3] = MERGE (a0, sh_1, a1, sh_2);
		do3: a2 = ((op_t *) srcp)[2];
		((op_t *) dstp)[2] = MERGE (a3, sh_1, a0, sh_2);
		do2: a1 = ((op_t *) srcp)[1];
		((op_t *) dstp)[1] = MERGE (a2, sh_1, a3, sh_2);
		do1: a0 = ((op_t *) srcp)[0];
		((op_t *) dstp)[0] = MERGE (a1, sh_1, a2, sh_2);

		srcp -= 4 * OPSIZ;
		dstp -= 4 * OPSIZ;
		len -= 4;
	}
	while (len != 0);

	/* This is the right position for do0.  Please don't move
	 it into the loop.  */
	do0: ((op_t *) dstp)[3] = MERGE (a0, sh_1, a1, sh_2);
}

/* BE VERY CAREFUL IF YOU CHANGE THIS CODE!  */

/* The strategy of this memcmp is:

 1. Compare bytes until one of the block pointers is aligned.

 2. Compare using memcmp_common_alignment or
 memcmp_not_common_alignment, regarding the alignment of the other
 block after the initial byte operations.  The maximum number of
 full words (of type op_t) are compared in this way.

 3. Compare the few remaining bytes.  */

/* memcmp_bytes -- Compare A and B bytewise in the byte order of the machine.
 A and B are known to be different.
 This is needed only on little-endian machines.  */
static int
memcmp_bytes(op_t a, op_t b)
{
	long int srcp1 = (long int) &a;
	long int srcp2 = (long int) &b;
	op_t a0, b0;

	do
	{
		a0 = ((byte *) srcp1)[0];
		b0 = ((byte *) srcp2)[0];
		srcp1 += 1;
		srcp2 += 1;
	}
	while (a0 == b0);
	return a0 - b0;
}

/* memcmp_common_alignment -- Compare blocks at SRCP1 and SRCP2 with LEN `op_t'
 objects (not LEN bytes!).  Both SRCP1 and SRCP2 should be aligned for
 memory operations on `op_t's.  */
static int
memcmp_common_alignment(long int srcp1, long int srcp2, size_t len)
{
	op_t a0, a1;
	op_t b0, b1;

	switch (len % 4)
	{
	case 2:
		a0 = ((op_t *) srcp1)[0];
		b0 = ((op_t *) srcp2)[0];
		srcp1 -= 2 * OPSIZ;
		srcp2 -= 2 * OPSIZ;
		len += 2;
		goto do1;
	case 3:
		a1 = ((op_t *) srcp1)[0];
		b1 = ((op_t *) srcp2)[0];
		srcp1 -= OPSIZ;
		srcp2 -= OPSIZ;
		len += 1;
		goto do2;
	case 0:
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			return 0;
		a0 = ((op_t *) srcp1)[0];
		b0 = ((op_t *) srcp2)[0];
		goto do3;
	case 1:
		a1 = ((op_t *) srcp1)[0];
		b1 = ((op_t *) srcp2)[0];
		srcp1 += OPSIZ;
		srcp2 += OPSIZ;
		len -= 1;
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
		/* Fall through.  */
	}

	do
	{
		a0 = ((op_t *) srcp1)[0];
		b0 = ((op_t *) srcp2)[0];
		if (a1 != b1)
			return CMP_LT_OR_GT (a1, b1);

		do3: a1 = ((op_t *) srcp1)[1];
		b1 = ((op_t *) srcp2)[1];
		if (a0 != b0)
			return CMP_LT_OR_GT (a0, b0);

		do2: a0 = ((op_t *) srcp1)[2];
		b0 = ((op_t *) srcp2)[2];
		if (a1 != b1)
			return CMP_LT_OR_GT (a1, b1);

		do1: a1 = ((op_t *) srcp1)[3];
		b1 = ((op_t *) srcp2)[3];
		if (a0 != b0)
			return CMP_LT_OR_GT (a0, b0);

		srcp1 += 4 * OPSIZ;
		srcp2 += 4 * OPSIZ;
		len -= 4;
	}
	while (len != 0);

	/* This is the right position for do0.  Please don't move
	 it into the loop.  */
	do0: if (a1 != b1)
		return CMP_LT_OR_GT (a1, b1);
	return 0;
}

/* memcmp_not_common_alignment -- Compare blocks at SRCP1 and SRCP2 with LEN
 `op_t' objects (not LEN bytes!).  SRCP2 should be aligned for memory
 operations on `op_t', but SRCP1 *should be unaligned*.  */
static int
memcmp_not_common_alignment(long int srcp1, long int srcp2, size_t len)
{
	op_t a0, a1, a2, a3;
	op_t b0, b1, b2, b3;
	op_t x;
	int shl, shr;

	/* Calculate how to shift a word read at the memory operation
	 aligned srcp1 to make it aligned for comparison.  */

	shl = 8 * (srcp1 % OPSIZ);
	shr = 8 * OPSIZ - shl;

	/* Make SRCP1 aligned by rounding it down to the beginning of the `op_t'
	 it points in the middle of.  */
	srcp1 &= -OPSIZ;

	switch (len % 4)
	{
	case 2:
		a1 = ((op_t *) srcp1)[0];
		a2 = ((op_t *) srcp1)[1];
		b2 = ((op_t *) srcp2)[0];
		srcp1 -= 1 * OPSIZ;
		srcp2 -= 2 * OPSIZ;
		len += 2;
		goto do1;
	case 3:
		a0 = ((op_t *) srcp1)[0];
		a1 = ((op_t *) srcp1)[1];
		b1 = ((op_t *) srcp2)[0];
		srcp2 -= 1 * OPSIZ;
		len += 1;
		goto do2;
	case 0:
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			return 0;
		a3 = ((op_t *) srcp1)[0];
		a0 = ((op_t *) srcp1)[1];
		b0 = ((op_t *) srcp2)[0];
		srcp1 += 1 * OPSIZ;
		goto do3;
	case 1:
		a2 = ((op_t *) srcp1)[0];
		a3 = ((op_t *) srcp1)[1];
		b3 = ((op_t *) srcp2)[0];
		srcp1 += 2 * OPSIZ;
		srcp2 += 1 * OPSIZ;
		len -= 1;
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
		/* Fall through.  */
	}

	do
	{
		a0 = ((op_t *) srcp1)[0];
		b0 = ((op_t *) srcp2)[0];
		x = MERGE(a2, shl, a3, shr);
		if (x != b3)
			return CMP_LT_OR_GT (x, b3);

		do3: a1 = ((op_t *) srcp1)[1];
		b1 = ((op_t *) srcp2)[1];
		x = MERGE(a3, shl, a0, shr);
		if (x != b0)
			return CMP_LT_OR_GT (x, b0);

		do2: a2 = ((op_t *) srcp1)[2];
		b2 = ((op_t *) srcp2)[2];
		x = MERGE(a0, shl, a1, shr);
		if (x != b1)
			return CMP_LT_OR_GT (x, b1);

		do1: a3 = ((op_t *) srcp1)[3];
		b3 = ((op_t *) srcp2)[3];
		x = MERGE(a1, shl, a2, shr);
		if (x != b2)
			return CMP_LT_OR_GT (x, b2);

		srcp1 += 4 * OPSIZ;
		srcp2 += 4 * OPSIZ;
		len -= 4;
	}
	while (len != 0);

	/* This is the right position for do0.  Please don't move
	 it into the loop.  */
	do0: x = MERGE(a2, shl, a3, shr);
	if (x != b3)
		return CMP_LT_OR_GT (x, b3);
	return 0;
}

int
k_memcmp(ptr_t s1, ptr_t s2, size_t len)
{
	op_t a0;
	op_t b0;
	long int srcp1 = (long int) s1;
	long int srcp2 = (long int) s2;
	op_t res;

	if (len >= OP_T_THRES)
	{
		/* There are at least some bytes to compare.  No need to test
		 for LEN == 0 in this alignment loop.  */
		while (srcp2 % OPSIZ != 0)
		{
			a0 = ((byte *) srcp1)[0];
			b0 = ((byte *) srcp2)[0];
			srcp1 += 1;
			srcp2 += 1;
			res = a0 - b0;
			if (res != 0)
				return res;
			len -= 1;
		}

		/* SRCP2 is now aligned for memory operations on `op_t'.
		 SRCP1 alignment determines if we can do a simple,
		 aligned compare or need to shuffle bits.  */

		if (srcp1 % OPSIZ == 0)
			res = memcmp_common_alignment(srcp1, srcp2, len / OPSIZ);
		else
			res = memcmp_not_common_alignment(srcp1, srcp2, len / OPSIZ);
		if (res != 0)
			return res;

		/* Number of bytes remaining in the interval [0..OPSIZ-1].  */
		srcp1 += len & -OPSIZ;
		srcp2 += len & -OPSIZ;
		len %= OPSIZ;
	}

	/* There are just a few bytes to compare.  Use byte memory operations.  */
	while (len != 0)
	{
		a0 = ((byte *) srcp1)[0];
		b0 = ((byte *) srcp2)[0];
		srcp1 += 1;
		srcp2 += 1;
		res = a0 - b0;
		if (res != 0)
			return res;
		len -= 1;
	}

	return 0;
}

ptr_t
k_memchr(const ptr_t s, int c, size_t n)
{
	const unsigned char *char_ptr;
	const unsigned long int *longword_ptr;
	unsigned long int longword, magic_bits, charmask;

	c = (unsigned char) c;

	/* Handle the first few characters by reading one character at a time.
	 Do this until CHAR_PTR is aligned on a longword boundary.  */
	for (char_ptr = s;
			n > 0
					&& ((unsigned long int) char_ptr & (sizeof(longword) - 1))
							!= 0; --n, ++char_ptr)
		if (*char_ptr == c)
			return (ptr_t) char_ptr;

	/* All these elucidatory comments refer to 4-byte longwords,
	 but the theory applies equally well to 8-byte longwords.  */

	longword_ptr = (unsigned long int *) char_ptr;

	/* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
	 the "holes."  Note that there is a hole just to the left of
	 each byte, with an extra at the end:

	 bits:  01111110 11111110 11111110 11111111
	 bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

	 The 1-bits make sure that carries propagate to the next 0-bit.
	 The 0-bits provide holes for carries to fall into.  */
	switch (sizeof(longword))
	{
	case 4:
		magic_bits = 0x7efefeffL;
		break;
	default:
		return NULL;
	}

	/* Set up a longword, each of whose bytes is C.  */
	charmask = c | (c << 8);
	charmask |= charmask << 16;

	/* Instead of the traditional loop which tests each character,
	 we will test a longword at a time.  The tricky part is testing
	 if *any of the four* bytes in the longword in question are zero.  */
	while (n >= sizeof(longword))
	{
		/* We tentatively exit the loop if adding MAGIC_BITS to
		 LONGWORD fails to change any of the hole bits of LONGWORD.

		 1) Is this safe?  Will it catch all the zero bytes?
		 Suppose there is a byte with all zeros.  Any carry bits
		 propagating from its left will fall into the hole at its
		 least significant bit and stop.  Since there will be no
		 carry from its most significant bit, the LSB of the
		 byte to the left will be unchanged, and the zero will be
		 detected.

		 2) Is this worthwhile?  Will it ignore everything except
		 zero bytes?  Suppose every byte of LONGWORD has a bit set
		 somewhere.  There will be a carry into bit 8.  If bit 8
		 is set, this will carry into bit 16.  If bit 8 is clear,
		 one of bits 9-15 must be set, so there will be a carry
		 into bit 16.  Similarly, there will be a carry into bit
		 24.  If one of bits 24-30 is set, there will be a carry
		 into bit 31, so all of the hole bits will be changed.

		 The one misfire occurs when bits 24-30 are clear and bit
		 31 is set; in this case, the hole at bit 31 is not
		 changed.  If we had access to the processor carry flag,
		 we could close this loophole by putting the fourth hole
		 at bit 32!

		 So it ignores everything except 128's, when they're aligned
		 properly.

		 3) But wait!  Aren't we looking for C, not zero?
		 Good point.  So what we do is XOR LONGWORD with a longword,
		 each of whose bytes is C.  This turns each byte that is C
		 into a zero.  */

		longword = *longword_ptr++ ^ charmask;

		/* Add MAGIC_BITS to LONGWORD.  */
		if ((((longword + magic_bits)

		/* Set those bits that were unchanged by the addition.  */
		^ ~longword)

		/* Look at only the hole bits.  If any of the hole bits
		 are unchanged, most likely one of the bytes was a
		 zero.  */
		& ~magic_bits) != 0)
		{
			/* Which of the bytes was C?  If none of them were, it was
			 a misfire; continue the search.  */

			const unsigned char *cp = (const unsigned char *) (longword_ptr - 1);

			if (cp[0] == c)
				return (ptr_t) cp;
			if (cp[1] == c)
				return (ptr_t) &cp[1];
			if (cp[2] == c)
				return (ptr_t) &cp[2];
			if (cp[3] == c)
				return (ptr_t) &cp[3];
			if (sizeof(longword) > 4)
			{
				if (cp[4] == c)
					return (ptr_t) &cp[4];
				if (cp[5] == c)
					return (ptr_t) &cp[5];
				if (cp[6] == c)
					return (ptr_t) &cp[6];
				if (cp[7] == c)
					return (ptr_t) &cp[7];
			}
		}

		n -= sizeof(longword);
	}

	char_ptr = (const unsigned char *) longword_ptr;

	while (n-- > 0)
	{
		if (*char_ptr == c)
			return (ptr_t) char_ptr;
		else
			++char_ptr;
	}

	return NULL;
}

ptr_t
k_memcpy(ptr_t dstpp, const ptr_t srcpp, size_t len)
{
	unsigned long int dstp = (long int) dstpp;
	unsigned long int srcp = (long int) srcpp;

	/* Copy from the beginning to the end.  */

	/* If there not too few bytes to copy, use word copy.  */
	if (len >= OP_T_THRES)
	{
		/* Copy just a few bytes to make DSTP aligned.  */
		len -= (-dstp) % OPSIZ;
		BYTE_COPY_FWD(dstp, srcp, (-dstp) % OPSIZ);

		/* Copy from SRCP to DSTP taking advantage of the known
		 alignment of DSTP.  Number of bytes remaining is put
		 in the third argumnet, i.e. in LEN.  This number may
		 vary from machine to machine.  */

		WORD_COPY_FWD(dstp, srcp, len, len);

		/* Fall out and copy the tail.  */
	}

	/* There are just a few bytes to copy.  Use byte memory operations.  */
	BYTE_COPY_FWD(dstp, srcp, len);

	return dstpp;
}

/* Return the first occurrence of NEEDLE in HAYSTACK.  */
ptr_t
k_memmem(const ptr_t haystack, const size_t haystack_len, const ptr_t needle,
		const size_t needle_len)
{
	register const char *begin;
	register const char *last_possible = (const char *) haystack + haystack_len
			- needle_len;

	if (needle_len == 0)
		return (ptr_t) &((const char *) haystack)[needle_len - 1];

	for (begin = (const char *) haystack; begin <= last_possible; ++begin)
		if (begin[0] == ((const char *) needle)[0]
				&& !k_memcmp((const ptr_t) &begin[1],
						(const ptr_t) ((const char *) needle + 1),
						needle_len - 1))
			return (ptr_t) begin;

	return NULL;
}

ptr_t
k_memmove(ptr_t dest, const ptr_t src, size_t len)
{
	unsigned long int dstp = (long int) dest;
	unsigned long int srcp = (long int) src;

	/* This test makes the forward copying code be used whenever possible.
	 Reduces the working set.  */
	if (dstp - srcp >= len) /* *Unsigned* compare!  */
	{
		/* Copy from the beginning to the end.  */

		/* If there not too few bytes to copy, use word copy.  */
		if (len >= OP_T_THRES)
		{
			/* Copy just a few bytes to make DSTP aligned.  */
			len -= (-dstp) % OPSIZ;
			BYTE_COPY_FWD(dstp, srcp, (-dstp) % OPSIZ);

			/* Copy from SRCP to DSTP taking advantage of the known
			 alignment of DSTP.  Number of bytes remaining is put
			 in the third argumnet, i.e. in LEN.  This number may
			 vary from machine to machine.  */

			WORD_COPY_FWD(dstp, srcp, len, len);

			/* Fall out and copy the tail.  */
		}

		/* There are just a few bytes to copy.  Use byte memory operations.  */
		BYTE_COPY_FWD(dstp, srcp, len);
	}
	else
	{
		/* Copy from the end to the beginning.  */
		srcp += len;
		dstp += len;

		/* If there not too few bytes to copy, use word copy.  */
		if (len >= OP_T_THRES)
		{
			/* Copy just a few bytes to make DSTP aligned.  */
			len -= dstp % OPSIZ;
			BYTE_COPY_BWD(dstp, srcp, dstp % OPSIZ);

			/* Copy from SRCP to DSTP taking advantage of the known
			 alignment of DSTP.  Number of bytes remaining is put
			 in the third argumnet, i.e. in LEN.  This number may
			 vary from machine to machine.  */

			WORD_COPY_BWD(dstp, srcp, len, len);

			/* Fall out and copy the tail.  */
		}

		/* There are just a few bytes to copy.  Use byte memory operations.  */
		BYTE_COPY_BWD(dstp, srcp, len);
	}

	return dest;
}

ptr_t
k_memset(ptr_t dstpp, int c, size_t len)
{
	long int dstp = (long int) dstpp;

	if (len >= 8)
	{
		size_t xlen;
		op_t cccc;

		cccc = (unsigned char) c;
		cccc |= cccc << 8;
		cccc |= cccc << 16;
/*
		if (OPSIZ > 4)
			cccc |= cccc << 32;
*/

		/* There are at least some bytes to set.
		 No need to test for LEN == 0 in this alignment loop.  */
		while (dstp % OPSIZ != 0)
		{
			((byte *) dstp)[0] = c;
			dstp += 1;
			len -= 1;
		}

		/* Write 8 `op_t' per iteration until less than 8 `op_t' remain.  */
		xlen = len / (OPSIZ * 8);
		while (xlen > 0)
		{
			((op_t *) dstp)[0] = cccc;
			((op_t *) dstp)[1] = cccc;
			((op_t *) dstp)[2] = cccc;
			((op_t *) dstp)[3] = cccc;
			((op_t *) dstp)[4] = cccc;
			((op_t *) dstp)[5] = cccc;
			((op_t *) dstp)[6] = cccc;
			((op_t *) dstp)[7] = cccc;
			dstp += 8 * OPSIZ;
			xlen -= 1;
		}
		len %= OPSIZ * 8;

		/* Write 1 `op_t' per iteration until less than OPSIZ bytes remain.  */
		xlen = len / OPSIZ;
		while (xlen > 0)
		{
			((op_t *) dstp)[0] = cccc;
			dstp += OPSIZ;
			xlen -= 1;
		}
		len %= OPSIZ;
	}

	/* Write the last few bytes.  */
	while (len > 0)
	{
		((byte *) dstp)[0] = c;
		dstp += 1;
		len -= 1;
	}

	return dstpp;
}

