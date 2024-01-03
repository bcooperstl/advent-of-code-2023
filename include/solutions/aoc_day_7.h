#ifndef __AOC_DAY_7__
#define __AOC_DAY_7__

#include <map>

#include "aoc_day.h"

#define HAND_LENGTH 5

namespace Day7
{
    enum HandType
    {
        five_of_a_kind = 0,
        four_of_a_kind = 1,
        full_house = 2,
        three_of_a_kind = 3,
        two_pair = 4,
        one_pair = 5,
        high_card = 6
    };
    
    enum CardRank
    {
        ace = 0,
        king = 1,
        queen = 2,
        jack = 3,
        ten = 4,
        nine = 5,
        eight = 6,
        seven = 7,
        six = 8,
        five = 9,
        four = 10,
        three = 11,
        two = 12,
        joker = 13
    };
    
    
    class Hand
    {
        private:
            string m_hand;
            CardRank m_ranked_hand[HAND_LENGTH];
            HandType m_type;
            long int m_bid;
            long int m_rank;
            void assign_type(bool use_jokers=false);
            void assign_ranked_hand(bool use_jokers=false);
        public:
            Hand(string hand, long int bid, bool use_jokers=false);
            ~Hand();
            bool operator < (const Hand & other);
            HandType get_type();
            string get_hand();
            void set_rank(long int rank);
            long int get_winnings();
    };
    
    class Hands
    {
        private:
            vector<Hand> m_hands;
            void sort_hands();
        public:
            Hands();
            ~Hands();
            void init_hands(vector<vector<string>> input_data, bool use_jokers=false);
            void assign_ranks();
            long int get_total_winnings();
    };
}

using namespace Day7;

class AocDay7 : public AocDay
{
    private:
        vector<vector<string>> read_input(string filename);
    public:
        AocDay7();
        ~AocDay7();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
