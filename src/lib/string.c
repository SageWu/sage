#include <stdint.h>
#include <stddef.h>
#include "string.h"

/* Get the length of string */
size_t strlen(const char *str)
{
	size_t n = 0;
	while(str[n++]) ;

	return n - 1;
}
