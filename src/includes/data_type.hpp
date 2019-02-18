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
#ifndef DATA_TYPE_HPP
#define DATA_TYPE_HPP

#include "asm_data.hpp"
#include "num_utils.hpp"
#include "string_utils.hpp"

/*
Bass class for defining a data type that can be used in the program.
provides methods for getting data that sets the name and value of a data tyep.
*/
class Type
{
protected:
  AsmData &data;
  AssemblerState &state;
  TokenList &token_list;
  SymbolList &symbol_list;

  /*
  Get the identifier that will be the name of this data.
  The name will be prepended with the process name that it was defined 
  if it was defined inside of a process.

  returns 1 on finding a valid identifier and 0 on failure.

  Token &t  - a pointer to a taken object where the identifier will be stored
  */
  int getIdentifier(Token &t);

  /*
  Gets the that this data will hold.

  Returns 1 on finding a valid value and 0 on failure

  int &val  - pointer to an integer that will hold the value found.
  */
  int getValue(int &val);

  /*
  Checks to see if there are any more tokens in the token list to handle

  returns 1 if there are more tokens and 0 if there are not
  */
  int checkForMore(void);

  /*
  Types can be operated on when defined. This method looks for a
  valid mathematical operator followed by a vlaue 
  e.g.  const_value + 10
        const_value - const_vlaue2

  returns 1 on success and 0 on a fail.

  int &val    - a poitner to an int that will be used to store the value that was found
  */
  int getOperator(int &val);

public:
  Type(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : data(d), state(s), symbol_list(sl), token_list(tl){};

  /*
  Cretes the data.

  Gets the identifier for this data and the value associated with it and then stores this in the symbols table
  */
  void create();
};

/*
ConstType

This is data thatcan be defined in assembly that represents a value that needs to be reused often.
Const data types must be declared globally.
*/
class ConstType : public Type
{
  

public:
  ConstType(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : Type(d, s, tl, sl){};
  void create();
};

/*
DataType

Defines a .data register to be used by the processor
*/
class DataType : public Type
{
  /*
  .data types can be assigned as:
      - a numeric value
      - an array with a size
      - a string value
  As a .data type is not of a fixed size this method is used to get either the value
  of the data or the size of the data being defined.

  returns 1 on success and 0 on a fail.

  int &size     - a pointer to an int that will store the size of the data being set
  int &val      - a pointer to an int that will store the value of the data being set
  Token &tok    - a poitner to a token object that will store the value token that was found
                  important if the value is a string as we will need to put this data into memory
  */
  int getSizeAndValue(int &size, int &val, Token &tok);

public:
  DataType(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : Type(d, s, tl, sl){};
  void create();
};

/*
RegType

Defines a .reg value to be used by the processor
*/
class RegType : public Type
{

public:
  RegType(AsmData &d, AssemblerState &s, TokenList &tl, SymbolList &sl) : Type(d, s, tl, sl){};
  void create();
};

#endif
