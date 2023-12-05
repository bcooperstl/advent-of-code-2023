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
#define GEAR_SYMBOL '*'

namespace Day3
{
    Gear::Gear(int row, int col)
    {
        m_row = row;
        m_col = col;
    }
    
    Gear::~Gear()
    {
    }
    
    void Gear::add_part_number(int part_number)
    {
#ifdef DEBUG_DAY_3
        cout << "Gear at row " << m_row << " column " << m_col << " connect to part number " << part_number << endl;
#endif
        m_part_numbers.push_back(part_number);
    }
    
    bool Gear::is_valid_gear()
    {
        return (m_part_numbers.size() == 2);
    }
    
    int Gear::get_gear_ratio()
    {
        if (!is_valid_gear())
        {
            return 0;
        }
#ifdef DEBUG_DAY_3
        cout << "Gear at row " << m_row << " column " << m_col << " has gear ratio " << m_part_numbers[0] << " * " << m_part_numbers[1] << " = " << m_part_numbers[0] * m_part_numbers[1] << endl;
#endif
        return m_part_numbers[0] * m_part_numbers[1];
    }
    
    int Gear::get_row()
    {
        return m_row;
    }
    
    int Gear::get_col()
    {
        return m_col;
    }
    
    Gears::Gears()
    {
    }
    
    Gears::~Gears()
    {
        for (int i=0; i<m_gears.size(); i++)
        {
            delete m_gears[i];
        }
    }
    
    vector<Gear *> Gears::get_valid_gears()
    {
#ifdef DEBUG_DAY_3
        cout << "Fetching valid gears:" << endl;
#endif
        vector<Gear *> valid;
        for (int i=0; i<m_gears.size(); i++)
        {
            if (m_gears[i]->is_valid_gear())
            {
#ifdef DEBUG_DAY_3
                cout << "Gear at row " << m_gears[i]->get_row() << " column " << m_gears[i]->get_col() << " is valid" << endl;
#endif
                valid.push_back(m_gears[i]);
            }
            else
            {
#ifdef DEBUG_DAY_3
                cout << "Gear at row " << m_gears[i]->get_row() << " column " << m_gears[i]->get_col() << " is not valid" << endl;
#endif
            }
        }
        return valid;
    }
    
    void Gears::add_part_number_to_gear(int row, int col, int part_number)
    {
        Gear * match = NULL;
        for (int i=0; i<m_gears.size(); i++)
        {
            if ((m_gears[i]->get_row() == row) && (m_gears[i]->get_col() == col))
            {
                match = m_gears[i];
            }
        }
        if (match == NULL)
        {
#ifdef DEBUG_DAY_3
            cout << "Creating new gear at row " << row << " column " << col << endl;
#endif
            match = new Gear(row, col);
        }
        match->add_part_number(part_number);
    }

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
    
    // This function assumes the row and cols are in range and border can be examined
    // This function also assumes that there aren't digits to the left of start and right of end
    void Schematic::load_part_number_to_gears(int row, int start_col, int end_col, int part_number, Gears * gears)
    {
#ifdef DEBUG_DAY_3
        cout << " Checking for neighboring gears for " << part_number << " at row " << row << " cols " << start_col << "-" << end_col << endl;
#endif
        // check left for start col
        if (m_schematic[row][start_col-1] == GEAR_SYMBOL)
        {
#ifdef DEBUG_DAY_3
            cout << "  Gear symbol " << m_schematic[row][start_col-1] << " found at row " << row << " column " << start_col - 1 << endl;
#endif
            gears->add_part_number_to_gear(row, start_col-1, part_number);
        }
        // check right for end col
        if (m_schematic[row][end_col+1] == GEAR_SYMBOL)
        {
#ifdef DEBUG_DAY_3
            cout << "  Gear symbol " << m_schematic[row][end_col+1] << " found at row " << row << " column " << end_col + 1 << endl;
#endif
            gears->add_part_number_to_gear(row, end_col+1, part_number);
        }
        // check above and below rows from start_col-1 to end_col+1 to count for diagonals
        for (int col=(start_col-1); col<=(end_col+1); col++)
        {
            // above row
            if (m_schematic[row-1][col] != NON_SYMBOL)
            {
#ifdef DEBUG_DAY_3
                cout << "  Gear symbol " << m_schematic[row-1][col] << " found at row " << row-1 << " column " << col << endl;
#endif
                gears->add_part_number_to_gear(row-1, col, part_number);
            }
            // below row
            if (m_schematic[row+1][col] != NON_SYMBOL)
            {
#ifdef DEBUG_DAY_3
                cout << "  Gear symbol " << m_schematic[row+1][col] << " found at row " << row+1 << " column " << col << endl;
#endif
                gears->add_part_number_to_gear(row+1, col, part_number);
            }
        }
        return;
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

    void Schematic::match_part_numbers_to_gears(Gears * gears)
    {
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
                    load_part_number_to_gears(row, match_start_col, match_end_col, part_number, gears);
                }
            }
        }
        return;
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
