#include <stdint.h>
#include <stddef.h>

#ifndef __stdio__
#define __stdio__

/* Output api */
int8_t flush(void);
int8_t putc(char c);
int8_t puts(const char *str, size_t n);

#endif
