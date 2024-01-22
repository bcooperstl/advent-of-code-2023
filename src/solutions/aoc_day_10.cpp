#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "aoc_day_10.h"
#include "file_utils.h"

using namespace std;
using namespace Day10;

#define VERTICAL '|'
#define HORIZONTAL '-'
#define NORTH_EAST 'L'
#define NORTH_WEST 'J'
#define SOUTH_WEST '7'
#define SOUTH_EAST 'F'
#define GROUND '.'
#define START 'S'

#define NO_STEPS -1

namespace Day10
{
    Cell::Cell(char symbol)
    {
        m_symbol = symbol;
        m_step_count = NO_STEPS;
    }
    
    Cell::~Cell()
    {
    }
    
    char Cell::get_symbol()
    {
        return m_symbol;
    }
    
    bool Cell::is_start()
    {
        return (m_symbol == START);
    }
    
    bool Cell::can_connect(Direction direction)
    {
        // ground cannot connect to anything
        if (m_symbol == GROUND)
        {
            return false;
        }
        
        // start can connect to any direction
        if (m_symbol == START)
        {
            return true;
        }
        
        if (m_symbol == VERTICAL)
        {
            if (direction == north || direction == south)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        
        if (m_symbol == HORIZONTAL)
        {
            if (direction == east || direction == west)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        
        if (m_symbol == NORTH_EAST)
        {
            if (direction == north || direction == east)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        
        if (m_symbol == NORTH_WEST)
        {
            if (direction == north || direction == west)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        
        if (m_symbol == SOUTH_EAST)
        {
            if (direction == south || direction == east)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        
        if (m_symbol == SOUTH_WEST)
        {
            if (direction == south || direction == west)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
                      
        cerr << "Error trying to compute matching for " << m_symbol << endl;
        return false;
    }
    
    bool Cell::is_visited()
    {
        return (m_step_count != NO_STEPS);
    }
    
    int Cell::get_step_count()
    {
        return m_step_count;
    }
    
    void Cell::set_step_count(int step_count)
    {
        m_step_count = step_count;
    }
}

AocDay10::AocDay10():AocDay(10)
{
}

AocDay10::~AocDay10()
{
}

vector<string> AocDay10::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> data;
    // space is my delimiter
    if (!fileutils.read_as_list_of_strings(filename, data))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return data;
}

string AocDay10::part1(string filename, vector<string> extra_args)
{
    vector<string> data = read_input(filename);
    
    ostringstream out;
    out << 0;
    return out.str();
}
