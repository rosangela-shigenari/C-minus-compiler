
%{
#include "globals.h"
#include "util.h"
#include "scan.h"

char tokenString[MAXTOKENLEN+1];
static int yylex(void);
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"if"            {return IF;}
"else"            {return ELSE;}
"int"           {return INT;}
"float"         {return FLOAT;}
"return"        {return RETURN;}
"void"          {return VOID;}
"while"         {return WHILE;}

"<="             {return MENOREQ;} 
">="             {return MAIOREQ;}
"=="            {return EQEQ;}
"!="            {return DIF;}

"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}


"/*"     { 
           register int c;
           for (;;)
           {
             while ((c = input ()) != '*' && c != EOF)
               ; 
             if (c == '*')
             {
               while ((c = input ()) == '*')
                 ;
               if (c == '/')
                 break; 
             }

             if (c == EOF)
             {
               break;
             }
           }

         }
"/"             {return OVER;}	



"<"             {return LT;}
">"             {return RT;}
"="             {return EQ;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"["             {return LCHAVE;}
"]"             {return RCHAVE;}
"{"             {return LCOLCH;}
"}"             {return RCOLCH;}


";"             {return SEMI;}

","             {return COMMA;}
{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}  
.               {return ERROR;}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}


