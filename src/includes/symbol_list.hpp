#ifndef SYMBOL_LIST_HPP
#define SYMBOL_LIST_HPP

#include "symbols.hpp"

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