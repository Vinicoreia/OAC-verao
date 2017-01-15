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
uint32_t RI, PC, immediate26, HI, LO; // HI e LO sao registradores espceiais que nao fazem parte do banco
                                    // mas como devemos implementar as instrucoes MFHI e MFLO devemos tambem ter esses 2 registradores.
int32_t mem[MEM_SIZE];
uint32_t Breg[32]; // 32 registradores de 32 bits cada
uint8_t OPcode, rs, rt, rd, shamt, funct;
int16_t immediate16;


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

void fetch(){
    RI=mem[PC/4]; // Como nesse simulador incrementamos o PC em 4 (como é feito no MIPS) devemos acessar a memória dividindo seu indice por 4.
    PC= PC+4;
}

void decode(){
    uint8_t OPcode =  RI >> 26;
    uint8_t rs = (RI >> 21)& 0x1F;
    uint8_t rt = (RI >> 16) & 0x1F;
    uint8_t rd = (RI >> 11) & 0x1F;
    uint8_t shamt = (RI >> 6) &0x1F;
    uint8_t funct = RI & 0x2F;
    int16_t immediate16 = (int16_t)RI;
    uint32_t immediate26 = RI & 0x2FFFFFF;
}

void execute(){
    switch(OPcode){
        case ADDI:
            Breg[rt] = Breg[rs] + Breg[immediate16];
            break;
        case ANDI:
            Breg[rt] = Breg[rs] & Breg[immediate16];
            break;
        case BEQ:
                if(Breg[rt] == Breg[rs]){
                    PC = PC + immediate16*4;
                }// nao precisa escrever o caso do else pois se não for PC simplesmente incrementa normalmente
            break;
        case BNE:
             if(Breg[rt] != Breg[rs]){
                    PC = PC + immediate16*4;
                }// nao precisa escrever o caso do else pois se não for PC simplesmente incrementa normalmente
            break;
        case J:
            PC = ((PC & 0xF0000000) | (immediate26*4)); // a label é defininda no imediato de 26 bits
            break;
        case JAL:
            Breg[31] = PC + 4; // $ra recebe o endereco da proxima instrucao
            PC = ((PC & 0xF0000000) | (immediate26*4));
            break;
        case LW:
            Breg[rt] = mem[(Breg[rs] + immediate16)/4]; // o endereco de acesso é dado pelo valor em RS + offset dado pelo imediato.
            break;
        case ORI:
            Breg[rt] = (Breg[rs] | immediate16);
            break;
        case SW:
            mem[(Breg[rt] + immediate16)/4] = Breg[rs];
            break;
        case LUI:
            Breg[rs] = immediate16 << 16;
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
                    LO = Breg[rs] * Breg[rt];
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
                    if (Breg[rs] < Breg[rt]){
                        Breg[rd] = 1;
                        }
                    else{
                        Breg[rd] = 0;
                    }
                    break;
                case JR:
                    PC = Breg[31];
                    break;
                case SLL:
                    Breg[rd] = Breg[rs] << shamt;
                    break;
                case SRL:
                    Breg[rd] = Breg[rs] >> shamt;
                    break;
                case SRA:
                    Breg[rt] = ((Breg[rt] >> shamt) | (Breg[rt]& 0x80000000)); // Shift right aritmetica srl com extensão de sinal
                    break;
                case SYSCALL:
                    break;
                case MFHI:
                    Breg[rd] =  HI;
                    break;
                case MFLO:
                    Breg[rd] =  LO;
                    break;
            }
        break;

        default:
            printf("Instrucao nao existe nesse simulador!");
            break;
    }

}
void run(){
}
void step(){
    if(PC<4000){
    fetch();
    decode();
    execute();
    }
    else
        printf("\nFim do programa\n");

}
void dump_mem(){
}
void dump_reg(){
}
int load_mem(){
    FILE *text_fp;
    FILE *data_fp;

    text_fp = fopen("text.bin", "rb");
    data_fp = fopen("data.bin", "rb");

    if(text_fp == NULL || data_fp == NULL){
        printf("\nerro ao carregar arquivos.\n");
        return 1;
    }
    else
    {
    //FAZ A LEITURA
    }

}
void start(){
// ESSA FUNCAO EH RESPONSAVEL POR EXIBIR O MENU DE FUNCOES E
// COMECAR O PROGRAMA CHAMANDO load_mem .
}

int main (){

return 0;
}
