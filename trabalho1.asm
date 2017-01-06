# Implemente os seguintes procedimentos:
# a) void Soma_Matriz( int destino[], int origem1[], int origem2[], int n);
# b) void Mult_Matriz(int destino[], int origem1[], int origem2[], int n);
# c) int Det_Matriz(int origem[], int n); /* n<4 */
# d) void Show_Matriz(int origem[], int n); 


.data
	mensagemInicial: .asciiz "Esse programa realiza soma, multiplicação, calcula o determinante e mostra matrizes quadradas (sem tratamento de overflow):\n\n"
	promptValorLinha: .asciiz "Entre o valor da linha "
	Coluna: .asciiz " Coluna "
	dimensao: .word
	barra: .asciiz "|"
	tab: .asciiz "	"
	enter: .asciiz "\n"
	comma: .asciiz ","
	matrizD: .space 256 # Espaco na memoria para manter a matriz A
	matrizS: .space 256 # Espaco na memoria para manter a matriz A
	matrizT: .space 256 # Espaco na memoria para manter a matriz A
	doubleZero: .double 0.0 # Constante 0 com precisao dupla
	doubleUm: .double 1.0 # Constante 1 com precisao dupla
	
	mensagemMatrizD: .asciiz "\n\nA matriz Destino é : \n"
	mensagemMatrizS: .asciiz "\n\nA matriz 1 é : \n"
	mensagemMatrizT: .asciiz "\n\nA matriz 2 é : \n" # Analogia a rs, rt e rd
	mensagemDimensao: .asciiz "Digite a dimensao n da matriz \n	Ex: caso digite 3 será criada uma matriz quadrada de 3 linhas e 3 colunas"
.text

main: 
	la $a0, mensagemInicial
	li $v0, 4
	syscall
	
	ldc1 $f28, doubleZero # carrega em f28 constante zero precisao dupla
	ldc1 $f30,doubleUm # carrega em f30 constante um precisao dupla
	jal getMatrixes
	j exit
getMatrixes:
	la $a0, mensagemDimensao
	li $v0, 4
	syscall
	
	jr $ra

somaMatriz:

exit: 
	li $v0, 10
	syscall