#include <stdio.h>

int main(int argc, char **argv)
{
  int i=0;
 loop:
  {
    int stack_var=10;
    printf("BEGIN: %i\n", stack_var);
    if (i<2) {
      i++;
      stack_var++;
      printf("LOOP: %i -- i: %i\n", stack_var, i);
      goto loop;
    }
    printf("END: %i\n", stack_var);
  }
  return 0;
}
