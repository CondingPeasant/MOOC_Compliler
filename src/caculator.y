%{
#include <stdio.h>
#include <stdlib.h>
int isOperator(char c);
int isDigit(char c);
int yylex();
void yyerror(char* s);
%}

%token ADD
%token MINUS
%token MULTIPLY
%token DIVIDE
%token ZERO
%token NONZERO
%token LPAREN
%token RPAREN
%token LF
/*
When two tokens declared in diﬀerent precedence declarations associate,
the one declared later has the higher precedence
*/
%left ADD MINUS
%left MULTIPLY DIVIDE

%%

lines: line
     | line lines
;

line: exp LF;

exp: num
   | exp ADD exp
   | exp MINUS exp
   | exp MULTIPLY exp
   | exp DIVIDE exp
   | LPAREN exp RPAREN
;

num: ZERO
   | NONZERO digits
;

digit: ZERO
     | NONZERO
;

digits: digit
      | digit digits
;
%%

int yylex() {
    char c = getchar();
    int ret = 0;

    switch(c) {
    case EOF:
        ret = 0;
        break;
    case '+':
        ret = ADD;
        break;
    case '-':
        ret = MINUS;
        break;
    case '*':
        ret = MULTIPLY;
        break;
    case '/':
        ret = DIVIDE;
        break;
    case '0':
        ret = ZERO;
        break;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        ret = NONZERO;
        break;
    case '\n':
        ret = LF;
        break;
    case '(':
        ret = LPAREN;
        break;
    case ')':
        ret = RPAREN;
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
    yyparse();
    return 0;
}
