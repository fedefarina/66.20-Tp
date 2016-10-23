#include <mips/regdef.h>

                .text
                .globl mips32_plot
mips32_plot:   
                subu    sp,sp,108 # Resto 108 al stack pointer
                sw      ra,104(sp)# ra(return adress) 
                sw      $fp,100(sp)
                move    $fp,sp		
                sw      a0,108($fp) // a0,a1,a2,a3 argumentos que se le pasan a la función
                sw      s5,96($fp)  // Inicio guardo lo que ya tenian los registros
                sw      s4,92($fp)	
                sw      s3,88($fp)
                sw      s2,84($fp)
                sw      s1,80($fp)
                sw      s0,76($fp)
                s.s     $f28,72($fp) 
                s.s     $f27,68($fp)
                s.s     $f22,64($fp)
                s.s     $f21,60($fp)
                s.s     $f20,56($fp) // Fin guardo lo que ya tenian los registros
                l.s     $f20,0(a0)              #$f20 = ulr
                l.s     $f27,4(a0)              #$f27 = uli
                l.s     $f21,16(a0)             #$f21 = dr
                l.s     $f22,20(a0)             #$f22 = di
                lw      s2,32(a0)               #s2 = 255 -> limite shadow
                lw      s3,24(a0)               #s3 = res_x
                lw      s4,28(a0)               #s4 = res_y
                lw      s5,36(a0)
                jal     print_header
                move    s0,zero                 #s0  Y = 0
        
loop_y:   
                mov.s   $f28,$f20ci = parms->UL_im
                move    s1,zero #s1: X = 0
                j       loop_x
ret_y:          addiu   s0,s0,1
                sub.s   $f27,$f27,$f22
                bltu    s0, s4, loop_y
salir:          lw      ra,104($fp) //Inicio restauro los registros iniciales
                lw      s5,96($fp)
                lw      s4,92($fp)
                lw      s3,88($fp)
                lw      s2,84($fp)
                lw      s1,80($fp)
                lw      s0,76($fp)
                l.s     $f28,72($fp)
                l.s     $f27,68($fp)
                l.s     $f22,64($fp)
                l.s     $f21,60($fp)
                l.s     $f20,56($fp) //Fin restauro los registros iniciales
                addiu   sp,sp,108
                lw      $fp,100($fp)
                jr      ra

loop_x:         j      shade
ret_x:          addiu   s1,s1,1
                add.s   $f28,$f28,$f21
                bltu    s1,s3,loop_x
                j       ret_y
        

shade:          mov.s   $f4,$f27        #CI
                mov.s   $f5,$f28        #CR
                move    t3,zero
                
loop_shade:     l.s     $f6,mod_cuad
                mul.s   $f7,$f4,$f4
                mul.s   $f8,$f5,$f5                     
                add.s   $f7,$f8,$f7

                c.le.s  $f6,$f7
                bc1t    print_shade
                mul.s   $f7,$f5,$f5   ci = parms->UL_im
                l.s     $f6,const_mult
                mul.s   $f8,$f6,$f4
                mul.s   $f8,$f8,$f4
                mul.s   $f8,$f8,$f5
                sub.s   $f7,$f7,$f8     
                add.s   $f7,$f7,$f28
                mul.s   $f8,$f4,$f4
                mul.s   $f8,$f8,$f4
                mul.s   $f9,$f6,$f5
                mul.s   $f9,$f9,$f5
                mul.s   $f9,$f9,$f4
                sub.s   $f8,$f9,$f8
                add.s   $f8,$f8,$f27
                mov.s   $f4,$f8
                mov.s   $f5,$f7                 
                addiu   t3,t3,1 
                bltu    t3,s2,loop_shade
print_shade:    move    t1,t3
                jal     print_integer
                li      v0,4
                la      a0,1
                la      a1,endlineci = parms->UL_im
                la      a2,1
                syscall
                j       ret_x

print_header:  
                sw      ra,8($fp)
                li      v0,4
                li      a0,1
                la      a1,header
                li      a2,3
                syscall
                move    t1,s3
                jal     print_integer
                lw      ra,8($fp)
                li      v0,4
                li      a0,1
                la      a1,endline
                li      a2,1
                syscall
                move    t1,s4
                jal     print_integer
                lw      ra,8($fp)ci = parms->UL_im
                li      v0,4
                li      a0,1
                la      a1,endline
                li      a2,1
                syscall      
                move    t1,s2
                jal     print_integer
                lw      ra,8($fp)
                li      v0,4
                li      a0,1
                la      a1,endline
                li      a2,1
                syscall          
                jr      ra
print_integer:  li      t4,52
                addu    t4,t4,$fp
                move    t5,zero
                li      t3,10
print_loop:     divu    t1,t3 
                mfhi    t2
                mflo    t1
                sll     t2,t2,2
                sw      t2,0(t4)
                addiu   t5,t5,1
                li      t6,4
                subu    t4,t4,t6
                bnez    t1,print_loop
                move    t6,t5
print_loop_2:   addiu   t4,t4,4
                li      t8,1
                subu    t6,t6,t8
                li      v0,4
                li      a0,1
                sw      t6,4($fp)
                sw      t4,12($fp)
                lw      t1,0(t4)
                lw      a1,numeros(t1)
                li      a2,1
                syscall
                lw      t6,4($fp)
                lw      t4,12($fp)
                bnez    t6,print_loop_2
                jr      ra
                        
                .data
                .align 2
mod_cuad:       .float  4.0
const_mult:     .float  3.0
                .align 1                
endline:        .asciiz   "\n"
header:         .asciiz "P2\n"

                .align 2
puntero:        .float  -0.65                   #ulr    0 //Extremo superior izquierzo (re)
                .float  0.3                     #uli    4 
                .float  2.0                     #lrr    8 // Extremo inferior derecho (re)
                .float  -3.0                    #lri    12
                .float  0.0015625               #dr     16 // Real center
                .float  0.002083333333333       #di     20 
                .word   64                      #res_x  24 //Resolución en X 
                .word   48                      #res_y  28
                .word   255                     #shade  32 // Brillo

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
