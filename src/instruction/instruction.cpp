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
#include "instruction.hpp"

void instructions::Instruction::getReg(Symbol &sym)
{
    getSymbol(sym);
    if (state.error == 1)
        return;
    if (token_list.get().type == INDIRECTION)
    {
        state.error = 1;
        state.message = token_list.get().s_value + " can not be an indirection.";
    }
    if (sym.type() != REGISTER)
    {
        state.error = 1;
        state.message = token_list.get().s_value + " is not a valid register";
    }
}

void instructions::Instruction::getSymbol(Symbol &sym)
{
    Token dest = token_list.expect(state.error, {IDENTIFIER, INDIRECTION});
    if (state.error)
    {
        state.message = "Expected identifier but found -> " + token_list.get().s_value;
        return;
    }
    sym = symbol_list.getSymbolFromTable(state.error, dest.s_value, state.in_process, state.process_name);
    if (state.error)
    {
        state.message = dest.s_value + " not declared in this socpe";
        return;
    }
}

int instructions::Instruction::checkComma(void)
{
    token_list.expect(state.error, {COMMA});
    if (state.error)
    {
        state.error = 1;
        state.message = "Expected , but found -> " + token_list.getNext().s_value;
        return 0;
    }
    return 1;
}

int instructions::Instruction::checkForMore(void)
{
    if (token_list.hasNext())
    {
        state.error = 1;
        state.message = "Unexpected token found after instruction -> " + token_list.getNext().s_value;
        return 0;
    }
    return 1;
}