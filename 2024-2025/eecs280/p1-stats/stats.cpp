// stats.cpp
#include "stats.hpp"
#include <cassert>
#include <vector>
#include <algorithm> // sort
#include <cmath> // sqrt, modf

using namespace std;

int count(vector<double> v) {
    return v.size(); 
}

double sum(vector<double> v) {
    double total = 0;
    for (int i =0; i < v.size(); i++){
        total += v[i];
    }
    return total; 
}

double mean(vector<double> v){
    return sum(v)/count(v);
}

double median(vector<double> v) {
    sort(v.begin(), v.end());
    int l = v.size(); 
    if (l % 2 == 0){
        return (v[l/2]+v[l/2-1])/2;
    }
    else return v[(l-1)/2];
}

double min(vector<double> v) {
    sort(v.begin(), v.end());
    return v[0]; 
}

double max(vector<double> v) {
    sort(v.begin(), v.end());
    return v[v.size()-1]; 
}

double stdev(vector<double> v) {
    double m = mean(v); 
    double SD = 0; 
    for (int i = 0; i < v.size(); i++){
        SD += pow(v[i]-m,2); 
    }
    SD = sqrt(SD/(double)(v.size()-1)); 
    return SD; 
}

double percentile(vector<double> v, double p) {
    sort(v.begin(), v.end());
    double percentile; 
    double rank = p *(v.size()-1)+1;
    double intpart = 0;
    double fractpart = 0;
    fractpart = modf(rank, &intpart); 
    if (v.size() == intpart){
        return v[intpart-1];
    }
    else return percentile = v[intpart-1]+ fractpart* (v[intpart] - v[intpart - 1]);
}

vector<double> filter(vector<double> v,
                      vector<double> criteria,
                      double target) {
    vector<double> modified; 
    for (int i = 0; i < criteria.size(); i++){
        if (criteria[i] == target){
            modified.push_back(v[i]);
        }
    }
    return modified; 
}
