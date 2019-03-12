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
#ifndef TOKEN_LIST_HPP
#define TOKEN_LIST_HPP


#include "token.hpp"
#include <sstream>
/*
TokenList

A wrapper arround a std::vector<t> that provides functionality for
checking Token types when needed.

Passing a string into the getAllTokens(std::string line) method will 
populate the vector that this object contains with tokens found in that 
string. Calling get next will then return the next token in the list
for ecaluation. This object maintains a pointer to the current token 
being checked so that the user does not need to remember the location
of the token being checked.

A function expect(int &err, ArgsT ...types) is provided for checking 
if the token being fetched from the list matches the expected type 
that you are looking for.
*/
class TokenList
{

    /*
    Holds the tokens that are found in the current text that has been
    passed into void getAllTokens(const std::string line)
    */
    std::vector<Token> tokens;

    /*
    Itterator that points to the current token in this list
    */
    std::vector<Token>::iterator current;

    

  public:
    /*
    Empty constructor allows for creation of an empty TokenList
    */
    TokenList()
    {
        current = tokens.begin();
    };

    /*
    Constructor that takes a vector of tokens as an argument. 
    A TokenList created this way will contain the contents of the passed in vector

    std::vector<Token> t - A vector containing tokens that you wish to be the content of this object
    */
    TokenList(std::vector<Token> t) : tokens(t)
    {
        current = tokens.begin();
    };

    /*
    Clears all tokens currently in this TokenList
    */
    void clear();

    /*
    Returns true if there are more tokens in this list to check
    */
    bool hasNext();

    /*
    Get the next token in this list. This does not check to see if there 
    is a token available before trying to access it.
    */
    Token getNext();

    /*
    Get the current token that we are checking
    */
    Token get();

    /*
    move the pointer to the current token back one place
    */
    void goBack();

    /*
    Takes a string in an an argument and finds all of the tokens that are in the string.
    Tokens found this way are added to this TokenList vector. Calling this method will cleat
    any tokens that are already in this list

    const std::string line      - A string that we wish to split into tokens.
    */
    void getAllTokens(const std::string line);

    /*
    Gets the next token from this list and checks the type of that token.
    If the token being checked matches the expected types passed in then 
    the token is returned. If the token does not match one of the types then
    then the error indicator err is set to 1 and the token is still returned.
    If is the callers job to check this error value to see if an expected token was found or not.

    int &err            - An int that indicates if a match was found or not 
    ArgsT... types      - list of expected types that the token can be
    */
    Token expect(int &err, std::initializer_list<int> types);
};

#endif