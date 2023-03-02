#include "ff.h"
#include "lexer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]){

    clock_t    start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    start_time = clock();
    FILE * gfile = fopen("Reconstructed_grammar.txt", "r");
    FILE * output = fopen("rules.txt", "w");
    FILE * output1 = fopen("first.txt", "w");
    FILE * output2 = fopen("follow.txt", "w");
    labelNo = 0;
    tempNo = 0;
    retNo = 0;
    if(output==NULL){
        printf("Error in opening file Reconstructed_grammar.txt\n");
    }
    parserInitialize();
    grammarArrayInitialization(grammar);
    populateGrammarArray(grammar, gfile);
    for(int i = 0; i<150; i++){
        if(grammar[i][0]==-1){
            break;
        }
        //fprintf(output, "%d: <%s> ---> ", i, nonTerminals[grammar[i][0]-100]);
        for(int j=1; j<25; j++){
            if(grammar[i][j]==-1){
                break;
            }
            if(grammar[i][j]<MAX_TER){
                //fprintf(output, "%s ", terminalNames[grammar[i][j]]);
            }
            else{
                //fprintf(output, "<%s>", nonTerminals[grammar[i][j]-100]);
            }
        }
        //fprintf(output, "\n");
    }
    initializeFirst();
    populateFirstSet();
    for(int i = 0; i<MAX_NONTER; i++){
        //fprintf(output1, "First set of <%s>: ", nonTerminals[i]);
        for(int j = 0; j<MAX_TER; j++){
            if(first[i][j]==1){
                //fprintf(output1, "%s  ", terminalNames[j]);
            }
        }
        //fprintf(output1, "\n");
    }
    initializeFollow();
    //fprintf(output, "Populating follow sets...\n");
    populateFollowSet();
    //fprintf(output, "Done!\n");
    for(int i =0; i<MAX_NONTER; i++){
        //fprintf(output2, "Follow set of <%s> is: ", nonTerminals[i]);
        for(int j = 0; j<MAX_TER; j++){
            if(follow[i][j]==1){
                //fprintf(output2, "%s ", terminalNames[j]);
            }
        }
        //fprintf(output2, "\n"); 
    }
    initializeParseTable();
    //fprintf(output, "Populating parse table...\n");
    populateParseTable();
    //fprintf(output, "Done!\n");
    FILE * input = fopen(argv[1], "r");
    FILE * start = fopen("uc.txt", "w+");
    errorsFile = fopen("errors.txt", "w+");
    uncommentedSourceCode(input, start);
    for(int i = 0; i<MAX_NONTER; i++){
        for(int j = 0; j<MAX_TER; j++){
            //fprintf(output, "%d\t", parseTable[i][j]);
        }
        //fprintf(output, "\n\n");
    }
    lexerInitialize();
    bp.index = 0;
    bp.arrNum = 0;
    fp.index = 0;
    fp.arrNum = 0;
    lineNo = 1;
    fseek(start, 0, SEEK_SET); 
    file = start;
    for(int i = 0; i<20; i++){                  //filling buffer only row 0
        buff[0][i] = fgetc(file);
    }
    parseTree = buildParseTree();
    if(parseTree!=NULL){
        //printf(ANSI_COLOR_BLUE "\n\n\n!!!Parse tree built successfully!!!\n\n\n" ANSI_COLOR_RESET);
    }
    else{
        printf("\n\n\nCan't build parse tree\n\n\n");
    }
    numnum = 0;
    inOrderPrint1(parseTree->root);
    //printf("Number of nodes in parse tree = %d\n", numnum);
    initializeTtlist();
    buildAST();
    // syntaxTree = newAST();
    // record val;
    // val.n = 20;
    // astNode * leaf = makeLeafAST(55, &val);
    // syntaxTree->root->array[0] = leaf;
    // printf("\n\n\nState of leaf: %d\nValue in leaf: %d\n", syntaxTree->root->array[0]->state, syntaxTree->root->array[0]->val.n);
    //printf("\n\n\nTime for the test!\n\n\n");
    createSymbolTableTree(syntaxTree);
    //printf("Done creating symbol tables\n");
    modUseStmt * temp = modList->head;
    while(temp!=NULL){
        //printf("%s function\n", temp->statement->array[1]->val.var.id);
        temp = temp->next;
    }
    checkModList();
    
    long int parseSpace = numnum*sizeof(node*);
    long int ASTSpace = ASTNodeCount*sizeof(astNode*);
    float compression = (parseSpace-ASTSpace)*100/parseSpace;
    fseek(errorsFile, 0, SEEK_SET);
    char c = fgetc(errorsFile);
    int noErr = 0;
    if(c==EOF){
        printf(ANSI_COLOR_CYAN "\n\nCompilation Succesful\n\n" ANSI_COLOR_RESET);
    }
    else{
        while(c!=EOF){
            c = fgetc(errorsFile);
            if(c=='\n'){
                noErr++;
            }
        }
        if(noErr<=5){
            printf("LEVEL 1: Symbol table/ AST/ Semantic Rules/ Type Checking(With exceptions)/Code Generation \n");
        }
        else if(noErr<=10){
            printf("LEVEL 2: Symbol table/ AST/ Semantic Rules/ Type Checking(With exceptions)/Code Generation \n");
        }
        else if(noErr<=15){
            printf("LEVEL 3: Symbol table/ AST/ Semantic Rules/ Type Checking(With exceptions)/Code Generation \n");
        }
        else{
            printf("LEVEL 4: Symbol table/ AST/ Semantic Rules/ Type Checking(With exceptions)/Code Generation \n");
        }
    }
    c = '%';
    end_time = clock();

    total_CPU_time  =  (double) (end_time - start_time);

    total_CPU_time_in_seconds =   total_CPU_time / 1000000;
    while(1){
        printf("\n\nSelect an option for the defined task:\n"
        "0. To exit from the loop \n"
        "1. Lexer: For printing the token list generated by the lexer \n"
        "2. Parser: For parsing to verify the syntactic correctness of the input source code and to produce parse tree \n"
        "3. AST: For printing the Abstract Syntax Tree\n"
        "4. Memory: For displaying the amount of allocated memory and number of nodes to each of parse tree and abstract syntax tree\n"
        "5. Symbol Table: For printing the Symbol Table\n"
        "6. Activation record size: For printing the total memory requirement for each function\n"
        "7. Static and dynamic arrays: For printing the type expressions and width of array variables\n"
        "8. Errors reporting and total compiling time\n"
        "9. Code generation: For producing assembly code\n\n");
        int option;
        scanf("%d", &option);
        printf("\n\n");
        if(option==0){
            break;
        }
        switch(option){
            case 0:
                break;
            case 1:
                printTkList();
                break;
            case 2:
                printf("Printing in in-order fashion:\n");
                inOrderPrint(parseTree->root);
                break;
            case 3:
                printf("Printing AST\n");
                preOrderPrint(syntaxTree->root);
                break;
            case 4:
                printf("\nNumber of parse tree nodes = %d\n\tSpace allocated = %ld bytes\n\n"
                        "Number of AST nodes = %d\n\tSpace allocated = %ld bytes\n\n"
                        "Compression percentage = %f%c\n\n", numnum, parseSpace, ASTNodeCount, ASTSpace, compression, c);
                break;
            case 5:
                printSymTableTree(symTree->root);
                break;
            case 6:
                printARSizes();
                break;
            case 7:
                printArrays(symTree->root);
                break;
            case 8:
                fseek(errorsFile, 0, SEEK_SET);
                char c = fgetc(errorsFile);
                while(c!=EOF){
                    printf("%c", c);
                    c = fgetc(errorsFile);
                }
                printf("\n\nTOTAL CPU TIME = %lf\nTOTAL CPU TIME IN SECONDS = %lf", total_CPU_time, total_CPU_time_in_seconds);
                break;
            case 9:
                constructIntermediateCode();
                assembly_code_generator(interCodeList,argv[2]);
            default:
                break;                
        }
    }
}