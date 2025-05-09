// 9504853406CBAC39EE89AA3AD238AA12CA198043

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include "game.hpp"
#include "P2random.h"
 
using namespace std;

struct Options{
    int stats = 0; 
    bool median = false; 
    bool verbose = false; 
};

void getOptions(int argc, char **argv, Options & options) {
  
    int choice;
    int index = 0;

    option longOptions[] = {
        {"help", no_argument, nullptr, 'h'},
        {"median", no_argument, nullptr, 'm'},
        {"verbose", no_argument, nullptr, 'v'},
        {"statistics", required_argument, nullptr, 's'},
        {nullptr, 0, nullptr, '\0'}
    };  // longOptions[]

    while ((choice = getopt_long(argc, argv, "hmvs:", static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
        case 'h':
            cout << "help message"; 
            exit(0);

        case 'm': 
            options.median = true; 
            break; 

        case 'v':
            options.verbose = true; 
            break; 
        
        case 's':
            options.stats = stoi(optarg); 
            break; 

        default:
            cerr << "incorrect command line option";
            exit(1);
        }  // switch ..choice
    }  // while
}  // getOptions()


int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);

    uint32_t quiver_capacity = 0;
    uint32_t random_seed = 0; 
    uint32_t max_random_distance = 0;
    uint32_t max_random_speed = 0; 
    uint32_t max_random_health = 0; 

    string ignore; 
    getline(cin, ignore); 

    cin >> ignore; 
    cin >> quiver_capacity; 
    cin >> ignore; 
    cin >> random_seed; 
    cin >> ignore; 
    cin >> max_random_distance; 
    cin >> ignore; 
    cin >> max_random_speed;
    cin >> ignore; 
    cin >> max_random_health; 

    Options options;
    getOptions(argc, argv, options);
    Game game = Game(static_cast<uint32_t>(options.stats), options.verbose, options.median, quiver_capacity);
    game.init_random(random_seed, max_random_distance, max_random_speed, max_random_health);  
    game.play(); 

    return 0;
}  // main()