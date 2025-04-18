// C0F4DFE8B340D81183C208F70F9D2D797908754D

#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Field.h"
#include <map>

using namespace std;

enum class IndexType {unitialized, bst, hash};

class Table{
    private:
        string name; 
        vector<string> col_names;
        vector<ColumnType> col_types;
        unordered_map<string, size_t> col_indices; 

        unordered_map<Field, vector<size_t>> hash_index; 
        map<Field, vector<size_t>> bst_index; 
        string index_col; 
        IndexType index_type; 

        inline static unordered_map<string, ColumnType> type = {
            {"string", ColumnType::String}, 
            {"int", ColumnType::Int}, 
            {"bool", ColumnType::Bool},
            {"double", ColumnType::Double}
        }; 

        vector<vector<Field>> data; 
    
    public:
        Table(string name);

        void insert(); 
        void print(bool quiet); 
        void delete_rows(); 
        void join(Table & table2, bool quiet); 
        void generate(); 
        void generate_helper();
};