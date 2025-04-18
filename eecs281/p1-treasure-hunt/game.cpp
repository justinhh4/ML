// 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include "game.hpp"

Game::Game(size_t grid_size, bool show_stats, bool verbose, string hunt_order, char path_type, 
        bool captain_stack, bool first_mate_queue) : previous(grid_size, vector<char>(grid_size, 0)),
                water_locations_visited(0), land_locations_investigated(0), ashore_num(0),
                hunt_order(hunt_order), show_stats(show_stats), verbose(verbose), 
                path_type(path_type), captain_stack(captain_stack), first_mate_queue(first_mate_queue) 
                  {}

pair<size_t, size_t> Game::first_mate_find(pair<size_t, size_t> starting_coordinate, vector<vector<char>> & grid){

    deque<pair<size_t, size_t>> first_mate; 
    pair<size_t, size_t> first_mate_pos; 
    size_t grid_size = grid.size(); 

    first_mate.push_back(starting_coordinate);  

    while(!first_mate.empty()){
        land_locations_investigated++; 
        //get current coordinate and check if empty 
        if (first_mate_queue){
            first_mate_pos = first_mate.front(); 
            first_mate.pop_front();  
        }
        else {
            first_mate_pos = first_mate.back();
            first_mate.pop_back();  
        }
        if (grid[first_mate_pos.first][first_mate_pos.second] == '$'){
            if (verbose){
                cout << "Searching island... party found treasure at " << first_mate_pos.first << "," << first_mate_pos.second << ".\n"; 
            }
            return {first_mate_pos.first, first_mate_pos.second}; 
        }

        // first mate discovering nearby squares
        size_t newrow; 
        size_t newcol; 
        char prev_dir; 
        for (size_t i = 0; i < 4; i++){
            if (hunt_order[i] == 'N'){
                if (first_mate_pos.first == 0) continue; 
                newrow = first_mate_pos.first - 1;
                newcol = first_mate_pos.second; 
                prev_dir = 'S'; 
            }
            else if (hunt_order[i] == 'E'){
                if (first_mate_pos.second == grid_size - 1) continue; 
                newrow = first_mate_pos.first;
                newcol = first_mate_pos.second + 1; 
                prev_dir = 'W';
            }
            else if (hunt_order[i] == 'S'){
                if (first_mate_pos.first == grid_size - 1) continue; 
                newrow = first_mate_pos.first + 1;
                newcol = first_mate_pos.second; 
                prev_dir = 'N';
            }
            else {
                if (first_mate_pos.second == 0) continue; 
                newrow = first_mate_pos.first;
                newcol = first_mate_pos.second - 1; 
                prev_dir = 'E'; 
            }

            if (previous[newrow][newcol] == 0 && grid[newrow][newcol] == '$'){
                land_locations_investigated++; 
                previous[newrow][newcol] = prev_dir; 
                if (verbose){
                    cout << "Searching island... party found treasure at " << newrow << "," << newcol << ".\n";
                }
                return {newrow, newcol}; 
            }
            else if (previous[newrow][newcol] == 0 && grid[newrow][newcol] == 'o'){
                first_mate.push_back({newrow, newcol});
                previous[newrow][newcol] = prev_dir; 
            }

        }
    }
    if (verbose){
        cout << "Searching island... party returned with no treasure.\n";
    }
    return {grid_size, grid_size}; 
}

void Game::play(pair<size_t, size_t> starting_coordinate, vector<vector<char>> & grid){
    if (verbose)
        cout << "Treasure hunt started at: " << starting_coordinate.first << "," << starting_coordinate.second << "\n";

    deque<pair<size_t, size_t>> captain; 
    pair<size_t, size_t> captain_pos; 
    size_t grid_size = grid.size(); 

    captain.push_back(starting_coordinate); 

    pair<size_t, size_t> discovered_treasure = {grid_size , grid_size}; 
    //runs when captain has new squares to go to
    while (!captain.empty()){
        //captain going to next position
        if (captain_stack){
            captain_pos = captain.back(); 
            captain.pop_back();  
        }
        else {
            captain_pos = captain.front();
            captain.pop_front();  
        }
        water_locations_visited++; 

        //captain discovering nearby squares
        size_t newrow;
        size_t newcol;
        char prev_dir; 
        for (size_t i = 0; i < 4; i++){
            if (hunt_order[i] == 'N'){
                if (captain_pos.first == 0) continue; 
                newrow = captain_pos.first - 1;
                newcol = captain_pos.second; 
                prev_dir = 'S'; 
            }
            else if (hunt_order[i] == 'E'){
                if (captain_pos.second == grid_size -1) continue; 
                newrow = captain_pos.first;
                newcol = captain_pos.second + 1; 
                prev_dir = 'W';              
            }
            else if (hunt_order[i] == 'S'){
                if (captain_pos.first == grid_size - 1) continue; 
                newrow = captain_pos.first + 1;
                newcol = captain_pos.second;    
                prev_dir = 'N';             
            }
            else {
                if (captain_pos.second == 0) continue; 
                newrow = captain_pos.first;
                newcol = captain_pos.second - 1; 
                prev_dir = 'E'; 
            }

            // check what kind of tile discovered
            if (previous[newrow][newcol] == 0 && grid[newrow][newcol] == '.'){
                captain.push_back({newrow, newcol}); 
                previous[newrow][newcol] = prev_dir;
            }
            else if (previous[newrow][newcol] == 0 && (grid[newrow][newcol] == 'o' || grid[newrow][newcol] == '$')){
                if (verbose){
                    cout << "Went ashore at: " << newrow << "," << newcol << "\n";
                }
                ashore_num++; 
                previous[newrow][newcol] = prev_dir; 
                discovered_treasure = first_mate_find({newrow, newcol}, grid);
                if (discovered_treasure != make_pair(grid_size, grid_size)){
                    break; 
                }
            }            
        }
        if (discovered_treasure != make_pair(grid_size, grid_size)){
             break; 
        }  
    }
    
    //Postgame stats
    if (discovered_treasure == make_pair(grid_size, grid_size)){
        cout << "Treasure hunt failed\n"; 
        if (show_stats){
            cout << "--- STATS ---\n"
                << "Starting location: " << starting_coordinate.first << "," << starting_coordinate.second << "\n" 
                << "Water locations investigated: " << water_locations_visited << "\n"
                << "Land locations investigated: " << land_locations_investigated << "\n"
                << "Went ashore: " << ashore_num << "\n"
                << "--- STATS ---" << "\n"; 
        }
        cout << "No treasure found after investigating " << water_locations_visited + land_locations_investigated << " locations.\n"; 
    }
    else {
        // finding path to treasure
        pair<size_t, size_t> temp = discovered_treasure;   
        vector<char> path; 
        while (previous[temp.first][temp.second] != 0){
            path.push_back(previous[temp.first][temp.second]); 
            if (path.back() == 'N'){
                temp = {temp.first - 1, temp.second}; 
            }
            else if (path.back() == 'E'){
                temp = {temp.first, temp.second + 1}; 
            }
            else if (path.back() == 'S'){
                temp = {temp.first + 1, temp.second}; 
            }
            else {
                temp = {temp.first, temp.second - 1}; 
            }
        }

        //show stats
        if (show_stats){
            cout << "--- STATS ---\n"
                << "Starting location: " << starting_coordinate.first << "," << starting_coordinate.second << "\n" 
                << "Water locations investigated: " << water_locations_visited << "\n"
                << "Land locations investigated: " << land_locations_investigated << "\n"
                << "Went ashore: " << ashore_num << "\n"
                << "Path length: " << path.size() << "\n"
                << "Treasure location: " << discovered_treasure.first << "," << discovered_treasure.second << "\n"
                << "--- STATS ---\n"; 
        }

        //show path 
        if (path_type == 'M'){
            temp = discovered_treasure; 
            grid[discovered_treasure.first][discovered_treasure.second] = 'X'; 
            for (size_t i = 1; i < path.size(); i++){
                if (path[i-1] == 'N'){
                    temp = {temp.first - 1, temp.second}; 
                }
                else if (path[i-1] == 'E'){
                    temp = {temp.first, temp.second + 1}; 
                }
                else if (path[i-1] == 'S'){
                    temp = {temp.first + 1, temp.second}; 
                }
                else {
                    temp = {temp.first, temp.second - 1}; 
                }                
                if ((path[i-1] == 'N' && path[i] == 'N') || (path[i-1] == 'S' && path[i] == 'S')){
                    grid[temp.first][temp.second] = '|'; 
                }
                else if ((path[i-1] == 'E' && path[i] == 'E') || (path[i-1] == 'W' && path[i] == 'W')){
                    grid[temp.first][temp.second] = '-'; 
                }
                else {
                    grid[temp.first][temp.second] = '+'; 
                }
            }

            for (size_t i = 0; i < grid_size; i++){
                for (size_t j = 0; j < grid_size; j++){
                    cout << grid[i][j]; 
                }
                cout << "\n";
            }
        }
        else if (path_type == 'L'){
            bool land = false; 
            cout << "Sail:\n";
            temp = starting_coordinate; 
            for (size_t i = path.size()-1; i != SIZE_MAX; i--){
                cout << temp.first << "," << temp.second << "\n"; 
                if (path[i] == 'N'){
                    temp = {temp.first + 1, temp.second}; 
                }
                else if (path[i] == 'E'){
                    temp = {temp.first, temp.second - 1}; 
                }
                else if (path[i] == 'S'){
                    temp = {temp.first - 1, temp.second}; 
                }
                else {
                    temp = {temp.first, temp.second + 1}; 
                }                 
                if (!land && (grid[temp.first][temp.second] == 'o' || grid[temp.first][temp.second] == '$')){
                    cout << "Search:\n"; 
                    land = true; 
                }
            }
            cout << discovered_treasure.first << "," << discovered_treasure.second << "\n";
        }

        cout << "Treasure found at " << discovered_treasure.first << "," 
                << discovered_treasure.second << " with path length " << path.size() << ".\n"; 
    }
}