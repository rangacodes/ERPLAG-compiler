#include "ff.h"
#include "lexer.h"

char * nonTerminals[MAX_NONTER];
char * terminals[MAX_TER];
char * terminalNames[MAX_TER];


void parserInitialize(){
    char * temp1[MAX_NONTER] = {"program", "moduleDeclarations", "moduleDeclaration", "otherModules", "driverModule", "module", "ret", "input_plist",
                            "input_plist1", "output_plist", "output_plist1", "dataType", "range_arrays", "type", "moduleDef", "statements", "statement",
                            "ioStmt", "var", "var_id_num", "boolConstt", "whichId", "simpleStmt", "assignmentStmt", "whichStmt", "lvalueIDStmt",
                            "lvalueARRStmt", "index", "moduleReuseStmt", "optional", "idList", "idList1", "expression", "U", "new_NT", "unary_op",
                            "arithmeticOrBooleanExpr", "arithmeticOrBooleanExpr1", "AnyTerm", "AnyTerm1", "arithmeticExpr", "arithmeticExpr1", "term",
                            "term1", "factor", "op1", "op2", "logicalOp", "relationalOp", "declareStmt", "conditionalStmt", "caseStmts", "caseStmts1",
                            "value", "default", "iterativeStmt", "range"};
    for(int i=0;i<MAX_NONTER;i++)
    {
        nonTerminals[i]=temp1[i];
    }
    char * temp2[MAX_TER] = {"+", "-", "*", "/", "<", "<=", ">", ">=", "==", "!=", "<<", ">>", "<<<", ">>>", ":", "..", ";", ",", ":=", "[", "]", "(", ")", "**",
                        "integer", "real", "boolean", "of", "array", "start", "end", "declare", "module", "driver", "program", "get_value", "print", "use",
                        "with", "parameters", "true", "false", "takes", "input", "returns", "and", "or", "for", "in", "switch", "case", "break", "default",
                        "while", "ID", "NUM", "RNUM", "$", "e"};
    for(int i=0;i<MAX_TER;i++)
    {
        terminals[i]=temp2[i];
    }
    char * temp3[MAX_TER] = {"PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GT", "GE", "EQ", "NE", "DEF", "ENDDEF", "DRIVERDEF", "DRIVERENDDEF", "COLON",
                            "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "COMMENTMARK", "INTEGER", "REAL", "BOOLEAN", "OF",
                            "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", "USE", "WITH", "PARAMETERS", "TRUE",
                            "FALSE", "TAKES", "INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH", "CASE", "BREAK", "DEFAULT","WHILE", "ID", "NUM",
                            "RNUM", "$", "e"};
    for(int i=0;i<MAX_TER;i++)
    {
        terminalNames[i]=temp3[i];
    }
    initializeTokensList();
}

node * inOrderPrint(node* nd){
    if(nd->child!=NULL){
        inOrderPrint(nd->child);
    }
    if(nd->state<MAX_TER){
        printf("%d) %s\n", nd->lineNo, nd->lexeme);    
    }
    else{
        printf("%d) <%s>\n", nd->grule, nonTerminals[nd->state-100]);
    }
    node * temp = nd->child;
    if(temp!=NULL){
        temp = temp->sibling;
        while(temp!=NULL){
            inOrderPrint(temp);
            temp = temp->sibling;
        }
    }
}

node * inOrderPrint1(node* nd){
    if(nd->child!=NULL){
        inOrderPrint1(nd->child);
    }
    numnum++;
    if(nd->state<MAX_TER){
        //printf("%d) %s\n", nd->lineNo, nd->lexeme);    
    }
    else{
        //printf("%d) <%s>\n", nd->grule, nonTerminals[nd->state-100]);
    }
    node * temp = nd->child;
    if(temp!=NULL){
        temp = temp->sibling;
        while(temp!=NULL){
            inOrderPrint1(temp);
            temp = temp->sibling;
        }
    }
}

pTree * buildParseTree(){
    stack * S = createStack();              //stack is created and $, start symbol <program> are pushed
    push(S, 57);
    push(S, 100);
    pTree * T = createTree();
    if(T->current->state<MAX_TER){
        //printf("Current head is %s\n", terminalNames[T->current->state]);    
    }
    else{
        //printf("Current head is <%s>\n", nonTerminals[T->current->state-100]);    
    }
    tokenInfo token = getToken();
    addToken(token);
    int input = getState(token.name);
    char * lexeme = token.lexeme;
    int line = token.lineNo;
    //printf("Input taken: %s\n", terminalNames[input]);
    while(input!=60){                       //EOF
        if((S->top)->state>=100){        //if current is a non terminal
            int rule = parseTable[(S->top)->state-100][input];
            if(rule==-1){
                printf("Error here in parseTable: %d, %d\n", (S->top)->state, input);
                return NULL;
            }
            expand(rule, T);
            pop(S);
            if(grammar[rule][1]!=58){
                int i = 1;
                while(grammar[rule][i]!=-1){
                    i++;
                }
                for(i--;i>0; i--){
                    push(S, grammar[rule][i]);
                }
            }           
        }
        else if((S->top)->state<MAX_TER){
            T->current->lexeme = lexeme;
            T->current->lineNo = line;
            pop(S);
            token = getToken();
            addToken(token);
            input = getState(token.name);
            lexeme = token.lexeme;
            line = token.lineNo;
            //printf("Input taken: %s\n", terminalNames[input]);
            if((T->current)->sibling!=NULL){
                T->current = (T->current)->sibling;
            }
            else{
                node * temp = T->current;
                while((temp->parent)->sibling==NULL){
                    temp = temp->parent;
                    if(temp->parent==NULL){
                        T->current = T->root; 
                    }
                }
                T->current = (temp->parent)->sibling;
            }
            if(T->current->state<MAX_TER){
                //printf("Current head is %s\n", terminalNames[T->current->state]);    
            }
            else{
                //printf("Current head is <%s>\n", nonTerminals[T->current->state-100]);    
            }
        }
        else{
            printf("\n\n\n\n\nWHAT A WIERD ERROR\n\n\n\n\n");
        }
    }
    if((S->top)->state>=100){        //input is finished, removing A->e type rules
        //printf("Top of stack is %d\n", (S->top)->state);
        int rule = parseTable[(S->top)->state-100][57];
        if(rule==-1){
            printf("Error here in parseTable: %d, 57\n", (S->top)->state);
            return NULL;
        }
        expand(rule, T);
        pop(S);
        if(grammar[rule][1]!=58){
            int i = 1;
            while(grammar[rule][i]!=-1){
                i++;
            }
            for(i--;i>0; i--){
                push(S, grammar[rule][i]);
            }
        }           
    }
    if((S->top)->state==57 && (T->current)->state==100){
        return T;
    }
    return NULL;
}

pTree * createTree(){
    //printf("Tree created\n");
    pTree * T = malloc(sizeof(pTree));
    node * gsym = malloc(sizeof(node));
    gsym->state = 100;
    gsym->sibling = NULL;
    gsym->child = NULL;
    gsym->parent = NULL;
    T->root = gsym;
    T->current = gsym;
    return T;
}

node * newNode(int state){
    node * gsym = malloc(sizeof(node));
    gsym->state = state;
    gsym->sibling = NULL;
    gsym->child = NULL;
    gsym->parent = NULL;
    return gsym;
}

int expand(int rule, pTree * T){
    T->current->grule = rule;
    //printf("Expanding using rule %d: %s -> ", rule, nonTerminals[grammar[rule][0]-100]);
    int p = 1;
    while(grammar[rule][p]!=-1){
        if(grammar[rule][p]<MAX_TER){
            //printf(" %s", terminalNames[grammar[rule][p]]);
        }
        else{
            //printf(" %s", nonTerminals[grammar[rule][p]-100]);
        }
        p++;
    }
    //printf("\n");
    if(grammar[rule][0]!=(T->current)->state){
        printf("Wrong rule %d or non terminal %d to expand\n", rule, (T->current)->state);
        return -1;
    }
    if(grammar[rule][1]==58){           //for rule A->e
        if((T->current)->sibling!=NULL){
            T->current = (T->current)->sibling;
            if(T->current->state<MAX_TER){
                //printf("Current head is %s\n", terminalNames[T->current->state]);    
            }
            else{
                //printf("Current head is <%s>\n", nonTerminals[T->current->state-100]);    
            }
            return grammar[rule][0];
        }
        else{
            node * temp = T->current;
            while((temp->parent)->sibling==NULL){
                temp = temp->parent;
                if(temp->parent==NULL){
                    T->current = T->root;
                    if(T->current->state<MAX_TER){
                        //printf("Current head is %s\n", terminalNames[T->current->state]);    
                    }
                    else{
                        //printf("Current head is <%s>\n", nonTerminals[T->current->state-100]);    
                    } 
                    return 100;
                }
            }
            T->current = (temp->parent)->sibling;
            if(T->current->state<MAX_TER){
                //printf("Current head is %s\n", terminalNames[T->current->state]);    
            }
            else{
                //printf("Current head is <%s>\n", nonTerminals[T->current->state-100]);    
            }
            return grammar[rule][0];
        }
    }
    node * temp;
    int i = 1;
    temp = newNode(grammar[rule][1]);
    (T->current)->child = temp;
    (temp->parent) = T->current;
    T->current = temp;
    for(i = 2; grammar[rule][i]!=-1; i++){
        node * temp1 = newNode(grammar[rule][i]);
        temp->sibling = temp1;
        temp1->parent = temp->parent;
        temp = temp1;
    }
    if(T->current->state<MAX_TER){
        //printf("Current head is %s\n", terminalNames[T->current->state]);    
    }
    else{
        //printf("Current head is <%s>\n", nonTerminals[T->current->state-100]);    
    }
    return grammar[rule][i-1];
}

stack * createStack(){
    //printf("Stack created\n");
    stack * S = malloc(sizeof(stack));      //S is pointer to created stack
    S->top = NULL;
    S->bottom = NULL;
    return S;
}

snode * newSNode(int state){                //returns pointer to a new node
    snode * gsym = malloc(sizeof(snode));   
    gsym->state = state;
    gsym->above = NULL;
    gsym->below = NULL;
    return gsym;
}

void push(stack * S, int state){            //creates a node with "state" and pushes it onto the stack
    snode * node = newSNode(state);
    if(S->bottom==NULL){
        S->bottom = node;
        S->top = node;
    }
    else{
        node->below = S->top;
        (S->top)->above = node;
        S->top = node;
    }
    if(state>=100){
        //printf("Non terminal <%s> has been pushed onto the stack.\n", nonTerminals[state-100]);
    }
    else{
        //printf("Terminal %s has been pushed onto the stack.\n", terminalNames[state]);
    }
}

int pop(stack * S){
    if(S->bottom == NULL){                  //if stack is empty
        return -1;
    }
    int state = (S->top)->state;
    snode * temp = S->top;
    if(S->top==S->bottom){
        S->top = NULL;
        S->bottom = NULL;
    }
    else{
        snode * temp = S->top;
        S->top = (S->top)->below;
        (S->top)->above = NULL;
    }
    free(temp);
    if(state>=100){
        //printf("Non terminal %s has been popped from the stack.\n", nonTerminals[state-100]);
    }
    else{
        //printf("Terminal %s has been popped from the stack.\n", terminalNames[state]);
    }
    return state;
}

void populateGrammarArray(int grammar[150][25], FILE * gfile){
    int rule = 0;
    char line[150];
    char gsymbol[30];
        
    for(int i = 0; i<MAX_NONTER; i++){
        for(int k = 0; k<150; k++){
            line[k] = '\0';
        }
        for(int k = 0; k<30; k++){
            gsymbol[k] = '\0';
        }
        fgets(line, 150, gfile);
        int j = 1;                                                      //starts from after the <
        while(line[j]!='>'){
            gsymbol[j-1] = line[j];
            j++;
        }
        j++;
        grammar[rule][0] = getState(gsymbol);
        //fprintf(output, "\n%s  -> ", gsymbol);
        for(int k = 0; k<30; k++){
            gsymbol[k] = '\0';
        }
        while(line[j]!='>'){
            j++;
        }
        j++;
        while(line[j]==' '||line[j]=='\t'){
            j++;
        }
        int rhs = 1;
        while(line[j]!='\n'){
            if(line[j]=='<'){
                int k = 0;
                j++;
                while(line[j]!='>'){
                    gsymbol[k] = line[j];
                    k++;
                    j++;
                }
                j++;
                grammar[rule][rhs] = getState(gsymbol);
                //fprintf(output, "%s ", gsymbol);
                for(int k = 0; k<30; k++){
                    gsymbol[k] = '\0';
                }
                rhs++;
            }
            else if(line[j]!='|'){
                int k=0;
                while((line[j]>='A'&&line[j]<='Z')||line[j]=='$'||line[j]=='_'||line[j]=='e'){
                    gsymbol[k] = line[j];
                    k++;
                    j++;
                }
                j++;
                grammar[rule][rhs] = getState(gsymbol);
                //fprintf(output, "%s ", gsymbol);
                for(int k = 0; k<30; k++){
                    gsymbol[k] = '\0';
                }
                rhs++;
            }
            else if(line[j]=='|'){
                rule++;
                grammar[rule][0] = grammar[rule-1][0];
                rhs = 1;
                //fprintf(output, "\n\t\t-> ");
                for(int k = 0; k<30; k++){
                    gsymbol[k] = '\0';
                }
                j++;
            }
            while(line[j]==' '||line[j]=='\t'){
                j++;
            }            
        }
        rule++;
    }
}

int getState(char * gsym){
    if(gsym==NULL){
        return -1;
        printf("not null\n");
    }
    if(gsym[0]=='E'&&gsym[1]=='O'&&gsym[2]=='F'&&gsym[3]=='\0'){
        return 60;
    }
    for(int i=0; i<MAX_NONTER; i++){
        int j = 0;
        while(nonTerminals[i][j]!='\0' && gsym[j]!='\0' && nonTerminals[i][j]==gsym[j]){
            j++;
        }
        if(nonTerminals[i][j]=='\0' && gsym[j]=='\0'){
            return 100+i;
        }
    }
    for(int i=0; i<MAX_TER; i++){
        int j = 0;
        while(terminalNames[i][j]!='\0' && gsym[j]!='\0' && terminalNames[i][j]==gsym[j]){
            j++;
        }
        if(terminalNames[i][j]=='\0' && gsym[j]=='\0'){
            return i;
        }
    }
    return -1;
}

void populateFirstSet(){
    for(int i = 0; i<MAX_NONTER; i++){
        if(statusFirst[i]==-1){
            calculateFirst(i+100);
        }
    }
}

void populateFollowSet(){
    for(int i = 0; i<MAX_NONTER; i++){
        if(statusFollow[i]==-1){
            calculateFollow(i+100);
        }
    }
}

void calculateFirst(int state){             //initialize first to -1
    int rule = state-100;                      //for non terminals
    while(grammar[rule][0]!=state && rule<149){
        rule++;
    }
    while(grammar[rule][0]==state){            //for each production rule, do this:
        for(int i = 1; grammar[rule][i]!=-1; i++){
            if(grammar[rule][i]<MAX_TER){               //if immediate symbol on rhs is a terminal, add it to the first set and move to the next production rule
                first[state-100][grammar[rule][i]]=1;
                break; 
            }
            else{
                if(statusFirst[grammar[rule][i]-100] == -1){    //if first set of nt has not been calculated yet, calculate it
                    calculateFirst(grammar[rule][i]);
                }
                if(first[grammar[rule][i]-100][58] != 1){       //if nt does not have e in its first set, add its first set to the first set of "state"
                    for(int j = 0; j<MAX_TER; j++){
                        if(first[grammar[rule][i]-100][j]==1){
                            first[state-100][j] = 1;
                        }
                    }
                    break;                                  //move on to the next production rule
                }
                if(first[grammar[rule][i]-100][58] == 1){       //if nt has e in its first set, add its first set except e to the first set of "state"
                    for(int j = 0; j<MAX_TER-1; j++){       
                        if(first[grammar[rule][i]-100][j]==1){
                            first[state-100][j] = 1;
                        }
                    }
                    if(grammar[rule][i+1]==-1){
                        first[state-100][58] = 1;
                    }
                    continue;                               //then move on to the next symbol in the same production rule
                }
            }
        }
        rule++;
    }
    statusFirst[state-100] = 1;
}

void calculateFollow(int state){
    if(state==100){
        follow[0][57] = 1;
    }
    for(int i=0; i<149; i++){
        for(int j = 1; j<24; j++){
            if(grammar[i][j]==state){           //if grammar symbol with state "state" is present in the rhs, do the following:
                for(int k = j+1; k<24; k++){
                    if(grammar[i][k]==-1){
                        if(grammar[i][j]==grammar[i][0]){
                            break;
                        }
                        if(statusFollow[grammar[i][0]-100]==-1){    //copy follow lhs into follow of symbol, and proceed to search for the next occurence of the symbol in rhs
                            calculateFollow(grammar[i][0]);
                        }
                        for(int l = 0; l<MAX_TER-1; l++){
                            if(follow[grammar[i][0]-100][l]==1){
                                follow[grammar[i][j]-100][l] = 1;
                            }
                        }
                        break;
                    }
                    else if(grammar[i][k]<MAX_TER){
                        follow[grammar[i][j]-100][grammar[i][k]] = 1;
                        break;
                    }
                    else{
                        for(int l = 0; l<MAX_TER-1; l++){
                            if(first[grammar[i][k]-100][l]==1){
                                follow[grammar[i][j]-100][l] = 1;
                            }
                        }
                        if(first[grammar[i][k]-100][58]!=1){
                            break;
                        }
                    }
                }
                
            }
        }
        if(grammar[i][0]==-1){
            break;
        }
    }
    statusFollow[state-100] = 1;
}

void populateParseTable(){
    for(int i = 0; i<149; i++){
        int j = 1;
        if(grammar[i][1]==58){
            for(int k=0; k<MAX_TER-1; k++){
                if(follow[grammar[i][0]-100][k]==1){
                    parseTable[grammar[i][0]-100][k] = i;
                }
            }    
        }
        while(grammar[i][j]!=-1){
            if(grammar[i][j]<MAX_TER-1){
                parseTable[grammar[i][0]-100][grammar[i][j]] = i;
                break;
            }
            else{
                for(int k = 0; k<MAX_TER-1; k++){
                    if(first[grammar[i][j]-100][k]==1){
                        parseTable[grammar[i][0]-100][k] = i;
                    }
                }
                if(first[grammar[i][j]-100][58]!=1){
                    break;
                }
            }
            j++;
        }
        if(grammar[i][j]==-1 && grammar[i][0]!=-1){                  //this implies while loop is exited without a break => with e in all first sets 
            for(int k=0; k<MAX_TER-1; k++){
                if(follow[grammar[i][0]-100][k]==1){
                    parseTable[grammar[i][0]-100][k] = i;
                }
            }
        }
    }
}

void grammarArrayInitialization(){			//initializes values to -1       
    for(int i=0; i<150; i++){
        for(int j=0; j<25; j++){
            grammar[i][j]=-1;
        }
    }
}

void initializeFirst(){
    for(int i = 0; i<MAX_NONTER; i++){
        for(int j = 0; j<MAX_TER; j++){
            first[i][j] = -1;
        }
        statusFirst[i] = -1;
    }
}

void initializeFollow(){
    for(int i = 0; i<MAX_NONTER; i++){
        for(int j = 0; j<MAX_TER; j++){
            follow[i][j] = -1;
        }
        statusFollow[i] = -1;
    }
}

void initializeParseTable(){
    for(int i = 0; i<MAX_NONTER; i++){
        for(int j = 0; j<MAX_TER; j++){
            parseTable[i][j] = -1;
        }
    }
}

void initializeTokensList(){
    tokensList = malloc(sizeof(tkList));
    tokensList->head = NULL;
    tokensList->tail = NULL;
}

void addToken(tokenInfo  ti){
    tkf * token = malloc(sizeof(tkf));
    token->tk = malloc(sizeof(tokenInfo));
    token->tk->name = ti.name;
    token->tk->state = ti.state;
    token->tk->lineNo = ti.lineNo;
    token->tk->lexeme = ti.lexeme;
    if(tokensList->tail == NULL){
        tokensList->head = token;
        tokensList->tail = token;
    }
    else{
        tokensList->tail->next = token;
        tokensList->tail = token;
    }
    token->next = NULL;
}

void printTkList(){
    tkf * token = tokensList->head;
    int i = 0;
    printf("Printing list of tokens generated by lexer\nS.no\tLine No.\t Lexeme\t Token Name\n");
    while(token!=NULL){
        printf("%d)\t\t%d\t\t%s\t\t%s\n", i, token->tk->lineNo, token->tk->lexeme, token->tk->name);
        i++;
        token = token->next;
    }
}
