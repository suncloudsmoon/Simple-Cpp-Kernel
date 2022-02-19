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

%include "boot/macros.asm"

; Parameters needed: ch (track number), cl (sector number), dh (head number), al (number of sectors) es:bx
read_hdd_manual:
	pusha
	mov ah, 02
	mov dl, 0x80
	int 13h
	jc error
	popa
	ret

error:
	outch 'e'
	outch 'r'
	outch 'r'
	outch 'o'
	outch 'r'
	outch '!'   
	cli
	hlt
