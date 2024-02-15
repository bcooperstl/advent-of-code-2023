#ifndef __AOC_DAY_10__
#define __AOC_DAY_10__

#include <map>

#include "aoc_day.h"

#define DAY_10_DIRECTIONS 4
#define DAY_10_MAX_EDGE_LENGTH 150

namespace Day10
{
    enum Direction
    {
        north = 0,
        east = 1,
        south = 2,
        west = 3
    };
        
    class Cell
    {
        private:
            char m_symbol;
            int m_step_count;
        public:
            Cell();
            ~Cell();
            char get_symbol();
            void set_symbol(char symbol);
            bool is_start();
            bool can_connect(Direction direction);
            bool is_visited();
            int get_step_count();
            void set_step_count(int step_count);
    };
    
    class Area
    {
        private:
            Cell m_map[DAY_10_MAX_EDGE_LENGTH][DAY_10_MAX_EDGE_LENGTH];
            int m_rows;
            int m_cols;
            void replace_start_with_symbol();
        public:
            Area();
            ~Area();
            int get_rows();
            int get_cols();
            void load_area(vector<string> data);
            void display(bool visited_only = false);
            Cell * get_cell(int row, int col);
            int get_inside_count();
            void set_inside_outside();
    };
    
    struct Location
    {
        int row;
        int col;
    };
    
    class PathSolver
    {
        private:
            Area * m_area;
            Location m_start;
            int m_steps;
            Location m_ends[2];
        public:
            PathSolver(Area * area);
            ~PathSolver();
            void init_start();
            bool advance_path();
            int get_steps();
    };
}

using namespace Day10;

class AocDay10 : public AocDay
{
    private:
        vector<string> read_input(string filename);
    public:
        AocDay10();
        ~AocDay10();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
