#include "globals.h"
#include "code.h"
#include "util.h"
#include "stdio.h"

int binaryToInstructionMem = 1;
int skipPrintFlag = 0;
int lineCounterAssembly = 1;
int aux_count = 0;


int getLineCounterAssembly(){
  return lineCounterAssembly;
}
void setLineCounterAssembly(int newlineCounterAssembly){
  lineCounterAssembly = newlineCounterAssembly;
}
int getSkipPrint(){
  return skipPrintFlag;
}
void setSkipPrint(int i){
  skipPrintFlag = i;
}



char *binValueFromInt(int n, int length){
  int c, d, count;
  char *pointer;

  count = 0;
  pointer = (char*) malloc (32+1);

  if(pointer == NULL)
    exit(EXIT_FAILURE);

  for(c = length-1; c>=0; c--){
    d = n>>c;
    if(d&1)
      *(pointer + count) = 1 + '0';
    else
      *(pointer + count) = 0 + '0';
    count++;
  }
  *(pointer + count) = '\0';
  return pointer;
}
void FunctionMain(){
  if(skipPrintFlag)
    return;
  if(binaryToInstructionMem){

    sprintf(linhas[0], "MEM_INSTRUCTION[%d] = 32'b%s%s; /*%-6s  %-2d */\n", 0, "001101", binValueFromInt(lineCounterAssembly-1,26), "jump", lineCounterAssembly -1);

  }

  else
    sprintf(linhas[0], "%4d:\t%-6s $%-2d\n", 0, jump, lineCounterAssembly-1);
}

void printLines(/* arguments */) {
  /* code */
  int i;
  FILE *f = fopen("assembly.txt", "w");
  if(f == NULL){
    printf("Erro ao abrir o arquivo\n");
    exit(EXIT_FAILURE);
  }
  for(i = 0; i<lineCounterAssembly; i++){
      if(binaryToInstructionMem)
        fprintf(f,"%s", linhas[i]);
    printf("%s\n",linhas[i] );
  }
  fclose(f);
}

//definicao dos formatos de instrucao
/*
OneRegType: 5 bits de opcode, 5 de reg1 e 22 de imediato
DoubleRegType: 5 bits de opcode, 5 de reg1, 5 reg2 e 17 de imediato
TriRegType: 5 bits de opcode, 5 de reg1, 5 bits de reg2, 5 de reg3 e 12 de imediato
JNH Type: 5 bits de opcode e 27 de imediato
*/

void HDtypeFormat (char *opcode, int process, int reg){
  if(skipPrintFlag)
    lineCounterAssembly++;
  if(binaryToInstructionMem){
    sprintf(linhas[lineCounterAssembly], "MEM_INSTRUCTION[%d] = 32'b%s%s%s0000000000000000; /*%-6s  %-2d, %-2d*/\n",  lineCounterAssembly  , binValueFromOp(opcode), binValueFromInt(process,4),binValueFromInt(reg,6), opcode, process, reg);
  }else
    sprintf(linhas[lineCounterAssembly], "%4d:\t%-6s  %-2d, %-2d\n",  lineCounterAssembly  , opcode, process, reg);
  lineCounterAssembly++;
}

void COPYtypeFormat (char *opcode, int posHD, int posMEM){
  if(skipPrintFlag)
    lineCounterAssembly++;
  if(binaryToInstructionMem){
    sprintf(linhas[lineCounterAssembly], "MEM_INSTRUCTION[%d] = 32'b%s%s%s; /*%-6s  %-2d, %-2d*/\n",  lineCounterAssembly  , binValueFromOp(opcode), binValueFromInt(posHD,5),binValueFromInt(posMEM,21), opcode, posHD, posMEM);
  }else
    sprintf(linhas[lineCounterAssembly], "%4d:\t%-6s  %-2d, %-2d\n",  lineCounterAssembly  , opcode, posHD, posMEM);
  lineCounterAssembly++;
}

void JNHTypeFormat (char *opcode, int immediate27){
  if(skipPrintFlag){
    lineCounterAssembly++;

  }

  if(binaryToInstructionMem){
    sprintf(linhas[lineCounterAssembly], "MEM_INSTRUCTION[%d] = 32'b%s%s; /*%-6s  %-2d */\n",  lineCounterAssembly  , binValueFromOp(opcode), binValueFromInt(immediate27,26), opcode, immediate27);
  }else
    sprintf(linhas[lineCounterAssembly], "%4d:\t%-6s  %-2d\n",  lineCounterAssembly  , opcode, immediate27);
  lineCounterAssembly++;
}

void OneRegTypeFormat (char *opcode, int reg, int immediate22){
  if(skipPrintFlag)
    lineCounterAssembly++;
  if(binaryToInstructionMem){
    sprintf(linhas[lineCounterAssembly], "MEM_INSTRUCTION[%d] = 32'b%s%s%s;/*%-6s $%-2d  %-2d */\n",  lineCounterAssembly  , binValueFromOp(opcode), binValueFromInt(reg, 5), binValueFromInt(immediate22,21), opcode, reg, immediate22);
  }
  else
    sprintf(linhas[lineCounterAssembly], "%4d:\t%-6s $%-2d  %-2d\n",  lineCounterAssembly  , opcode, reg, immediate22);
  lineCounterAssembly++;
}
void OneRegTypeFormat6BITS (char *opcode, int reg, int immediate20){
  if(skipPrintFlag)
    lineCounterAssembly++;
  if(binaryToInstructionMem){
    sprintf(linhas[lineCounterAssembly], "MEM_INSTRUCTION[%d] = 32'b%s%s%s;/*%-6s $%-2d  %-2d */\n",  lineCounterAssembly  , binValueFromOp(opcode), binValueFromInt(reg, 6), binValueFromInt(immediate20,20), opcode, reg, immediate20);
  }
  else
    sprintf(linhas[lineCounterAssembly], "%4d:\t%-6s $%-2d  %-2d\n",  lineCounterAssembly  , opcode, reg, immediate20);
  lineCounterAssembly++;
}

void DoubleRegTypeFormat (char *opcode, int reg1, int reg2, int immediate17){
  if(skipPrintFlag)
    lineCounterAssembly++;
  if(binaryToInstructionMem){
    sprintf(linhas[lineCounterAssembly], "MEM_INSTRUCTION[%d] = 32'b%s%s%s%s;/*%-6s $%-2d $%-2d  %-2d*/\n",  lineCounterAssembly  , binValueFromOp(opcode), binValueFromInt(reg1, 5), binValueFromInt(reg2,5),binValueFromInt(immediate17,16), opcode, reg1, reg2, immediate17);
  }
  else
    sprintf(linhas[lineCounterAssembly], "%4d:\t%-6s $%-2d $%-2d  %-2d\n",  lineCounterAssembly  , opcode, reg1, reg2, immediate17);
  lineCounterAssembly++;
}

void TriRegTypeFormat (char *opcode, int reg1, int reg2, int reg3){
  if(skipPrintFlag)
    lineCounterAssembly++;
  if(binaryToInstructionMem){
    sprintf(linhas[lineCounterAssembly], "MEM_INSTRUCTION[%d] = 32'b%s%s%s%s00000000000; /*%-6s $%-2d $%-2d  $%-2d*/\n",  lineCounterAssembly  , binValueFromOp(opcode), binValueFromInt(reg1, 5), binValueFromInt(reg2,5),binValueFromInt(reg3,5), opcode, reg1, reg2, reg3);
  }
  else
    sprintf(linhas[lineCounterAssembly], "%4d:\t%-6s $%-2d $%-2d  $%-2d\n",  lineCounterAssembly  , opcode, reg1, reg2, reg3);
  lineCounterAssembly++;

}
