#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <stddef.h>

typedef struct{
  void *car;
  void *cdr;
}cell_st;

typedef cell_st* cell;

#define TAG_NUMBER 0
#define TAG_STRING 1
#define TAG_SYMBOL 2

#define EMPTY_LIST NULL
extern const cell T_SYMBOL;

cell cons(cell first, cell second);
cell car(cell pair);
cell cdr(cell pair);
cell set_car(cell pair, cell val);
cell set_cdr(cell pair, cell val);

cell caar(cell list);
cell cadr(cell list);
cell cdar(cell list);
cell cddr(cell list);

cell caaar(cell list);
cell caadr(cell list);
cell cadar(cell list);
cell caddr(cell list);
cell cdaar(cell list);
cell cdadr(cell list);
cell cddar(cell list);
cell cdddr(cell list);

cell length(cell list);

cell is_null(cell value);
cell is_pair(cell value);
cell is_atom(cell value);
cell is_number(cell value);
cell is_string(cell value);
cell is_symbol(cell value);
cell make_number(long int value);
cell make_string(char const *value);
cell make_symbol(char const *value);
long int integer(cell value);
char* string(cell value);
char* symbol(cell value);

cell eq_str(cell str1, cell str2);
cell eq_sym(cell sym1, cell sym2);
cell eq_num(cell num1, cell num2);
cell gt_num(cell num1, cell num2);
cell lt_num(cell num1, cell num2);
  
#endif //MEMORY_H_INCLUDED
