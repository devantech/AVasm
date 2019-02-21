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
#ifndef MACRO_HPP
#define MACRO_HPP

#include "asm_data.hpp"
#include "assembler_state.hpp"
#include "token_list.hpp"
#include "symbols.hpp"
#include "string_utils.hpp"
#include "instruction.hpp"

namespace instructions
{

const std::string COMMAND_EXPANDER = "<________|";

/*
Returns the length in instructions of the requested macro.
If the string passed in is not a valid macro then returns 0.
User should check return byte to validate input.
*/
int getLengthOfMacro(std::string m);

/*
MacroCommand

The base class for all macro commands that are available.
A macro command is any command that is typed in the file as one instruction but 
when built is a confluence of more than one instruction in the program.
*/
class MacroCommand : public Instruction
{
protected:
  /*
  Get a label from the symbols table.
  This method grabs the next token from the token list and then if it is an idetifier
  it grabs the appropriate symbol from the symbol list.

  Symbol & sym     - Pointer to a symbol object that will be the found register
  */
  void getLabel(Symbol &sym);

  /*
  Some macros require an extra register to function, this method creates that register if 
  it does not already exist and then fetches it. The register being created has a suffix
  appended to its name incase a macro needs more than one extra register to function
  The registers name is in the format: 
      "___macro" + state.process_name + suffix + "__"

  Symbol &sym      - Pointer to a symbol object that will be set to the register found
  std::string suffix        - the suffix to be applied to the register name
  */
  void getMacroRegister(Symbol &sym, std::string suffix);

  /*
  checks to see if the next token is a register or an indirection. This register is tored in a symbol 
  passed into this function. If the found register is an indirection then this function returns true
  otherwise it retuurns false. If the token is not or a register or indirection type thenthe error status 
  indicator is set to 1 and an error message generated.

  Symbol & sym  - a pointer to a Smbol where the found register is to be stored.
  */
  bool getPossibleIndirectReg(Symbol &sym);

public:
  /*
  Constructor that takes in all of the apropriate objects for a command to be processed

  AsmData &d                - The asm data that the command will be placed into
  AssemblerState &s         - The current state of the assembler
  TokenList &tl             - The tokens that make up the line being processed
  SymbolList &sl   - Symbol list that holds all  of the symbols found in the program
  int len                   - The length of this macto command in instructions
  */
  MacroCommand(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : Instruction(d, s, tl, sl){};

  /*
  Builds the macro command and places appropriate data in the output files
  Base class implementation is empty so that is has no effect if it is accidentaly called.
  */
  void build(){};
};

/****************************************************************
 * 
 * CLASS DEFINITIONS FOR MACRO INSTRUCTIONS
 * 
****************************************************************/

class Sub : public MacroCommand
{
public:
  Sub(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl){};
  void build();
};

class DJNZ : public MacroCommand
{
public:
  DJNZ(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl){};
  void build();
};

class JMP : public MacroCommand
{
public:
  JMP(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl){};
  void build();
};

class Call : public MacroCommand
{
public:
  Call(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl){};
  void build();
};

class RET : public MacroCommand
{
public:
  RET(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl){};
  void build();
};

class SLL : public MacroCommand
{
public:
  SLL(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl){};
  void build();
};

class INC : public MacroCommand
{
public:
  INC(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl){};
  void build();
};

class DEC : public MacroCommand
{
public:
  DEC(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl){};
  void build();
};

class JEQ : public MacroCommand
{
protected:
  int command;

public:
  JEQ(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl)
  {
    command = INSTRUCTION_JEQR_VALUE;
  };
  void build();
};

class JNE : public JEQ
{

public:
  JNE(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : JEQ(d, s, tl, sl)
  {
    command = INSTRUCTION_JNER_VALUE;
  };
};

class JLER : public JEQ
{

public:
  JLER(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : JEQ(d, s, tl, sl)
  {
    command = INSTRUCTION_JLTR_VALUE;
  };
  void build(void);
};

class JGTR : public JEQ
{

public:
  JGTR(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : JEQ(d, s, tl, sl)
  {
    command = INSTRUCTION_JGER_VALUE;
  };
  void build(void);
};

class JLT : public JEQ
{
public:
  JLT(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : JEQ(d, s, tl, sl)
  {
    command = INSTRUCTION_JLTR_VALUE;
  };
};

class JGE : public JEQ
{
public:
  JGE(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : JEQ(d, s, tl, sl)
  {
    command = INSTRUCTION_JGER_VALUE;
  };
};

class JBS : public MacroCommand
{
protected:
  int command;
  int getImmValue(void);

public:
  JBS(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl)
  {
    command = INSTRUCTION_JBSR_VALUE;
  };
  void build();
};

class JBC : public JBS
{
public:
  JBC(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : JBS(d, s, tl, sl)
  {
    command = INSTRUCTION_JBCR_VALUE;
  };
};

class JLE : public MacroCommand
{
public:
  JLE(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl)
  {
    command = INSTRUCTION_JGER_VALUE;
  };
  void build();
};

class JGT : public JLE
{
public:
  JGT(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : JLE(d, s, tl, sl)
  {
    command = INSTRUCTION_JLTR_VALUE;
  };
};

class LD : public MacroCommand
{
  public:
    LD(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl)
    {
      command = INSTRUCTION_ADD_VALUE;
    }
    void build();
};

class ST : public MacroCommand
{
  public:
    ST(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl)
    {
      command = INSTRUCTION_ADD_VALUE;
    }
    void build();
};

class MOV : public MacroCommand
{
  public:
    MOV(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : MacroCommand(d, s, tl, sl)
    {
      command = INSTRUCTION_ADD_VALUE;
    }
    void build();
};

} // namespace instructions

#endif