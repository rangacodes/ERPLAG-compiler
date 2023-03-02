#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
//#include "intermediate_code_generator.h"

#define MAX_NONTER 57
#define MAX_TER 59

typedef struct tokenForList{
    tokenInfo * tk;
    struct tokenForList * next;
}tkf;

typedef union indx{
    int N;
    char * id;
}indx;

typedef struct tokenList{
    tkf * head;
    tkf * tail;
}tkList;

typedef struct arrayInfo{
    int isDynamic;
    int isStartID;
    int isEndID;
    indx start;
    indx end;
}arrayInfo;

typedef union address addr;

typedef struct symbolTableEntry{
    char * lexeme;
    int type;           //24 for integer, 25 for real, 26 for boolean
    int offset;
    int isArr;
    int width;
    addr * temp;
    int lineNo;
    int isOutput;       //1 if it is an output variable, 2 if it is an output variable and has been given a value in the function
    arrayInfo indices;
    struct symbolTableEntry * next;
}entry;

typedef union address{
    entry * id;     //0
    int num;        //1
    float rnum;     //2
    int bool;       //3
    int temp;       //4
    int label;      //5
    int ret;        //6
}addr;

typedef struct tempType{
    struct tempType * next;
    int temp;
    int type;
    entry * e;
}tempType;

typedef struct temptypeList{
    tempType * head;
    tempType * tail;
}tempTypeList;

/*
rule    semantic    
0       NOP         
1       x(res) = y(arg1) op z(arg2)  
2       x(res) = op y(arg1)    
3       x(res) = y(arg1)
4       goto L(arg1)
5       if x(arg1) goto L(arg2)
6       if x(arg1) relop y(arg2) goto L(res)
7       param x(arg1)
8       call modDefLab(arg1) retLab(res) noInput(arg2)
9       return ti(res), rj(arg1)
10      x(res) = a(arg1)[i(arg2)]
11      b(res)[i(arg1)] = y(arg2)
12      goback (from callee to caller)
13      function (start of function definition)
14      input ti(arg1)    
15      getValue(id(arg1))
16      printf(x(arg1))
17      dynamicArray A(res)[a(arg1)..b(arg2)]
*/

typedef struct interCode{
    int label;      //label = -1 =>label is invalid
    int rule;
    int op;
    int arg1type;   //-1 means invalid
    int arg2type;
    int restype;
    addr * arg1;
    addr * arg2;
    addr * res;
    struct interCode * next;
}codeLine;

typedef struct intermediateCodeList{
    codeLine * head;
    codeLine * tail;
}intermediateCodeList;

typedef struct symbolTable{
    entry * head;
    entry * tail;
    char * scope;
    int level;
    int scopeStart;
    int scopeEnd;
    int prevChildVisited;
    int block;          //one of <inputList>, <statements>, 
    int offset;
    struct symbolTable * child;
    struct symbolTable * sibling;
    struct symbolTable * parent;
}sTable;

typedef struct symbolTableTree{
    sTable * root;
    sTable * current;
}symTableTree;

typedef struct Function{
    char * moduleName;
    sTable * inputList;
    sTable * outputList;
    int size;
    int label;
    int retLabel;
    struct Function * next;
}function;

typedef struct functionsList{
    function * head;
    function * current;    
}fList;

struct variable{
    char * id;
    entry * ptr;
};

typedef union Record{
    int n;          //holds integer or boolean value 0 or 1
    float f;
    struct variable var;
}record;

typedef struct astnode{
    int state;
    int nodeNumber;
    int lineNo;
    int trueLabel;
    int falseLabel;
    addr * address;
    int addressType;
    struct astnode * array[5];         //maximum 5 children
    struct astnode * sibling;
    struct astnode * parent;
    record val;
}astNode;

typedef struct AStree{
    astNode * root;
}AST;

typedef struct moduleUseStatement{
    astNode * statement;
    sTable * table;
    struct moduleUseStatement * next;
}modUseStmt;

typedef struct modUseStmtsList{
    modUseStmt * head;
    modUseStmt * tail;
}modUseStmtsList;


typedef struct pTreeNode{
    int state;
    struct pTreeNode * sibling;
    struct pTreeNode * parent;
    struct pTreeNode * child;
    astNode * node;
    char * lexeme;
    int lineNo;
    int grule;
}node;

typedef struct PTree{
    node * root;
    node * current;
}pTree;

typedef struct stackNode{
    int state;
    struct stackNode * above;
    struct stackNode * below;
}snode;

typedef struct Stack{
    snode * top;
    snode * bottom;
}stack;

int grammar[150][25];
int first[MAX_NONTER][MAX_TER];
int statusFirst[MAX_NONTER];
int follow[MAX_NONTER][MAX_TER];
int statusFollow[MAX_NONTER];
int parseTable[MAX_NONTER][MAX_TER];
pTree * parseTree;
AST * syntaxTree;
symTableTree * symTree;
int tableCount;
int ASTNodeCount;
int numnum;
fList * functions;
modUseStmtsList * modList;
int moduleOffset;
int driverStart;
int driverEnd;
char * moduleName;

intermediateCodeList * interCodeList;
int tempNo;
int labelNo;
int retNo;
tempTypeList * ttlist;
tkList * tokensList;
FILE * errorsFile;

extern char * nonTerminals[MAX_NONTER];

extern char * terminals[MAX_TER]; //$, e should be the last two terminals in this set(for calculation of first, follow set to work properly)
                            //if index for $, e is different from 57, 58, changes should be made in a lot of places
extern char * terminalNames[MAX_TER];

void constructIntermediateCode();
intermediateCodeList * initializeCodeList();
intermediateCodeList * createCodeList();
codeLine * createCodeLine(int label, int rule, int op, addr* arg1, addr * arg2, addr * res, int arg1type, int arg2type, int restype);
intermediateCodeList * addLineToList(codeLine * line, intermediateCodeList * list);
intermediateCodeList * combineLists(intermediateCodeList * list1, intermediateCodeList * list2);
intermediateCodeList * generateCodeExpression(astNode * expr);
intermediateCodeList * generateCodeArith(astNode* expr);
intermediateCodeList * generateCodeArith1(astNode* expr);
intermediateCodeList * generateCodeAnyTerm(astNode* expr);
intermediateCodeList * generateCodeArithmeticOrBooleanExpr1(astNode* abe1, int trueLabel, int falseLabel);
intermediateCodeList * generateCodeTerm(astNode * term);
intermediateCodeList * generateCodeTerm1(astNode* term1);
intermediateCodeList * generateCodeFactor(astNode * factor);
intermediateCodeList * generateCodeAssignment(astNode * assignStmt);
intermediateCodeList * generateCodeFor(astNode * forStmt);
intermediateCodeList * generateCodeWhile(astNode * whileStmt);
intermediateCodeList * generateCodeSwitch(astNode * switchStmt);
intermediateCodeList * generateCodeIO(astNode * ioStmt);
intermediateCodeList * generateCodeStatements(astNode * stmts);
intermediateCodeList * generateCodeModuleReuse(astNode * stmts);
void printList(intermediateCodeList * list);
void printAddress(addr * a1, int type);
void initializeTtlist();
void addtt(int temp, int type, entry * e);
int getTypeOfTemp(int temp);
void printTTList();

void sectiontext_print(FILE * fp);
void driver_return_print(FILE * fp);
void sectiondata_print(FILE * fp);
void assembly_code_generator(intermediateCodeList * intList, char * outputfile);
void rule0_print(codeLine * cd, FILE * fp);
void rule1_print(codeLine * cd, FILE * fp);
void rule2_print(codeLine * cd, FILE * fp);
void rule3_print(codeLine * cd, FILE * fp);
void rule4_print(codeLine * cd, FILE * fp);
void rule5_print(codeLine * cd, FILE * fp);
void rule6_print(codeLine * cd, FILE * fp);
void rule8_print(codeLine * cd, FILE * fp);
void rule9_print(codeLine * cd, FILE * fp);
void rule11_print(codeLine * cd, FILE * fp);
void rule12_print(codeLine * cd, FILE * fp);
void rule10_print(codeLine * cd, FILE * fp);
void rule11_print(codeLine * cd, FILE * fp);
void rule15_print(codeLine * cd, FILE * fp);
void rule16_print(codeLine * cd, FILE * fp);
int GetArraySize(int temp);
int getStartIndex(int temp);
int IsTempArray(int temp);
int getEndIndex(int temp);

void initializeTokensList();
void addToken(tokenInfo ti);
void printTkList();

int newTemp();
int newLabel();
int newRet();


int checkModList();
function * searchFunction(char * name);
node * inOrderPrint1(node* nd);
modUseStmtsList * makeModList();
void addStmtModList(astNode * stmt, sTable * table);
function * addFunctionEntry(char * name);
int checkType(astNode * assignmentStmt);
int getTypeExpr(astNode * node);
int getTypeU(astNode * node);
int getTypeNewNT(astNode * node);
int getTypeArithOrBoolExpr(astNode * node);
int getTypeArithOrBoolExpr1(astNode * node);
int getTypeAnyTerm(astNode * node);
int getTypeAnyTerm1(astNode * node);
int getTypeArithExpr(astNode * node);
int getTypeArithExpr1(astNode * node);
int getTypeTerm(astNode * node);
int getTypeTerm1(astNode * node);
int getTypeFactor(astNode * node);
int getTypeVarIdNum(astNode * node);
int getTypeVar(astNode * node);
int getTypeBoolConstt(astNode * node);
int getTypeTerminal(astNode * node);
int presentInTable(char * name, sTable * table);
entry * presentInTableOutput(char * name, sTable * table);
entry * presentInTableEntry(char * name, sTable * table);
entry * getPtrEntry(char * name);
int findVar(char * iterId, sTable * table);
int presentInTableLine(char * name, sTable * table);
void printSymTableTree(sTable * table);
entry * newEntry();
symTableTree * createSymbolTableTree(AST * syntaxTree);
void addEntriesToTable(function * fun, astNode * statements);
sTable * addChildTable();
entry * addEntry(int lineNo, sTable * table, char * lexeme, int type, int isArr);
void connectChildrenToParent(astNode * node);
void parserInitialize();
int getState(char * gsym);  //matches the grammar symbol with a terminal or non terminal from the list and returns appropriate state number
void grammarArrayInitialization();
void populateGrammarArray(int grammar[150][25], FILE * gfile);
void populateFirstSet();
void calculateFirst(int state);
void initializeFirst();
void populateFollowSet();
void initializeFollow();
void calculateFollow(int state);
void initializeParseTable();
void populateParseTable();
int pop(stack * S);
void push(stack * S, int state);
snode * newSNode(int state);
stack * createStack();
pTree * buildParseTree();
pTree * createTree();
node * newNode(int state);
int expand(int rule, pTree * T);
void printTree(pTree * T);
node * inOrderPrint(node* node);
astNode * makeNodeAST(int state, astNode * arr[5]);
AST * newAST();
astNode * getPtr(node * pnode);
astNode * makeLeafAST(int lineNo, int state, int valueType, record * value);
int buildAST();
astNode * preOrderPrint(astNode * nd);
int getNum(char* lexeme);
void printARSizes();
int calculateARSize(sTable * table, function * fun);
void printArrays(sTable * table);

/*
0: PLUS
1: MINUS
2: MUL
3: DIV
4: LT
5: LE
6: GT
7: GE
8: EQ
9: NE
10: DEF
11: ENDDEF
12: DRIVERDEF
13: DRIVERENDDEF
14: COLON
15: RANGEOP
16: SEMICOL
17: COMMA
18: ASSIGNOP
19: SQBO
20: SQBC
21: BO
22: BC
23: COMMENTMARK
24: INTEGER
25: REAL
26: BOOLEAN
27: OF
28: ARRAY
29: START
30: END
31: DECLARE
32: MODULE
33: DRIVER
34: PROGRAM
35: GET_VALUE
36: PRINT
37: USE
38: WITH
39: PARAMETERS
40: TRUE
41: FALSE
42: TAKES
43: INPUT
44: RETURNS
45: AND
46: OR
47: FOR
48: IN
49: SWITCH
50: CASE
51: BREAK
52: DEFAULT
53: WHILE
54: ID
55: NUM
56: RNUM
57: $
58: e
*/