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
#ifndef PROCESS_MAP_HPP
#define PROCESS_MAP_HPP

#include <map>
#include <vector>


class ProcessData
{

  public:

    bool split;
    std::string top;
    std::vector<std::string> subs;
    std::vector<int> locs;
    std::vector<int>::iterator loc_pos;

    /*
    Create a process data object with the name of the found process and location of its pc_data entry in the pc ram.
    */
    ProcessData(std::string name, int pc_location);

    /*
    Add a sub process to this process.
    */
    void addSubProcess(std::string sub, int location);

    /*
    Return the number of splits this process has, if this process has no splits then returns 1
    */
    int getNumberOfSplits(void);

    /*
    get the name of this top level process
    */
    std::string getTopName(void);

    /*
    check to see if the process has a named sub process in it.

    returns true if it does and false if it does not.
    */
    bool containsSubProcess(std::string sub);

    /*
    Create an ordered vector of this processes location in memory
    this must be called before the process can be used to create the sequence file.
    */
    void buildLocationVector(void);

    /*
    Gets the next ram location for the sequence file
    */
    std::string getNextLocationOutput(void);


};

/*
HGolds a list of all the processes that have been declered in this program
*/
extern std::vector<ProcessData> p_list;

/*
Checks to see if a process has already been declared or not.

returns true if the process has already been declared and false if it has not.

std::string & name     - the name  of the process we are checking
*/
bool checkIfProcessExists(std::string & name);

/*
Checks to see if a defined process is a split process or not

returns true if the process is split and false if not.

std::string & name     - the name  of the process to be checked

*/
bool isProcessSplit(std::string & name);

/*
Checks to see if a top level process is already defined in this program.

returns true if it is and false if it is not.

std::string & name      - the name of the top level process to check
*/
bool doesTopLevelProcessExist(std::string &name);

/*
returns the LCM for the processes that have been defined in this program. 
This is how many itterations that the seq data will need to create a loop.
*/
int getLCMForSeqData(void);

/*
Create a new ProcessData object and add it to the list.
*/
void addProcessData(std::string name, int loc);

/*
Get the process that is defined with the passed in top level process name

throws invalid_argument of name does not appear in the list
*/
ProcessData* getProcessWithTopName(std::string name);

#endif