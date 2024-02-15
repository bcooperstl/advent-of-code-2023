#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "aoc_day_11.h"
#include "file_utils.h"

using namespace std;
using namespace Day11;

#define SPACE '.'
#define GALAXY '#'

namespace Day11
{
    Galaxy::Galaxy(int id, int row, int col)
    {
        m_id = id;
        m_row = row;
        m_col = col;
    }
    
    Galaxy::~Galaxy()
    {
    }

    int Galaxy::get_id()
    {
        return m_id;
    }
    
    int Galaxy::get_row()
    {
        return m_row;
    }
    
    int Galaxy::get_col()
    {
        return m_col;
    }
    
    void Galaxy::adjust_for_empties(vector<int> & empty_rows, vector<int> & empty_cols)
    {
        int num_rows_to_add = 0;
        int num_cols_to_add = 0;
        for (int i=0; i<empty_rows.size(); i++)
        {
            if (empty_rows[i] < m_row)
            {
                num_rows_to_add++;
            }
        }
        for (int i=0; i<empty_cols.size(); i++)
        {
            if (empty_cols[i] < m_col)
            {
                num_cols_to_add++;
            }
        }
        
#ifdef DEBUG_DAY_11
        cout << "Adjust galaxy " << m_id << " from row=" << m_row << " col=" << m_col;
#endif
        m_row += num_rows_to_add;
        m_col += num_cols_to_add;
#ifdef DEBUG_DAY_11
        cout << " to row=" << m_row << " col=" << m_col << endl;
#endif
    }
    
    Galaxies::Galaxies()
    {
    }
    
    Galaxies::~Galaxies()
    {
    }
    
    void Galaxies::load_galaxies(vector<string> data)
    {
        int id=0;
        for (int row=0; row<data.size(); row++)
        {
            for (int col=0; col<data[row].size(); col++)
            {
                if (data[row][col] == GALAXY)
                {
                    id++;
#ifdef DEBUG_DAY_11
                    cout << "Loading galaxy " << id << " at row=" << row << " col=" << col << endl;
#endif
                    Galaxy galaxy(id, row, col);
                    m_galaxies.push_back(galaxy);
                }
            }
        }
    }
    
    vector<int> Galaxies::find_empty_rows(vector<string> data)
    {
        vector<int> empties;
        for (int row=0; row<data.size(); row++)
        {
            bool galaxy_in_row = false;
            for (int col=0; col<data[row].size(); col++)
            {
                if (data[row][col] == GALAXY)
                {
                    galaxy_in_row = true;
                    break;
                }
            }
            if (galaxy_in_row == false)
            {
#ifdef DEBUG_DAY_11
                cout << "Empty row found at row=" << row << endl;
#endif
                empties.push_back(row);
            }
        }
        return empties;
    }

    vector<int> Galaxies::find_empty_cols(vector<string> data)
    {
        vector<int> empties;
        for (int col=0; col<data[0].size(); col++)
        {
            bool galaxy_in_col = false;
            for (int row=0; row<data.size(); row++)
            {
                if (data[row][col] == GALAXY)
                {
                    galaxy_in_col = true;
                    break;
                }
            }
            if (galaxy_in_col == false)
            {
#ifdef DEBUG_DAY_11
                cout << "Empty col found at col=" << col << endl;
#endif
                empties.push_back(col);
            }
        }
        return empties;
    }

    int Galaxies::get_steps(Galaxy * left, Galaxy * right)
    {
        int row_diff = abs(left->get_row() - right->get_row());
        int col_diff = abs(left->get_col() - right->get_col());
        return row_diff+col_diff;
    }
    
    void Galaxies::adjust_all_for_empties(vector<int> & empty_rows, vector<int> & empty_cols)
    {
        for (int i=0; i<m_galaxies.size(); i++)
        {
            m_galaxies[i].adjust_for_empties(empty_rows, empty_cols);
        }
    }
    
    int Galaxies::find_sum_of_distances()
    {
        int sum = 0;
        
        for (int first_idx=0; first_idx < (m_galaxies.size() - 1); first_idx++)
        {
            for (int second_idx=first_idx+1; second_idx < m_galaxies.size(); second_idx++)
            {
                int steps = get_steps(&m_galaxies[first_idx], &m_galaxies[second_idx]);
                sum+=steps;
#ifdef DEBUG_DAY_11
                cout << "Galaxy " << m_galaxies[first_idx].get_id() 
                     << " and Galaxy " << m_galaxies[second_idx].get_id() 
                     << " are " << steps << " steps apart" << endl;
#endif

            }
        }
        return sum;
    }
}

AocDay11::AocDay11():AocDay(11)
{
}

AocDay11::~AocDay11()
{
}

vector<string> AocDay11::read_input(string filename)
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

string AocDay11::part1(string filename, vector<string> extra_args)
{
    vector<string> data = read_input(filename);
    
    Galaxies galaxies;
    galaxies.load_galaxies(data);
    
    vector<int> empty_rows = galaxies.find_empty_rows(data);
    vector<int> empty_cols = galaxies.find_empty_cols(data);
    
    galaxies.adjust_all_for_empties(empty_rows, empty_cols);
    
    ostringstream out;
    out << galaxies.find_sum_of_distances();
    return out.str();
}
