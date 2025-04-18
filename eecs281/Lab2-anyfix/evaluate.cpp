// Project identifier: 1CAEF3A0FEDD0DEC26BA9808C69D4D22A9962768

#include "evaluate.hpp"
#include <stack> 
#include <cstdint>
#include <iostream> 
using namespace std; 

int64_t do_calculation(int64_t a, int64_t b, char op){
    switch (op){
        case '+':
            return a+b; 
            break; 
        case '-':
            return a-b; 
            break; 
        case '*':
            return a*b; 
            break; 
        case '/':
            return a/b; 
            break; 
        default:
            cout << "no operator found";
            exit(1); 
    }
}
/*
bool func(stack<pair<int64_t, char>> & stack){  
    pair<int64_t, char> third = stack.top(); 
    stack.pop(); 
    pair<int64_t, char> second = stack.top(); 
    stack.pop();
    pair<int64_t, char> first = stack.top(); 
    stack.pop(); 
    if (first.second != '.' && second.second == '.' && third.second == '.'){
       int64_t result = do_calculation(second.first, third.first, first.second);
       stack.push({result, '.'}); 
       return true; 
    }
    else if (first.second == '.' && second.second != '.' && third.second == '.'){
       int64_t result = do_calculation(first.first, third.first, second.second);
       stack.push({result, '.'}); 
       return true;
    }    
    else if (first.second == '.' && second.second == '.' && third.second != '.'){
       int64_t result = do_calculation(first.first, second.first, third.second);
       stack.push({result, '.'}); 
       return true;
    }
    else {
        stack.push(first);
        stack.push(second);
        stack.push(third); 
        return false; 
    }
}
*/

std::int64_t evaluate(std::string const& expression){
    stack<int64_t> num_stack;
    stack<char> operator_stack;     

    for (char c : expression) {
        if (isdigit(c)) {
            num_stack.push(c - '0');  
        } 
        else { 
            operator_stack.push(c);
        }
        while (!operator_stack.empty() && num_stack.size() >= 2) {
            int64_t b = num_stack.top(); 
            num_stack.pop();
            int64_t a = num_stack.top(); 
            num_stack.pop();
            char op = operator_stack.top(); 
            operator_stack.pop();
            num_stack.push(do_calculation(a, b, op));
        }
    }
    return num_stack.top();
}
