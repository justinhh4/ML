#include "TextBuffer.hpp"
using namespace std; 
#include <cassert>

TextBuffer::TextBuffer() : row(1), column(0), index(0) {
    cursor = data.end(); 
}

bool TextBuffer::forward(){
    if (cursor == data.end()) return false; 
    if (*cursor == '\n'){
        row++;
        column = 0;
    }
    else {
        column++;
    }
    index++;
    cursor++; 
    return true; 
}

bool TextBuffer::backward(){
    if (cursor == data.begin()) return false; 
    if (column == 0){
        row--;
        cursor--;
        column = compute_column();
    }
    else {
        cursor--; 
        column--;
    }
    index--;
    return true; 
}

void TextBuffer::insert(char c){
    cursor = data.insert(cursor, c); 
    forward(); 
}
bool TextBuffer::remove(){
    if (cursor == data.end()) return false; 
    cursor = data.erase(cursor); 
    return true; 
}
void TextBuffer::move_to_row_start(){
    while (column != 0){
        backward(); 
    }
}
void TextBuffer::move_to_row_end(){
    while (cursor != data.end() && *cursor != '\n'){
        cursor++; 
        column++;
        index++; 
    }
}
void TextBuffer::move_to_column(int new_column){
    //assert(false);
    if (new_column > column){
        while (new_column > column && cursor != data.end() && *cursor != '\n'){
            forward(); 
        }
    }
    else if (new_column < column){
        while (new_column < column){
            backward(); 
        }
    }
}
bool TextBuffer::up(){
    if (row == 1) return false; 
    int current_column = column; 
    int current_row = row; 
    while (row == current_row || column > current_column){
        backward(); 
    }
    return true; 
}
bool TextBuffer::down(){
    Iterator temp = cursor; 
    while (true){
        if (temp == data.end()){
            return false; 
        }
        if (*temp == '\n'){
            break; 
        }
        temp++;
    }
    int current_column = column; 
    move_to_row_end(); 
    forward(); 
    while (column < current_column && cursor != data.end() && *cursor != '\n'){
        forward(); 
    }
    return true; 
}


bool TextBuffer::is_at_end() const{
    return (cursor == data.end()); 
}
char TextBuffer::data_at_cursor() const{
    return *cursor; 
}
int TextBuffer::get_row() const{
    return row; 
}
int TextBuffer::get_column() const{
    return column;
}
int TextBuffer::get_index() const{
    return index; 
}
int TextBuffer::size() const{
    return data.size(); 
}
string TextBuffer::stringify() const{
    return string(data.begin(), data.end()); 
}
int TextBuffer::compute_column() const {
    if (cursor == data.begin()){
        return 0; 
    }
    int col = 0;
    Iterator temp = cursor;
    --temp;
    if (row == 1){
        col++; 
        while (temp != data.begin()){
            col++;
            temp--;
        }
    }
    else {
        while(*temp != '\n'){
            col++; 
            temp--;
        }
    }
    return col; 
}