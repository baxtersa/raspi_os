.section ".text.startup"

.global _start
.global _get_stack_pointer
.global _exception_table
.global _enable_interrupts

.equ CPSR_MODE_USER, 0x10
.equ CPSR_MODE_FIQ, 0x11
.equ CPSR_MODE_IRQ, 0x12
.equ CPSR_MODE_SVR, 0x13
.equ CPSR_MODE_ABORT, 0x17
.equ CPSR_MODE_UNDEFINED, 0x1B
.equ CPSR_MODE_SYSTEM, 0x1F

.equ CPSR_IRQ_INHIBIT, 0x80
.equ CPSR_FIQ_INHIBIT, 0x40
.equ CPSR_THUMB, 0x20

_start:
	ldr pc, _reset_h
	ldr pc, _undefined_instruction_vector_h
	ldr pc, _software_interrupt_vector_h
	ldr pc, _prefetch_abort_vector_h
	ldr pc, _data_abort_vector_h
	ldr pc, _unused_handler_h
	ldr pc, _interrupt_vector_h
	ldr pc, _fast_interrupt_vector_h

_reset_h:
	.word _reset_

_undefined_instruction_vector_h:
	.word undefined_instruction_vector

_software_interrupt_vector_h:
	.word software_interrupt_vector

_prefetch_abort_vector_h:
	.word prefetch_abort_vector

_data_abort_vector_h:
	.word data_abort_vector

_unused_handler_h:
	.word _reset_

_interrupt_vector_h:
	.word interrupt_vector

_fast_interrupt_vector_h:
	.word fast_interrupt_vector

_reset_:
	mov r0, #0x8000
	mov r1, #0x0000
	ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
	ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia r1!, {r2, r3, r4, r5, r6, r7, r8, r9}

	mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
	msr cpsr_c, r0
	mov sp, #0x8000

	mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
	msr cpsr_c, r0

	mov sp, #0x4000

	bl _cstartup
	
_inf_loop:
	b _inf_loop

_get_stack_pointer:
	// Return the xtack pointer value
	str sp, [sp]
	ldr r0, [sp]

	// Return from the function
	mov pc, lr

_enable_interrupts:
	mrs r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c, r0

	mov pc, lr