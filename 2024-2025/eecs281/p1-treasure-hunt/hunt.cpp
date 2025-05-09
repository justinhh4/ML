// 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <map> 
#include "game.hpp"
 
using namespace std;

pair<int, int> initialize_2D_map(vector<vector<char>> & grid){
    pair<int, int> starting; 
    for (size_t i = 0; i < grid.size(); i++){
        for (size_t j = 0; j < grid.size(); j++){
            cin >> grid[i][j]; 
            if (grid[i][j] == '@'){
                starting = {i, j};
            }
        }
    }
    return starting; 
}

pair<int, int> initialize_ctt_map(vector<vector<char>> & grid){
    size_t x, y; 
    char terrain; 
    pair<int, int> starting;
    while (cin >> x && cin >> y && cin >> terrain){
        grid[x][y] = terrain; 
        if (grid[x][y] == '@'){
            starting = {x, y};
        }
    }
    return starting;
}

Game getOptions(int argc, char **argv, size_t grid_size) {
  
    int choice;
    int index = 0;

    option longOptions[] = {
        {"help", no_argument, nullptr, 'h'},
        {"captain", required_argument, nullptr, 'c'},
        {"first-mate", required_argument, nullptr, 'f'},
        {"hunt-order", required_argument, nullptr, 'o'},
        {"verbose", no_argument, nullptr, 'v'},
        {"stats", no_argument, nullptr, 's'},
        {"show-path", required_argument, nullptr, 'p'},
        {nullptr, 0, nullptr, '\0'}
    };  // longOptions[]

    bool show_stats = false; 
    bool verbose = false;
    string hunt_order = "NESW";
    char path_type = 0;
    bool captain_stack = true;
    bool first_mate_queue = true;
    bool path_set = false; 
    while ((choice = getopt_long(argc, argv, "hc:f:o:vsp:", static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
        case 'h':
            cout << "help message"; 
            exit(0);

        case 'c': 
            if (optarg == nullptr) exit(1); 
            if (string(optarg) == "QUEUE"){
                captain_stack = false; 
                break;
            }
            else if (string(optarg) == "STACK"){
                break; 
            }
            else {
                cerr << "didnt specify captain stack or queue"; 
                exit(1); 
            }

        case 'f':
            if (optarg == nullptr) exit(1); 
            if (string(optarg) == "QUEUE"){
                break;
            }
            else if (string(optarg) == "STACK"){
                first_mate_queue = false; 
                break; 
            }
            else {
                cerr << "didnt specify first mate stack or queue"; 
                exit(1); 
            }

        case 'o':{
            if (optarg == nullptr) break; 
            string temp = optarg; 
            if (temp.size() != 4) exit(1); 
            sort(temp.begin(), temp.end());
            if (temp == "ENSW"){
                hunt_order = optarg; 
                break;
            }
            else {
                cerr << "hunt order not following format";
                exit(1); 
            }
        }
        case 'v': 
            verbose = true;
            break;

        case 's':
            show_stats = true; 
            break; 

        case 'p':
            if (path_set) {
                cerr << "cannot specify path more than once"; 
                exit(1); 
            }
            if (string(optarg) == "M"){
                path_type = 'M'; 
                path_set = true; 
                break; 
            }
            else if (string(optarg) == "L"){
                path_type = 'L';
                path_set = true; 
                break; 
            }
            else {
                cerr << "path type was not M or L"; 
                exit(1); 
            }
        default:
            cerr << "incorrect command line option";
            exit(1);

        }  // switch ..choice
    }  // while
    return Game(grid_size, show_stats, verbose, hunt_order, path_type, captain_stack, first_mate_queue);

}  // getOptions()


int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);

    string ignore; 
    char map_input_type;
    while (cin >> map_input_type){
        if (map_input_type == '#'){
            getline(cin, ignore); 
        }
        else break; 
    }
    size_t map_size; 
    cin >> map_size; 

    vector<vector<char>> grid(map_size, vector<char>(map_size, '.')); 
    pair<size_t, size_t> starting_coordinate; 

    if (map_input_type == 'M'){
        starting_coordinate = initialize_2D_map(grid);
    }
    else if (map_input_type == 'L'){
        starting_coordinate = initialize_ctt_map(grid);
    }
    else cerr << "Invalid map input type"; 

    Game game = getOptions(argc, argv, map_size);
    game.play(starting_coordinate, grid);

    return 0;
}  // main()

