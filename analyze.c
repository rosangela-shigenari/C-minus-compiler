
#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

static char * funcName;
static int preserveLastScope = FALSE;
int verify_main = 0;

static void traverse( TreeNode * t,
 void (* preProc) (TreeNode *),
 void (* postProc) (TreeNode *) )
{ if (t != NULL)
  { preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

static void insertInput(void)
{ TreeNode *in, *stmt, *tipo;
  TreeNode *param;

  in = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = INT;
  in->type = Integer;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  in->lineno = 0;
  in->attr.name = "input";
  in->child[0] = tipo;
  in->child[1] = NULL;
  in->child[2] = stmt;

  st_insert("input", -1, addLocation(), in, FUNC);}

static void insertOutput(void){
  TreeNode *out, *stmt, *tipo;
  TreeNode *param;

  out = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  out->type = Void;

  param = newParamNode(NonArrParamK);
  param->attr.name = "arg";
  param->child[0] = newDeclNode(FunctionK);
  param->child[0]->attr.type = INT;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  out->lineno = 0;
  out->attr.name = "output";
  out->child[0] = tipo;
  out->child[1] = param;
  out->child[2] = stmt;

  st_insert("output", -1, addLocation(), out, FUNC);
}

static void insertCopy(void){
  TreeNode *cpy, *stmt, *tipo;
  TreeNode *param;

  cpy = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  cpy->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  cpy->lineno = 0;
  cpy->attr.name = "copyCS";
  cpy->child[0] = tipo;
  cpy->child[1] = NULL;
  cpy->child[2] = stmt;

  st_insert("copyCS", -1, addLocation(), cpy, FUNC);
}

static void insertLoadReg(void){
  TreeNode *lr, *stmt, *tipo;
  TreeNode *param;

  lr = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  lr->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  lr->lineno = 0;
  lr->attr.name = "LoadRegister";
  lr->child[0] = tipo;
  lr->child[1] = NULL;
  lr->child[2] = stmt;

  st_insert("LoadRegister", -1, addLocation(), lr, FUNC);
}

static void insertStoreReg(void){
  TreeNode *sr, *stmt, *tipo;
  TreeNode *param;

  sr = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  sr->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  sr->lineno = 0;
  sr->attr.name = "StoreRegister";
  sr->child[0] = tipo;
  sr->child[1] = NULL;
  sr->child[2] = stmt;

  st_insert("StoreRegister", -1, addLocation(), sr, FUNC);
}

static void insertBIOS(void){
  TreeNode *lb, *stmt, *tipo;
  TreeNode *param;

  lb = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  lb->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  lb->lineno = 0;
  lb->attr.name = "LoadBIOS";
  lb->child[0] = tipo;
  lb->child[1] = NULL;
  lb->child[2] = stmt;

  st_insert("LoadBIOS", -1, addLocation(), lb, FUNC);
}

static void insertFIFOVazia(void){
  TreeNode *fv, *stmt, *tipo;
  TreeNode *param;

  fv = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  fv->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  fv->lineno = 0;
  fv->attr.name = "FIFOvazia";
  fv->child[0] = tipo;
  fv->child[1] = NULL;
  fv->child[2] = stmt;

  st_insert("FIFOvazia", -1, addLocation(), fv, FUNC);
}

static void insertRefazer(void){
  TreeNode *ref, *stmt, *tipo;
  TreeNode *param;

  ref = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  ref->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  ref->lineno = 0;
  ref->attr.name = "Refaz";
  ref->child[0] = tipo;
  ref->child[1] = NULL;
  ref->child[2] = stmt;

  st_insert("Refaz", -1, addLocation(), ref, FUNC);
}

static void insertExecuteProc(void){
  TreeNode *proc, *stmt, *tipo;
  TreeNode *param;

  proc = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  proc->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  proc->lineno = 0;
  proc->attr.name = "Executa";
  proc->child[0] = tipo;
  proc->child[1] = NULL;
  proc->child[2] = stmt;

  st_insert("Executa", -1, addLocation(), proc, FUNC);
}

static void insertAddictAddr(void){
  TreeNode *add, *stmt, *tipo;
  TreeNode *param;

  add = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  add->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  add->lineno = 0;
  add->attr.name = "AddictAddr";
  add->child[0] = tipo;
  add->child[1] = NULL;
  add->child[2] = stmt;

  st_insert("AddictAddr", -1, addLocation(), add, FUNC);
}

static void insertPagingLoad(void){
  TreeNode *pgl, *stmt, *tipo;
  TreeNode *param;

  pgl = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  pgl->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  pgl->lineno = 0;
  pgl->attr.name = "LoadingPage";
  pgl->child[0] = tipo;
  pgl->child[1] = NULL;
  pgl->child[2] = stmt;

  st_insert("LoadingPage", -1, addLocation(), pgl, FUNC);
}

static void insertPagingStore(void){
  TreeNode *pgs, *stmt, *tipo;
  TreeNode *param;

  pgs = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  pgs->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  pgs->lineno = 0;
  pgs->attr.name = "StoringPage";
  pgs->child[0] = tipo;
  pgs->child[1] = NULL;
  pgs->child[2] = stmt;

  st_insert("StoringPage", -1, addLocation(), pgs, FUNC);
}

static void insertMenu(void){
  TreeNode *menu, *stmt, *tipo;
  TreeNode *param;

  menu = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  menu->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  menu->lineno = 0;
  menu->attr.name = "createMenu";
  menu->child[0] = tipo;
  menu->child[1] = NULL;
  menu->child[2] = stmt;

  st_insert("createMenu", -1, addLocation(), menu, FUNC);

}

static void insertFIFOCreate(void){
  TreeNode *insert, *stmt, *tipo;
  TreeNode *param;

  insert = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  insert->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  insert->lineno = 0;
  insert->attr.name = "criaFIFO";
  insert->child[0] = tipo;
  insert->child[1] = NULL;
  insert->child[2] = stmt;

  st_insert("criaFIFO", -1, addLocation(), insert, FUNC);

}

static void insertRenameProg(void){
  TreeNode *ren, *stmt, *tipo;
  TreeNode *param;

  ren = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  ren->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  ren->lineno = 0;
  ren->attr.name = "renomearPrograma";
  ren->child[0] = tipo;
  ren->child[1] = NULL;
  ren->child[2] = stmt;

  st_insert("renomearPrograma", -1, addLocation(), ren, FUNC);
}

static void insertExcluirProg(void){
  TreeNode *exc, *stmt, *tipo;
  TreeNode *param;

  exc = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  exc->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  exc->lineno = 0;
  exc->attr.name = "excluirPrograma";
  exc->child[0] = tipo;
  exc->child[1] = NULL;
  exc->child[2] = stmt;

  st_insert("excluirPrograma", -1, addLocation(), exc, FUNC);
}

static void insertExecutarProg(void){
  TreeNode *exe, *stmt, *tipo;
  TreeNode *param;

  exe = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  exe->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  exe->lineno = 0;
  exe->attr.name = "executarPrograma";
  exe->child[0] = tipo;
  exe->child[1] = NULL;
  exe->child[2] = stmt;

  st_insert("executarPrograma", -1, addLocation(), exe, FUNC);
}


static void insertRetiraFIFO(void){
  TreeNode *retira, *stmt, *tipo;
  TreeNode *param;

  retira = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  retira->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  retira->lineno = 0;
  retira->attr.name = "retiraFIFO";
  retira->child[0] = tipo;
  retira->child[1] = NULL;
  retira->child[2] = stmt;

  st_insert("retiraFIFO", -1, addLocation(), retira, FUNC);
}

static void insertCarregaTabDeProgramas(void){
  TreeNode *carrega, *stmt, *tipo;
  TreeNode *param;

  carrega = newDeclNode(FunctionK);

  tipo = newDeclNode(FunctionK);
  tipo->attr.type = VOID;
  carrega->type = Void;

  stmt = newStmtNode(CompoundK);
  stmt->child[0] = NULL;
  stmt->child[1] = NULL;

  carrega->lineno = 0;
  carrega->attr.name = "carregaTabelaDeProgramas";
  carrega->child[0] = tipo;
  carrega->child[1] = NULL;
  carrega->child[2] = stmt;

  st_insert("carregaTabelaDeProgramas", -1, addLocation(), carrega, FUNC);
}
/* nullProc is a do-nothing procedure to
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ if (t==NULL) return;
  else return;
}



static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
Error = TRUE;
}


static void insertNode( TreeNode * t)
{ switch (t->nodekind)
  {
    case StmtK:
    switch (t->kind.stmt)
    { case CompoundK:
      if (preserveLastScope) {
        preserveLastScope = FALSE;
      } else {
        Scope scope = sc_create(funcName);
        sc_push(scope);
      }
      t->attr.scope = sc_top();
      break;
      default:
      break;
    }
    break;
    case ExpK:
    switch (t->kind.exp)
    { case IdK:
      case ArrIdK:
      if (st_lookup(t->attr.name) == -1){
        typeError(t, " ");
      	printf("ERROR 1: Variable %s is not declared\n\n",t->attr.name);}
       // typeError(t,t->attr.name "ERROR 1: Variable %s is not declared");
      else
          /* Na tabela de símbolos, adiciona o numero da linha somente (t->lineno) */
        st_add_lineno(t->attr.name,t->lineno);
      break;
      case CallK:
      if (st_lookup(t->attr.name) == -1){
      	typeError(t, " ");
      	printf("ERROR 5: Function %s is not declared\n\n",t->attr.name);}
        //typeError(t, "ERROR 5: Function is not declared");
      else
          /* Na tabela de símbolos, adiciona o numero da linha somente (t->lineno) */
        st_add_lineno(t->attr.name,t->lineno);
      break;
      default:
      break;
    }
    break;

    case DeclK:
    switch (t->kind.decl)
    {
     case FunctionK:
     funcName = t->attr.name;
            if(strcmp(funcName,"main")==0) /*verifica se o nome da função é main*/
                verify_main ++;

     if ((st_lookup_top(funcName) >= 0)) {
        typeError(t, " ");
     	printf("ERROR 7.1: The function %s already exists\n\n",t->attr.name);
      //typeError(t,"ERROR 7.1: The functions already exists");
      break;
    }
    st_insert(funcName,t->lineno,addLocation(),t, FUNC);
    sc_push(sc_create(funcName));
    preserveLastScope = TRUE;
    switch (t->child[0]->attr.type)
    { case INT:
      t->type = Integer;
      break;
      case VOID:
      default:
      t->type = Void;
      break;
    }
    break;

   case VarK:
    switch (t->child[0]->attr.type)
    { case INT:
      t->type = Integer;
      break;
      case VOID:
      default:
      t->type = Void;
      break;
    }
    if(t->child[0]->attr.type == VOID){
     typeError(t, " ");
     printf("ERROR 3: The variable %s can't be VOID type\n\n",t->attr.name);
     //typeError(t,"ERROR 3: The variable can't be VOID type");
   }
   else if (st_lookup(t->attr.name) == -1)
     st_insert(t->attr.name,t->lineno,addLocation(),t, VAR);
   else {
   		if(verifica(t->attr.name) ==1){
   		  typeError(t, " ");
   		  printf("ERROR 7.2: The variable %s has function name\n\n",t->attr.name);}
   			//typeError(t,"ERROR 7.2: This variable have a function name");
   		else {typeError(t, " ");
   			printf("ERROR 4: The variable %s already exists\n\n",t->attr.name);}
   		//typeError(t," ERROR 4: This variable already exists");
   }

   break;

   case VetK:
    switch (t->child[0]->attr.type)
    { case INT:
      t->type = Integer;
      break;
      case VOID:
      default:
      t->type = Void;
      break;
    }
   if(t->child[0]->attr.arr.type == VOID){
   	  typeError(t, " ");
   	  printf("ERROR 3: The variable %s can't be VOID type\n\n",t->attr.name);
     //typeError(t,"ERROR 3: The variable can't be VOID type");
   }
   else
    if (st_lookup(t->attr.arr.name) == -1)
      st_insert(t->attr.arr.name,t->lineno,addLocation(),t, VAR);
   else{
   		if(verifica(t->attr.name) ==1){
   			typeError(t, " ");
   			printf("ERROR 7.2: The variable %s has function name\n\n",t->attr.name);}
   			//typeError(t,"ERROR 7.2: This variable have a function name");
   		else{
   		typeError(t, " ");
   		 printf("ERROR 4: The variable %s already exists\n\n",t->attr.name);}}
    break;

  }
  break;
      case ParamK://declaração da variável e parametros
      switch (t->kind.param)
      {
       case ArrParamK:
       case NonArrParamK:
           switch (t->child[0]->attr.type)
            { case INT:
              t->type = Integer;
              break;
              case VOID:
              default:
              t->type = Void;
              break;
            }
       if (st_lookup(t->attr.name) == -1)
        st_insert(t->attr.name,t->lineno,addLocation(),t, VAR);
         else{
   		if(verifica(t->attr.name) ==1){
   			typeError(t, " ");
   			printf("ERROR 7.2: The variable %s has function name\n\n",t->attr.name);}
   		//	typeError(t,"ERROR 7.2: This variable have a function name");
   		else {typeError(t, " ");
   			printf("ERROR 4: The variable %s already exists\n\n",t->attr.name);}}
   		//typeError(t,"ERROR 4: This variable already exists");}
      break;
      default:
      break;
    }
    break;

  }
}

static void afterInsertNode( TreeNode * t )
{ switch (t->nodekind)
  { case StmtK:
    switch (t->kind.stmt)
    { case CompoundK:
      sc_pop();
      break;
      default:
      break;
    }
    break;
    default:
    break;
  }
}

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{ globalScope = sc_create(NULL);
  sc_push(globalScope);
  insertInput();
  insertOutput();
  insertCopy();
  insertLoadReg();
  insertStoreReg();
  insertBIOS();
  insertExecuteProc();
  insertFIFOVazia();
  insertRefazer();
  insertAddictAddr();
  insertPagingStore();
  insertPagingLoad();
  insertMenu();
  insertFIFOCreate();
  insertRenameProg();
  insertExcluirProg();
  insertExecutarProg();
  insertRetiraFIFO();
  insertCarregaTabDeProgramas();

  traverse(syntaxTree,insertNode,afterInsertNode);
  sc_pop();
  if (TraceAnalyze)
    { fprintf(listing,"\nSymbol table:\n\n");
  printSymTab(listing);
}
}

static void beforeCheckNode(TreeNode * t)
{ switch (t->nodekind)
  { case DeclK:
    switch (t->kind.decl)
    { case FunctionK:
      funcName = t->attr.name;
      break;
      default:
      break;
    }
    break;
    case StmtK:
    	if(t->kind.stmt ==CompoundK)
    		sc_push(t->attr.scope);
    default:
    break;
  }
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
char *callingFuncName;
TreeNode * funcDecl;
static void checkNode(TreeNode * t)
{
  switch (t->nodekind)
  {
    case ExpK:
    switch (t->kind.exp)
    {
     case AssignK:
     switch(t->child[1]->kind.exp)
     {
       case CallK:
       sc_push(globalScope);
       if (st_lookup_top_node(t->child[1]->attr.name) == 1){
       		typeError(t, " ");
       		printf("ERROR 2: Invalid assignment, void function %s hasn't return\n\n",t->child[1]->attr.name);
        }
      break;
    }
    break;
  }
}
}


/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree)
{ sc_push(globalScope);
  traverse(syntaxTree,beforeCheckNode,checkNode);
  sc_pop();
  if(verify_main==0)
  	printf("ERROR 6.1: MAIN function is not declared\n");
}
