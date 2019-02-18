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
#ifndef ASSEMBLER_STATE_HPP
#define ASSEMBLER_STATE_HPP

#include <string>
/*
AssemblerState

Monitors the curent state of the Assembler
*/
class AssemblerState
{
  public:
    /*
    Flag used to show if the assembler is currently inside of a process or not
    */
    bool in_process = false;

    /*
    Total number of processes defined in code
    */
    int process_count = 0;
    
    /*
    The number of registers that have been defined in this program
    */
    int register_count = 0;
    
    /*
    The number of data registers that have been defined in this program
    offset to 0x200 as this is the starting point in the processor memory
    where data registers starting
    */
    int data_count = 0x0200;
    
    /*
    The address in memory of the current instruction or label being
    assembled
    */
    int prog_count = 0;
    
    /*
    Error flag, 0 is no error
    */
    int error = 0;

    /*
    Current line number in the file
    */
    int line_number = 0;
    
    /*
    Name of the processwe are in at the moment.
    If we are not in a process then this is empty
    */
    std::string process_name = "";
    
    /*
    If there is an error this will hold the error message
    */
    std::string message;
    
    /*
    The current line of text being processed
    */
    std::string line;
};

#endif