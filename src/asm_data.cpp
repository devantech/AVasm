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
#include "asm_data.hpp"

void AsmData::setListingLength(int len)
{
    asm_listing.resize(len);
}

void AsmData::log(int ln, int location, std::string data, std::string line)
{
    char fill = '.';
    if ((ln % 2) == 0)
    {
        fill = ' ';
    }

    std::string location_line;
    if (location == -1)
    {
        location_line = "";
    }
    else
    {
        location_line += stutils::int_to_hex((location >> 8) & 0xff);
        location_line += stutils::int_to_hex(location & 0xff);
    }

    std::stringstream loc;
    loc << std::left << std::setw(8) << std::setfill(fill)
        << location_line;

    std::stringstream dat;
    dat << std::left << std::setw(12) << std::setfill(fill)
        << data;

    std::string l = loc.str() + dat.str() + " " + line + '\n';

    asm_listing.at((ln - 1) + insert_offset) = l;
}

void AsmData::insertLog(int ln, int location, std::string data, std::string line)
{
    char fill = '.';
    if ((ln % 2) == 0)
    {
        fill = ' ';
    }

    std::string location_line;
    if (location == -1)
    {
        location_line = "";
    }
    else
    {
        location_line += stutils::int_to_hex((location >> 8) & 0xff);
        location_line += stutils::int_to_hex(location & 0xff);
    }

    std::stringstream loc;
    loc << std::left << std::setw(8) << std::setfill(fill)
        << location_line;

    std::stringstream dat;
    dat << std::left << std::setw(12) << std::setfill(fill)
        << data;

    std::string l = loc.str() + dat.str() + " " + line + '\n';
    asm_listing.insert((asm_listing.begin() + (ln - 1)) + insert_offset, l);
    insert_offset++;
}

void AsmData::createListingFile(std::string name)
{
    std::ofstream f(name);
    for (auto it = asm_listing.begin(); it != asm_listing.end(); ++it)
    {
        f << *it;
    }
    f.flush();
    f.close();
}

void AsmData::createProgramFile(std::string name)
{
    std::ofstream f(name);
    for (auto it = ins_list.begin(); it != ins_list.end(); ++it)
    {
        f << *it + "\n";
    }
    f.flush();
    f.close();
}

void AsmData::createDataFile(std::string name)
{
    std::ofstream f(name);
    for (auto it = data_list.begin(); it != data_list.end(); ++it)
    {
        f << *it + "\n";
    }
    f.flush();
    f.close();
}

void AsmData::createRegFile(std::string name)
{
    std::ofstream f(name);
    for (auto it = reg_list.begin(); it != reg_list.end(); ++it)
    {
        f << *it + "\n";
    }
    f.flush();
    f.close();
}

void AsmData::createProcessFile(std::string name)
{
    std::ofstream f(name);
    for (auto it = pc_list.begin(); it != pc_list.end(); ++it)
    {
        f << *it + "\n";
    }
    f.flush();
    f.close();
}

void AsmData::createConfigFile(std::string name)
{
    std::ofstream f(name);

    f << "// These are assembler maintained constants.\n";
    f << "// Do not change manually.\n";
    f << "\n";
    f << "parameter process_count = " << process_count << ";";
    f.flush();
    f.close();
}
