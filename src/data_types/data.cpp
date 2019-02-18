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
#include "data_type.hpp"
#include <iostream>

void DataType::create(void)
{
    Token iden;
    if (!getIdentifier(iden))
        return;

    int val = 0;  // Data registers do not have to have a value set so a value of 0 is defautlt
    int size = 1; // Unless this data is a string or array it will have a size of 1
    Token t;

    if (checkForMore()) // If there is an associated value grab it
    {
        if (!getSizeAndValue(size, val, t))
            return;
    }

    if (checkForMore())
    {
        state.error = 1;
        state.message = "Unexpected token after setting register value -> " + token_list.getNext().s_value;
        return;
    }

    int location = state.data_count;
    state.data_count += size;

    std::string s = stutils::int_to_hex((val >> 8) & 0xff);
    s += stutils::int_to_hex(val & 0xff);
    data.log(state.line_number, location, s, state.line);

    std::string line;

    /*
    Place however many entries size dictates into the data_list
    If this data is an array it will be filled with all 0s
    If this data is a string it will be fileld with the contents of the string
    If it is just a regular data value it will contain the set value
    */
    for (int i = 0; i < size; i++)
    {
        if (t.type == STRING)
        {
            int sval = int(t.s_value[i]);
            line = stutils::int_to_hex(sval & 0xff);
            data.data_list.push_back(line);
        }
        else
        {
            if (size == 1)
            {
                line = stutils::int_to_hex(val & 0xff);
            }
            else
            {
                line = "00";
            }
            data.data_list.push_back(line);
        }
    }

    /*
    String types of data have a terminating 0.
    Put one last data entry in the listing to represent this
    */
    if (t.type == STRING)
    {
        data.data_list.push_back("00");
        state.data_count++;
    }

    std::string name = state.process_name + iden.s_value;
    int type = DATA;

    symbol_list.addSymbolToTable(state.error, type, val, size, location, name);
    if (state.error)
        state.message = iden.s_value + " was already defined in this scope";
}

int DataType::getSizeAndValue(int &size, int &val, Token &tok)
{
    tok = token_list.expect(state.error, {IDENTIFIER, STRING, NUMBER, SIZE});
    if (state.error)
    {
        state.message = "Invalid value being set -> " + tok.s_value;
        return 0;
    }

    Symbol sym;
    switch (tok.type)
    {
    case STRING:
        size = tok.s_value.length();
        val = tok.s_value[0];
        break;
    case NUMBER:
    case IDENTIFIER:
        val = numutils::getAValue(tok, token_list, symbol_list, state);
        if (state.error)
            return 0;
        break;
    case SIZE:
        size = numutils::getSizeValue(tok.s_value, symbol_list, state);
        if (state.error)
            return 0;
    }
    return 1;
}

