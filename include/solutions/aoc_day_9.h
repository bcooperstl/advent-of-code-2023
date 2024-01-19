#ifndef __AOC_DAY_9__
#define __AOC_DAY_9__

#include <map>

#include "aoc_day.h"

#define DAY_9_MAX_VALUES 32

namespace Day9
{
    class ValueHistory
    {
        private:
            long m_values[DAY_9_MAX_VALUES];
            int m_num_values;
        public:
            ValueHistory();
            ~ValueHistory();
            long extrapolate_next();
//            long extrapolate_prior();
            void load_history(vector<long> input_data);
    };
    
    class Values
    {
        private:
            vector<ValueHistory> m_values;
        public:
            Values();
            ~Values();
            void init_history(vector<vector<long>> input_data);
            long get_sum_extrapolated_next_values();
//            long get_sum_extrapolated_prior_values();
    };
}

using namespace Day9;

class AocDay9 : public AocDay
{
    private:
        vector<vector<long>> read_input(string filename);
    public:
        AocDay9();
        ~AocDay9();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
