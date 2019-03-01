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

#include "symbols.hpp"
#include "token.hpp"

namespace instructions
{

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

/*
    The target for ALU operations can be an idirect value. In this case bit 8 of the 
    command byte is set to show that the target register is an indirection

    Symbol &sym        - pointer to a symbol that will store the register that was found
    */
void getALUTarget(Symbol &sym, int & command);

/*
    The target for ALU operations can be an idirect value. In this case bit 7 of the 
    command byte is set to show that the target register is an indirection

    Symbol &sym        - pointer to a symbol that will store the register that was found
    */
void getALUReg(Symbol &sym, int & command);

/*
    An immediate is can either be a number, a numeric constant or a lable.

    returns the discovered immediate vlaue.
    */
int getImmValue();

void createALUInstruction(int command);

void createSRLInstruction(int command);

void createLDIInstruction(int command);

void createJALRInstruction(int command);

void createJEQRInstruction(int command);

void createJBSRInstruction(int command);

} // namespace instructions
#endif