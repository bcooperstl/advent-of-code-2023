#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "aoc_day_9.h"
#include "file_utils.h"

using namespace std;
using namespace Day9;

namespace Day9
{
    ValueHistory::ValueHistory()
    {
        m_num_values = 0;
    }
    
    ValueHistory::~ValueHistory()
    {
    }
    
    void ValueHistory::load_history(vector<long> input_data)
    {
        for (int i=0; i<input_data.size(); i++)
        {
            m_values[i] = input_data[i];
        }
        m_num_values = input_data.size();
    }
    
/*
write_row = 1
0 3 6 9 12 15
3 3 3 3 3
0 0 0 0 0
table[1][5] = table[1][4] + table[2][4]

write_row = 0
0 3 6 9 12 15
3 3 3 3 3 3
0 0 0 0 0
table[0][6] = table[0][5] + table[1][5]

table[write_row][m_num_values-write_row] = table[write_row][m_num_values-write_row-1] + table[write_row+1][m_num_values-write_row-1]


*/


    long ValueHistory::extrapolate_next()
    {
        long work_table[DAY_9_MAX_VALUES+1][DAY_9_MAX_VALUES+1];
#ifdef DEBUG_DAY_9
        cout << "Working history for [";
#endif
        for (int i=0; i<m_num_values; i++)
        {
            work_table[0][i] = m_values[i];
#ifdef DEBUG_DAY_9
        cout << " " << m_values[i];
#endif
        }
#ifdef DEBUG_DAY_9
        cout << " ]" << endl;
#endif
        
        bool all_zero = false;
        int write_row = 1;
        while (!all_zero)
        {
            all_zero = true;
#ifdef DEBUG_DAY_9
            cout << "Row " << write_row << " is [";
#endif
            for (int i=0; i<(m_num_values-write_row); i++)
            {
                work_table[write_row][i] = work_table[write_row-1][i+1]-work_table[write_row-1][i];
                if (work_table[write_row][i] != 0)
                {
                    all_zero = false;
                }
#ifdef DEBUG_DAY_9
                cout << " " << work_table[write_row][i];
#endif
            }
#ifdef DEBUG_DAY_9
            cout << " ]" << endl;
#endif
            if (!all_zero)
            {
                write_row++;
            }
        }
#ifdef DEBUG_DAY_9
        cout << "All zero row found on write row " << write_row << endl;
#endif

        work_table[write_row][m_num_values-write_row] = 0;
#ifdef DEBUG_DAY_9
        cout << "Extrapolated 0 to row " << write_row << endl; 
#endif        
        write_row--;

        while (write_row >= 0)
        {
            
            work_table[write_row][m_num_values-write_row] = work_table[write_row][m_num_values-write_row-1] + work_table[write_row+1][m_num_values-write_row-1];
#ifdef DEBUG_DAY_9
            cout << "Extrapolated " << work_table[write_row][m_num_values-write_row-1] << " + " 
                 << work_table[write_row+1][m_num_values-write_row-1] << " = " 
                 << work_table[write_row][m_num_values-write_row] << " to row " << write_row << endl;
#endif
            write_row--;
        }
        
#ifdef DEBUG_DAY_9
        cout << "Extrapolated value is " << work_table[0][m_num_values] << endl;
#endif
        return work_table[0][m_num_values];
    }
    
    Values::Values()
    {
    }
    
    Values::~Values()
    {
    }
    
    void Values::init_history(vector<vector<long>> input_data)
    {
        for (int i=0; i<input_data.size(); i++)
        {
            ValueHistory vh;
            vh.load_history(input_data[i]);
            m_values.push_back(vh);
        }
    }

    long Values::get_sum_extrapolated_next_values()
    {
        long sum = 0;
        for (int i=0; i<m_values.size(); i++)
        {
            sum+=m_values[i].extrapolate_next();
        }
        return sum;
    }
}

AocDay9::AocDay9():AocDay(9)
{
}

AocDay9::~AocDay9()
{
}

vector<vector<long>> AocDay9::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<long>> data;
    // space is my delimiter
    if (!fileutils.read_as_list_of_split_longs(filename, data, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return data;
}

string AocDay9::part1(string filename, vector<string> extra_args)
{
    vector<vector<long>> data = read_input(filename);
    Values values;
    
    values.init_history(data);
    
    ostringstream out;
    out << values.get_sum_extrapolated_next_values();
    return out.str();
}
