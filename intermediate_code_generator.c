#include "ff.h"
#include "lexer.h"

void constructIntermediateCode(){
    addr * driverLabel = malloc(sizeof(addr));
    driverLabel->label = newLabel();
    addLineToList(createCodeLine(-1, 4, -1, driverLabel, NULL, NULL,  5, -1, -1), initializeCodeList());    //first line of interCodeList is goto driver
    astNode * node  = syntaxTree->root;
    //example case
    astNode * driverStatements = node->array[2]->array[1];        // in driver module
    symTree->current = symTree->root->child;  
    // printf("EnHere\n");
    while(symTree->current->block!=driverStatements->nodeNumber){       //updating current symbol table to move to driver statements
        symTree->current = symTree->current->sibling;
    }
    intermediateCodeList * driverLines = generateCodeStatements(driverStatements);
    driverLines->head->label = driverLabel->label;      //first line of driverLines has label driver
    symTree->current = symTree->root->child;
    astNode * module = node->array[1];  //othermodules
    while(module!=NULL && symTree->current->block!=driverStatements->nodeNumber){
        sTable * table1 = symTree->current;
        module = module->array[0];  //module
        astNode * moduleStatements = module->array[3]->array[1];
        intermediateCodeList * inputLines = createCodeList();
        addLineToList(createCodeLine(-1, 13, -1, NULL, NULL, NULL, -1, -1, -1), inputLines);
        astNode * inputs = module->array[1];
        while(inputs!=NULL){
            addLineToList(createCodeLine(-1, 14, -1, getPtrEntry(inputs->array[0]->val.var.id)->temp, NULL, NULL, 4, -1, -1), inputLines);
            inputs = inputs->array[2];
        }
        symTree->current = symTree->current->child;
        intermediateCodeList * moduleLines = generateCodeStatements(moduleStatements);
        inputLines->head->label = searchFunction(module->array[0]->val.var.id)->label;
        // printf("EnDone with statements\n");
        int retNo = 0;
        astNode * outputList = module->array[2];
        while(outputList!=NULL){
            // printf("EnYES\n");
            addr * retTemp = malloc(sizeof(addr));
            retTemp->ret = retNo;            
            addLineToList(createCodeLine(-1, 3, -1, getPtrEntry(outputList->array[0]->val.var.id)->temp, NULL, retTemp, 4, -1, 6), moduleLines);
            retNo++;
            outputList = outputList->array[2];
        }
        // printf("EnHere\n");
        addLineToList(createCodeLine(-1, 12, -1, NULL, NULL, NULL, -1, -1, -1), moduleLines);
        module = module->parent->array[1];
        symTree->current = table1;
        symTree->current = symTree->current->sibling;
        combineLists(inputLines, moduleLines);
        combineLists(interCodeList, inputLines);
    }
    // printf("EnDone with first other modules\n");
    symTree->current = symTree->current->sibling;
    module = node->array[3];  //othermodules
    while(module!=NULL && symTree->current->block!=driverStatements->nodeNumber){
        sTable * table1 = symTree->current;
        module = module->array[0];  //module
        astNode * moduleStatements = module->array[3]->array[1];
        intermediateCodeList * inputLines = createCodeList();
        addLineToList(createCodeLine(-1, 13, -1, NULL, NULL, NULL, -1, -1, -1), inputLines);
        astNode * inputs = module->array[1];
        while(inputs!=NULL){
            addLineToList(createCodeLine(-1, 14, -1, getPtrEntry(inputs->array[0]->val.var.id)->temp, NULL, NULL, 4, -1, -1), inputLines);
            inputs = inputs->array[2];
        }
        intermediateCodeList * moduleLines = generateCodeStatements(moduleStatements);
        symTree->current = symTree->current->child;
        inputLines->head->label = searchFunction(module->array[0]->val.var.id)->label;
        int retNo = 0;
        astNode * outputList = module->array[2];
        while(outputList!=NULL){
            addr * retTemp = malloc(sizeof(addr));
            retTemp->ret = retNo;            
            addLineToList(createCodeLine(-1, 3, -1, getPtrEntry(outputList->array[0]->val.var.id)->temp, NULL, retTemp, 4, -1, 6), moduleLines);
            retNo++;
            outputList = outputList->array[2];
        }
        addLineToList(createCodeLine(-1, 12, -1, NULL, NULL, NULL, -1, -1, -1), moduleLines);
        module = module->parent->array[1];
        symTree->current = table1;
        symTree->current = symTree->current->sibling;
        combineLists(inputLines, moduleLines);
        combineLists(interCodeList, inputLines);
    }
    combineLists(interCodeList, driverLines);
    codeLine * temp = interCodeList->head;
    if(temp==NULL){
        // printf("EnCode list is null\n");
    }
    printf(ANSI_COLOR_YELLOW "\n\nDone Generating Intermediate Code\n" ANSI_COLOR_RESET);
    //printList(interCodeList);
    return;
}

void printList(intermediateCodeList * list){
    codeLine * line = list->head;
    while(line!=NULL){
        printf("Rule %d:\t", line->rule);
        switch (line->rule)
        {
        case 0:
            if(line->label!=-1){
                printf("L%d: NOP\n", line->label);
            }
            else{
                printf("     NOP\n");
            }
            break;
        case 1: //x = y op z
            //printf("restype:%d, arg1type:%d, arg2type:%d\n", line->restype, line->arg1type, line->arg2type);
            if(line->label!=-1){
                printf("L%d: ", line->label);
            }
            else{
                printf("     ");
            }
            printAddress(line->res, line->restype);
            printf(" = ");
            printAddress(line->arg1, line->arg1type);
            printf(" %s ", terminals[line->op]);
            printAddress(line->arg2, line->arg2type);
            printf("\n");
            break;
        case 2: //2       x(res) = op y(arg1)    
            if(line->label!=-1){
                printf("L%d: ", line->label);
            }
            else{
                printf("     ");
            }
            printAddress(line->res, line->restype);
            printf(" = %s", terminals[line->op]);
            printAddress(line->arg1, line->arg1type);
            printf("\n");
            break;
        case 3: //x = y
            if(line->label!=-1){
                printf("L%d: ", line->label);
            }
            else{
                printf("     ");
            }
            printAddress(line->res, line->restype);
            printf(" = ");
            printAddress(line->arg1, line->arg1type);
            printf("\n");
            break;
        case 4: //goto L
            if(line->label!=-1){
                printf("L%d: goto ", line->label);
            }
            else{
                printf("     goto ");
            }
            printAddress(line->arg1, line->arg1type);
            printf("\n");
            break;
        case 5: //if x(arg1) goto L(arg2)
            if(line->label!=-1){
                printf("L%d: if ", line->label);
            }
            else{
                printf("     if ");
            }
            printAddress(line->arg1, line->arg1type);
            printf(" goto ");
            printAddress(line->arg2, line->arg2type);
            printf("\n");
            break;
        case 6: //if x relop y goto L
            if(line->label!=-1){
                printf("L%d: if ", line->label);
            }
            else{
                printf("     if ");
            }
            printAddress(line->arg1, line->arg1type);
            printf(" %s ", terminals[line->op]);
            printAddress(line->arg2, line->arg2type);
            printf(" goto ");
            printAddress(line->res, line->restype);
            printf("\n");
            break;
        case 7:     //param x(arg1)
            if(line->label!=-1){
                printf("L%d: param ", line->label);
            }
            else{
                printf("     param ");
            }
            printAddress(line->arg1, line->arg1type);
            printf("\n");
            break;
        case 8:     //call modDefLab(arg1) retLab(res) noInput(arg2)
            if(line->label!=-1){
                printf("L%d:  call ", line->label);
            }
            else{
                printf("      call ");
            }
            printAddress(line->arg1, line->arg1type);
            printf(" ");
            printAddress(line->res, line->restype);
            printf(" ");
            printAddress(line->arg2, line->arg2type);
            printf("\n");
            break;
        case 9:     //return ti(res), rj(arg1)
            if(line->label!=-1){
                printf("L%d:  return ", line->label);
            }
            else{
                printf("      return ");
            }
            printAddress(line->res, line->restype);
            printf(" ");
            printAddress(line->arg1, line->arg1type);
            printf("\n");
            break;
        case 10:    //x(res) = a(arg1)[i(arg2)]
            if(line->label!=-1){
                printf("L%d:  ", line->label);
            }
            else{
                printf("      ");
            }
            printAddress(line->res, line->restype);
            printf(" = ");
            printAddress(line->arg1, line->arg1type);
            printf("[");
            printAddress(line->arg2, line->arg2type);
            printf("]\n");
            break;
        case 11:    //b(res)[i(arg1)] = y(arg2)
            if(line->label!=-1){
                printf("L%d:  ", line->label);
            }
            else{
                printf("      ");
            }
            printAddress(line->res, line->restype);
            printf("[");
            printAddress(line->arg1, line->arg1type);
            printf("] = ");
            printAddress(line->arg2, line->arg2type);
            printf("\n");
            break;
        case 12:    //goback
            if(line->label!=-1){
                printf("L%d:  goback\n", line->label);
            }
            else{
                printf("      goback\n");
            }
            break;
        case 13:    //function
            if(line->label!=-1){
                printf("L%d:  function\n", line->label);
            }
            else{
                printf("      function\n");
            }
            break;
        case 14:    //input ti
            if(line->label!=-1){
                printf("L%d:  input ", line->label);
            }
            else{
                printf("      input ");
            }
            printAddress(line->arg1, line->arg1type);
            printf("\n");
            break;
        case 15:    //get_value(id)
            if(line->label!=-1){
                printf("L%d: get_value(", line->label);
            }
            else{
                printf("     get_value(");
            }
            printAddress(line->arg1, line->arg1type);
            printf(")\n");
            break;
        case 16:    //print(val)
            if(line->label!=-1){
                printf("L%d: print(", line->label);
            }
            else{
                printf("     print(");
            }
            printAddress(line->arg1, line->arg1type);
            printf(")\n");
            break;
        case 17:    //dynamicArray A(res)[a(arg1)..b(arg2)]
            if(line->label!=-1){
                printf("L%d:  dynamicArray ", line->label);
            }
            else{
                printf("      dynamicArray ");
            }
            printAddress(line->res, line->restype);
            printf("[");
            printAddress(line->arg1, line->arg1type);
            printf("..");
            printAddress(line->arg2, line->arg2type);
            printf("]\n");
            break;
        default:
            printf("L%d: Rule %d\n", line->label, line->rule);
            break;
        }
        line = line->next;
    }
    return;
}

void printAddress(addr * a1, int type){
    switch (type)
    {
    case 0:
        printf("%s", a1->id->lexeme);
        //printf("res");
        break;
    case 1:
        printf("%d", a1->num);
        break;
    case 2:
        printf("%f", a1->rnum);
    case 3:
        if(a1->bool==0){
            printf("FALSE");
        }
        else{
            printf("TRUE");
        }
        break;
    case 4:
        printf("t%d", a1->temp);
        break;
    case 5:
        printf("L%d", a1->label);
        break;
    case 6:
        printf("r%d", a1->ret);
    default:
        printf("NULL");
        break;
    }
}

intermediateCodeList * generateCodeStatements(astNode * stmts){
    if(stmts==NULL){
        return NULL;
    }
    astNode * stmt = stmts->array[0];
    intermediateCodeList * list = createCodeList();
    while(stmt!=NULL){
        if(stmt->state==117){   //IO
            combineLists(list, generateCodeIO(stmt));
        }
        else if(stmt->state==18){   //assignment
            // printf("n assignment statement\n");
            combineLists(list, generateCodeAssignment(stmt));
        }
        else if(stmt->state==128){ //module reuse
            // printf("n module reuse statement\n");
            combineLists(list, generateCodeModuleReuse(stmt));
        }
        else if(stmt->state==31){   //declare
            // printf("n declare\n");
            if(stmt->array[1]->state==28 && (stmt->array[1]->array[0]->array[0]->state == 54 || stmt->array[1]->array[0]->array[1]->state == 54)){
                addr * index1;
                addr * index2;
                int index1type, index2type;
                if(stmt->array[1]->array[0]->array[0]->state == 54){
                    index1 = getPtrEntry(stmt->array[1]->array[0]->array[0]->val.var.id)->temp;
                    index1type = 4;
                }
                else{
                    index1 = malloc(sizeof(addr));
                    index1->num = stmt->array[1]->array[0]->array[0]->val.n;
                    index1type = 1;
                }
                if(stmt->array[1]->array[0]->array[1]->state == 54){
                    index2 = getPtrEntry(stmt->array[1]->array[0]->array[1]->val.var.id)->temp;
                    index2type = 4;
                }
                else{
                    index2 = malloc(sizeof(addr));
                    index2->num = stmt->array[1]->array[0]->array[1]->val.n;
                    index2type = 1;
                }
                astNode * idList = stmt->array[0];
                while(idList!=NULL){//A(res)[a(arg1)..b(arg2)]
                    addLineToList(createCodeLine(-1, 17, -1, index1, index2, getPtrEntry(idList->array[0]->val.var.id)->temp, index1type, index2type, 4), list);
                    idList = idList->array[1];
                }
            }
            else{
                addLineToList(createCodeLine(-1, 0, -1, NULL, NULL, NULL, -1, -1 ,-1), list);
            }
        }
        else if(stmt->state==49){   //switch
            sTable * table1 = symTree->current;
            symTree->current = symTree->current->child;
            while(symTree->current->block!=stmt->nodeNumber){       //updating current symbol table to move to switch statement
                symTree->current = symTree->current->sibling;
            }
            combineLists(list, generateCodeSwitch(stmt));
            symTree->current = table1; 
        }
        else if(stmt->state==47){   //for
            sTable * table1 = symTree->current;
            symTree->current = symTree->current->child;
            while(symTree->current->block!=stmt->nodeNumber){       //updating current symbol table to move to for statement
                symTree->current = symTree->current->sibling;
            }
            combineLists(list, generateCodeFor(stmt));
            symTree->current = table1; 
        }
        else if(stmt->state==53){   //while
            sTable * table1 = symTree->current;
            symTree->current = symTree->current->child;
            while(symTree->current->block!=stmt->nodeNumber){       //updating current symbol table to move to while statement
                symTree->current = symTree->current->sibling;
            }
            combineLists(list, generateCodeWhile(stmt));
            symTree->current = table1;
        }
        else{
            printf("Error in statement?\n");
        }
        stmts = stmt->parent->array[1];
        if(stmts!=NULL){
            stmt = stmts->array[0];
        }
        else{
            break;
        }
    }
    return list;
}

intermediateCodeList * generateCodeModuleReuse(astNode * reuseStmt){
    intermediateCodeList * l1 = createCodeList();
    astNode * outList = reuseStmt->array[0];
    astNode * id;
    int noOut = 0;
    while(outList!=0){
        noOut++;
        outList = outList->array[1];
    }
    astNode * inpList = reuseStmt->array[2];
    int noInp = 0;
    while(inpList!=0){
        noInp++;
        id = inpList->array[0];
        inpList = inpList->array[1];
    }
    int i = noInp;
    while(i>0){
        addLineToList(createCodeLine(-1, 7, -1, getPtrEntry(id->val.var.id)->temp, NULL, NULL, 4, -1, -1), l1);  //param ti
        id = id->parent->parent->array[0];
        i--;
    }
    addr * modDefLabel = malloc(sizeof(addr));
    modDefLabel->label = searchFunction(reuseStmt->array[1]->val.var.id)->label;
    addr * L1 = malloc(sizeof(addr));
    L1->label = newLabel();
    addr * inputNo = malloc(sizeof(addr));
    inputNo->num = noInp;
    addLineToList(createCodeLine(-1, 8, -1, modDefLabel, inputNo, L1, 5, 1, 5), l1); //call modDefLab(arg1) retLab(res) noInput(arg2)
    outList = reuseStmt->array[0];
    int retNo = 0;
    intermediateCodeList * returnList = createCodeList();
    while(outList!=0){
        id = outList->array[0];
        addr * retTemp = malloc(sizeof(addr));
        retTemp->ret = retNo; 
        addLineToList(createCodeLine(-1, 9, -1, retTemp, NULL, getPtrEntry(id->val.var.id)->temp, 6, -1, 4), returnList);
        retNo++;
        noOut++;
        outList = outList->array[1];
    }
    returnList->head->label = L1->label;
    return combineLists(l1, returnList);
}

intermediateCodeList * generateCodeSwitch(astNode * switchStmt){
    entry * e =getPtrEntry(switchStmt->array[0]->val.var.id);
    if(e->type==26){//boolean
        intermediateCodeList * list = createCodeList();
        addr * t = malloc(sizeof(addr));
        t->temp = newTemp();
        addLineToList(createCodeLine(-1, 3, -1, e->temp, NULL, t, 4, -1, 4), list); //t=ID
        addr * case1Label = malloc(sizeof(addr));
        case1Label->label = newLabel();
        addr * case2Label = malloc(sizeof(addr));
        case2Label->label = newLabel();
        addr * endLabel = malloc(sizeof(addr));
        endLabel->label = newLabel();
        addr * case1 = malloc(sizeof(addr));
        case1->bool = switchStmt->array[1]->array[0]->val.n;
        // addr * case2 = malloc(sizeof(addr));
        // case2->bool = switchStmt->array[1]->array[2]->array[0]->val.n;
        addLineToList(createCodeLine(-1, 6, 8, t, case1, case1Label, 4, 3, 5), list);   //if(t==case1) goto case1 label
        // addLineToList(createCodeLine(-1, 6, 8, t, case2, case2Label, 4, 3, 5), list);   //if(t==case2) goto case2 label
        addLineToList(createCodeLine(-1, 4, -1, case2Label, NULL, NULL, 5, -1, -1), list);    //goto case2
        intermediateCodeList * st1 = generateCodeStatements(switchStmt->array[1]->array[1]); //case1 statements
        st1->head->label = case1Label->label;
        combineLists(list, st1);
        addLineToList(createCodeLine(-1, 4, -1, endLabel, NULL, NULL, 5, -1, -1), list);    //goto endLabel
        intermediateCodeList * st2 = generateCodeStatements(switchStmt->array[1]->array[2]->array[1]); //case2 statements
        st2->head->label = case2Label->label;
        combineLists(list, st2);
        addLineToList(createCodeLine(endLabel->label, 0, -1, NULL, NULL, NULL, -1, -1, -1), list);
        return list;
    }
    else{
        intermediateCodeList * list = createCodeList();
        addr * t = malloc(sizeof(addr));
        t->temp = newTemp();
        addLineToList(createCodeLine(-1, 3, -1, e->temp, NULL, t, 4, -1, 4), list); //t=ID
        astNode * s0 = switchStmt->array[1];
        int n = 0;
        while(s0!=NULL){
            s0 = s0->array[2];
            n++;
        }
        addr ** caseLabels;
        caseLabels = malloc(n*sizeof(addr*));
        for(int i = 0; i<n; i++){
            caseLabels[i] = malloc(sizeof(addr));
            caseLabels[i]->label = newLabel();
        }
        addr ** caseVal;
        caseVal = malloc(n*sizeof(addr*));
        s0 = switchStmt->array[1];
        int i = 0;
        while(s0!=NULL){
            caseVal[i] = malloc(sizeof(addr));
            caseVal[i]->num = s0->array[0]->val.n;
            addLineToList(createCodeLine(-1, 6, 8, t, caseVal[i], caseLabels[i], 4, 1, 5), list);   //if(t==casei) goto casei label
            s0 = s0->array[2];
            i++;
        }
        addr * defLabel = malloc(sizeof(addr));
        defLabel->label = newLabel();
        addr * endLabel = malloc(sizeof(addr));
        endLabel->label = newLabel();
        addLineToList(createCodeLine(-1, 4, -1, defLabel, NULL, NULL, 5, -1, -1), list);    //goto defLabel
        s0 = switchStmt->array[1];
        i = 0;
        while(s0!=NULL){
            intermediateCodeList * caseBlock = generateCodeStatements(s0->array[1]);
            caseBlock->head->label = caseLabels[i]->label;
            combineLists(list, caseBlock);            
            addLineToList(createCodeLine(-1, 4, -1, endLabel, NULL, NULL, 5, -1, -1), list);    //goto endLabel
            s0 = s0->array[2];
            i++;
        }
        intermediateCodeList * caseBlock = generateCodeStatements(switchStmt->array[2]->array[0]);
        caseBlock->head->label = defLabel->label;
        combineLists(list, caseBlock);
        addLineToList(createCodeLine(endLabel->label, 0, -1, NULL, NULL, NULL, -1, -1, -1), list);
        return list;
    }
}

intermediateCodeList * generateCodeFor(astNode * forStmt){
    addr * id = getPtrEntry(forStmt->array[0]->val.var.id)->temp;
    forStmt->array[0]->address = id;
    forStmt->array[0]->addressType = 4;
    addr * num1 = malloc(sizeof(addr));
    num1->num = forStmt->array[1]->array[0]->val.n;
    addr * num2 = malloc(sizeof(addr));
    num2->num = forStmt->array[1]->array[1]->val.n;
    intermediateCodeList * l1 = addLineToList(createCodeLine(-1, 3, -1, num1, NULL, id, 1, -1, 4), createCodeList());
    intermediateCodeList * stmts = generateCodeStatements(forStmt->array[2]);
    int loopLabel = newLabel();
    stmts->head->label = loopLabel;
    addr * loop = malloc(sizeof(addr));
    loop->label = loopLabel;
    int endLoopLabel = newLabel();
    addr * NOP = malloc(sizeof(addr));
    NOP->label = endLoopLabel;
    combineLists(l1, stmts);
    addr * increment = malloc(sizeof(addr));
    increment->num = 1;
    addLineToList(createCodeLine(-1, 1, 0, id, increment, id, 4, 1, 4), l1); //id = id + 1
    addLineToList(createCodeLine(-1, 6, 6, id, num2, NOP, 4, 1, 5), l1);    //if id>num2 goto endLoopLabel
    addLineToList(createCodeLine(-1, 4, -1, loop, NULL, NULL, 5, -1, -1), l1);  //goto loopLabel
    addLineToList(createCodeLine(endLoopLabel, 0, -1, NULL, NULL, NULL, -1, -1, -1), l1);   //endLoopLabel: NOP
    return l1;
}

intermediateCodeList * generateCodeWhile(astNode * whileStmt){
    intermediateCodeList * evalExpr = generateCodeExpression(whileStmt->array[0]);
    intermediateCodeList * stmts = generateCodeStatements(whileStmt->array[1]);
    addr * L0 = malloc(sizeof(addr));
    L0->label = newLabel();
    evalExpr->head->label = L0->label;
    addr * fls = malloc(sizeof(addr));
    fls->bool = 0;
    addr * L1 = malloc(sizeof(addr));
    L1->label = newLabel();
    addLineToList(createCodeLine(-1, 6, 8, whileStmt->array[0]->address, fls, L1, whileStmt->array[0]->addressType, 3, 5), evalExpr);   //if(expr.address == FALSE) goto L1
    combineLists(evalExpr, stmts);
    addLineToList(createCodeLine(-1, 4, -1, L0, NULL, NULL, 5, -1, -1), evalExpr);  //goto L0
    addLineToList(createCodeLine(L1->label, 0, -1, NULL, NULL, NULL, -1, -1, -1), evalExpr);  //L1: NOP
    return evalExpr;
}

intermediateCodeList * generateCodeIO(astNode * ioStmt){
    //<ioStmt> ---> GET_VALUE BO ID BC SEMICOL
    if(ioStmt->array[0]->state==35){
        addr * id = getPtrEntry(ioStmt->array[1]->val.var.id)->temp;
        return addLineToList(createCodeLine(-1, 15, -1, id, NULL, NULL, 4, -1, -1), createCodeList());
    }
    //<ioStmt> ---> PRINT BO <var>BC SEMICOL
    else{
        astNode * var = ioStmt->array[1];
        if(var->state==40){
            addr * tru = malloc(sizeof(addr));
            tru->bool = 1;
            return addLineToList(createCodeLine(-1, 16, -1, tru, NULL, NULL, 3, -1, -1), createCodeList());
        }
        else if(var->state == 41){
            addr * fls = malloc(sizeof(addr));
            fls->bool = 0;
            return addLineToList(createCodeLine(-1, 16, -1, fls, NULL, NULL, 3, -1, -1), createCodeList());            
        }
        else if(var->state == 55){
            addr * num = malloc(sizeof(addr));
            num->num = var->val.n;
            return addLineToList(createCodeLine(-1, 16, -1, num, NULL, NULL, 1, -1, -1), createCodeList());
        }
        else if(var->state == 56){
            addr * rnum = malloc(sizeof(addr));
            rnum->rnum = var->val.f;
            return addLineToList(createCodeLine(-1, 16, -1, rnum, NULL, NULL, 2, -1, -1), createCodeList());            
        }
        else if(var->array[1]==0){
            //<var> ---> ID
            return addLineToList(createCodeLine(-1, 16, -1, getPtrEntry(var->array[0]->val.var.id)->temp, NULL, NULL, 4, -1, -1), createCodeList());            
        }
        else{
            //<var> ---> ID [<index>]
            addr * id = getPtrEntry(var->array[0]->val.var.id)->temp;
            if(var->array[1]->state==55){
                addr * ret = malloc(sizeof(addr));
                ret->temp = newTemp();
                addr * num = malloc(sizeof(addr));
                num->num = var->array[1]->val.n;
                intermediateCodeList * l1 = createCodeList();
                addLineToList(createCodeLine(-1, 10, -1, id, num, ret, 4, 1, 4), l1); //res = id[num]
                return addLineToList(createCodeLine(-1, 16, -1, ret, NULL, NULL, 4, -1, -1), l1); //print(ret);
            }
        }        
    }
}

intermediateCodeList * generateCodeAssignment(astNode * assignStmt){
    //<assignmentStmt> ---> ID <whichStmt> ... <whichStmt> ---> <lvalueIDStmt> ... <lvalueIDStmt> ---> ASSIGNOP <expression>SEMICOL
    if(assignStmt->array[1]->array[0]->state==18){
        intermediateCodeList * l1 = generateCodeExpression(assignStmt->array[1]->array[1]);
        //assignStmt->array[0]->address = assignStmt->array[1]->array[1]->address;
        // printf("Here in assignment, going to get ptr entry of %s\n", assignStmt->array[0]->val.var.id);
        entry * e1 = getPtrEntry(assignStmt->array[0]->val.var.id);
        if(e1==NULL){
            // printf("Entry is null\n");
        }
        assignStmt->array[0]->address = e1->temp;
        // printf("And back here\n");
        assignStmt->array[0]->addressType = 4;
        addLineToList(createCodeLine(-1, 3, -1, assignStmt->array[1]->array[1]->address, NULL, assignStmt->array[0]->address, 4, -1, 4), l1);
        return l1;
    }
    //<assignmentStmt> ---> ID <whichStmt> ... <lvalueARRStmt> ---> SQBO <index>SQBC ASSIGNOP <expression>SEMICOL
    else{
        addr * res = getPtrEntry(assignStmt->array[0]->val.var.id)->temp;
        assignStmt->array[0]->address = res;
        assignStmt->array[0]->addressType = 4;
        intermediateCodeList * l1 = generateCodeExpression(assignStmt->array[1]->array[2]);
        addr * a1 = malloc(sizeof(addr));
        if(assignStmt->array[1]->array[0]->state==55){  //<index> ---> NUM
            addr * a2 = malloc(sizeof(addr));
            a2->num = assignStmt->array[1]->array[0]->val.n;
            a1->temp = newTemp();
            addLineToList(createCodeLine(-1, 3, -1, a2, NULL, a1, 1, -1, 4), l1); //a1 = NUM
        }
        else{   //<index> ---> ID
            a1 = getPtrEntry(assignStmt->array[1]->array[0]->val.var.id)->temp;
            //addLineToList(createCodeLine(-1, 3, -1, a2, NULL, a1, 0, -1, 4), l1); //a1 = a2
        }
        return addLineToList(createCodeLine(-1, 11, -1, a1, assignStmt->array[1]->array[2]->address, res, 4, 4, 4), l1);  //id[index] = expression
    }
}

intermediateCodeList * generateCodeExpression(astNode * expr){
    if(expr->state==133){   //for <expression> ---> <U>
        addr * res = malloc(sizeof(addr));
        res->temp = newTemp();
        // printf("EnExpression temp value = t%d\n", res->temp);
        expr->address = res;
        expr->addressType = 4;
        if(expr->array[1]->state==140){//<U> ---> <unary_op><new_NT> ... <new_NT> ---> BO <arithmeticExpr>BC 
            intermediateCodeList * l1 = generateCodeArith(expr->array[1]);
            addLineToList(createCodeLine(-1, 2, expr->array[0]->state, expr->array[1]->address, NULL, res, expr->array[1]->addressType, -1, 4), l1);
            return l1;
        }
        else if(expr->array[1]->array[1]==NULL){//<U> ---> <unary_op><new_NT> ... <new_NT> ---> <var_id_num> ... <var_id_num> ---> NUM/RNUM
            addr * arg1 = malloc(sizeof(addr));                     //creating arg1, res
            if(expr->array[1]->state==55){
                arg1->num = expr->array[1]->val.n;
            }
            else{
                arg1->rnum = expr->array[1]->val.f;
            }
            return addLineToList(createCodeLine(-1, 2, expr->array[0]->state, arg1, NULL, res, expr->array[1]->state-54, -1, 4), createCodeList());
        }
        else if(expr->array[1]->array[1]->state==55){//<U> ---> <unary_op><new_NT> ... <new_NT> ---> <var_id_num> ... <var_id_num> ---> ID <whichId> ... <whichId> ---> SQBO <index>SQBC ... <index> ---> NUM
            //arr[4]
            intermediateCodeList * l1 = createCodeList();
            addr * arg1 = malloc(sizeof(addr));
            arg1->num = expr->array[1]->array[1]->val.n;
            // addr * arg2 = malloc(sizeof(addr));
            // arg2->id = getPtrEntry(expr->array[1]->array[0]->val.var.id);
            addr * arg2 = getPtrEntry(expr->array[1]->array[0]->val.var.id)->temp;
            addr * res = malloc(sizeof(addr));
            res->temp = newTemp();
            expr->address = res;
            expr->addressType = 4;
            addLineToList(createCodeLine(-1, 10, -1, arg2, arg1, res, 4, 1, 4), l1);  //t1 = arr[NUM]
            return l1;
        }
        else if(expr->array[1]->array[1]->state==54){//<U> ---> <unary_op><new_NT> ... <new_NT> ---> <var_id_num> ... <var_id_num> ---> ID <whichId> ... <whichId> ---> SQBO <index>SQBC ... <index> ---> ID
            intermediateCodeList * l1 = createCodeList();
            // addr * arg1 = malloc(sizeof(addr));
            // arg1->id = getPtrEntry(expr->array[1]->array[1]->val.var.id);
            addr * arg1 = getPtrEntry(expr->array[1]->array[1]->val.var.id)->temp;
            // addr * arg2 = malloc(sizeof(addr));
            // arg2->id = getPtrEntry(expr->array[1]->array[0]->val.var.id);
            addr * arg2 = getPtrEntry(expr->array[1]->array[0]->val.var.id)->temp;
            addr * res = malloc(sizeof(addr));
            res->temp = newTemp();
            expr->address = res;
            expr->addressType = 4;
            addLineToList(createCodeLine(-1, 10, -1, arg2, arg1, res, 4, 4, 4), l1);  //t1 = arr[ID]
            return l1;
        }   
    }
    else{//<expression> ---> <arithmeticOrBooleanExpr> ... <arithmeticOrBooleanExpr> ---> <AnyTerm><arithmeticOrBooleanExpr1>
        if(expr->array[1]==NULL){
            intermediateCodeList * l1 = generateCodeAnyTerm(expr->array[0]);
            expr->address = expr->array[0]->address;
            expr->addressType = expr->array[0]->addressType;
            // printf("EnExpression temp value = t%d\n", expr->address->temp);
            return l1;
        }
        else{
            intermediateCodeList * l1 = generateCodeAnyTerm(expr->array[0]);
            addr * a0 = malloc(sizeof(addr));
            a0->temp = newTemp();
            expr->address = a0;
            expr->addressType = 4;
            // printf("EnExpression temp value = t%d\n", a0->temp);
            addr * a1 = expr->array[0]->address;
            int a1Type = expr->array[0]->addressType;
            expr->trueLabel = newLabel();
            expr->falseLabel = newLabel();
            intermediateCodeList * l2 = generateCodeArithmeticOrBooleanExpr1(expr->array[1], expr->trueLabel, expr->falseLabel);
            addr * a2 = expr->array[1]->address;
            int a2Type = expr->array[1]->addressType;
            astNode * temp = expr;
            if(temp->array[1]->array[0]->state==45){  //AND
                expr->array[0]->trueLabel = newLabel();
                l2->head->label = expr->array[0]->trueLabel;
                expr->array[0]->falseLabel = expr->falseLabel;
                addr * a3 = malloc(sizeof(addr));
                a3->bool = 0;
                addr * a4 = malloc(sizeof(addr));
                int noOplabel = newLabel();
                a4->label = noOplabel;
                addr * a5 = malloc(sizeof(addr));
                a5->bool = 1;
                addr * lab = malloc(sizeof(addr));
                lab->label = expr->falseLabel;
                addLineToList(createCodeLine(expr->falseLabel, 3, -1, a3, NULL, a0, 3, -1, 4), l2); //false: expr.address = false;
                addLineToList(createCodeLine(-1, 4, -1, a4, NULL, NULL, 5, -1, -1), l2);            //goto noOp (skip next statement)
                addLineToList(createCodeLine(expr->trueLabel, 3, -1, a5, NULL, a0, 3, -1, 4), l2); //true: expr.address = true;
                addLineToList(createCodeLine(noOplabel, 0, -1, NULL, NULL, NULL, -1, -1, -1), l2); //noOP;
                addLineToList(createCodeLine(-1, 6, 8, expr->array[0]->address, a3, lab, 4, 3, 5), l1);  //if(anyTerm.address==FALSE) goto expr->falseLabel          
                return combineLists(l1, l2);
            }
            else{                                       //OR
                expr->array[0]->falseLabel = newLabel();
                l2->head->label = expr->array[0]->falseLabel;
                expr->array[0]->trueLabel = expr->trueLabel;
                addr * a3 = malloc(sizeof(addr));
                a3->bool = 0;
                addr * a4 = malloc(sizeof(addr));
                int noOplabel = newLabel();
                a4->label = noOplabel;
                addr * a5 = malloc(sizeof(addr));
                a5->bool = 1;
                addr * lab = malloc(sizeof(addr));
                lab->label = expr->trueLabel;
                addLineToList(createCodeLine(expr->falseLabel, 3, -1, a3, NULL, a0, 3, -1, 4), l2); //false: expr.address = false;
                addLineToList(createCodeLine(-1, 4, -1, a4, NULL, NULL, 5, -1, -1), l2);            //goto noOp (skip next statement)
                addLineToList(createCodeLine(expr->trueLabel, 3, -1, a5, NULL, a0, 3, -1, 4), l2); //true: expr.address = true;
                addLineToList(createCodeLine(noOplabel, 0, -1, NULL, NULL, NULL, -1, -1, -1), l2); //noOP;            
                addLineToList(createCodeLine(-1, 6, 8, expr->array[0]->address, a3, lab, 4, 3, 5), l1);  //if(anyTerm.address==FALSE) goto expr->falseLabel          
                return combineLists(l1, l2);
            }
        }
    }
}

intermediateCodeList * generateCodeArith(astNode* arith){
    //<arithmeticExpr> ---> <term><arithmeticExpr1> ... <arithmeticExpr1> ---> e
    if(arith->array[1]==NULL){
        intermediateCodeList * l1 = generateCodeTerm(arith->array[0]);
        arith->address = arith->array[0]->address;
        arith->addressType = arith->array[0]->addressType;
        return l1;
    }
    //<arithmeticExpr> ---> <term><arithmeticExpr1> ... <arithmeticExpr1> ---> <op1><term><arithmeticExpr1>
    else{
        intermediateCodeList * l1 = generateCodeTerm(arith->array[0]);
        intermediateCodeList * l2 = generateCodeArith1(arith->array[1]);
        addr * res = malloc(sizeof(addr));
        res->temp = newTemp();
        arith->address = res;
        arith->addressType = 4;
        combineLists(l1, l2);
        return addLineToList(createCodeLine(-1, 1, arith->array[1]->array[0]->state, arith->array[0]->address, arith->array[1]->address, arith->address, arith->array[0]->addressType, arith->array[1]->addressType, 4), l1);
    }
}

intermediateCodeList * generateCodeArith1(astNode* arith1){
    //<arithmeticExpr1> ---> <op1><term><arithmeticExpr1>
    if(arith1->array[2]==NULL){
        intermediateCodeList * l1 = generateCodeTerm(arith1->array[1]);
        arith1->address = arith1->array[1]->address;
        arith1->addressType = arith1->array[1]->addressType;
        return l1;
    }
    else{
        intermediateCodeList * l1 = generateCodeTerm(arith1->array[1]);
        intermediateCodeList * l2 = generateCodeArith1(arith1->array[2]);
        addr * res = malloc(sizeof(addr));
        res->temp = newTemp();
        arith1->address = res;
        arith1->addressType = 4;
        combineLists(l1, l2);
        return addLineToList(createCodeLine(-1, 1, arith1->array[2]->array[0]->state, arith1->array[1]->address, arith1->array[2]->array[1]->address, arith1->address, arith1->array[1]->addressType, arith1->array[2]->array[1]->addressType, 4), l1);
    }
}

intermediateCodeList * generateCodeTerm(astNode * term){
    //<term> ---> <factor><term1> ... <term1> ---> e
    if(term->array[1]==NULL){
        intermediateCodeList * l1 = generateCodeFactor(term->array[0]);
        term->address = term->array[0]->address;
        term->addressType = term->array[0]->addressType;
        return l1;
    }
    //<term> ---> <factor><term1> ... <term1> ---> <op2><factor><term1>
    else{
        intermediateCodeList * l1 = generateCodeFactor(term->array[0]);
        intermediateCodeList * l2 = generateCodeTerm1(term->array[1]);
        addr * res = malloc(sizeof(addr));
        res->temp = newTemp();
        term->address = res;
        term->addressType = 4;
        combineLists(l1, l2);
        return addLineToList(createCodeLine(-1, 1, term->array[1]->array[0]->state, term->array[0]->address, term->array[1]->address, term->address, term->array[0]->addressType, term->array[1]->addressType, 4), l1);
    }
}

intermediateCodeList * generateCodeTerm1(astNode* term1){
    //<term1> ---> <op2><factor><term1>
    if(term1->array[2]==NULL){
        intermediateCodeList * l1 = generateCodeFactor(term1->array[1]);
        term1->address = term1->array[1]->address;
        term1->addressType = term1->array[1]->addressType;
        return l1;
    }
    else{
        intermediateCodeList * l1 = generateCodeFactor(term1->array[1]);
        intermediateCodeList * l2 = generateCodeTerm1(term1->array[2]);
        addr * res = malloc(sizeof(addr));
        res->temp = newTemp();
        term1->address = res;
        term1->addressType = 4;
        combineLists(l1, l2);
        return addLineToList(createCodeLine(-1, 1, term1->array[2]->array[0]->state, term1->array[1]->address, term1->array[2]->array[1]->address, term1->address, term1->array[1]->addressType, term1->array[2]->array[1]->addressType, 4), l1);
    }
}

intermediateCodeList * generateCodeFactor(astNode * factor){
    // printf("Entered factor\n");
    if(factor->state == 136){
        //<factor> ---> BO <arithmeticOrBooleanExpr>BC
        return generateCodeExpression(factor); 
    }
    else if(factor->state == 119){
        if(factor->array[1]==NULL){
            //<factor> ---> <var_id_num> ... <var_id_num> ---> ID <whichId> ... <whichId> ---> e
            // addr * a1 = malloc(sizeof(addr));
            // a1->id = getPtrEntry(factor->array[0]->val.var.id);
            addr * a1 = getPtrEntry(factor->array[0]->val.var.id)->temp;            
            addr * a2 = malloc(sizeof(addr));
            a2->temp = newTemp();
            factor->address = a2;
            factor->addressType = 4;
            return addLineToList(createCodeLine(-1, 3, -1, a1, NULL, a2, 4, -1, 4), createCodeList());
        }
        else{
            //<factor> ---> <var_id_num> ... <var_id_num> ---> ID <whichId> ... <whichId> ---> SQBO <index>SQBC ... <index> ---> NUM/ID
            if(factor->array[1]->state==55){
                // addr * a1 = malloc(sizeof(addr));
                // a1->id = getPtrEntry(factor->array[0]->val.var.id);
                addr * a1 = getPtrEntry(factor->array[0]->val.var.id)->temp;            
                addr * a2 = malloc(sizeof(addr));
                a2->num = factor->array[1]->val.n;
                addr * a3 = malloc(sizeof(addr));
                a3->temp = newTemp();
                factor->address = a3;
                factor->addressType = 4;
                return addLineToList(createCodeLine(-1, 10, -1, a1, a2, a3, 4, 1, 4), createCodeList());
            }
            else{
                // addr * a1 = malloc(sizeof(addr));
                // a1->id = getPtrEntry(factor->array[0]->val.var.id);
                addr * a1 = getPtrEntry(factor->array[0]->val.var.id)->temp;            
                // addr * a2 = malloc(sizeof(addr));
                // a2->id = getPtrEntry(factor->array[1]->val.var.id);
                addr * a2 = getPtrEntry(factor->array[1]->val.var.id)->temp;            
                addr * a3 = malloc(sizeof(addr));
                a3->temp = newTemp();
                factor->address = a3;
                factor->addressType = 4;
                return addLineToList(createCodeLine(-1, 10, -1, a1, a2, a3, 4, 4, 4), createCodeList());
            }
        }

    }
    else if(factor->state == 55){
        //<factor> ---> <var_id_num> ... <var_id_num> ---> NUM
        // printf("Entered num part of factor\n");
        addr * a1 = malloc(sizeof(addr));
        a1->num = factor->val.n;
        addr * a2 = malloc(sizeof(addr));
        a2->temp = newTemp();
        factor->address = a2;
        factor->addressType = 4;
        return addLineToList(createCodeLine(-1, 3, -1, a1, NULL, a2, 1, -1, 4), createCodeList());
    }
    else if(factor->state == 56){
        //<factor> ---> <var_id_num> ... <var_id_num> ---> RNUM
        addr * a1 = malloc(sizeof(addr));
        a1->rnum = factor->val.f;
        addr * a2 = malloc(sizeof(addr));
        a2->temp = newTemp();
        factor->address = a2;
        factor->addressType = 4;
        return addLineToList(createCodeLine(-1, 3, -1, a1, NULL, a2, 2, -1, 4), createCodeList());
    }
}

intermediateCodeList * generateCodeAnyTerm(astNode* anyterm){    
    if(anyterm->state == 40 || anyterm->state == 41){
        //<AnyTerm> ---> TRUE/FALSE
        addr * temp = malloc(sizeof(addr));
        temp->bool = anyterm->val.n;
        addr * temp1 = malloc(sizeof(addr));
        temp1->temp = newTemp();
        anyterm->address = temp1;
        anyterm->addressType = 4;
        codeLine * line = createCodeLine(-1, 3, -1, temp, NULL, temp1, 3, -1, 4);   //t0 = TRUE/FALSE
        intermediateCodeList * l1 = createCodeList();
        return addLineToList(line, l1);
    }
    else if(anyterm->array[1]==NULL){
        //<AnyTerm> ---> <arithmeticExpr><AnyTerm1> ... <AnyTerm1> ---> e
        intermediateCodeList * l1 = generateCodeArith(anyterm->array[0]);
        anyterm->address = anyterm->array[0]->address;
        anyterm->addressType = anyterm->array[0]->addressType;
        return l1;
    }
    else{
        //<AnyTerm> ---> <arithmeticExpr><AnyTerm1> ... <AnyTerm1> ---> <relationalOp><arithmeticExpr>
        intermediateCodeList * l1 = generateCodeArith(anyterm->array[0]);
        intermediateCodeList * l2 = generateCodeArith(anyterm->array[1]->array[1]);
        combineLists(l1, l2);
        addr * temp = malloc(sizeof(addr));
        temp->temp = newTemp();
        anyterm->address = temp;
        anyterm->addressType = 4;
        int label2 = newLabel();
        addr * temp2 = malloc(sizeof(addr));
        temp2->label = label2;
        addr * fls = malloc(sizeof(addr));
        fls->bool = 0;
        addr * tru = malloc(sizeof(addr));
        tru->bool = 1;
        addr * noOp = malloc(sizeof(addr));
        noOp->label = newLabel();      
        addLineToList(createCodeLine(-1, 6, anyterm->array[1]->array[0]->state, anyterm->array[0]->address, anyterm->array[1]->array[1]->address, temp2, 4, 4, 5), l1); //if(arithExpr1 relop arithExpr2) goto L2
        addLineToList(createCodeLine(-1, 3, -1, fls, NULL, temp, 3, -1, 4), l1); //anyterm->address =FALSE
        addLineToList(createCodeLine(-1, 4, -1, noOp, NULL, NULL, 5, -1, -1), l1);  //goto noOp (skip the next instruction)
        addLineToList(createCodeLine(label2, 3, -1, tru, NULL, temp, 3, -1, 4), l1);    //l2: anyterm->address = TRUE
        addLineToList(createCodeLine(noOp->label, 0, -1, NULL, NULL, NULL, -1, -1, -1), l1);    //noOp: (no operation)
        return l1;
    }
}

intermediateCodeList * generateCodeArithmeticOrBooleanExpr1(astNode* abe1, int trueLabel, int falseLabel){
    if(abe1->array[2]==NULL){   //abe1 ---> logicalOp anyTerm
        intermediateCodeList * l1 = generateCodeAnyTerm(abe1->array[1]);
        abe1->address = abe1->array[1]->address;
        abe1->addressType = abe1->array[1]->addressType;
        abe1->falseLabel = falseLabel;
        abe1->trueLabel = trueLabel;
        addr * tru = malloc(sizeof(addr));
        tru->bool = 1; 
        addr * lab = malloc(sizeof(addr));
        lab->label = trueLabel; 
        addLineToList(createCodeLine(-1, 6, 8, abe1->array[1]->address, tru, lab, 4, 3, 5), l1);  //if(anyTerm.address==FALSE) goto expr->falseLabel          
        return l1;
    }
    else{
        intermediateCodeList * l1 = generateCodeAnyTerm(abe1->array[1]);    //l1 has code for anyterm
        addr * a0 = malloc(sizeof(addr));
        a0->temp = newTemp();
        abe1->address = a0;         //abe1 now has an address
        abe1->addressType = 4;
        addr * a1 = abe1->array[1]->address;        //a1 has anyterm address
        int a1Type = abe1->array[1]->addressType;
        abe1->trueLabel = newLabel();
        abe1->falseLabel = newLabel();
        intermediateCodeList * l2 = generateCodeArithmeticOrBooleanExpr1(abe1->array[2], abe1->trueLabel, abe1->falseLabel);
        addr * a2 = abe1->array[2]->address;    //a2 has abe12 address
        int a2Type = abe1->array[2]->addressType;
        if(abe1->array[2]->array[0]->state==45){  //AND
            abe1->array[1]->trueLabel = newLabel();
            l2->head->label = abe1->array[1]->trueLabel;
            abe1->array[1]->falseLabel = abe1->falseLabel;
            addr * a3 = malloc(sizeof(addr));
            a3->bool = 0;
            addr * a4 = malloc(sizeof(addr));
            int noOplabel = newLabel();
            a4->label = noOplabel;
            addr * a5 = malloc(sizeof(addr));
            a5->bool = 1;
            addr * lab = malloc(sizeof(addr));
            lab->label = abe1->falseLabel;
            addLineToList(createCodeLine(abe1->falseLabel, 3, -1, a3, NULL, a0, 3, -1, 4), l2); //false: expr.address = false;
            addLineToList(createCodeLine(-1, 4, -1, a4, NULL, NULL, 5, -1, -1), l2);            //goto noOp (skip next statement)
            addLineToList(createCodeLine(abe1->trueLabel, 3, -1, a5, NULL, a0, 3, -1, 4), l2); //true: expr.address = true;
            addLineToList(createCodeLine(noOplabel, 0, -1, NULL, NULL, NULL, -1, -1, -1), l2); //noOP;
            addLineToList(createCodeLine(-1, 6, 8, abe1->array[1]->address, a3, lab, 4, 3, 5), l1);  //if(anyTerm.address==FALSE) goto expr->falseLabel          
            return combineLists(l1, l2);
        }
        else{                                       //OR
            abe1->array[1]->falseLabel = newLabel();
            l2->head->label = abe1->array[1]->falseLabel;
            abe1->array[1]->trueLabel = abe1->trueLabel;
            addr * a3 = malloc(sizeof(addr));
            a3->bool = 0;
            addr * a4 = malloc(sizeof(addr));
            int noOplabel = newLabel();
            a4->label = noOplabel;
            addr * a5 = malloc(sizeof(addr));
            a5->bool = 1;
            addr * lab = malloc(sizeof(addr));
            lab->label = abe1->trueLabel;
            addLineToList(createCodeLine(abe1->falseLabel, 3, -1, a3, NULL, a0, 3, -1, 4), l2); //false: expr.address = false;
            addLineToList(createCodeLine(-1, 4, -1, a4, NULL, NULL, 5, -1, -1), l2);            //goto noOp (skip next statement)
            addLineToList(createCodeLine(abe1->trueLabel, 3, -1, a5, NULL, a0, 3, -1, 4), l2); //true: expr.address = true;
            addLineToList(createCodeLine(noOplabel, 0, -1, NULL, NULL, NULL, -1, -1, -1), l2); //noOP;            
            addLineToList(createCodeLine(-1, 6, 8, abe1->array[1]->address, a5, lab, 4, 3, 5), l1);  //if(anyTerm.address==TRUE) goto expr->trueLabel          
            return combineLists(l1, l2);
        }
    }
}

int newTemp(){
    tempNo ++;
    return tempNo-1;
}

int newLabel(){
    labelNo ++;
    return labelNo-1;
}

intermediateCodeList * initializeCodeList(){
    interCodeList = malloc(sizeof(intermediateCodeList));
    interCodeList->head = NULL;
    interCodeList->tail = NULL;
    return interCodeList;
}

intermediateCodeList * createCodeList(){
    intermediateCodeList * codeList = malloc(sizeof(intermediateCodeList));
    codeList->head = NULL;
    codeList->tail = NULL;
    return codeList;
}

codeLine * createCodeLine(int label, int rule, int op, addr* arg1, addr * arg2, addr * res, int arg1type, int arg2type, int restype){
    codeLine * code = malloc(sizeof(codeLine));
    code->arg1 = arg1;
    code->arg2 = arg2;
    code->res = res;
    code->arg1type = arg1type;
    code->arg2type = arg2type;
    code->restype = restype;
    code->op = op;
    code->rule = rule;
    code->label = label;
    return code;
}

intermediateCodeList * addLineToList(codeLine * line, intermediateCodeList * list){
    if(list->tail==NULL){
        list->tail = line;
        list->head = line;
    }
    else{
        list->tail->next = line;
        list->tail = line;
    }
    return list;
}

intermediateCodeList * combineLists(intermediateCodeList * list1, intermediateCodeList * list2){
    if(list1->tail!=NULL){
        if(list2->head!=NULL){
            // printf("n cond both are non empty\n");
            list1->tail->next =list2->head;
            list1->tail = list2->tail;
            return list1;
        }
        else{
            return list1;
        }
    }
    else{
        // printf("n cond list 1 is empty\n");
        list1->head = list2->head;
        list1->tail = list2->tail;
        return list1;
    }
}


