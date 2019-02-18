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
#ifndef OPTIONS_CPP
#define OPTIONS_CPP

#include <string>

/**
 * A class for processing the command line options and storing the settings for them
 */
class Options
{
  public:
    std::string input_file = "";
    
    std::string pc_file = "pc_data";
    std::string inst_file = "inst_data";
    std::string reg_file = "reg_data";
    std::string data_file = "dta_data";

    bool help = false;
    bool version = false;
    bool quiet = false;

    /*
    Process the command line options

    int ac      - thenumber of command line options passed in
    char **av   - pointers to the options and values
    */
    void processOptions(int ac, char **av);
};

#endif