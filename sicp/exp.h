#ifndef EXPRESSIONS_H_INCLUDED
#define EXPRESSIONS_H_INCLUDED

#include "memory.h"

// From Ch. 4.1.2 - Representing expressions

cell is_self_evaluating(cell exp);
cell is_variable(cell exp);
cell is_quoted(cell exp);
cell text_of_quotation(cell exp);
cell is_tagged_list(cell exp, char const *tag);
cell is_assignment(cell exp);
cell assignment_variable(cell exp);
cell assignment_value(cell exp);
cell is_definition(cell exp);
cell definition_variable(cell exp);
cell definition_value(cell exp);
cell is_lambda(cell exp);
cell lambda_parameters(cell exp);
cell lambda_body(cell exp);
cell make_lambda(cell parameters, cell body);
cell is_if(cell exp);
cell if_predicate(cell exp);
cell if_consequent(cell exp);
cell if_alternative(cell exp);
// cell make_if(cell pred, cell conseq, cell alt);
cell is_begin(cell exp);
cell begin_actions(cell exp);
cell is_last_exp(cell seq);
cell first_exp(cell seq);
cell rest_exps(cell seq);
// cell sequence2exp(cell seq);
// cell make_begin(cell seq);
cell is_application(cell exp);
cell operator(cell exp);
cell operands(cell exp);
cell no_operands(cell ops);
cell first_operand(cell ops);
cell rest_operands(cell ops);

// From Ch. 4.1.3 - Operations on environments

cell make_procedure(cell parameters, cell body, cell env);
cell is_compound_procedure(cell p);
cell procedure_parameters(cell p);
cell procedure_body(cell p);
cell procedure_environment(cell p);
cell is_true(cell value);
  
// From Ch. 4.1.4 - Runing the evaluator as a command

cell is_primitive_procedure(cell proc);
cell primitive_implementation(cell proc);

// From Ch. 5.4.1 - Footnote
cell is_last_operand(cell ops);

#endif //EXPRESSIONS_H_INCLUDED
