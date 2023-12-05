#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cctype>

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
    
    // This function assumes the row and cols are in range and border can be examined
    // This function also assumes that there aren't digits to the left of start and right of end
    bool Schematic::has_neighbor_symbol(int row, int start_col, int end_col)
    {
#ifdef DEBUG_DAY_3
        cout << " Checking for neighboring symbols for row " << row << " cols " << start_col << "-" << end_col << endl;
#endif
        // check left for start col
        if (m_schematic[row][start_col-1] != NON_SYMBOL)
        {
#ifdef DEBUG_DAY_3
            cout << "  Symbol " << m_schematic[row][start_col-1] << " found at row " << row << " column " << start_col - 1 << endl;
#endif
            return true;
        }
        // check right for end col
        if (m_schematic[row][end_col+1] != NON_SYMBOL)
        {
#ifdef DEBUG_DAY_3
            cout << "  Symbol " << m_schematic[row][end_col+1] << " found at row " << row << " column " << end_col + 1 << endl;
#endif
            return true;
        }
        // check above and below rows from start_col-1 to end_col+1 to count for diagonals
        for (int col=(start_col-1); col<=(end_col+1); col++)
        {
            // above row
            if (m_schematic[row-1][col] != NON_SYMBOL)
            {
#ifdef DEBUG_DAY_3
                cout << "  Symbol " << m_schematic[row-1][col] << " found at row " << row-1 << " column " << col << endl;
#endif
                return true;
            }
            // below row
            if (m_schematic[row+1][col] != NON_SYMBOL)
            {
#ifdef DEBUG_DAY_3
                cout << "  Symbol " << m_schematic[row+1][col] << " found at row " << row+1 << " column " << col << endl;
#endif
                return true;
            }
        }
#ifdef DEBUG_DAY_3
        cout << "  No symbol found" << endl;
#endif
        return false;
    }
    
    vector<int> Schematic::find_part_numbers()
    {
        vector<int> part_numbers;
        
        int part_number, match_start_col, match_end_col;
        for (int row=1; row<=m_num_rows; row++)
        {
            for (int col=1; col<=m_num_cols; col++)
            {
                if (isdigit(m_schematic[row][col])) // found the leading digit
                {
                    part_number = 0;
                    match_start_col = col;
                    while (isdigit(m_schematic[row][col]))
                    {
                        part_number = (part_number * 10) + m_schematic[row][col]-'0';
                        match_end_col = col;
                        col++;
                    }
                    // at this point, m_schematic[row][col] is not a digit. need to check if the value we found has a symbol neighbor or not
#ifdef DEBUG_DAY_3
                    cout << "Found " << part_number << " at row " << row << " columns " << match_start_col << "-" << match_end_col << ". Checking if neighbor has symbol" << endl;
#endif
                    if (has_neighbor_symbol(row, match_start_col, match_end_col))
                    {
#ifdef DEBUG_DAY_3
                        cout << " Matching neighbor symbol found for " << part_number << endl;
#endif
                        part_numbers.push_back(part_number);
                    }
                    else
                    {
#ifdef DEBUG_DAY_3
                        cout << " No matching neighbor symbol found for " << part_number << endl;
#endif
                    }
                }
            }
        }
        
        return part_numbers;
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
    
    vector<int> part_numbers = schematic.find_part_numbers();
    
    int sum = 0;
    for (int i=0; i<part_numbers.size(); i++)
    {
        sum+=part_numbers[i];
    }
    
    ostringstream out;
    out << sum;
    return out.str();
}
