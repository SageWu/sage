#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "idt.h"
#include "driver/vga.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

static inline void outb(uint16_t port, uint8_t value)
{
	asm volatile ("outb %0, %1": : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t value;

	asm volatile ("inb %1, %0": "=a"(value): "Nd"(port));

	return value;
}

static inline void io_wait(void)
{
	asm volatile ("outb %%al, $0x80": : "a"(0));
}

/*
 Reinitialize the PIC controller, giving them specified vector offsets rather
 than 08h and 70h, as configured by default
 */
void remap_pic(void)
{
	outb(PIC1_COMMAND, 0x11);	//initialize command
	outb(PIC2_COMMAND, 0x11);
	outb(PIC1_DATA, 0x20);	//tell vector offset
	outb(PIC2_DATA, 0x28);
	outb(PIC1_DATA, 0x04);	//tell master PIC that slave PIC at IRQ2(0000 0100)
	outb(PIC2_DATA, 0x02);	//tell slave PIC its cascade identity (0000 0010)
	outb(PIC1_DATA, 0x01);	//8086 mode
	outb(PIC2_DATA, 0x01);
	outb(PIC1_DATA, 0xFD);	//IRQ masks
	outb(PIC2_DATA, 0XFF);
	
	return;
}

/* Keyboard interrupt handler */
void int_keyboard(void)
{
	uint8_t c;
	
	do
	{
		if(inb(0x60) != c)
		{
			c = inb(0x60);
			if(c > 0)
			{
				break;
			}
		}
	}while(true);

	if(c < 0x81)
	{
		vgaPutc(scanToChar(c));
	}

	outb(PIC1_COMMAND, 0x20);
	outb(PIC2_COMMAND, 0x20);

	return;
}
