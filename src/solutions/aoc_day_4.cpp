#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_4.h"
#include "file_utils.h"

using namespace std;
using namespace Day4;

namespace Day4
{
    Card::Card()
    {
        m_card_number = 0;
        m_num_winning_numbers = 0;
        m_num_my_numbers = 0;
    }
    
    Card::~Card()
    {
    }
    
    /*
    Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
    card number in position 1
    winning numbers start in position 2
    my numbers start after the | character
    */
    void Card::load_from_input(vector<string> input_data)
    {
        m_card_number = strtol(input_data[1].c_str(), NULL, 10);
#ifdef DEBUG_DAY_4
        cout << "Loading card " << m_card_number << ". Winning numbers: ";
#endif
        bool loading_winning_numbers = true;
        for (int pos=2; pos<input_data.size(); pos++)
        {
            if (input_data[pos] == "|")
            {
#ifdef DEBUG_DAY_4
                cout << "...My numbers: ";
#endif
                loading_winning_numbers = false;
            }
            else
            {
                if (loading_winning_numbers == true)
                {
                    m_winning_numbers[m_num_winning_numbers] = strtol(input_data[pos].c_str(), NULL, 10);
#ifdef DEBUG_DAY_4
                    cout << m_winning_numbers[m_num_winning_numbers] << " ";
#endif
                    m_num_winning_numbers++;
                }
                else
                {
                    m_my_numbers[m_num_my_numbers] = strtol(input_data[pos].c_str(), NULL, 10);
#ifdef DEBUG_DAY_4
                    cout << m_my_numbers[m_num_my_numbers] << " ";
#endif
                    m_num_my_numbers++;
                }
            }            
        }
#ifdef DEBUG_DAY_4
        cout << endl;
#endif
        
    }
    
    vector<int> Card::get_matched_numbers()
    {
        vector<int> matched;
        
#ifdef DEBUG_DAY_4
        cout << "Finding matched numbers for card " << m_card_number << ": ";
#endif
        
        for (int win_pos=0; win_pos<m_num_winning_numbers; win_pos++)
        {
            for (int my_pos=0; my_pos<m_num_my_numbers; my_pos++)
            {
                if (m_winning_numbers[win_pos] == m_my_numbers[my_pos])
                {
                    matched.push_back(m_winning_numbers[win_pos]);
#ifdef DEBUG_DAY_4
                    cout << m_winning_numbers[win_pos] << " ";
#endif
                    break;
                }
            }
        }
        
#ifdef DEBUG_DAY_4
        if (matched.size() == 0)
        {
            cout << " none found";
        }
        cout << endl;
#endif
        return matched;
    }
    
    int Card::get_card_number()
    {
        return m_card_number;
    }
}

AocDay4::AocDay4():AocDay(4)
{
}

AocDay4::~AocDay4()
{
}

vector<vector<string>> AocDay4::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> data;
    if (!fileutils.read_as_list_of_split_strings(filename, data, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return data;
}


string AocDay4::part1(string filename, vector<string> extra_args)
{
    vector<vector<string>> data = read_input(filename);
    vector<Card> cards;
    
    for (int i=0; i<data.size(); i++)
    {
        Card card;
        card.load_from_input(data[i]);
        cards.push_back(card);
    }
    
    int sum = 0;
    for (int i=0; i<cards.size(); i++)
    {
        vector<int> matched = cards[i].get_matched_numbers();
        if (matched.size() > 0)
        {
            int points = (1<<(matched.size() - 1));
            sum+=points;
#ifdef DEBUG_DAY_4
            cout << "Card " << cards[i].get_card_number() << " is worth " << points << " points." << endl;
#endif
        }
    }
    
    ostringstream out;
    out << sum;
    return out.str();
}
