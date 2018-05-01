#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "lib/stdio.h"

/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* Check if compiler is targeting 32-bit ix86. */
#if !defined(__i386__)
#error "Need to be compiled with a ix86-elf compiler"
#endif

size_t strlen(const char *str)
{
	size_t len = 0;
	
	while(str[len++]) ;

	return len - 1;
}

void terminalWriteString(const char *str)
{
	puts(str, strlen(str));
}

void kernel_main(void)
{
	/* Initialize terminal interface */
	terminalInitialize();

	terminalWriteString("1Hello, Kernel World!\n");
	terminalWriteString("2Hello, Kernel World!\n");
	terminalWriteString("3Hello, Kernel World!\n");
	terminalWriteString("4Hello, Kernel World!\n");
	terminalWriteString("5Hello, Kernel World!\n");
	terminalWriteString("6Hello, Kernel World!\n");
	terminalWriteString("7Hello, Kernel World!\n");
	terminalWriteString("8Hello, Kernel World!\n");
	terminalWriteString("9Hello, Kernel World!\n");
	terminalWriteString("10Hello, Kernel World!\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
	terminalWriteString("\n");
}
