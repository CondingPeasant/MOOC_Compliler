#include <stdio.h>
#include <stdlib.h>

#define TODO()                                  \
  do{                                                           \
    printf ("\nAdd your code here: file \"%s\", line %d\n",     \
            __FILE__, __LINE__);                                \
  }while(0)

///////////////////////////////////////////////
// Data structures for the Sum language.

enum Exp_Kind_t {EXP_INT, EXP_SUM};
struct Exp_t
{
  enum Exp_Kind_t kind;
};

struct Exp_Int
{
  enum Exp_Kind_t kind;
  int i;
};

struct Exp_Sum
{
  enum Exp_Kind_t kind;
  struct Exp_t *left;
  struct Exp_t *right;
};

// "constructors"
struct Exp_t *Exp_Int_new (int i)
{
  struct Exp_Int *p = malloc (sizeof(*p));
  p->kind = EXP_INT;
  p->i = i;
  return (struct Exp_t *)p;
}

struct Exp_t *Exp_Sum_new (struct Exp_t *left, struct Exp_t *right)
{
  struct Exp_Sum *p = malloc (sizeof(*p));
  p->kind = EXP_SUM;
  p->left = left;
  p->right = right;
  return (struct Exp_t *)p;
}

// "printer"
void Exp_print (struct Exp_t *exp)
{
  switch (exp->kind){
  case EXP_INT:{
    struct Exp_Int *p = (struct Exp_Int *)exp;
    printf ("%d", p->i);
    break;
  }
  case EXP_SUM:{
    struct Exp_Sum *p = (struct Exp_Sum *)exp;
    Exp_print (p->left);
    printf ("+");
    Exp_print (p->right);
    break;
  }
  default:
    break;
  }
}

//////////////////////////////////////////////
// Data structures for the Stack language.
enum Stack_Kind_t {STACK_ADD, STACK_PUSH};
struct Stack_t
{
  enum Stack_Kind_t kind;
};

struct Stack_Add
{
  enum Stack_Kind_t kind;
};

struct Stack_Push
{
  enum Stack_Kind_t kind;
  int i;
};

// "constructors"
struct Stack_t *Stack_Add_new ()
{
  struct Stack_Add *p = malloc (sizeof(*p));
  p->kind = STACK_ADD;
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
// list stack
struct List_t
{
  struct Stack_t *instr;
  struct List_t *next;
};

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
    case STACK_PUSH:
        printf("push %d\n", ((struct Exp_Int*)list->instr)->i);
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
// a compiler with const folding
struct List_t *folding = 0;

void emit (struct Stack_t *instr)
{
  all = List_new (instr, all);
}

void emit_with_folding (struct Stack_t *instr)
{
  // do folding at ADD instruction
  // pop two const varibles, then push the sum of them.
  if (STACK_ADD == instr->kind) {
    struct List_t *p = folding;
    if (p && p->instr && STACK_PUSH == p->instr->kind) {
      struct List_t *q = p->next;
      if (q && q->instr && STACK_PUSH == q->instr->kind) {
        folding = List_new(
                Stack_Push_new (((struct Stack_Push *)p->instr)->i + ((struct Stack_Push *)q->instr)->i),
                q->next);
        free(p);
        free(q);
#ifdef DEBUG
        printf("Folding!\n");
#endif
      }
    } else {
#ifdef DEBUG
      printf("instruction error!\n");
#endif
    }
  } else {
#ifdef DEBUG
    printf("No folding!\n");
#endif
    folding = List_new (instr, folding);
  }
}

void compile (struct Exp_t *exp)
{
  switch (exp->kind){
  case EXP_INT:{
    struct Exp_Int *p = (struct Exp_Int *)exp;
    emit (Stack_Push_new (p->i));
    emit_with_folding (Stack_Push_new (p->i));
    break;
  }
  case EXP_SUM:{
    struct Exp_Sum *p = (struct Exp_Sum *) exp;
    compile(p->left);
    compile(p->right);
    emit (Stack_Add_new());
    emit_with_folding (Stack_Add_new());
    break;
  }
  default:
    break;
  }
}

//////////////////////////////////////////////////
// program entry
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
