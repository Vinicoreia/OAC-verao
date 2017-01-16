//
//  Trabalho2.c OAC-Verao
//
//
//  Created by Vinicius Filgueiras Filho on 13/01/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define MEM_SIZE 4096
uint32_t RI, PC, immediate26, HI, LO; // HI e LO sao registradores especiais que nao fazem parte do banco
                                    // mas como devemos implementar as instrucoes MFHI e MFLO devemos tambem ter esses 2 registradores.
int32_t mem[MEM_SIZE];
uint32_t Breg[32]; // 32 registradores de 32 bits cada
uint8_t OPcode, rs, rt, rd, shamt, funct;
int16_t immediate16;
uint32_t posicao;
uint64_t aux_hi;


enum OPCODES	{//	lembrem	que soh sao considerados os 6 primeiros bits dessas constantes
    ADDI=0x08,
    ANDI=0x0C,
    BEQ=0x04,
    BNE=0x05,
    EXT=0x00,
    J=0x02,
    JAL=0x03,
    LW=0x23,
    ORI=0x0D,
    SW=0x2B,
    LUI=0x0F
};
enum FUNCT{
    ADD=0x20,
    SUB=0x22,
    MULT=0x18,
    SLT=0x2A,
    JR=0x08,
    SYSCALL=0x0c,
    MFHI=0x10,
    AND=0x24,
    SLL=0x00,
    MFLO=0x12,
    OR=0x25,
    XOR=0x26,
    SRL=0x02,
    NOR=0x27,
    SRA=0x03,
};

int32_t sign_extend(uint16_t immediate){
    immediate = immediate & 0x0000FFFF;
        if(immediate & 0x0000FFFF){
            immediate +=0xFFFF0000;
        }
    return immediate;
}

void fetch(){
    RI= (uint32_t)mem[PC/4]; // Como nesse simulador incrementamos o PC em 4 (como é feito no MIPS) devemos acessar a memória dividindo seu indice por 4.
    PC= PC+4;
}

void decode(){
    OPcode =  RI >> 26;
    rs = (RI >> 21)& 0x1F;
    rt = (RI >> 16) & 0x1F;
    rd = (RI >> 11) & 0x1F;
    shamt = (RI >> 6) &0x1F;
    funct = RI & 0x3F;
    immediate16 = (int16_t)RI;
    immediate26 = RI & 0x2FFFFFF;
}

void execute(){
    switch(OPcode){

        case ADDI: // sign-extended
            immediate16 = sign_extend(immediate16);
            Breg[rt] = Breg[rs] + immediate16;

            break;
        case ANDI: // zero-extended
            Breg[rt] = Breg[rs] & (immediate16 & 0x0000FFFF);
            break;
        case BEQ:// sign-extended
               immediate16 = sign_extend(immediate16);
                if(Breg[rt] == Breg[rs]){
                    PC = PC + (immediate16 << 2);
                }// nao precisa escrever o caso do else pois se não for PC simplesmente incrementa normalmente
            break;
        case BNE:
               immediate16 = sign_extend(immediate16);
             if(Breg[rt] != Breg[rs]){
                    PC = PC + (immediate16 << 2);
                }// nao precisa escrever o caso do else pois se não for PC simplesmente incrementa normalmente
            break;
        case J:
            PC = ((PC & 0xF0000000) | (immediate26<<2)); // a label é defininda no imediato de 26 bits
            break;
        case JAL:
            Breg[31] = PC; // $ra recebe o endereco da proxima instrucao
            PC = ((PC & 0xF0000000) | (immediate26<<2));
            break;
        case LW:
            posicao = (Breg[rs] + ((int32_t)(immediate16 & 0x0000FFFF)))/4;
            Breg[rt] = mem[(Breg[rs] + ((int32_t)(immediate16 & 0x0000FFFF)))/4]; // o endereco de acesso é dado pelo valor em RS + offset dado pelo imediato.
            break;
        case ORI: // zero-extended
            Breg[rt] = (Breg[rs] | ((int32_t)(immediate16 & 0x0000FFFF)));
            break;
        case SW:
            posicao = (Breg[rs] + (((int32_t)(immediate16 & 0x0000FFFF))))/4;
            mem[posicao] = Breg[rt];
            break;
        case LUI:
            Breg[rt] = (immediate16 & 0x0000FFFF) << 16;
            break;
        case EXT:// Caso opcode = 0 então a instrucao eh do tipo R
            switch(funct){
                case ADD:
                    Breg[rd] = Breg[rs] + Breg[rt];
                    break;
                case SUB:
                    Breg[rd] = Breg[rs] - Breg[rt];
                    break;
                case MULT:
                    aux_hi = (int64_t)((int64_t)Breg[rs] * (int64_t)Breg[rt])>>32;
                    HI = (uint32_t)(aux_hi);
                    LO = (Breg[rs] * Breg[rt]);
                    break;
                case AND:
                    Breg[rd] = Breg[rs] & Breg[rt];
                    break;
                case OR:
                    Breg[rd] = Breg[rs] | Breg[rt];
                    break;
                case XOR:
                    Breg[rd] = Breg[rs] ^ Breg[rt];
                    break;
                case NOR:
                    Breg[rd] = ~(Breg[rs] | Breg[rt]);
                    break;
                case SLT:
                    if (Breg[rt] > Breg[rs]){
                        Breg[rd] = 0;
                        }
                    else{
                        Breg[rd] = 1;
                    }
                    break;
                case JR:
                    PC = Breg[31];
                    break;
                case SLL:
                    Breg[rd] = Breg[rt] << shamt;
                    break;
                case SRL:
                    Breg[rd] = Breg[rt] >> shamt;
                    break;
                case SRA://Sign-extended
                    Breg[rd] = (int8_t)(Breg[rt] >> shamt); // Shift right aritmetic SRA com extensão de sinal
                    break;
                case SYSCALL:
                    if(Breg[2] == 10){
                    printf("End of Program");
                    exit(0);
                    }
                    break;
                case MFHI:
                    Breg[rd] =  HI;
                    break;
                case MFLO:
                    Breg[rd] =  LO;
                    break;
                default:
                    printf("This instruction is not implemented here!");
                break;
            }
            break;
        default:
            printf("This instruction is not implemented here!");
            break;
    }

}
void step(){
    if(PC<4096){

    fetch();
    decode();
    execute();
    }
    else
        printf("\nEnd of Program\n");

}
void run(){
    while(PC<4096){
    step();
    }
}

void dump_mem(int start, int end, char format){

    int i=0;
    if(format=='h'){
        for(i=start; i<=end; i++){
            printf("%2d \t %08x\n",i, mem[i]);
        }
        printf("\n");
    }

    else if(format == 'd'){
        for(i=start; i<=end; i++){
            printf("%2d \t %d\n",i, mem[i]);
        }
        printf("\n\n");
    }
    else
        printf("Invalid option");
}
void dump_reg(char format){
    int i=0;
    char register_names[32][6] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2",
                             "$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5",
                             "$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};
    if(format=='h'){
        for(i=0; i<32; i++){
            printf("%d:\t%s: %08x\n",i,register_names[i], Breg[i]);
            }
        printf("PC: %08x\n",PC);
        printf("HI: %08x\n",HI);
        printf("LO: %08x\n",LO);
    }else if (format == 'd'){
        for(i=0; i<32; i++){
            printf("%d:\t%s: %d\n",i,register_names[i], Breg[i]);
            }
        printf("PC: %d\n",PC);
        printf("HI: %d\n",HI);
        printf("LO: %d\n",LO);
    }
     else
        printf("Invalid option");

}

int load_to_mem(){
    FILE *text_fp;
    int i;
    uint32_t instrucao;
    FILE *data_fp;
    text_fp = fopen("text.bin", "rb");
    data_fp = fopen("data.bin", "rb");

    if(!text_fp){
        printf("\nerror loading binary file .text\n");
        return 1;
    }

    else if(!data_fp){
        printf("\nerror loading binary file .data\n");
        return 1;
    }
    else
    {
        i=0;
        //FAZ A LEITURA DO BINARIO
        while(!feof(text_fp)){
            instrucao = 0;
            instrucao |= getc(text_fp);
            instrucao |= getc(text_fp) << 8;
            instrucao |= getc(text_fp) << 16;
            instrucao |= getc(text_fp) << 24;
            mem[i] = instrucao;
            i++;
        }
            i=0;
       while(!feof(data_fp)){
            instrucao = 0;
            instrucao |= getc(data_fp);
            instrucao |= getc(data_fp) << 8;
            instrucao |= getc(data_fp) << 16;
            instrucao |= getc(data_fp) << 24;
            mem[2048+i] = instrucao;
            i++;
        }
   }
    fclose(text_fp);
    fclose(data_fp);
    }
void start(){
// ESSA FUNCAO EH RESPONSAVEL POR EXIBIR O MENU DE FUNCOES E
// COMECAR O PROGRAMA CHAMANDO load_mem .
    load_to_mem();
    int start_point, end_point;
    char c;

    int opcao;
    do{
        printf("\nWich function would you like to execute?\n\n");
        printf("1- Run Program\n2- Run Step.\n3- Dump Memory\n4- Dump register values\n5- Quit\n");
        scanf("%d", &opcao);
        getchar();
        switch(opcao){
            case 1:
                run();
                break;
            case 2:
                step();
                break;
            case 3:
                printf("enter the starting point");
                scanf("%d", &start_point);
                getchar();

                printf("Enter the ending point");
                scanf("%d", &end_point);
                getchar();

                printf("\n\nEnter the format (\'h\') for hex and (\'d\') for decimal\n\n");

                c=getc(stdin);

                if(c!='h' && c!='d'){
                    printf("opcao inexistente");
                }
                else{
                    dump_mem(start_point, end_point, c);
                }
                printf("\n\n\n");
                break;
            case 4:
                printf("\n\nEnter the format (\'h\') for hex and (\'d\') for decimal\n\n");
                c=getc(stdin);
                if(c!='h' && c!='d'){
                    printf("opcao inexistente");
                }
                else{
                    dump_reg(c);
                }
                printf("\n\n\n");
                break;
                break;
            case 5:
                printf("\n\nEnd of Program\n\n");
                exit(0);
            default:
                printf("invalid operation!");
                break;
        }
    }while(1);
}

int main (){
   start();
return 0;
}
