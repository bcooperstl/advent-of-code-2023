#ifndef __AOC_DAY_12__
#define __AOC_DAY_12__

#include <map>

#include "aoc_day.h"

#define DAY12_MAX_LENGTH 50
#define DAY12_MAX_GROUPS 10
#define DAY12_MAX_GROUP_LENGTH 16

namespace Day12
{
    struct Positions
    {
        bool processed[DAY12_MAX_GROUP_LENGTH+1];
        int start_positions[DAY12_MAX_GROUP_LENGTH+1][DAY12_MAX_LENGTH];
        int num_good_positions[DAY12_MAX_GROUP_LENGTH+1];
    };
    
    class Group
    {
        private:
            char m_conditions[DAY12_MAX_LENGTH+1];
            int m_condition_length;
            int m_groups[DAY12_MAX_GROUPS];
            int m_num_groups;
            bool can_be_operational(int pos);
            bool can_be_damaged(int pos);
            void find_starting_positions(Positions * positions);
            void display();
            int get_num_arrangements_recursive(Positions * positions, int group_number, int start_position);
        public:
            Group();
            ~Group();
            void load_group(vector<string> data);
            int get_num_arrangements();
    };
    
    class Groups
    {
        private:
            vector<Group> m_groups;
        public:
            Groups();
            ~Groups();
            void load_groups(vector<vector<string>> data);
            int get_num_arrangements();
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
