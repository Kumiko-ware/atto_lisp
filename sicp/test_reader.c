#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "memory.h"
#include "reader.h"
#include "printer.h"

int main(int argc, char **argv)
{
  /* value val; */
  /* while(1){ */
  /*   int tok = get_token(&val, argv[1]); */
  /*   printf("Tok: %i\n", tok); */
  /*   if (tok == TOK_INT) */
  /*     printf("\t%i\n", val.int_value); */
  /*   if (tok == TOK_STR || tok == TOK_SYM) */
  /*     printf("\t%2$*1$.*1$s\n", val.int_value, val.str_value); */
  /*   if (tok <=0) */
  /*     return tok; */
  /* } */

  cell exp;

  while (read(&exp, argv[1]) == READ_EXP){
    print_cell(exp);
    printf("\n");
  }
}
