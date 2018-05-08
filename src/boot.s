begin:
/* declare constants for the mutiboot header */
.set ALIGN, 1 << 0				/* align loaded modules on page boundaries */
.set MEMINFO, 1 << 1			/* provide memory map */
.set FLAGS, ALIGN | MEMINFO		/* multiboot 'flag' field */
.set MAGIC, 0x1BADB002			/* 'magic number' lets bootloader fin the header */
.set CHECKSUM, -(MAGIC + FLAGS)	/* checksum of above, to prove we are multiboot */

/*
 Declare a multiboot header that marks the program as a kernel. These are magic
 values that are documented in the mutiboot standard. The bootloader will search
 for this signature in the first 8 KiB of the kernel file, aligned at a 32-bit
 boundary. The signature is in its own section so the header can be forced to be
 within the first 8 KiB of kernel file.
 */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*
 The multiboot standard does not define the value of the stack pointer register
 (esp) and it is up to the kernel to provide a stack. This allocates room for a
 small stack by creating a symbol at the bottom of it, then allocating 16384
 bytes for it, and finally creating a symbol at the top. The stack grow
 downwards on x86. The stack is in its own section so it can be marked nobits,
 which means the kernel file is smaller because it does not contain an 
 uninitialized stack. The stack on x86 must be 16-byte aligned according to the
 System V ABI standard and de-facto extensions. The compiler will assume the
 stack is properly aligned and failure to align the stack will result in
 undefined behavior.
 */
.section .bss
.align 16
stack_top:
	.skip 16384 #16KiB
stack_bottom:

/*
 
*/
.section .data
/* Task status segment */
tss_start:
	.word 0x0	//link
	.word 0x0	//link_h

	.long 0x0	//esp0
	.word 0x0	//ss0
	.word 0x0	//ss0_h
	.long 0x0	//esp1
	.word 0x0	//ss1
	.word 0x0	//ss1_h
	.long 0x0	//esp2
	.word 0x0	//ss2
	.word 0x0	//ss2_h

	.long 0x0	//cr3
	.long 0x0	//eip
	.long 0x0	//eflags
	.long 0x0	//eax
	.long 0x0	//ecx
	.long 0x0	//edx
	.long 0x0	//ebx
	.long 0x0	//esp
	.long 0x0	//ebp
	.long 0x0	//esi
	.long 0x0	//edi

	.word 0x0	//es
	.word 0x0	//es_h
	.word 0x0	//cs
	.word 0x0	//cs_h
	.word 0x0	//ss
	.word 0x0	//ss_h
	.word 0x0	//ds
	.word 0x0	//ds_h
	.word 0x0	//fs
	.word 0x0	//fs_h
	.word 0x0	//gs
	.word 0x0	//gs_h
	.word 0x0	//ldtr
	.word 0x0	//ldtr_h
	.word 0x0	//trap
	.word 0x0	//iopb_offset
tss_end:

/* Global descriptor table */
gdt_start:
gdt_null:
	.long 0x0
	.long 0x0
gdt_code:
	.word 0xFFFF             # limit_low
	.word 0x0                # base_low
	.byte 0x0                # base_middle
	.byte 0b10011010         # access type	format: Pr | Privl(2) | 1 | Ex | DC | RW | Ac
	.byte 0b11001111         # flags and limit_high format: Gr | Sz | 0 | 0 | limit_high
	.byte 0x0                # base_high
gdt_data:
	.word 0xFFFF
	.word 0x0
	.byte 0x0
	.byte 0b10010010
	.byte 0b11001111
	.byte 0x0
gdt_tss:
	.word (tss_end - tss_start) & 0xFFFF
	.word 0x0
	.byte 0x0
	.byte 0b10001001
	.byte (((tss_end - tss_start) >> 16) &  0x0F) | 0b01000000
	.byte 0x0
gdt_ucode:
	.word 0xFFFF
	.word 0x0
	.byte 0x0
	.byte 0b11111010
	.byte 0b11001111
	.byte 0x0
gdt_udata:
	.word 0xFFFF
	.word 0x0
	.byte 0x0
	.byte 0b11110010
	.byte 0b11001111
	.byte 0x0
gdt_end:

/* GDT description structure */
gdtr:
	.word (gdt_end - gdt_start - 1)
	.long gdt_start

/* Interrupt descriptor table */
idt_start:
int_0:	//divide-by-zero error
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_1:	//debug
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_2:	//non-maskable interrupt
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_3:	//breakpoint
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_4:	//overflow
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_5:	//bound range exceeded
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_6:	//invalid opcode
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_7:	//device not available
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_8:	//double fault
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_9:	//coprocessor segment overrun
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_A:	//invalid tss
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_B:	//segment not present
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_C:	//stack-segment fault
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_D:	//general protection fault
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_E:	//page fault
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_F:	//reserved
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_10:	//x87 floating-point exception
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_11:	//alignment check
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_12:	//machine check
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_13:	//SIMD floating-point exception
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_14:	//virtualization exception
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
	.skip 8 * 9
int_1E:	//security exception
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_1F:	//reserved
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_20:	//programmable interrupt timer interrupt
	.word 0x00
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x00
int_21:	//keyboard interrupt
	.word 0x0
	.word 0x0008
	.byte 0x0
	.byte 0b10001110
	.word 0x0
idt_end:

/* IDT description structure */
idtr:
	.word 8 * 256 - 1
	.long idt_start

/*
 The linker script specifies _start as the entry point to the kernel and the
 bootloader will jump to this position once the kernel has been loaded. It
 doesn't make sense to return from this function as the bootloader is gone.
 */
.section .text
.global _start
.type _start, @function
_start:
	/*
	 The bootloader has loaded us into 32-bit protected mode on a x86
	 machine. Interrupts are disabled. Paging is disabled. The processor
	 state is as defined in the mutiboot standard. The kernel has full
	 control of the CPU. The kernel can only make use of hardware features
	 and any code it provides as part of itself. There's no printf
	 function, unless the kernel provides its own <stdio.h> header and a
	 printf implementation. There are no security restrictions, no
	 safeguards, no debugging mechanisms, only what the kernel provides
	 itself. It has absolute and complete power over the machine.
	 */

	/*
	 To set up a stack, we set the esp register to point to the bottom of our
	 stack (as it grows downwards on x86 systems). This is necessarily done
	 in assembly as languages such as C cannot function without a stack.
	 */
	movl $stack_bottom, %esp

	/*
	 This is a good place to initialize crucial processor state before the
	 high-level kernel is enter. It's best to minimize the early
	 environment where crucial features are offline. Note that the
	 processor is not fully initialized yet: Features such as floating
	 point instructions and instruction set extensions are not initialized
	 yet. The GDT should be loaded here. Paging should be enabled here.
	 C++ features such as global constructors and exceptions will requre
	 runtime support to work as well.
	 */
	call vgaInit
	call _load_gdt
	call _load_idt
	call remap_pic
	sti

	/*
	 Enter the high-level kernel. The ABI requires the stack is 16-byte
	 aligned at the time of the call instruction (which afterward pushed
	 the return pointer of size 4 bytes). The stack was originally 16-byte
	 aligned above and we've sice pushed a multiple of 16 bytes to the
	 stack since (pushed 0 bytes so far) and alignment is thus preserved
	 and the call is well defined.
	 */
	call kernel_main

	/*
	 If the system has noting more to do, put the computer into an
	 infinite loop. To do that:
	 1) Disable interrupts with cli (clear interrupt enable in eflags).
		They are already disabled by the bootloader, so this is no needed.
		Mind that you mignt later enable interrupts and return from
		kernel_main (which is sort of nonsensical to do).
	 2) Wait for the next interrupt to arrive with hlt (halt instruction).
		Since they are disabled, this will lock up the computer.
	 3) Jump to the hlt instruction if it ever wakes up due to a
		non-maskable interrupt occurring or due to system management mode.
	 */
	cli
1:	hlt
	jmp 1b

/*
 Set the size of the _start symbol to the current location '.' minus its start.
 This is useful when debugging or when you implement call tracing.
 */
.size _start, . - _start

/* Load the GDT and reload sagment register */
.global _load_gdt
.type _load_gdt, @function
_load_gdt:
	pushl %ebp
	movl %esp, %ebp
	cli
	movl tss_start, %eax		//fixed up the tss entry
	movw %ax, gdt_tss + 2
	shr $16, %eax
	movb %al, gdt_tss + 4
	movb %ah, gdt_tss + 7
	lgdt (gdtr)
	movl $0x0010, %eax		//reload sagment register
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %fs
	movl %eax, %gs
	movl %eax, %ss
	ljmp $0x0008, $_load_gdt_end
_load_gdt_end:
	popl %ebp
	ret

/* Load the IDT */
.global _load_idt
.type _load_idt, @function
_load_idt:
	pushl %ebp
	movl $_int_keyboard, %eax
	movw %ax, int_21
	shr $16, %eax
	movw %ax, int_21 + 6
	lidt (idtr)
	popl %ebp
	ret

/* Keyboard interrupt handler */
.global _int_keyboard
.type _unint, @function
_int_keyboard:
	pushal
	cld
	call int_keyboard
	popal
	iret
