%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define YYDEBUG 1

int yylex(void);
void yyerror(char *s);
%}

%token Start
%token InBetween
%token Between
%token num
%token str
%token bol
%token Perhaps
%token Enter
%token Leave

%token identifier
%token constant

%start program

%%

program : Start cmpdstmt

type : num | str | bol

stmt : simplstmt | structstmt

cmpdstmt : '{' stmtlist '}'

stmtlist : stmt | stmt '\n' stmtlist

simplstmt : assignstmt | decl | iostmt

decl : normalDecl | arrayDecl | emptyDecl

normalDecl : identifier '<-' identifier | constant ':' type

arrayDecl : identifier '<-' '[' identifier | constant ']' ':' type

emptyDecl : identifier ':' type

assignstmt : identifier '<-' expression

iostmt : Enter '{' identifier '}' | Leave '{' identifier | constant '}'

structstmt : cmpdstmt | ifstmt | whileforstmt | arrayParsing

ifstmt : Perhaps '{' condition '}' '{' stmtlist '}'

whileforstmt : Between  '{' condition '}' '{'  stmtlist '}' | Between  '{' identifier | constant ',' identifier | constant ','  constant ',' identifier '}' '{'  stmtlist '}'

arrayParsing : InBetween '{' identifier | constant ',' identifier ',' identifier ':' type '}' '{' stmtlist '}'

expression : expression '+' term | expression '-' term | term

term : term '*' factor | term '/' factor | term '%' factor | factor 

factor : '{' expression '}' | identifier | constant

condition : expression relation expression 

relation : '<' | '<=' | '=' | '<>' | '>=' | '>'

%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

extern FILE *yyin;

int main(int argc, char **argv)
{
	if(argc>1) yyin :  fopen(argv[1],"r");
	if(argc>2 && !strcmp(argv[2],"-d")) yydebug = 1;
	if(!yyparse()) fprintf(stderr, "\tProgram is syntactically correct.\n");
}
