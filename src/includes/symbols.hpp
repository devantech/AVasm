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

/**
 * SymbolList
 * 
 * Maintains a list of all symbols that have been defined in the code and provides functionality to 
 * add new symbols and search for ones that exist.
 * 
 */
class SymbolList
{
private:
  /*
  A map that holds all of the symbols defined in the program linked to the symbol namespace
  */
  std::map<std::string, Symbol> symbol_table;

public:
  /*
  Adds a symbol to the list. If the symbol allready exists then an exception is thrown.

  const std::string &key          - The name of the symbol that we are adding to the list, this is th variable name in code.
  const Symbol &value    - The data that the symbol represents
  */
  void addSymbol(const std::string &key, const Symbol &value);

  /*
  Gets a symbol from this list. If the requested object does not exist in this list then an exception is thrown.

  const std::string &key          - The name of the symbol that we are looking for.
  */
  Symbol getSymbol(const std::string &key);

  /*
  Add a symbol to the symbol list. If the symbol is being declared or accessed inside of a 
  process then the process name will be appended to the front of the symbols name.

  If there is an error adding the symbol to the list then the error indicator err will be set to 1
  i t is the users responsibility to check this error value.

  int &err            - pointer to an int that will signify if there was an error or not
  int &type           - the type of the symbol that we are adding
  int &val            - the value associated withthte symbol
  int &size,          - the size of the symbol in memory
  int &location,      - the starting location of the symbol in memory
  std::string &iden   - the name of the symbol being set
  */
  void addSymbolToTable(int &err, int &type, int &val, int &size, int &location,
                        std::string &iden);

  /*
  Gets a symbol from this list. If there is a problen getting this symbol then the error indicator will be set to 1
  an empty symbol will be returned. It is the users responsibility to check this error value to validate the returned 
  symbol.

  This method first checks for a symbol that was declared local to a process and then if none is fount it then checks
  to see if a matching symbol was declared globally.

  int &err                        - pointer to an int that will represent if there has been an error fetching this symbol
  std::string iden                - The name of the symbol we are looking for
  bool &in_process                 - Signals if we are inside a process
  std::string &current_process    - the name of the process we are in
  */
  Symbol getSymbolFromTable(int &err, std::string & iden, bool & in_process, std::string &current_process);
};


#endif