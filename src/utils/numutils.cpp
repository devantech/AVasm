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
#include "num_utils.hpp"

namespace numutils
{

int getAValue(Token &tok, TokenList &token_list, SymbolList &symbol_list, AssemblerState &state)
{
    int val;
    if (tok.type == NUMBER)
    {
        val = tok.value;
    }
    else
    {
        Symbol sym = symbol_list.getSymbolFromTable(state.error, tok.s_value, state.in_process, state.process_name);
        if (state.error)
        {
            state.message = tok.s_value + " was not declared in this scope";
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
            state.message = tok.s_value + " must be a number, .const or .data value";
            return 0;
        }
    }

    while (token_list.hasNext())
    {
        if (token_list.hasNext())
        {
            checkOp(token_list, symbol_list, state, val);
            if (state.error)
                return 0;
        }
        if (!token_list.hasNext())
            break;
    }
    if (state.error)
        return 0;
    return val;
}

int getIValue(Token &tok, TokenList &token_list, SymbolList &symbol_list, AssemblerState &state)
{
    int val;
    if (tok.type == NUMBER)
    {
        val = tok.value;
    }
    else
    {
        Symbol sym = symbol_list.getSymbolFromTable(state.error, tok.s_value, state.in_process, state.process_name);
        if (state.error)
        {
            state.message = tok.s_value + " was not declared in this scope";
            return 0;
        }
        switch (sym.type())
        {
        case CONST:
            val = sym.value();
            break;
        case DATA:
        case LABEL:
            val = sym.location();
            break;
        default:
            state.error = 1;
            state.message = tok.s_value + " must be a number, label, .const or .data value";
            return 0;
        }
    }

    while (token_list.hasNext())
    {
        if (token_list.hasNext())
        {
            checkOp(token_list, symbol_list, state, val);
            if (state.error)
                return 0;
        }
        if (!token_list.hasNext())
            break;
    }
    if (state.error)
        return 0;
    return val;
}

int getSizeValue(std::string &s_value, SymbolList &sl, AssemblerState &state)
{
    TokenList tl;
    tl.getAllTokens(s_value);
    // size = getTotalValue(tl, sl, state);
    int val = getNextValue(tl, sl, state);
    if (state.error)
        return 0;
    while (tl.hasNext())
    {
        if (tl.hasNext())
        {
            checkOp(tl, sl, state, val);
            if (state.error)
                return 0;
        }
        if (!tl.hasNext())
            break;
    }
    if (state.error)
        return 0;
    return val;
}

int getNextValue(TokenList &tl, SymbolList &sl, AssemblerState &state)
{
    int val;
    Token t = tl.expect(state.error, {IDENTIFIER, NUMBER});
    Symbol s;

    if (state.error)
        return 0;
    switch (t.type)
    {
    case NUMBER:
        val = t.value;
        break;
    case IDENTIFIER:
        s = sl.getSymbolFromTable(state.error, t.s_value, state.in_process, state.process_name);
        if (state.error){
            state.message = t.s_value + " was not declared in this scope";
            return 0;
        }
        switch (s.type())
        {
        case CONST:
            val = s.value();
            break;
        case DATA:
        case LABEL:
            val = s.location();
            break;
        default:
            state.error = 1;
            state.message = t.s_value + " must be a .const or .data value";
            return 0;
        }
        break;
    default:
        state.error = 1;
        state.message = t.s_value + " must be a .const or .data value";
        return 0;
    }
    return val;
}

void checkOp(TokenList &tl, SymbolList &sl, AssemblerState &state, int &val)
{
    Token t = tl.expect(state.error, {OPERATOR});
    if (state.error)
    {
        state.message = "Expected math operator (= - / *) but found -> " + t.s_value;
        return;
    }
    int mod = getNextValue(tl, sl, state);
    if (state.error)
        return;

    if (t.s_value == "*")
        val = val * mod;
    else if (t.s_value == "+")
        val = val + mod;
    else if (t.s_value == "/")
        val = val / mod;
    else if (t.s_value == "-")
        val = val - mod;
    return;
}

} // namespace numutils