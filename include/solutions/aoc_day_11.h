#ifndef __AOC_DAY_11__
#define __AOC_DAY_11__

#include <map>

#include "aoc_day.h"

namespace Day11
{
    class Galaxy
    {
        private:
            int m_id;
            int m_row;
            int m_col;
        public:
            Galaxy(int id, int row, int col);
            ~Galaxy();
            void adjust_for_empties(vector<int> & empty_rows, vector<int> & empty_cols);
            int get_id();
            int get_row();
            int get_col();
    };
    
    class Galaxies
    {
        private:
            vector<Galaxy> m_galaxies;
            int get_steps(Galaxy * left, Galaxy * right);
        public:
            Galaxies();
            ~Galaxies();
            void load_galaxies(vector<string> data);
            void adjust_all_for_empties(vector<int> & empty_rows, vector<int> & empty_cols);
            vector<int> find_empty_rows(vector<string> data);
            vector<int> find_empty_cols(vector<string> data);
            int find_sum_of_distances();
    };
}

using namespace Day11;

class AocDay11 : public AocDay
{
    private:
        vector<string> read_input(string filename);
    public:
        AocDay11();
        ~AocDay11();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
