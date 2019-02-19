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
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <regex>

#include "options.hpp"
#include "assemble.hpp"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_BUILD 0

/*
Process the passed command line arguments and return an options ibject with the setting in

int ac      - The number of arguments passed 
char **av,  - Pointer to the actual arguments
int *err    - An error value, this will be set if an eerror occured while building the options.
*/
Options processArguments(int ac, char **av, int *err);

/*
Show a help screen in the terminal
*/
void showHelp();

/*
Takes a file path in and outputs what the listing file should be called

std::string &n  - the path.
*/
std::string listingName(std::string &n);

/*
Prints the version number to the screen
*/
void printVersion(void);

/**********************************************

main() 

**********************************************/
int main(int argc, char *argv[])
{
    auto error = 0; // should be zero, a non zero value for error signals an error has been encountered

    Options opts = processArguments(argc, argv, &error);

    if (opts.help)
    {
        showHelp();
        return error;
    }

    if (opts.version)
    {
        printVersion();
        return error;
    }

    // The input file ha not been set
    if (opts.input_file.empty())
    {
        error = 1;
        std::cout << "No input file to assemble" << std::endl;
    }

    if (!error)
    {
        std::ifstream source = std::ifstream(opts.input_file);

        if (source.is_open())
        {
            Assemble comp(source);
            comp.go();

            if (comp.state.error)
            {
                std::cout << "Build failed" << std::endl;
                error = 1;
            }
            else
            {
                comp.data.createConfigFile("config.v");
                comp.data.createDataFile(opts.data_file);
                comp.data.createProcessFile(opts.pc_file);
                comp.data.createProgramFile(opts.inst_file);
                comp.data.createRegFile(opts.reg_file);
                comp.data.createListingFile(listingName(opts.input_file));
                if (!opts.quiet)
                {
                    std::cout << "processes " << comp.data.process_count << ", ";
                    std::cout << "registers " << comp.data.reg_list.size() << ", ";
                    std::cout << "data " << comp.data.data_list.size() << ", ";
                    std::cout << "instructions " << comp.data.ins_list.size() << std::endl;
                }
            }

            source.close();
        }
        else
        {
            std::cout << "Failed to open file - " + opts.input_file << std::endl;
            error = 1;
        }
    }

    return error;
}

Options processArguments(int ac, char **av, int *err)
{
    Options opts;
    try
    {
        opts.processOptions(ac, av);
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << std::endl;
        *err = 1;
    }
    catch (const std::out_of_range &e)
    {
        std::cout << e.what() << std::endl;
        *err = 1;
    }
    return opts;
}

void showHelp(void)
{
    std::cout << "Avalanche assembler" << std::endl;
    std::cout << "Assembler for devantech Avalanche processor." << std::endl;
    std::cout << "Assembles given assembly file and assembles into output files." << std::endl;
    std::cout << "These files are:" << std::endl;
    std::cout << "    pc_data       - Contains the starting locations in memory of every process defined in the program." << std::endl;
    std::cout << "    inst_data     - Contains the opcodes that make the program." << std::endl;
    std::cout << "    dta_data      - Contains the values for any data registers defined." << std::endl;
    std::cout << "    reg_data      - Contains the values for any registers defined." << std::endl;
    std::cout << "    config.v      - Assembler created config file." << std::endl;
    std::cout << "    <file>.lst    - listing file." << std::endl;
    std::cout << "The names of these files can be changed by the user." << std::endl;
    std::cout << "USAGE:" << std::endl;
    std::cout << "avalanche [d <name>] [p <name>] [l <name>] [r <name>] [-v] [-q] <input file>" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "  -d <name> the name of the dta_data" << std::endl;
    std::cout << "  -p <name> the name of the inst_data file" << std::endl;
    std::cout << "  -l <name> the name of the pc_data file" << std::endl;
    std::cout << "  -r <name> the name of the reg_data file" << std::endl;
    std::cout << "  -h This help text" << std::endl;
    std::cout << "  -v Display the version number" << std::endl;
    std::cout << "  -q Quitet mode, turn off the assembler memory useage output" << std::endl;
}

std::string listingName(std::string &n)
{
    char seperator = '/';
    // Get last dot position
    std::size_t dotPos = n.rfind('.');
    std::size_t sepPos = 0;
    std::size_t mod = 0;
    if (n.find(seperator) != std::string::npos)
    {
        sepPos = n.rfind(seperator);
        mod = 1;
    }

    if (sepPos != std::string::npos)
    {
        return n.substr(sepPos + mod, n.size() - (true || dotPos != std::string::npos ? 1 : dotPos)) + "lst";
    }
    return "output.lst";
}

void printVersion(void)
{
    std::cout << "avalanche assembler version ";
    std::cout << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_BUILD << std::endl;
}
