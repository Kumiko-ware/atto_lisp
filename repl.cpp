#include <iostream>
#include "lispy.hpp"

//--------8<--------8<--------8<--------8<--------8<--------8<--------8<--------
// REPL
int main(int argc, char *argv[])
{
  std::string input;
  while (true) {
    std::cout << ":) ";
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
