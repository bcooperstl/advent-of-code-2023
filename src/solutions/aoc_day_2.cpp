#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_2.h"
#include "file_utils.h"

#define PART_1_RED   12
#define PART_1_GREEN 13
#define PART_1_BLUE  14


using namespace std;
using namespace Day2;

namespace Day2
{
    Round::Round()
    {
        for (int i=0; i<DAY_2_NUM_COLORS; i++)
        {
            m_values[i] = 0;
        }
    }
    
    Round::~Round()
    {
    }
    
    void Round::set_count(Color color, int value)
    {
        m_values[color] = value;
    }
    
    int Round::get_count(Color color)
    {
        return m_values[color];
    }
    
    bool Round::is_possible(int num_red, int num_green, int num_blue)
    {
        return ((m_values[red]   <= num_red) &&
                (m_values[green] <= num_green) &&
                (m_values[blue]  <= num_blue));
    }
    
    Game::Game(int id)
    {
        m_id = id;
    }
    
    Game::~Game()
    {
        for (int i=0; i<m_rounds.size(); i++)
        {
            delete m_rounds[i];
        }
        m_rounds.clear();
    }
    
    int Game::get_id()
    {
        return m_id;
    }
    
    void Game::add_round(Round * round)
    {
        m_rounds.push_back(round);
    }
    
    bool Game::is_possible(int num_red, int num_green, int num_blue)
    {
        bool all_possible = true;
#ifdef DEBUG_DAY_2
        cout << "Checking if game " << m_id << " is possible against red=" << num_red
             << " green=" << num_green << " blue=" << num_blue << endl;
#endif
        for (int i=0; i<m_rounds.size(); i++)
        {
            bool possible = m_rounds[i]->is_possible(num_red, num_green, num_blue);
#ifdef DEBUG_DAY_2
            cout << " Round red=" << m_rounds[i]->get_count(red)
                 << " green=" << m_rounds[i]->get_count(green)
                 << " blue=" << m_rounds[i]->get_count(blue)
                 << (possible ? " is" : "is not") << " possible" << endl;
#endif
            all_possible = all_possible && possible;
        }
#ifdef DEBUG_DAY_2
        cout << " End result " << (all_possible ? "is" : "is not") << " possible" << endl;
#endif
        return all_possible;
    }
    
}

AocDay2::AocDay2():AocDay(2)
{
}

AocDay2::~AocDay2()
{
}

vector<vector<string>> AocDay2::read_input(string filename)
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
Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green

position 1 is game id
after that, even positions are values and odd positions are corresponding colors
if a color ends in a semicolon, a new round starts
if a color ends in a comma, it is part of the same round
*/

Game * AocDay2::create_game(vector<string> input_line)
{
    int id = strtol(input_line[1].c_str(), NULL, 10);
#ifdef DEBUG_DAY_2
    cout << "Creating game " << id << endl;
#endif
    Game * game = new Game(id);
    Round * round = new Round();;
    for (int count_pos=2; count_pos<input_line.size(); count_pos+=2)
    {
        int color_pos = count_pos+1;
        int count = strtol(input_line[count_pos].c_str(), NULL, 10);
        if (input_line[color_pos].find("red") != string::npos)
        {
            round->set_count(red, count);
        }
        else if (input_line[color_pos].find("green") != string::npos)
        {
            round->set_count(green, count);
        }
        else if (input_line[color_pos].find("blue") != string::npos)
        {
            round->set_count(blue, count);
        }
        else
        {
            cerr << "***Invalid color " << input_line[color_pos] << " specified!!" << endl;
        }
        
        // only need new round on a semicolor in the last spot of the color
        if (input_line[color_pos][input_line[color_pos].length()-1] == ';')
        {
#ifdef DEBUG_DAY_2
            cout << " Adding round with red=" << round->get_count(red)
                << " green=" << round->get_count(green)
                << " blue=" << round->get_count(blue) << endl;
#endif
            game->add_round(round);
            round = new Round();
        }
    }
    
#ifdef DEBUG_DAY_2
    cout << " Adding last round with red=" << round->get_count(red)
         << " green=" << round->get_count(green)
         << " blue=" << round->get_count(blue) << endl;
#endif
    game->add_round(round);
    
    return game;
}
    
    
string AocDay2::part1(string filename, vector<string> extra_args)
{
    vector<vector<string>> data = read_input(filename);
    
    vector<Game *> games;
    for (int i=0; i<data.size(); i++)
    {
        games.push_back(create_game(data[i]));
    }
    
    int sum = 0;
    
    for (int i=0; i<games.size(); i++)
    {
        if (games[i]->is_possible(PART_1_RED, PART_1_GREEN, PART_1_BLUE))
        {
            sum+=games[i]->get_id();
        }
    }
    
    ostringstream out;
    out << sum;

    for (int i=0; i<games.size(); i++)
    {
        delete games[i];
    }

    return out.str();
}

