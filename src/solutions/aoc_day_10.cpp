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
    Cell::Cell()
    {
        m_symbol = GROUND;
        m_step_count = NO_STEPS;
    }
    
    Cell::~Cell()
    {
    }
    
    char Cell::get_symbol()
    {
        return m_symbol;
    }
    
    void Cell::set_symbol(char symbol)
    {
        m_symbol = symbol;
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
    
    Area::Area()
    {
        m_rows = 0;
        m_cols = 0;
    }
    
    Area::~Area()
    {
    }
    
    int Area::get_rows()
    {
        return m_rows;
    }
    
    int Area::get_cols()
    {
        return m_cols;
    }
    
    void Area::load_area(vector<string> data)
    {
        m_rows = data.size() + 2;
        m_cols = data[0].length() + 2;
#ifdef DEBUG_DAY_10
        cout << "Loading for data of " << data.size() << " rows and " << data[0].length() << " cols" << endl;
        cout << "Adding padding gives " << m_rows << " rows and " << m_cols << " cols" << endl;
#endif
        // Set the top and bottom rows to be ground
        for (int col=0; col<m_cols; col++)
        {
            m_map[0][col].set_symbol(GROUND);
            m_map[m_rows-1][col].set_symbol(GROUND);
        }
        
        // load the data; row and col refer to destination, which are each one more than source
        for (int row=1; row<(m_rows-1); row++)
        {
            // set ground in first and last column
            m_map[row][0].set_symbol(GROUND);
            m_map[row][m_cols-1].set_symbol(GROUND);
            for (int col=1; col<(m_cols-1); col++)
            {
                m_map[row][col].set_symbol(data[row-1][col-1]);
            }
        }

#ifdef DEBUG_DAY_10
        cout << "After loading, the area is:" << endl;
        display();
#endif
    }
    
    void Area::display(bool visited_only)
    {
        for (int row=0; row<m_rows; row++)
        {
            for (int col=0; col<m_cols; col++)
            {
                if ((visited_only == false) ||
                    (visited_only == true && m_map[row][col].is_visited() == true))
                {
                    cout << m_map[row][col].get_symbol();
                }
                else
                {
                    cout << GROUND;
                }
            }
            cout << endl;
        }
    }
    
    Cell * Area::get_cell(int row, int col)
    {
        return &(m_map[row][col]);
    }
    
    PathSolver::PathSolver(Area * area)
    {
        m_area = area;
        m_steps = 0;
    }
    
    PathSolver::~PathSolver()
    {
    }
    
    void PathSolver::init_start()
    {
        for (int row=0; row<m_area->get_rows(); row++)
        {
            for (int col=0; col<m_area->get_cols(); col++)
            {
                if (m_area->get_cell(row, col)->is_start())
                {
                    m_start.row = row;
                    m_start.col = col;
                    m_area->get_cell(row, col)->set_step_count(0);
#ifdef DEBUG_DAY_10
                    cout << "Start found at row=" << row << " col=" << col << endl;
                    m_area->display(true);
#endif
                }
            }
        }
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
    
    Area area;
    area.load_area(data);
    
    PathSolver solver(&area);
    solver.init_start();
    
    ostringstream out;
    out << 0;
    return out.str();
}
