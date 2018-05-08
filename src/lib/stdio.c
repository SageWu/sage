#include <stdint.h>
#include <stddef.h>
#include "stdio.h"
#include "../driver/vga.h"

/* Global scope variables */
extern const size_t out_buffer_max;
extern size_t out_buffer_size;
extern char out_buffer[];

/* Type declaration */
enum STDIO_STATUS {
	SUCCESS = 0,
	ERROR = -1,
	OVERFLOW = -2
};

/* Flush the output buffer to vga buufer */
int8_t flush(void)
{
	if(vgaRefresh() == 0)
		return SUCCESS;
	else
		return ERROR;
}

/* Put one character into output buffer at the current position */
int8_t putc(char c)
{
	out_buffer[out_buffer_size++] = c;
	if(out_buffer_size >= out_buffer_max)
	{
		if(vgaRefresh() != 0)
			return ERROR;
	}

	return SUCCESS;
}

/* Print string having n sizes of character */
int8_t puts(const char *str, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		if(putc(str[i]) != 0)
			return ERROR;
	}

	return SUCCESS;
}

