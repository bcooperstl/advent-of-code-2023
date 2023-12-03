#ifndef __AOC_DAY_2__
#define __AOC_DAY_2__

#include "aoc_day.h"

#define DAY_2_NUM_COLORS 3

namespace Day2
{
    enum Color
    {
        red = 0,
        green = 1,
        blue = 2
    };
    
    class Round
    {
        private:
            int m_values[DAY_2_NUM_COLORS];
        public:
            Round();
            ~Round();
            void set_count(Color color, int value);
            int get_count(Color color);
            bool is_possible(int num_red, int num_green, int num_blue);
    };
    
    class Game
    {
        private:
            vector<Round *> m_rounds;
            int m_id;
            Round calculate_minimal_round();
        public:
            Game(int id);
            ~Game();
            int get_id();
            void add_round(Round * round);
            bool is_possible(int num_red, int num_green, int num_blue);
            int get_power_for_minimal_round();
    };
    
}

using namespace Day2;

class AocDay2 : public AocDay
{
    private:
        vector<vector<string>> read_input(string filename);
        Game * create_game(vector<string> input_line);
    public:
        AocDay2();
        ~AocDay2();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
