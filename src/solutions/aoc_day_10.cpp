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
#define INSIDE 'I'
#define OUTSIDE 'O'

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
    
    int Area::get_inside_count()
    {
        int num_inside = 0;
        for (int row=0; row<m_rows; row++)
        {
            for (int col=0; col<m_cols; col++)
            {
                if (m_map[row][col].get_symbol() == INSIDE)
                {
                    num_inside++;
                }
            }
        }
        return num_inside;
    }
    
    void Area::replace_start_with_symbol()
    {
        int start_row;
        int start_col;
        for (int row=0; row<m_rows; row++)
        {
            for (int col=0; col<m_cols; col++)
            {
                if (m_map[row][col].is_start())
                {
                    start_row = row;
                    start_col = col;
                }
            }
        }
        
        // north south
        if ((m_map[start_row-1][start_col].is_visited() == true) &&
            (m_map[start_row-1][start_col].get_step_count() == 1) &&
            (m_map[start_row+1][start_col].is_visited() == true) &&
            (m_map[start_row+1][start_col].get_step_count() == 1))
        {
            m_map[start_row][start_col].set_symbol(VERTICAL);
        }
        // north east
        else if ((m_map[start_row-1][start_col].is_visited() == true) &&
            (m_map[start_row-1][start_col].get_step_count() == 1) &&
            (m_map[start_row][start_col+1].is_visited() == true) &&
            (m_map[start_row][start_col+1].get_step_count() == 1))
        {
            m_map[start_row][start_col].set_symbol(NORTH_EAST);
        }
        // north west
        else if ((m_map[start_row-1][start_col].is_visited() == true) &&
            (m_map[start_row-1][start_col].get_step_count() == 1) &&
            (m_map[start_row][start_col-1].is_visited() == true) &&
            (m_map[start_row][start_col-1].get_step_count() == 1))
        {
            m_map[start_row][start_col].set_symbol(NORTH_WEST);
        }
        // south east
        else if ((m_map[start_row+1][start_col].is_visited() == true) &&
            (m_map[start_row+1][start_col].get_step_count() == 1) &&
            (m_map[start_row][start_col+1].is_visited() == true) &&
            (m_map[start_row][start_col+1].get_step_count() == 1))
        {
            m_map[start_row][start_col].set_symbol(SOUTH_EAST);
        }
        // south west
        else if ((m_map[start_row+1][start_col].is_visited() == true) &&
            (m_map[start_row+1][start_col].get_step_count() == 1) &&
            (m_map[start_row][start_col-1].is_visited() == true) &&
            (m_map[start_row][start_col-1].get_step_count() == 1))
        {
            m_map[start_row][start_col].set_symbol(SOUTH_WEST);
        }
        // east west
        else if ((m_map[start_row][start_col+1].is_visited() == true) &&
            (m_map[start_row][start_col+1].get_step_count() == 1) &&
            (m_map[start_row][start_col-1].is_visited() == true) &&
            (m_map[start_row][start_col-1].get_step_count() == 1))
        {
            m_map[start_row][start_col].set_symbol(HORIZONTAL);
        }
        else
        {
            cerr << "Unable to determine replacement symbol for start" << endl;
        }
#ifdef DEBUG_DAY_10
        cout << "Replaced start symbol at row=" << start_row << " col=" << start_col << " with " << m_map[start_row][start_col].get_symbol() << endl;
        display();
#endif            
    }
    
    void Area::set_inside_outside()
    {
        replace_start_with_symbol();

        // First do the rows left to right
        for (int row=0; row<m_rows; row++)
        {
#ifdef DEBUG_DAY_10
            cout << "Checking for inside cell on row " << row << endl;
#endif            
            int num_crossed = 0;
            char last_vertical = GROUND;
            for (int col=0; col<m_cols; col++)
            {                
                // on the border
                if (m_map[row][col].is_visited())
                {
                    char symbol = m_map[row][col].get_symbol();
                    if (symbol == VERTICAL)
                    {
                        num_crossed++;
#ifdef DEBUG_DAY_10
                        cout << " Crossing vertical at col " << col << "; increasing num_crossed=" << num_crossed << endl;
#endif            
                    }
                    else if (symbol == NORTH_WEST)
                    {
                        if (last_vertical == SOUTH_EAST)
                        {
                            num_crossed++;
#ifdef DEBUG_DAY_10
                            cout << " Crossing north_west at col " << col << " with prior south_east; increase num_crossed=" << num_crossed << endl;
#endif
                        }
                        else if (last_vertical == NORTH_EAST)
                        {
#ifdef DEBUG_DAY_10
                            cout << " Crossing north_west at col " << col << " with prior north_east; keeping num_crossed=" << num_crossed << endl;
#endif
                        }
                        else
                        {
                            cerr << "*****ERROR with NORTH_WEST at row=" << row << " col=" << col << endl;
                        }
                    }
                    else if (symbol == SOUTH_WEST)
                    {
                        if (last_vertical == NORTH_EAST)
                        {
                            num_crossed++;
#ifdef DEBUG_DAY_10
                            cout << " Crossing south_west at col " << col << " with prior north_east; increase num_crossed=" << num_crossed << endl;
#endif
                        }
                        else if (last_vertical == SOUTH_EAST)
                        {
#ifdef DEBUG_DAY_10
                            cout << " Crossing south_west at col " << col << " with prior south_east; keeping num_crossed=" << num_crossed << endl;
#endif
                        }
                        else
                        {
                            cerr << "*****ERROR with SOUTH_WEST at row=" << row << " col=" << col << endl;
                        }
                    }
                    else if (symbol == SOUTH_EAST)
                    {
                        last_vertical = SOUTH_EAST;
#ifdef DEBUG_DAY_10
                        cout << " Setting last_vertical to south_east at col " << col << endl;
#endif
                    }
                    else if (symbol == NORTH_EAST)
                    {
                        last_vertical = NORTH_EAST;
#ifdef DEBUG_DAY_10
                        cout << " Setting last_vertical to north_east at col " << col << endl;
#endif
                    }
                }
                else
                {
                    if ((num_crossed % 2) == 0)
                    {
                        m_map[row][col].set_symbol(OUTSIDE);
#ifdef DEBUG_DAY_10
                        cout << " Setting col " << col << " to outside because num_crossed=" << num_crossed << endl;
#endif
                    }
                    else
                    {
                        m_map[row][col].set_symbol(INSIDE);
#ifdef DEBUG_DAY_10
                        cout << " Setting col " << col << " to inside because num_crossed=" << num_crossed << endl;
#endif
                    }
                }
            }
#ifdef DEBUG_DAY_10
            display();
#endif            
        }
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
        m_ends[0].row = m_start.row;
        m_ends[0].col = m_start.col;
        m_ends[1].row = m_start.row;
        m_ends[1].col = m_start.col;        
    }

    bool PathSolver::advance_path()
    {
        bool advanced[2] = {true, true};
        
        for (int i=0; i<2; i++)
        {
#ifdef DEBUG_DAY_10
            cout << "Checking neighbors from level " << m_steps 
                 << " for end " << i 
                 << " at row=" << m_ends[i].row << " col=" << m_ends[i].col << endl;
#endif
            Cell * current = m_area->get_cell(m_ends[i].row, m_ends[i].col);
            Cell * neighbors[DAY_10_DIRECTIONS];
            neighbors[north] = m_area->get_cell(m_ends[i].row-1, m_ends[i].col);
            neighbors[east] = m_area->get_cell(m_ends[i].row, m_ends[i].col+1);
            neighbors[south] = m_area->get_cell(m_ends[i].row+1, m_ends[i].col);
            neighbors[west] = m_area->get_cell(m_ends[i].row, m_ends[i].col-1);
            
            if (current->can_connect(north) && 
                neighbors[north]->can_connect(south) && 
                (!neighbors[north]->is_visited()))
            {
#ifdef DEBUG_DAY_10
                cout << " Current symbol " << current->get_symbol() 
                     << " can connect north to symbol " << neighbors[north]->get_symbol() 
                     << " at row=" << m_ends[i].row-1 << " col=" << m_ends[i].col << endl;
#endif
                advanced[i] = true;
                neighbors[north]->set_step_count(m_steps+1);
                m_ends[i].row = m_ends[i].row-1;
                m_ends[i].col = m_ends[i].col;
            }
            else if (current->can_connect(east) && 
                neighbors[east]->can_connect(west) && 
                (!neighbors[east]->is_visited()))
            {
#ifdef DEBUG_DAY_10
                cout << " Current symbol " << current->get_symbol() 
                     << " can connect east to symbol " << neighbors[east]->get_symbol() 
                     << " at row=" << m_ends[i].row << " col=" << m_ends[i].col+1 << endl;
#endif
                advanced[i] = true;
                neighbors[east]->set_step_count(m_steps+1);
                m_ends[i].row = m_ends[i].row;
                m_ends[i].col = m_ends[i].col+1;
            }
            else if (current->can_connect(south) && 
                neighbors[south]->can_connect(north) && 
                (!neighbors[south]->is_visited()))
            {
#ifdef DEBUG_DAY_10
                cout << " Current symbol " << current->get_symbol() 
                     << " can connect south to symbol " << neighbors[south]->get_symbol() 
                     << " at row=" << m_ends[i].row+1 << " col=" << m_ends[i].col << endl;
#endif
                advanced[i] = true;
                neighbors[south]->set_step_count(m_steps+1);
                m_ends[i].row = m_ends[i].row+1;
                m_ends[i].col = m_ends[i].col;
            }
            else if (current->can_connect(west) && 
                neighbors[west]->can_connect(east) && 
                (!neighbors[west]->is_visited()))
            {
#ifdef DEBUG_DAY_10
                cout << " Current symbol " << current->get_symbol() 
                     << " can connect west to symbol " << neighbors[west]->get_symbol() 
                     << " at row=" << m_ends[i].row << " col=" << m_ends[i].col-1 << endl;
#endif
                advanced[i] = true;
                neighbors[west]->set_step_count(m_steps+1);
                m_ends[i].row = m_ends[i].row;
                m_ends[i].col = m_ends[i].col-1;
            }
            else
            {
#ifdef DEBUG_DAY_10
                cout << " Current symbol " << current->get_symbol() 
                     << " cannot connect to any neighbors" << endl;
#endif                
                advanced[i] = false;
            }
        }
        if (advanced[0] == true || advanced[1] == true)
        {
            m_steps++;
        }
#ifdef DEBUG_DAY_10_STEPS
        cout << "After step " << m_steps << " the area is:" << endl;
        m_area->display(true);
#endif
        return ((advanced[0] == true) && (advanced[1] == true));
    }

    int PathSolver::get_steps()
    {
        return m_steps;
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
    bool keep_going = true;
    while (keep_going)
    {
        keep_going = solver.advance_path();
    }
    
#ifdef DEBUG_DAY_10
    cout << "Final pattern:" << endl;
    area.display(true);
#endif

    ostringstream out;
    out << solver.get_steps();
    return out.str();
}

string AocDay10::part2(string filename, vector<string> extra_args)
{
    vector<string> data = read_input(filename);
    
    Area area;
    area.load_area(data);
    
    PathSolver solver(&area);
    solver.init_start();
    bool keep_going = true;
    while (keep_going)
    {
        keep_going = solver.advance_path();
    }
    
#ifdef DEBUG_DAY_10
    cout << "Final pattern:" << endl;
    area.display(true);
#endif

    area.set_inside_outside();

    ostringstream out;
    out << area.get_inside_count();
    return out.str();
}
