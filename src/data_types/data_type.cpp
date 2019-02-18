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

int Type::getIdentifier(Token &t)
{
    t = token_list.getNext();
    if (t.type != IDENTIFIER)
    {
        state.error = 1;
        state.message = "Expected identifier for data but found -> " + t.s_value;
        return 0;
    }
    return 1;
}

int Type::getValue(int &val)
{
    Token t = token_list.expect(state.error, {IDENTIFIER, NUMBER});

    if (state.error)
    {
        state.message = "Undefined value being assigned ->  " + t.s_value;
        return 0;
    }

    if (t.type == NUMBER)
    {
        val = t.value;
    }
    else
    {
        Symbol sym = symbol_list.getSymbolFromTable(state.error, t.s_value, state.in_process, state.process_name);
        if (state.error)
        {
            state.message = t.s_value + " not declared in this scope";
            return 0;
        }

        switch (sym.type())
        {
        case CONST:
            val = sym.value();
            break;
        case DATA:
            val = sym.location();
            break;
        default:
            state.error = 1;
            state.message = t.s_value + " must be a number .const value or .data";
            return 0;
        }
    }
    if (checkForMore())
    {
        if (!getOperator(val))
            return 0;
    }
    return 1;
}

int Type::checkForMore(void)
{
    if (token_list.hasNext())
    {
        return 1;
    }
    return 0;
}

int Type::getOperator(int &val)
{

    Token t = token_list.expect(state.error, {OPERATOR});
    if (state.error)
    {
        state.message = "Unexpected token while setting value -> " + t.s_value;
        return 0;
    }

    int mod = 0;

    if (!getValue(mod))
        return 0;
    if (t.s_value == "*")
        val = val * mod;
    else if (t.s_value == "+")
        val = val + mod;
    else if (t.s_value == "/")
        val = val / mod;
    else if (t.s_value == "-")
        val = val - mod;

    return 1;
}