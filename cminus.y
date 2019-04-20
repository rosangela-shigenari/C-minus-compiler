%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *
static char * savedName; /* for use in assignments */
static int savedNumber;
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void);
int yyerror(char * message);
char* nome;

%}

%token ID NUM SEMI LCHAVE RCHAVE INT VOID FLOAT LPAREN RPAREN LCOLCH RCOLCH
%token IF ELSE WHILE RETURN MENOREQ MAIOREQ EQEQ DIF LT RT EQ
%token PLUS MINUS TIMES OVER COMMA
%token ERROR

%% /* Grammar for TINY */

programa     : decl_lista
                 { savedTree = $1;}
            ;
decl_lista  : decl_lista decl
     	    { YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $2;
                     $$ = $1; }
                     else $$ = $2;
                 }
            | decl
	        { $$ = $1; }
            ;
decl        : var_decl
		 { $$ = $1; }
	    | fun_decl
		{ $$ = $1; }
	    ;
saveName    : ID
                 { savedName = copyString(tokenString);
                   savedLineNo = lineno;
                   nome = savedName;
                 }
            ;
saveNumber  : NUM
                 {
		   savedNumber = atoi(tokenString);
                   savedLineNo = lineno;
                 }
            ;
var_decl    : tipo_espec saveName SEMI
		 { $$ = newDeclNode(VarK);
                   $$->child[0] = $1;
                   $$->lineno = lineno;
                   $$->attr.name = savedName;
                 }
	    | tipo_espec saveName LCHAVE saveNumber RCHAVE SEMI
		 { $$ = newDeclNode(VetK);
                   $$->child[0] = $1;
                   $$->lineno = lineno;
                   $$->attr.arr.name = savedName;
                   $$->attr.arr.size = savedNumber;
                 }
            ;
tipo_espec  : INT
		 { $$ = newTypeNode(TypeNameK);
                   $$->attr.type = INT;
                 }
	    | VOID
		  { $$ = newTypeNode(TypeNameK);
                   $$->attr.type = VOID;
                 }
            ;
fun_decl    : tipo_espec saveName {
                   $$ = newDeclNode(FunctionK);
                   $$->lineno = lineno;
                   $$->attr.name = savedName;
                 }
              LPAREN params RPAREN composto_decl
                 {
                   $$ = $3;
                   $$->child[0] = $1;
                   $$->child[1] = $5;
                   $$->child[2] = $7;
                 }
            ;
params      : param_lista
		{ $$ = $1; }
            | VOID
		 { $$ = newTypeNode(TypeNameK);
                   $$->attr.type = VOID;
                 }
	    ;
param_lista : param_lista COMMA param
                 { YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $3;
                     $$ = $1; }
                     else $$ = $3;
                 }
            | param { $$ = $1; }
	    ;
param       : tipo_espec saveName
		 { $$ = newParamNode(NonArrParamK);
                   $$->child[0] = $1;
                   $$->attr.name = savedName;
                 }
	    | tipo_espec saveName LCHAVE RCHAVE
		 { $$ = newParamNode(ArrParamK);
                   $$->child[0] = $1;
                   $$->attr.name = savedName;
                 }
	    ;
composto_decl : LCOLCH local_decl stmt_lista RCOLCH
		  { $$ = newStmtNode(CompoundK);
                   $$->child[0] = $2;
                   $$->child[1] = $3;
                 }
	    ;
local_decl  : local_decl var_decl
		   { YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $2;
                     $$ = $1; }
                     else $$ = $2;
                 }
	    | /* empty */ { $$ = NULL; }
	    ;
stmt_lista  : stmt_lista stmt
                 { YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $2;
                     $$ = $1; }
                     else $$ = $2;
                 }
            | /* empty */ { $$ = NULL; }
	    ;
stmt        :  exp_decl
		{ $$ = $1; }
  	    | composto_decl
		{ $$ = $1; }
	    | selecao_decl
		{ $$ = $1; }
	    | iteracao_decl
		{ $$ = $1; }
	    | retorno_decl
		{ $$ = $1; }
	    ;
exp_decl    :  exp SEMI
		{ $$ = $1; }
	    | SEMI
		{ $$ = NULL; }
	    ;
selecao_decl : IF LPAREN exp RPAREN stmt
		{ $$ = newStmtNode(IfK);
                   $$->child[0] = $3;
                   $$->child[1] = $5;

                 }
	    | IF LPAREN exp RPAREN stmt ELSE stmt
		 { $$ = newStmtNode(IfK);
                   $$->child[0] = $3;
                   $$->child[1] = $5;
                   $$->child[2] = $7;
                 }
	    ;
iteracao_decl : WHILE LPAREN exp RPAREN stmt
		{ $$ = newStmtNode(IterK);
                   $$->child[0] = $3;
                   $$->child[1] = $5;
                 }
	    ;
retorno_decl : RETURN SEMI
		{ $$ = newStmtNode(ReturnK);
                   $$->child[0] = NULL;
                 }
	    | RETURN exp SEMI
		{ $$ = newStmtNode(ReturnK);
                   $$->child[0] = $2;
                 }
	    ;
exp         : var EQ exp
	     { $$ = newExpNode(AssignK);
         $$->child[0] = $1;
         $$->child[1] = $3;
         }
        | simples_exp
    		 { $$ = $1; }
           ;
var         : saveName
             { $$ = newExpNode(IdK);
               $$->attr.name = savedName;
               $$->child[0] = $1;
                                   }
              | saveName {
                $$ = newExpNode(ArrIdK);
                $$->attr.name = savedName;
                }LCHAVE exp RCHAVE
                 {
                   $$ = $2;
                   $$->child[0] = $1;
                   $$->child[1] = $4;
                 }
                              ;
simples_exp  : add_exp MENOREQ add_exp
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = MENOREQ;
                 }
            | add_exp LT add_exp
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = LT;
                 }
            | add_exp RT add_exp
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = RT;
                 }
            | add_exp MAIOREQ add_exp
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = MAIOREQ;
                 }
            | add_exp EQEQ add_exp
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = EQ;
                 }
            | add_exp DIF add_exp
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = DIF;
                 }
            | add_exp { $$ = $1; }
            ;
add_exp     : add_exp PLUS termo
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = PLUS;
                 }
            | add_exp MINUS termo
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = MINUS;
                 }
            | termo { $$ = $1; }
            ;
termo        : termo mult fator
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = $2 ->attr.op;
                 }
            | fator { $$ = $1; }
            ;
mult        : TIMES{ $$ = newTypeNode(TypeNameK);
                    $$->attr.op = TIMES;
              }
              |OVER { $$ = newTypeNode(TypeNameK);
                      $$->attr.op = OVER;
              }
fator      : LPAREN exp RPAREN { $$ = $2; }
            | var { $$ = $1; }
            | call { $$ = $1; }
            | NUM
                 { $$ = newExpNode(ConstK);
                   $$->attr.val = atoi(tokenString);
                 }

            ;
call        : saveName {
                 $$ = newExpNode(CallK);
                 $$->attr.name = savedName;
              }
              LPAREN args RPAREN
                 { $$ = $2;
                   $$->child[0] = $4;
                 }
            ;
args        : arg_list { $$ = $1; }
            | /* empty */ { $$ = NULL; }
            ;
arg_list    : arg_list COMMA exp
                 { YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $3;
                     $$ = $1; }
                     else $$ = $3;
                 }
            | exp { $$ = $1; }
            ;
%%

int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex ativacaos getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}
