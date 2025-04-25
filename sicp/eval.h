#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

#include "memory.h"

// From Ch. 5.4 - Explicit control evaluator

cell eval_dispatch(cell exp, cell env);
cell ev_self_eval(cell exp);
cell ev_variable(cell exp, cell env);
cell ev_quoted(cell exp);
cell ev_assignment(cell exp, cell env);
cell ev_definition(cell exp, cell env);
cell ev_lambda(cell exp, cell env);

#endif //EVAL_H_INCLUDED
