#include <mips/regdef.h>
#include <sys/syscall.h>

#define STACK_SIZE 112
#define A0_STACK (STACK_SIZE)

.data
	.align 1                
	endline:        .asciiz   "\n"
	header:         .asciiz "P2\n"
	space:         .asciiz " "
	.align 2
	mod_cuad:       .float  4.0
	const_mult:     .float  3.0


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

jal     print_header  
move    t0,zero 
jal loop_y

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
	jal save_temp		# Guardo los reg temporales
    #Header magic code
	li      v0,SYS_write
    la      a1,header
    li      a2,3
    move	a0,t7
    syscall
    jal restore_temp	# Cargo los reg temporales

    #Ancho
	jal save_temp		
	move    t1,t5
    jal     print_integer       
    jal restore_temp	

	#Espacio
	jal save_temp		
	li      v0,SYS_write
    la      a1,space
    li      a2,1
    move	a0,t7
    syscall
    jal restore_temp	

    #Alto
    jal save_temp		
	move    t1,t6
    jal     print_integer       
    jal restore_temp	
	

	jal save_temp		
    li      v0,SYS_write
    la      a1,endline
    li      a2,1
    move	a0,t7
    syscall
    jal restore_temp	

	#Shade max value
    jal save_temp		
	move    t1,t4
    jal     print_integer       
    jal restore_temp	

    jal save_temp		
    li      v0,SYS_write
    la      a1,endline
    li      a2,1
    move	a0,t7
    syscall
    jal restore_temp	

    lw      ra,8($fp)
    jr      ra        

print_integer:  li      t4,0
                addu    t4,t4,$fp # t4=52+fp
                move    t5,zero
                li      t3,10
ascii_loop:     divu    t1,t3 # integer/10
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
				sw      t6,4($fp)
                sw      t4,12($fp)
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

save_temp:
	sw t0,104($fp)
	sw t1,100($fp)
	sw t2,96($fp)
	sw t3,92($fp)
	sw t4,88($fp)
	sw t5,84($fp)
	sw t6,80($fp)
	sw t7,76($fp)
	j ra

restore_temp:	
	lw t0,104($fp)
	lw t1,100($fp)
	lw t2,96($fp)
	lw t3,92($fp)
	lw t4,88($fp)
	lw t5,84($fp)
	lw t6,80($fp)
	lw t7,76($fp)
	j ra


loop_y:   
                mov.s   $f21,$f12
                move    t1,zero
                j       loop_x
ret_y:          addiu   t0,t0,1
                sub.s   $f28,$f28,$f30
                li      v0,4
                move	a0,t7
                la      a1,endline
                la      a2,1
                syscall
                bltu    t0, t6, loop_y
                j salir

loop_x:         j      shade
ret_x:          addiu   s1,s1,1
                add.s   $f21,$f21,$f14
                bltu    s1,s3,loop_x
                j       ret_y
        

shade:          mov.s   $f4,$f28        #CI
                mov.s   $f5,$f21        #CR
                move    t3,zero
                
loop_shade:     l.s     $f6,mod_cuad
                mul.s   $f7,$f4,$f4
                mul.s   $f8,$f5,$f5                     
                add.s   $f7,$f8,$f7

                c.le.s  $f6,$f7
                bc1t    print_shade
                mul.s   $f7,$f5,$f5   
                l.s     $f6,const_mult
                mul.s   $f8,$f6,$f4
                mul.s   $f8,$f8,$f4
                mul.s   $f8,$f8,$f5
                sub.s   $f7,$f7,$f8     
                add.s   $f7,$f7,$f21
                mul.s   $f8,$f4,$f4
                mul.s   $f8,$f8,$f4
                mul.s   $f9,$f6,$f5
                mul.s   $f9,$f9,$f5
                mul.s   $f9,$f9,$f4
                sub.s   $f8,$f9,$f8
                add.s   $f8,$f8,$f28
                mov.s   $f4,$f8
                mov.s   $f5,$f7                 
                addiu   t3,t3,1 
                bltu    t3,t4,loop_shade
print_shade:    move    t1,t3
                jal     print_integer
                li      v0,4
                move	a0,t7
                la      a1,space
                la      a2,1
                syscall
                j       ret_x



.end mips32_plot
