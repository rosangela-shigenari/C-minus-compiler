
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "symtab.h"


/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

#define MAX_SCOPE 1000
int identif =0;
int memCounter = 0;
/* the hash function */
int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

static Scope scopes[MAX_SCOPE];
static int nScope = 0;
static Scope scopeStack[MAX_SCOPE];
static int nScopeStack = 0;
static int location[MAX_SCOPE];

Scope sc_top( void )
{ return scopeStack[nScopeStack - 1];
}

void sc_pop( void )
{
  //printf("pop %s\n", sc_top()->funcName);
  --nScopeStack;
}

int addLocation( void )
{
  return location[nScopeStack - 1]++;
}

int returnLocation(){
  return location[nScopeStack-1];
}

void sc_push( Scope scope )
{ scopeStack[nScopeStack] = scope;
  location[nScopeStack++] = 0;
  //printf("push %s\n", scope->funcName);
}

Scope sc_create(char *funcName)
{ Scope newScope;

  newScope = (Scope) malloc(sizeof(struct ScopeRec));
  newScope->funcName = funcName;
  newScope->nestedLevel = nScopeStack;
  newScope->parent = sc_top();

  scopes[nScope++] = newScope;

  return newScope;
}

BucketList st_bucket( char * name )
{ int h = hash(name);
  Scope sc = sc_top();
  while(sc) {
    l = sc->hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0))
      l = l->next;
    if (l != NULL) return l;
    sc = sc->parent;
  }
  return NULL;
}


/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(char * name, int lineno, int loc, TreeNode * treeNode, SelectKind s)
{ if(s!= FUNC)
    identif++;
  else
    identif = 0;
  int h = hash(name);
  Scope top = sc_top();
  l =  top->hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) /* variable not yet in table */
  { l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->treeNode = treeNode;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->select = s;
    l->id = identif;
    l->lines->next = NULL;
    l->next = top->hashTable[h];
    top->hashTable[h] = l; }
  else /* found in table, so just add line number */
  { // ERROR!
  }
} /* st_insert */


void setFunc(char * name, int loc, TreeNode * t)
{   l = st_bucket(name);

    l->treeNode = t;
    t->memloc = loc;
} /* st_insert */

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup ( char * name )
{ l = st_bucket(name);

  if (l != NULL){
    return l->memloc;
  }
  return -1;
}

void allocateVarMemLoc (char * name, int reg){
  l = st_bucket(name);

    if (l != NULL){
      if(l->memloc==0)
        l->memloc = reg;
    }
    return;
}

int st_lookup_top (char * name)
{ int h = hash(name);
  Scope sc = sc_top();
  while(sc) {
    l = sc->hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0))
      l = l->next;
    if (l != NULL){
      return l->memloc;
    }
    break;
  }
  return -1;
}


int verifica (char * name )
{
l = st_bucket(name);
  if (l->select== FUNC)
  		return 1;
  else return 0;
}
int st_find (TreeNode* tree, int loc)
{
  l = st_bucket(tree->attr.name);
  if(l == NULL)
    st_insert(tree->attr.name, 0, loc, tree, VAR);

  return l->memloc;
}
int getVarMemLoc ( char * name )
{ l = st_bucket(name);

  if (l != NULL){
    return l->memloc;
  }
  return -1;
}

int set_value(char* name, int loc){
  l = st_bucket(name);
  l->memloc = loc;

  return l->memloc;
}

int setfuncRet(TreeNode* tree, int pos)
{
  l = st_bucket(tree->attr.name);
  if(l == NULL)
    st_insert(tree->attr.name, 0, pos, tree, FUNC);

  return l->memloc;
}


TreeNode* AddrFind (char* name)
{
  TreeNode* tree;
  l = st_bucket(name);
  tree = l->treeNode;

  return tree;
}



int st_lookup_top_node (char * name)
{ int h = hash(name);
  Scope sc = sc_top();
  while(sc) {
    l = sc->hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0))
      l = l->next;
    if (l != NULL){
        TreeNode *node = l->treeNode;
        switch (node->type) {
        case Void:
              return 1;
          break;
        }
    }
    break;
  }
  return -1;
}



int st_add_lineno(char * name, int lineno)
{ l = st_bucket(name);
  LineList ll = l->lines;
  while (ll->next != NULL) ll = ll->next;
  ll->next = (LineList) malloc(sizeof(struct LineListRec));
  ll->next->lineno = lineno;
  ll->next->next = NULL;
}

void printSymTabRows(BucketList *hashTable, FILE *listing) {
  int j;
  for (j=0;j<SIZE;++j)
  { if (hashTable[j] != NULL)
    { l = hashTable[j];
      TreeNode *node = l->treeNode;

      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing,"%-14s ",l->name);
        fprintf(listing,"%4d ",l->memloc);
        switch (node->nodekind) {
        case DeclK:
          switch (node->kind.decl) {
           case FunctionK:
          	if(node->type == Void)
           		 fprintf(listing,"%8s", "             VOID Function               ");
           	else  fprintf(listing,"%8s", "             INT Function               ");
            break;
          case VarK:
            if(node->type == Integer)
           		 fprintf(listing,"%8s", "             INT Variable               ");
           	else  fprintf(listing,"%8s", "             BOOLEAN Variable               ");
            break;
          case VetK:
           if(node->type == Integer)
           		 fprintf(listing,"%8s", "             INT Vector                  ");
           	else  fprintf(listing,"%8s", "             BOOLEAN Vector               ");
            break;

          default:
            break;
          }
          break;
        case ParamK:
          switch (node->kind.param) {
          case NonArrParamK:
            if(node->type == Integer)
           		 fprintf(listing,"%15s", "             INT Variable               ");
           	else  fprintf(listing,"%15s", "             BOOLEAN Variable               ");
            break;
          case ArrParamK:
             if(node->type == Integer)
           		 fprintf(listing,"%15s", "             INT Vector                 ");
           	else  fprintf(listing,"%15s", "             BOOLEAN Vector               ");
            break;
          default:
            break;
          }
          break;
        default:
          break;
        }

        while (t != NULL)
        { if(t->lineno>-1)
       		 fprintf(listing,"%4d ",t->lineno);
          t = t->next;
        }

        fprintf(listing,"\n");
        l = l->next;
      }
    }
  }
}

/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */
void printSymTab(FILE * listing)
{ int i;

  for (i = 0; i < nScope; ++i) {
    Scope scope = scopes[i];
    BucketList * hashTable = scope->hashTable;

    if (i == 0){
    fprintf(listing, "                           Scope: Global\n\n");
    }    // global scope
    else{
    	if(scope->nestedLevel <=1)
   			 fprintf(listing, "                           Scope:%s \n\n", scope->funcName);
    }
    if(scope->nestedLevel <=1){
	fprintf(listing,"Name of var/func               Type                  		Location\n");
    fprintf(listing,"________________            _______________          ____________________________________\n");
}
    printSymTabRows(hashTable, listing);
    printf("\n");
  }
} /* printSymTab */
