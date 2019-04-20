
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Yacc/Bison generates internally its own values
 * for the tokens. Other files can access these values
 * by including the tab.h file generated using the
 * Yacc/Bison option -d ("generate header")
 *
 * The YYPARSER flag prevents inclusion of the tab.h
 * into the Yacc/Bison output itself
 */

#ifndef YYPARSER

/* the name of the following file may change */
#include "cminus.tab.h"

/* ENDFILE is implicitly defined by Yacc/Bison,
 * and not included in the tab.h file
 */
#define ENDFILE 0

#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 8

/* Yacc/Bison generates its own integer values
 * for tokens
 */
typedef int TokenType;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */
int memloc;
extern int lineno; /* source line number for listing */

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum {StmtK,ExpK,DeclK,ParamK,TypeK} NodeKind;
typedef enum {IntegerK, ReturnK,IfK, VoidK, ElseK ,CompoundK,IterK} StmtKind;
typedef enum {VarK,VetK, FunctionK} DeclKind;
typedef enum {AssignK,OpK,ConstK,IdK,SizeK, ArrIdK,CallK} ExpKind;
typedef enum {TypeNameK} TypeKind;
typedef enum {ArrParamK,NonArrParamK} ParamKind;

typedef enum {FUNC, VAR} SelectKind;

/* ExpType is used for type checking */
typedef enum {Void,Integer,Boolean,IntegerArray} ExpType;


/* ArrayAttr is used for attributes for array variables */
typedef struct arrayAttr {
    TokenType type;
    char * name;
    char * info;
    SelectKind select;
    int size;
} ArrayAttr;


#define MAXCHILDREN 3

struct ScopeRec;

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     int memloc;
     NodeKind nodekind;
     union { StmtKind stmt;
             ExpKind exp;
             DeclKind decl;
             ParamKind param;
             TypeKind type; } kind;
     union { TokenType op;
             TokenType type;
             int val;
             SelectKind select;
             char * name;
             char * info;
             int id;
             ArrayAttr arr;
             struct ScopeRec * scope; } attr;
     ExpType type; /* for type checking of exps */
   } TreeNode;

//instrucoes do processador

static char *adicao = "ADC";
static char *subtracao = "SUB";
static char *adicao_imediato = "ADCI";
static char *subtracao_imediato = "SUBI";
static char * store_pag = "SPAGE";
static char *load_pag = "LPAGE";
static char *countPC = "CPC";
static char *load_word = "LOWO";
static char *store_word = "STWO";
static char *load_immediate = "LOI";
static char *movimentacao = "MOV";
static char *insert_inst = "INSERT";
static char *desloc_direito = "SRIL";
static char *jump = "JMP";
static char *jump_register = "JMPR";
static char *saveProc = "SPROC";
static char *branch_not_equal = "BNEQ";
static char *branch_less_zero = "BLZ";
static char *set_less_than = "SLET";
static char *set_greater_than = "SGRT";
static char *entrada = "IN";
static char *saida = "OUT";
static char *nop = "NOP";
static char *halt = "HALT";
static char *multiplicao = "MULT";
static char *multiplicao_imediato = "MULTI";
static char *jump_and_link = "JAL";
static char *divisao = "DIV";
static char *copy = "COPY";
static char *loadReg= "LR";
static char *storeReg = "SR";
static char *reset = "RESET";
static char *inCMD = "inCMD";
static char *emitCMD = "emit";
static char *loi6bits = "li6";
static char *savePC = "savePC";
static char *saveadd_buffer = "saveadd_buffer";
static char *renameProg = "renameProg";
static char *troca_contexto = "switchCtx";
static char *renameProgReg = "renameProgReg";

static char * binValueFromOp (char* operacao){

  char * opcode;
  if(operacao == adicao)
    opcode = "000000";
  else if(operacao == subtracao)
    opcode = "000001";
  else if(operacao == adicao_imediato)
    opcode = "000010";
  else if(operacao == subtracao_imediato)
    opcode = "000011";
  else if(operacao == store_pag)
    opcode = "000100";
  else if(operacao == load_pag)
    opcode = "000101";
  else if(operacao == countPC)
    opcode = "000110";
  else if(operacao == load_word)
    opcode = "000111";
  else if(operacao == store_word)
    opcode = "001000";
  else if(operacao == load_immediate)
    opcode = "001001";
  else if(operacao == movimentacao)
    opcode = "001010";
  else if(operacao == insert_inst)
    opcode = "001011";
  else if(operacao == desloc_direito)
    opcode = "001100";
  else if(operacao == jump)
    opcode = "001101";
  else if(operacao == jump_register)
    opcode = "001110";
  else if(operacao == saveProc)
    opcode = "001111";
  else if(operacao == branch_not_equal)
    opcode = "010000";
  else if(operacao == branch_less_zero)
    opcode = "010001";
  else if(operacao == set_less_than)
    opcode = "010010";
  else if(operacao == set_greater_than)
    opcode = "010011";
  else if(operacao == entrada)
    opcode = "010100";
  else if(operacao == saida)
    opcode = "010101";
  else if(operacao == nop)
    opcode = "010110";
  else if(operacao == halt)
    opcode = "010111";
  else if(operacao == multiplicao)
    opcode = "011000";
  else if(operacao == multiplicao_imediato)
    opcode = "011001";
  else if(operacao == jump_and_link)
    opcode = "011010";
  else if(operacao == divisao)
    opcode = "011011";
  else if (operacao ==copy)
    opcode = "011100";
  else if(operacao == storeReg)
    opcode = "011110";
  else if(operacao == loadReg)
    opcode = "011101";
  else if(operacao == reset)
    opcode = "011111";

  else if(operacao == inCMD)
      opcode = "100000";
  else   if(operacao == emitCMD)
      opcode = "100001";
  else   if(operacao == loi6bits)
      opcode = "100010";
  else if (operacao == savePC)
    opcode = "100011";
  else if (operacao == saveadd_buffer)
      opcode = "100100";

  else if(operacao == renameProg)
    opcode = "100101";
  else if(operacao == troca_contexto)
    opcode = "100110";
  else if(operacao == renameProgReg)
    opcode = "100111";
  else
    printf("OPCODE INEXISTENTE!\n");

  return opcode;
}
/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes ments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error;
#endif
