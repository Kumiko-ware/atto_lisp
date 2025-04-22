#include <string.h>
#include "memory.h"
#include "exp.h"

// From Ch. 4.1.2 - Representing expressions

cell is_self_evaluating(cell exp)
{
  if (is_number(exp))
    return T_SYMBOL;
  if (is_string(exp))
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell is_variable(cell exp)
{
  return is_symbol(exp);
}

cell is_quoted(cell exp)
{
  return is_tagged_list(exp, "QUOTE");
}

cell text_of_quotation(cell exp)
{
  return cadr(exp);
}

cell is_tagged_list(cell exp, char const *tag)
{
  if (is_pair(exp) &&
      is_symbol(car(exp)) &&
      eq_sym(car(exp), make_symbol(tag)))
    return T_SYMBOL;
  return EMPTY_LIST;
}

cell is_assignment(cell exp)
{
  return is_tagged_list(exp, "SET!");
}

cell assignment_variable(cell exp)
{
  return cadr(exp);
}

cell assignment_value(cell exp)
{
  return caddr(exp);
}

cell is_definition(cell exp)
{
  return is_tagged_list(exp, "DEFINE");
}

cell definition_variable(cell exp)
{
  if (is_symbol(cadr(exp)))
    return cadr(exp); // variable
  else
    return caadr(exp); // lambda
}

cell definition_value(cell exp)
{
  if (is_symbol(cadr(exp)))
    return caddr(exp);
  else
    return make_lambda(cdadr(exp), cddr(exp));
}

cell is_lambda(cell exp)
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
  return cons(make_symbol("LAMBDA"),
	      cons(parameters, body));
}

cell is_if(cell exp)
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

/* Optional cond->if
   NOTE: implies implementing list primitive
cell make_if(cell pred, cell conseq, cell alt)
{
  return list(pred, conseq, alt);
}
*/

cell is_last_exp(cell seq)
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

/* Optional cond->if
cell sequence2exp(cell seq)
{
  if (is_null(seq)){
    return seq;
  }
  if (is_last_exp(seq)){
    return first_exp(seq);
  }
  return make_begin(seq);
}

cell make_begin(cell seq)
{
  return cons(make_symbol("begin"), seq);
}
*/

cell is_begin(cell exp)
{
  return is_tagged_list(exp, "BEGIN");
}

cell begin_actions(cell exp)
{
  return cdr(exp);
}

cell is_application(cell exp)
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

cell no_operands(cell ops)
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
