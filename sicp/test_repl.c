#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "env.h"
#include "memory.h"
#include "reader.h"
#include "printer.h"
#include "eval.h"

int main(int argc, char **argv)
{
  cell exp;
  cell env;
  cell plus = cons(make_symbol("PRIMITIVE"), make_symbol("+"));
  cell minus = cons(make_symbol("PRIMITIVE"), make_symbol("-"));
  cell mult = cons(make_symbol("PRIMITIVE"), make_symbol("*"));

  env = cons(cons(EMPTY_LIST,EMPTY_LIST), EMPTY_LIST);
  env = extend_environment(cons(make_symbol("+"),
				cons(make_symbol("-"),
				     cons(make_symbol("*"), EMPTY_LIST))),
			   cons(plus,
				cons(minus,
				     cons(mult, EMPTY_LIST))),
			   env);
  while (read(&exp, argv[1]) == READ_EXP){
    print_cell(eval_dispatch(exp, env));
    printf("\n");
  }
}
