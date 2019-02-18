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
#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "asm_data.hpp"
#include "num_utils.hpp"
#include "string_utils.hpp"

namespace instructions
{

class Instruction
{
  protected:
    AsmData &data;
    AssemblerState &state;
    TokenList &token_list;
    SymbolList &symbol_list;

    /*
    the command value that this command uses
    e.g add uses 0
    */
    int command;

    /*
    Then if the next token is a symbol then get it from the symbol list.
    If the symbol is not set then the error flag is set.

    Symbol & sym     - Pointer to a symbol object that will be the found 
    */
    void getSymbol(Symbol &sym);

    /*
    Get a register from the symbols table.
    This method grabs the next token from the token list and then if it is an idetifier
    it grabs the appropriate symbol from the symbol list.

    Symbol & sym     - Pointer to a symbol object that will be the found register
    */
    void getReg(Symbol &sym);

    /*
    Checks to see if the next token is a comma.
    Returns 1 if it is and 0 if it is not.
    */
    int checkComma();

    /*
    Checks to see if there are more tokens available on the curent line.
    If there are then the error flag is set and a 0 is returned.
    If there are not then the error flag is not set and a 1 is returned
    */
    int checkForMore();

  public:
    Instruction(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : data(d), state(s), token_list(tl), symbol_list(sl){};

    /*
    Build the instruction
    */
    void build();
};

/****************************************************************
 * 
 * CLASS DEFINITIONS FOR INSTRUCTIONS
 * 
****************************************************************/

class ADD : public Instruction
{
  protected:
    /*
    The target for ALU operations can be an idirect value. In this case bit 8 of the 
    command byte is set to show that the target register is an indirection

    Symbol &sym        - pointer to a symbol that will store the register that was found
    */
    void getALUTarget(Symbol &sym);

    /*
    The target for ALU operations can be an idirect value. In this case bit 7 of the 
    command byte is set to show that the target register is an indirection

    Symbol &sym        - pointer to a symbol that will store the register that was found
    */
    void getALUReg(Symbol &sym);

  public:
    ADD(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : Instruction(d, s, tl, sl)
    {
        command = INSTRUCTION_ADD_VALUE;
    };
    void build();
};

class OR : public ADD
{
  public:
    OR(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : ADD(d, s, tl, sl)
    {
        command = INSTRUCTION_OR_VALUE;
    };
};

class AND : public ADD
{
  public:
    AND(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : ADD(d, s, tl, sl)
    {
        command = INSTRUCTION_AND_VALUE;
    };
};

class XOR : public ADD
{
  public:
    XOR(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : ADD(d, s, tl, sl)
    {
        command = INSTRUCTION_XOR_VALUE;
    };
};

class SRL : public ADD
{
  public:
    SRL(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : ADD(d, s, tl, sl)
    {
        command = INSTRUCTION_SRL_VALUE;
    };
    void build(); // Override the build function of ADD here because srl only has 2 registers passed
};

class LDI : public Instruction
{
  protected:
    /*
    An immediate is can either be a number, a numeric constant or a lable.

    returns the discovered immediate vlaue.
    */
    int getImmValue();

  public:
    LDI(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : Instruction(d, s, tl, sl)
    {
        command = INSTRUCTION_LDI_VALUE;
    };
    void build();
};

class JZ : public LDI
{
  public:
    JZ(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : LDI(d, s, tl, sl)
    {
        command = INSTRUCTION_JZ_VALUE;
    };
};

class JNZ : public LDI
{
  public:
    JNZ(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : LDI(d, s, tl, sl)
    {
        command = INSTRUCTION_JNZ_VALUE;
    };
};

class JAL : public LDI
{
  public:
    JAL(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : LDI(d, s, tl, sl)
    {
        command = INSTRUCTION_JAL_VALUE;
    };
};

class JALR : public Instruction
{
  public:
    JALR(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : Instruction(d, s, tl, sl)
    {
        command = INSTRUCTION_JALR_VALUE;
    };
    void build();
};

class JEQR : public Instruction
{
  public:
    JEQR(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : Instruction(d, s, tl, sl)
    {
        command = INSTRUCTION_JEQR_VALUE;
    };
    void build();
};

class JNER : public JEQR
{
  public:
    JNER(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : JEQR(d, s, tl, sl)
    {
        command = INSTRUCTION_JNER_VALUE;
    };
};

class JLTR : public JEQR
{
  public:
    JLTR(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : JEQR(d, s, tl, sl)
    {
        command = INSTRUCTION_JLTR_VALUE;
    };
};

class JGER : public JEQR
{
  public:
    JGER(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : JEQR(d, s, tl, sl)
    {
        command = INSTRUCTION_JGER_VALUE;
    };
};

class JBSR : public LDI
{
  public:
    JBSR(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : LDI(d, s, tl, sl)
    {
        command = INSTRUCTION_JBSR_VALUE;
    };
    void build();
};

class JBCR : public JBSR
{
  public:
    JBCR(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : JBSR(d, s, tl, sl)
    {
        command = INSTRUCTION_JBCR_VALUE;
    };
};

} // namespace instructions
#endif