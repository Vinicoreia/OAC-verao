# Implemente os seguintes procedimentos:
# a) void Soma_Matriz( int destino[], int origem1[], int origem2[], int n);
# b) void Mult_Matriz(int destino[], int origem1[], int origem2[], int n);
# c) int Det_Matriz(int origem[], int n); /* n<4 */
# d) void Show_Matriz(int origem[], int n); 


.data
	mensagemInicial: .asciiz "Esse programa realiza soma, multiplica��o, calcula o determinante e mostra matrizes inteiras quadradas (sem tratamento de overflow):\n\n"
	promptValorLinha: .asciiz "\n\nEntre o valor da linha "
	Coluna: .asciiz " Coluna "
	barra: .asciiz "|"
	tab: .asciiz "	"
	enter: .asciiz "\n"
	comma: .asciiz ","
	matrizD: .space 256 # Espaco na memoria para manter a matriz A
	matrizS: .space 256 # Espaco na memoria para manter a matriz A
	matrizT: .space 256 # Espaco na memoria para manter a matriz A	
	mensagemPedeMatriz: .asciiz "\n\n Digite a matriz seguindo os passos"
	mensagemMatrizD: .asciiz "\n\nA matriz Destino � : \n"
	mensagemMatrizS: .asciiz "\n\nA matriz 1 � : \n"
	mensagemMatrizT: .asciiz "\n\nA matriz 2 � : \n" # Analogia a rs, rt e rd
	mensagemDimensao: .asciiz "Digite a dimensao n da matriz \n	Ex: caso digite 3 ser� criada uma matriz quadrada de 3 linhas e 3 colunas\n\n"
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
	move $a1, $v0 #armazena o valor das dimens�es da matriz no registrador $a1, por conven��o os registradores a's devem ser utilizados como argumento das fun��es
		
	la $a0, mensagemPedeMatriz
	li $v0, 4
	syscall
	
	jal getMatrixes
	j exit
	
getMatrixes:
	move $t0, $a1 # $t0 vai ser o limitador dos contadores
	li $t1, 1 # $t1 vai ser o contador I (referente a Linha)
	li $t2, 1 # $t2 vai ser o contador J (referente a Coluna)
	li $t4, 4 # prepara a constante 4 para incrementar o endere�o da matriz
	
	loop1: # Esse la�o controla o input das colunas da matriz.

		li $v0, 4
		la $a0, promptValorLinha
		syscall
		
		li $v0, 1
		move $a0, $t2
		syscall
		
		li $v0, 5
		syscall
				
		
		
		
		
		beq $t0, $t2, loop2
		addi $t2, $t2, 1
		j loop1
	loop2: # Esse la�o controla o input das linhas da matriz
		li $t2, 0
		addi $t1, $t1, 1
		beq $t0, $t1, exit
		j loop1

somaMatriz:


exit: 
	li $v0, 10
	syscall