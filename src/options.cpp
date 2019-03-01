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
#include <getopt.h>
#include <iostream>
#include <stdexcept>

#include "options.hpp"

void Options::processOptions(int ac, char **av)
{

    static struct option long_options[] = {
        // {"input", required_argument, 0, 'i'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"quiet", no_argument, 0, 'q'},
        {"data-file", required_argument, 0, 'd'},
        {"inst-file", required_argument, 0, 'p'},
        {"pc-file", required_argument, 0, 'l'},
        {"reg-file", required_argument, 0, 'r'},
        {"seq-file", required_argument, 0, 's'},
        {0, 0, 0, 0}};

    if (ac < 2)
    {
        throw std::out_of_range("Not enough arguments to program");
    }

    while (optind < ac)
    {
        auto option_index = 0;
        // auto c = getopt_long(ac, av, "hdplri:", long_options, &option_index);
        int c;
        if ((c = getopt_long(ac, av, "qvhd:p:l:r:", long_options, &option_index)) != -1) {
            switch (c)
            { 
            case 'h':
                Options::help = true;
                return;
            case 'd':
                Options::data_file = optarg;
                break;
            case 'p':
                Options::inst_file = optarg;
                break;
            case 'l':
                Options::pc_file = optarg;
                break;
            case 'r':
                Options::reg_file = optarg;
                break;
            case 's':
                Options::seq_file = optarg;
                break;
            case 'v':
                Options::version = true;
                break;
            case 'q':
                Options::quiet = true;
                break;
            case '?':
                throw std::invalid_argument("Invalid argument");
                break;
            default:
                abort();
            }
        } else
        {
            if (Options::input_file != "") {
                std::cout << "Unknown argument passed to program, are you passing more than one input file to the application?" << std::endl;
                std::cout << "--> " << av[++optind] << std::endl;
                throw std::invalid_argument("Invalid argument");
            }
            Options::input_file = av[optind++];
        }

    }
}