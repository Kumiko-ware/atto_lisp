#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "memory.h"

// NOTE: the defition of t_symbol_st must comply with make_symbol
// function below!
struct cell_st t_symbol_st={(void*) (TAG_SYMBOL << 1 | 1),
		     (void*) "T"};
const cell T_SYMBOL = &t_symbol_st;

/*
  Here is the core of the memory managememnt. All memory allocations
  use CONS in the core.

  A cell value is a pair of 2 pointers: CAR and CDR (see cell type
  definition in memory.h)

      +----------------+----------------+
      | CAR            | CDR            |
      +----------------+----------------+

  Now, becase of alignment, in virtually all modern architectures,
  pointers have a 4 byte alignment, which means the the 2 LSB are
  0. We start by using this fact to distinguish between a pair and an
  atom. But is not strictly necessary (s. below).

  If the CAR has the LSB set, then it cannot be a pointer, and the
  rest of the bits will be interpreted as information about what is in
  the CDR. If the LSB is 0, then the CAR is interpreted as a pointer,
  and also the CDR is assumed to be a pointer.

  Pair:
      +----------------+----------------+
      | xxxxxxxxxxxxx0 | xxxxxxxxxxxxx0 |
      +----------------+----------------+

      Note that we only check the LSB in the CAR part, we assume the
      CDR is correct, which should hold.
      
  Atom:
      +----------------+----------------+
      | META     TAG 1 | VALUE          |
      +----------------+----------------+

      Note each in the CAR from LSB to MSB can be interpreted based on
      the values of the previous bits, so the TAG and META fields can
      have dynamic length. META would be for example the length of the
      string pointed to by the CDR, when the TAG implies a limited
      length string.
  
  Now, note that becase every allocation consist of 2 processor words,
  every cell will start in a multiple of 2 of the first allocation
  (assuming the allocations are made contiguous). That means that even
  in architectures where the allocations does not follow 4 bytes
  alignment, this method can still work.

  In the function we first we make sure that the types we use for
  atoms (float and long int) and pointers is the same. So we are sure
  we can place atom values in the CDR. Note that strings are just a
  pointer, and symbols are just strings.

  Then we do a malloc, and check the result with assert, making sure
  is not a null pointer, and that satisfies the assumption of LSB=0.

  Last we just populate the contents of the created pair, and return
  it.
*/
cell cons(cell first, cell second)
{
  assert(sizeof(void*) == sizeof(long int)); // == sizeof(float) ?;
  cell cons = malloc(sizeof(cell_st));
  assert(cons); // malloc ok
  assert(!((unsigned long int)cons&3)); // address has 00 LSBs
  set_car(cons, first);
  set_cdr(cons, second);
  return cons;
}

cell car(cell pair)
{
  if (pair){
    return pair->car;
  } else {
    return EMPTY_LIST;
  }
}

cell cdr(cell pair)
{
  if (pair) {
    return pair->cdr;
  } else {
    return EMPTY_LIST;
  }
}

cell set_car(cell pair, cell val)
{
  pair->car = val;
}

cell set_cdr(cell pair, cell val)
{
  pair->cdr = val;
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

cell length(cell list)
{
  int length = 0;
  if (! is_pair(list))
    return make_number(length);
  for (cell elm = list; elm = elm->cdr ; is_pair(elm) ){
    ++length;
  }
  return make_number(length);
}

cell is_null(cell value)
{
  if (value == EMPTY_LIST)
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell is_pair(cell value)
{
  if (!is_null(value) &&
      !((unsigned long int)value->car & 1))
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell is_atom(cell value)
{
  if (!is_null(value) &&
      ((unsigned long int)value->car & 1))
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell is_number(cell value)
{
  if (is_atom(value) &&
      ((unsigned long int)value->car >> 1 == TAG_NUMBER))
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell is_string(cell value)
{
  if (is_atom(value) &&
      ((unsigned long int)value->car >> 1 == TAG_STRING))
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell is_symbol(cell value)
{
  if (is_atom(value) &&
      ((unsigned long int)value->car >> 1 == TAG_SYMBOL))
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell make_number(long int value)
{
  return cons((cell) (TAG_NUMBER << 1 | 1),
	      (cell) value);
}

cell make_string(char const *value)
{
  return cons((cell) (TAG_STRING << 1 | 1),
	      (cell) value);
}

cell make_symbol(char const *value)
{
  return cons((cell) (TAG_SYMBOL << 1 | 1),
	      (cell) value);
}

long int integer(cell value)
{
  (long int)cdr(value);
}

char* string(cell value)
{
  (char *)cdr(value);
}

char* symbol(cell value)
{
  (char *)cdr(value);
}

cell eq_str(cell str1, cell str2)
{
  if (!strcmp(string(str1), string(str2)))
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell eq_sym(cell sym1, cell sym2)
{
  if (!strcmp(symbol(sym1), symbol(sym2)))
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell eq_num(cell num1, cell num2)
{
  if (integer(num1) == integer(num2))
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell gt_num(cell num1, cell num2)
{
  if (integer(num1) > integer(num2))
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell lt_num(cell num1, cell num2)
{
  if (integer(num1) < integer(num2))
    return T_SYMBOL;
  return EMPTY_LIST;
}
