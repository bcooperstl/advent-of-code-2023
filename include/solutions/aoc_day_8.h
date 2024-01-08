#ifndef __AOC_DAY_8__
#define __AOC_DAY_8__

#include <map>

#include "aoc_day.h"

#define DAY_8_LEFT 0
#define DAY_8_RIGHT 1

#define DAY_8_MAX_NODES 1024
#define DAY_8_MAX_INSTRUCTIONS 512

namespace Day8
{
    class Node;
    
    class Node
    {
        private:
            string m_name;
            Node * m_nodes[2];
        public:
            Node();
            ~Node();
            string get_name();
            void set_name(string name);
            Node * get_node(int direction);
            void set_node(int direction, Node * child);
            bool is_start_node();
            bool is_end_node();
    };
    
    class Nodes
    {
        private:
            Node m_nodes[DAY_8_MAX_NODES];
            int m_num_nodes;
        public:
            Nodes();
            ~Nodes();
            void init_nodes(vector<vector<string>> input_data);
            vector<Node *> get_start_nodes();
            Node * get_node(string name);
    };

    class Network
    {
        private:
            int m_instructions[DAY_8_MAX_INSTRUCTIONS];
            int m_num_instructions;
            int m_current_instruction;
            Nodes m_nodes;
            bool all_ghosts_at_end(vector<Node *> ghost_nodes);
        public:
            Network();
            ~Network();
            void init_network(vector<vector<string>> input_data);
            void reset_network();
            int get_count_to_move(string start, string end);
            int get_count_to_move_ghosts();
    };
    
            
}

using namespace Day8;

class AocDay8 : public AocDay
{
    private:
        vector<vector<string>> read_input(string filename);
    public:
        AocDay8();
        ~AocDay8();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
