#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "aoc_day_3.h"
#include "file_utils.h"

using namespace std;
using namespace Day3;

#define NON_SYMBOL '.'

namespace Day3
{
    Schematic::Schematic()
    {
        m_num_rows = 0;
        m_num_cols = 0;
        memset(m_schematic, NON_SYMBOL, sizeof(m_schematic));
    }
    
    Schematic::~Schematic()
    {
    }
    
    void Schematic::load_schematic(vector<string> input_data)
    {
        memset(m_schematic, NON_SYMBOL, sizeof(m_schematic));
        m_num_rows = input_data.size();
        m_num_cols = input_data[0].length();
        // set the top and bottom rows
        m_schematic[0][m_num_cols+2] = '\0'; // top row
        m_schematic[m_num_rows+1][m_num_cols+2] = '\0'; // bottom row
#ifdef DEBUG_DAY_3
        cout << "top row is [" << m_schematic[0] << "]" << endl;
        cout << "bottom row is [" << m_schematic[m_num_rows+1] << "]" << endl;
#endif
        for (int src_row=0; src_row<m_num_rows; src_row++)
        {
            
            memcpy(m_schematic[src_row+1] + 1, input_data[src_row].c_str(), m_num_cols);
            // set the null terminator after one .
            m_schematic[src_row+1][m_num_cols+2] = '\0';
        }
#ifdef DEBUG_DAY_3
        cout << "Loaded input schematic of " << m_num_rows << " rows and " << m_num_cols << " columns to:" << endl;
        display_schematic();
#endif
    }
    
    void Schematic::display_schematic()
    {
        for (int row=0; row<m_num_rows+2; row++)
        {
            cout << m_schematic[row] << endl;
        }
        cout << endl;
    }
}

AocDay3::AocDay3():AocDay(3)
{
}

AocDay3::~AocDay3()
{
}

vector<string> AocDay3::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> data;
    if (!fileutils.read_as_list_of_strings(filename, data))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return data;
}

string AocDay3::part1(string filename, vector<string> extra_args)
{
    vector<string> data = read_input(filename);
    Schematic schematic;
    schematic.load_schematic(data);
    
    int sum = 0;
    
    
    ostringstream out;
    out << sum;
    return out.str();
}
