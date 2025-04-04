#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "memory.h"

cell cons(cell first, cell second)
{
  assert(sizeof(void*) == sizeof(long int));
  cell cons = malloc(sizeof(cell_st));
  assert(cons);
  assert(!((long int)cons & 3));
  cons->car = first;
  cons->cdr = second;
  return cons;
}

cell car(cell pair)
{
  return pair->car;
}

cell cdr(cell pair)
{
  return pair->cdr;
}

cell caar(cell list)
{
  return car(car(list));
}

cell cadr(cell list)
{
  return car(cdr(list));
}

cell cdar(cell list)
{
  return cdr(car(list));
}

cell cddr(cell list)
{
 return cdr(cdr(list));
}

cell caaar(cell list)
{
  return car(car(car(list)));
}

cell caadr(cell list)
{
  return (car(car(cdr(list))));
}

cell cadar(cell list)
{
  return (car(cdr(car(list))));
}

cell caddr(cell list)
{
  return (car(cdr(cdr(list))));
}

cell cdaar(cell list)
{
  return (cdr(car(car(list))));
}

cell cdadr(cell list)
{
   return (cdr(car(cdr(list))));
}

cell cddar(cell list)
{
  return (cdr(cdr(car(list))));
}

cell cdddr(cell list)
{
  return (cdr(cdr(cdr(list))));
}

int is_null(cell value)
{
  return !value;
}

int is_pair(cell value)
{
  return !(*((unsigned long int *) value) & 3);
}

int is_number(cell value)
{
  return (*((unsigned long int *) value) & 1) &&
    (*((unsigned long int *) value) >> 1 == TAG_NUMBER);
}


int is_string(cell value)
{
  return (*((unsigned long int *) value) & 1) &&
    (*((unsigned long int *) value) >> 1 == TAG_STRING);
}


int is_symbol(cell value)
{
  return (*((unsigned long int *) value) & 1) &&
    (*((unsigned long int *) value) >> 1 == TAG_SYMBOL);
}


cell number2cell(long int value)
{
  return cons((void*) (TAG_NUMBER << 1 | 1),
	      (void*) value);
}


cell string2cell(char *value)
{
  return cons((void*) (TAG_STRING << 1 | 1),
	      (void*) value);
}


cell symbol2cell(char *value)
{
  return cons((void*) (TAG_SYMBOL << 1 | 1),
	      (void*) value);
}


long int cell2number(cell value)
{
  (long int)cdr(value);
}


char *cell2string(cell value)
{
  (char *)cdr(value);
}


char *cell2symbol(cell value)
{
  (char *)cdr(value);
}
