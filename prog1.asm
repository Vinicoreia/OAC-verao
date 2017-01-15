.data
vetor: .word 0xFF000000 0x00FF0000

.text

la $s0, vetor
lw $t0, 0($s0)
lw $t1, 4($s0)
or $t0, $t0, $t1
sw $t0, 0($s0)
and $t0, $t0, $t1
addi $t5, $zero, 12
ori $t5, $t5, 3
andi $t5, $t5, 2

L1:
addi $t5, $t5, -1
bne $t5, $zero, L1
srl $t0, $t0, 16
sll $t1, $t0, 24
sra $t2, $t1, 24

L2:
beq $t2, $zero, L3
addi $t2, $t2, 1
j L2

L3:
jal volta
ori $t2, $zero, 0x0FF00000
mult $t2, $t0
mfhi $t3
mflo $t4
slt $s1, $t3, $t4
slt $s2, $t4, $t3
ori $v0, $zero, 10
syscall

volta:
	jr $ra