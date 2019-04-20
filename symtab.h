
#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"


/* SIZE is the size of the hash table */
#define SIZE 211


/* the list of line numbers of the source
 * code in which a variable is referenced
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

/* The record in the bucket lists for
 * each variable, including name,
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec
   { char * name;
   	  char* scope;
      int id;
     SelectKind select;
     LineList lines;
     TreeNode *treeNode;
     int memloc ; /* memory location for variable */
     struct BucketListRec * next;
   } * BucketList;

BucketList l;

typedef struct ScopeRec
   { char * funcName;
     int nestedLevel;
     struct ScopeRec * parent;
     BucketList hashTable[SIZE]; /* the hash table */
   } * Scope;


Scope globalScope;

int hash ( char * key );
/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert( char * name, int lineno, int loc, TreeNode * treeNode, SelectKind s);

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup ( char * name );
int st_add_lineno(char * name, int lineno);
BucketList st_bucket( char * name );
int st_lookup_top (char * name);
int st_lookup_top_node (char * name);
int st_find(TreeNode* tree, int loc);
TreeNode* AddrFind (char* name);
void allocateVarMemLoc (char* name, int reg);
int getVarMemLoc (char* name);
int st_find(TreeNode* tree, int loc);
int setfuncRet(TreeNode* tree, int pos);
int set_value(char* name, int loc);
void st_delete(char * name, TreeNode * treeNode);
void setFunc(char * name, int loc, TreeNode * t);


Scope sc_create(char *funcName);
Scope sc_top( void );
void sc_pop( void );
void sc_push( Scope scope );
int addLocation( void );
int returnLocation();

int verifica (char * name );
/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */
void printSymTab(FILE * listing);

#endif
