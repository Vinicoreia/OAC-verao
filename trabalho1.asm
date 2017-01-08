# Implemente os seguintes procedimentos:
# a) void Soma_Matriz( int destino[], int origem1[], int origem2[], int n);
# b) void Mult_Matriz(int destino[], int origem1[], int origem2[], int n);
# c) int Det_Matriz(int origem[], int n); /* n<4 */
# d) void Show_Matriz(int origem[], int n); 


.data
	mensagemInicial: .asciiz "Esse programa realiza soma, multiplicacao, calcula o determinante e mostra matrizes inteiras quadradas (sem tratamento de overflow):\n\n"
	promptValorLinha: .asciiz "\n\nEntre o valor da linha "
	coluna: .asciiz " Coluna "
	barra: .asciiz "|"
	tab: .asciiz "	"
	enter: .asciiz "\n"
	comma: .asciiz ","
	.align 2
	matrizD: .space 128 # Espaco na memoria para manter a matriz A
	matrizS: .space 128 # Espaco na memoria para manter a matriz A
	matrizT: .space 128 # Espaco na memoria para manter a matriz A	
	mensagemPedeMatriz1: .asciiz "\n\n Digite a matriz 1 seguindo os passos"
	mensagemPedeMatriz2: .asciiz "\n\n Digite a matriz 2 seguindo os passos"
	mensagemMatrizD: .asciiz "\n\nA matriz Destino eh : \n"
	mensagemMatrizS: .asciiz "\n\nA matriz 1 eh : \n"
	mensagemMatrizT: .asciiz "\n\nA matriz 2 eh : \n" # Analogia a rs, rt e rd
	mensagemDimensao: .asciiz "Digite a dimensao n da matriz \n	Ex: caso digite 3 sera criada uma matriz quadrada de 3 linhas e 3 colunas\n\n"
.text

main: 
	la $a0, mensagemInicial
	li $v0, 4
	syscall
	
	la $a0, mensagemDimensao
	li $v0, 4
	syscall
	
	li $v0, 5
	syscall
	move $s0, $v0 #armazena o valor das dimensoes da matriz no registrador $s0, por convencao os registradores a's devem ser utilizados como argumento das funcoes
		
	la $a0, mensagemPedeMatriz1
	li $v0, 4
	syscall
	
	la $a1, matrizS
	jal write_cel
	
	la $a0, mensagemPedeMatriz2
	li $v0, 4
	syscall
	
	la $a1, matrizT
	jal write_cel
	
	
	la $a0, mensagemMatrizS
	li $v0, 4
	syscall
	la $a2, matrizS
	jal print_matrix
		
	la $a0, mensagemMatrizT
	li $v0, 4
	syscall
	
	la $a2, matrizT
	jal print_matrix
	
	
	j exit
#####################################################################################
write_cel:
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	li $t1, 0 # $t1 vai ser o contador I (referente a Linha)
	li $t2, 0 # $t2 vai ser o contador J (referente a Coluna)
	move $t0, $a1 # usa o registrador $t0 para o calculo do endereco de cada inteiro a ser adicionado na matriz
	
	write_loop: # Esse laco controla o input das colunas da matriz
		beq $t2, $s0, resetIncrement
		jal prompt_message
		li $v0, 5 # codigo para receber input de inteiro pelo usuario
		syscall
		move $t3, $v0
		sw $t3, 0($t0)
		addi $t0, $t0, 4 # incrementa o ponteiro que aponta para o endereco de memoria que vai guardar o dado da matriz
		addi $t2, $t2, 1 # incrementa o contador de colunas
		j write_loop
	resetIncrement: # Esse laco controla o input das linhas da matriz
		li $t2, 0
		addi $t1, $t1, 1
		beq $t1, $s0, continue
		j write_loop
	continue:
		lw $ra, 0($sp)
		addi $sp, $sp 8
		jr $ra
		
##################################################################################################
read_cell:
	
	sw $ra, 4($sp)
	# $a0 sera a linha
	# $a1 sera a coluna
	# $a2 sera o endereco da matriz
	move $t0, $a2 #t0 endereco da matriz
	move $t1, $a0 # t1 linha
	move $t2, $a1 # t2 coluna
	
	# O valor de a[i][j] = a[ i*n + j]
	
	mul $t3, $s0, $t1
	add $t3, $t3, $t2 #t3 = i*n + j
	
	sll $t3, $t3, 2 # multiplica o indice por 4 para acessar na matriz
	add $t0, $t0, $t3 # somando $t3 a $t0 para calcular o endereco do valor na matriz
	
	lw $v0, 0($t0) # armazena o valor em $v0 e retorna

	lw $ra, 4($sp)
	jr $ra
	
#################################################################################################
print_matrix:

	addi $sp, $sp, -8
	sw $ra, 0($sp)

	# Controle dos lacos
	add $t6, $zero, $zero # i (linha)
	add $t7, $zero, $zero # j (coluna)
	
	la $a0, barra
	li $v0, 4
	syscall
	
	
	print_loop:
		
		la $a0, tab
		li $v0, 4
		syscall
	
		beq $t7, $s0, jump_line
		move $a0, $t6
		move $a1, $t7
		
		jal read_cell
		
		move $a0, $v0 # imprimindo retorno do $v0
		li $v0, 1
		syscall
		
		addi $t7, $t7, 1
		j print_loop
	
	jump_line:
		addi $t6, $t6, 1
	
		la $a0, barra
		li $v0, 4
		syscall
	
		la $a0, enter
		li $v0, 4
		syscall
	
		la $a0, barra
		li $v0, 4
		syscall
	
		beq $t6, $s0, exit_print
		li $t7, 0
		j print_loop
		
	exit_print:
		lw $ra, 0($sp)
		addi $sp, $sp 8
		jr $ra
#################################################################################################
exit: 
	li $v0, 10
	syscall
##################################################################################################
prompt_message:
	sw $ra, 4($sp)
	li $v0, 4
	la $a0, promptValorLinha
	syscall
	
	li $v0, 1
	move $a0, $t1
	syscall
		
	li $v0, 4
	la $a0, coluna
	syscall
	
	li $v0, 1
	move $a0, $t2
	syscall

	li $v0, 4
	la $a0, enter
	syscall
	lw $ra, 4($sp)
	jr $ra