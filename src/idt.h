#include <stdint.h>

#ifndef __idt__
#define __idt__

/* Interrupt descriptor table entry */
struct IDT_ENTRY {
	uint16_t offset_low;
	uint16_t selector;	//code segment selector in GDT or LDT
	uint8_t zero;
	uint8_t attribute;	// P | DPL(2) | S | GateType
	uint16_t offset_high;
}__attribute__((packed));

/* Register of idt */
struct IDTR {
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

void remap_pic(void);
void idtSet(size_t i, uint32_t offset, uint16_t selector, uint8_t attr);
void unhandler(void);

#endif
