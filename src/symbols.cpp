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
#include "symbols.hpp"
#include "symbol_list.hpp"

int Symbol::type(void)
{
    return typ;
}

int Symbol::value(void)
{
    return val;
}

int Symbol::location(void)
{
    return loc;
}

void SymbolList::addSymbol(const std::string &key, const Symbol &value)
{
    if (key.empty())
    {
        throw std::invalid_argument("Can not add an empty key");
    }
    if (symbol_table.find(key) == symbol_table.end())
    {
        symbol_table.insert(std::pair<std::string, Symbol>(key, value));
    }
    else
    {
        throw std::invalid_argument("allready defined in this scope --> ");
    }
}

Symbol SymbolList::getSymbol(const std::string &key)
{
    if (symbol_table.find(key) != symbol_table.end())
    {
        return symbol_table.at(key);
    }
    throw std::invalid_argument("does not exist in this scope --> ");
}

void SymbolList::addSymbolToTable(int &err, int &type, int &val, int &size, int &location,
                                           std::string &iden)
{
    try
    {
        Symbol sy(type, val, size, location);
        addSymbol(iden, sy);
    }
    catch (std::invalid_argument &e)
    {
        throw;
    }
}

Symbol SymbolList::getSymbolFromTable(int &err, std::string & iden, bool & in_process, std::string &current_process)
{
    Symbol s_sym;
    
    if (in_process)
    {
        try
        {   
            std::string s = current_process + iden;
            s_sym = getSymbol(s);
            return s_sym;
        }
        catch (std::invalid_argument &e)
        {
            // Do nothing here as we will next look for a global as this local does not exist.
        }
    }
    try
    {
        s_sym = getSymbol(iden);
        return s_sym;
    }
    catch (std::invalid_argument &e)
    {
        err = 1;
        return Symbol();
    }
}