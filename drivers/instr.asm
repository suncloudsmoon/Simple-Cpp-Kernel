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

[bits 32]
extern os_drivers_instr_insw
extern os_drivers_instr_inb
extern os_drivers_instr_outb

os_drivers_instr_insw:
	push ebp
	push edx
	push edi
	mov ebp, esp


	mov edi, [ebp + 12]
	mov edx, [ebp + 16]
	insw


	mov esp, ebp
	pop edi
	pop edx
	pop ebp
	ret

os_drivers_instr_inb:
	push ebp ; storing any previous call's base address
	push edx
	mov ebp, esp

	; edx has the port number
	mov edx, [ebp + 8]
	in eax, dx ; eax is the return value anyway

	mov esp, ebp
	pop edx
	pop ebp
	ret

os_drivers_instr_outb:
	push ebp ; storing any previous call's base address
	push eax
	push edx
	mov ebp, esp

	; eax has the data, edx has the port number
	mov eax, [ebp + 12]
	mov edx, [ebp + 16]
	out dx, ax

	mov esp, ebp
	pop edx
	pop eax
	pop ebp
	ret