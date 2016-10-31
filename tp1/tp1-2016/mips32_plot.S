#include <mips/regdef.h>
#include <sys/syscall.h>

#define STACK_SIZE 112
#define A0_STACK (STACK_SIZE)

.data
	.align 1                
	endline:        .asciiz   "\n"
	header:         .asciiz "P2\n"


numeros:    .word num_0, num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9
num_0:  .asciiz "0"
num_1:  .asciiz "1"
num_2:  .asciiz "2"
num_3:  .asciiz "3"
num_4:  .asciiz "4"
num_5:  .asciiz "5"
num_6:  .asciiz "6"
num_7:  .asciiz "7"
num_8:  .asciiz "8"
num_9:  .asciiz "9"

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

subu    sp,sp,112 
.cprestore 16

sw      ra,24(sp)
sw      $fp,20(sp) 
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

l.s $f10, 24(t0)		# cpr = parms->cp_re
l.s $f26, 28(t0)		# cpi = parms->cp_im
l.s $f12, 0(t0)			# parms->UL_re
l.s $f28, 4(t0)			# parms->UL_im
l.s $f14, 16(t0)		# parms->d_re
l.s $f30, 20(t0)		# parms->d_im

#arranca la magia by RE, danger
li t0, 0                #y=0
move $f46, $f28         #ayux de ul_im

loop_y:
    li      t1, 0       #x=0
    move $f38, $f12     #auxiliar de ul_re

    j       loop_x
    addiu   t0,1        #y++
    sub.s   $f46, $f46, $f30 #ci -= parms->d_im

loop_x:
    mov.s   $f0, $f38   #zr = cr;
    mov.s   $f1, $f46   #zi = ci;
    j       bright_loop
    addiu   t1, 1   #x++
    add.s   $f38, $f14

bright_loop_init
    li      t3, 0 #c = 0
bright_loop
    #aqui iran las cuentas
    addiu   t3,1    #c++


# end of danger

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
    li      v0,SYS_write
    la      a1,header
    li      a2,3
    move	a0,t7
    syscall

    li 		s2,123
	move    t1,s2
    jal     print_integer       
	lw      ra,8($fp)

    li      v0,SYS_write
    la      a1,endline
    li      a2,1
    move	a0,t7
    syscall
    jr      ra        

print_integer:  li      t4,52
                addu    t4,t4,$fp
                move    t5,zero
                li      t3,10
ascii_loop:     divu    t1,t3 
                mfhi    t2
                mflo    t1
                sll     t2,t2,2
                sw      t2,0(t4)
                addiu   t5,t5,1
                li      t6,4
                subu    t4,t4,t6
                bnez    t1,ascii_loop
                move    t6,t5
ascii_loop_2:   addiu   t4,t4,4
                li      t8,1
                subu    t6,t6,t8
                li      v0,4
                move	a0,t7
                sw      t6,4($fp)
                sw      t4,12($fp)
                lw      t1,0(t4)
                lw      a1,numeros(t1)
                li      a2,1
                syscall
                lw      t6,4($fp)
                lw      t4,12($fp)
                bnez    t6,ascii_loop_2
                jr      ra

.end mips32_plot
