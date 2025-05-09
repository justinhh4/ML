// 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
#include <utility>
#include <queue> 
#include <iostream>
#include <map>
using namespace std;

class Game{
private:

vector<vector<char>> previous; 
int water_locations_visited;
int land_locations_investigated; 
int ashore_num; 
string hunt_order;
bool show_stats; 
bool verbose;
char path_type;
bool captain_stack;
bool first_mate_queue;

    

public:

    Game(size_t grid_size, bool show_stats, bool verbose, string hunt_order, char path_type, 
        bool captain_stack, bool first_mate_queue);

    pair<size_t, size_t> first_mate_find(pair<size_t, size_t> starting_coordinate, vector<vector<char>> & grid);

    void play(pair<size_t, size_t> starting_coordinate, vector<vector<char>> & grid);
};

#endif