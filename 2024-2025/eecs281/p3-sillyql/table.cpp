// C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "table.hpp"
#include <unordered_map>

using namespace std;

struct Equals{
    Field x;
    size_t i;
    string op;  
    Equals(Field x, size_t i, string& op) : x(x), i(i), op(op) {}

    bool operator()(vector<Field> & a){
        if (op == "="){
            return a[i] == x; 
        }
        else if (op == "<"){
            return a[i] < x; 
        }
        else {
            return a[i] > x; 
        }
    }
};

// CREATE
Table::Table(string name) : name(name), index_type(IndexType::unitialized){
    size_t num_cols; 
    cin >> num_cols; 
    col_names.resize(num_cols); 
    col_types.resize(num_cols);
    string current; 
    cout << "New table " << name << " with column(s) ";
    // parsing column types
    for (size_t i = 0; i < num_cols; i++){
        cin >> current; 
        col_types[i] = type[current]; 
    }
    //parsing column names
    for (size_t i = 0; i < num_cols; i++){
        cin >> col_names[i];  
        cout << col_names[i] << " "; 
        col_indices[col_names[i]] = i; 
    }
    cout << "created\n"; 
}

void Table::insert(){
    size_t num_rows;
    cin >> num_rows; 
    string ignore; 
    getline(cin, ignore); 
    data.resize(data.size() + num_rows);
    for (size_t i = data.size() - num_rows; i < data.size(); i++){
        data[i].reserve(col_names.size()); 
        for (size_t j = 0; j < col_names.size(); j++){
            switch (col_types[j]){
            case (ColumnType::Bool): {
                bool temp; 
                cin >> temp; 
                data[i].emplace_back(Field(temp));
                break; 
            } case (ColumnType::Int): {
                int temp; 
                cin >> temp; 
                data[i].emplace_back(Field(temp));
                break; 
            } case (ColumnType::Double): {
                double temp; 
                cin >> temp; 
                data[i].emplace_back(Field(temp));
                break; 
            } case (ColumnType::String): {
                string temp; 
                cin >> temp; 
                data[i].emplace_back(Field(temp));
                break; 
            }}   
            // update index   --- might be sus
            if (j == col_indices[index_col]){
                if (index_type == IndexType::bst){
                    bst_index[data[i][j]].push_back(i); 
                }
                else if (index_type == IndexType::hash){
                    hash_index[data[i][j]].push_back(i); 
                }
            }
        }
    }
    cout << "Added " << num_rows << " rows to " << name << " from position " << data.size() - num_rows << " to " << data.size()-1 << "\n"; 
}

//print where using bst can be optimized
void Table::print(bool quiet){
    size_t num_cols; 
    cin >> num_cols; 
    vector<size_t> indices(num_cols, 0); 
    vector<string> cols(num_cols, ""); 
    string current; 
    for (size_t i = 0; i < num_cols; i++){
        cin >> current; 
        if (col_indices.find(current) == col_indices.end()){
            cout << "Error during PRINT: " << current << " does not name a column in " << name << "\n"; 
            getline(cin, current); 
            return; 
        }
        cols[i] = current;  
        indices[i] = col_indices[current]; 
    }
    cin >> current; 
    size_t count = 0; 
    // PRINT ALL
    if (current == "ALL"){
        if (!quiet){
            for (string i : cols){
                cout << i << " "; 
            }
            cout << "\n";
            for (size_t i = 0; i < data.size(); i++){
                for (size_t j = 0; j < indices.size(); j++){
                    cout << data[i][indices[j]] << " "; 
                }
                cout << "\n"; 
                count++; 
            }
        }
        else {
            count = data.size(); 
        }
    }
    // PRINT WHERE
    else if (current == "WHERE"){
        cin >> current; 
        if (col_indices.find(current) == col_indices.end()){
            cout << "Error during PRINT: " << current << " does not name a column in " << name << "\n"; 
            getline(cin, current); 
            return; 
        }
        if (!quiet){
            for (string i : cols){
                cout << i << " "; 
            }
            cout << "\n";
        }
        string op; 
        cin >> op; 
        size_t index = col_indices[current]; 
        switch(col_types[index]){
            case (ColumnType::Bool):{
                bool val; 
                cin >> val; 
                Equals comp = Equals{Field(val), index, op}; 
                //bst
                if (current == index_col && index_type == IndexType::bst){
                    for (auto i : bst_index){
                        for (size_t col : i.second){
                            if (comp(data[col])){
                                if (!quiet){
                                    for (size_t j = 0; j < indices.size(); j++){
                                        cout << data[col][indices[j]] << " "; 
                                    }
                                    cout << "\n";  
                                }
                                count++;                            
                            }
                        }
                    }
                }
                //hash
                else if (current == index_col && index_type == IndexType::hash && op == "="){
                    Field f = Field(val); 
                    if (hash_index.find(f) != hash_index.end()){
                        for (size_t i : hash_index[f]){
                            if (!quiet){
                                for (size_t j = 0; j < indices.size(); j++){
                                    cout << data[i][indices[j]] << " "; 
                                }
                                cout << "\n";                                 
                            }
                            count++; 
                        }
                    }
                }
                else {
                    for (size_t i = 0; i < data.size(); i++){
                        if (comp(data[i])){
                            if (!quiet){
                                for (size_t j = 0; j < indices.size(); j++){
                                    cout << data[i][indices[j]] << " "; 
                                }
                                cout << "\n";  
                            }
                            count++;                            
                        }
                    }
                    
                }
                break; 
            }  
            case (ColumnType::Int):{
                int val; 
                cin >> val; 
                Equals comp = Equals{Field(val), index, op}; 
                //bst
                if (current == index_col && index_type == IndexType::bst){
                    for (auto i : bst_index){
                        for (size_t col : i.second){
                            if (comp(data[col])){
                                if (!quiet){
                                    for (size_t j = 0; j < indices.size(); j++){
                                        cout << data[col][indices[j]] << " "; 
                                    }
                                    cout << "\n";  
                                }
                                count++;                           
                            }
                        }
                    }
                }
                //hash
                else if (current == index_col && index_type == IndexType::hash && op == "="){
                    Field f = Field(val); 
                    if (hash_index.find(f) != hash_index.end()){
                        for (size_t i : hash_index[f]){
                            if (!quiet){
                                for (size_t j = 0; j < indices.size(); j++){
                                    cout << data[i][indices[j]] << " "; 
                                }
                                cout << "\n";                                 
                            }
                            count++; 
                        }
                    }
                }
                else {
                    for (size_t i = 0; i < data.size(); i++){
                        if (comp(data[i])){
                            if (!quiet){
                                for (size_t j = 0; j < indices.size(); j++){
                                    cout << data[i][indices[j]] << " "; 
                                }
                                cout << "\n";  
                            }
                            count++;                            
                        }
                    }
                }
                break;
            }
            case (ColumnType::Double):{
                double val; 
                cin >> val; 
                Equals comp = Equals{Field(val), index, op}; 
                //bst
                if (current == index_col && index_type == IndexType::bst){
                    for (auto i : bst_index){
                        for (size_t col : i.second){
                            if (comp(data[col])){
                                if (!quiet){
                                    for (size_t j = 0; j < indices.size(); j++){
                                        cout << data[col][indices[j]] << " "; 
                                    }
                                    cout << "\n";  
                                }
                                count++;                               
                            }
                        }
                    }
                }
                //hash
                else if (current == index_col && index_type == IndexType::hash && op == "="){
                    Field f = Field(val); 
                    if (hash_index.find(f) != hash_index.end()){
                        for (size_t i : hash_index[f]){
                            if (!quiet){
                                for (size_t j = 0; j < indices.size(); j++){
                                    cout << data[i][indices[j]] << " "; 
                                }
                                cout << "\n";                                 
                            }
                            count++; 
                        }
                    }
                }
                else {
                    for (size_t i = 0; i < data.size(); i++){
                        if (comp(data[i])){
                            if (!quiet){
                                for (size_t j = 0; j < indices.size(); j++){
                                    cout << data[i][indices[j]] << " "; 
                                }
                                cout << "\n";  
                            }
                            count++;                             
                        }
                    }
                }
                break;
            }
            case (ColumnType::String):{
                string val; 
                cin >> val; 
                Equals comp = Equals{Field(val), index, op}; 
                //bst
                if (current == index_col && index_type == IndexType::bst){
                    for (auto i : bst_index){
                        for (size_t col : i.second){
                            if (comp(data[col])){
                                if (!quiet){
                                    for (size_t j = 0; j < indices.size(); j++){
                                        cout << data[col][indices[j]] << " "; 
                                    }
                                    cout << "\n";  
                                }
                                count++;                             
                            }
                        }
                    }
                }
                //hash
                else if (current == index_col && index_type == IndexType::hash && op == "="){
                    Field f = Field(val); 
                    if (hash_index.find(f) != hash_index.end()){
                        for (size_t i : hash_index[f]){
                            if (!quiet){
                                for (size_t j = 0; j < indices.size(); j++){
                                    cout << data[i][indices[j]] << " "; 
                                }
                                cout << "\n";                                 
                            }
                            count++; 
                        }
                    }
                }
                else {
                    for (size_t i = 0; i < data.size(); i++){
                        if (comp(data[i])){
                            if (!quiet){
                                for (size_t j = 0; j < indices.size(); j++){
                                    cout << data[i][indices[j]] << " "; 
                                }
                                cout << "\n";  
                            }    
                            count++;                        
                        }
                    }
                }
                break;
            }
        }
    }
    cout << "Printed " << count << " matching rows from " << name << "\n"; 
}

void Table::delete_rows(){
    string current; 
    cin >> current; // "WHERE"
    cin >> current; // col name
    string col_name = current; 
    cin >> current; 
    string op = current; 
    if (col_indices.find(col_name) == col_indices.end()){
        cout << "Error during DELETE: " << col_name << " does not name a column in " << name << "\n"; 
        getline(cin, current); 
        return;       
    }
    size_t original_size = data.size(); 
    size_t index = col_indices[col_name]; 
    switch (col_types[index]){
        case (ColumnType::Bool):{
            bool val; 
            cin >> val; 
            Equals comp = Equals{Field(val), index, op}; 
            auto it = remove_if(data.begin(), data.end(), comp);
            data.erase(it, data.end());  
            break;
        }
        case (ColumnType::Int):{
            int val; 
            cin >> val; 
            Equals comp = Equals{Field(val), index, op}; 
            auto it = remove_if(data.begin(), data.end(), comp);
            data.erase(it, data.end());  
            break;
        }
        case (ColumnType::Double):{
            double val; 
            cin >> val; 
            Equals comp = Equals{Field(val), index, op}; 
            auto it = remove_if(data.begin(), data.end(), comp);
            data.erase(it, data.end());  
            break;
        }
        case (ColumnType::String):{
            string val; 
            cin >> val; 
            Equals comp = Equals{Field(val), index, op}; 
            auto it = remove_if(data.begin(), data.end(), comp);
            data.erase(it, data.end());  
            break;
        }
    }
    cout << "Deleted " << original_size - data.size() << " rows from " << name << "\n"; 

    // update index generate
    if (index_type == IndexType::bst || index_type == IndexType::hash){
        generate_helper(); 
    }
}

// does col1 type have to = col2 type??????
void Table::join(Table & table2, bool quiet){
    string current; 
    cin >> current; // "WHERE"
    cin >> current; 
    string col1 = current; 
    cin >> current; // "="
    cin >> current; 
    string col2 = current; 
    cin >> current; // "AND"
    cin >> current; // "PRINT"
    size_t num_cols; 
    cin >> num_cols; 
    vector<bool> table1or2(num_cols, 0);
    vector<size_t> indices(num_cols, 0); 
    vector<string> print_col_names(num_cols, ""); 
    int table_num; 
    // error handling 
    if (col_indices.find(col1) == col_indices.end()){
        cout << "Error during JOIN: " << col1 << " does not name a column in " << name << "\n"; 
        getline(cin, current); 
        return;
    }
    if (table2.col_indices.find(col2) == table2.col_indices.end()){
        cout << "Error during JOIN: " << col2 << " does not name a column in " << table2.name << "\n"; 
        getline(cin, current); 
        return;
    }

    //reading in stuff
    for (size_t i = 0; i < num_cols; i++){
        cin >> current; 
        cin >> table_num; 
        if (table_num == 1){
            if (col_indices.find(current) == col_indices.end()){
                cout << "Error during JOIN: " << current << " does not name a column in " << name << "\n"; 
                getline(cin, current); 
                return;                
            }
            table1or2[i] = 0; 
            indices[i] = col_indices[current]; 
        }
        else if (table_num == 2){
            if (table2.col_indices.find(current) == table2.col_indices.end()){
                cout << "Error during JOIN: " << current << " does not name a column in " << table2.name << "\n"; 
                getline(cin, current); 
                return;
            }
            table1or2[i] = 1; 
            indices[i] = table2.col_indices[current]; 
        }
        print_col_names[i] = current;
    }

    // print first line of output
    if (!quiet){
        for (size_t i = 0; i < num_cols; i++){
            cout << print_col_names[i] << " "; 
        }
        cout << "\n"; 
    }

    //join stuff
    size_t printed_row_count = 0; 
    size_t col1_index = col_indices[col1]; 
    size_t col2_index = table2.col_indices[col2]; 

    // create index 
    unordered_map<Field, vector<size_t>> hash; 
    for (size_t i = 0; i < table2.data.size(); i++){
        hash[table2.data[i][col2_index]].push_back(i);
    }

    for (size_t i = 0; i < data.size(); i++){
        if (hash.find(data[i][col1_index]) != hash.end()){
            printed_row_count += hash[data[i][col1_index]].size();
            if (!quiet){
                for (size_t j : hash[data[i][col1_index]]){
                    for (size_t k = 0; k < num_cols; k++){
                        if (table1or2[k] == 0){
                            cout << data[i][indices[k]] << " "; 
                        }
                        else if (table1or2[k] == 1){
                            cout << table2.data[j][indices[k]] << " "; 
                        }
                    }
                    cout << "\n"; 
                }
            }
        }
        // for (size_t j = 0; j < table2.data.size(); j++){
        //     // check if found match
        //     if (data[i][col1_index] == table2.data[j][col2_index]){
        //         printed_row_count++; 
        //         // print joined row
        //         if (!quiet){
        //             for (size_t k = 0; k < num_cols; k++){
        //                 if (table1or2[k] == 0){
        //                     cout << data[i][indices[k]] << " "; 
        //                 }
        //                 else if (table1or2[k] == 1){
        //                     cout << table2.data[j][indices[k]] << " "; 
        //                 }
        //             }
        //             cout << "\n"; 
        //         }
        //     }
        // }  
    }

    cout << "Printed " << printed_row_count << " rows from joining " << name << " to " << table2.name << "\n"; 
}

void Table::generate(){
    string current;
    cin >> current; 
    string map_type = current; 
    cin >> current; cin >> current; //" INDEX ON"
    cin >> current; 
    if (col_indices.find(current) == col_indices.end()){
        cout << "Error during GENERATE: " << current << " does not name a column in " << name << "\n"; 
        getline(cin, current); 
        return;
    }
    size_t col = col_indices[current]; 
    index_col = current; 
    if (map_type == "bst"){
        index_type = IndexType::bst; 
        bst_index.clear(); 
        for (size_t i = 0; i < data.size(); i++){
            bst_index[data[i][col]].push_back(i);
        }
        cout << "Generated " << map_type << " index for table " << name << " on column " << current << ", with " << bst_index.size() << " distinct keys\n"; 
    }
    else if (map_type == "hash"){
        index_type = IndexType::hash; 
        hash_index.clear();
        for (size_t i = 0; i < data.size(); i++){
            hash_index[data[i][col]].push_back(i);
        }        
        cout << "Generated " << map_type << " index for table " << name << " on column " << current << ", with " << hash_index.size() << " distinct keys\n"; 
    }
    
}

void Table::generate_helper(){
    size_t col = col_indices[index_col]; 
    if (index_type == IndexType::bst){
        bst_index.clear(); 
        for (size_t i = 0; i < data.size(); i++){
            bst_index[data[i][col]].push_back(i);
        }
    }
    else if (index_type == IndexType::hash){
        hash_index.clear();
        for (size_t i = 0; i < data.size(); i++){
            hash_index[data[i][col]].push_back(i);
        }
    }
}


