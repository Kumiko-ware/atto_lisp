#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "memory.h"
#include "printer.h"

void dump_cell(cell c)
{
  printf("car: %16.16lX\n", c->car);
  printf("cdr: %16.16lX\n", c->cdr);
}

int main(int argc, char **argv)
{
  cell list = cons(make_number(42),
		  cons(make_string("Hello"),
		       cons(make_symbol("World"),EMPTY_LIST)));
  
  dump_cell(list);
  dump_cell(car(list));
  dump_cell(cdr(list));
  dump_cell(cadr(list));
  dump_cell(cddr(list));
  dump_cell(caddr(list));
  cdddr(list);
  print_cell(list);
}
