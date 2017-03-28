#include <stdio.h>

enum Token_Kind_t {TOKEN_ID, TOKEN_NUM, TOKEN_IF};

struct Token_t
{
  enum Token_Kind_t kind;
};

struct Token_IF_t
{
  enum Token_Kind_t kind;
};

struct Token_ID_t
{
  enum Token_Kind_t kind;
  char* content;
};

struct Token_NUM_t
{
  enum Token_Kind_t kind;
  char* content;
};

int main() {
    return 0;
}
