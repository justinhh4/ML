// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include "cMST.hpp"
#include <iomanip>

using namespace std;

enum class Mode{
    MST, 
    FASTTSP, 
    OPTTSP
}; 

Mode getOptions(int argc, char **argv) {
  
    int choice;
    int index = 0;

    option longOptions[] = {
        {"help", no_argument, nullptr, 'h'},
        {"mode", required_argument, nullptr, 'm'},
        {nullptr, 0, nullptr, '\0'}
    };  // longOptions[]

    while ((choice = getopt_long(argc, argv, "hm:", static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
        case 'h':
            cout << "help message"; 
            exit(0);

        case 'm': 
            if (string(optarg) == "MST"){
                return Mode::MST; 
                break; 
            }
            else if (string(optarg) == "FASTTSP"){
                return Mode::FASTTSP;
                break; 
            }
            else if (string(optarg) == "OPTTSP"){
                return Mode::OPTTSP;
                break;
            }
            else {
                cerr << "specified mode does not exist"; 
                exit(1); 
            }

        default:
            cerr << "incorrect command line option or mode not specified";
            exit(1);
        }  // switch ..choice
    }  // while
    exit(1); 
}  // getOptions()

int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);

    cout << std::setprecision(2); //Always show 2 decimal places
    cout << std::fixed; //Disable scientific notation for large numbers

    
    Mode mode = getOptions(argc, argv); 
    size_t num_vertices; 
    cin >> num_vertices; 
    vector<Vertex> vertices(num_vertices); 
    for (size_t i = 0; i < num_vertices; i++){
        cin >> vertices[i].x; 
        cin >> vertices[i].y; 
    }

    if (mode == Mode::MST){
        find_MST(vertices); 
    }
    else if (mode == Mode::FASTTSP){
        find_FASTTSP(vertices, false); 
    }
    else if (mode == Mode::OPTTSP){
        find_OPTTSP(vertices); 
    }
    else {
        cout << "no such mode"; 
        return 1; 
    }
    return 0; 
}