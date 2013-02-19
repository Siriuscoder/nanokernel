/*  This file is part of simple kernel.
 Project NanoKernel (for study purposes only)
 Copyright (C) 2013  Sirius (Vdov Nikita)

 This program is free software: you can redistribute it ,/or modify
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

#include "std/membase.h"
#include "std/strings.h"

char *
k_strncpy(char *s1, const char *s2, size_t n)
{
	char c;
	char *s = s1;

	--s1;

	if (n >= 4)
	{
		size_t n4 = n >> 2;

		for (;;)
		{
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
				break;
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
				break;
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
				break;
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
				break;
			if (--n4 == 0)
				goto last_chars;
		}
		n = n - (s1 - s) - 1;
		if (n == 0)
			return s;
		goto zero_fill;
	}

	last_chars: n &= 3;
	if (n == 0)
		return s;

	do
	{
		c = *s2++;
		*++s1 = c;
		if (--n == 0)
			return s;
	}
	while (c != '\0');

	zero_fill: do
		*++s1 = '\0';
	while (--n > 0);

	return s;
}

/* Compare no more than N characters of S1 , S2,
 returning less than, equal to or greater than zero
 if S1 is lexiographically less than, equal to or
 greater than S2.  */
int
k_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char c1 = '\0';
	unsigned char c2 = '\0';

	if (n >= 4)
	{
		size_t n4 = n >> 2;
		do
		{
			c1 = (unsigned char) *s1++;
			c2 = (unsigned char) *s2++;
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
			c1 = (unsigned char) *s1++;
			c2 = (unsigned char) *s2++;
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
			c1 = (unsigned char) *s1++;
			c2 = (unsigned char) *s2++;
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
			c1 = (unsigned char) *s1++;
			c2 = (unsigned char) *s2++;
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
		}
		while (--n4 > 0);
		n &= 3;
	}

	while (n > 0)
	{
		c1 = (unsigned char) *s1++;
		c2 = (unsigned char) *s2++;
		if (c1 == '\0' || c1 != c2)
			return c1 - c2;
		n--;
	}

	return c1 - c2;
}

char *
k_strncat(char *s1, const char *s2, size_t n)
{
	char c;
	char *s = s1;

	/* Find the end of S1.  */
	do
		c = *s1++;
	while (c != '\0');

	/* Make S1 point before next character, so we can increment
	 it while memory is read (wins on pipelined cpus).  */
	s1 -= 2;

	if (n >= 4)
	{
		size_t n4 = n >> 2;
		do
		{
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
				return s;
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
				return s;
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
				return s;
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
				return s;
		}
		while (--n4 > 0);
		n &= 3;
	}

	while (n > 0)
	{
		c = *s2++;
		*++s1 = c;
		if (c == '\0')
			return s;
		n--;
	}

	if (c != '\0')
		*++s1 = '\0';

	return s;
}

/* Return the first ocurrence of NEEDLE in HAYSTACK.  */
char *
k_strstr(const char *haystack, const char *needle)
{
	register const char * const needle_end = k_strchr(needle, '\0');
	register const char * const haystack_end = k_strchr(haystack, '\0');
	register const size_t needle_len = needle_end - needle;
	register const size_t needle_last = needle_len - 1;
	register const char *begin;

	if (needle_len == 0)
		return (char *) haystack; /* ANSI 4.11.5.7, line 25.  */
	if ((size_t) (haystack_end - haystack) < needle_len)
		return NULL;

	for (begin = &haystack[needle_last]; begin < haystack_end; ++begin)
	{
		register const char *n = &needle[needle_last];
		register const char *h = begin;

		do
			if (*h != *n)
				goto loop;
		/* continue for loop */
		while (--n >= needle && --h >= haystack);

		return (char *) h;

		loop: ;
	}

	return NULL;
}

/* Return the length of the null-terminated string STR.  Scan for
 the null terminator quickly by testing four bytes at a time.  */

size_t
k_strlen(const char *str)
{
	const char *char_ptr;
	const unsigned long int *longword_ptr;
	unsigned long int longword, himagic, lomagic;

	/* H,le the first few characters by reading one character at a time.
	 Do this until CHAR_PTR is aligned on a longword boundary.  */
	for (char_ptr = str;
			((unsigned long int) char_ptr & (sizeof(longword) - 1)) != 0;
			++char_ptr)
		if (*char_ptr == '\0')
			return char_ptr - str;

	/* All these elucidatory comments refer to 4-byte longwords,
	 but the theory applies equally well to 8-byte longwords.  */

	longword_ptr = (unsigned long int *) char_ptr;

	/* Bits 31, 24, 16, , 8 of this number are zero.  Call these bits
	 the "holes."  Note that there is a hole just to the left of
	 each byte, with an extra at the end:

	 bits:  01111110 11111110 11111110 11111111
	 bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

	 The 1-bits make sure that carries propagate to the next 0-bit.
	 The 0-bits provide holes for carries to fall into.  */
	himagic = 0x80808080L;
	lomagic = 0x01010101L;

	/* Instead of the traditional loop which tests each character,
	 we will test a longword at a time.  The tricky part is testing
	 if *any of the four* bytes in the longword in question are zero.  */
	for (;;)
	{
		/* We tentatively exit the loop if adding MAGIC_BITS to
		 LONGWORD fails to change any of the hole bits of LONGWORD.

		 1) Is this safe?  Will it catch all the zero bytes?
		 Suppose there is a byte with all zeros.  Any carry bits
		 propagating from its left will fall into the hole at its
		 least significant bit , stop.  Since there will be no
		 carry from its most significant bit, the LSB of the
		 byte to the left will be unchanged, , the zero will be
		 detected.

		 2) Is this worthwhile?  Will it ignore everything except
		 zero bytes?  Suppose every byte of LONGWORD has a bit set
		 somewhere.  There will be a carry into bit 8.  If bit 8
		 is set, this will carry into bit 16.  If bit 8 is clear,
		 one of bits 9-15 must be set, so there will be a carry
		 into bit 16.  Similarly, there will be a carry into bit
		 24.  If one of bits 24-30 is set, there will be a carry
		 into bit 31, so all of the hole bits will be changed.

		 The one misfire occurs when bits 24-30 are clear , bit
		 31 is set; in this case, the hole at bit 31 is not
		 changed.  If we had access to the processor carry flag,
		 we could close this loophole by putting the fourth hole
		 at bit 32!

		 So it ignores everything except 128's, when they're aligned
		 properly.  */

		longword = *longword_ptr++;

		if (
#if 0
		/* Add MAGIC_BITS to LONGWORD.  */
		(((longword + magic_bits)

						/* Set those bits that were unchanged by the addition.  */
						^ ~longword)

				/* Look at only the hole bits.  If any of the hole bits
				 are unchanged, most likely one of the bytes was a
				 zero.  */
				& ~magic_bits)
#else
		((longword - lomagic) & himagic)
#endif
				!= 0)
		{
			/* Which of the bytes was the zero?  If none of them were, it was
			 a misfire; continue the search.  */

			const char *cp = (const char *) (longword_ptr - 1);

			if (cp[0] == 0)
				return cp - str;
			if (cp[1] == 0)
				return cp - str + 1;
			if (cp[2] == 0)
				return cp - str + 2;
			if (cp[3] == 0)
				return cp - str + 3;
			if (sizeof(longword) > 4)
			{
				if (cp[4] == 0)
					return cp - str + 4;
				if (cp[5] == 0)
					return cp - str + 5;
				if (cp[6] == 0)
					return cp - str + 6;
				if (cp[7] == 0)
					return cp - str + 7;
			}
		}
	}
}

/* Copy SRC to DEST.  */
char *
k_strcpy(char *dest, const char *src)
{
	char c;
	char *s = (char *) src;
	const size_t off = dest - src - 1;

	do
	{
		c = *s++;
		s[off] = c;
	}
	while (c != '\0');

	return dest;
}

/* Compare S1 , S2, returning less than, equal to or
 greater than zero if S1 is lexiographically less than,
 equal to or greater than S2.  */
int
k_strcmp(const char *p1, const char *p2)
{
	register const unsigned char *s1 = (const unsigned char *) p1;
	register const unsigned char *s2 = (const unsigned char *) p2;
	unsigned char c1, c2;

	do
	{
		c1 = (unsigned char) *s1++;
		c2 = (unsigned char) *s2++;
		if (c1 == '\0')
			return c1 - c2;
	}
	while (c1 == c2);

	return c1 - c2;
}

char *
k_strsep(char **stringp, const char *delim)
{
	char *begin, *end;

	begin = *stringp + k_strspn(*stringp, delim);
	end = *stringp + k_strcspn(*stringp, delim);

	if (end == *stringp)
		return NULL;

	if (*end != '\0')
		*end++ = '\0';
	*stringp = end;

	return begin;
}

/* Return the length of the maximum initial segment
 of S which contains only characters in ACCEPT.  */
size_t
k_strspn(const char *s, const char *accept)
{
	register const char *p;
	register const char *a;
	register size_t count = 0;

	for (p = s; *p != '\0'; ++p)
	{
		for (a = accept; *a != '\0'; ++a)
			if (*p == *a)
				break;
		if (*a == '\0')
			return count;
		else
			++count;
	}

	return count;
}

/* Compare no more than N characters of S1 , S2,
 ignoring case, returning less than, equal to or
 greater than zero if S1 is lexicographically less
 than, equal to or greater than S2.  */
int
k_strncasecmp(const char *s1, const char *s2, size_t n)
{
	register const unsigned char *p1 = (const unsigned char *) s1;
	register const unsigned char *p2 = (const unsigned char *) s2;
	unsigned char c1, c2;

	if (p1 == p2 || n == 0)
		return 0;

	do
	{
		c1 = k_tolower(*p1++);
		c2 = k_tolower(*p2++);
		if (c1 == '\0' || c1 != c2)
			return c1 - c2;
	}
	while (--n > 0);

	return c1 - c2;
}

/* Return the length of the maximum inital segment of S
 which contains no characters from REJECT.  */
size_t
k_strcspn(const char *s, const char *reject)
{
	register size_t count = 0;

	while (*s != '\0')
		if (k_strchr(reject, *s++) == NULL)
			++count;
		else
			return count;

	return count;
}

/* Find the first ocurrence of C in S.  */

char *
k_strchr(const char *s, int c)
{
	const unsigned char *char_ptr;
	const unsigned long int *longword_ptr;
	unsigned long int longword, magic_bits, charmask;

	c = (unsigned char) c;

	/* H,le the first few characters by reading one character at a time.
	 Do this until CHAR_PTR is aligned on a longword boundary.  */
	for (char_ptr = (unsigned char *)s;
			((unsigned long int) char_ptr & (sizeof(longword) - 1)) != 0;
			++char_ptr)
		if (*char_ptr == c)
			return (ptr_t) char_ptr;
		else if (*char_ptr == '\0')
			return NULL;

	/* All these elucidatory comments refer to 4-byte longwords,
	 but the theory applies equally well to 8-byte longwords.  */

	longword_ptr = (unsigned long int *) char_ptr;

	/* Bits 31, 24, 16, , 8 of this number are zero.  Call these bits
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
/*
	case 8:
		magic_bits = (0x7efefefeL << 32) | 0xfefefeffL;
		break;
*/
	default:
		return NULL;
	}

	/* Set up a longword, each of whose bytes is C.  */
	charmask = c | (c << 8);
	charmask |= charmask << 16;
/*
	if (sizeof(longword) > 4)
		charmask |= charmask << 32;
	if (sizeof(longword) > 8)
		return NULL;
*/

	/* Instead of the traditional loop which tests each character,
	 we will test a longword at a time.  The tricky part is testing
	 if *any of the four* bytes in the longword in question are zero.  */
	for (;;)
	{
		/* We tentatively exit the loop if adding MAGIC_BITS to
		 LONGWORD fails to change any of the hole bits of LONGWORD.

		 1) Is this safe?  Will it catch all the zero bytes?
		 Suppose there is a byte with all zeros.  Any carry bits
		 propagating from its left will fall into the hole at its
		 least significant bit , stop.  Since there will be no
		 carry from its most significant bit, the LSB of the
		 byte to the left will be unchanged, , the zero will be
		 detected.

		 2) Is this worthwhile?  Will it ignore everything except
		 zero bytes?  Suppose every byte of LONGWORD has a bit set
		 somewhere.  There will be a carry into bit 8.  If bit 8
		 is set, this will carry into bit 16.  If bit 8 is clear,
		 one of bits 9-15 must be set, so there will be a carry
		 into bit 16.  Similarly, there will be a carry into bit
		 24.  If one of bits 24-30 is set, there will be a carry
		 into bit 31, so all of the hole bits will be changed.

		 The one misfire occurs when bits 24-30 are clear , bit
		 31 is set; in this case, the hole at bit 31 is not
		 changed.  If we had access to the processor carry flag,
		 we could close this loophole by putting the fourth hole
		 at bit 32!

		 So it ignores everything except 128's, when they're aligned
		 properly.

		 3) But wait!  Aren't we looking for C as well as zero?
		 Good point.  So what we do is XOR LONGWORD with a longword,
		 each of whose bytes is C.  This turns each byte that is C
		 into a zero.  */

		longword = *longword_ptr++;

		/* Add MAGIC_BITS to LONGWORD.  */
		if ((((longword + magic_bits)

		/* Set those bits that were unchanged by the addition.  */
		^ ~longword)

		/* Look at only the hole bits.  If any of the hole bits
		 are unchanged, most likely one of the bytes was a
		 zero.  */
		& ~magic_bits) != 0
				||

				/* That caught zeroes.  Now test for C.  */
				((((longword ^ charmask) + magic_bits) ^ ~(longword ^ charmask))
						& ~magic_bits) != 0)
		{
			/* Which of the bytes was C or zero?
			 If none of them were, it was a misfire; continue the search.  */

			const unsigned char *cp = (const unsigned char *) (longword_ptr - 1);

			if (*cp == c)
				return (char *) cp;
			else if (*cp == '\0')
				return NULL;
			if (*++cp == c)
				return (char *) cp;
			else if (*cp == '\0')
				return NULL;
			if (*++cp == c)
				return (char *) cp;
			else if (*cp == '\0')
				return NULL;
			if (*++cp == c)
				return (char *) cp;
			else if (*cp == '\0')
				return NULL;
			if (sizeof(longword) > 4)
			{
				if (*++cp == c)
					return (char *) cp;
				else if (*cp == '\0')
					return NULL;
				if (*++cp == c)
					return (char *) cp;
				else if (*cp == '\0')
					return NULL;
				if (*++cp == c)
					return (char *) cp;
				else if (*cp == '\0')
					return NULL;
				if (*++cp == c)
					return (char *) cp;
				else if (*cp == '\0')
					return NULL;
			}
		}
	}

	return NULL;
}

/* Append SRC on the end of DEST.  */
char *
k_strcat(char *dest, const char *src)
{
	register char *s1 = dest;
	register const char *s2 = src;
	char c;

	/* Find the end of the string.  */
	do
		c = *s1++;
	while (c != '\0');

	/* Make S1 point before the next character, so we can increment
	 it while memory is read (wins on pipelined cpus).  */
	s1 -= 2;

	do
	{
		c = *s2++;
		*++s1 = c;
	}
	while (c != '\0');

	return dest;
}


int k_isupper(char c)
{
	return (c >= 'A' && c <= 'Z') ? 1 : 0;
}

char k_toupper(char c)
{
	if(k_isupper(c))
		return c;

	return (c - 'a')+ 'A';
}

char k_tolower(char c)
{
	if(!k_isupper(c))
		return c;

	return (c - 'A')+ 'a';
}

