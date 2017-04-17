#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "compile.h"

struct Stack_t *Stack_Add_new ()
{
  struct Stack_Add *p = malloc (sizeof(*p));
  p->kind = STACK_ADD;
  return (struct Stack_t *)p;
}

struct Stack_t *Stack_Minus_new ()
{
  struct Stack_Minus *p = malloc (sizeof(*p));
  p->kind = STACK_MINUS;
  return (struct Stack_t *)p;
}

struct Stack_t *Stack_Times_new ()
{
  struct Stack_Times *p = malloc (sizeof(*p));
  p->kind = STACK_TIMES;
  return (struct Stack_t *)p;
}

struct Stack_t *Stack_Divide_new ()
{
  struct Stack_Divide *p = malloc (sizeof(*p));
  p->kind = STACK_DIVIDE;
  return (struct Stack_t *)p;
}

struct Stack_t *Stack_Push_new (int i)
{
  struct Stack_Push *p = malloc (sizeof(*p));
  p->kind = STACK_PUSH;
  p->i = i;
  return (struct Stack_t *)p;
}

// instruction list
struct List_t *List_new (struct Stack_t *instr, struct List_t *next)
{
  struct List_t *p = malloc (sizeof (*p));
  p->instr = instr;
  p->next = next;
  return p;
}

// "printer"
void List_reverse_print (struct List_t *list)
{
  if (list) {
    switch(list->instr->kind){
    case STACK_ADD:
        printf("add\n");
      break;
    case STACK_MINUS:
        printf("minus\n");
      break;
    case STACK_TIMES:
        printf("times\n");
      break;
    case STACK_DIVIDE:
        printf("divide\n");
      break;
    case STACK_PUSH:
        printf("push %d\n", ((struct Exp_Int*)list->instr)->n);
      break;
    default:
      break;
    }
    List_reverse_print(list->next);
  }
}

//////////////////////////////////////////////////
// a compiler from Sum to Stack
struct List_t *all = 0;

void emit (struct Stack_t *instr)
{
  all = List_new (instr, all);
}

void compile (struct Exp_t *exp)
{
  switch (exp->kind){
  case EXP_INT:
    emit (Stack_Push_new (((struct Exp_Int *)exp)->n));
    break;
  
  case EXP_ADD:
    compile(((struct Exp_Add *) exp)->left);
    compile(((struct Exp_Add *) exp)->right);
    emit (Stack_Add_new());
    break;
  case EXP_MINUS:
    compile(((struct Exp_Minus *) exp)->left);
    compile(((struct Exp_Minus *) exp)->right);
    emit (Stack_Minus_new());
    break;
  case EXP_TIMES:
    compile(((struct Exp_Times *) exp)->left);
    compile(((struct Exp_Times *) exp)->right);
    emit (Stack_Times_new());
    break;
  case EXP_DIVIDE:
    compile(((struct Exp_Divide *) exp)->left);
    compile(((struct Exp_Divide *) exp)->right);
    emit (Stack_Divide_new());
    break;
  default:
    break;
  }
}

struct List_t* getStack() {
    return all;
}
//////////////////////////////////////////////////
// program entry
#if 0
int main()
{
  printf("Compile starting\n");
  // build an expression tree:
  //            +
  //           / \
  //          +   4
  //         / \
  //        2   3
  struct Exp_t *exp = Exp_Sum_new (Exp_Sum_new(Exp_Int_new (2)
                                               , Exp_Int_new (3))
                                   , Exp_Int_new (4));
  // print out this tree:
  printf ("the expression is:\n");
  Exp_print (exp);
  // compile this tree to Stack machine instructions
  compile (exp);

  // print out the generated Stack instructons:
  printf("\nStack instructions: \n");
  List_reverse_print (all);
  
  printf("\nStack instructions with const folding: \n");
  List_reverse_print (folding);

  printf("\nCompile finished\n");
  return 0;
}
#endif
