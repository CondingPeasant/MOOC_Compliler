#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semant.h"

#define TODO()\
  do{\
  printf ("TODO: add your code at file: \"%s\", line: %d\n", __FILE__, __LINE__);\
}while(0)

//////////////////////////////////////
// the symbol table
List_t table = 0;

// lookup an "id" in the symbol table.
// return "type" on success; and -1 for failure
Type_t Table_lookup (char *id)
{
  List_t p = table;
  while (p){
    Dec_t d = (Dec_t)p->data;
    if (strcmp(d->id, id)==0)
      return d->type;
    p = p->next;
  }
  return -1;
}

void Table_insert (Dec_t dec)
{
  
  if (Table_lookup (dec->id) != -1){
    fprintf (stderr, "Error: the variable "
	     "\"%s\" is declared duplicately!\n", dec->id);
    exit (0);
  }
  table = List_new (dec, table);
  return;
}

//////////////////////////////////////////
// dec
void check_dec(Dec_t d)
{
  Table_insert (d);
}

void check_decs(List_t decs)
{
  while (decs){
    Dec_t d = (Dec_t)decs->data;
    check_dec(d);
    decs = decs->next;
  }
  return;
}

////////////////////////////////////////
// exp

// Your job:
Type_t check_exp (Exp_t exp)
{
  switch(exp->kind) {
  case EXP_INT:
    return TYPE_INT;
  case EXP_TRUE:
  case EXP_FALSE:
    return TYPE_BOOL;
  case EXP_ID:
  {
    Type_t t = Table_lookup(((Exp_Id)exp)->id);
    if (t == -1) {
      fprintf (stderr, "Error: the variable "
	     "\"%s\" has not benn declared!\n", ((Exp_Id)exp)->id);
      exit(-1);
    } else {
      return t;
    }
  }
  case EXP_ADD:
  case EXP_SUB:
  case EXP_TIMES:
  case EXP_DIVIDE:
  {
    Exp_Add e = (Exp_Add) exp;
    Type_t t1 = check_exp(e->left);
    Type_t t2 = check_exp(e->right);
    if (t1 == TYPE_INT && t2 == TYPE_INT) {
      return TYPE_INT;
    } else {
      fprintf(stderr, "The type of arithmetic operand must be integer!\n");
      exit(-1);
    }
  }
  case EXP_AND:
  case EXP_OR:
  {
    Exp_And e = (Exp_And) exp;
    Type_t t1 = check_exp(e->left);
    Type_t t2 = check_exp(e->right);
    if (t1 == TYPE_BOOL && t2 == TYPE_BOOL) {
      return TYPE_BOOL;
    } else {
      fprintf(stderr, "The type of boolean operand must be bool!\n");
      exit(-1);
    }
  }
  }
}

////////////////////////////////////////
// stm

// Your job:
void check_stm (Stm_t stm)
{
  switch(stm->kind) {
  case STM_ASSIGN: {
    Type_t t1 = Table_lookup(((Stm_Assign)stm)->id);
    Type_t t2 = check_exp(((Stm_Assign)stm)->exp);
    if (t1 == -1) {
      fprintf (stderr, "Error: the variable "
	     "\"%s\" has not benn declared!\n", ((Stm_Assign)stm)->id);
      exit(-1);
    } else if (t1 != t2) {
      fprintf(stderr, "The type of id and that of expression don't match!\n");
      exit(-1);
    }
    return;
  }
  case STM_PRINTI:
  {
    Type_t t = check_exp(((Stm_Printi)stm)->exp);
    if (t != TYPE_INT) {
      fprintf(stderr, "The type of parameter of printi() should be integer!\n");
      exit(-1);
    }
    return;
  }
  case STM_PRINTB:
  {
    Type_t t = check_exp(((Stm_Printb)stm)->exp);
    if (t != TYPE_BOOL) {
      fprintf(stderr, "The type of parameter of printb() should be bool!\n", t);
      exit(-1);
    }
    return;
  }
  }
}

void check_stms(List_t stms)
{
  while (stms){
    Stm_t s = (Stm_t)stms->data;
    check_stm(s);
    stms = stms->next;
  }
  return;
}


void Semant_check(Prog_t prog)
{
  // create table
  check_decs(prog->decs);
  // check stm
  check_stms(prog->stms);
  return;
}
