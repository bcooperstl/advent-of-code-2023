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
        m_copies = 0;
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
        m_copies = 1; // start with 1 copy of every card
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
    
    int Card::get_copies()
    {
        return m_copies;
    }
    
    void Card::add_copies(int num_copies)
    {
#ifdef DEBUG_DAY_4
        cout << "Card " << m_card_number << ": added " << num_copies << " copies, resulting in ";
#endif
        m_copies+=num_copies;
#ifdef DEBUG_DAY_4
        cout << m_copies << " copies" << endl;
#endif
    }
    
    void Card::check_matches(Cards * cards)
    {
        int num_matches = get_matched_numbers().size();
#ifdef DEBUG_DAY_4
        cout << "Card " << m_card_number << " has " << num_matches << " matches and " << m_copies << " copies" << endl;
#endif
        for (int i=1; i<=num_matches; i++)
        {
#ifdef DEBUG_DAY_4
            cout << " Adding " << m_copies << " to card " << m_card_number+1 << endl;
#endif
            cards->add_copies_to_card(m_copies, m_card_number+i);
        }
    }
    
    Cards::Cards()
    {
    }
    
    Cards::~Cards()
    {
        map<int, Card *>::iterator pos = m_card_map.begin();
        while (pos != m_card_map.end())
        {
            delete pos->second;
            ++pos;
        }
        m_card_map.clear();
    }
    
    void Cards::load_from_input(vector<vector<string>> input_data)
    {
        for (int i=0; i<input_data.size(); i++)
        {
            Card * card = new Card();
            card->load_from_input(input_data[i]);
            m_card_map[card->get_card_number()] = card;
        }
    }
    
    int Cards::get_total_points()
    {
        int sum = 0;
        map<int, Card *>::iterator pos = m_card_map.begin();
        while (pos != m_card_map.end())
        {
            vector<int> matched = pos->second->get_matched_numbers();
            if (matched.size() > 0)
            {
                int points = (1<<(matched.size() - 1));
                sum+=points;
#ifdef DEBUG_DAY_4
                cout << "Card " << pos->first << " is worth " << points << " points." << endl;
#endif
            }
            ++pos;
        }
        return sum;
    }

    void Cards::add_copies_to_card(int num_copies, int card_number)
    {
        m_card_map[card_number]->add_copies(num_copies);
    }
    
    void Cards::check_all_matches()
    {
        map<int, Card *>::iterator pos = m_card_map.begin();
        while (pos != m_card_map.end())
        {
            pos->second->check_matches(this);
            ++pos;
        }
    }

    int Cards::get_total_cards()
    {
        int sum = 0;
        map<int, Card *>::iterator pos = m_card_map.begin();
        while (pos != m_card_map.end())
        {
#ifdef DEBUG_DAY_4
            cout << "Card " << pos->first << " has " << pos->second->get_copies() << " copies." << endl;
#endif
            sum+=pos->second->get_copies();
            ++pos;
        }
        return sum;
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
    Cards cards;
    
    cards.load_from_input(data);
        
    ostringstream out;
    out << cards.get_total_points();
    return out.str();
}

string AocDay4::part2(string filename, vector<string> extra_args)
{
    vector<vector<string>> data = read_input(filename);
    Cards cards;
    
    cards.load_from_input(data);
    
    cards.check_all_matches();
    
    ostringstream out;
    out << cards.get_total_cards();
    return out.str();
}
