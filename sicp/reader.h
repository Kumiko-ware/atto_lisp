#ifndef READER_H_INCLUDED
#define READER_H_INCLUDED

#include "memory.h"

typedef struct{
  int int_value;
  char const *str_value;
} value;

enum TOKENS {
  TOK_EOF_ERR = -1,
  TOK_EOF, // End of file
  TOK_OPN, // Open parenthesis
  TOK_CLS, // Close parenthesis
  TOK_QOT, // Quote
  TOK_QQT, // Quasiquote
  TOK_UNQ, // Unquote
  TOK_UQS, // Unquote slicing
  TOK_DOT, // Dot (for pair notation)
  TOK_INT, // Integer
  TOK_STR, // String
  TOK_SYM}; // Symbol

enum READ_VAL {
  READ_EXP,
  READ_ERR_EOF,
  READ_ERR_CLS,
  READ_ERR_DOT,
  READ_ERR_QUO,
  READ_BAD_LST
};

int get_token(value *val, char const *str);
int read(cell* val, const char* str);
  
#endif //READER_H_INCLUDED
