#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

typedef void* cell;

cell cons(cell first, cell second);
cell car(cell pair);
cell cdr(cell pair);

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

int is_null(cell value);
int is_pair(cell value);
int is_number(cell value);
int is_string(cell value);
int is_symbol(cell value);
cell *number2cell(int value);
cell *string2cell(char *value);
cell *symbol2cell(char *value);
int cell2number(cell value);
char *cell2string(cell value);
char *cell2symbol(cell value);

#endif //MEMORY_H_INCLUDED
