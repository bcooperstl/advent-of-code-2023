#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>

#include "aoc_day_5.h"
#include "file_utils.h"

using namespace std;
using namespace Day5;

namespace Day5
{
    Path::Path()
    {
    }
    
    Path::~Path()
    {
    }
    
    long long int Path::get_value(Category category)
    {
        return m_values[category];
    }
    
    void Path::set_value(Category category, long long int value)
    {
        m_values[category] = value;
    }

    Range::Range()
    {
    }
    
    Range::~Range()
    {
    }
    
    void Range::init_range(vector<string> input_data)
    {
        long long int destination_range_start = strtoll(input_data[0].c_str(), NULL, 10);
        long long int source_range_start = strtoll(input_data[1].c_str(), NULL, 10);
        long long int range_length = strtoll(input_data[2].c_str(), NULL, 10);
        
        m_source_start = source_range_start;
        m_source_end = source_range_start + range_length - 1ll; // 1 long long
        m_adjustment = destination_range_start - source_range_start;
        
#ifdef DEBUG_DAY_5
        cout << "Range line [" << destination_range_start << " " << source_range_start << " " << range_length
             << "] corresponds to source_start=" << m_source_start 
             << " source_end=" << m_source_end 
             << " adjustment=" << m_adjustment << endl;
#endif
        return;
    }
    
    bool Range::is_in_range(long long int value)
    {
        return (value >= m_source_start && value <= m_source_end);
    }
    
    long long int Range::calculate_destination(long long int source)
    {
        return source + m_adjustment;
    }
    
    CategoryMap::CategoryMap(Category source, Category destination)
    {
        m_source = source;
        m_destination = destination;
    }
    
    CategoryMap::~CategoryMap()
    {
        for (int i=0; i<m_ranges.size(); i++)
        {
            delete m_ranges[i];
        }
    }
    
    void CategoryMap::add_range(Range * range)
    {
        m_ranges.push_back(range);
    }
    
    vector<Range *> CategoryMap::get_ranges()
    {
        return m_ranges;
    }
    
    long long int CategoryMap::apply_map(long long int value)
    {
#ifdef DEBUG_DAY_5
        cout << "Applying category " << m_source << " to " << m_destination << " map to value " << value;
#endif
        for (int i=0; i<m_ranges.size(); i++)
        {
            if (m_ranges[i]->is_in_range(value))
            {
                long long int applied_value = m_ranges[i]->calculate_destination(value);
#ifdef DEBUG_DAY_5
                cout << "  Maps to " << applied_value << endl;
#endif
                return applied_value;
            }
        }        
#ifdef DEBUG_DAY_5
        cout << "  No range apples; keeping as " << value << endl;
#endif        
        return value;
    }
    
    Almanac::Almanac()
    {
        m_maps[seed] = new CategoryMap(seed, soil);
        m_maps[soil] = new CategoryMap(soil, fertilizer);
        m_maps[fertilizer] = new CategoryMap(fertilizer, water);
        m_maps[water] = new CategoryMap(water, light);
        m_maps[light] = new CategoryMap(light, temperature);
        m_maps[temperature] = new CategoryMap(temperature, humidity);
        m_maps[humidity] = new CategoryMap(humidity, location);
    }
    
    Almanac::~Almanac()
    {
        for (int i=0; i<NUM_CATEGORIES-1; i++)
        {
            delete m_maps[i];
        }
    }
    
    void Almanac::load_map(Category cat, int start_data_line, vector<vector<string>> input_data)
    {
#ifdef DEBUG_DAY_5
        cout << "Loading Map " << cat << " from line " << start_data_line << endl;
#endif        
        int range_line = start_data_line+1;
        while (range_line < input_data.size() && input_data[range_line].size() > 0 && input_data[range_line][0].length() > 0)
        {
            Range * range = new Range();
            range->init_range(input_data[range_line]);
            m_maps[cat]->add_range(range);
            range_line++;
        }
#ifdef DEBUG_DAY_5
        cout << "Done loading Map " << cat << ". Map has " << m_maps[cat]->get_ranges().size() << " ranges" << endl;;
#endif        
    }
    
    void Almanac::create_maps(vector<vector<string>> input_data)
    {
        for (int i=2; i<input_data.size(); i++)
        {
            if (input_data[i].size() > 0)
            {
                if (input_data[i][0] == "seed")
                {
                    load_map(seed, i, input_data);
                }
                else if (input_data[i][0] == "soil")
                {
                    load_map(soil, i, input_data);
                }
                else if (input_data[i][0] == "fertilizer")
                {
                    load_map(fertilizer, i, input_data);
                }
                else if (input_data[i][0] == "water")
                {
                    load_map(water, i, input_data);
                }
                else if (input_data[i][0] == "light")
                {
                    load_map(light, i, input_data);
                }
                else if (input_data[i][0] == "temperature")
                {
                    load_map(temperature, i, input_data);
                }
                else if (input_data[i][0] == "humidity")
                {
                    load_map(humidity, i, input_data);
                }
            }
        }
    }
    
    void Almanac::apply_path_seed_to_location(Path * path)
    {
        path->set_value(soil, m_maps[seed]->apply_map(path->get_value(seed)));
        path->set_value(fertilizer, m_maps[soil]->apply_map(path->get_value(soil)));
        path->set_value(water, m_maps[fertilizer]->apply_map(path->get_value(fertilizer)));
        path->set_value(light, m_maps[water]->apply_map(path->get_value(water)));
        path->set_value(temperature, m_maps[light]->apply_map(path->get_value(light)));
        path->set_value(humidity, m_maps[temperature]->apply_map(path->get_value(temperature)));
        path->set_value(location, m_maps[humidity]->apply_map(path->get_value(humidity)));

#ifdef DEBUG_DAY_5
        cout << "Applying path for seed " << path->get_value(seed) << endl;
        cout << " Soil is " << path->get_value(soil) << endl;
        cout << " Fertilizer is " << path->get_value(fertilizer) << endl;
        cout << " Water is " << path->get_value(water) << endl;
        cout << " Light is " << path->get_value(light) << endl;
        cout << " Temperature is " << path->get_value(temperature) << endl;
        cout << " Humidity is " << path->get_value(humidity) << endl;
        cout << " Location is " << path->get_value(location) << endl;
#endif        
        
    }
}

AocDay5::AocDay5():AocDay(5)
{
}

AocDay5::~AocDay5()
{
}

vector<vector<string>> AocDay5::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> data;
    char delimiters[3] = " -";
    // using 2 delimiters to split the 'seed-to-soil' labels
    if (!fileutils.read_as_list_of_split_strings(filename, data, delimiters, 2, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return data;
}

/*
seeds: 79 14 55 13
seeds are on line 1, starting at index 1
*/

vector<long long int> AocDay5::parse_seeds(vector<vector<string>> input_data)
{
    vector<long long int> seeds;
    
    for (int i=1; i<input_data[0].size(); i++)
    {
        long long int seed = strtoll(input_data[0][i].c_str(), NULL, 10);
        seeds.push_back(seed);
    }
    
    return seeds;
}

string AocDay5::part1(string filename, vector<string> extra_args)
{
    vector<vector<string>> data = read_input(filename);
    Almanac almanac;
    
    vector<long long int> seeds = parse_seeds(data);
    
    almanac.create_maps(data);
    
    long long int closest_location = LLONG_MAX;
    for (int i=0; i<seeds.size(); i++)
    {
        Path path;
        path.set_value(seed, seeds[i]);
        almanac.apply_path_seed_to_location(&path);
        
#ifdef DEBUG_DAY_5
        cout << "Seed " << path.get_value(seed) << " corresponds to location " << path.get_value(location) << endl;
#endif
        if (path.get_value(location) < closest_location)
        {
            closest_location = path.get_value(location);
#ifdef DEBUG_DAY_5
            cout << " New closes location " << closest_location << endl;
#endif
        }
    }
    ostringstream out;
    out << closest_location;
    return out.str();
}

