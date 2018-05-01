#include "stdio.h"
#include <stdint.h>
#include <stddef.h>

/* VGA screen dimensons. */
const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

/* Current state of terminal */
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;

/* Note the use of the volatile keyword to prevent the compiler from eliminating dead stores. */
volatile uint16_t *terminal_buffer;

/* Initialize the terminal filling with space */
void terminalInitialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vgaEntryColor(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t *) 0xB8000;

	for(size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for(size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vgaEntry(' ', terminal_color);
		}
	}
}

/* Scroll up terminal n line */
void terminalScrollUp(size_t n)
{
	for(size_t y = n; y < VGA_HEIGHT; y++)
	{
		for(size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			const size_t new_index = index - n * VGA_WIDTH;
			terminal_buffer[new_index] = terminal_buffer[index];
		}
	}

	for(size_t y = VGA_HEIGHT - n; y < VGA_HEIGHT; y++)
	{
		for(size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vgaEntry(' ', terminal_color);
		}
	}
}

/* Put one character into output buffer at the current position */
void putc(char c)
{
	if(c == '\n')
	{
		terminal_column = VGA_WIDTH - 1;
	}
	else
	{
		const size_t index = terminal_row * VGA_WIDTH + terminal_column;
		terminal_buffer[index] = vgaEntry(c, terminal_color);
	}

	if(++terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		if(++terminal_row == VGA_HEIGHT)
		{
			terminalScrollUp(1);
		}
	}
}

/* Output string having n sizes of character */
void puts(const char *str, size_t size)
{
	for(size_t i = 0; i < size; i++)
		putc(str[i]);
}

