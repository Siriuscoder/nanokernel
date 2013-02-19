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

#ifndef STRINGS_H
#define STRINGS_H

#include "types.h"

/* Append SRC on the end of DEST.  */
char *
k_strcat(char *dest, const char *src);

/* Find the first ocurrence of C in S.  */
char *
k_strchr(const char *s, int c);

/* Return the length of the maximum inital segment of S
 which contains no characters from REJECT.  */
size_t
k_strcspn(const char *s, const char *reject);

/* Compare no more than N characters of S1 , S2,
 ignoring case, returning less than, equal to or
 greater than zero if S1 is lexicographically less
 than, equal to or greater than S2.  */
int
k_strncasecmp(const char *s1, const char *s2, size_t n);

/* Return the length of the maximum initial segment
 of S which contains only characters in ACCEPT.  */
size_t
k_strspn(const char *s, const char *accept);

char *
k_strsep(char **stringp, const char *delim);

/* Compare S1 , S2, returning less than, equal to or
 greater than zero if S1 is lexiographically less than,
 equal to or greater than S2.  */
int
k_strcmp(const char *p1, const char *p2);

/* Copy SRC to DEST.  */
char *
k_strcpy(char *dest, const char *src);

/* Return the length of the null-terminated string STR.  Scan for
 the null terminator quickly by testing four bytes at a time.  */

size_t
k_strlen(const char *str);

/* Return the first ocurrence of NEEDLE in HAYSTACK.  */
char *
k_strstr(const char *haystack, const char *needle);

char *
k_strncat(char *s1, const char *s2, size_t n);

/* Compare no more than N characters of S1 , S2,
 returning less than, equal to or greater than zero
 if S1 is lexiographically less than, equal to or
 greater than S2.  */
int
k_strncmp(const char *s1, const char *s2, size_t n);

char *
k_strncpy(char *s1, const char *s2, size_t n);

int k_isupper(char c);

char k_toupper(char c);

char k_tolower(char c);

#endif
