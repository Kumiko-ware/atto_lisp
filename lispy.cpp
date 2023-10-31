#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>

//--------8<--------8<--------8<--------8<--------8<--------8<--------8<--------
// TYPES
enum class Type { Symbol, String, Integer, Floating, List, Error, Builtin};

struct Cell{
  Type type;

  std::string string;
  int64_t integer;
  double floating;
  std::vector<Cell> list;

  int line;
  int pos;
};

std::map<std::string, Cell> environment = {
					   {"+" , {Type::Builtin, "+", 0, 0, std::vector<Cell>(), 0, 0}},
					   {"-" , {Type::Builtin, "-", 0, 0, std::vector<Cell>(), 0, 0}},
					   {"*" , {Type::Builtin, "*", 0, 0, std::vector<Cell>(), 0, 0}},
					   {"/" , {Type::Builtin, "/", 0, 0, std::vector<Cell>(), 0, 0}},
					   {">" , {Type::Builtin, ">", 0, 0, std::vector<Cell>(), 0, 0}},
					   {"<" , {Type::Builtin, "<", 0, 0, std::vector<Cell>(), 0, 0}},
					   {"=" , {Type::Builtin, "=", 0, 0, std::vector<Cell>(), 0, 0}}
};

//--------8<--------8<--------8<--------8<--------8<--------8<--------8<--------
// READER
int next_token_list_end(const char* str)
{
  // Consume all whitespace
  while((*str == ' ') || (*str == '\t') || (*str == '\r') || (*str == '\n'))
    str++;
  if (*str == ')'){
    return 1;
  }
  return 0;
}


Cell read(const char* string)
{
  static const char* str;
  std::regex int_re("(\\+|-)?[0123456789]+",
		    std::regex_constants::extended);
  std::regex hex_re("0x[0123456789ABCDEF]+",
		    std::regex_constants::extended |
		    std::regex_constants::icase);
  std::regex float_re("(\\+|-)?[0123456789]*\\.?[0123456789]+(e(\\+|-)?[0123456789]+)?",
		      std::regex_constants::extended |
		      std::regex_constants::icase);
  Cell cell = {};
    
  if(string) {
    str=string;
    return cell;
  }

  // EOI
  if (*str == '\0'){
    cell.type = Type::Symbol;
    cell.string = "";
    return cell;
  }

  // Consume all whitespace
  while((*str == ' ') || (*str == '\t') || (*str == '\r') || (*str == '\n'))
    str++;

  if (*str == '('){
    str++;
    cell.type = Type::List;
    while (!next_token_list_end(str)) {
      Cell elem = read(NULL);
      if ( (elem.type != Type::Symbol) || (elem.string != "") )
	cell.list.push_back(elem);
      else{
	std::cout << "List without end!" << std::endl;
	return elem;
      }
    }
    // Consume all whitespace
    while((*str == ' ') || (*str == '\t') || (*str == '\r') || (*str == '\n'))
      str++;
    if (*str == ')'){
      str++;
    }
    return cell;
  }

  if (*str == '\''){
    str++;
    cell.type = Type::List;
    Cell quote = {};
    quote.type = Type::Symbol;
    quote.string = "QUOTE";
    cell.list.push_back(quote);
    cell.list.push_back(read(NULL));
    return cell;
  }
  
  if (*str == ')'){
    str++;
    std::cout << "End without list!" << std::endl;
  }


  if (*str == '"'){
    cell.type = Type::String;
    str++;
    const char* token = str;
    while ((*str != '\"') && (*str != '\0')) {
      if ((*str == '\\')&&(*(str+1) != '\0'))
	str++; // skip scaped characters
      str++;
    }
    if (*str == '\0'){
      std::cout << "End of input reading string!" << std::endl;
      return cell;
    }
    str++; // consume closing "
    std::string token_str(token, str-token -1); // -1 ignore the closing "
    while (std::string::size_type pos = token_str.find("\\\"") !=
	   std::string::npos){
      token_str.replace(pos,2,"\"");
    }
    cell.string = token_str;
    return cell;
  }
  
  const char* token=str;
  while((*str) && // NOT EOI
	// nor whitespace
	(*str != ' ') && (*str != '\t') && (*str != '\r') && (*str != '\n') &&
	// nor list end
        (*str != ')')){ 
    str++;
  }
 
  std::string token_str(token, str-token);
  
  if(std::regex_match(token_str, int_re)){
    cell.type =Type::Integer;
    cell.integer = stoll(token_str);
    return cell;
  }
  if(std::regex_match(token_str, hex_re)){
    cell.type =Type::Integer;
    cell.integer = stoll(token_str,nullptr,16);
    return cell;
  }
  if(std::regex_match(token_str, float_re)){
    cell.type =Type::Floating;
    cell.floating = stod(token_str);
    return cell;
  }


  cell.type = Type::Symbol;
  cell.string = token_str;
  return cell; 
}

//--------8<--------8<--------8<--------8<--------8<--------8<--------8<--------
// PRINTER

void print_cell(Cell cell)
{
  switch(cell.type){
  case Type::Integer:
    std::cout << cell.integer;
    break;
  case Type::Floating:
    std::cout << cell.floating;
    break;
  case Type::String:
    std::cout << cell.string;
    break;
  case Type::Symbol:
    std::cout << cell.string;
    break;
  case Type::List:
    std::cout << "(" << " ";
    for (auto item: cell.list)
      print_cell(item);
    std::cout << ")";
    break;
  }
  std::cout << " ";
}

//--------8<--------8<--------8<--------8<--------8<--------8<--------8<--------
// EVAL-APPLY
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

Cell dispatch_builtin(Cell oper, Cell args)
{
  //8<-------- ADDITION
  if (oper.string == "+") {
    Cell ret={};
    for (auto arg: args.list){
      switch (arg.type){
      case Type::Integer:
	ret.integer += arg.integer;
	break;
      case Type::Floating:
	ret.floating += arg.floating;
	break;
      default:
	std::cout << "Invalid type for + operator" << std::endl;
      }
    }
    if (ret.floating != 0) {
      ret.type = Type::Floating;
      ret.floating += ret.integer;
      return ret;
    } else {
      ret.type = Type::Integer;
      return ret;
    }
  }
  //8<-------- SUBSTRACTION
  if (oper.string == "-") {
    Cell ret={};
    switch (args.list.size()){
    case 0:
      ret.type = Type::Integer;
      ret.integer = 0;
      break;
    case 1:
      switch (args.list[0].type){
      case Type::Integer:
	ret.type = Type::Integer;
	ret.integer = - args.list[0].integer;
	break;
      case Type::Floating:
	ret.type = Type::Floating;
	ret.floating = - args.list[0].floating;
	break;
      default:
	std::cout << "Invalid type for - operator" << std::endl;
      }
      break;
    case 2:
      switch (args.list[0].type){
      case Type::Integer:
	switch (args.list[1].type) {
	case Type::Integer:
	  ret.type = Type::Integer;
	  ret.integer = args.list[0].integer - args.list[1].integer;
	  break;
	case Type::Floating:
	  ret.type = Type::Floating;
	  ret.floating = args.list[0].integer - args.list[1].floating;
	  break;
	default:
	  std::cout << "Invalid type for - operator" << std::endl;
	}
	break;
      case Type::Floating:
	switch (args.list[1].type) {
	case Type::Integer:
	  ret.type = Type::Floating;
	  ret.floating = args.list[0].floating - args.list[1].integer;
	  break;
	case Type::Floating:
	  ret.type = Type::Floating;
	  ret.floating = args.list[0].floating - args.list[1].floating;
	  break;
	default:
	  std::cout << "Invalid type for - operator" << std::endl;
	}
	break;
      default:
	std::cout << "Invalid type for - operator" << std::endl;
      }
      break;
    default:
      std::cout << "Invalid number of arguments for - operator" << std::endl;
    }
    return ret;
  } 
  //8<-------- MULTIPLICATION
  if (oper.string == "*") {
    Cell ret={};
    ret.integer = 1; ret.floating = 1;
    for (auto arg: args.list){
      switch (arg.type){
      case Type::Integer:
	ret.integer *= arg.integer;
	break;
      case Type::Floating:
	ret.floating *= arg.floating;
	break;
      default:
	std::cout << "Invalid type for * operator" << std::endl;
      }
    }
    if (ret.floating != 1) {
      ret.type = Type::Floating;
      ret.floating *= ret.integer;
    } else {
      ret.type = Type::Integer;
    }
    return ret;
  }
  //8<-------- DIVISION
  if (oper.string == "/") {
    Cell ret={};
    switch (args.list.size()){
    case 0:
      ret.type = Type::Integer;
      ret.integer = 0;
      break;
    case 1:
      ret.type = Type::Floating;
      switch (args.list[0].type){
      case Type::Integer:
	ret.floating = 1/ args.list[0].integer;
	break;
      case Type::Floating:
	ret.floating = 1/ args.list[0].floating;
	break;
      default:
	std::cout << "Invalid type for / operator" << std::endl;
      }
      break;
    case 2:
      ret.type = Type::Floating;
      switch (args.list[0].type){
      case Type::Integer:
	switch (args.list[1].type) {
	case Type::Integer:
	  ret.floating = args.list[0].integer / (float) args.list[1].integer;
	  break;
	case Type::Floating:
	  ret.floating = args.list[0].integer / args.list[1].floating;
	  break;
	default:
	  std::cout << "Invalid type for / operator" << std::endl;
	}
	break;
      case Type::Floating:
	switch (args.list[1].type) {
	case Type::Integer:
	  ret.floating = args.list[0].floating / args.list[1].integer;
	  break;
	case Type::Floating:
	  ret.floating = args.list[0].floating / args.list[1].floating;
	  break;
	default:
	  std::cout << "Invalid type for / operator" << std::endl;
	}
	break;
      default:
	std::cout << "Invalid type for / operator" << std::endl;
      }
      break;
    default:
      std::cout << "Invalid number of arguments for / operator" << std::endl;
    }
    return ret;
  }
  //8<-------- GREATER THAN
  if (oper.string == "+") {
    Cell ret={};
    for (auto arg: args.list){
      switch (arg.type){
      case Type::Integer:
	ret.integer += arg.integer;
	break;
      case Type::Floating:
	ret.floating += arg.floating;
	break;
      default:
	std::cout << "Invalid type for + operator" << std::endl;
      }
    }
    if (ret.floating != 0) {
      ret.type = Type::Floating;
      ret.floating += ret.integer;
      return ret;
    } else {
      ret.type = Type::Integer;
      return ret;
    }
  }
  if (oper.string == "+") {
    Cell ret={};
    for (auto arg: args.list){
      switch (arg.type){
      case Type::Integer:
	ret.integer += arg.integer;
	break;
      case Type::Floating:
	ret.floating += arg.floating;
	break;
      default:
	std::cout << "Invalid type for + operator" << std::endl;
      }
    }
    if (ret.floating != 0) {
      ret.type = Type::Floating;
      ret.floating += ret.integer;
      return ret;
    } else {
      ret.type = Type::Integer;
      return ret;
    }
  }
  if (oper.string == "+") {
    Cell ret={};
    for (auto arg: args.list){
      switch (arg.type){
      case Type::Integer:
	ret.integer += arg.integer;
	break;
      case Type::Floating:
	ret.floating += arg.floating;
	break;
      default:
	std::cout << "Invalid type for + operator" << std::endl;
      }
    }
    if (ret.floating != 0) {
      ret.type = Type::Floating;
      ret.floating += ret.integer;
      return ret;
    } else {
      ret.type = Type::Integer;
      return ret;
    }
  }
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
// REPL
int main(int argc, char *argv[])
{
  std::string input;
  while (true) {
    std::cout << "Lisp> ";
    std::getline(std::cin, input);
    if (input == "exit") {
      break;
    }

    read(input.c_str());
    Cell cell;
    do{
      cell = read(NULL);
      print_cell(eval(cell, environment));
      std::cout << std::endl;
    } while( (cell.type != Type::Symbol) || (cell.string != "") );
  }
  return 0;
}
