/*
 *  
 *  Copyright(C) 2018 Gerald Coe, Devantech Ltd <gerry@devantech.co.uk>
 * 
 *  Permission to use, copy, modify, and/or distribute this software for any purpose with or
 *  without fee is hereby granted, provided that the above copyright notice and 
 *  this permission notice appear in all copies.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
 *  THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. 
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL 
 *  DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 *  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 */
#ifndef ASSEMBLE_HPP
#define ASSEMBLE_HPP

#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <regex>

#include "string_utils.hpp"
#include "macro.hpp"
#include "symbols.hpp"
#include "token_list.hpp"
#include "options.hpp"
#include "asm_data.hpp"
#include "assembler_state.hpp"
#include "data_type.hpp"

/*
Assemble

This class handles the assembling of thecode file into an output that can be used
by the Avalanche processor.
*/
class Assemble
{
private:
  std::ifstream &file;

  /*
  A list of symbols that have been defined in the program
  */
  SymbolList symbol_list;

  /*
  A loist of tokens that have been found on the current line
  */
  TokenList token_list;

  /*
  Preprocess the assembly file. During preprocessing the following happens:

   - All defined registers are found and assigned their space in memory. 
   - Any constant values defined are stored ready for use
   - The memory location of any labels are stored
   - the starting memory locations of any processes are stored
   - The number of processes defined is checked (must be >= 7)

  preprocessisng happens line by line.
  */
  void preprocess();

  /*
  Assemble the instructions in a file. During assembly the following happens:

   - All instructions are converted into byte codes that run on the processor

  Assembly happens line by line
  */
  void assemble();

  /*
  Preprocess the curent line of the assembly file being processed
  */
  void preprocessLine();

  /*
  Assemble the current line of the assembly file being processed
  */
  void assembleLine();

  /*
  Creates a lable in the symbol table for this assembler.
  The label symbol will hold the memory location of the program counter 
  when this label was found allowing this location to be referenced later.
  */
  void doLabel(Token &label);

  /*
  Create a process.
  Stores the name of the process in the compiler state and then sets the in_process flag to true.
  then stores the program counter location where the process was started in the pc_list so that
  the processor can tell where the process started.
  */
  void doProcess();

  /*
  Clears the name of the current process and in_process flag
  */
  void doEndProcess();

  /*
  Created a symbol that has been declared in the code.
  This could be .reg, .data or .const
  The symbol is stored in this assemblers SymbolTable

  int type    - The type of symbol we are defining
  */
  void doSymbol(int type);

  /**
  Gets the token that represents a value that a .reg, .const or .data will be
  set to. This can be a numeric value, string or size of an array depending on
  the type of data being set.
  
  int type     - The type of the symbol we are creating 
  Token &v     - A pointer to a token to store the value
  
  returns 1 on success.
 */
  int getSymbolValueToken(int type, Token &v);

  /*
  Sets the value of a symbol and the size fo the data being created.

  returns 1 on success;
  */
  int setSymbolValueAndSize(int &val, int &size, Token &v);

  /*
  Sets the location inmemory where the symbol is placed. For a constnat this value is irrelevant.
  For .data types the location of the data_counter is stored as the location and then incremented by 
  the datas size. For .reg elements thee data register_counter is stored and then incremented by 1.
  This will set the error flag to 1 if the register memory is too full.
  
  int &location   - pointer to an int where thelocation will be stored
  int size        - the size of the symbol
  int type        - the type of symbol being assigned
  */
  int setSymbolLocation(int &location, int size, int type);

  /*
  Add the symbol to the assembly listing
  */
  int addSymbolToListing(int type, int size, int location, Token &v);

  void doInstruction(Token &ins);
  int getDestinationRegister(Token &dest, Symbol &sym);
  int getInstructionRegisters(Token &rs1, Token &rs2, int ins);
  int appendRegisterValue(Token &t, std::string &line, bool indirect);
  int buildInstructionString(std::string &line, Token &dest, Token &rs1, Token &rs2, int ins);

  void doMacro(Token &tok);

public:
  /*
  The data that the assembler will produce
  */
  AsmData data;

  /*
  Stores the current state of this assembler
  */
  AssemblerState state;

  Assemble(std::ifstream &f) : file(f)
  {
    int line_count = 0;
    std::string unused;
    while (std::getline(file, unused))
      ++line_count;
    file.clear();
    file.seekg(0, file.beg);
    data.setListingLength(line_count);
  };

  /*
  Assemble the file
  */
  int go();
};

#endif