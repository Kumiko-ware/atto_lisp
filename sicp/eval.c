#include "memory.h"
#include "env.h"
#include "exp.h"
#include "eval.h"
#include "primitives.h"

// From Ch. 5.4 - Explicit control evaluator

cell eval_dispatch(cell exp, cell env){
 eval_dispatch:
  if (is_self_evaluating(exp))
    return ev_self_eval(exp);
  if (is_variable(exp))
    return ev_variable(exp, env);
  if (is_quoted(exp))
    return ev_quoted(exp);
  if (is_assignment(exp))
    return ev_assignment(exp, env);
  if (is_definition(exp))
    return ev_definition(exp, env);
  if (is_if(exp)){
    cell if_pred = eval_dispatch(if_predicate(exp), env);
    if (is_true(if_pred)) {
      exp = if_consequent(exp);
    } else {
      exp = if_alternative(exp);
    }
    goto eval_dispatch;
  }
  if (is_lambda(exp))
    return ev_lambda(exp, env);
  if (is_begin(exp)){
    cell actions = begin_actions(exp);
    while(!is_last_exp(actions)){
      cell act = car(actions);
      eval_dispatch(act, env);
      actions = cdr(actions);
    }
    exp = car(actions);
    goto eval_dispatch;
    }
  if (is_application(exp)){
    cell proc = eval_dispatch(operator(exp), env);
    cell argl = EMPTY_LIST;
    cell ops = operands(exp);
    while(ops){
      cell last;
      cell op = car(ops);
      if (!argl){
	argl = cons(eval_dispatch(op, env), EMPTY_LIST);
	last = argl;
      }
      else{
	set_cdr(last, cons(eval_dispatch(op, env), EMPTY_LIST));
	last = cdr(last);
      }
      ops = cdr(ops);
    }
    if (is_primitive_procedure(proc)){
      return apply_primitive_procedure(cdr(proc), argl);
    }
    if (is_compound_procedure(proc)){
      env = extend_environment(procedure_parameters(proc),
			 argl, env);
      exp = cons(make_symbol("BEGIN"),
		 procedure_body(proc));
      goto eval_dispatch;
    }
    // ERROR "unknown procedure type";
  }
  // ERROR "Unknown expression type";
  return EMPTY_LIST;
}

cell ev_self_eval(cell exp)
{
  return exp;
}

cell ev_variable(cell exp, cell env)
{
  return lookup_variable_value(exp, env);
}

cell ev_quoted(cell exp)
{
  return text_of_quotation(exp);
}

cell ev_assignment(cell exp, cell env)
{
  cell val = eval_dispatch(assignment_value(exp), env);
  cell var = assignment_variable(exp);
  return set_variable_value(var, val, env);
}

cell ev_definition(cell exp, cell env)
{
  cell val = eval_dispatch(definition_value(exp), env);
  cell var = definition_variable(exp);
  return define_variable(var, val, env);
}

cell ev_lambda(cell exp, cell env)
{
  cell params = lambda_parameters(exp);
  cell body = lambda_body(exp);
  return make_procedure(params, body, env);
}
