#ifndef __AOC_DAY_1__
#define __AOC_DAY_1__

#include "aoc_day.h"

class AocDay1 : public AocDay
{
    private:
        vector<string> read_input(string filename);
        long get_calibration_value(string line);
        long get_calibration_value_with_words(string line);
    public:
        AocDay1();
        ~AocDay1();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
