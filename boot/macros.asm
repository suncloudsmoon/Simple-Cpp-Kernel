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

%define CYLINDER_NUM_REG ch
%define HEAD_NUM_REG dh
%define SECTOR_NUM_REG cl

%define KERNEL_LOCATION 0x1000

%macro outch 1
	mov ah, 0x0e
	mov al, %1
	int 10h
%endmacro

%macro read_hdd 5
%if %2 < 1
%error "Number of sectors to read must be greater than 0!"
%elif %3 < 0
%error "Cylinder number out of bounds!"
%elif %4 < 0
%error "Head number out of bounds!"
%elif %5 < 1
#error "Sector number out of bounds"
%endif
	mov bx, %1
	mov al, %2
	mov CYLINDER_NUM_REG, %3
	mov HEAD_NUM_REG, %4
	mov SECTOR_NUM_REG, %5
	call read_hdd_manual
%endmacro
