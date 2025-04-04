#include <string.h>
#include "memory.h"
#include "syntax.h"

/*
  This is based on the file
  https://github.com/ivanjovanovic/sicp/blob/master/5.4/ch5-syntax.scm
*/

int is_self_evaluating(cell exp)
{
  if (is_number(exp)) return 1;
  if (is_string(exp)) return 1;
  return 0;
}

int is_quoted(cell exp)
{
  return is_tagged_list(exp, "QUOTE");
}

char *text_of_quotation(cell exp)
{
  cell2string(cadr(exp));
}

int is_tagged_list(cell exp, char const *tag)
{
  return (is_pair(exp) &&
	  is_symbol(car(exp)) &&
	  !strcmp(cell2symbol(car(exp)), tag));
}

int is_variable(cell exp)
{
  return is_symbol(exp);
}

int is_assignment(cell exp)
{
  return is_tagged_list(exp, "SET!");
}

char *assignment_variable(cell exp)
{
  return cell2string(cadr(exp));
}

cell assignment_value(cell exp)
{
  return caddr(exp);
}

int is_definition(cell exp)
{
  return is_tagged_list(exp, "DEFINE");
}

char *definition_variable(cell exp)
{
  if (is_symbol(cadr(exp)))
    return cell2symbol(cadr(exp)); // variable
  else
    return cell2symbol(caadr(exp)); // lambda
}

cell definition_value(cell exp)
{
  if (is_symbol(cadr(exp)))
    return caddr(exp);
  else
    return make_lambda(cdadr(exp), cddr(exp));
}

int is_lambda(cell exp)
{
  return is_tagged_list(exp, "LAMBDA");
}

cell lambda_parameters(cell exp)
{
  return cadr(exp);
}

cell lambda_body(cell exp)
{
  return cddr(exp);
}

cell make_lambda(cell parameters, cell body)
{
  return cons(string2cell("LAMBDA"),
	      cons(parameters, body));
}

int is_if(cell exp)
{
  is_tagged_list(exp, "IF");
}

cell if_predicate(cell exp)
{
  return cadr(exp);
}

cell if_consequent(cell exp)
{
  return caddr(exp);
}

cell if_alternative(cell exp)
{
  return cdddr(exp);
}

int is_begin(cell exp)
{
  return is_tagged_list(exp, "BEGIN");
}

cell begin_actions(cell exp)
{
  return cdr(exp);
}

int is_last_exp(cell seq)
{
  return is_null(cdr(seq));
}

cell first_exp(cell seq)
{
  return car(seq);
}

cell rest_exps(cell seq)
{
  return cdr(seq);
}

int is_application(cell exp)
{
  return is_pair(exp);
}

cell operator(cell exp)
{
  return car(exp);
}

cell operands(cell exp)
{
  return cdr(exp);
}

int no_operands(cell ops)
{
  return is_null(ops);
}

cell first_operand(cell ops)
{
  return car(ops);
}

cell rest_operands(cell ops)
{
  return cdr(ops);
}
