#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "reader.h"
#include "memory.h"

int char_in(char ch, char const *chars)
{
  while (*chars){
    if (ch == *chars++)
      return 1;
  }
  return 0;
}

int is_blank(char ch)
{
  return(char_in(ch, " \t\r\n"));
}

int is_newline(char ch)
{
  return(char_in(ch, "\r\n"));
}

int is_sep(char ch)
{
  return(char_in(ch, " \t\r\n;()\""));
}

int is_digit(char ch)
{
  return(char_in(ch, "0123456789"));
}

int is_eos(char ch)
{
  return !ch;
}

int get_token(value *val, char const *str)
{
  static int pos = 0;
  static char const *last_str = 0;
    
  if (str != last_str){
    pos = 0;
    last_str = str;
  }
  int start_pos;
  do{
    start_pos = pos;
    while(is_blank(str[pos]))
      ++pos; // skip whitespace
    if (str[pos] == ';'){
      while(!is_newline(str[pos]) && !is_eos(str[pos]))
	++pos; // skip comment
      if(!is_eos(str[pos]))
	++pos; // skip end line
    }
  } while (pos != start_pos); // do until no more blanks or comments found
  if (is_eos(str[pos]))
    return TOK_EOF;
  start_pos = pos;
  if (str[pos] == '('){
    ++pos;
    return TOK_OPN;
  }
  if (str[pos] == ')') {
    ++pos;
    return TOK_CLS;
  }
  if (str[pos] == '\'') {
    ++pos;
    return TOK_QOT;
  }
  if (str[pos] == '`') {
    ++pos;
    return TOK_QQT;
  }
  if (str[pos] == ',') {
    ++pos;
    if (str[pos] == '@'){
      ++pos;
      return TOK_UQS;
    }
    return TOK_UNQ;
  }
  if ((str[pos] == '.') && is_sep(str[pos+1])){
    ++pos;
    return TOK_DOT;
  }
  if (str[pos] == '"'){
    while(1){
      ++pos;
      if (str[pos] == '"'){
	val->str_value = str + start_pos +1;
	val->int_value = pos - start_pos -1;
	++pos;
	return TOK_STR;
      }
      if (str[pos] == '\\'){
	++pos; // skip scape char
	if (!is_eos(str[pos]))
	  ++pos; // skip scaped char (if not \0)
      }
      if (str[pos] == '\0')
	return TOK_EOF_ERR;
    }
  }
  // TODO: If floats are wanted, here we should call to a function
  // "parse_num(str+pos, &data)", which would be basically the FSM
  // implementation of the RE for any number, like for example:
  // "[-+]?((\d*\.)?\d+([eE][-+]?\d+)?|Inf|NaN)", and also radix #b,
  // #x, #d, and give back if a number is present, what type is it,
  // etc.
  //
  // Right now we stick to decimal integers
  if (char_in(str[pos], "+-"))
    ++pos;
  while(is_digit(str[pos])){
    ++pos;
    if (is_sep(str[pos]) || is_eos(str[pos])){
      val->int_value = strtol(str + start_pos, NULL, 10);
      return TOK_INT;
    }
  }
  while(!is_sep(str[pos]) && !is_eos(str[pos]))
    ++pos; // consume all until next separator or EOS
  val->str_value = str + start_pos;
  val->int_value = pos - start_pos;
  return TOK_SYM;
}

int read(cell *exp, const char* str)
{
  cell first, second, last;
  value val;
  int ret;
  int tok = get_token(&val, str);
  switch(tok){
  case TOK_EOF_ERR:
    return READ_ERR_EOF;
    break;
  case TOK_EOF:
    return READ_ERR_EOF;
    break;
  case TOK_OPN:
    ret = read(&first, str);
    if (ret == READ_ERR_CLS){
      *exp = EMPTY_LIST;
      return READ_EXP;
    }
    if (ret == READ_ERR_DOT){
      *exp = EMPTY_LIST;
      return READ_BAD_LST;
    }
    *exp = cons(first, EMPTY_LIST);
    last = *exp;
    while (ret == READ_EXP){
      ret = read(&second, str);
      if (ret == READ_EXP){
	first = cons(second, EMPTY_LIST);
	set_cdr(last, first);
	last = first;
      }
      }
    if (ret == READ_ERR_CLS)
      return READ_EXP;
    if (ret == READ_ERR_DOT){
      ret = read(&second, str);
      if (ret == READ_EXP){
	set_cdr(last, second);
	return READ_EXP;
      }
    }
    *exp = EMPTY_LIST;
    return READ_ERR_DOT;
    break;
  case TOK_CLS:
    return READ_ERR_CLS;
    break;
  case TOK_QOT:
    first = make_symbol("QUOTE");
    ret = read(&second, str);
    if (ret == READ_EXP){
      *exp = cons(first, cons(second, EMPTY_LIST));
      return READ_EXP;
    }
    return READ_ERR_QUO;
    break;
  case TOK_QQT:
    first = make_symbol("QUASI-QUOTE");
    ret = read(&second, str);
    if (ret == READ_EXP){
      *exp = cons(first, cons(second, EMPTY_LIST));
      return READ_EXP;
    }
    return READ_ERR_QUO;
    break;
  case TOK_UNQ:
    first = make_symbol("UNQUOTE");
    ret = read(&second, str);
    if (ret == READ_EXP){
      *exp = cons(first, cons(second, EMPTY_LIST));
      return READ_EXP;
    }
    return READ_ERR_QUO;
    break;
  case TOK_UQS:
    first = make_symbol("UNQUOTE-SPLICING");
    ret = read(&second, str);
    if (ret == READ_EXP){
      *exp = cons(first, cons(second, EMPTY_LIST));
      return READ_EXP;
    }
    return READ_ERR_QUO;
    break;
  case TOK_DOT:
    return READ_ERR_DOT;
    break;
  case TOK_INT:
    *exp = make_number(val.int_value);
    return READ_EXP;
    break;
  case TOK_STR:
    *exp = make_string(strndup(val.str_value,val.int_value));
    //TODO: make_string should not need strndup!
    return READ_EXP;
    break;
  case TOK_SYM:
    //TODO: make_string should not need strndup!
    *exp = make_symbol(strndup(val.str_value,val.int_value));
    return READ_EXP;
    break;
  default:
    assert("Unknown token from tokenizer");
  }
}
