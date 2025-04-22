#include <stddef.h>
#include "memory.h"

// From Ch. 4.1.3 - Operations on environments

cell enclosing_environment(cell env)
{
  return cdr(env);
}

cell first_frame(cell env)
{
  return car(env);
}

static cell EMPTY_ENV = EMPTY_LIST;

cell make_frame(cell variables, cell values)
{
  return cons(variables, values);
}

cell frame_variables(cell frame)
{
  return car(frame);
}

cell frame_values(cell frame)
{
  return cdr(frame);
}

cell add_binding_to_frame(cell var, cell val, cell frame)
{
  set_car(frame, cons(var, car(frame)));
  set_cdr(frame, cons(val, cdr(frame)));
  return var;
}

cell extend_environment(cell vars, cell vals, cell base_env)
{
  if (eq_num(length(vars), length(vals))){
    return cons(make_frame(vars, vals), base_env);
  }
  if (lt_num(length(vars), length(vals))){
      // ERROR "Too many argument supplied" vars vals 
    } else {
      // ERROR "Too few arguments supplied" vars vals
    }
    return EMPTY_LIST;
}

cell lookup_variable_value(cell var, cell env)
{
 env_loop:
  if (env == EMPTY_ENV){
    // ERORR "Unbound variable" var
    return EMPTY_LIST;
  }
  cell frame = first_frame(env);
  for (cell vars = frame_variables(frame), vals = frame_values(frame);
       vars = cdr(vars), vals = cdr(vals);
       vars != EMPTY_LIST){
    if (eq_str(var, car(vars))) {
      return car(vals);
    }
  }
  // Here we could just do:
  // return lookup_variable_value(var, enclosing_environment(env));
  // But just in case we work with a lame compiler, we use goto:
  env = enclosing_environment(env);
  goto env_loop;
}

cell set_variable_value(cell var, cell val, cell env)
{
 env_loop:
  if (env == EMPTY_ENV){
    // ERORR "Unbound variable: SET!" var
    return EMPTY_LIST;
  }
  cell frame = first_frame(env);
  for (cell vars = frame_variables(frame), vals = frame_values(frame);
       vars = cdr(vars), vals = cdr(vals);
       vars != EMPTY_LIST){
    if (eq_str(var, car(vars))) {
      set_car(vals, val);
      return var;
    }
  }
  env = enclosing_environment(env);
  goto env_loop;
} 

cell define_variable(cell var, cell val, cell env)
{
  cell frame = first_frame(env);
  for (cell vars = frame_variables(frame), vals = frame_values(frame);
       vars = cdr(vars), vals = cdr(vals);
       vars != EMPTY_LIST){
    if (eq_str(var, car(vars))) {
      set_car(vals, val);
      return var;
    }
  }
  return add_binding_to_frame(var, val, frame);
}
