#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "driver/vga.h"
#include "lib/stdio.h"
#include "lib/string.h"

/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error You are not using a cross-compiler, you will most certainly run into trouble
#endif

/* Check if compiler is targeting 32-bit ix86. */
#if !defined(__i386__)
#error Need to be compiled with a ix86-elf compiler
#endif

/* GDT Entry */
struct GDT_ENTRY {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;	//high 4bit for granularity flags and low 4bit for limit_high
	uint8_t base_high;
}__attribute__((packed));

/* GDT Descriptor */
struct GDTR {
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

/* TSS structure */
struct TSS {
	uint16_t link;
	uint16_t link_h;

	uint32_t esp0;
	uint16_t ss0;
	uint16_t ss0_h;
	uint32_t esp1;
	uint16_t ss1;
	uint16_t ss1_h;
	uint32_t esp2;
	uint16_t ss2;
	uint16_t ss2_h;

	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;

	uint16_t es;
	uint16_t es_h;
	uint16_t cs;
	uint16_t cs_h;
	uint16_t ss;
	uint16_t ss_h;
	uint16_t ds;
	uint16_t ds_h;
	uint16_t fs;
	uint16_t fs_h;
	uint16_t gs;
	uint16_t gs_h;
	uint16_t ldtr;
	uint16_t ldtr_h;
	uint16_t trap;
	uint16_t iopb_offset;
}__attribute__((packed));

struct GDT_ENTRY gdt[4];
struct GDTR gdtr;
struct TSS tss;

/* Output buffer */
const size_t out_buffer_max = 1024;
size_t out_buffer_size;
char out_buffer[1024];

/* Setup a descriptor in the Global Descriptor Table */
void gdtSet(size_t i, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
	/* Setup the descriptor base address */
	gdt[i].base_low = base & 0xFFFF;
	gdt[i].base_middle = (base >> 16) & 0xFF;
	gdt[i].base_high = (base >> 24) & 0xFF;

	/* Setup the descriptor limit */
	gdt[i].limit_low = limit & 0xFFFF;
	gdt[i].granularity = (limit >> 16) & 0x0F;

	/* Setup the descriptor granularity flags and access byte */
	gdt[i].granularity |= granularity & 0xF0;	// Gr | Sz | 0 | 0
	gdt[i].access = access;						// Pr | Privl(2) | 1 | Ex | DC | RW | Ac

	return;
}

/* Initialize the GDT */
void gdtInit(void)
{
	gdtr.limit = (sizeof(struct GDT_ENTRY) * 4) - 1;
	gdtr.base = (uint32_t)gdt;

	/* The null descriptor */
	gdtSet(0, 0, 0, 0, 0);

	/* Code segment is 4GBytes using 4KBytes granularity and 32bit protected mode */
	gdtSet(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

	/* Data segment is 4GBytes using 4KBytes granularity and 32bit protected mode  */
	gdtSet(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	/* TSS segment using byte granularity and 32bit protected mode */
	gdtSet(3, (uint32_t)&tss, sizeof(struct TSS), 0x89, 0x4F);

	return;
}

void kernel_main(void)
{
	/* Initialize output buffer */
	out_buffer_size = 0;
	for(size_t i = 0; i < out_buffer_max; i++)
	{
		out_buffer[i] = 0;
	}

	/* Initialize vga interface */
	//vgaInit();

	while(1) ;

	return;
}
