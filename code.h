#ifndef _CODE_H_
#define _CODE_H_

char linhas[30000][10000];

int getLineCounterAssembly();
void setLineCounterAssembly(int newlineCounterAssembly);
int getSkipPrint();
void setSkipPrint(int i);
void printLines();
void FunctionMain();

int SaveRegister(char* savedName);
int Branch_Addr(int loc);

void INSERTtypeFormat (char *opcode, int formatID, int campo, int info);
void COPYtypeFormat (char *opcode, int posHD, int posMEM);
void HDtypeFormat (char *opcode, int process, int reg);
void JNHTypeFormat (char *opcode, int immediate27);
void OneRegTypeFormat (char *opcode, int reg, int immediate22);
void DoubleRegTypeFormat (char *opcode, int reg1, int reg2, int immediate17);
void TriRegTypeFormat (char *opcode, int reg1, int reg2, int reg3);
void OneRegTypeFormat6BITS (char *opcode, int reg, int immediate20);

#endif
