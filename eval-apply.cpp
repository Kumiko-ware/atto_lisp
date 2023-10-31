#include <iostream>
#include <regex>
#include "lispy.hpp"

//--------8<--------8<--------8<--------8<--------8<--------8<--------8<--------
// Forward declaration for mutual recursion
Cell eval(Cell expr, std::map<std::string, Cell>& env);

Cell ev_list(Cell params, std::map<std::string, Cell>& env)
{
  Cell args={};
  args.type = Type::List;
  for (auto arg: params.list){
    args.list.push_back(eval(arg, env));
  }
  return args;
}



Cell apply(Cell oper, Cell args)
{
  if(oper.type == Type::Builtin)
    return dispatch_builtin(oper, args);
  // we should handle lambdas here!
  return oper;
}

int is_true(Cell value)
{
  switch (value.type){
  case Type::Symbol:
    return (value.string == "T");
  case Type::Integer:
    return (value.integer !=0);
  case Type::Floating:
    return (value.floating !=0);
  case Type::String:
    return (value.string != "");
  case Type::List:
    return (value.list.size() != 0);
  }
  return 0;
}

Cell eval(Cell expr, std::map<std::string, Cell>& env)
{
  if (expr.type == Type::Symbol) {
    return env[expr.string];
  }
  if (expr.type == Type::List) {
    // First check special forms (QUOTE, DEFINE, SET!, COND, LAMBDA)
    if (expr.list[0].string == "QUOTE") {
      Cell cell = expr.list[1];
      return cell;
    }
    if (expr.list[0].string == "DEFINE") {
      if (expr.list.size() != 3) {
	std::cout << "DEFINE needs only 2 arguments" << std::endl;
      }
      if (env.find(expr.list[1].string) != env.end()) {
	std::cout << "New DEFINE of "<< expr.list[1].string << std::endl;
      }
      Cell cell = expr.list[1];
      env[expr.list[1].string] = expr.list [2];
      return cell;
    }
    if (expr.list[0].string == "SET!") {
      if (expr.list.size() != 3) {
	std::cout << "SET! needs only 2 arguments" << std::endl;
      }
      if (env.find(expr.list[1].string) == env.end()) {
	std::cout << "Nothing was DEFINEd as " << expr.list[1].string << std::endl;
      }
      Cell cell = expr.list[1];
      env[expr.list[1].string] = expr.list [2];
      return cell;
    }
    if (expr.list[0].string == "COND") {
      if (expr.list[1].type != Type::List){
	std::cout << "COND needs a list of clauses" << std::endl;
      }
      for (Cell clause: expr.list[1].list){
	if (clause.type != Type::List) {
	  std::cout << "Each clause must be a list" << std::endl;
	}
	if (clause.list.size() != 2) {
	  std::cout << "Each clause must be length 2" << std::endl;
	}
	if (is_true(eval(clause.list[0], env))){
	  return eval(clause.list[1], env);
	}
      }
    }
    // No special form, is an application:
    return apply(eval(expr.list[0],env), ev_list({Type::List,"",0,0,std::vector<Cell>(expr.list.begin()+1,expr.list.end()),0,0}, env));
  }
  // expr must be an atom, return it back
  return expr;   
}
//--------8<--------8<--------8<--------8<--------8<--------8<--------8<--------
