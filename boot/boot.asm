;
; Copyright (c) 2022, suncloudsmoon and the Simple-Cpp-Kernel contributors.
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so.
; 
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.

[org 0x7c00]

%include "boot/macros.asm"

[bits 16]
boot:
	mov bp, 0x8000
	mov sp, bp
	read_hdd 0x1000,20,0,0,2
	jmp switch_to_protected
	hlt

%include "boot/readhdd.asm"
%include "boot/gdt.asm"

[bits 16]
switch_to_protected:
	cli
	lgdt [GDT_Info]
	mov eax, cr0
	or eax, 1 ; Change the last bit of cr0 to 1 ("or eax, 1" is "or eax, 00000001")
	mov cr0, eax
	jmp CODE_SEG:protected_mode ; far jump to protected mode
	hlt

[bits 32]
protected_mode:
	; Setting up the segment registers
	mov ax, DATA_SEG
	mov ds, ax 
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; New memory location for the kernel to start
	mov ebp, 0x90000
	mov esp, ebp

	call KERNEL_LOCATION

	hlt

times 510 - ($ - $$) db 0
dw 0xaa55
