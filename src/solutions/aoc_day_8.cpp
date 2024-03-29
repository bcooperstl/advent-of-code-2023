#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdint>

#include "aoc_day_8.h"
#include "file_utils.h"
#include "math_utils.h"

using namespace std;
using namespace Day8;

namespace Day8
{
    Node::Node()
    {
        m_name = "";
        m_nodes[DAY_8_LEFT] = NULL;
        m_nodes[DAY_8_RIGHT] = NULL;
    }
    
    Node::~Node()
    {
    }
    
    string Node::get_name()
    {
        return m_name;
    }
    
    void Node::set_name(string name)
    {
        m_name = name;
    }
    
    Node * Node::get_node(int direction)
    {
        return m_nodes[direction];
    }
    
    void Node::set_node(int direction, Node * node)
    {
        m_nodes[direction] = node;
    }
    
    bool Node::is_start_node()
    {
        return (m_name[2] == 'A');
    }
    
    bool Node::is_end_node()
    {
        return (m_name[2] == 'Z');
    }
    
    Nodes::Nodes()
    {
        m_num_nodes = 0;
    }
    
    Nodes::~Nodes()
    {
    }
    
    Node * Nodes::get_node(string name)
    {
        for (int i=0; i<m_num_nodes; i++)
        {
            if (m_nodes[i].get_name() == name)
            {
                return &(m_nodes[i]);
            }
        }
        cerr << "*****ERROR: Requested node " << name << " not found!!" << endl;
        return NULL;
    }
    
    
    /* 
    Input starting at line 2 (3rd line):
    AAA = (BBB, CCC)
    field 0 = target
    field 2 from chars 1 to 3 = left
    field 3 from chars 0 to 2 = right
    */
    
    void Nodes::init_nodes(vector<vector<string>> input_data)
    {
        // first set all the target node names
        for (int i=2; i<input_data.size(); i++)
        {
            m_nodes[m_num_nodes].set_name(input_data[i][0]);
#ifdef DEBUG_DAY_8
            cout << "Target node " << m_nodes[m_num_nodes].get_name() << endl;
#endif
            m_num_nodes++;
        }
        // now set the destination nodes
        for (int i=2; i<input_data.size(); i++)
        {
            Node * source = get_node(input_data[i][0]);
            string left  = input_data[i][2].substr(1,3);
            string right = input_data[i][3].substr(0,3);
            source->set_node(DAY_8_LEFT, get_node(left));
            source->set_node(DAY_8_RIGHT, get_node(right));
#ifdef DEBUG_DAY_8
            cout << "Node " << source->get_name() 
                 << " has left node " << source->get_node(DAY_8_LEFT)->get_name() 
                 << " and right node " << source->get_node(DAY_8_RIGHT)->get_name() << endl;
#endif
        }
    }
    
    vector<Node *> Nodes::get_start_nodes()
    {
        vector<Node *> start_nodes;
        for (int i=0; i<m_num_nodes; i++)
        {
            if (m_nodes[i].is_start_node())
            {
                start_nodes.push_back(&(m_nodes[i]));
            }
        }
        return start_nodes;
    }
    
    Network::Network()
    {
        m_num_instructions = 0;
        m_current_instruction = 0;
    }
    
    Network::~Network()
    {
    }
    
    /* First line is LR list of instructions.
       Second line is blank
       Third line through end are Nodes
    */
    void Network::init_network(vector<vector<string>> input_data)
    {
        for (int i=0; i<input_data[0][0].length(); i++)
        {
            if (input_data[0][0][i] == 'L')
            {
                m_instructions[i] = DAY_8_LEFT;
            }
            else
            {
                m_instructions[i] = DAY_8_RIGHT;
            }
        }
        
        m_num_instructions = input_data[0][0].length();
#ifdef DEBUG_DAY_8
        cout << "There were " <<  m_num_instructions << " insturctions loaded" << endl;
#endif
        
        m_nodes.init_nodes(input_data);
    }
    
    void Network::reset_network()
    {
        m_current_instruction = 0;
    }
    
    int Network::get_count_to_move(string start, string end)
    {
#ifdef DEBUG_DAY_8
        cout << "Moving from " << start << " to " << end << endl;
#endif
        Node * current_node = m_nodes.get_node(start);
        Node * target_node = m_nodes.get_node(end);
        
        int move_count = 0;
        while (current_node != target_node)
        {
            move_count++;
            Node * next_node = current_node->get_node(m_instructions[m_current_instruction]);
#ifdef DEBUG_DAY_8
            string dir_str = m_instructions[m_current_instruction] == DAY_8_LEFT ? "left" : "right";
            cout << " Move " << move_count 
                 << " is from instruction " << m_current_instruction
                 << " going " << dir_str 
                 << " from node " << current_node->get_name() 
                 << " to node " << next_node->get_name() << endl;
#endif
            current_node = next_node;
            m_current_instruction = ((m_current_instruction + 1) % m_num_instructions);
        }
#ifdef DEBUG_DAY_8
        cout << "Final node " << end << " reached after " << move_count << " moves" << endl;
#endif

        return move_count;
    }
    
    bool Network::all_ghosts_at_end(vector<Node *> ghost_nodes)
    {
        for (int i=0; i<ghost_nodes.size(); i++)
        {
            if (!ghost_nodes[i]->is_end_node())
            {
                return false;
            }
        }
        return true;
    }

    //TODO: This didn't need Chinese Remainder Theorm, and that doesn't work. I need the LCM of the values. That's it.
    
    int64_t Network::get_count_to_move_ghosts()
    {
        vector<Node *> current_nodes = m_nodes.get_start_nodes();
        int64_t values[64];
        
        /*
            For each ghost:
              the value for the Chinese Remainder theorm will be the move count when the ghost first reaches an end
              the modulus will be the move count the second time minus the move count the first time
        */
        
#ifdef DEBUG_DAY_8
        cout << "There are " << current_nodes.size() << " start ghosts:";
        for (int i=0; i<current_nodes.size(); i++)
        {
            cout << " " << current_nodes[i]->get_name();
        }
        cout << endl;
#endif

        int num_ghosts = current_nodes.size();
        int values_set = 0;

        for (int i=0; i<num_ghosts; i++)
        {
            values[i] = 0;
        }
        
        int move_count = 0;
        while (values_set != num_ghosts)
        {
            move_count++;
            vector<Node *> next_nodes;
            
            for (int i=0; i<current_nodes.size(); i++)
            {
                Node * next_node = current_nodes[i]->get_node(m_instructions[m_current_instruction]);
                if (next_node->is_end_node())
                {
#ifdef DEBUG_DAY_8
                    cout << "Ghost " << i << " reached end node " << next_node->get_name() << " at move " << move_count << endl;
#endif
                    // first check the value
                    if (values[i] == 0)
                    {
                        values[i] = move_count;
                        values_set++;
#ifdef DEBUG_DAY_8
                        cout << " Value for Ghost " << i << " CRT set to " << values[i] << endl;
#endif
                    }
                    else
                    {
#ifdef DEBUG_DAY_8
                        cout << " Value already set for Ghost " << i << " CRT " << endl;
#endif
                    }
                
                }
                next_nodes.push_back(next_node);
            }
            current_nodes = next_nodes;
            m_current_instruction = ((m_current_instruction + 1) % m_num_instructions);
        }

        return MathUtils::lcm_multi(num_ghosts, values);
    }    
}

AocDay8::AocDay8():AocDay(8)
{
}

AocDay8::~AocDay8()
{
}

vector<vector<string>> AocDay8::read_input(string filename)
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

string AocDay8::part1(string filename, vector<string> extra_args)
{
    vector<vector<string>> data = read_input(filename);
    Network network;
    
    network.init_network(data);
    
    ostringstream out;
    out << network.get_count_to_move("AAA", "ZZZ");
    return out.str();
}

string AocDay8::part2(string filename, vector<string> extra_args)
{
    vector<vector<string>> data = read_input(filename);
    Network network;
    
    network.init_network(data);
    
    ostringstream out;
    out << network.get_count_to_move_ghosts();
    return out.str();
}
