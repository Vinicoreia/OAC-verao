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
    RI= (uint32_t)mem[PC/4]; // Como nesse simulador incrementamos o PC em 4 (como é feito no MIPS) devemos acessar a memória dividindo seu indice por 4.
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
            }
        break;

        default:
            printf("This is instruction is not implemented here!");
            break;
    }

}
void run(){
    while(PC<4096){
    step();
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
    unsigned char str[4];
    uint8_t c1, c2, c3, c4;
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
        //FAZ A LEITURA
        while(!feof(text_fp)){//Ver como isso funciona direito
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
    int opcao;
    printf("\nWich function would you like to execute?\n\n");

    printf("1- Run Program\n2- Run Step.\n3- Dump Memory\n4- Dump register values\n\n");
    switch(opcao){
        case 1:
            run();
            break;
        case 2:
            step();
            break;
        case 3://dump_mem
            break;
        case 4:
            dump_reg('h');
            break;
        default:
            printf("invalid operation!");
            break;
    }
}

int main (){
    load_to_mem();
    dump_mem(0,10,'g');
   // start();

return 0;
}
