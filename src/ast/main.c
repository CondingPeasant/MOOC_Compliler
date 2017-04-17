#include <stdio.h>
#include "ast.h"
#include "compile.h"

extern Exp_t tree;
void yyparse ();

int main (int argc, char **argv)
{
  yyparse();
  Exp_print (tree);

  compile (tree);
  struct List_t* stack = getStack();

  printf("\nStack instructions: \n");
  List_reverse_print (stack);
  printf("\nCompile finished\n");
  return 0;
}
