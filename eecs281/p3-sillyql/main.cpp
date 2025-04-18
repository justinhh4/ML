// C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include "Field.h" 
#include "table.hpp"
#include <unordered_map>

using namespace std;

bool getOptions(int argc, char **argv) {
  
    int choice;
    int index = 0;

    option longOptions[] = {
        {"help", no_argument, nullptr, 'h'},
        {"quiet", no_argument, nullptr, 'q'},
        {nullptr, 0, nullptr, '\0'}
    };  // longOptions[]

    while ((choice = getopt_long(argc, argv, "hq", static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
        case 'h':
            cout << "help message"; 
            exit(0);

        case 'q': 
            return true; 

        default:
            cerr << "incorrect command line option";
            exit(1);
        }  // switch ..choice
    }  // while
    return false; 
}  // getOptions()


int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); 
    cin >> std::boolalpha;  
    cout << std::boolalpha; 

    bool quiet = getOptions(argc, argv);

    unordered_map<string, Table> database; 
    string current; 
    while (cin >> current){
        cout << "% ";
        // comment
        if (current[0] == '#'){
            getline(cin, current);
        }

        // CREATE
        else if (current == "CREATE"){
            cin >> current;
            if (database.find(current) != database.end()){
                cout << "Error during CREATE: Cannot create already existing table " << current << "\n"; 
                getline(cin, current); 
            }
            else {
                database.insert({current, Table(current)}); 
            }
        }

        // QUIT
        else if (current == "QUIT"){
            cout << "Thanks for being silly!\n"; 
            return 0; 
        }

        // REMOVE
        else if (current == "REMOVE"){
            cin >> current; 
            if (database.find(current) == database.end()){
                cout << "Error during REMOVE: " << current << " does not name a table in the database\n"; 
                getline(cin, current); 
            }
            else {
                database.erase(current); 
                cout << "Table " << current << " removed\n"; 
            }
        }

        //INSERT
        else if (current == "INSERT"){
            cin >> current;  // "INTO"
            cin >> current; 
            if (database.find(current) == database.end()){
                cout << "Error during INSERT: " << current << " does not name a table in the database\n"; 
                getline(cin, current);
            }
            else {
                database.at(current).insert();
            }
        }

        //PRINT
        else if (current == "PRINT"){
            cin >> current; // "FROM"
            cin >> current; 
            if (database.find(current) == database.end()){
                cout << "Error during PRINT: " << current << " does not name a table in the database\n"; 
                getline(cin, current); 
            }
            else {
                database.at(current).print(quiet); 
            }
        }

        //DELETE
        else if (current == "DELETE"){
            cin >> current; // "FROM"
            cin >> current; 
            if (database.find(current) == database.end()){
                cout << "Error during DELETE: " << current << " does not name a table in the database\n"; 
                getline(cin, current); 
            } 
            else {
                database.at(current).delete_rows(); 
            }
        }

        //JOIN
        else if (current == "JOIN"){
            cin >> current; 
            string table1 = current; 
            cin >> current; // "AND"
            cin >> current; 
            if (database.find(table1) == database.end()){
                cout << "Error during JOIN: " << table1 << " does not name a table in the database\n"; 
                getline(cin, current);                 
            }
            else if (database.find(current) == database.end()){
                cout << "Error during JOIN: " << current << " does not name a table in the database\n"; 
                getline(cin, current);                
            }
            else {
                database.at(table1).join(database.at(current), quiet);
            }
        }

        //GENERATE
        else if (current == "GENERATE"){
            cin >> current; // "FOR"
            cin >> current; 
            if (database.find(current) == database.end()){
                cout << "Error during GENERATE: " << current << " does not name a table in the database\n"; 
                getline(cin, current);                 
            }
            else {
                database.at(current).generate(); 
            }
        }

        else {
            cout << "Error: unrecognized command\n"; 
            getline(cin, current);  
        }
    }

    cout << "no quit command"; 
    return 1; 
}  // main()