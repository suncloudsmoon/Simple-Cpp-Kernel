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
