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
#include "token_list.hpp"

bool TokenList::hasNext(void)
{
    if (current != tokens.end())
    {
        return true;
    }
    return false;
}

Token TokenList::getNext(void)
{
    auto t = *current;  // The token is grabbed before itterating the pointer
    ++current;          // This is soa call to getNext will return the first 
    return t;           // Token in the list on a freshly created object
}

Token TokenList::get(void)
{
    return *(current - 1);
}

void TokenList::clear()
{
    tokens.clear();
    current = tokens.begin();
    
}

void TokenList::getAllTokens(const std::string line)
{
    clear();
    // https://regex101.com/r/aF8RHP/7/
    // "[^\\s\"\',]+|\"([^\"]*)\"|\'([^\']*)\'|,"
    // std::regex r("\\[([\\s\\S]*)\\]|(-[0-9]+)|[^-\\s\"\',\\+\\*\\/]+|\"([^\"]*)\"|\'([^\']*)\'|[-,\\+\\*\\/]"); // Get all words but grouping
    std::regex r("(\\[(.*)\\])|(\"(.*)\")|(\'(.*)\')|([^-,\\+\\*\\/\\s]+)|([-,\\+\\/\\*])|([\\s][-,\\+\\/\\*][\\d]?[x]?[\\d]*)"); 
    // Get all words but grouping
                                                     // words enclosed by " or '
                                                     // marks and grab commas
                                                     // seperatly.
    
    for (std::sregex_iterator i =
             std::sregex_iterator(line.begin(), line.end(), r);
         i != std::sregex_iterator(); ++i)
    {
        std::smatch m = *i;
        Token t(m.str());
        tokens.push_back(t);
    }
    current = tokens.begin();
}

Token TokenList::expect(int & err, std::initializer_list<int> types)
{
    if (!hasNext())
    {
        err = 1; 
        return Token();
    }
    Token t = getNext();
    if (std::find(types.begin(), types.end(), t.type) != types.end())
    {
        return t;
    }
    err = 1;
    return t;
}
