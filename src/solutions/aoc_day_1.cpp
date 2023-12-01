#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_1.h"
#include "file_utils.h"

#define DIGITS "0123456789"

using namespace std;

AocDay1::AocDay1():AocDay(1)
{
}

AocDay1::~AocDay1()
{
}

vector<string> AocDay1::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> data;
    if (!fileutils.read_as_list_of_strings(filename, data))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return data;
}

long AocDay1::get_calibration_value(string input)
{
    string::size_type first_index = input.find_first_of(DIGITS);
    string::size_type last_index = input.find_last_of(DIGITS);
    
    // calibration value is first digit in tens position and last digit in ones position
    long value = ((input[first_index] - '0')*10) + (input[last_index] - '0');
    
#ifdef DEBUG_DAY_1
    cout << input << " has value " << value << " from positions " 
         << first_index << "=" << input[first_index] << " and " 
         << last_index << "=" << input[last_index] << endl;
#endif
    return value;
}

string AocDay1::part1(string filename, vector<string> extra_args)
{
    vector<string> data = read_input(filename);
    long sum = 0;
    for (vector<string>::iterator iter = data.begin(); iter != data.end(); ++iter)
    {
        sum+=get_calibration_value(*iter);
    }
    ostringstream out;
    out << sum;
    return out.str();
}

