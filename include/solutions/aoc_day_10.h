#ifndef __AOC_DAY_10__
#define __AOC_DAY_10__

#include <map>

#include "aoc_day.h"

#define DAY_10_DIRECTIONS 4

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
            Cell(char symbol);
            ~Cell();
            char get_symbol();
            bool is_start();
            bool can_connect(Direction direction);
            bool is_visited();
            int get_step_count();
            void set_step_count(int step_count);
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
        //string part2(string filename, vector<string> extra_args);
};

#endif
