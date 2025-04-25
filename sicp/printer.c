#include <stdio.h>
#include "memory.h"

void print_cell(cell val)
{
  if (is_null(val))
    printf("() ");
  if (is_number(val))
    printf("#d%li ", integer(val));
  if (is_string(val))
    printf("\"%s\" ", string(val));
  if (is_symbol(val))
    printf("%s ", symbol(val));
  if (is_pair(val)){
    printf("(");
    do{
      print_cell(car(val));
    } while(is_pair(val = cdr(val)));
    if (is_atom(val)){
      printf(" . ");
      print_cell(val);
    }
    printf(")");
  }
  fflush(stdout);
}
