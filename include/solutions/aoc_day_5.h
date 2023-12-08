#ifndef __AOC_DAY_5__
#define __AOC_DAY_5__

#include <map>

#include "aoc_day.h"

#define NUM_CATEGORIES 8

namespace Day5
{
    enum Category
    {
        seed = 0,
        soil = 1,
        fertilizer = 2,
        water = 3,
        light = 4,
        temperature = 5,
        humidity = 6,
        location = 7
    };

    class Path
    {
        private:
            long long int m_values[NUM_CATEGORIES];
        public:
            Path();
            ~Path();
            long long int get_value(Category category);
            void set_value(Category category, long long int value);
    };
    
    class Range
    {
        private:
            long long int m_source_start;
            long long int m_source_end;
            long long int m_adjustment;
        public:
            Range();
            ~Range();
            void init_range(vector<string> input_data);
            bool is_in_range(long long int value);
            long long int calculate_destination(long long int source);
    };
    
    class CategoryMap
    {
        private:
            Category m_source;
            Category m_destination;
            vector<Range *> m_ranges;
        public:
            CategoryMap(Category source, Category destination);
            ~CategoryMap();
            void add_range(Range * range);
            vector<Range *> get_ranges();
            long long int apply_map(long long int value);
    };
    
    class Almanac
    {
        private:
            CategoryMap * m_maps[NUM_CATEGORIES-1];
            void load_map(Category cat, int start_data_line, vector<vector<string>> input_data);
        public:
            Almanac();
            ~Almanac();
            void create_maps(vector<vector<string>> input_data);
            void apply_path_seed_to_location(Path * path);
    };
}

using namespace Day5;

class AocDay5 : public AocDay
{
    private:
        vector<vector<string>> read_input(string filename);
        vector<long long int> parse_seeds(vector<vector<string>> input_data);
    public:
        AocDay5();
        ~AocDay5();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
