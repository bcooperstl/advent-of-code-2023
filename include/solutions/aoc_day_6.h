#ifndef __AOC_DAY_6__
#define __AOC_DAY_6__

#include <map>

#include "aoc_day.h"

namespace Day6
{
    class Race
    {
        private:
            int m_time;
            int m_distance;
        public:
            Race();
            ~Race();
            void set_time(int time);
            int get_time();
            void set_distance(int distance);
            int get_distance();
            int get_num_ways_to_record();
    };
}

using namespace Day6;

class AocDay6 : public AocDay
{
    private:
        vector<vector<string>> read_input(string filename);
        vector<Race> parse_races(vector<vector<string>> input_data);
    public:
        AocDay6();
        ~AocDay6();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
