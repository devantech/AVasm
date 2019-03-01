#include "data_type.hpp"
#include "data.hpp"
#include "num_utils.hpp"
#include "string_utils.hpp"

namespace data_type
{

int getIdentifier(Token &t)
{
    t = data::token_list.getNext();
    if (t.type != IDENTIFIER)
    {
        data::setError("Expected identifier for data but found -> " + t.s_value);
        return 0;
    }
    return 1;
}

int getValue(int &val)
{
    Token t = data::token_list.expect(data::state.error, {IDENTIFIER, NUMBER});

    if (data::state.error)
    {
        data::setError("Undefined value being assigned ->  " + t.s_value);
        return 0;
    }

    if (t.type == NUMBER)
    {
        val = t.value;
    }
    else
    {
        Symbol sym = data::symbol_list.getSymbolFromTable(data::state.error, t.s_value, data::state.in_process, data::state.process_name);
        if (data::state.error)
        {
            data::setError(t.s_value + " not declared in this scope");
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
            data::setError(t.s_value + " must be a number .const value or .data");
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

int checkForMore(void)
{
    if (data::token_list.hasNext())
    {
        return 1;
    }
    return 0;
}

int getOperator(int &val)
{

    Token t = data::token_list.expect(data::state.error, {OPERATOR});
    if (data::state.error)
    {
        data::setError("Unexpected token while setting value -> " + t.s_value);
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

void createConst(void)
{
    if (data::state.in_process)
    {
        data::setError("Constant valaues must be declared in global scope");
    }

    Token iden;
    if (!getIdentifier(iden))
        return;

    if (!data::token_list.hasNext())
    {
        data::setError("No value set for constant");
        return;
    }

    Token t = data::token_list.expect(data::state.error, {NUMBER, IDENTIFIER});
    if (data::state.error)
        return;

    int val = numutils::getAValue(t);
    if (data::state.error)
        return;

    // if (!getValue(val))
    //     return;

    if (checkForMore())
    { // There should be no more tokens here
        data::setError("Unexpected token after setting constant value -> " + data::token_list.getNext().s_value);
        return;
    }

    int size = 1; // Constvalues always have a size of 1
    int loc = 0;  // Const values alwayshave a location of 0 as they are not actually put in processor memory
    int type = CONST;
    std::string name = data::state.process_name + iden.s_value;

    data::symbol_list.addSymbolToTable(data::state.error, type, val, size, loc, name);
    if (data::state.error)
        data::setError("Duplicate constant definition -> " + iden.s_value);

    std::string line = stutils::int_to_hex((val >> 8) & 0xff);
    line += stutils::int_to_hex(val & 0xff);

    data::data.log(data::state.line_number, loc, line, data::state.line);
}

void createData(void)
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
        data::setError("Unexpected token after setting register value -> " + data::token_list.getNext().s_value);
        return;
    }

    int location = data::state.data_count;
    data::state.data_count += size;

    std::string s = stutils::int_to_hex((val >> 8) & 0xff);
    s += stutils::int_to_hex(val & 0xff);
    data::data.log(data::state.line_number, location, s, data::state.line);

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
            data::data.data_list.push_back(line);
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
            data::data.data_list.push_back(line);
        }
    }

    /*
    String types of data have a terminating 0.
    Put one last data entry in the listing to represent this
    */
    if (t.type == STRING)
    {
        data::data.data_list.push_back("00");
        data::state.data_count++;
    }

    std::string name = data::state.process_name + iden.s_value;
    int type = DATA;

    data::symbol_list.addSymbolToTable(data::state.error, type, val, size, location, name);
    if (data::state.error)
        data::setError(iden.s_value + " was already defined in this scope");
}

int getSizeAndValue(int &size, int &val, Token &tok)
{
    tok = data::token_list.expect(data::state.error, {IDENTIFIER, STRING, NUMBER, SIZE});
    if (data::state.error)
    {
        data::setError("Invalid value being set -> " + tok.s_value);
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
        val = numutils::getAValue(tok);
        if (data::state.error)
            return 0;
        break;
    case SIZE:
        
        size = numutils::getSizeValue(tok.s_value);
        if (data::state.error)
            return 0;
    }
    return 1;
}

void createReg(void)
{
    Token iden;
    if (!getIdentifier(iden))
        return;

    int val = 0;
    if (checkForMore())
    {
        Token t = data::token_list.expect(data::state.error, {NUMBER, IDENTIFIER});
        if (data::state.error)
            return;

        val = numutils::getAValue(t);
        if (data::state.error)
            return;
    }

    if (checkForMore())
    {
        data::setError("unexpected token after setting register value -> " + data::token_list.getNext().s_value);
        return;
    }

    int location = data::state.register_count++;
    int type = REGISTER;
    int size = 1;

    std::string name = data::state.process_name + iden.s_value;

    std::string line = stutils::int_to_hex((val >> 8) & 0xff);
    line += stutils::int_to_hex(val & 0xff);
    data::data.reg_list.push_back(line);
    data::data.log(data::state.line_number, location, line, data::state.line);

    data::symbol_list.addSymbolToTable(data::state.error, type, val, size, location, name);
    if (data::state.error)
        data::setError("Duplicate register definition -> " + iden.s_value);
}

} // namespace data_type