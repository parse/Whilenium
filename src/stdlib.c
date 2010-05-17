#include "stdlib.h"

char *itoa(int n, char *s, int b) {
	static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	int i=0, sign;
    
	if ((sign = n) < 0)
		n = -n;
	
	do {
		s[i++] = digits[n % b];
	} while ((n /= b) > 0);
	
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	
	return strrev(s);
}

size_t strlen(const char *string) {
	const char *s;
	
	s = string;
	while (*s)
		s++;
	return s - string;
}

char *strrev(char *str) {
	char *p1, *p2;
	
	if (!str || !*str)
		return str;
	
	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	
	return str;
}	

char* strcpy(char* dest, const char* src)
	{
	   const char* p;
	   char* q;

	   for(p = src, q = dest; *p != '\0'; p++, q++)
	      *q = *p;

	   *q = '\0';

	   return dest;
	}

char strcmp(char* strA, char* strB) {
	while (*strA == *strB && *strA != '\0') {
		strA++;
		strB++;
	}
	
	if (*strA == '\0' && *strB == '\0')
		return 1;
	else
		return 0;
}


int atoi(const char *s) {
	static const char digits[] = "0123456789";  /* legal digits in order */
	unsigned val=0;	 /* value we're accumulating */
	int neg=0;	      /* set to true if we see a minus sign */

	/* skip whitespace */
	while (*s==' ' || *s=='\t') {
		s++;
	}

	/* check for sign */
	if (*s=='-') {
		neg=1;
		s++;
	}
	else if (*s=='+') {
		s++;
	}

	/* process each digit */
	while (*s) {
		const char *where;
		unsigned digit;
		
		/* look for the digit in the list of digits */
		where = strchr(digits, *s);
		if (where==NULL) {
			/* not found; not a digit, so stop */
			break;
		}

		/* get the index into the digit list, which is the value */
		digit = (where - digits);

		/* could (should?) check for overflow here */

		/* shift the number over and add in the new digit */
		val = val*10 + digit;

		/* look at the next character */
		s++;
	}
	
	/* handle negative numbers */
	if (neg) {
		return -val;
	}
	
	/* done */
	return val;
}

 /*
  * C standard string function: find leftmost instance of a character
  * in a string.
  */
char *strchr(const char *s, int ch) {
	/* scan from left to right */
	while (*s) {
		/* if we hit it, return it */
		if (*s==ch) {
			return (char *)s;
		}
		s++;
	}

	/* if we were looking for the 0, return that */
	if (*s==ch) {
		return (char *)s;
	}

	/* didn't find it */
	return NULL;
}


