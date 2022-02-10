
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
