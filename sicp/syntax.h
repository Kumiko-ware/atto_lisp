#ifndef SYNTAX_H_INCLUDED
#define SYNTAX_H_INCLUDED

#include "memory.h"

/*
  This is based on the file
  https://github.com/ivanjovanovic/sicp/blob/master/5.4/ch5-syntax.scm
*/

int is_self_evaluating(cell exp);
int is_quoted(cell exp);
char *text_of_quotation(cell exp);
int is_tagged_list(cell exp, char const *tag);
int is_variable(cell exp);
int is_assignment(cell exp);
char *assignment_variable(cell exp);
cell assignment_value(cell exp);
int is_definition(cell exp);
char *definition_variable(cell exp);
cell definition_value(cell exp);
int is_lambda(cell exp);
cell lambda_parameters(cell exp);
cell lambda_body(cell exp);
cell make_lambda(cell parameters, cell body);
int is_if(cell exp);
cell if_predicate(cell exp);
cell if_consequent(cell exp);
cell if_alternative(cell exp);
int is_begin(cell exp);
cell begin_actions(cell exp);
int is_last_exp(cell seq);
cell first_exp(cell seq);
cell rest_exps(cell seq);
int is_application(cell exp);
cell operator(cell exp);
cell operands(cell exp);
int no_operands(cell ops);
cell first_operand(cell ops);
cell rest_operands(cell ops);

#endif //SYNTAX_H_INCLUDED
