#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <utility>

#include "aoc_day_5.h"
#include "file_utils.h"

#define MAX_VALUE 4294967295

using namespace std;
using namespace Day5;

namespace Day5
{
    Path::Path()
    {
        m_num_in_path_ranges = LLONG_MAX;
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
    
    long long int Path::get_num_in_path_ranges()
    {
        return m_num_in_path_ranges;
    }
    
    void Path::set_num_in_path_ranges_if_lower(long long int num)
    {
        if (num < m_num_in_path_ranges)
        {
            m_num_in_path_ranges = num;
        }
    }

    Range::Range()
    {
    }
    
    Range::~Range()
    {
    }
    
    long long int Range::get_source_start()
    {
        return m_source_start;
    }
    
    long long int Range::get_source_end()
    {
        return m_source_end;
    }
    
    long long int Range::get_num_to_end(long long int value)
    {
        return m_source_end - value;
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
    
    void Range::set_range(long long int source_start, long long int source_end, long long int adjustment)
    {
        m_source_start = source_start;
        m_source_end = source_end;
        m_adjustment = adjustment;
    }
    
    bool Range::is_in_range(long long int value)
    {
        return (value >= m_source_start && value <= m_source_end);
    }
    
    long long int Range::calculate_destination(long long int source)
    {
        return source + m_adjustment;
    }
    
    bool Range::compare_range_pointers(Range * left, Range * right)
    {
        return (left->get_source_start() < right->get_source_end());
    };

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
    
    long long int CategoryMap::apply_map(long long int value, Range ** matched_range)
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
                if (matched_range != NULL)
                {
                    *matched_range = m_ranges[i];
                }
                return applied_value;
            }
        }
        cerr << "*****SHOULD NOT REACH HERE - RANGE SHOULD BE FOUND" << endl;
        return value;
    }
        
    void CategoryMap::add_no_change_ranges()
    {
#ifdef DEBUG_DAY_5
        cout << "Adding No Change Ranges" << endl;
#endif        
        sort(m_ranges.begin(), m_ranges.end(), Range::compare_range_pointers);
        
        int num_ranges = m_ranges.size();
        
        // check if we need a starting range
#ifdef DEBUG_DAY_5
        cout << " Checking before range " << m_ranges[0]->get_source_start() << " to " << m_ranges[0]->get_source_end() << endl;
#endif        
        if (m_ranges[0]->get_source_start() > 0ll)
        {
            Range * range = new Range();
            range->set_range(0ll, m_ranges[0]->get_source_start() - 1ll, 0ll); // from 0 to before the first range (1 long long); no adjustment
#ifdef DEBUG_DAY_5
            cout << "  Created starting range from " << range->get_source_start () << " to " << range->get_source_end() << endl;
#endif        
            m_ranges.push_back(range);
        }
        else
        {
#ifdef DEBUG_DAY_5
            cout << "  No starting range needed" << endl;
#endif
        }
        
        // check between all the ranges
        for (int i=0; i<num_ranges-1; i++)
        {
#ifdef DEBUG_DAY_5
            cout << " Checking between ranges " 
                 << m_ranges[i]->get_source_start() << " to " << m_ranges[i]->get_source_end() << " and " 
                 << m_ranges[i+1]->get_source_start() << " to " << m_ranges[i+1]->get_source_end() << endl;
#endif
            if ((m_ranges[i]->get_source_end() + 1ll) != m_ranges[i+1]->get_source_start()) // 1 long long
            {
                Range * range = new Range();
                range->set_range(m_ranges[i]->get_source_end() + 1ll, m_ranges[i+1]->get_source_start() - 1ll, 0ll); // from 0 to before the first range; no adjustment
#ifdef DEBUG_DAY_5
                cout << "  Created in-between range from " << range->get_source_start () << " to " << range->get_source_end() << endl;
#endif        
                m_ranges.push_back(range);
            }
            else
            {
#ifdef DEBUG_DAY_5
                cout << "  No in-between range needed" << endl;
#endif
            }
        }
                
        // check if we need a ending range
#ifdef DEBUG_DAY_5
        cout << " Checking before range " << m_ranges[num_ranges-1]->get_source_start() << " to " << m_ranges[num_ranges-1]->get_source_end() << endl;
#endif        
        if (m_ranges[num_ranges-1]->get_source_end() < MAX_VALUE)
        {
            Range * range = new Range();
            range->set_range(m_ranges[num_ranges-1]->get_source_end() + 1ll, MAX_VALUE, 0ll); // from after the last range to MAX_VALUE; no adjustment
#ifdef DEBUG_DAY_5
            cout << "  Created ending range from " << range->get_source_start () << " to " << range->get_source_end() << endl;
#endif        
            m_ranges.push_back(range);
        }
        else
        {
#ifdef DEBUG_DAY_5
            cout << "  No ending range needed" << endl;
#endif
        }
        
        sort(m_ranges.begin(), m_ranges.end(), Range::compare_range_pointers);
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
        m_maps[cat]->add_no_change_ranges();
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
        Range ** matched_range;

        // seed to soil
        path->set_value(soil, m_maps[seed]->apply_map(path->get_value(seed), matched_range));
#ifdef DEBUG_DAY_5
        cout << "  seed " << path->get_value(seed) << " corresponds to soil " << path->get_value(soil) << endl;
        cout << "   seed-to-soil range goes from " <<  (*matched_range)->get_source_start() << " to " << (*matched_range)->get_source_end() << endl;
        cout << "   There are " << (*matched_range)->get_num_to_end(path->get_value(seed)) << " seed values to the end" << endl;
#endif
        path->set_num_in_path_ranges_if_lower((*matched_range)->get_num_to_end(path->get_value(seed)));
        
        // soil to fertilizer
        path->set_value(fertilizer, m_maps[soil]->apply_map(path->get_value(soil), matched_range));
#ifdef DEBUG_DAY_5
        cout << "  soil " << path->get_value(soil) << " corresponds to fertilizer " << path->get_value(fertilizer) << endl;
        cout << "   soil-to-fertilizer range goes from " <<  (*matched_range)->get_source_start() << " to " << (*matched_range)->get_source_end() << endl;
        cout << "   There are " << (*matched_range)->get_num_to_end(path->get_value(soil)) << " soil values to the end" << endl;
#endif
        path->set_num_in_path_ranges_if_lower((*matched_range)->get_num_to_end(path->get_value(soil)));

        // fertilizer to water
        path->set_value(water, m_maps[fertilizer]->apply_map(path->get_value(fertilizer), matched_range));
#ifdef DEBUG_DAY_5
        cout << "  fertilizer " << path->get_value(fertilizer) << " corresponds to water " << path->get_value(water) << endl;
        cout << "   fertilizer-to-water range goes from " <<  (*matched_range)->get_source_start() << " to " << (*matched_range)->get_source_end() << endl;
        cout << "   There are " << (*matched_range)->get_num_to_end(path->get_value(fertilizer)) << " fertilizer values to the end" << endl;
#endif
        path->set_num_in_path_ranges_if_lower((*matched_range)->get_num_to_end(path->get_value(fertilizer)));

        // water to light
        path->set_value(light, m_maps[water]->apply_map(path->get_value(water), matched_range));
#ifdef DEBUG_DAY_5
        cout << "  water " << path->get_value(water) << " corresponds to light " << path->get_value(light) << endl;
        cout << "   water-to-light range goes from " <<  (*matched_range)->get_source_start() << " to " << (*matched_range)->get_source_end() << endl;
        cout << "   There are " << (*matched_range)->get_num_to_end(path->get_value(water)) << " water values to the end" << endl;
#endif
        path->set_num_in_path_ranges_if_lower((*matched_range)->get_num_to_end(path->get_value(water)));
        
        // light to temperature
        path->set_value(temperature, m_maps[light]->apply_map(path->get_value(light), matched_range));
#ifdef DEBUG_DAY_5
        cout << "  light " << path->get_value(light) << " corresponds to temperature " << path->get_value(temperature) << endl;
        cout << "   light-to-temperature range goes from " <<  (*matched_range)->get_source_start() << " to " << (*matched_range)->get_source_end() << endl;
        cout << "   There are " << (*matched_range)->get_num_to_end(path->get_value(light)) << " light values to the end" << endl;
#endif
        path->set_num_in_path_ranges_if_lower((*matched_range)->get_num_to_end(path->get_value(light)));
        
        // temperature to humidity
        path->set_value(humidity, m_maps[temperature]->apply_map(path->get_value(temperature), matched_range));
#ifdef DEBUG_DAY_5
        cout << "  temperature " << path->get_value(temperature) << " corresponds to humidity " << path->get_value(humidity) << endl;
        cout << "   temperature-to-humidity range goes from " <<  (*matched_range)->get_source_start() << " to " << (*matched_range)->get_source_end() << endl;
        cout << "   There are " << (*matched_range)->get_num_to_end(path->get_value(temperature)) << " temperature values to the end" << endl;
#endif
        path->set_num_in_path_ranges_if_lower((*matched_range)->get_num_to_end(path->get_value(temperature)));
        
        // humidity to location
        path->set_value(location, m_maps[humidity]->apply_map(path->get_value(humidity), matched_range));
#ifdef DEBUG_DAY_5
        cout << "  humidity " << path->get_value(humidity) << " corresponds to location " << path->get_value(location) << endl;
        cout << "   humidity-to-location range goes from " <<  (*matched_range)->get_source_start() << " to " << (*matched_range)->get_source_end() << endl;
        cout << "   There are " << (*matched_range)->get_num_to_end(path->get_value(humidity)) << " humidity values to the end" << endl;
#endif
        path->set_num_in_path_ranges_if_lower((*matched_range)->get_num_to_end(path->get_value(humidity)));

#ifdef DEBUG_DAY_5
        cout << "  Smalles values to the end is " << path->get_num_in_path_ranges() << endl;
#endif

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

/*
seeds: 79 14 55 13
ranges are on line 1, starting with index 1
first item in range is starting seed number. second item is number of values
this returns in start/end pairs
*/

vector<pair<long long int, long long int>> AocDay5::parse_seeds_to_ranges(vector<vector<string>> input_data)
{
    vector<pair<long long int, long long int>> seed_ranges;
    
    for (int i=1; i<input_data[0].size(); i+=2)
    {
        pair<long long int, long long int> seed_range;
        seed_range.first = strtoll(input_data[0][i].c_str(), NULL, 10);
        seed_range.second = seed_range.first + strtoll(input_data[0][i+1].c_str(), NULL, 10) - (long long int)1;
#ifdef DEBUG_DAY_5
        cout << "Input range definition " << input_data[0][i] << " " << input_data[0][i+1]
             << " corresponds to range " << seed_range.first << " - " << seed_range.second << endl;
#endif
        seed_ranges.push_back(seed_range);
    }
    
    return seed_ranges;
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

string AocDay5::part2(string filename, vector<string> extra_args)
{
    vector<vector<string>> data = read_input(filename);
    Almanac almanac;
    
    vector<pair<long long int, long long int>> seed_ranges = parse_seeds_to_ranges(data);
    
    almanac.create_maps(data);
    
    long long int closest_location = LLONG_MAX;

    for (int i=0; i<seed_ranges.size(); i++)
    {
#ifdef DEBUG_DAY_5
        cout << "Working through seed range " << seed_ranges[i].first << " - " << seed_ranges[i].second << endl;
#endif
        long long int seed_value = seed_ranges[i].first;
        while (seed_value <= seed_ranges[i].second)
        {
#ifdef DEBUG_DAY_5
            cout << " Checking seed " << seed_value << endl;
#endif
            Path path;
            path.set_value(seed, seed_value);
            almanac.apply_path_seed_to_location(&path);
        
#ifdef DEBUG_DAY_5
            cout << " Seed " << path.get_value(seed) << " corresponds to location " << path.get_value(location) << endl;
#endif
            if (path.get_value(location) < closest_location)
            {
                closest_location = path.get_value(location);
#ifdef DEBUG_DAY_5
                cout << "  New closes location " << closest_location << endl;
#endif
            }            
            seed_value += (path.get_num_in_path_ranges() + (long long int)1);
        }
    }
    ostringstream out;
    out << closest_location;
    return out.str();
}

