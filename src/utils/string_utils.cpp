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
#include "string_utils.hpp"
#include "token.hpp"
#include <sstream>
#include <iostream>

/**
 * Calculate the base of a string representation of a number.
 * Will not throw error for a badly formatted number, just checks the prefix and returns the base.
 */
int stutils::calculateBase(const std::string &st)
{
    auto base = 10;

    if (startsWith(st, "0x") || startsWith(st, "0X"))
    {
        base = 16;
    }
    else if (startsWith(st, "0"))
    {
        base = 8;
    }
    return base;
}

/**
 * Returns true if mainStr starts with toMatch
 */
bool stutils::startsWith(const std::string &mainStr, const std::string &toMatch)
{
    if (mainStr.find(toMatch) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Removes a commant from the line of text passed in
 */
std::string stutils::stripComment(std::string st)
{
    std::string::size_type pos;
    pos = st.find(";");
    st.erase(pos == std::string::npos ? st.size() : pos);
    return st;
}

/**
 * Returns a string representation of the token type that was found, basically ease of reading on the output
 */
std::string stutils::tokenType(int t)
{
    switch (t)
    {
    case NONE:
        return "NONE";
    case IDENTIFIER:
        return "IDENTIFIER";
    case INSTRUCTION:
        return "INSTRUCTION";
    case COMMA:
        return "COMMA";
    case PROCESS:
        return "PROCESS";
    case ENDPROCESS:
        return "ENDPROCESS";
    case REGISTER:
        return "REGISTER";
    case DATA:
        return "DATA";
    case CONST:
        return "CONST";
    case MACRO:
        return "MACRO";
    case NUMBER:
        return "NUMBER";
    case INDIRECTION:
        return "INDIRECTION";
    case SIZE:
        return "SIZE";
    case STRING:
        return "STRING";
    default:
        return "NONE";
    }
}

std::string stutils::getTopProcessFromSplit(std::string s)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, '.'))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings[0];
}

std::string stutils::getSubProcessFromSplit(std::string s)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, '.'))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings[1];
}