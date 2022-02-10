%include "boot/macros.asm"

; Code segment descriptor
; Limit = 1111 1111 1111 1111 (20 bits) -> 0xfffff
; Base = starting point = 0 (32 bits)
; Present = 1
; Privilege = 00 (root)
; Segment type = 1 (code segment)
; Type flags = 1 (code), 0 (non-conforming), 1 (readable + executable), 0 (managed by CPU -> accessed)
; Other flags = 1 (4 GB RAM of mem access), 1 (32 bit mem), 0 (not 64 bit), 0 (not 64 bit)

; Data segment descriptor
; Present, privilege, segment type: 1000
; Type flags: data (0), direction (0), writable (1), 0 (managed by CPU)
; Other flags : [same as code segment descriptor]
protected_mode_flags_start:
	null_descriptor:
		dd 0
		dd 0
	code_descriptor:
		dw 0xffff	; First 16 bits
		dw 0 ; First 24 bits of the base
		db 0 ; First 24 bits of the base
		db 10011010b ; Present, privilege, segment type (4 bits), general type flags (4 bits)
		db 11001111b ; Other flags (4 bits) + last 4 bits of limit flag
		db 0 ; Last 8 bits of the base flag
	data_descriptor:
		dw 0xffff
		dw 0
		db 0
		db 10010010b ; [Present, privilege, segment type] + [general type flags]
		db 11001111b ; Other flags (4 bits) + last 4 bits of the limit flag
		db 0 ; Last 8 bits of the base flag
	protected_mode_flags_end:
		; End location of the Global Descriptor Table ("GDT")

	GDT_Info:
		dw protected_mode_flags_end - protected_mode_flags_start - 1 ; size of the GDT
		dd protected_mode_flags_start ; Pointer to the start of the GDT

CODE_SEG equ code_descriptor - protected_mode_flags_start
DATA_SEG equ data_descriptor - protected_mode_flags_start	