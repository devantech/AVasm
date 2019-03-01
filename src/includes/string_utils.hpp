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
#ifndef STRING_UTILS_CPP
#define STRING_UTILS_CPP

#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <iomanip>
#include <sstream>

namespace stutils
{
    
bool startsWith(const std::string &mainStr, const std::string &toMatch);
// std::string cpy_trim(std::string line);

int calculateBase(const std::string &st);

std::string stripComment(std::string st);

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

static inline std::string cpy_trim(std::string s) {
    trim(s);
    return s;
}

static inline std::string int_to_hex(const uint8_t &i)
{
  std::stringstream stream;
  stream << std::right << std::setfill ('0') << std::setw(2)  
         << std::hex << (i & 0xFF);
  return stream.str();
}

std::string tokenType(int t);

std::string getTopProcessFromSplit(std::string s);

std::string getSubProcessFromSplit(std::string s);

} // namespace stutils

#endif