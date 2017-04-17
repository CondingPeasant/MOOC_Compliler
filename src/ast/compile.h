#ifndef COMPILE_H
#define COMPILE_H
// Data structures for the Stack language.
// "constructors" struct Stack_t *Stack_Add_new ()
enum Stack_Kind_t {STACK_ADD, STACK_MINUS, STACK_TIMES, STACK_DIVIDE, STACK_PUSH};
struct Stack_t
{
  enum Stack_Kind_t kind;
};

struct Stack_Add
{
  enum Stack_Kind_t kind;
};
struct Stack_t *Stack_Add_new ();

struct Stack_Minus
{
  enum Stack_Kind_t kind;
};
struct Stack_t *Stack_Minus_new ();

struct Stack_Times
{
  enum Stack_Kind_t kind;
};
struct Stack_t *Stack_Times_new ();

struct Stack_Divide
{
  enum Stack_Kind_t kind;
};
struct Stack_t *Stack_Divide_new ();

struct Stack_Push
{
  enum Stack_Kind_t kind;
  int i;
};
struct Stack_t *Stack_Push_new (int i)

// instruction list
// list stack
struct List_t
{
  struct Stack_t *instr;
  struct List_t *next;
};

struct List_t *List_new (struct Stack_t *instr, struct List_t *next);
void List_reverse_print (struct List_t *list);
void emit (struct Stack_t *instr);
void compile (struct Exp_t *exp);
#endif
