#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>

#include "aoc_day_12.h"
#include "file_utils.h"

using namespace std;
using namespace Day12;

#define OPERATIONAL '.'
#define DAMAGED '#'
#define UNKNOWN '?'

namespace Day12
{
    Group::Group()
    {
        m_condition_length = 0;
        m_num_groups = 0;
    }
    
    Group::~Group()
    {
    }

    void Group::load_group(vector<string> data)
    {
        strncpy(m_conditions, data[0].c_str(), DAY12_MAX_LENGTH);
        m_condition_length = data[0].length();
        for (int i=1; i<data.size(); i++)
        {
            m_groups[i-1] = strtol(data[i].c_str(), NULL, 10);
        }
        m_num_groups = data.size()-1;
        
#ifdef DEBUG_DAY_12
        display();
#endif
    }
    
    void Group::display()
    {
        cout << "Group " << m_conditions << " has groups of:";
        for (int i=0; i<m_num_groups; i++)
        {
            cout << ' ' << m_groups[i];
        }
        cout << endl;
    }
    
    bool Group::can_be_operational(int pos)
    {
        return (m_conditions[pos] == OPERATIONAL || m_conditions[pos] == UNKNOWN);
    }
    
    bool Group::can_be_damaged(int pos)
    {
        return (m_conditions[pos] == DAMAGED || m_conditions[pos] == UNKNOWN);
    }
    
    void Group::find_starting_positions(Positions * positions)
    {
#ifdef DEBUG_DAY_12
        cout << "Finding starting positions for ";
        display();
#endif
        for (int i=1; i<=DAY12_MAX_GROUP_LENGTH; i++)
        {
            positions->processed[i] = false;
            positions->num_good_positions[i] = 0;
        }
        
        for (int group=0; group<m_num_groups; group++)
        {
            int length = m_groups[group];
            if (positions->processed[length] == true)
            {
#ifdef DEBUG_DAY_12
                cout << " Skipping group " << group << " because " << length << " is already processed" << endl;
#endif
                continue;
            }
#ifdef DEBUG_DAY_12
            cout << " Processing group " << group << " with value " << length << endl;
#endif
            for (int start=0; start<=m_condition_length-length; start++)
            {
#ifdef DEBUG_DAY_12
                cout << "  Checking at start " << start << " ";
#endif
                bool good = true;
                if (start > 0 && m_conditions[start-1] == DAMAGED)
                {
                    good = false;
#ifdef DEBUG_DAY_12
                    cout << "Does not work because of damaged at " << start - 1 << endl;
#endif
                    continue;
                }
                for (int pos=start; pos<start+length; pos++)
                {
                    if (!can_be_damaged(pos))
                    {
                        good = false;
#ifdef DEBUG_DAY_12
                        cout << "Does not work because of operational at " << pos << endl;
#endif
                        break;
                    }
                }
                if (good)
                {
#ifdef DEBUG_DAY_12
                    cout << "Works as a starting position" << endl;
#endif
                    positions->start_positions[length][positions->num_good_positions[length]] = start;
                    positions->num_good_positions[length]++;
                }
            }
            positions->processed[length] = true;
        }
    }
    
    int Group::get_num_arrangements()
    {
        Positions positions;
        find_starting_positions(&positions);
        
        return 0;
    }
    

    Groups::Groups()
    {
    }
    
    Groups::~Groups()
    {
    }
    
    void Groups::load_groups(vector<vector<string>> data)
    {
        for (int i=0; i<data.size(); i++)
        {
            Group group;
            group.load_group(data[i]);
            m_groups.push_back(group);
        }
    }
    
    int Groups::get_num_arrangements()
    {
        int num_arrangements = 0;
        for (int i=0; i<m_groups.size(); i++)
        {
            num_arrangements+=m_groups[i].get_num_arrangements();
        }
        return num_arrangements;
    }
}

AocDay12::AocDay12():AocDay(12)
{
}

AocDay12::~AocDay12()
{
}

vector<vector<string>> AocDay12::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> data;
    char delimiters[2];
    delimiters[0]=' ';
    delimiters[1]=',';
    
    // spaces and commas are delimiters
    if (!fileutils.read_as_list_of_split_strings(filename, data, delimiters, 2, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return data;
}

string AocDay12::part1(string filename, vector<string> extra_args)
{
    vector<vector<string>> data = read_input(filename);
    
    Groups groups;
    groups.load_groups(data);
    
    ostringstream out;
    out << groups.get_num_arrangements();
    return out.str();
}
