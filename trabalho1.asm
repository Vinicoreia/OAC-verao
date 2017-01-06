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
	matrizA: .space 72 # Espaco na memoria para manter a matriz A
.text

main: 
	la $a0, mensagemInicial
	li $v0, 4
	syscall