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

long AocDay1::get_calibration_value_with_words(string input)
{
    string words[19]={"0", "1", "one", "2", "two", "3", "three", "4", "four", "5", "five", "6", "six", "7", "seven", "8", "eight", "9", "nine"};
    long values[19]={0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9};
    
    string::size_type first_index = string::npos;
    string::size_type last_index = string::npos;
    
    int first_value = 0;
    int last_value = 0;
    
#ifdef DEBUG_DAY_1
    cout << "searching input string " << input << endl;
#endif
    for (int i=0; i<19; i++)
    {
        string::size_type tmp;
        tmp = input.find(words[i]);
        if (tmp != string::npos)
        {
#ifdef DEBUG_DAY_1
            cout << " " << words[i] << " found at position " << tmp;
#endif
            if ((first_index == string::npos) || (tmp < first_index))
            {
                first_value = values[i];
                first_index = tmp;
#ifdef DEBUG_DAY_1
                cout << " and set as new first location with value " << first_value << endl;
#endif
            }
            else
            {
#ifdef DEBUG_DAY_1
                cout << " which is after existing first location " << first_index << endl;
#endif
            }
        }

        tmp = input.rfind(words[i]);
        if (tmp != string::npos)
        {
#ifdef DEBUG_DAY_1
            cout << " " << words[i] << " found at position " << tmp;
#endif
            if ((last_index == string::npos) || (tmp > last_index))
            {
                last_value = values[i];
                last_index = tmp;
#ifdef DEBUG_DAY_1
                cout << " and set as new last location with value " << last_value << endl;
#endif
            }
            else
            {
#ifdef DEBUG_DAY_1
                cout << " which is before existing last location " << last_index << endl;
#endif
            }
        }
    }
    
    // calibration value is first digit in tens position and last digit in ones position
    long value = first_value*10 + last_value;
    
#ifdef DEBUG_DAY_1
    cout << " " << input << " has value " << value << " from positions " 
         << first_index << "=" << first_value << " and " 
         << last_index << "=" << last_value << endl;
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


string AocDay1::part2(string filename, vector<string> extra_args)
{
    vector<string> data = read_input(filename);
    long sum = 0;
    for (vector<string>::iterator iter = data.begin(); iter != data.end(); ++iter)
    {
        sum+=get_calibration_value_with_words(*iter);
    }
    ostringstream out;
    out << sum;
    return out.str();
}

