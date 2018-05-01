#include <stddef.h>
#include <stdint.h>

#ifndef __stdio__
#define __stdio__

/* Hardware text mode color constants. */
enum VGA_COLOR {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15
};

/* Get the VGA color code (background with frontground). */
inline uint8_t vgaEntryColor(enum VGA_COLOR fg, enum VGA_COLOR bg)
{
	return fg | bg << 4;
}

/* Get the VGA character with color. */
inline uint16_t vgaEntry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

/* Terminal api */
void terminalInitialize(void);
void terminalScrollUp(size_t n);

/* Output api */
void putc(char c);
void puts(const char *str, size_t size);

#endif
