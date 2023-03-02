#include "ff.h"
#include "lexer.h"


int buildAST(){
    ASTNodeCount = 0;
    syntaxTree = newAST();
    syntaxTree->root = getPtr(parseTree->root);
    connectChildrenToParent(syntaxTree->root);
    // printf("Done\n");
    //preOrderPrint(syntaxTree->root);
}

void connectChildrenToParent(astNode * node){
    if(node!=NULL){
        for(int i=0; i<5; i++){
            if(node->array[i]!=NULL){
                node->array[i]->parent = node;
                connectChildrenToParent(node->array[i]);
            }
        }
    }
}

astNode * preOrderPrint(astNode * nd){
    if(nd==NULL){
        return NULL;
    }
    else{
        if(nd->state==55){
            printf("%d) %s = %d", nd->nodeNumber, terminalNames[nd->state], nd->val.n);
            // for(int i =0; i<5; i++){
            //     if(nd->array[i]!=NULL){
            //         printf("%d ", nd->array[i]->state);
            //     }
            // }
            printf("\n");
        }
        else if(nd->state==54){            
            printf("%d) %s = %s", nd->nodeNumber, terminalNames[nd->state], nd->val.var.id);
            // for(int i =0; i<5; i++){
            //     if(nd->array[i]!=NULL){
            //         printf("%d ", nd->array[i]->state);
            //     }
            // }
            printf("\n");
        }
        else if(nd->state<MAX_TER){
            printf("%d) %s", nd->nodeNumber, terminalNames[nd->state]);
            // for(int i =0; i<5; i++){
            //     if(nd->array[i]!=NULL){
            //         printf("%d ", nd->array[i]->state);
            //     }
            // }
            printf("\n");
        }
        else{
            printf("%d) <%s>", nd->nodeNumber, nonTerminals[nd->state-100]);
            // for(int i =0; i<5; i++){
            //     if(nd->array[i]!=NULL){
            //         printf("%d ", nd->array[i]->state);
            //     }
            // }
            printf("\n");
        }    
    }
    for(int i=0; i<5; i++){
        preOrderPrint(nd->array[i]);
    }
    return nd;
}


astNode * getPtr(node * pnode){
    if(pnode->state >= 100){
        node * temp = pnode->child;
        node * temp1;
        astNode * arr[5] = {NULL};
        record * value = malloc(sizeof(record));
        record * value1 = malloc(sizeof(record));
        record * value2 = malloc(sizeof(record));
        switch (pnode->grule)
        {
        case 0:     //<program> ---> <moduleDeclarations><otherModules><driverModule><otherModules>
            // printf"In case 0\n");
            // printf"Pnode is %d, Temp is %d, temp sibling is %d, temp's sibling rule is %d\n", pnode->state, temp->state, temp->sibling->state, temp->sibling->grule);                    
            arr[0] = getPtr(temp);
            if(arr[0]==NULL){
            // printf"Got AST node for <moduleDeclarations> as null\n");
            }
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling;
            arr[2] = getPtr(temp);
            temp = temp->sibling;
            arr[3] = getPtr(temp);
            // printf"here1\n");
            return makeNodeAST(pnode->state, arr);
        case 1:     //<moduleDeclarations> ---> <moduleDeclaration><moduleDeclarations>
            // printf"In case 1\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);        
            return makeNodeAST(pnode->state, arr);
        case 2:     //<moduleDeclarations> ---> e
            // printf"In case 2\n");
            return NULL;
        case 3:     //<moduleDeclaration> ---> DECLARE MODULE ID SEMICOL
            // printf"In case 3\n");
            temp = temp->sibling->sibling;
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 32, 54, value);
        case 4:     //<otherModules> ---> <module><otherModules>
            // printf"In case 4\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);        
            return makeNodeAST(pnode->state, arr);
        case 5:     //<otherModules> ---> e 
            // printf"In case 5\n");
            return NULL;
        case 6:     //<driverModule> ---> DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>
            // printf"In case 6\n");
            return getPtr(temp->sibling->sibling->sibling->sibling);
        case 7:     //<module> ---> DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist>SQBC SEMICOL <ret><moduleDef>
            // printf"In case 7\n");
            temp = temp->sibling->sibling;
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 32, 54, value);
            temp = temp->sibling->sibling->sibling->sibling->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling->sibling->sibling;
            arr[2] = getPtr(temp);
            temp =temp->sibling;
            arr[3] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 8:     //<ret> ---> RETURNS SQBO <output_plist>SQBC SEMICOL
            // printf"In case 8\n");
            return getPtr(temp->sibling->sibling);
        case 9:     //<ret> ---> e
            // printf"In case 9\n");
            return NULL;
        case 10:    //<input_plist> ---> ID COLON <dataType><input_plist1>
            // printf"In case 10\n");
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 54, 54, value);
            temp = temp->sibling->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling;
            arr[2] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 11: //<input_plist1> ---> COMMA ID COLON <dataType><input_plist1>
            // printf"In case 11\n");
            temp = temp->sibling;
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 54, 54, value);
            temp = temp->sibling->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling;
            arr[2] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 12:    //<input_plist1> ---> e
            // printf"In case 21\n");
            return NULL;
        case 13:    //<output_plist> ---> ID COLON <type><output_plist1>
            // printf"In case 13\n");
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 54, 54, value);
            temp = temp->sibling->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling;
            arr[2] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 14: //<output_plist1> ---> COMMA ID COLON <type><output_plist1>
            // printf"In case 14\n");
            temp = temp->sibling;
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 54, 54, value);
            temp = temp->sibling->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling;
            arr[2] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 15:    //<output_plist1> ---> e
            // printf"In case 15\n");
            return NULL;
        case 16:    //<dataType> ---> INTEGER
            // printf"In case 16\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 24, 54, value);
        case 17:    //<dataType> ---> REAL
            // printf"In case 17\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 25, 54, value);
        case 18:    //<dataType> ---> BOOLEAN
            // printf"In case 18\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 26, 54, value);
        case 19:    //<dataType> ---> ARRAY SQBO <range_arrays>SQBC OF <type>
            // printf"In case 19\n");
            temp = temp->sibling->sibling;
            arr[0] = getPtr(temp);
            temp = temp->sibling->sibling->sibling;
            arr[1] = getPtr(temp);
            // printf"After 50 in datatype\n");
            return makeNodeAST(28, arr);
        case 20:    //<range_arrays> ---> <index>RANGEOP <index>
            // printf"In case 20\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 21:    //<type> ---> INTEGER
            // printf"In case 21\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 24, 54, value);
        case 22:    //<type> ---> REAL
            // printf"In case 22\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 25, 54, value);
        case 23:    //<type> ---> BOOLEAN
            // printf"In case 23\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 26, 54, value);
        case 24:    //moduleDef> ---> START <statements>END
            // printf"In case 24\n");
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 26, 54, value);
            arr[1] = getPtr(temp->sibling);
            value1->var.id = temp->sibling->sibling->lexeme;
            arr[2] = makeLeafAST(temp->sibling->sibling->lineNo, 26, 54, value);
            return makeNodeAST(pnode->state, arr);
        case 25:    //<statements> ---> <statement><statements>
            // printf"In case 25\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 26:    //<statements> ---> e
            // printf"In case 26\n");
            return NULL;
        case 27:    //<statement> ---> <ioStmt>
            // printf"In case 27\n");
            return getPtr(temp);
        case 28:    //<statement> ---> <simpleStmt>
            // printf"In case 28\n");
            return getPtr(temp);
        case 29:    //<statement> ---> <declareStmt>
            // printf"In case 29\n");
            return getPtr(temp);
        case 30:    //<statement> ---> <conditionalStmt>
            // printf"In case 30\n");
            return getPtr(temp);
        case 31:    //<statement> ---> <iterativeStmt>
            // printf"In case 31\n");
            return getPtr(temp);
        case 32:    //<ioStmt> ---> GET_VALUE BO ID BC SEMICOL
            // printf"In case 32\n");
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 35, 54, value);
            temp = temp->sibling->sibling;
            value1->var.id = temp->lexeme;
            arr[1] = makeLeafAST(temp->lineNo, 54, 54, value1);
            return makeNodeAST(pnode->state, arr);
        case 33:    //<ioStmt> ---> PRINT BO <var>BC SEMICOL
            // printf"In case 33\n");
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 36, 54, value);
            temp = temp->sibling->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 34:    //<var> ---> <var_id_num>
            // printf"In case 34\n");
            return getPtr(temp);
        case 35:    //<var> ---> <boolConstt>
            // printf"In case 35\n");
            return getPtr(temp);
        case 36:    //<var_id_num> ---> ID <whichId>
            // printf"In case 36\n");
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 54, 54, value);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(119, arr);
        case 37:    //<var_id_num> ---> NUM
            // printf"In case 37\n");
            value->n = getNum(temp->lexeme);
            return makeLeafAST(temp->lineNo, 55, 55, value);
        case 38:    //<var_id_num> ---> RNUM
            // printf"In case 38\n");
            value->f = getNum(temp->lexeme);
            return makeLeafAST(temp->lineNo, 56, 56, value);
        case 39:    //<boolConstt> ---> TRUE
            // printf"In case 39\n");
            value->n = 1;
            return makeLeafAST(temp->lineNo, 40, 40, value);
        case 40:    //<boolConstt> ---> FALSE
            // printf"In case 40\n");
            value->n = 0;
            return makeLeafAST(temp->lineNo, 41, 41, value);
        case 41:    //<whichId> ---> SQBO <index>SQBC 
            // printf"In case 43\n");
            return getPtr(temp->sibling);       
        case 42:    //<whichId> ---> e
            // printf"In case 42\n");
            return NULL;
        case 43:    //<simpleStmt> ---> <assignmentStmt>
            // printf"In case 43\n");
            return getPtr(temp);
        case 44:    //<simpleStmt> ---> <moduleReuseStmt>
            // printf"In case 44\n");
            return getPtr(temp);
        case 45:    //<assignmentStmt> ---> ID <whichStmt>
            // printf"In case 45\n");
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 54, 54, value);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(18, arr);
        case 46:    //<whichStmt> ---> <lvalueIDStmt>
            // printf"In case 46\n");
            return getPtr(temp);
        case 47:    //<whichStmt> ---> <lvalueARRStmt>
            // printf"In case 47\n");
            return getPtr(temp);
        case 48:    //<lvalueIDStmt> ---> ASSIGNOP <expression>SEMICOL
            // printf"In case 46\n");
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 18, 54, value);
            arr[1] = getPtr(temp->sibling);
            return makeNodeAST(pnode->state, arr);
        case 49:    //<lvalueARRStmt> ---> SQBO <index>SQBC ASSIGNOP <expression>SEMICOL
            // printf"In case 49\n");
            temp = temp->sibling;
            arr[0] = getPtr(temp);
            temp = temp->sibling->sibling;
            value->var.id = temp->lexeme;
            arr[1] = makeLeafAST(temp->lineNo, 18, 54, value);
            temp = temp->sibling;
            arr[2] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 50:    //<index> ---> NUM
            // printf"In case 50\n");
            value->n = getNum(temp->lexeme);
            return makeLeafAST(temp->lineNo, 55, 55, value);
        case 51:    //<index> ---> ID
            // printf"In case 51\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 54, 54, value);
        case 52:    //<moduleReuseStmt> ---> <optional>USE MODULE ID WITH PARAMETERS <idList>SEMICOL
            // printf"In case 52\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling->sibling->sibling;
            value->var.id = temp->lexeme;
            arr[1] = makeLeafAST(temp->lineNo, 54, 54, value);
            temp = temp->sibling->sibling->sibling;
            arr[2] = getPtr(temp);
            return makeNodeAST(128, arr);
        case 53:    //<optional> ---> SQBO <idList>SQBC ASSIGNOP
            // printf"In case 53\n");
            return getPtr(temp->sibling);
        case 54:    //<optional> ---> e
            // printf"In case 54\n");
            return NULL;
        case 55:    //<idList> ---> ID <idList1>
            // printf"In case 55\n");
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 54, 54, value);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 56: //<idList1> ---> COMMA ID <idList1>
            // printf"In case 56\n");
            temp = temp->sibling;
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 54, 54, value);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 57:    //<idList1> ---> e
            // printf"In case 57\n");
            return NULL;
        case 58:    //<expression> ---> <arithmeticOrBooleanExpr>
            // printf"In case 58\n");
            return getPtr(temp);
        case 59:    //<expression> ---> <U>
            // printf"In case 59\n");
            return getPtr(temp);
        case 60:    //<U> ---> <unary_op><new_NT>
            // printf"In case 60\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(133, arr);
        case 61:    //<new_NT> ---> BO <arithmeticExpr>BC
            // printf"In case 61\n");
            return getPtr(temp->sibling);
        case 62:    //<new_NT> ---> <var_id_num>
            // printf"In case 62\n");
            return getPtr(temp);
        case 63:    //<unary_op> ---> PLUS
            // printf"In case 63\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 0, 54, value);
        case 64:    //<unary_op> ---> MINUS
            // printf"In case 64\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 1, 54, value);
        case 65:    //<arithmeticOrBooleanExpr> ---> <AnyTerm><arithmeticOrBooleanExpr1>
            // printf"In case 65\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(136, arr);
        case 66:    //<arithmeticOrBooleanExpr1> ---> <logicalOp><AnyTerm><arithmeticOrBooleanExpr1>
            // printf"In case 66\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling;
            arr[2] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 67:    //<arithmeticOrBooleanExpr1> ---> e 
            // printf"In case 67\n");
            return NULL;
        case 68:    //<AnyTerm> ---> <arithmeticExpr><AnyTerm1>
            // printf"In case 69\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(138, arr);
        case 69:    //<AnyTerm> ---> <boolConstt>
            // printf"In case 70\n");
            return getPtr(temp);
        case 70:    //<AnyTerm1> ---> <relationalOp><arithmeticExpr>
            // printf"In case 71\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);    
        case 71:    //<AnyTerm1> ---> e 
            // printf"In case 72\n");
            return NULL;
        case 72:    //<arithmeticExpr> ---> <term><arithmeticExpr1>
            // printf"In case 73\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(140, arr);
        case 73:    //<arithmeticExpr1> ---> <op1><term><arithmeticExpr1>
            // printf"In case 74\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling;
            arr[2] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 74:    //<arithmeticExpr1> ---> e
            // printf"In case 75\n");
            return NULL;
        case 75:    //<term> ---> <factor><term1>
            // printf"In case 76\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 76:    //<term1> ---> <op2><factor><term1>
            // printf"In case 77\n");
            arr[0] = getPtr(temp);
            temp = temp->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling;
            arr[2] = getPtr(temp);
            return makeNodeAST(pnode->state, arr);
        case 77:    //<term1> ---> e
            // printf"In case 78\n");
            return NULL;
        case 78:    //<factor> ---> <var_id_num>
            // printf"In case 79\n");
            return getPtr(temp);
        case 79:    //<factor> ---> BO <arithmeticOrBooleanExpr>BC
            // printf"In case 80");
            return getPtr(temp->sibling);
        case 80:    //<op1> ---> PLUS
            // printf"In case 81\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 0, 54, value);
        case 81:    //<op1> ---> MINUS
            // printf"In case 82\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 1, 54, value);
        case 82:    //<op2> ---> MUL
            // printf"In case 83\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 2, 54, value);
        case 83:    //<op2> ---> DIV
            // printf"In case 84\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 3, 54, value);
        case 84:    //<logicalOp> ---> AND
            // printf"In case 85\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 45, 54, value);
        case 85:    //<logicalOp> ---> OR
            // printf"In case 86\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 46, 54, value);
        case 86:    //<relationalOp> ---> LT
            // printf"In case 87\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 4, 54, value);
        case 87:    //<relationalOp> ---> LE
            // printf"In case 88\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 5, 54, value);
        case 88:    //<relationalOp> ---> GT
            // printf"In case 89\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 6, 54, value);
        case 89:    //<relationalOp> ---> GE
            // printf"In case 90\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 7, 54, value);
        case 90:    //<relationalOp> ---> EQ
            // printf"In case 91\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 8, 54, value);
        case 91:    //<relationalOp> ---> NE
            // printf"In case 92\n");
            value->var.id = temp->lexeme;
            return makeLeafAST(temp->lineNo, 9, 54, value);        
        case 92:    //<declareStmt> ---> DECLARE <idList>COLON <dataType>SEMICOL
            // printf"In case 93\n");
            temp = temp->sibling;
            arr[0] = getPtr(temp);
            temp = temp->sibling->sibling;
            arr[1] = getPtr(temp);
            // printf"BACK in declare\n");
            return makeNodeAST(31, arr);//for DECLARE
        case 93:    //<conditionalStmt> ---> SWITCH BO ID BC START <caseStmts><default>END
            // printf"In case 94\n");
            temp = temp->sibling->sibling;
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 54, 54, value);
            temp = temp->sibling->sibling->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling;
            arr[2] = getPtr(temp);
            value1->var.id = "SWITCH_END";
            arr[3] = makeLeafAST(temp->sibling->lineNo, 54, 54, value1);
            return makeNodeAST(49, arr);//for SWITCH
        case 94:    //<caseStmts> ---> CASE <value>COLON <statements>BREAK SEMICOL <caseStmts1>
            // printf"In case 95\n");
            temp = temp->sibling;
            arr[0] = getPtr(temp);
            temp1 = temp->sibling;
            temp = temp->sibling->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling->sibling->sibling;
            arr[2] = getPtr(temp);
            value1->var.id = "CASE_START";
            arr[3] = makeLeafAST(temp1->lineNo, 54, 54, value);
            value2->var.id = "CASE_END";
            temp1 = temp1->sibling->sibling->sibling;
            arr[4] = makeLeafAST(temp1->lineNo, 54, 54, value);
            return makeNodeAST(pnode->state, arr);
        case 95:    //<caseStmts1> ---> CASE <value>COLON <statements>BREAK SEMICOL <caseStmts1>
            // printf"In case 96\n");
            temp = temp->sibling;
            arr[0] = getPtr(temp);
            temp1 = temp->sibling;
            temp = temp->sibling->sibling;
            arr[1] = getPtr(temp);
            temp = temp->sibling->sibling->sibling;
            arr[2] = getPtr(temp);
            value1->var.id = "CASE_START";
            arr[3] = makeLeafAST(temp1->lineNo, 54, 54, value);
            value2->var.id = "CASE_END";
            temp1 = temp1->sibling->sibling->sibling;
            arr[4] = makeLeafAST(temp1->lineNo, 54, 54, value);
            return makeNodeAST(pnode->state, arr);
        case 96:    //<caseStmts1> ---> e
            // printf"In case 97\n");
            return NULL;
        case 97:    //<value> ---> NUM
            // printf"In case 98\n");
            value->n = getNum(temp->lexeme);
            return makeLeafAST(temp->lineNo, 55, 55, value);
        case 98:    //<value> ---> TRUE
            // printf"In case 99\n");
            value->n = 1;
            return makeLeafAST(temp->lineNo, 40, 40, value);
        case 99:    //<value> ---> FALSE
            // printf"In case 100\n");
            value->n = 0;
            return makeLeafAST(temp->lineNo, 41, 41, value);
        case 100:   //<default> ---> DEFAULT COLON <statements>BREAK SEMICOL
            // printf"In case 101\n");
            arr[0] = getPtr(temp->sibling->sibling);
            value1->var.id = "CASE_START";
            arr[3] = makeLeafAST(temp->lineNo, 54, 54, value);
            value2->var.id = "CASE_END";
            temp = temp->sibling->sibling->sibling;
            arr[4] = makeLeafAST(temp->lineNo, 54, 54, value);
            return makeNodeAST(pnode->state, arr);
        case 101:   //<default> ---> e
            // printf"In case 102\n");
            return NULL;
        case 102:   //<iterativeStmt> ---> FOR BO ID IN <range>BC START <statements>END
            // printf"In case 103\n");
            temp = temp->sibling->sibling;
            value->var.id = temp->lexeme;
            arr[0] = makeLeafAST(temp->lineNo, 54, 54, value);
            temp = temp->sibling->sibling;
            arr[1] = getPtr(temp);
            temp1 = temp->sibling->sibling;
            temp = temp->sibling->sibling->sibling;
            arr[2] = getPtr(temp);
            value1->var.id = "FOR_START";
            arr[3] = makeLeafAST(temp1->lineNo, 54, 54, value);
            value2->var.id = "FOR_END";
            temp1 = temp1->sibling->sibling;
            arr[4] = makeLeafAST(temp1->lineNo, 54, 54, value);
            return makeNodeAST(47, arr);//for FOR
        case 103:   //<iterativeStmt> ---> WHILE BO <arithmeticOrBooleanExpr>BC START <statements>END
            // printf"In case 104\n");
            temp = temp->sibling->sibling;
            arr[0] = getPtr(temp);
            temp1 = temp;
            temp = temp->sibling->sibling->sibling;
            arr[1] = getPtr(temp);
            value->var.id = "WHILE_START";
            arr[2] = makeLeafAST(temp1->sibling->sibling->lineNo, 54, 54, value);
            value1->var.id = "WHILE_END";
            arr[3] = makeLeafAST(temp->sibling->lineNo, 54, 54, value);            
            return makeNodeAST(53, arr);//for WHILE
        case 104:   //<range> ---> NUM RANGEOP NUM
            // printf"In case 105\n");
            value->n = getNum(temp->lexeme);
            arr[0] = makeLeafAST(temp->lineNo, 55, 55, value);
            temp = temp->sibling->sibling;
            value1->n = getNum(temp->lexeme);
            arr[1] = makeLeafAST(temp->lineNo, 55, 55, value1);
            return makeNodeAST(pnode->state, arr);
        default:
            printf("Error: Entered default state during AST creation\n");
            return NULL;
            break;
        }
    }
}

int getNum(char* lexeme){
    int num = 0;
    int p = 1;
    int i=0;
    while(lexeme[i]!='\0'){
        i++;
    }
    i--;
    while(i>=0){
        num += p*(lexeme[i]-'0');
        p *= 10;
        i--;
    }
    return num;
}

AST * newAST(){
    AST * T = malloc(sizeof(AST));
    return T;
}

astNode * makeNodeAST(int state, astNode * arr[5]){
    astNode * node = malloc(sizeof(astNode));
    node->state = state;
    node->nodeNumber = ASTNodeCount;
    ASTNodeCount++;
    for(int i=0; i<5; i++){
        node->array[i] = arr[i];
    }
    for(int i=0; i<4; i++){
        if(node->array[i]!=NULL)
            node->array[i]->sibling = arr[i+1];
    }
    if(node->array[4]!=NULL){
        node->array[4]->sibling = NULL;
    }
    return node;
}

astNode * makeLeafAST(int lineNo, int state, int valueType, record * value){
    astNode * leaf = malloc(sizeof(astNode));
    leaf->nodeNumber = ASTNodeCount;
    leaf->lineNo = lineNo;
    ASTNodeCount++;
    leaf->state = state;
    if(valueType == 54){
        leaf->val.var.id = value->var.id;
    }
    else if(valueType==55){
        leaf->val.n = value->n;
    }
    else if(valueType = 56){
        leaf->val.f = value->f;
    }
    else if(valueType = 40){
        leaf->val.n = 1;
    }
    else if(valueType = 41){
        leaf->val.n = 0;
    }
    return leaf;
}
