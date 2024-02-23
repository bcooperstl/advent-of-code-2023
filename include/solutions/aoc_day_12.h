#ifndef __AOC_DAY_12__
#define __AOC_DAY_12__

#include <map>

#include "aoc_day.h"

#define DAY12_MAX_LENGTH 50
#define DAY12_MAX_GROUPS 10

namespace Day12
{
    class Group
    {
        private:
            char m_conditions[DAY12_MAX_LENGTH+1];
            int m_condition_length;
            int m_groups[DAY12_MAX_GROUPS];
            int m_num_groups;
            bool can_be_operational(int pos);
            bool can_be_damaged(int pos);
        public:
            Group();
            ~Group();
            void load_group(vector<string> data);
    };
    
    class Groups
    {
        private:
            vector<Group> m_groups;
        public:
            Groups();
            ~Groups();
            void load_groups(vector<vector<string>> data);
    };
}

using namespace Day12;

class AocDay12 : public AocDay
{
    private:
        vector<vector<string>> read_input(string filename);
    public:
        AocDay12();
        ~AocDay12();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
