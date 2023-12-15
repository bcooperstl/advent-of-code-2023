#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_6.h"
#include "file_utils.h"

using namespace std;
using namespace Day6;

namespace Day6
{
    Race::Race()
    {
        m_time=0;
        m_distance=0;
    }
    
    Race::~Race()
    {
    }
    
    void Race::set_time(int time)
    {
        m_time = time;
    }
    
    int Race::get_time()
    {
        return m_time;
    }
    
    void Race::set_distance(int distance)
    {
        m_distance = distance;
    }
    
    int Race::get_distance()
    {
        return m_distance;
    }
    
    int Race::get_num_ways_to_record()
    {
#ifdef DEBUG_DAY_6
                cout << "Searching for ways to get record for race with time " << m_time << " and record distance " << m_distance << endl;
#endif

        int ways = 0;
        for (int i=0; i<=m_time; i++)
        {
            int distance = i * (m_time - i);
            if (distance > m_distance)
            {
#ifdef DEBUG_DAY_6
                cout << " Time " << i << " results in distance " << distance << " which is a record" << endl;
                ways++;
#endif
            }
        }                    
        
        return ways;
    }
    
}

AocDay6::AocDay6():AocDay(6)
{
}

AocDay6::~AocDay6()
{
}

vector<vector<string>> AocDay6::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> data;
    if (!fileutils.read_as_list_of_split_strings(filename, data, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return data;
}

/*
Time:      7  15   30
Distance:  9  40  200
// create races from time at [0][i] and distance at [1][i]
*/
vector<Race> AocDay6::parse_races(vector<vector<string>> input_data)
{
    vector<Race> races;
    for (int i=1; i<input_data[0].size(); i++)
    {
        Race race;
        race.set_time(strtol(input_data[0][i].c_str(), NULL, 10));
        race.set_distance(strtol(input_data[1][i].c_str(), NULL, 10));

        races.push_back(race);
#ifdef DEBUG_DAY_6
        cout << "Race " << i << " has time " << race.get_time() << " and record distance " << race.get_distance() << endl;
#endif
    }

    return races;
}


string AocDay6::part1(string filename, vector<string> extra_args)
{
    vector<vector<string>> data = read_input(filename);
        
    vector<Race> races = parse_races(data);
    
    int product = 1;
    
    for (int i=0; i<races.size(); i++)
    {
        product *= races[i].get_num_ways_to_record();
    }
    
    ostringstream out;
    out << product;
    return out.str();
}
