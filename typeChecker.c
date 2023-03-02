#include "ff.h"
#include "lexer.h"

int checkType(astNode * assignmentStmt){
    int lhsType = getTypeTerminal(assignmentStmt->array[0]);
    entry * entrylhs = getPtrEntry(assignmentStmt->array[0]->val.var.id);
    if(entrylhs->isArr==1){
        lhsType = entrylhs->type;
    }
    // printf("Got lhs\n");
    int rhsType;
    if(assignmentStmt->array[1]->array[2]==NULL){
        //printf("In simple part, asking to get type of node with state %d\n", assignmentStmt->array[1]->array[1]->state);
        if(getPtrEntry(assignmentStmt->array[0]->val.var.id)->isArr==1){
            astNode * temp = assignmentStmt->array[1]->array[1];
            if(temp==NULL || temp->array[1]!=NULL){
                fprintf(errorsFile, "%d) Error: Array %s is assigned to an expression1\n", assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id);
                return -1;
            }
            if(1){
                //fprintf(errorsFile, "Temp is now %d\n", temp->state);
                temp = temp->array[0];
                if(temp==NULL || temp->array[1]!=NULL){
                    fprintf(errorsFile, "%d) Error: Array %s is assigned to an expression2\n", assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id);
                    return -1;
                }
                if(1){
                    //fprintf(errorsFile, "Temp is now %d\n", temp->state);
                    temp = temp->array[0];//temp is now arith expr
                    if(temp==NULL || temp->array[1]!=NULL){
                        fprintf(errorsFile, "%d) Error: Array %s is assigned to an expression3\n", assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id);
                        return -1;
                    }
                    if(1){
                        //fprintf(errorsFile, "Temp is now %d\n", temp->state);
                        temp = temp->array[0];//temp is now term 
                        if(temp==NULL || temp->array[1]!=NULL){
                            fprintf(errorsFile, "%d) Error: Array %s is assigned to an expression4\n", assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id);
                            return -1;
                        }
                        if(1){
                            //fprintf(errorsFile, "Temp is now %d\n", temp->state);
                            temp = temp->array[0];//temp is now varidnum
                            if(temp==NULL || temp->array[1]!=NULL){
                                fprintf(errorsFile, "%d) Error: Array %s is assigned to an expression6\n", assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id);
                                return -1;
                            }
                            if(1){
                                //fprintf(errorsFile, "Temp is now %d\n", temp->state);
                                temp = temp->array[0];//temp is now terminal
                                if(temp==NULL || temp->state!=54){
                                    fprintf(errorsFile, "%d) Error: Array %s is assigned to an expression7\n", assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id);
                                    if(temp==NULL){
                                        fprintf(errorsFile, "Temp is null\n");
                                    }
                                    return -1;
                                }
                                else{
                                    entry * e1 = getPtrEntry(temp->val.var.id);
                                    entry * e2 = getPtrEntry(assignmentStmt->array[0]->val.var.id);
                                    if(e1->type!=e2->type){
                                        fprintf(errorsFile, "%d) Error: Type mismatch\n", assignmentStmt->array[0]->lineNo);                                        
                                    }
                                    if(e1==NULL || e2==NULL){
                                        fprintf(errorsFile, "%d)Undeclares variable of type array is used", assignmentStmt->array[0]->lineNo);
                                        return 1;
                                    }
                                    if(e1->indices.isStartID==1){
                                        if(e2->indices.isStartID!=1){
                                            fprintf(errorsFile, "%d) Error: Type mismatch\n", assignmentStmt->array[0]->lineNo);                                        
                                            return -1;
                                        }
                                        else if(getPtrEntry(e1->indices.start.id)!=getPtrEntry(e2->indices.start.id)){
                                            fprintf(errorsFile, "%d) Error: Type mismatch\n", assignmentStmt->array[0]->lineNo);                                                                                        
                                            return -1;
                                        }
                                    }
                                    else{
                                        if(e2->indices.isStartID==1){
                                            fprintf(errorsFile, "%d) Error: Type mismatch\n", assignmentStmt->array[0]->lineNo);                                        
                                            return -1;
                                        }
                                        else if(e1->indices.start.N!=e2->indices.start.N){
                                            fprintf(errorsFile, "%d) Error: Type mismatch\n", assignmentStmt->array[0]->lineNo);                                                                                        
                                            return -1;
                                        }
                                    }
                                    if(e1->indices.isEndID==1){
                                        if(e2->indices.isEndID!=1){
                                            fprintf(errorsFile, "%d) Error: Type mismatch\n", assignmentStmt->array[0]->lineNo);                                        
                                            return -1;
                                        }
                                        else if(getPtrEntry(e1->indices.end.id)!=getPtrEntry(e2->indices.end.id)){
                                            fprintf(errorsFile, "%d) Error: Type mismatch\n", assignmentStmt->array[0]->lineNo);                                                                                        
                                            return -1;
                                        }
                                    }
                                    else{
                                        if(e2->indices.isEndID==1){
                                            fprintf(errorsFile, "%d) Error: Type mismatch\n", assignmentStmt->array[0]->lineNo);                                        
                                            return -1;
                                        }
                                        else if(e1->indices.end.N!=e2->indices.end.N){
                                            fprintf(errorsFile, "%d) Error: Type mismatch\n", assignmentStmt->array[0]->lineNo);                                                                                        
                                            return -1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        rhsType = getTypeExpr(assignmentStmt->array[1]->array[1]);
    }
    else{
        //printf(" %d : In array part\n", assignmentStmt->array[0]->lineNo);
        //<assignmentStmt> ---> ID <whichStmt/valueARRStmt> ---> SQBO <index>SQBC ASSIGNOP <expression>SEMICOL
        entry * id = getPtrEntry(assignmentStmt->array[0]->val.var.id);
        if(id==NULL){
            fprintf(errorsFile, "%d) Error: %s was not declared\n", assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id);
        }
        else if(id->isArr==0){
            fprintf(errorsFile, "%d) Error: %s is not an array. It cannot be indexed\n", assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id);
        }
        else if(id->indices.isDynamic==0){
            //printf("Static array\n");
            if(assignmentStmt->array[1]->array[0]->state==55){
                if(assignmentStmt->array[1]->array[0]->val.n>id->indices.end.N || assignmentStmt->array[1]->array[0]->val.n<id->indices.start.N){
                    fprintf(errorsFile, "%d) Error: %s index is out of bounds\n" , assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id);
                }
            }
            if(getTypeTerminal(assignmentStmt->array[1]->array[0])!=24){
                fprintf(errorsFile, "%d) Error: %s index is not of type integer\n", assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id);
            }
        }
        else{
            if(getTypeTerminal(assignmentStmt->array[1]->array[0])!=24){
                fprintf(errorsFile,  "%d) Error: %s index is not of type integer\n" , assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id);
            }
        }
        rhsType = getTypeExpr(assignmentStmt->array[1]->array[2]);
    }
    if(rhsType==-1){
        fprintf(errorsFile,  "%d) Error: Type mismatch in RHS of assignment operation %s = ...\n" , assignmentStmt->array[0]->lineNo, assignmentStmt->array[0]->val.var.id );
        return -1;
    }
    if(lhsType!=rhsType){
        fprintf(errorsFile,  "%d) Error: Type mismatch in assignment operation\n" , assignmentStmt->array[0]->lineNo);
        return -1;
    }
    // printf("%d) Correct Assignment statement %d\n", assignmentStmt->array[0]->lineNo, rhsType);
    return 0;
}

int getTypeExpr(astNode * node){
    if(node->state == 133){
        //printf("In Expr, going to U with state %d\n", node->state);
        return getTypeU(node);
    }
    else{
        //printf("Going to arithOrBoolExpr with state %d\n", node->state);
        return getTypeArithOrBoolExpr(node);
    }
}

int getTypeU(astNode * node){
    return getTypeNewNT(node->array[1]);
}

int getTypeNewNT(astNode * node){
    if(node->state == 140){
        //printf("Going to arith expr with state %d", node->state);
        return getTypeArithExpr(node);
    }
    else{
        return getTypeVarIdNum(node);
    }
}

int getTypeArithOrBoolExpr(astNode * node){
    //printf("Going to AnyTerm, with state %d\n", node->array[0]->state);
    int t1 = getTypeAnyTerm(node->array[0]);
    if(node->array[1]!=NULL){
        int t2 = getTypeArithOrBoolExpr1(node->array[1]);
        if(t1==-1||t2==-1||t1!=t2){
        //printf("Here t1!=t2\n");
            return -1;
        }
        else{
            return 26;
        }
    }
    else{
        return t1;
    }
}

int getTypeArithOrBoolExpr1(astNode * node){
    if(node->array[2]==NULL){
        return getTypeAnyTerm(node->array[1]);
    }
    else{
        int t1 = getTypeAnyTerm(node->array[1]);
        if(t1==-1){
            return -1;
        }
        int t2 = getTypeArithOrBoolExpr1(node->array[2]);
        if(t1!=t2){
            return -1;
        }
        return t1;
    }
}

int getTypeAnyTerm(astNode * node){
    if(node->state==40 || node->state==41){
        return 26;
    }
    else{
        //printf("Going to ArithExpr\n");
        int temp = getTypeArithExpr(node->array[0]);
        //printf("Got arith\n");
        if(temp == -1){ 
            return -1;
        }        
        else if(node->array[1]==NULL){            
            return temp;
        }
        else{
            int temp1 = getTypeArithExpr(node->array[1]->array[1]);
            if(temp==temp1){
                return 26;
            }
            else{
                return -1;
            }

        }
    }
}

int getTypeArithExpr(astNode * node){
    // printf("Entered arith\n");
    if(node==NULL){
        // printf("Node is null!?!?!?!\n");
    }
    if(node->array[1]==NULL){
        //printf("Going to term\n");
        return getTypeTerm(node->array[0]);
    }
    else{
        // printf("Here instead, now going to get term\n");
        int t1 = getTypeTerm(node->array[0]);
        // printf("Here now, now going to get arith expr 1\n");
        int t2 = getTypeArithExpr1(node->array[1]);
        if(t1!=t2||t1==-1){
            return -1;
        }
        else{
            return t1;
        }
    }
}

int getTypeArithExpr1(astNode * node){
    if(node->array[2]==NULL){
        // printf("Getting term\n");
        return getTypeTerm(node->array[1]);
    }
    else{
        int t1 = getTypeTerm(node->array[1]);
        int t2 = getTypeArithExpr1(node->array[2]);
        if(t1!=t2||t1==-1){
            return -1;
        }
        else{
            return t1;
        }
    }
}

int getTypeTerm(astNode * node){
    if(node->array[1]==NULL){
        //printf("Going to factor with state %d\n", node->array[0]->state);
        return getTypeFactor(node->array[0]);
    }
    else{
        //printf("Going to factor with state %d\n", node->array[0]->state);
        int t1 = getTypeFactor(node->array[0]);
        //printf("Back from factor. t1 is %s, getting term1\n", terminalNames[t1]);
        int t2 = getTypeTerm1(node->array[1]);
        if(t1!=t2||t1==-1){
            //printf("In term, returning -1. t2 is %s\n", terminalNames[t2]);
            return -1;
        }
        else{
            return t1;
        }
    }
}

int getTypeTerm1(astNode * node){
    if(node->array[2]==NULL){
        //printf("Going to factor\n");
        return getTypeFactor(node->array[1]);
    }
    else{
        int t1 = getTypeFactor(node->array[1]);
        int t2 = getTypeTerm1(node->array[2]);
        if(t1!=t2||t1==-1){
            return -1;
        }
        else{
            return t1;
        }
    }
}

int getTypeFactor(astNode * node){
    if(node->state==136){
        return getTypeArithOrBoolExpr(node);
    }
    else{
        //printf("Going to varidnum with state %d\n", node->state);
        return getTypeVarIdNum(node);
    }
}

int getTypeVarIdNum(astNode * node){
    if(node->state!=119){
        //printf("Going to terminal, with state %d\n", node->state);
        return getTypeTerminal(node);
    }
    else{
        //<var_id_num> ---> ID <whichId>
        //<whichId> ---> SQBO <index>SQBC
        int IDType = getTypeTerminal(node->array[0]);
        if(node->array[1]!=NULL){
            //fprintf(errorsFile,  "//<var_id_num> ---> ID <index>\n" );
            //<var_id_num> ---> ID <index>
            entry * id = getPtrEntry(node->array[0]->val.var.id);
            if(id!=NULL  && id->isArr == 0){
                fprintf(errorsFile,  "%d) ERROR: %s is not an Array\n" , node->array[0]->lineNo, node->array[0]->val.var.id);
            }else{
                // printf("In else\n");
                int indexType = getTypeTerminal(node->array[1]);
                if(indexType==-1){
                    // printf("Indextype is -1\n");
                }
                else if(node->array[1]->state == 55){    //index->NUM
                    // printf("Index ---> NUM\n");
                    if(id->indices.isDynamic==0){
                        //printf(" %d : Checking index bounds\n", node->array[0]->lineNo);
                        if(node->array[1]->val.n > id->indices.end.N || node->array[1]->val.n < id->indices.start.N){
                            fprintf(errorsFile,  "%d) ERROR: Index of array %s out of bounds\n" , node->array[0]->lineNo, node->array[0]->val.var.id);
                        }else{
                            // printf("Correct\n");
                        }
                    }
                }
                else if(node->array[1]->state == 54){//index->ID                
                    // printf("Index ---> ID\n");
                    if(indexType!=24){
                        fprintf(errorsFile,  "%d) ERROR: Index %s of array %s is not of type integer\n" , node->array[0]->lineNo, node->array[1]->val.var.id, node->array[0]->val.var.id);
                    }
                }
            }
        }
        //printf("Going to terminal, with state %d\n", node->array[0]->state);
        return IDType;        
    }
}

int getTypeBoolConstt(astNode * node){
    return 26;
}

int getTypeTerminal(astNode * node){
    if(node->state == 55){
        return 24;
    }
    else if(node->state == 56){
        return 25;
    }
    else if(node->state == 40 || node->state == 41){
        return 26;
    }
    else if(node->state == 54){     //for id
        sTable * table = symTree->current;
        //fprintf(errorsFile,  "This table was searched for %s\n" , node->val.var.id);
        int present = presentInTable(node->val.var.id, table);
        //fprintf(errorsFile, A"Here, type = %d", present);
        while(present ==-1){
            table = table->parent;
            if(table==NULL){
                fprintf(errorsFile,  "\n%d) ERROR: Undeclared variable %s is used\n\n" , node->lineNo ,node->val.var.id);
                return -1;
            }
            present = presentInTable(node->val.var.id, table);
        }
        return present;
    }
}

entry * presentInTableEntry(char * name, sTable * table){       //returns ptr to entry if id name is present in symbol table table
    entry * e = table->head;
    while(e!=NULL){
        int j = 0;
        while(name[j]!='\0' && e->lexeme[j]!='\0' && name[j]==e->lexeme[j]){
            j++;
        }
        if(name[j]=='\0' && e->lexeme[j]=='\0'){
            return e;
        }
        e = e->next;
    }
    return NULL;
}

entry * getPtrEntry(char * name){
    // printf("Here\n");
    sTable * table = symTree->current;
    if(table==NULL){
        return NULL;
    }
    else{
        // printf("Table is not null\n");
    }
    entry * present = presentInTableEntry(name, table);
    while(present == NULL){
        table = table->parent;
        if(table==NULL){
            return NULL;
        }
        present = presentInTableEntry(name, table);
    }
    return present;
}

int presentInTable(char * name, sTable * table){        //returns type of variable if present, -1 otherwise
    entry * e = table->head;
    while(e!=NULL){
        int j = 0;
        while(name[j]!='\0' && e->lexeme[j]!='\0' && name[j]==e->lexeme[j]){
            j++;
        }
        if(name[j]=='\0' && e->lexeme[j]=='\0'){
            return e->type;
        }
        e = e->next;
    }
    return -1;
}

entry * presentInTableOutput(char * name, sTable * table){        //returns type of variable if present and is an output variable, -1 otherwise
    entry * e = table->head;
    while(e!=NULL){
        int j = 0;
        while(name[j]!='\0' && e->lexeme[j]!='\0' && name[j]==e->lexeme[j]){
            j++;
        }
        if(name[j]=='\0' && e->lexeme[j]=='\0' && (e->isOutput==1 || e->isOutput==2)){
            return e;
        }
        e = e->next;
    }
    return NULL;
}

// int inOrderTypeCheck(astNode * node){
//     if(node == NULL){
//         return 0;
//     }
//     if(node->state == 54 || node->state == 55 || node->state == 56 || node->state == 40 || node->state == 41){
//         return getTypeTerminal(node);
//     }
//     else{
//         int type[5] = {0};
//         int totalType = 0;
//         for(int i=0; i<5; i++){
//             type[i] = inOrderTypeCheck(node->array[i]);
//             if(type[i]!=0){
//                 totalType = type[i];
//                 if(type[i]==-1){
//                     return -1;
//                 }
//             }
//         }
//         for(int i=0; i<5; i++){
//             if(type[i]!=0 && type[i]!=totalType){
//                 return -1;
//             }
//         }
//         return totalType;
//     }
// }
