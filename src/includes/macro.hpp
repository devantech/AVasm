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

void macroSub();
void macroDjnz();
void macroJmp();
void macroCall();
void macroRet();
void macroSll();
void macroInc();
void macroDec();
void macroLd();
void macroSt();
void macroMov();
void macroJler();
void macroJgtr();
void macroJgtr(int command);
void macroJeq(int command);
void macroJbs(int commnad);
void macroJle(int command);

} // namespace instructions

#endif