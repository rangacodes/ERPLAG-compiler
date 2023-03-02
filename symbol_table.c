#include "ff.h"
#include "lexer.h"
#include <string.h>

entry * newEntry(){
    entry * e =  malloc(sizeof(entry));
    return e;
}

void printSymTableTree(sTable * table){
    entry * e = table->head;
    while(e!=NULL){
        printf("%s\t%s\t%d-%d\t%d\t", e->lexeme, table->scope, table->scopeStart, table->scopeEnd, e->width);
        //printf("%s with type %s, Output? %d. Offset = %d. Temp address: t%d", e->lexeme, terminalNames[e->type], e->isOutput, e->offset, e->temp->temp);
        if(e->isOutput==0){
            if(e->isArr == 1){
                printf("yes\t");
                if(e->indices.isDynamic==0){
                    printf("Static array\t[%d, %d]\t%s\t%d\t%d\n", e->indices.start.N, e->indices.end.N, terminals[e->type], e->offset, table->level);
                }
                else{
                    if(e->indices.isStartID==1&&e->indices.isEndID==1){
                        printf("Dynamic array\t[%s, %s]\t%s\t%d\t%d\n", e->indices.start.id, e->indices.end.id, terminals[e->type], e->offset, table->level);
                    }
                    else if(e->indices.isStartID==1&&e->indices.isEndID==0){
                        printf("Dynamic array\t[%s, %d]\t%s\t%d\t%d\n", e->indices.start.id, e->indices.end.N, terminals[e->type], e->offset, table->level);
                    }
                    else if(e->indices.isStartID==0&&e->indices.isEndID==1){
                        printf("Dynamic array\t[%d, %s]\t%s\t%d\t%d\n", e->indices.start.N, e->indices.end.id, terminals[e->type], e->offset, table->level);
                    }
                }
            }
            else{
                printf("no\t---\t---\t\t%s\t%d\t%d\n", terminals[e->type], e->offset, table->level);
            }
        }
        else{
            if(e->isArr == 1){
                printf("yes\t");
                if(e->indices.isDynamic==0){
                    printf("Static array\t[%d, %d]\t%s\t%d\t%d\n", e->indices.start.N, e->indices.end.N, terminals[e->type], e->offset, table->level-1);
                }
                else{
                    if(e->indices.isStartID==1&&e->indices.isEndID==1){
                        printf("Dynamic array\t[%s, %s]\t%s\t%d\t%d\n", e->indices.start.id, e->indices.end.id, terminals[e->type], e->offset, table->level-1);
                    }
                    else if(e->indices.isStartID==1&&e->indices.isEndID==0){
                        printf("Dynamic array\t[%s, %d]\t%s\t%d\t%d\n", e->indices.start.id, e->indices.end.N, terminals[e->type], e->offset, table->level-1);
                    }
                    else if(e->indices.isStartID==0&&e->indices.isEndID==1){
                        printf("Dynamic array\t[%d, %s]\t%s\t%d\t%d\n", e->indices.start.N, e->indices.end.id, terminals[e->type], e->offset, table->level-1);
                    }
                }
            }
            else{
                printf("no\t---\t---\t\t%s\t%d\t%d\n", terminals[e->type], e->offset, table->level-1);
            }
        }
        e = e->next;
    }
    sTable * kid = table->child;
    int i = 0;
    while(kid!=NULL){
        printSymTableTree(kid);
        kid = kid->sibling;
        i++;
    }    
}

void printArrays(sTable * table){
    entry * e = table->head;
    while(e!=NULL){
        //printf("%s with type %s, Output? %d. Offset = %d. Temp address: t%d", e->lexeme, terminalNames[e->type], e->isOutput, e->offset, e->temp->temp);
            if(e->isArr == 1){
                printf("%s\t%d-%d\t%s\t", table->scope, table->scopeStart, table->scopeEnd, e->lexeme);        
                if(e->indices.isDynamic==0){
                    printf("Static array\t[%d, %d]\t%s\n", e->indices.start.N, e->indices.end.N, terminals[e->type]);
                }
                else{
                    if(e->indices.isStartID==1&&e->indices.isEndID==1){
                        printf("Dynamic array\t[%s, %s]\t%s\n", e->indices.start.id, e->indices.end.id, terminals[e->type]);
                    }
                    else if(e->indices.isStartID==1&&e->indices.isEndID==0){
                        printf("Dynamic array\t[%s, %d]\t%s\n", e->indices.start.id, e->indices.end.N, terminals[e->type]);
                    }
                    else if(e->indices.isStartID==0&&e->indices.isEndID==1){
                        printf("Dynamic array\t[%d, %s]\t%s\n", e->indices.start.N, e->indices.end.id, terminals[e->type]);
                    }
                }
            }
        e = e->next;
    }
    sTable * kid = table->child;
    int i = 0;
    while(kid!=NULL){
        printArrays(kid);
        kid = kid->sibling;
        i++;
    }    
}

void printARSizes(){
    sTable * table  = symTree->root->child;
    while(table!=NULL){
        function * fun = searchFunction(table->scope);
        if(fun!=NULL){
            printf("%s: \t\t%d\n", table->scope, calculateARSize(table->child, fun));
        }
        table = table->sibling;
    }
}

int calculateARSize(sTable * table, function * fun){
    //printf("%s...\n", fun->moduleName);
    if(table==NULL){
        return 0;
    }
    entry * e = table->head;
    while(e!=NULL){
        if(e->isOutput==0){
            fun->size += e->width;
        }
        e = e->next;    
    }
    sTable * kid = table->child;
    while(kid!=NULL){
        calculateARSize(kid, fun);
        kid = kid->sibling;
    } 
    return fun->size;
}

function * addFunctionEntry(char * name){
    if(searchFunction(name)!=NULL){
        return NULL;
    }
    function * fun = malloc(sizeof(function));
    fun->moduleName = name;
    fun->inputList = NULL;
    fun->outputList = NULL;
    fun->size = 0;
    fun->label = newLabel();
    //fprintf(errorsFile,  "Adding function %s to the list\n" , name);
    if(functions->head==NULL){
        functions->head = fun;
        functions->current = fun;
        fun->next = NULL;
        return fun;
    }
    else{
        functions->current->next = fun;
        functions->current = fun;
        fun->next = NULL;
        return fun;
    }
}

function * searchFunction(char * name){
    function * fun = functions->head;
    while(fun!=NULL){
        int i=0;
        while(name[i]==fun->moduleName[i] && fun->moduleName[i]!='\0' && name[i]!='\0'){
            i++;
        }
        if(fun->moduleName[i]=='\0' && name[i]=='\0'){
            return fun;
        }
        fun =  fun->next;
    }
    return NULL;
}

symTableTree * createSymbolTableTree(AST * syntaxTree){
    moduleOffset = 0;
    tableCount = 1;
    symTree = malloc(sizeof(symTableTree));
    sTable * master = malloc(sizeof(sTable));
    master->head = NULL;
    master->tail = NULL;
    master->child = NULL;
    master->sibling = NULL;
    master->parent = NULL;
    master->level = -1;
    master->offset = 0;
    master->prevChildVisited = -1;
    master->block = ASTNodeCount;
    symTree->current = master;
    symTree->root = master;
    astNode * node  = syntaxTree->root;
    astNode * moduleDecls = node->array[0];
    functions = malloc(sizeof(fList));
    functions->current = NULL;
    functions->head = NULL;
    while(moduleDecls!=NULL){
        if(addFunctionEntry(moduleDecls->array[0]->val.var.id)==NULL){
            fprintf(errorsFile,  "%d) Module %s was declared already\n" , moduleDecls->array[0]->lineNo, moduleDecls->array[0]->val.var.id);
        }
        moduleDecls = moduleDecls->array[1];
    }
    makeModList();
    astNode * module = node->array[1];
    while(module!=NULL){                              //for each module in <otherModules>
        module = module->array[0];
        moduleName = module->array[0]->val.var.id;
        function * funMod = searchFunction(moduleName);
        if(funMod!=NULL && funMod->inputList!=NULL){
            fprintf(errorsFile,  "%d) Module %s was defined already\n" , module->array[0]->lineNo, moduleName);
        }
        else{
            addFunctionEntry(moduleName);
        }
        sTable * inputTable = addChildTable();
        inputTable->scopeStart = module->array[3]->array[0]->lineNo;
        inputTable->scopeEnd = module->array[3]->array[2]->lineNo;
        inputTable->block = module->array[1]->nodeNumber;
        astNode * inputplist = module->array[1];
        while(inputplist!=NULL){
            //entry is made to symbol table and its reference is given to ID
            if(inputplist->array[1]->array[0]==NULL){
                inputplist->array[0]->val.var.ptr = addEntry(inputplist->array[0]->lineNo, inputTable, inputplist->array[0]->val.var.id, inputplist->array[1]->state, 0);
            }
            else{
                inputplist->array[0]->val.var.ptr = addEntry(inputplist->array[0]->lineNo, inputTable, inputplist->array[0]->val.var.id, inputplist->array[1]->array[1]->state, 1);
                if(inputplist->array[1]->array[0]->array[0]->state==55 && inputplist->array[1]->array[0]->array[1]->state==55){
                    inputplist->array[0]->val.var.ptr->indices.isDynamic = 0;
                    inputplist->array[0]->val.var.ptr->indices.isStartID = 0;
                    inputplist->array[0]->val.var.ptr->indices.isEndID = 0;
                    inputplist->array[0]->val.var.ptr->indices.start.N = inputplist->array[1]->array[0]->array[0]->val.n;
                    inputplist->array[0]->val.var.ptr->indices.end.N = inputplist->array[1]->array[0]->array[1]->val.n;
                }
                else{
                    inputplist->array[0]->val.var.ptr->indices.isDynamic = 1;
                    if(inputplist->array[1]->array[0]->array[0]->state==54){
                        inputplist->array[0]->val.var.ptr->indices.isStartID = 1;
                        inputplist->array[0]->val.var.ptr->indices.start.id = inputplist->array[1]->array[0]->array[0]->val.var.id;
                    }
                    else{
                        inputplist->array[0]->val.var.ptr->indices.isStartID = 0;
                        inputplist->array[0]->val.var.ptr->indices.start.N = inputplist->array[1]->array[0]->array[0]->val.n;
                    }
                    if(inputplist->array[1]->array[0]->array[1]->state==54){
                        inputplist->array[0]->val.var.ptr->indices.isEndID = 1;
                        inputplist->array[0]->val.var.ptr->indices.end.id = inputplist->array[1]->array[0]->array[1]->val.var.id;
                    }
                    else{
                        inputplist->array[0]->val.var.ptr->indices.isEndID = 0;
                        inputplist->array[0]->val.var.ptr->indices.end.N = inputplist->array[1]->array[0]->array[1]->val.n;
                    }
                }
            }
            inputplist = inputplist->array[2];
        }
        astNode * outputplist = module->array[2];
        sTable * othersTable = addChildTable();
        othersTable->scopeStart = module->array[3]->array[0]->lineNo;
        othersTable->scopeEnd = module->array[3]->array[2]->lineNo;
        othersTable->block = module->array[3]->nodeNumber;

        while(outputplist!=NULL){
            if(outputplist->array[1]->array[0]==NULL){
                outputplist->array[0]->val.var.ptr = addEntry(outputplist->array[0]->lineNo, othersTable, outputplist->array[0]->val.var.id, outputplist->array[1]->state, 0);
                outputplist->array[0]->val.var.ptr->isOutput = 1;
                outputplist = outputplist->array[2];
            }
            else{
                fprintf(errorsFile,  "Array declared in output! ERROR!!\n" );
            }
        }
        function * fun = searchFunction(module->array[0]->val.var.id);  //if there was a declaration, update information
        if(fun!=NULL){
            if(fun->inputList==NULL){
                fun->inputList = inputTable;
                fun->outputList = othersTable;
            }
            else{
                fprintf(errorsFile,  "%d) Function %s defined already\n" , module->array[1]->array[0]->lineNo, fun->moduleName);
            }
            
        }
        //add other variables to the same table
        astNode * statements = module->array[3]->array[1];
        //fprintf(errorsFile,  "Going into ad entries to table\n" );
        addEntriesToTable(fun, statements);
        //fprintf(errorsFile,  "Here, out of add entries to table\n" );
        entry * out = NULL;
        if(fun!=NULL){
            out = fun->outputList->head;
        }
        while(out!=NULL){
            if(out->isOutput==1){
                fprintf(errorsFile,  "%d) Error: Return parameter %s not assigned a value\n" , out->lineNo, out->lexeme);
            }
            out = out->next;
        }
        symTree->current = symTree->current->parent;
        symTree->current = symTree->current->parent;
        module = module->parent->array[1];  //move to the next module
        // printf("Moving to the next module\n");
    }
    astNode * driverStatements = node->array[2]->array[1];
    if(driverStatements!=NULL){
        moduleName = "driver";
        sTable * subtable = addChildTable();
        subtable->scopeStart = node->array[2]->array[0]->lineNo;
        subtable->scopeEnd = node->array[2]->array[2]->lineNo;
        subtable->block = driverStatements->nodeNumber;
        addEntriesToTable(NULL, driverStatements);
        symTree->current = symTree->current->parent;
    }
    module = node->array[3];
    while(module!=NULL){                              //for each module in <otherModules>
        module = module->array[0];                     
        moduleName = module->array[0]->val.var.id;
        function * funMod = searchFunction(moduleName);
        if(funMod!=NULL && funMod->inputList!=NULL){
            fprintf(errorsFile,  "%d) Module %s was defined already\n" , module->array[0]->lineNo, moduleName);
        }
        else{
            addFunctionEntry(moduleName);
        }
        sTable * inputTable = addChildTable();
        inputTable->scopeStart = module->array[3]->array[0]->lineNo;
        inputTable->scopeEnd = module->array[3]->array[2]->lineNo;
        inputTable->block = module->array[1]->nodeNumber;
        astNode * inputplist = module->array[1];
        while(inputplist!=NULL){
            //entry is made to symbol table and its reference is given to ID
            if(inputplist->array[1]->array[0]==NULL){
                inputplist->array[0]->val.var.ptr = addEntry(inputplist->array[0]->lineNo, inputTable, inputplist->array[0]->val.var.id, inputplist->array[1]->state, 0);
            }
            else{
                inputplist->array[0]->val.var.ptr = addEntry(inputplist->array[0]->lineNo, inputTable, inputplist->array[0]->val.var.id, inputplist->array[1]->array[1]->state, 1);
                if(inputplist->array[1]->array[0]->array[0]->state==55 && inputplist->array[1]->array[0]->array[1]->state==55){
                    inputplist->array[0]->val.var.ptr->indices.isDynamic = 0;
                    inputplist->array[0]->val.var.ptr->indices.isStartID = 0;
                    inputplist->array[0]->val.var.ptr->indices.isEndID = 0;
                    inputplist->array[0]->val.var.ptr->indices.start.N = inputplist->array[1]->array[0]->array[0]->val.n;
                    inputplist->array[0]->val.var.ptr->indices.end.N = inputplist->array[1]->array[0]->array[1]->val.n;
                }
                else{
                    inputplist->array[0]->val.var.ptr->indices.isDynamic = 1;
                    if(inputplist->array[1]->array[0]->array[0]->state==54){
                        inputplist->array[0]->val.var.ptr->indices.isStartID = 1;
                        inputplist->array[0]->val.var.ptr->indices.start.id = inputplist->array[1]->array[0]->array[0]->val.var.id;
                    }
                    else{
                        inputplist->array[0]->val.var.ptr->indices.isStartID = 0;
                        inputplist->array[0]->val.var.ptr->indices.start.N = inputplist->array[1]->array[0]->array[0]->val.n;
                    }
                    if(inputplist->array[1]->array[0]->array[1]->state==54){
                        inputplist->array[0]->val.var.ptr->indices.isEndID = 1;
                        inputplist->array[0]->val.var.ptr->indices.end.id = inputplist->array[1]->array[0]->array[1]->val.var.id;
                    }
                    else{
                        inputplist->array[0]->val.var.ptr->indices.isEndID = 0;
                        inputplist->array[0]->val.var.ptr->indices.end.N = inputplist->array[1]->array[0]->array[1]->val.n;
                    }
                }
                
            }
            inputplist = inputplist->array[2];
        }
        astNode * outputplist = module->array[2];
        sTable * othersTable = addChildTable();
        othersTable->scopeStart = module->array[3]->array[0]->lineNo;
        othersTable->scopeEnd = module->array[3]->array[2]->lineNo;
        othersTable->block = module->array[3]->nodeNumber;
        while(outputplist!=NULL){
            if(outputplist->array[1]->array[0]==NULL){
                outputplist->array[0]->val.var.ptr = addEntry(outputplist->array[0]->lineNo, othersTable, outputplist->array[0]->val.var.id, outputplist->array[1]->state, 0);
                outputplist->array[0]->val.var.ptr->isOutput = 1;
                outputplist = outputplist->array[2];
            }
            else{
                fprintf(errorsFile,  "Array declared in output! ERROR!!\n" );
            }
        }
        function * fun = searchFunction(module->array[0]->val.var.id);  //if there was a declaration, update information
        if(fun!=NULL){
            if(fun->inputList==NULL){
                fun->inputList = inputTable;
                fun->outputList = othersTable;
            }
            else{
                fprintf(errorsFile,  "%d) Function %s defined already\n" , module->array[1]->array[0]->lineNo, fun->moduleName);
            }
            
        }
        //add other variables to the same table
        astNode * statements = module->array[3]->array[1];
        addEntriesToTable(fun, statements);
        entry * out = fun->outputList->head;
        while(out!=NULL){
            if(out->isOutput==1){
                fprintf(errorsFile,  "%d) Error: Return parameter %s not assigned a value\n", out->lineNo, out->lexeme);
            }
            out = out->next;
        }
        symTree->current = symTree->current->parent;
        symTree->current = symTree->current->parent;
        module = module->parent->array[1];  //move to the next module
    }
}

void addEntriesToTable(function * fun, astNode * statements){
    while(statements!=NULL){
        astNode * statement = statements->array[0];
        if(statement->state==31){       //declare
            if(statement->array[1]->array[0]==NULL){
                int datatype = statement->array[1]->state;
                astNode * idlist = statement->array[0];
                while(idlist!=NULL){
                    idlist->array[0]->val.var.ptr = addEntry(idlist->array[0]->lineNo, symTree->current, idlist->array[0]->val.var.id, datatype, 0);
                    idlist = idlist->array[1];
                }
            }
            else{
                //printf("Here\n");
                int datatype = statement->array[1]->array[1]->state;
                astNode * idlist = statement->array[0];
                while(idlist!=NULL){
                    idlist->array[0]->val.var.ptr = addEntry(idlist->array[0]->lineNo, symTree->current, idlist->array[0]->val.var.id, datatype, 1);
                    if(statement->array[1]->array[0]->array[0]->state==55 && statement->array[1]->array[0]->array[1]->state==55){
                        //printf("Static array\n");
                        int typesize;
                        if(idlist->array[0]->val.var.ptr->type==24){
                            typesize = 2;
                        }
                        else if(idlist->array[0]->val.var.ptr->type==25){
                            typesize = 4;
                        }
                        else{
                            typesize = 1;
                        }
                        idlist->array[0]->val.var.ptr->width += (statement->array[1]->array[0]->array[1]->val.n-statement->array[1]->array[0]->array[0]->val.n+1)*typesize;
                        moduleOffset+=(statement->array[1]->array[0]->array[1]->val.n-statement->array[1]->array[0]->array[0]->val.n+1)*typesize;
                        idlist->array[0]->val.var.ptr->indices.isDynamic = 0;
                        idlist->array[0]->val.var.ptr->indices.isStartID = 0;
                        idlist->array[0]->val.var.ptr->indices.isEndID = 0;
                        idlist->array[0]->val.var.ptr->indices.start.N = statement->array[1]->array[0]->array[0]->val.n;
                        idlist->array[0]->val.var.ptr->indices.end.N = statement->array[1]->array[0]->array[1]->val.n;
                    }
                    else{
                        printf("Dynamic array\n");
                        idlist->array[0]->val.var.ptr->indices.isDynamic = 1;
                        if(statement->array[1]->array[0]->array[0]->state==54){
                            printf("Start index is id\n");
                            idlist->array[0]->val.var.ptr->indices.isStartID = 1;
                            idlist->array[0]->val.var.ptr->indices.start.id = statement->array[1]->array[0]->array[0]->val.var.id;
                        }
                        else{
                            printf("Start index is NUM");
                            idlist->array[0]->val.var.ptr->indices.isStartID = 0;
                            idlist->array[0]->val.var.ptr->indices.start.N = statement->array[1]->array[0]->array[0]->val.n;
                        }
                        if(statement->array[1]->array[0]->array[1]->state==54){
                            printf("End index is id\n");
                            idlist->array[0]->val.var.ptr->indices.isEndID = 1;
                            idlist->array[0]->val.var.ptr->indices.end.id = statement->array[1]->array[0]->array[1]->val.var.id;
                        }
                        else{
                            printf("End index is NUM\n");
                            idlist->array[0]->val.var.ptr->indices.isEndID = 0;
                            idlist->array[0]->val.var.ptr->indices.end.N = statement->array[1]->array[0]->array[1]->val.n;
                        }
                    }
                    idlist = idlist->array[1];
                }
            }
        }
        else if(statement->state==117){   //IO Stmt
            if(statement->array[0]->state==35){      //<ioStmt> ---> GET_VALUE BO ID BC SEMICOL
                sTable * tbl = symTree->current;
                int present = presentInTable(statement->array[1]->val.var.id, tbl);
                while(present ==-1){
                    tbl = tbl->parent;
                    if(tbl==NULL){
                        fprintf(errorsFile,  "\n%d) ERROR: Undeclared variable %s is used\n\n" , statement->array[0]->lineNo ,statement->array[1]->val.var.id);
                        break;
                    }
                    present = presentInTable(statement->array[1]->val.var.id, tbl);
                }
            }
            else if(statement->array[1]->state==119){   //<ioStmt> ---> PRINT BO <var_id_num> BC SEMICOL ... <var_id_num> ---> ID <whichId>
                sTable * tbl = symTree->current;
                int present = presentInTable(statement->array[1]->array[0]->val.var.id, tbl);
                while(present ==-1){
                    tbl = tbl->parent;
                    if(tbl==NULL){
                        fprintf(errorsFile,  "\n%d) ERROR: Undeclared variable %s is used\n\n" , statement->array[0]->lineNo ,statement->array[1]->array[0]->val.var.id);
                        break;
                    }
                    present = presentInTable(statement->array[1]->array[0]->val.var.id, tbl);
                }
            }

        }
        else if(statement->state==49){  //switch
            // printf("Adding child table\n");
            sTable * subtable = addChildTable();
            subtable->scopeStart = statement->array[0]->lineNo;
            subtable->scopeEnd = statement->array[3]->lineNo;
            subtable->block = statement->nodeNumber;
            //fprintf(errorsFile,  "Done\n" );
            if(statement->array[1]!=NULL){
                astNode * caseStatements = statement->array[1];
                while(caseStatements!=NULL){                    
                    addEntriesToTable(fun, caseStatements->array[1]);
                    caseStatements = caseStatements->array[2];
                }
            }
            if(statement->array[2]!=NULL){
                addEntriesToTable(fun, statement->array[2]->array[0]);
            }
            symTree->current = symTree->current->parent;
            int switchIDType = getTypeTerminal(statement->array[0]);            
            if(switchIDType==26){
                astNode * temp = statement->array[1];
                while(temp!=NULL){
                    if(temp->array[0]->state==55){
                        fprintf(errorsFile,  "%d) Error: Case variable is not of boolean type\n" , temp->array[0]->lineNo);
                    }
                    temp = temp->array[2];
                }
                if(statement->array[2]!=NULL){
                    fprintf(errorsFile,  "%d) Error: Switch variable %s is boolean. It should not have a default statement\n" , statement->array[0]->lineNo, statement->array[0]->val.var.id);                
                }
            }
            else if(switchIDType==25){
                fprintf(errorsFile,  "%d) Error: Switch variable is of real type\n" , statement->array[0]->lineNo);
            }
            else{
                if(statement->array[2]==NULL){
                    fprintf(errorsFile,  "%d) Error: Switch variable %s is INTEGER type. It should have a default statement\n" , statement->array[0]->lineNo, statement->array[0]->val.var.id);                
                }
                astNode * temp = statement->array[1];
                while(temp!=NULL){
                    if(temp->array[0]->state!=55){
                        fprintf(errorsFile,  "%d) Error: Case variable is not of INTEGER type\n" , temp->array[0]->lineNo);
                    }
                    temp = temp->array[2];
                }
            }
        }
        else if(statement->state==47){  //for
            if(statement->array[2]!=NULL){
                sTable * subtable = addChildTable();
                subtable->block = statement->nodeNumber;
                subtable->scopeStart = statement->array[3]->lineNo;
                subtable->scopeEnd = statement->array[4]->lineNo;
                addEntriesToTable(fun, statement->array[2]);
                char * iterId = statement->array[0]->val.var.id;
                int reDeclare = findVar(iterId, subtable);
                if(reDeclare!=-1){
                    fprintf(errorsFile,  "%d) Error: Iterator variable %s of for loop of line %d redeclared\n" , reDeclare, iterId, statement->array[0]->lineNo);
                }
                symTree->current = symTree->current->parent;
            }
        }
        else if(statement->state==53){  //while
            if(statement->array[1]!=NULL){
                sTable * subtable = addChildTable();
                subtable->block = statement->nodeNumber;
                subtable->scopeStart = statement->array[2]->lineNo;
                subtable->scopeEnd = statement->array[3]->lineNo;
                addEntriesToTable(fun, statement->array[1]);
                symTree->current = symTree->current->parent;
            }
        }
        else if(statement->state == 18){    //assignment
            //printf("Checking type of assignment statement\n");
            checkType(statement);
            //printf("Going to if fun!=NULL\n");
            if(fun!=NULL){
                entry * present = presentInTableOutput(statement->array[0]->val.var.id, fun->outputList);
                if(present!=NULL && getTypeTerminal(statement->array[0]) == present->type){
                    present->isOutput = 2;
                }            
            }
        }
        else if(statement->state == 128){   //module reuse
            if(statement->array[0]!=NULL){
                astNode * idList = statement->array[0];
                while(idList!=NULL){
                    sTable * tbl = symTree->current;
                    int present = presentInTable(idList->array[0]->val.var.id, tbl);
                    while(present ==-1){
                        tbl = tbl->parent;
                        if(tbl==NULL){
                            fprintf(errorsFile,  "\n%d) ERROR: Undeclared variable %s is used\n\n" , statement->array[1]->lineNo ,idList->array[0]->val.var.id);
                            break;
                        }
                        present = presentInTable(idList->array[0]->val.var.id, tbl);
                    }
                    idList = idList->array[1];
                }
            }
            //input list check
            astNode * idList = statement->array[2];
            while(idList!=NULL){
                sTable * tbl = symTree->current;
                int present = presentInTable(idList->array[0]->val.var.id, tbl);
                while(present ==-1){
                    tbl = tbl->parent;
                    if(tbl==NULL){
                        fprintf(errorsFile,  "\n%d) ERROR: Undeclared variable %s is used\n\n" , statement->array[1]->lineNo ,idList->array[0]->val.var.id);
                        break;
                    }
                    present = presentInTable(idList->array[0]->val.var.id, tbl);
                }
                idList = idList->array[1];
            }
            // printf("\n\n\nIn module reuse stmt\n");
            function * fun = searchFunction(statement->array[1]->val.var.id);
            if(fun==NULL){
                fprintf(errorsFile,  "%d) Error: Function %s was never declared/defined\n" , statement->array[1]->lineNo, statement->array[1]->val.var.id);
                statements = statements->array[1];
                continue;
            }
            if(fun->inputList==NULL){
                //fprintf(errorsFile, "%d) Function %s not defined yet\n" , statement->array[1]->lineNo, statement->array[1]->val.var.id);
                addStmtModList(statement, symTree->current);
                // printf("Here\n");
                statements = statements->array[1];
                continue;
            }
            else{
                // printf("Checking for recursion\n");
                sTable * temp = symTree->current;
                while(temp->parent->parent!=NULL){
                    temp = temp->parent;
                }
                if(temp==fun->inputList){
                    fprintf(errorsFile,  "%d) Error: Function %s is recursively defined\n" , statement->array[1]->lineNo, statement->array[1]->val.var.id);
                }
                else{
                    // printf("No recursion\n");
                }
            }
            astNode * list = statement->array[0];
            if(list!=NULL){     //output list
                entry * out = fun->outputList->head;
                int flag = 0;
                if((out==NULL||out->isOutput==0) && list!=NULL){
                    fprintf(errorsFile,  "%d) Error: Function %s doesn't return a value\n", statement->array[1]->lineNo, statement->array[1]->val.var.id);
                    flag = 1;
                }
                if(flag=0){
                    while(list!=NULL){
                        // printf("Here\n");
                        if(out==NULL||out->isOutput==0){
                            fprintf(errorsFile,  "%d) Error: Number of parameters is module reuse statement's output list does not match with that in definition\n" , statement->array[1]->lineNo);
                            break;
                        }
                        //if(out->type!=presentInTable(list->array[0]->val.var.id, symTree->current)){
                        if(out->type!=getPtrEntry(list->array[0]->val.var.id)->type){
                            fprintf(errorsFile,  "%d) Error: Type of parameter %s in output list does not match with that in definition\n" , statement->array[1]->lineNo, list->array[0]->val.var.id);
                        }
                        list = list->array[1];
                        out = out->next;
                    }
                    // printf("Outside while\n");
                    if(out!=NULL&&out->isOutput==1){
                        fprintf(errorsFile,  "%d) Error: Number of parameters is module reuse statement's output list does not match with that in definition\n" , statement->array[1]->lineNo);
                    }
                    if(list!=NULL && (out==NULL||out->isOutput!=1)){
                        fprintf(errorsFile,  "%d) Error: Number of parameters is module reuse statement's output list does not match with that in definition\n" , statement->array[1]->lineNo);
                    }
                    else{
                        // printf("Output is proper\n");
                    }
                }
            }
            list = statement->array[2];
            if(list!=NULL){     //input list
                entry * in = fun->inputList->head;
                while(list!=NULL){
                    if(in==NULL){
                        fprintf(errorsFile,  "%d) Error: Number of parameters is module reuse statement's input list does not match with that in definition\n" , statement->array[1]->lineNo);
                        break;
                    }
                    if(in->type!=getPtrEntry(list->array[0]->val.var.id)->type){
                        fprintf(errorsFile,  "%d) Error: Type of parameter %s in input list does not match with that in definition\n" , statement->array[1]->lineNo, list->array[0]->val.var.id);
                    }
                    list = list->array[1];
                    in = in->next;
                }
                if(in!=NULL){
                    fprintf(errorsFile,  "%d) Error: Number of parameters is module reuse statement's input list does not match with that in definition\n" , statement->array[1]->lineNo);
                }
            }
            
        }
        statements = statements->array[1];
        if(statements==NULL){
            // printf("Statements==NULL\n");
        }
    }
    // printf("Out of while loop\n");
}

modUseStmtsList * makeModList(){
    modList = malloc(sizeof(modUseStmtsList));
    modList->head = NULL;
    modList->tail = NULL;
    return modList;
}

void addStmtModList(astNode * stmt, sTable * table){
    modUseStmt * temp = malloc(sizeof(modUseStmt));
    temp->statement = stmt;
    temp->table = table;
    temp->next = NULL;
    if(modList->head==NULL){
        modList->head = temp;
        modList->tail = temp;
    }
    else{
        modList->tail->next = temp;
        modList->tail = temp;
    }
}

int checkModList(){
    // printf("Entered checkmodlist\n");
    modUseStmt * temp = modList->head;
    while(temp!=NULL){
        // printf("%s is being processed\n", temp->statement->array[1]->val.var.id);
        function * fun = searchFunction(temp->statement->array[1]->val.var.id);
        if(fun->inputList==NULL){
            fprintf(errorsFile,  "%d) Error: Function %s was never defined\n" , temp->statement->array[1]->lineNo, temp->statement->array[1]->val.var.id);
            temp = temp->next;
            continue;
        }
        // printf("Error only after this\n");
        astNode * list = temp->statement->array[0];
        if(list!=NULL){     //output list
            entry * out = fun->outputList->head;
            int flag = 0;
            // printf("Flag is %d\n", flag);
            if((out==NULL||out->isOutput==0) && list!=NULL){
                fprintf(errorsFile,  "%d) Error: Function %s doesn't return a value\n", temp->statement->array[1]->lineNo, temp->statement->array[1]->val.var.id);
                flag = 1;
            }
            // printf("Flag is %d\n", flag);
            if(flag=0){
                while(list!=NULL){
                    // printf("Here\n");
                    if(out==NULL||out->isOutput==0){
                        fprintf(errorsFile,  "%d) Error: Number of parameters is module reuse statement's output list does not match with that in definition\n" , temp->statement->array[1]->lineNo);
                        break;
                    }
                    if(out->type!=presentInTable(list->array[0]->val.var.id, temp->table)){
                        fprintf(errorsFile,  "%d) Error: Type of parameter %s in output list does not match with that in definition\n" , temp->statement->array[1]->lineNo, list->array[0]->val.var.id);
                    }
                    list = list->array[1];
                    out = out->next;
                }
                // printf("Outside while\n");
                if(out!=NULL&&out->isOutput==1){
                    fprintf(errorsFile,  "%d) Error: Number of parameters is module reuse statement's output list does not match with that in definition\n" , temp->statement->array[1]->lineNo);
                }
                else{
                    // printf("Output is proper\n");
                }
            }   
        }
        // printf("Outside output list\n");
        list = temp->statement->array[2];
        if(list!=NULL){     //input list
            entry * in = fun->inputList->head;
            while(list!=NULL){
                // printf("Here\n");
                if(in==NULL){
                    fprintf(errorsFile,  "%d) Error: Number of parameters is module reuse statement's input list does not match with that in definition\n" , temp->statement->array[1]->lineNo);
                    break;
                }
                if(in->type!=presentInTable(list->array[0]->val.var.id, temp->table)){
                    fprintf(errorsFile,  "%d) Error: Type of parameter %s in input list does not match with that in definition\n" , temp->statement->array[1]->lineNo, list->array[0]->val.var.id);
                }
                list = list->array[1];
                in = in->next;
            }
            if(in!=NULL){
                fprintf(errorsFile,  "%d) Error: Number of parameters is module reuse statement's input list does not match with that in definition\n" , temp->statement->array[1]->lineNo);
            }
        }
        // printf("Outside inputList\n"); 
        temp = temp->next; 
    }
}

int findVar(char * iterId, sTable * table){         //returns lineNo of declaration if found, -1 otherwise
    int res = presentInTableLine(iterId, table);
    if(res!=-1){
        return res;
    }
    else{
        sTable * temp = table->child;
        while(temp!=NULL){
            res = presentInTableLine(iterId, temp);
            if(res!=-1){
                return res;
            }
            temp = temp->sibling;
        }
        return -1;
    }
}

int presentInTableLine(char * name, sTable * table){        //returns type of variable if present, -1 otherwise
    entry * e = table->head;
    while(e!=NULL){
        int j = 0;
        while(name[j]!='\0' && e->lexeme[j]!='\0' && name[j]==e->lexeme[j]){
            j++;
        }
        if(name[j]=='\0' && e->lexeme[j]=='\0'){
            return e->lineNo;
        }
        e = e->next;
    }
    return -1;
}

sTable * addChildTable(){
    tableCount ++;
    sTable * table = malloc(sizeof(sTable));
    table->head = NULL;
    table->tail = NULL;
    table->child = NULL;
    table->sibling = NULL;
    table->level = symTree->current->level+1;
    if(symTree->current->parent==NULL){
        moduleOffset = 0;
    }
    table->scope = moduleName;
    table->offset = moduleOffset;
    table->parent = symTree->current;
    table->prevChildVisited = -1;
    table->block = -1;
    sTable * temp = symTree->current->child;
    if(temp==NULL){
        symTree->current->child = table; 
    }
    else{
        while(temp->sibling!=NULL){
            temp = temp->sibling;
        }
        temp->sibling = table;
    }
    symTree->current = table;
    return table;
}

entry * addEntry(int lineNo, sTable * table, char * lexeme, int type, int isArr){
    entry * temp = table->head;
    entry * e = newEntry();
    addr * tmp = malloc(sizeof(addr));
    tmp->temp = newTemp();
    e->temp = tmp;
    while(temp!=NULL){
        if(strcmp(temp->lexeme, lexeme)==0){
            fprintf(errorsFile,  "\n%d) Variable %s declared already!!\n\n" , lineNo, lexeme);
            break;
        }
        temp = temp->next;
    }
    e->isOutput = 0;
    e->lineNo = lineNo;
    e->lexeme = lexeme;
    e->type = type;
    e->isArr = isArr;
    if(isArr==0){
        if(type == 24){
            e->width = 2;
        }
        else if(type == 25){
            e->width = 4;
        }
        else if(type == 26){
            e->width = 1;
        }
    }
    else if(table->level==0){
        e->width = 5;
    }
    else{
        e->width = 1;
    }
    e->offset = moduleOffset;
    moduleOffset+=e->width;
    if(table->head==NULL && table->tail==NULL){
        table->head = e;
        table->tail = e;
        e->next = NULL;
    }
    else{
        table->tail->next = e;
        table->tail = e;
    }
    addtt(tmp->temp, type, e);
    return e;
}

void initializeTtlist(){
    ttlist = malloc(sizeof(tempTypeList));
    ttlist->head = NULL;
    ttlist->tail = NULL;
}

void addtt(int temp, int type, entry * e){
    tempType * t = malloc(sizeof(tempType));
    t->temp =temp;
    t->type = type;
    t->e = e;
    t->next = NULL;
    if(ttlist->head==NULL){
        ttlist->head = t;
        ttlist->tail = t;
        return;
    }
    else{
        ttlist->tail->next = t;
        ttlist->tail = t;
        return;
    }
}

int getTypeOfTemp(int temp){
    tempType * t = ttlist->head;
    while(t!=NULL && t->temp!=temp){
        t = t->next;
    }
    if(t==NULL){
        // printf("Temp has no entry, returning -1");
        return -1;
    }
    return t->type;
}

int IsTempArray(int temp){
    tempType * t = ttlist->head;
    while(t!=NULL && t->temp!=temp){
        t = t->next;
    }
    if(t==NULL){
        //printf("Temp has no entry, returning -1\n");
        return -1;
    }
    if(t->e->indices.isDynamic == 1){
        printf("Searching for a Dynamic Array? Returning -2\n");
        return -2;
    }
    if(t->e->isArr == 1){
        return t->type;
    }
    return 0;
}

int GetArraySize(int temp){
    tempType * t = ttlist->head;
    while(t!=NULL && t->temp!=temp){
        t = t->next;
    }
    if(t==NULL){
        //printf("Temp has no entry, returning -1\n");
        return -1;
    }
    return (t->e->indices.end.N - t->e->indices.start.N);
}

int getStartIndex(int temp){
    tempType * t = ttlist->head;
    while(t!=NULL && t->temp!=temp){
        t = t->next;
    }
    if(t==NULL){
        //printf("Temp has no entry, returning -1");
        return -1;
    }
    return t->e->indices.start.N;
}

int getEndIndex(int temp){
    tempType * t = ttlist->head;
    while(t!=NULL && t->temp!=temp){
        t = t->next;
    }
    if(t==NULL){
        //printf("Temp has no entry, returning -1");
        return -1;
    }
    return t->e->indices.end.N;
}

void printTTList(){
    tempType * t = ttlist->head;
    while(t!=NULL){
        printf("t%d: %s, type-%s\n", t->temp, t->e->lexeme, terminalNames[t->type]);
        t = t->next;
    }
}
