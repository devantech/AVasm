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

#include "process_map.hpp"
#include "string_utils.hpp"
#include "num_utils.hpp"
#include <iostream>

ProcessData::ProcessData(std::string name, int pc_location)
{
    std::size_t found = name.find(".");
    if (found != std::string::npos)
    {
        split = true;
        top = stutils::getTopProcessFromSplit(name);
        subs.push_back(stutils::getSubProcessFromSplit(name));
        locs.push_back(pc_location);
    }
    else
    {
        split = false;
        top = name;
        locs.push_back(pc_location);
    }
}

void ProcessData::addSubProcess(std::string sub, int location)
{
    std::string s = stutils::getSubProcessFromSplit(sub);
    subs.push_back(s);
    locs.push_back(location);
}

int ProcessData::getNumberOfSplits()
{
    return locs.size();
}

std::string ProcessData::getTopName()
{
    return top;
}

bool ProcessData::containsSubProcess(std::string sub)
{
    for (auto it = subs.begin(); it != subs.end(); ++it)
    {
        if (*it == sub)
        {
            return true;
        }
    }
    return false;
}

void ProcessData::buildLocationVector()
{
    std::sort(locs.begin(), locs.end());
    loc_pos = locs.begin();
}

std::string ProcessData::getNextLocationOutput()
{
    if (loc_pos == locs.end())
    {
        loc_pos = locs.begin();
    }
    std::string s = stutils::int_to_hex(*loc_pos);
    ++loc_pos;
    return s;
}

std::vector<ProcessData> p_list;

bool checkIfProcessExists(std::string &name)
{
    try
    {
        ProcessData *pc = getProcessWithTopName(name);
        if (pc->split)
        {
            return pc->containsSubProcess(stutils::getSubProcessFromSplit(name));
        }
        return true;
    }
    catch (std::invalid_argument &ex)
    {

        return false;
    }
}

bool isProcessSplit(std::string &name)
{
    try
    {
        ProcessData *pc = getProcessWithTopName(name);
        return pc->split;
    }
    catch (std::invalid_argument &ex)
    {
        return false;
    }
}

bool doesTopLevelProcessExist(std::string &name)
{
    try
    {
        ProcessData *pc = getProcessWithTopName(name);
        return true;
    }
    catch (std::invalid_argument &ex)
    {
        return false;
    }
}

void addProcessData(std::string name, int loc)
{

    if (checkIfProcessExists(name))
    {
        throw std::invalid_argument(" <-- process already defined");
    }
    std::size_t found = name.find(".");
    if (found != std::string::npos)
    {
        try
        {
            ProcessData *pd = getProcessWithTopName(name);

            if (pd->split)
            {
                pd->addSubProcess(name, loc);
                return;
            }
            else
            {
                throw std::invalid_argument(name + " <-- this process can not be split");
            }
            return;
        }
        catch (std::invalid_argument &ex)
        {

        }
    }
    ProcessData pd(name, loc);
    p_list.push_back(pd);
}

int getLCMForSeqData(void)
{
    std::vector<int> vals;
    for (auto it = p_list.begin(); it != p_list.end(); ++it)
    {
        ProcessData pc = *it;
        vals.push_back(pc.getNumberOfSplits());
    }
    return numutils::getLcm(vals);
}

ProcessData* getProcessWithTopName(std::string name)
{
    std::size_t found = name.find(".");
    if (found != std::string::npos)
    {
        name = stutils::getTopProcessFromSplit(name);
    }
    for (auto it = p_list.begin(); it != p_list.end(); ++it)
    {
        ProcessData pd = *it;
        if (pd.top == name)
        {
            return &*it;
        }
    }
    throw std::invalid_argument("name does not exist.");
}