#include <string>
#include <vector>
#include <map>

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

extern std::map<std::string, Cell> environment;

// EXPORTED FUNCTIONS
Cell read(const char* string);
Cell dispatch_builtin(Cell oper, Cell args);
Cell eval(Cell expr, std::map<std::string, Cell>& env);
void print_cell(Cell cell);
