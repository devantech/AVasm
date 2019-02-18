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
#ifndef ASM_DATA_HPP
#define ASM_DATA_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "string_utils.hpp"
#include "token.hpp"

class AsmData
{
private:
  /*
    This is the listing output for the generated assembly code
    */
  std::vector<std::string> asm_listing;

  /*
  An offset used when inserting lines into the listing output.
  when a line is inserted into the listing the line numbers no
  longer match up. This accounts for that. 
  */
  int insert_offset = 0;

public:
  /*
    This is the listing that will comprise the contents of the data file
    that is output by the assembler on a sucess. It should contain the 
    values of each data point as a 16 bit hex number in string format.
    */
  std::vector<std::string> data_list;

  /*
    This is the listing that will comprise the contents of the reg file
    that is output by the assembler on a success. It should contain the 
    values of each redister defined as a 16b bit hex number in string format.
    */
  std::vector<std::string> reg_list;

  /*
    This contains the memory locations of the start of any process that
    is defines in the code, it is stored as a 16 bit hex number in string format
    */
  std::vector<std::string> pc_list;

  /*
    This contains all of the instructions that have been defined in the program.
    Each instruction is stored as a 32 bit hex number in string format.
    */
  std::vector<std::string> ins_list;

  /*
    This is the number of processes that are defined in the program.
    This number should always be >= 7 when a project is finished building.
    */
  int process_count = 0;

  /*
    Reserve space in the listing for however many lines there are in the
    assembled file.
    */
  void setListingLength(int len);

  /*
    Add data to the assembly listing. This will format the data passed to it
    into a human readable assembly listing output.

    int ln              - The line number which we are inserting
    int location        - The memory location of the instruction or assignment
    std::string data    - the data to listing
    std::string line    - the line which produced the data
    */
  void log(int ln, int location, std::string data, std::string line);

  /*
    Add data to the assembly listing. This will format the data passed to it
    into a human readable assembly listing output. This will insert a new 
    string into the listing at the location requested and push all other data
    down one place

    int ln              - The line number which we are inserting
    int location        - The memory location of the instruction or assignment
    std::string data    - the data to listing
    std::string line    - the line which produced the data
    */
  void insertLog(int ln, int location, std::string data, std::string line);

  /*
    Create the listing file on the disk. Contents of the listing file will be the contents of the asm_listing
    vector output in order.

    std::string dir     - the name of the file to be created.
    */
  void createListingFile(std::string dir);

  /*
    Create the program instructions file on the disk.

    std::string dir     - the name of the file to be created.
    */
  void createProgramFile(std::string dir);

  /*
    Create the data file on the disk.

    std::string dir     - the name of the file to be created.
    */
  void createDataFile(std::string dir);

  /*
    Create the register file on the disk.

    std::string dir     - the name of the file to be created.
    */
  void createRegFile(std::string dir);

  /*
    Create the process file on the disk.

    std::string dir     - the name of the file to be created.
    */
  void createProcessFile(std::string dir);

  /*
    Create the config.v file on the disk.

    std::string dir     - the name of the file to be created.
    */
  void createConfigFile(std::string dir);
};

#endif