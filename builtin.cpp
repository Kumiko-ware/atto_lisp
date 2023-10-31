#include <iostream>
#include "lispy.hpp"

// DEFAULT ENV
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
  if (oper.string == ">") {
    Cell ret={};
    ret.type = Type::Integer;
    ret.integer = 0;
    if (args.list.size() < 2){
      std::cout << "Too few arguments for > operator" << std::endl;
      return ret;
	}
    int last = args.list.size();
    for(int i=0; i<(last-1) ; i++) {
      switch (args.list[i].type){
      case Type::Integer:
	switch (args.list[i+1].type) {
	case Type::Integer:
	  if (! (args.list[i].integer > args.list[i+1].integer))
	    return ret;
	  break;
	case Type::Floating:
	  if (! (args.list[i].integer > args.list[i+1].floating))
	    return ret;
	  break;
	default:
	  std::cout << "Invalid type for > operator" << std::endl;
	}
	break;
      case Type::Floating:
	switch (args.list[1].type) {
	case Type::Integer:
	  if (! (args.list[i].floating > args.list[i+1].integer))
	    return ret;
	  break;
	case Type::Floating:
	  if (! (args.list[i].floating > args.list[i+1].floating))
	    return ret;
	  break;
	default:
	  std::cout << "Invalid type for > operator" << std::endl;
	}
	break;
      default:
	std::cout << "Invalid type for > operator" << std::endl;
      }
    }
    ret.integer = 1;
    return ret;
  }
  //8<-------- LESS THAN
  if (oper.string == "<") {
    Cell ret={};
    ret.type = Type::Integer;
    ret.integer = 0;
    if (args.list.size() < 2){
      std::cout << "Too few arguments for < operator" << std::endl;
      return ret;
	}
    int last = args.list.size();
    for(int i=0; i<(last-1) ; i++) {
      switch (args.list[i].type){
      case Type::Integer:
	switch (args.list[i+1].type) {
	case Type::Integer:
	  if (! (args.list[i].integer < args.list[i+1].integer))
	    return ret;
	  break;
	case Type::Floating:
	  if (! (args.list[i].integer < args.list[i+1].floating))
	    return ret;
	  break;
	default:
	  std::cout << "Invalid type for < operator" << std::endl;
	}
	break;
      case Type::Floating:
	switch (args.list[1].type) {
	case Type::Integer:
	  if (! (args.list[i].floating < args.list[i+1].integer))
	    return ret;
	  break;
	case Type::Floating:
	  if (! (args.list[i].floating < args.list[i+1].floating))
	    return ret;
	  break;
	default:
	  std::cout << "Invalid type for < operator" << std::endl;
	}
	break;
      default:
	std::cout << "Invalid type for < operator" << std::endl;
      }
    }
    ret.integer = 1;
    return ret;
  }
  //8<-------- EQUAL
  if (oper.string == "=") {
    Cell ret={};
    ret.type = Type::Integer;
    ret.integer = 0;
    if (args.list.size() < 2){
      std::cout << "Too few arguments for = operator" << std::endl;
      return ret;
	}
    int last = args.list.size();
    for(int i=0; i<(last-1) ; i++) {
      switch (args.list[i].type){
      case Type::Integer:
	switch (args.list[i+1].type) {
	case Type::Integer:
	  if (! (args.list[i].integer == args.list[i+1].integer))
	    return ret;
	  break;
	case Type::Floating:
	  if (! (args.list[i].integer == args.list[i+1].floating))
	    return ret;
	  break;
	default:
	  std::cout << "Invalid type for = operator" << std::endl;
	}
	break;
      case Type::Floating:
	switch (args.list[1].type) {
	case Type::Integer:
	  if (! (args.list[i].floating == args.list[i+1].integer))
	    return ret;
	  break;
	case Type::Floating:
	  if (! (args.list[i].floating == args.list[i+1].floating))
	    return ret;
	  break;
	default:
	  std::cout << "Invalid type for = operator" << std::endl;
	}
	break;
      default:
	std::cout << "Invalid type for = operator" << std::endl;
      }
    }
    ret.integer = 1;
    return ret;
  }
}
//--------8<--------8<--------8<--------8<--------8<--------8<--------8<--------
