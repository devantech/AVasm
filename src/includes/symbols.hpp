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
#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP

#include <string>
#include <map>

/*
The types of symbol thata symbol cna be
*/
enum symbol_types
{
  SYMBOL_DATA,
  SYMBOL_CONST,
  SYMBOL_REG,
  SYMBOL_LABEL
};

/*
Symbol

A symbol that has been found in the program being assembles. This may be a .data, .const, .reg or a label that has been defined.
Holds the location in memory that the symbol was created, the type of symbol that it is and the value associated with it.
*/
class Symbol
{
private:
  int typ = 0;
  int val = 0;
  int sze = 1;
  int loc = 0;

public:
  /*
  Constructor that created an empty symbol.
  */
  Symbol(){};

  /*
  Constructor that takes a type, value and location in as an argument and creates a symbol with these values

  int t     - the type of the symbol
  int v     - the value assigned to the symbol
  int l     - the location of the symbol
  */
  Symbol(int t, int v, int l) : typ(t), val(v), loc(l){};

  /*
  Constructor that takes a type, value, size and location in as an argument and creates a symbol with these values.

  int t     - the type of tis symbol 
  int v     - the the value assigned to this symbol
  int s     - the the size of this symbol in memory
  int l     - the the location this symbol was created at
  */
  Symbol(int t, int v, int s, int l) : typ(t), val(v), sze(s), loc(l){};

  /*
  returns tha location in memory that the symbol was created
  */
  int location();

  /*
  returns the value that was assigned to the symbol when it was created
  */
  int value();

  /*
  returns the type of this symbol
  */
  int type();
};




#endif