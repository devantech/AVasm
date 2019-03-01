#ifndef DATA_HPP
#define DATA_HPP

#include "symbol_list.hpp"
#include "token_list.hpp"
#include "assembler_state.hpp"
#include "asm_data.hpp"

namespace data
{

/*
  The data that the assembler will produce
  */
extern AsmData data;

/*
  Stores the current state of this assembler
  */
extern AssemblerState state;

/*
  A list of symbols that have been defined in the program
  */
extern SymbolList symbol_list;

/*
  A loist of tokens that have been found on the current line
  */
extern TokenList token_list;

void setError(std::string s);

void printError();

void log(std::string s);

void logMacro(std::string s);

} // namespace data

#endif