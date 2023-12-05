#ifndef __AOC_DAY_3__
#define __AOC_DAY_3__

#include "aoc_day.h"

#define DAY_3_MAX_LENGTH 140

namespace Day3
{
    class Gear
    {
        private:
            int m_row;
            int m_col;
            vector<int> m_part_numbers;
        public:
            Gear(int row, int col);
            ~Gear();
            void add_part_number(int part_number);
            bool is_valid_gear();
            int get_gear_ratio();
            int get_row();
            int get_col();
    };
    
    class Gears
    {
        private:
            vector<Gear *> m_gears;
        public:
            Gears();
            ~Gears();
            vector<Gear *> get_valid_gears();
            void add_part_number_to_gear(int row, int col, int part_number);
    };
    
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
            void load_part_number_to_gears(int row, int start_col, int end_col, int part_number, Gears * gears);
        public:
            Schematic();
            ~Schematic();
            void load_schematic(vector<string> input_data);
            void display_schematic();
            vector<int> find_part_numbers();
            void match_part_numbers_to_gears(Gears * gears);
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
