/* stats_tests.cpp
 *
 * Unit tests for the simple statistics library
 *
 * EECS 280 Project 1
 *
 * Protip #1: Write tests for the functions BEFORE you implement them!  For
 * example, write tests for median() first, and then write median().  It sounds
 * like a pain, but it helps make sure that you are never under the illusion
 * that your code works when it's actually full of bugs.
 *
 * Protip #2: Instead of putting all your tests in main(),  put each test case
 * in a function!
 */


#include "stats.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
using namespace std;

// Add prototypes for you test functions here.



void test_count(){
  vector<double> data;
  data.push_back(1);
  data.push_back(2.6);
  data.push_back(3.5);
  data.push_back(4); 
  if (count(data) != 4){
    assert(false); 
  }
}

void test_sum() {
  vector<double> data;
  data.push_back(1);
  data.push_back(2.6);
  data.push_back(3.5);
  data.push_back(4); 
  if (sum(data) != 11.1){
    assert(false); 
  }
}

void test_mean(){
  vector<double> data;
  data.push_back(1);
  data.push_back(2.6);
  data.push_back(3.5);
  data.push_back(4);  
  if (mean(data) != 2.775){
    assert(false); 
  }
}

void test_median(){
  vector<double> data;
  data.push_back(1);
  data.push_back(2.6);
  data.push_back(3.5);
  data.push_back(4);  
  if (median(data) != 3.05){
    assert(false); 
  }
}

void test_min(){
  vector<double> data;
  data.push_back(1);
  data.push_back(2.6);
  data.push_back(3.5);
  data.push_back(4);   
  if (min(data) != 1){
    assert(false); 
  }
}

void test_max(){
  vector<double> data;
  data.push_back(1);
  data.push_back(2.6);
  data.push_back(3.5);
  data.push_back(4);   
  if (max(data) != 4){
    assert(false); 
  }
}

void test_stdev(){
  vector<double> data;
  data.push_back(1);
  data.push_back(2.6);
  data.push_back(3.5);
  data.push_back(4);   
  if (abs(stdev(data)-1.31751028) > 0.0001){
    assert(false); 
  }
}

void test_percentile(){
  vector<double> data;
  data.push_back(1);
  data.push_back(2.6);
  data.push_back(3.5);
  data.push_back(4);   
  if (percentile(data, 1) != 4){
    assert(false); 
  }
}

void test_filter(){
  vector<double> data;
  data.push_back(1);
  data.push_back(2.6);
  data.push_back(3.5);
  data.push_back(4);  
  vector<double> criteria = {2,3,2,1};
  vector<double> result = {1,3.5}; 
  if (filter(data, criteria, 2) != result){
    assert(false); 
  }
}

int main() {
  test_count();
  test_sum();
  test_mean();
  test_median();
  test_min();
  test_max();
  test_stdev();
  test_percentile();
  test_filter(); 
  cout << "PASS" << endl;
  // Call your test functions here
  return 0;
}
// Add the test function implementations here.
