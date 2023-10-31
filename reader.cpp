#include <iostream>
#include <regex>
#include "lispy.hpp"

//--------8<--------8<--------8<--------8<--------8<--------8<--------8<--------
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

//--------8<--------8<--------8<--------8<--------8<--------8<--------8<--------
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
