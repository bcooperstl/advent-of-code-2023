#ifndef __AOC_DAY_4__
#define __AOC_DAY_4__

#include "aoc_day.h"

#define DAY_4_MAX_WINNING_NUMBERS 10
#define DAY_4_MAX_MY_NUMBERS 25

namespace Day4
{
    class Card
    {
        private:
            int m_card_number;
            int m_winning_numbers[DAY_4_MAX_WINNING_NUMBERS];
            int m_num_winning_numbers;
            int m_my_numbers[DAY_4_MAX_MY_NUMBERS];
            int m_num_my_numbers;
        public:
            Card();
            ~Card();
            void load_from_input(vector<string> input_data);
            vector<int> get_matched_numbers();
            int get_card_number();
    };
}

using namespace Day4;

class AocDay4 : public AocDay
{
    private:
        vector<vector<string>> read_input(string filename);
    public:
        AocDay4();
        ~AocDay4();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
