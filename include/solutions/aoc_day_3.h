#ifndef __AOC_DAY_3__
#define __AOC_DAY_3__

#include "aoc_day.h"

#define DAY_3_MAX_LENGTH 140

namespace Day3
{
    class Schematic
    {
        private:
            /* m_schematic has extra rows for the top and bottom padding lines
               m_schematic has extra cols for the left and right padding columns plus a null-terminator
             */
            char m_schematic[DAY_3_MAX_LENGTH+2][DAY_3_MAX_LENGTH+2+1];
            int m_num_rows; // actual number of rows with input data
            int m_num_cols; // actual number of cols with input data
            bool has_neighbor_symbol(int row, int start_col, int end_col);
        public:
            Schematic();
            ~Schematic();
            void load_schematic(vector<string> input_data);
            void display_schematic();
            vector<int> find_part_numbers();
    };    
}

using namespace Day3;

class AocDay3 : public AocDay
{
    private:
        vector<string> read_input(string filename);
    public:
        AocDay3();
        ~AocDay3();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
