#include "memory.h"

cell apply_primitive_procedure(cell proc, cell argl)
{
  if (eq_sym(proc, make_symbol("+"))){
    long int result = 0;
    while (argl){
      result += integer(car(argl));
      argl = cdr(argl);
    }
    return (make_number(result));
  }
  if (eq_sym(proc, make_symbol("-"))){
    long int result = 0;
    if (integer(length(argl)) == 1)
      result = -integer(car(argl));
    if (integer(length(argl)) > 1){
      result = integer(car(argl));
      do{
	argl = cdr(argl);
	result -= integer(car(argl));
      }while(cdr(argl));
    }
    return (make_number(result));
  }
  if (eq_sym(proc, make_symbol("*"))){
    long int result = 1;
    while (argl){
      result *= integer(car(argl));
      argl = cdr(argl);
    }
    return (make_number(result));
  }
}
