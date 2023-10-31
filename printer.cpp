#include <iostream>
#include "lispy.hpp"

//--------8<--------8<--------8<--------8<--------8<--------8<--------8<--------
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
