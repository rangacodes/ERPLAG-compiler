#ifndef LEXERDEF_H
#define LEXERDEF_H
#include <stdlib.h>
#include <stdio.h>

typedef struct ptr{
    int index;
    int arrNum;
} ptr;

typedef struct tokeninfo{
    char * name;
    char * lexeme;
    int state;
    int lineNo;
}tokenInfo;

extern char buff[2][20];
extern int lineNo;

extern char* names[35];
extern int states[35];

extern char * keywordLexemes[34];
extern char * keywordTokens[34];

extern ptr bp, fp;

extern FILE * file;                                    //current file pointer
extern FILE * start;                                   //starting file pointer

extern char *error;
extern char *eof;
#endif