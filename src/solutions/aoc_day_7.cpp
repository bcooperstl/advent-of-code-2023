#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "aoc_day_7.h"
#include "file_utils.h"

using namespace std;
using namespace Day7;

namespace Day7
{
    Hand::Hand(string hand, long int bid, bool use_jokers)
    {
        m_hand = hand;
        m_bid = bid;
        m_rank = 0;
        assign_type(use_jokers);
        assign_ranked_hand(use_jokers);
    }
    
    Hand::~Hand()
    {
    }
    
    void Hand::assign_type(bool use_jokers)
    {
        int num_unique = 0;
        char cards[5];
        int count[5];
        int num_jokers = 0;
        
        for (int i=0; i<HAND_LENGTH; i++)
        {
            if (use_jokers == true && m_hand[i] == 'J')
            {
                num_jokers++;
            }
            else
            {
                bool new_unique = true;
                for (int j=0; j<num_unique; j++)
                {
                    if (cards[j] == m_hand[i])
                    {
                        new_unique = false;
                        count[j]++;
                        break;
                    }
                }
                if (new_unique)
                {
                    cards[num_unique] = m_hand[i];
                    count[num_unique] = 1;
                    num_unique++;
                }
            }
        }
        
        // assign the jokers to the highest-valued count
        if (use_jokers == true && num_jokers > 0)
        {
#ifdef DEBUG_DAY_7
            cout << "Hand " << m_hand << " is has " << num_jokers << " jokers" << endl;
#endif
            // special case for 5 jokers
            if (num_jokers == 5)
            {
#ifdef DEBUG_DAY_7
                cout << " hard-coding as five of a kind" << endl;
#endif
                num_unique = 1;
                cards[0] = 'J';
                count[0] = 5;
            }
            else
            {
                int high_count_idx = 0;
                for (int i=1; i<num_unique; i++)
                {
                    if (count[i] > count[high_count_idx])
                    {
                        high_count_idx = i;
                    }
                }
#ifdef DEBUG_DAY_7
                cout << " adding " << num_jokers << " to the " << count[high_count_idx] << " " << cards[high_count_idx] << "'s resulting in ";
#endif
                count[high_count_idx]+=num_jokers;
#ifdef DEBUG_DAY_7
                cout << count[high_count_idx] << endl;;
#endif
            }
        }            
        
        // check for five of a kind
        if (num_unique == 1 && count[0] == 5)
        {
            m_type = five_of_a_kind;
#ifdef DEBUG_DAY_7
            cout << "Hand " << m_hand << " is five-of-a-kind" << endl;
#endif
        }        
        // check for four of a kind or full house
        else if (num_unique == 2)
        {
            if ((count[0] == 4 && count[1] == 1) || 
                (count[0] == 1 && count[1] == 4))
            {
                m_type = four_of_a_kind;
#ifdef DEBUG_DAY_7
                cout << "Hand " << m_hand << " is four-of-a-kind" << endl;
#endif
            }
            else if ((count[0] == 3 && count[1] == 2) || 
                     (count[0] == 2 && count[1] == 3))

            {
                m_type = full_house;
#ifdef DEBUG_DAY_7
                cout << "Hand " << m_hand << " is full-house" << endl;
#endif
            }
            else
            {
                cerr << "!!!!!!!LOGIC FAILED FOR HAND " << m_hand << endl;
            }
        }
        // check for three of a kind or two pair
        else if (num_unique == 3)
        {
            if ((count[0] == 3 && count[1] == 1 && count[2] == 1) ||
                (count[0] == 1 && count[1] == 3 && count[2] == 1) ||
                (count[0] == 1 && count[1] == 1 && count[2] == 3))
            {
                m_type = three_of_a_kind;
#ifdef DEBUG_DAY_7
                cout << "Hand " << m_hand << " is three-of-a-kind" << endl;
#endif
            }
            else if ((count[0] == 2 && count[1] == 2 && count[2] == 1) ||
                     (count[0] == 2 && count[1] == 1 && count[2] == 2) ||
                     (count[0] == 1 && count[1] == 2 && count[2] == 2))
            {
                m_type = two_pair;
#ifdef DEBUG_DAY_7
                cout << "Hand " << m_hand << " is two-pair" << endl;
#endif
            }
            else
            {
                cerr << "!!!!!!!LOGIC FAILED FOR HAND " << m_hand << endl;
            }
        }
        // check for one pair
        else if (num_unique == 4)
        {
            if ((count[0] == 2 && count[1] == 1 && count[2] == 1 && count[3] == 1) ||
                (count[0] == 1 && count[1] == 2 && count[2] == 1 && count[3] == 1) ||
                (count[0] == 1 && count[1] == 1 && count[2] == 2 && count[3] == 1) ||
                (count[0] == 1 && count[1] == 1 && count[2] == 1 && count[3] == 2))
            {
                m_type = one_pair;
#ifdef DEBUG_DAY_7
                cout << "Hand " << m_hand << " is one-pair" << endl;
#endif
            }
            else
            {
                cerr << "!!!!!!!LOGIC FAILED FOR HAND " << m_hand << endl;
            }                
        }
        else if (num_unique == 5 && count[0] == 1 && count[1] == 1 && 
                 count[2] == 1 && count[3] == 1 && count[4] == 1)
        {
            m_type = high_card;
#ifdef DEBUG_DAY_7
            cout << "Hand " << m_hand << " is high-card" << endl;
#endif
        }
        else
        {
            cerr << "!!!!!!!LOGIC FAILED FOR HAND " << m_hand << endl;
        }
    }
    
    void Hand::assign_ranked_hand(bool use_jokers)
    {
        for (int i=0; i<HAND_LENGTH; i++)
        {
            switch (m_hand[i])
            {
                case 'A':
                    m_ranked_hand[i] = ace;
                    break;
                case 'K':
                    m_ranked_hand[i] = king;
                    break;
                case 'Q':
                    m_ranked_hand[i] = queen;
                    break;
                case 'J':
                    m_ranked_hand[i] = (use_jokers ? joker : jack);
                    break;
                case 'T':
                    m_ranked_hand[i] = ten;
                    break;
                case '9':
                    m_ranked_hand[i] = nine;
                    break;
                case '8':
                    m_ranked_hand[i] = eight;
                    break;
                case '7':
                    m_ranked_hand[i] = seven;
                    break;
                case '6':
                    m_ranked_hand[i] = six;
                    break;
                case '5':
                    m_ranked_hand[i] = five;
                    break;
                case '4':
                    m_ranked_hand[i] = four;
                    break;
                case '3':
                    m_ranked_hand[i] = three;
                    break;
                case '2':
                    m_ranked_hand[i] = two;
                    break;
                default:
                    cerr << "INVALID CARD RANK " << m_hand[i] << endl;
                    break;
            }
        }
    }
        
    bool Hand::operator < (const Hand & other)
    {
#ifdef DEBUG_DAY_7_COMPARE
        cout << "Comparing " << m_hand << " < " << other.m_hand << endl;
#endif

        if (m_type != other.m_type)
        {
#ifdef DEBUG_DAY_7_COMPARE
            cout << " Types " << m_type << " and " << other.m_type << " not equal. returning " << (m_type < other.m_type) << endl;
#endif
            return (m_type < other.m_type);
        }
        
#ifdef DEBUG_DAY_7_COMPARE
        cout << " Types " << m_type << " and " << other.m_type << " equal." << endl;
#endif
        
        for (int i=0; i<HAND_LENGTH; i++)
        {
            if (m_ranked_hand[i] != other.m_ranked_hand[i])
            {
#ifdef DEBUG_DAY_7_COMPARE
                cout << " Position " << i << " CardRanks " << m_ranked_hand[i] << " and " << other.m_ranked_hand[i] << " not equal. returning " << (m_ranked_hand[i] < other.m_ranked_hand[i]) << endl;
#endif
                return (m_ranked_hand[i] < other.m_ranked_hand[i]);
            }
#ifdef DEBUG_DAY_7_COMPARE
            cout << " Position " << i << " CardRanks " << m_ranked_hand[i] << " and " << other.m_ranked_hand[i] << " equal." << endl;
#endif
        }

#ifdef DEBUG_DAY_7_COMPARE
        cout << " Got to end. returning false" << endl;
#endif
        
        return false; // equal at this point, so return false
    }
    
    HandType Hand::get_type()
    {
        return m_type;
    }
    
    string Hand::get_hand()
    {
        return m_hand;
    }
    
    void Hand::set_rank(long int rank)
    {
        m_rank = rank;
    }
    
    long int Hand::get_winnings()
    {
        return m_bid * m_rank;
    }
    
    
    Hands::Hands()
    {
    }
    
    Hands::~Hands()
    {
    }
    
    void Hands::init_hands(vector<vector<string>> input_data, bool use_jokers)
    {
        for (int i=0; i<input_data.size(); i++)
        {
            Hand hand(input_data[i][0], strtol(input_data[i][1].c_str(), NULL, 10), use_jokers);
            m_hands.push_back(hand);
        }
        
        return;
    }
    
    void Hands::sort_hands()
    {
        sort(m_hands.begin(), m_hands.end());
    }
    
    void Hands::assign_ranks()
    {
#ifdef DEBUG_DAY_7
        cout << "Assigning ranks to hands" << endl;
#endif
        sort_hands(); // sorted from best to worst
        for (int i=0; i<m_hands.size(); i++)
        {
            long int rank = m_hands.size() - i;
            
            m_hands[i].set_rank(rank);
#ifdef DEBUG_DAY_7
            cout << " Hand " << m_hands[i].get_hand() << " is position " << i << " and assigned rank " << rank << endl;
#endif
        }
    }
    
    long int Hands::get_total_winnings()
    {
        long int total_winnings = 0;
        for (int i=0; i<m_hands.size(); i++)
        {
            total_winnings+= m_hands[i].get_winnings();
        }
        return total_winnings;
    }
}

AocDay7::AocDay7():AocDay(7)
{
}

AocDay7::~AocDay7()
{
}

vector<vector<string>> AocDay7::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> data;
    // space is my delimiter
    if (!fileutils.read_as_list_of_split_strings(filename, data, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return data;
}

string AocDay7::part1(string filename, vector<string> extra_args)
{
    vector<vector<string>> data = read_input(filename);
    Hands hands;
    
    hands.init_hands(data);
    
    hands.assign_ranks();
    
    ostringstream out;
    out << hands.get_total_winnings();
    return out.str();
}

string AocDay7::part2(string filename, vector<string> extra_args)
{
    vector<vector<string>> data = read_input(filename);
    Hands hands;
    
    hands.init_hands(data, true);
    
    hands.assign_ranks();
    
    ostringstream out;
    out << hands.get_total_winnings();
    return out.str();
}
