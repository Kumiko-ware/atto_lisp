#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "memory.h"

void dump_cell(cell c)
{
  printf("car: %16.16lX\n", c->car);
  printf("cdr: %16.16lX\n", c->cdr);
}

int main(int argc, char **argv)
{
  printf("Sizes\n");
  printf("int32_t: %li\n", sizeof(long int));
  printf("void* %li\n", sizeof(void*));
  printf("ptr %p\n", malloc(1));
  printf("ptr %p\n", malloc(1));
  printf("ptr %p\n", malloc(1));
  printf("ptr %p\n", malloc(1));
  printf("ptr %p\n", malloc(1));
  printf("ptr %p\n", malloc(1));
  printf("ptr %p\n", malloc(1));
  printf("ptr %p\n", malloc(1));

  dump_cell(number2cell(42));
  dump_cell(string2cell("Hello wolrd"));
  
  printf("int: %li\n", cell2number(number2cell(42)));
  printf("string: %s\n", cell2string(string2cell("Hello world")));
  printf("symbol: %s\n", cell2symbol(symbol2cell("func-name")));
}
