//BATCH NUMBER 49
//RANGA SRIRAM 2017A7PS0047P
//RAYAPATI YASWANTH KUMAR 2017A7PS0038P
//KATTA SIVA KUMAR 2017A7A7PS0078P

#ifndef LEXER_H
#define LEXER_H
#include "lexerDef.h"

#define ANSI_COLOR_RED "\x1b[31m"

#define ANSI_COLOR_GREEN "\x1b[32m"

#define ANSI_COLOR_YELLOW "\x1b[33m"

#define ANSI_COLOR_BLUE "\x1b[34m"

#define ANSI_COLOR_MAGENTA "\x1b[35m"

#define ANSI_COLOR_CYAN "\x1b[36m"

#define ANSI_COLOR_RESET "\x1b[0m"

tokenInfo getToken();
int fillBuffer(int n);
int increment();
int retract();
int isKeyword(char * token);
void lexerInitialize();
void uncommentedPrint(FILE * ptr);
void uncommentedSourceCode(FILE * input, FILE * output);
#endif