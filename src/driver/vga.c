#include <stdint.h>
#include <stddef.h>
#include "vga.h"

/* Global scope variables */
extern const size_t out_buffer_max;
extern size_t out_buffer_size;
extern char out_buffer[];

/* Type declaration */
/* Function return status code */
enum VGA_STATUS {
	SUCCESS = 0,
	ERROR = -1,
	OVERFLOW = -1
};

/* File scope variables */
/* The dimension of the vga screen */
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

/* Current cursor position */
static size_t cursor_row;
static size_t cursor_column;
static uint8_t char_color;

/* Note the use of the volatile keyword to prevent the compiler from eliminating dead stores. */
volatile uint16_t *vga_buffer;

/* Get the VGA color code (background with frontground). */
static inline uint8_t vgaEntryColor(enum VGA_COLOR fg, enum VGA_COLOR bg)
{
	return fg | bg << 4;
}

/* Get the VGA character with color. */
static inline uint16_t vgaEntry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

/* Initialize the vga buffer filling with space */
void vgaInit(void)
{
	cursor_row = 0;
	cursor_column = 0;
	char_color = vgaEntryColor(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
	vga_buffer = (uint16_t *) 0xB8000;

	size_t index;

	for(size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for(size_t x = 0; x < VGA_WIDTH; x++)
		{
			index = y * VGA_WIDTH + x;
			vga_buffer[index] = vgaEntry(' ', char_color);
		}
	}
}

/* Scroll up vga buffer n row */
void vgaScrollUp(size_t n)
{
	uint16_t space = vgaEntry(' ', char_color);
	const size_t offset_column = n * VGA_WIDTH;
	const size_t offset_row = VGA_HEIGHT - n;

	size_t index, new_index;

	for(size_t y = n; y < VGA_HEIGHT; y++)
	{
		for(size_t x = 0; x < VGA_WIDTH; x++)
		{
			index = y * VGA_WIDTH + x;
			new_index = index - offset_column;
			vga_buffer[new_index] = vga_buffer[index];
			
			if(y >= offset_row)
			{
				vga_buffer[index] = space;
			}
		}
	}
}

/* Get character from output buffer to refresh the vga data buffer */
int8_t vgaRefresh(void)
{
	if(out_buffer_size > out_buffer_max)
		return OVERFLOW;

	size_t i = 0;
	while(i < out_buffer_size)
	{
		vgaPutc(out_buffer[i]);
		out_buffer[i] = 0;

		i++;
	}

	out_buffer_size = 0;

	return SUCCESS;
}

/* Translate scan code to character */
char scanToChar(uint8_t code)
{
	char *lowercase = "??1234567890-=\b\tqwertyuiop[]\n?asdfghjkl;'`?\\zxcvbnm,./?*? ";

	return lowercase[code];
}

/* Put one character into vga buffer to display */
void vgaPutc(char c)
{
	if(c == '\n')
	{
		cursor_column = VGA_WIDTH - 1;
	}
	else
	{
		const size_t index = cursor_row * VGA_WIDTH + cursor_column;
		vga_buffer[index] = vgaEntry(c, char_color);
	}

	if(++cursor_column >= VGA_WIDTH)
	{
		cursor_column = 0;
		if(++cursor_row >= VGA_HEIGHT)
		{
			vgaScrollUp(1);
		}
	}
}
