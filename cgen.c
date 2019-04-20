#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"
#include "analyze.h"


/*declarando os registradores*/
int regDestino;
int init_menu;
int regOutput = 31;//IO reg
int regFonte1 = 29;
int regFonte2 = 30;
int regTemp = 7;
int regAdress = 0;
int regFuncRet = 0;
int regArrDestino = 0;
int regOf = 17;
int pass_idk = 0;
int offset;
int ctrlExp = 0;
int regFuncArgs = 3;
int verify_immediate = 0;
int endControl = 0;
int table_program = 0;
static int future_addr;
int function_out = 0;
int is_store = 0;
int input_call = 0;
int pass_result = 0;
int control_pass = 0;
int no_store = 0;
int recursion = 0;
int is_const = 0;
int ctrlCall = 0;
int soma;
int is_op = 0;
int params;
int mainFunc = 0;
int output_parameter = 0;
int id = 0;
int vet = 0;
int mode2 = 0;
int assign = 0;
int vet_id = 0;
int is_load = 0;
int reg_pass = 0;
int reg_load = 0;
int reg_save =  0;
int aux = 0;
int is_var = 0;
int is_vet = 0;
int mode;
int id_cont = 0;
int value_param = 0;
int return_verify = 0;
int control_exp = 0;
int id_pass = 0;
int cont_stack = 0;
int control_return;
int cont_exp = 0;
int is_if_node = 0;
int aux_func = 0;
int cont_params = 0;
int compare_vet = 0;
int returnToValue = 0;
int calling = 0;
int is_int_function = 0;
int reverse_exp = 0;
int no_pass = 0;
int exp_control = 0; //controla as expressoes de um if
int control_pass_value = 0; //controla a passagem de valor para um registradaor fonte
int stack_jal = 0;
char *nameFunc; //para detectar recursao
int no_result = 0 ;
int pass_to = 0;
int num_func = 0;
int contwhile = 0;
int branch_zero = 0;
int isvar = 0;
int control_returnk;
int save_addr_ret;
int endereco_insere = 0;
int tab_prog = 23;//reg do endereco base (da memoria de dados) da tabela de programas
int  tam = 28;//reg do tamanho da fila
int fifo = 25;//reg do endereço base (da memoria de dados) da fila de processos
int i_id = 26; //reg do indice da tabela de programas
int proc = 27; //reg que armazena o valor do processo atual
int i_FIFO = 7;//reg do indice da fila de processos
int init_retira_FIFO;
int end_pos_retira_FIFO;
int FIFO_size = 22;//reg do tamanho da fila de processos
int kernel_start = 0;

/**************ENDERECO DO INICIO DAS FUNCOES DE OPERACAO********************/
int addr_renomear;
int addr_excluir;
int adrr_executar;
int addr_carregaTabProg;



static void cGen (TreeNode *tree);
static void genDec(TreeNode *tree);


int congelarLineLoc(TreeNode* node){
  int lineLocFuture;
  if(getSkipPrint()){
    setSkipPrint(1);
    int lineLocBackUp = getLineCounterAssembly();
    cGen(node);
    lineLocFuture = getLineCounterAssembly()+1;
    setLineCounterAssembly(lineLocBackUp);
  }
  else{
    setSkipPrint(1);
    int lineLocBackUp = getLineCounterAssembly();
    cGen(node);
    lineLocFuture = getLineCounterAssembly()+1;
    setLineCounterAssembly(lineLocBackUp);
    setSkipPrint(0);
  }
  return lineLocFuture;
}

int congelarLineLocWhile(TreeNode* node){
  int lineLocFuture;
    int lineLocBackUp = getLineCounterAssembly();
    cGen(node);
    lineLocFuture = getLineCounterAssembly();
    setLineCounterAssembly(lineLocBackUp);
    setSkipPrint(0);
  return lineLocFuture;
}

  static void genStmt(TreeNode *tree){
    TreeNode *p1, *p2, *p3;

    switch (tree->kind.stmt) {
      case IntegerK:
        cGen(tree->child[0]);
        break;
      case ReturnK:
        p1 = tree->child[0];
        p2 = tree->child[1];
        return_verify = 1;
        is_int_function = 1;
        pass_result = 0;


        cGen(p1);
      //  if(p1){
        //  if(function_out!=2 && control_return ==0){
          regFuncRet = 1;
          function_out=0;
          control_return = 1;
          if(!recursion){
            DoubleRegTypeFormat(movimentacao, getVarMemLoc(p1->attr.name), regFuncRet, 0);
            if(control_returnk)
              JNHTypeFormat(jump, save_addr_ret);
          }
        break;
      case VoidK:
        break;
      case ElseK:

        break;
      case CompoundK:
        p1 = tree->child[0];
        p2 = tree->child[1];

        if(p1)
          cGen(p1);

        if(p2){
          cGen(p2);
        }

        break;
      case IterK:
      p1 = tree->child[0];
      p2 = tree->child[1];
      p3 = tree->child[2];
      is_if_node = 1;
      mode2 = 0;
      no_result = 0;
      int lineLocationFuture;
      ctrlExp = 0;

      int linePast = getLineCounterAssembly();
      lineLocationFuture = congelarLineLoc(p2);
      cGen(p1);
      no_result = 0;
      lineLocationFuture = congelarLineLocWhile(p2) +3;

      DoubleRegTypeFormat(branch_not_equal, regFonte2, regFonte1, lineLocationFuture);

      pass_result = 0;
      is_if_node = 0;
      ctrlExp = 0;
      cGen(p2);
      JNHTypeFormat(jump, linePast);
      JNHTypeFormat(nop, 0);

        break;

      case IfK:
      p1 = tree->child[0];
      p2 = tree->child[1];
      p3= tree->child[2];
      is_if_node = 1;
      no_result = 0;

      aux = 0;
      ctrlExp = 0;
      int lineLocFuture; /*= CongelarLinhaBranchWhile(p1);*/

      int linePast2 = getLineCounterAssembly();

      cGen(p1);
      exp_control = 0;
      no_result = 0;
      lineLocFuture = congelarLineLocWhile(p2);

      if(branch_zero){
        OneRegTypeFormat(branch_less_zero, regFonte1, lineLocFuture + 2);
        branch_zero = 0;
      }

      else
        DoubleRegTypeFormat(branch_not_equal, regFonte2, regFonte1, lineLocFuture + 2);

      pass_result = 0;
      is_if_node = 0;
      ctrlExp = 0;
      if(p2){
        cGen(p2);
        function_out = 0;
        JNHTypeFormat(jump, congelarLineLocWhile(p3)+2);
        JNHTypeFormat(nop,0);
      }

      if(p3){
        function_out = 3;
        cGen(p3);
        JNHTypeFormat(nop,0);
      }

        break;
      default:

        break;
    }
  }
    static void genDeclaration (TreeNode *tree){
      TreeNode *p1, *p2, *p3;
      switch (tree->kind.decl) {
        case VarK:

          p1 = tree->child[0];
          is_var = 1;
          isvar = 1;
          regTemp++;
          st_find(tree, regTemp);


          cGen(tree->child[0]);
          break;
        case VetK:
          regTemp++;
          break;
        case FunctionK:
          endControl = 0;
          return_verify = 0;
          verify_immediate = 0;
          recursion = 0;
          nameFunc = tree->attr.name;

          is_int_function = 0;
          p2 = tree->child[2];
          p1 = tree->child[1];
          int endFunc;
          regFuncArgs = 3;

          if(strcmp("main", tree->attr.name) == 0){
            regFuncArgs = 4;
            mainFunc = 1;
            JNHTypeFormat(jump, getLineCounterAssembly()+1);
            FunctionMain();
            if(p1){
              endFunc = congelarLineLoc(p1);
              cGen(p1);
            }
            cGen(p2);

            //trocar o halt por jump pra refazFIFO
            JNHTypeFormat(halt, 0);

          }
          if(strcmp("main", tree->attr.name) != 0){
            JNHTypeFormat(nop, 0);
            cGen(p1);
            tree->lineno = getLineCounterAssembly();
            p2 = tree->child[2];
            p1 = tree->child[1];
            cGen(p2);


            OneRegTypeFormat(jump_register, regAdress, 0);

            }

          break;
        default:

          break;
      }
    }
    static void genExpression(TreeNode * tree){
      TreeNode *p1, *p2, *p3;
      switch (tree->kind.exp) {

        case AssignK:
          assign = 1;
          vet = 0;
          no_store = 0;
          is_load = 0;

          p1 = tree->child[0];
          p2 = tree->child[1];
          if(is_const){
            cGen(p1);
            is_const = 0;
          }
          if(p2 && !is_const){
            cGen(p2);

          }

          if(calling && is_int_function){
            DoubleRegTypeFormat(movimentacao, regFuncRet, regDestino, 0);
            calling = 0;
          }



          if((id == 1) || (vet_id ==0)){
            if(pass_result)
              vet = 1;
            else{
              if(!is_load)
                DoubleRegTypeFormat(movimentacao, regDestino,regFonte2,  0);
              assign = 2;
              vet = 1;
              pass_to = 1;
              is_store = 0;
            }

          }


          if(p1 && !is_const){

              cGen(p1);
              is_const = 0;

              if((id ==2 || ((vet_id !=1)||(vet==1))) && !is_store)
                DoubleRegTypeFormat(movimentacao, regFonte2, regDestino, 0);

                      id = 0;
                      assign = 0;
                break;
          }
          if(vet == 1 && is_load ){
              DoubleRegTypeFormat(movimentacao, regFonte2, regDestino, 0);
              is_load = 0;
          }
          verify_immediate=0;
          if(pass_result || input_call){
            if(input_call && !pass_result){
              pass_result = 0;
              is_load = 0;
              input_call = 0;
            }
            if((pass_result==1 && !p1) && is_store){
            DoubleRegTypeFormat(movimentacao, regFonte2, regDestino, 0);
          //  pass_result = 0;
            is_load = 0;
          }

          }
          cGen(p1);

          compare_vet = 1;
          if(vet_id>=2)
            vet_id = 0;

          break;
          is_store = 0;
        case ConstK:
            is_const = 1;
            if(verify_immediate==0){
              if(function_out){
                //is_const = 1;
                OneRegTypeFormat(load_immediate, regFuncArgs++, tree->attr.val );

                //regDestino = set_value(tree, regFuncArgs);
              }

              else{
                if(is_var && !pass_idk)
                    OneRegTypeFormat(load_immediate, regTemp , tree->attr.val);
                else
                  OneRegTypeFormat(load_immediate, regDestino, tree->attr.val);
              }


              }
            if(verify_immediate == 3)
              OneRegTypeFormat(load_immediate, regDestino, tree->attr.val);
            if(verify_immediate!=3 && verify_immediate!=0){
              offset = tree->attr.val;
              verify_immediate = 2;
            }
            cGen(tree->child[0]);
          break;


        case SizeK:
          cGen(tree->child[0]);
          break;
        case ArrIdK:
        params = output_parameter;

        cGen(tree->child[0]);
        cGen(tree->child[1]);

        if(strcmp("tabelaProgramas", tree->attr.name) == 0)
          regArrDestino = 23;

        else if(strcmp("tabelaPaginas", tree->attr.name) == 0)
          regArrDestino = 24;

        else if(strcmp("FIFO", tree->attr.name) == 0)
          regArrDestino = 25;
        else
          regArrDestino = st_find(tree, regTemp);

        if(output_parameter){
          TriRegTypeFormat(load_word, regArrDestino,  regDestino,regFonte2);
          DoubleRegTypeFormat(movimentacao, regFonte2, regOutput,0);
            break;
        }

        if(is_op){
          TriRegTypeFormat(load_word, regArrDestino, regDestino, regFonte1);
          is_op = 0;
          no_store = 1;
          reg_load = 1;
          is_load = 1;
          is_vet = 1;
          break;
        }
          if(vet ==0 && input_call ==0 && (function_out == 0) && !no_store){
            if(pass_result)
              regFonte2 = regFonte1;
            TriRegTypeFormat(load_word, regArrDestino,  regDestino,regFonte2);

            vet = 1;
            compare_vet = 0;
            vet_id ++;
            is_load = 1;
            regFonte2 = 30;
            is_vet = 1;
            if(params){
              DoubleRegTypeFormat(movimentacao, regFonte2, regOutput,0);

              input_call = 0;

            }
            break;
          }

          if(vet ==1 || input_call ==1){
            TriRegTypeFormat(store_word, regArrDestino,  regDestino, regFonte2);
            compare_vet = 1;
            vet_id++;
            vet = 0;
            is_load = 0;
            is_store = 1;
            no_store = 1;
            input_call = 0;
            pass_result = 0;
            break;

          }
          if(compare_vet ==0){
            TriRegTypeFormat(load_word, regArrDestino, regDestino, regFonte2);

            is_load = 1;
            is_vet = 1;
          }
          compare_vet = 1;


          if(output_parameter ){
            DoubleRegTypeFormat(movimentacao, regFonte1, regOutput, 0);

          }

          break;

/*****  NÓ DAS CHAMADAS DE FUNÇÕES - AQUI SÃO REALIZADAS A DEFINIÇÃO DAS
                                  MACROFUNÇÒES          ****/
        case CallK:
          verify_immediate = 0;
          regFuncArgs = 4;
          params = output_parameter;
          int saveLine;
          int i = 0, j=0;

          pass_result = 0;
          no_result = 0;
          if(strcmp(nameFunc, tree->attr.name) == 0)
            recursion = 1;

          /***** MACROFUNCAO: PARA ENTRADA DE DADOS*****/
          if(strcmp("input", tree->attr.name) == 0){
            OneRegTypeFormat(entrada, regOutput, 0);
            input_call = 1;

          }
          /***** MACROFUNCAO: PARA SAIDA DE DADOS*****/
          else if(strcmp("output", tree->attr.name) == 0){
              output_parameter = 1;
              //function_out = 1;
              cGen(tree->child[0]);
              OneRegTypeFormat(saida, regOutput, 0);
              output_parameter = 0;
              is_load = 0;
              pass_result = 0;
          }
          /***** MACROFUNCAO: UTILIZADO APENAS PARA TESTE *****/
          else if(strcmp("copyCS", tree->attr.name) == 0){

            /************* COM PREEMPCAO *********************/
            JNHTypeFormat(loi6bits, 17); //insira a quantidade de programas
            JNHTypeFormat(emitCMD, 0);
            OneRegTypeFormat(load_immediate, 20, 0);
            OneRegTypeFormat(entrada, regOutput,0);
            DoubleRegTypeFormat(movimentacao, regOutput, FIFO_size,0); //tam
            OneRegTypeFormat(load_immediate, i_FIFO, 0);

            int init1 = getLineCounterAssembly();
            TriRegTypeFormat(set_less_than, i_FIFO, FIFO_size, 29);
            OneRegTypeFormat(load_immediate, 30, 1);
            DoubleRegTypeFormat(branch_not_equal, 29, 30, init1 + 18);
            JNHTypeFormat(loi6bits, 18); //insira o nome do programa
            JNHTypeFormat(emitCMD, 0);
            OneRegTypeFormat(entrada, regOutput,0);
            //verifica se o programa existe na tabela
            OneRegTypeFormat(load_immediate, i_id, 1);
            int addr_x = getLineCounterAssembly();
            TriRegTypeFormat(set_less_than, i_id, tam, 30);
            OneRegTypeFormat(load_immediate, 29, 1);
            DoubleRegTypeFormat(branch_not_equal, 29, 30, init1);
            TriRegTypeFormat(load_word, tab_prog, i_id, 30);
            int p = getLineCounterAssembly();
            DoubleRegTypeFormat(branch_not_equal, 30, regOutput, p + 3);
            TriRegTypeFormat(store_word, fifo, i_FIFO,regOutput);
            JNHTypeFormat(jump, p+5);
            DoubleRegTypeFormat(adicao_imediato, i_id, i_id, 1);
            JNHTypeFormat(jump, addr_x);

            DoubleRegTypeFormat(adicao_imediato, i_FIFO,i_FIFO,1);
            JNHTypeFormat(jump, init1);
            JNHTypeFormat(nop,0);




            JNHTypeFormat(loi6bits, 23); //EXECUTANDO PROGRAMA ...
            JNHTypeFormat(emitCMD, 0);

            OneRegTypeFormat(load_immediate, i_FIFO, 0); //i = 0
            init1 = getLineCounterAssembly();
            TriRegTypeFormat(set_less_than, i_FIFO, FIFO_size, 30); //i<tam, se sim $30 = 1
            OneRegTypeFormat(load_immediate, 29, 1); //$29 = 1
            ///////////////////WHILE
            DoubleRegTypeFormat(branch_not_equal, 29, 30, init1 + 286 /*ARUMMAR!!!!!*/);
            TriRegTypeFormat(load_word, fifo, i_FIFO, 30);
            DoubleRegTypeFormat(movimentacao, 30, proc, 0);

            DoubleRegTypeFormat(troca_contexto, proc, 21, 0);
            OneRegTypeFormat(saida, 21, 0);


            for(i = 0; i<115; i++){
              COPYtypeFormat(copy, 21, 1000 + i);
              DoubleRegTypeFormat(adicao_imediato, 21, 21, 1);
            }
            OneRegTypeFormat(loadReg, i_FIFO, 0);
            OneRegTypeFormat(loadReg, i_FIFO, 1);
            OneRegTypeFormat(loadReg, i_FIFO, 2);
            OneRegTypeFormat(loadReg, i_FIFO, 3);
            OneRegTypeFormat(loadReg, i_FIFO, 4);
            OneRegTypeFormat(loadReg, i_FIFO, 5);
            OneRegTypeFormat(loadReg, i_FIFO, 6);
            OneRegTypeFormat(loadReg, i_FIFO, 8);
            OneRegTypeFormat(loadReg, i_FIFO, 9);
            OneRegTypeFormat(loadReg, i_FIFO, 10);
            OneRegTypeFormat(loadReg, i_FIFO, 11);
            OneRegTypeFormat(loadReg, i_FIFO, 12);
            OneRegTypeFormat(loadReg, i_FIFO, 13);
            OneRegTypeFormat(loadReg, i_FIFO, 14);
            OneRegTypeFormat(loadReg, i_FIFO, 15);
            OneRegTypeFormat(loadReg, i_FIFO, 17);
            OneRegTypeFormat(loadReg, i_FIFO, 18);
            OneRegTypeFormat(loadReg, i_FIFO, 19);
            OneRegTypeFormat(loadReg, i_FIFO, 29);
            OneRegTypeFormat(loadReg, i_FIFO, 30);
            OneRegTypeFormat(loadReg, i_FIFO, 31);

            int ver = getLineCounterAssembly();
            OneRegTypeFormat(load_immediate, 30, 0);
            DoubleRegTypeFormat(branch_not_equal, 30, 16, ver + 4);
            OneRegTypeFormat(load_immediate, 8, 0);
            DoubleRegTypeFormat(adicao_imediato, 8, 8, 999);
            //EXECUTA
            JNHTypeFormat(countPC, 0);
            OneRegTypeFormat(jump_register, 8, 0);
            //QUANDO TROCAR DE PROCESSO PULAR PRA ESSA LINHA

            OneRegTypeFormat(storeReg, i_FIFO, 0);
            OneRegTypeFormat(storeReg, i_FIFO, 1);
            OneRegTypeFormat(storeReg, i_FIFO, 2);
            OneRegTypeFormat(storeReg, i_FIFO, 3);
            OneRegTypeFormat(storeReg, i_FIFO, 4);
            OneRegTypeFormat(storeReg, i_FIFO, 5);
            OneRegTypeFormat(storeReg, i_FIFO, 6);
            OneRegTypeFormat(storeReg, i_FIFO, 8);
            OneRegTypeFormat(storeReg, i_FIFO, 9);
            OneRegTypeFormat(storeReg, i_FIFO, 10);
            OneRegTypeFormat(storeReg, i_FIFO, 11);
            OneRegTypeFormat(storeReg, i_FIFO, 12);
            OneRegTypeFormat(storeReg, i_FIFO, 13);
            OneRegTypeFormat(storeReg, i_FIFO, 14);
            OneRegTypeFormat(storeReg, i_FIFO, 15);
            OneRegTypeFormat(storeReg, i_FIFO, 17);
            OneRegTypeFormat(storeReg, i_FIFO, 18);
            OneRegTypeFormat(storeReg, i_FIFO, 19);
            OneRegTypeFormat(storeReg, i_FIFO, 29);
            OneRegTypeFormat(storeReg, i_FIFO, 30);
            OneRegTypeFormat(storeReg, i_FIFO, 31);

            DoubleRegTypeFormat(adicao_imediato, i_FIFO,i_FIFO, 1); //i++
            int a = getLineCounterAssembly();
            DoubleRegTypeFormat(branch_not_equal, i_FIFO, FIFO_size, a+3);
            OneRegTypeFormat(load_immediate, i_FIFO, 0);
            OneRegTypeFormat(load_immediate, 16, 10);
            JNHTypeFormat(jump, init1);
            JNHTypeFormat(halt, 0);
          }
          else if(strcmp("LoadRegister", tree->attr.name) == 0){
              OneRegTypeFormat(load_immediate, 20,3);
              JNHTypeFormat(renameProg, 300);
              OneRegTypeFormat(load_immediate, 27,3);
              COPYtypeFormat(copy, 0,0);
          }
            /***** MACROFUNCAO: EXECUÇÃO DA BIOS
            (*)INICIALIZACAO DO SISTEMA OPERACIONAL
            (*)TESTES DE PARTIDA
            (*)CARREGAMENTO DO BLOCO KERNEL DO HD PARA A MEMORIA DE INSTRUCOES
            *****/
          else if(strcmp("LoadBIOS", tree->attr.name) == 0){
            int addr, init, i;

            /*BIOS*/
            init = getLineCounterAssembly();
            JNHTypeFormat(loi6bits, 0); //inicialize com as chaves
            JNHTypeFormat(emitCMD, 0);
            OneRegTypeFormat(entrada, 31, 0);
            OneRegTypeFormat(load_immediate, 30, 10);
            addr = getLineCounterAssembly();
            DoubleRegTypeFormat(branch_not_equal, 30, 31, addr + 641);
            OneRegTypeFormat(load_immediate, 21, 0);
            for(i =0; i<=457; i++ ){
              COPYtypeFormat(copy, 21, i);
              DoubleRegTypeFormat(adicao_imediato, 21, 21, 1);
            }
            JNHTypeFormat(jump, 0);

            JNHTypeFormat(loi6bits, 25); //entrada incorreta
            JNHTypeFormat(emitCMD, 0);
            JNHTypeFormat(jump, init);

          }


          /***** MACROFUNCAO: GRAVAÇÃO  DOS DADOS (pos 0 a 49) MEMORIA DE DADOS
          PARA O HD, POSSUI A MESMA SINTAXE DA INSTRUCAO STORE REGISTER, PORÉM TODOS
          OS REGISTRADORES DE 0 A 49 SAO SALVOS NO HD
          *****/
          else if(strcmp("StoringPage", tree->attr.name) == 0){
                for(i = 0; i<50; i++){
                  OneRegTypeFormat(store_pag, i_FIFO, i);
                }
          }

          /***** MACROFUNCAO: CARREGAMENTO DOS DADOS (pos 0 a 49) MEMORIA DE DADOS
          PARA O HD, POSSUI A MESMA SINTAXE DA INSTRUCAO LOAD REGISTER, PORÉM TODOS
          OS REGISTRADORES DE 0 A 49 SAO CARREGADOS DO HD
          *****/
          else if(strcmp("LoadingPage", tree->attr.name) == 0){
                for(i = 0; i<50; i++){
                  OneRegTypeFormat(load_pag, i_FIFO, i);
                }
          }

          /***** MACROFUNCAO: CRIA O MENU LCD COM AS OPCOES
          PARA CADA SELECAO, É REALIZADO UM BRANCH PARA O INICIO
          DE SUA RESPECTIVA MACROFUNCAO
          *****/
          else if(strcmp("createMenu", tree->attr.name) == 0){
          //  addr_menu = getLineCounterAssembly() - 1;
            int end;
            init_menu = getLineCounterAssembly();
            JNHTypeFormat(loi6bits, 11);
            JNHTypeFormat(emitCMD, 0);
            OneRegTypeFormat(entrada, regOutput, 0);
            OneRegTypeFormat(load_immediate, regFonte2, 1);
            end = getLineCounterAssembly();
          //  DoubleRegTypeFormat(branch_not_equal, regFonte2, 31, end+5);
            JNHTypeFormat(loi6bits, 13);
            JNHTypeFormat(emitCMD, 0);
          //  JNHTypeFormat(jump , init_menu/*inicio da funcao*/);
            JNHTypeFormat(jump, init_menu/*inicio da main*/);


            OneRegTypeFormat(load_immediate, regFonte2, 2);
            end = getLineCounterAssembly();
            DoubleRegTypeFormat(branch_not_equal, regFonte2, 31, end+5/*jump p*/);
            JNHTypeFormat(loi6bits, 14);
            JNHTypeFormat(emitCMD, 0);
            JNHTypeFormat(jump, 107 /*inicio da funcao*/);
            JNHTypeFormat(jump, init_menu/*inicio da main*/);

            OneRegTypeFormat(load_immediate, regFonte2, 3);
            end = getLineCounterAssembly();
            DoubleRegTypeFormat(branch_not_equal, regFonte2, 31, end+5/*jump p*/);
            JNHTypeFormat(loi6bits, 15);
            JNHTypeFormat(emitCMD, 0);
            JNHTypeFormat(jump, 54/*inicio da funcao*/);
            JNHTypeFormat(jump, init_menu/*inicio da main*/);

            OneRegTypeFormat(load_immediate, regFonte2, 4);
            end = getLineCounterAssembly();
            DoubleRegTypeFormat(branch_not_equal, regFonte2, 31, init_menu/*jump p*/);
            JNHTypeFormat(loi6bits, 16);
            JNHTypeFormat(emitCMD, 0);
            JNHTypeFormat(jump, 88 /*inicio da funcao*/);
            JNHTypeFormat(jump, init_menu/*inicio da main*/);
          }

          /***** MACROFUNCAO: RENOMEAR UM PROGRAMA
          *****/
          else if(strcmp("renomearPrograma", tree->attr.name) == 0){
            addr_renomear = getLineCounterAssembly() - 1;
            int init1, init2, init3, init4;
            JNHTypeFormat(loi6bits, 19); //INSIRA O PROGRAMA
            JNHTypeFormat(emitCMD, 0);
            /*
            INSTRUCOES DE TESTE

            OneRegTypeFormat(load_immediate, 30, 10);
            OneRegTypeFormat(load_immediate, 26, 2);
            TriRegTypeFormat(store_word, 23, 26, 30);

            */

            OneRegTypeFormat(entrada, regOutput,0);
            OneRegTypeFormat(load_immediate, i_id, 1);

            init1 = getLineCounterAssembly();
            TriRegTypeFormat(set_less_than, i_id, tam, 29);
            OneRegTypeFormat(load_immediate, 30, 1);
            DoubleRegTypeFormat(branch_not_equal, 29, 30, init1 + 28);
            TriRegTypeFormat(load_word, tab_prog, i_id, 30);

            init2 = getLineCounterAssembly();
            DoubleRegTypeFormat(branch_not_equal, regOutput, 30, init2 + 22);//$20 - armazena o valor de entrada
            DoubleRegTypeFormat(troca_contexto, 30, 21, 0);
            DoubleRegTypeFormat(movimentacao, 30, 27, 0);
            JNHTypeFormat(loi6bits, 20); //INSIRA O NOVO NOME
            JNHTypeFormat(emitCMD, 0);


            OneRegTypeFormat(entrada, 20,0);

            //VEJO SE O NOME JA EXISTE

            OneRegTypeFormat(load_immediate, 3, 1);
            int init7, init8;

            init7 = getLineCounterAssembly();
            TriRegTypeFormat(set_less_than, 3, tam, 29);
            OneRegTypeFormat(load_immediate, 30, 1);
            DoubleRegTypeFormat(branch_not_equal, 29, 30, init7 + 10);
            TriRegTypeFormat(load_word, tab_prog, 3, 30);

            init8 = getLineCounterAssembly();
            DoubleRegTypeFormat(branch_not_equal, 20, 30, init8 + 4);//$20 - armazena o valor de entrada

            JNHTypeFormat(loi6bits, 26); //ARQUIVO JA EXISTENTE
            JNHTypeFormat(emitCMD, 0);
            JNHTypeFormat(jump, init7 - 2);
            DoubleRegTypeFormat(adicao_imediato, 3, 3, 1);
            JNHTypeFormat(jump, init7);

            DoubleRegTypeFormat(renameProgReg, 20, 21, 0);


            TriRegTypeFormat(store_word, tab_prog, i_id, 20);

            JNHTypeFormat(loi6bits, 20); //PROGRAMA RENOMEADO
            JNHTypeFormat(emitCMD, 0);

            JNHTypeFormat(jump, init2+23);
            DoubleRegTypeFormat(adicao_imediato, 26, 26, 1);
            JNHTypeFormat(jump, init1);
            JNHTypeFormat(jump, init_menu);
            JNHTypeFormat(nop, 0);

          }

          /***** MACROFUNCAO: DELETAR UM PROGRAMA
          *****/
          else if(strcmp("excluirPrograma", tree->attr.name) == 0){
            addr_renomear = getLineCounterAssembly() - 1;
            int init1, init2;
            JNHTypeFormat(loi6bits, 21); //INSIRA O PROGRAMA PARA EXCLUIR
            JNHTypeFormat(emitCMD, 0);

            OneRegTypeFormat(entrada, regOutput,0);
            DoubleRegTypeFormat(movimentacao, 31, 20,0); //prog = 20, i=3*/
            OneRegTypeFormat(load_immediate, i_id, 1);


            init1 = getLineCounterAssembly();
            TriRegTypeFormat(set_less_than, i_id, tam, 29);
            OneRegTypeFormat(load_immediate, 30, 1);
            DoubleRegTypeFormat(branch_not_equal, 29, 30, init1 + 12);
            TriRegTypeFormat(load_word, tab_prog, i_id, 30);

            init2 = getLineCounterAssembly();
            DoubleRegTypeFormat(branch_not_equal, regOutput, 30, init2 + 6);
            JNHTypeFormat(loi6bits, 22); //PROGRAMA EXCLUIDO
            JNHTypeFormat(emitCMD, 0);
            OneRegTypeFormat(load_immediate, 30,0);
            TriRegTypeFormat(store_word, tab_prog, i_id, 30);
            JNHTypeFormat(jump, init2+8);


            DoubleRegTypeFormat(adicao_imediato, i_id, i_id, 1);
            JNHTypeFormat(jump, init1);
            JNHTypeFormat(nop, 0);
            JNHTypeFormat(jump, init_menu);
          }


          /***** MACROFUNCAO: EXECUÇÃO DE UM PROGRAMA COM PREEMPCAO
                                  OU SEM PREEMPCAO
          *****/
          else if(strcmp("executarPrograma", tree->attr.name) == 0){
            int init1, init2, init3, init4 , init5, init6;
            init4 =
             getLineCounterAssembly();
            JNHTypeFormat(loi6bits, 27); //executar preemptivo ou nao preemptivo
            JNHTypeFormat(emitCMD, 0);
            OneRegTypeFormat(entrada, regOutput, 0);
            OneRegTypeFormat(load_immediate, 30, 1);
            OneRegTypeFormat(load_immediate, 29, 2);
            init6 = getLineCounterAssembly();

            DoubleRegTypeFormat(branch_not_equal,regOutput, 30, init_menu);
            JNHTypeFormat(loi6bits, 28); //insira o programa que deseja executar
            JNHTypeFormat(emitCMD, 0);
            OneRegTypeFormat(entrada, regOutput, 0);
            OneRegTypeFormat(load_immediate, i_id, 1);
            int vet = getLineCounterAssembly();
            TriRegTypeFormat(set_less_than, i_id, tam, 30);
            OneRegTypeFormat(load_immediate, 29, 1);
            DoubleRegTypeFormat(branch_not_equal, 29, 30, init_menu);
            TriRegTypeFormat(load_word, tab_prog, i_id, 27);
            init5 = getLineCounterAssembly();
            DoubleRegTypeFormat(branch_not_equal, regOutput, 27, init5+147);
            DoubleRegTypeFormat(troca_contexto, proc, 21, 0);

            int k;
            for(k = 0; k<=70; k++){
              COPYtypeFormat(copy, 21, 400 + k);
              DoubleRegTypeFormat(adicao_imediato, 21, 21, 1);
            }
            OneRegTypeFormat(load_immediate, 21, 300);
            COPYtypeFormat(copy, 21, 471);
            JNHTypeFormat(jump, 400);

            DoubleRegTypeFormat(adicao_imediato, i_id, i_id,1);
            JNHTypeFormat(jump, vet);
          }


          /***** MACROFUNCAO: RETIRA UM PROCESSO DA FILA
          (*) QUANDO UM PROGRAMA FINALIZA É REALIZADO UM JUMP (NO LUGAR
              DO HALT) PARA ESTA LINHA, A DEFINICAO DA LINHA ONDE É REALIZADO
              O DESVIO É FEITO MANUALMENTE, É GERADO O CÓDIGO BINÁRIO ENTÃO É VISTO QUAL
              A LINHA ESTA FUNCAO INICIA
          *****/
          else if(strcmp ("retiraFIFO", tree->attr.name) == 0){
            int init;
            init_retira_FIFO = getLineCounterAssembly();
            JNHTypeFormat(nop, 0);
            OneRegTypeFormat(load_immediate, 24, 0);
            DoubleRegTypeFormat(adicao_imediato, 24, 24, 1);
            JNHTypeFormat(jump, soma);
            JNHTypeFormat(jump, init_retira_FIFO);
          }

          /***** MACROFUNCAO: CARREGA TABELA DE PROGRAMAS
          *****/
          else if(strcmp("carregaTabelaDeProgramas", tree->attr.name) ==0){
            int i;
            addr_carregaTabProg = getLineCounterAssembly();
            OneRegTypeFormat(load_immediate, tam, 5);
            /*tam = tamanho da tabela de programas*/
            DoubleRegTypeFormat(adicao_imediato, tam, tam, 1);
            /*i_id = indice da tabela*/
            OneRegTypeFormat(load_immediate, i_id, 1);
            /*$16 = verifica quantas vezes percorreu a fila*/
            OneRegTypeFormat(load_immediate, 16, 0);
            /*$8 registrador que armazena o valor do PC que atingiu o quantum*/
            OneRegTypeFormat(load_immediate, 8, 0);
            /*Endereço base da fila de processos*/
            OneRegTypeFormat(load_immediate, fifo, 50);
            /*Endereço base da tabela de programas*/
            OneRegTypeFormat(load_immediate, tab_prog, 70);
            OneRegTypeFormat(load_immediate, 24, 0);

            for(i = 1; i<=5; i++){
              OneRegTypeFormat(load_immediate, 27, i);
              TriRegTypeFormat(store_word, tab_prog, i_id, 27);

              //PROGRAMA 1 COMEÇA NA LINHA 800 DO HD
              if(i == 1){
                OneRegTypeFormat(load_immediate, 30, 0);
                DoubleRegTypeFormat(renameProgReg, 27, 30, 0);
                DoubleRegTypeFormat(adicao_imediato, i_id, i_id, 1);
              }

              //PROGRAMA 2 COMEÇA NA LINHA 860 DO HD
              else if(i == 2)
              {
                OneRegTypeFormat(load_immediate, 30, 70);
                DoubleRegTypeFormat(renameProgReg, 27, 30, 0);
                DoubleRegTypeFormat(adicao_imediato, i_id, i_id, 1);
              }

              //PROGRAMA 3 COMEÇA NA LINHA 920 DO HD
              else if(i == 3){
                OneRegTypeFormat(load_immediate, 30, 140);
                DoubleRegTypeFormat(renameProgReg, 27, 30, 0);
                DoubleRegTypeFormat(adicao_imediato, i_id, i_id, 1);
              }
            }
          }
          else{
              calling = 1;
              num_func++;
              if(st_lookup_top_node (tree->attr.name) == -1)
                is_int_function = 1;
              if(return_verify){
                function_out = 2;
              }
              TreeNode *func;
              func  = AddrFind(tree->attr.name);
              int start_func = func->lineno;
              params = 2;
              ctrlCall = 1;
              function_out = 1;
              verify_immediate = 0;
              cont_params = 0;
              cGen(tree->child[0]);
              //cont_params = 0;
              if(ctrlCall==2)
                DoubleRegTypeFormat(movimentacao, regFonte2, regFuncArgs++, 0);

              int lineLocFuture = congelarLineLoc(tree->child[0]);
              cGen(func->child[1]->child[0]);
              cGen(func->child[0]->child[1]);
              function_out = 0;

              if(recursion == 1){
                JNHTypeFormat(jump, start_func);
              }
              else {
                if(mainFunc)
                  JNHTypeFormat(jump_and_link, start_func);
                else{

                  JNHTypeFormat(jump_and_link, start_func);
                  JNHTypeFormat(nop, 0);

                }
            }

              endControl = 1;
              function_out = 0;
              ctrlCall = 0;
              verify_immediate = 0;
              if(params && mainFunc){
                DoubleRegTypeFormat(movimentacao, regFuncRet, regOutput,0);
                params = 0;
                input_call = 0;

              //  mainFunc = 0;
              }
          }

          id = 0;
          assign = 0;
          break;

        case IdK:
        //  JNHTypeFormat(tree->attr.name, ctrlExp);
          if(strcmp("id", tree->attr.name) == 0)
            regDestino = 26;

          if(strcmp("tam", tree->attr.name) == 0)
              regDestino = 28;
          else
            regDestino = st_find(tree, regTemp);

          printf("%s, %d\n", tree->attr.name, regDestino);
          cont_exp++;
          id_pass ++;
          id_cont = 1;
          mode2 = 0;
          pass_idk = 1;
          isvar = 0;


          if(pass_to == 1)
            pass_to = 2;
          if(assign == 1){
            id = 1;
          }
          if(assign == 2)
            id = 2;


          if(mode ==1 && !is_var){
            aux_func = getVarMemLoc(tree->attr.name);
            regDestino = set_value(tree->attr.name, regFuncArgs + 10);
            DoubleRegTypeFormat(movimentacao, regFonte2,regDestino, 0);
            mode = 0;
            regDestino = aux_func;
          }
          p1 = tree->child[0];

          if(pass_result==1 && !is_if_node && assign){

          DoubleRegTypeFormat(movimentacao, regFonte2, regDestino, 0);
          pass_result = 0;
          is_load = 0;
          assign = 0;
        }

          if(output_parameter){
            DoubleRegTypeFormat(movimentacao, regDestino, regOutput, 0);
          }

          if(pass_result ==2){
            DoubleRegTypeFormat(movimentacao, regOutput, regDestino, 0);
            pass_result = 0;
            input_call = 0;
          }
          if(function_out==1){
            if(((getVarMemLoc(tree->attr.name) <=7) && (getVarMemLoc(tree->attr.name)>3)) && (getVarMemLoc(tree->attr.name) != regFuncArgs)){
              DoubleRegTypeFormat(movimentacao, regFuncArgs, regFonte2, 0);
              mode = 1;
            }
            returnToValue = regFuncArgs;

            DoubleRegTypeFormat(movimentacao, regDestino, regFuncArgs++, 0);
            if(regDestino!=regFuncArgs-1 && !cont_params && !is_int_function){

              regDestino = set_value(tree->attr.name, returnToValue);
              cont_params = 1;
            }
          }
          if(input_call == 1){
            DoubleRegTypeFormat(movimentacao, regOutput, regFonte2, 0);
            pass_result = 2;

          }


          if(cont_exp ==2 && !calling){
            verify_immediate = 3;
            control_exp = 0;
            cont_exp = 0;
          }


          break;

        case OpK:

          function_out = 0;
          int pass = 0;
          is_load = 0;

          no_pass = 1;
          is_op = 1;
          p1 = tree->child[0];
          p2 = tree->child[1];
          if(ctrlCall ==1)
            ctrlCall = 2;

          cGen(p1);

                DoubleRegTypeFormat(movimentacao, regDestino, regOf++ ,0);
                pass = 1;
                if(is_load){

                  DoubleRegTypeFormat(movimentacao, regDestino, regFonte2 ,0);
                }


          verify_immediate = 1;
          cGen(p2);


          if(is_load){

            DoubleRegTypeFormat(movimentacao, regDestino, regFonte2 ,0);
          }

            if(!is_load){
              if(!no_result)
                DoubleRegTypeFormat(movimentacao, regDestino, regFonte1 ,0);
              else
                DoubleRegTypeFormat(movimentacao, --regOf, regFonte1 ,0);
            }
              pass = 0;
            if(no_result!=1 && !is_load){
              if(id_cont)
                DoubleRegTypeFormat(movimentacao, --regOf, regFonte2,  0);
              else{
                DoubleRegTypeFormat(movimentacao, --regOf, regFonte1,  0);
                is_load = 1;
              }}
          //  }


          pass_result = 1;//para gravar o resultado na variavel
          id_cont = 0;
          //is_load= 0;
          no_result = 0;
          switch (tree->attr.op) {
            case PLUS:
              if(verify_immediate==2)
                DoubleRegTypeFormat(adicao_imediato, regFonte1, regFonte2, offset);
              else{
                TriRegTypeFormat(adicao, regFonte1, regFonte2, regFonte2);
                no_result = 1;
              }
              verify_immediate = 0;
              ctrlExp++;
              is_op = 0;
              no_pass = 0;

             break;
            case MINUS:

              if(verify_immediate==2){
                DoubleRegTypeFormat(subtracao_imediato, regFonte1, regFonte2, offset);

              }
              else{
                TriRegTypeFormat(subtracao, regFonte1, regFonte2, regFonte2);
                no_result = 1;
              }
              verify_immediate = 0;
              ctrlExp++;
              is_op = 0;
              no_pass = 0;

              break;
            case TIMES:

              if(verify_immediate==2)
                DoubleRegTypeFormat(multiplicao_imediato, regFonte1, regFonte2, offset);
              else{
                TriRegTypeFormat(multiplicao, regFonte1, regFonte2, regFonte2);
                no_result = 1;
              }
              ctrlExp++;
              verify_immediate = 0;
              is_op = 0;
              no_pass = 0;

              break;
            case OVER:

            if(verify_immediate==2){
                OneRegTypeFormat(load_immediate, regFonte2, offset);
                TriRegTypeFormat(divisao, regFonte1, regFonte2, regFonte2);
            }
              else{
                TriRegTypeFormat(divisao, regFonte2, regFonte1, regFonte2);
                no_result = 1;
              }

              ctrlExp++;
              verify_immediate = 0;
              is_op = 0;
              no_pass = 0;

              break;
            case MENOREQ:
              branch_zero = 1;
              if(verify_immediate == 2)
                OneRegTypeFormat(load_immediate, regFonte2, offset);

              TriRegTypeFormat(subtracao, regFonte1, regFonte2, regFonte1);

              is_op = 0;
              no_pass = 0;
              no_result = 0;
              break;
            case LT:
              if(verify_immediate == 2){
                OneRegTypeFormat(load_immediate, regFonte2, offset);
                TriRegTypeFormat(set_less_than, regFonte1, regFonte2, regFonte1);
              }
              else{
                if(is_load == 1){
                  TriRegTypeFormat(set_less_than, regFonte1, regFonte2, regFonte1);
                  is_vet = 0;
                }
                else
                  TriRegTypeFormat(set_less_than, regFonte2, regFonte1, regFonte1);

              }
              OneRegTypeFormat(load_immediate, regFonte2, 1);
              is_op = 0;
              pass_result = 0;
              no_pass = 0;no_result = 0;
              no_result = 0;
              regOf = 17;
              break;
            case RT:
              if(verify_immediate == 2)
                OneRegTypeFormat(load_immediate, regFonte2, offset);

              if(reverse_exp){
                TriRegTypeFormat(set_greater_than, regFonte2, regFonte1, regFonte1);
                reverse_exp = 0;
              }
              else
                TriRegTypeFormat(set_greater_than, regFonte1, regFonte2, regFonte1);
              OneRegTypeFormat(load_immediate, regFonte2, 1);
              verify_immediate = 0;
              is_op = 0;
              pass_result = 0;
              no_result = 0;
              no_pass = 0;
              break;
            case MAIOREQ:
              branch_zero = 1;
              if(verify_immediate == 2)
                OneRegTypeFormat(load_immediate, regFonte2, offset);

              TriRegTypeFormat(subtracao, regFonte2, regFonte1, regFonte2);

              verify_immediate = 0;
              is_op = 0;
              no_result = 0;
              pass_result = 0;
              no_pass = 0;
              break;
            case EQ:
              if(verify_immediate == 2)
                OneRegTypeFormat(load_immediate, regFonte1, offset);
                is_op = 0;
                verify_immediate = 0;
                pass_result = 0;
                no_pass = 0 ;

                no_result = 0;

              break;

            default:

              break;
            verify_immediate = 0;

          }
          default:

            break;
          compare_vet = 0;
          verify_immediate = 0;
          no_result = 0;
      }
    }
    static void genParam(TreeNode *tree){
      switch (tree-> kind.param) {
        case ArrParamK:
        regFuncArgs++;
        st_find(tree, regFuncArgs);
        printf("%s : %d\n",tree->attr.name, st_find(tree, regFuncArgs));
        cGen(tree->child[1]);
        params = 0;

          break;
        case NonArrParamK:
          regFuncArgs++;
          st_find(tree, regFuncArgs);
          printf("%s : %d\n",tree->attr.name, st_find(tree, regFuncArgs));
          cGen(tree->child[1]);
          params = 0;

          break;
      }
    }
    static void genType(TreeNode *tree){
      switch (tree->kind.type) {
        case TypeNameK:
          cGen(tree->child[0]);

          break;
        default:
          break;
      }
    }
static void cGen(TreeNode* tree) {

  if(tree != NULL){
    switch (tree->nodekind) {
      case StmtK:
        genStmt(tree);
        break;
      case ExpK:
        genExpression(tree);
        break;
      case DeclK:
        genDeclaration(tree);
        break;
      case ParamK:
        genParam(tree);
        break;
      case TypeK:
        genType(tree);
        break;

      default:

        break;
    }
    cGen(tree->sibling);
  }
}
void codeGen(TreeNode *syntaxTree){
  cGen(syntaxTree);
  printLines();
}
