#include <mips/regdef.h>
#include <sys/syscall.h>

#define STACK_SIZE 112
#define A0_STACK (STACK_SIZE)

.data
	.align 1                
	endline:        .asciiz   "\n"
	header:         .asciiz "P2\n"
.text
.abicalls
.align 2
.global mips32_plot
.ent mips32_plot

mips32_plot:
.frame $fp, 112, ra
.set noreorder
.cpload t9
.set reorder

subu    sp,sp,112 # Resto 112 al stack pointer
.cprestore 16

sw      ra,24(sp)# ra(return adress) 24(sp)=ra
sw      $fp,20(sp) # 20(sp)=fp
sw 		gp, 16(sp)

move    $fp,sp

s.d     $f20,32($fp)
s.d     $f22,40($fp)
s.d     $f24,48($fp)
s.d     $f26,56($fp)
s.d     $f28,64($fp)
s.d     $f30,72($fp)

move 	t0, a0			
sw t0, A0_STACK($fp)	        # Salvamos los punteros

lw t5, 32(t0)			# parms->x_res
lw t6, 36(t0)			# parms->y_res
lw t4, 40(t0)			# parms->shades
lw t7, 44(t0)			# parms->fd

l.s $f10, 24(t0)			# cpr = parms->cp_re
l.s $f26, 28(t0)			# cpi = parms->cp_im
l.s $f12, 0(t0)			# parms->UL_re
l.s $f28, 4(t0)			# parms->UL_im
l.s $f14, 16(t0)			# parms->d_re
l.s $f30, 20(t0)			# parms->d_im

 jal     print_header  

salir:

	lw ra, 24(sp)
	lw $fp, 20(sp)
	lw gp, 16(sp)

	l.d     $f20,32(sp)
	l.d     $f22,40(sp)
	l.d     $f24,48(sp)
	l.d     $f26,56(sp)
	l.d     $f28,64(sp)
	l.d     $f30,72(sp)


	addu   sp,sp,112
	j     ra

print_header:  
    sw      ra,8($fp)
    li      v0,4  # print_str system call code
    li      a0,1
    la      a1,header
    li      a2,3
    syscall

    lw      ra,8($fp)
    li      v0,4
    li      a0,1
    la      a1,endline
    li      a2,1
    syscall
    jr      ra           
.end mips32_plot
