// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <climits> 
#include <cmath> 
#include <map>
#include <set> 

using namespace std; 

struct Vertex{
    int x; 
    int y; 
    bool visited = false;
    long long distance = LLONG_MAX; 
    size_t parent; 
};

long long TSP_distance(Vertex & a, Vertex & b){
    return ((long long)(a.x - b.x) * (a.x - b.x) + (long long)(a.y - b.y) * (a.y - b.y)); 
}

long long distance(Vertex & a, Vertex & b){
    if (a.x < 0 && a.y < 0 && !(b.x <= 0 && b.y <= 0)){
        return LLONG_MAX; 
    }
    else if (b.x < 0 && b.y < 0 && !(a.x <= 0 && a.y <= 0)){
        return LLONG_MAX; 
    }
    else {
        return ((long long)(a.x - b.x) * (a.x - b.x) + (long long)(a.y - b.y) * (a.y - b.y)); 
    }
}

void find_MST(vector<Vertex> & vertices){
    double MST_weight = 0; 
    vector<pair<size_t, size_t>> MST_list; 
    vertices[0].distance = 0; 
    size_t min_index = 0; 
    // loop v times
    for (size_t i = 0; i < vertices.size(); i++){
        // find next vertex to connect
        long long min_distance = LLONG_MAX;
        for (size_t j = 0; j < vertices.size(); j++){
            if (vertices[j].visited == false && min_distance > vertices[j].distance){
                min_index = j; 
                min_distance = vertices[j].distance; 
            }
        }
        // check if MST is impossible  
        
        if (vertices[min_index].visited == true){
            cerr << "Cannot construct MST on iteration " << i; 
            exit(1); 
        }
            
        // add vertex to MST
        if (i != 0){
            MST_weight += sqrt(vertices[min_index].distance); 
            MST_list.push_back(make_pair(min_index, vertices[min_index].parent));
        }
        vertices[min_index].visited = true; 
        // shorten distances if possible 
        for (size_t j = 0; j < vertices.size(); j++){
            if (vertices[j].visited == false){
                long long dist = distance(vertices[min_index], vertices[j]); 
                if (vertices[j].distance > dist){
                    vertices[j].distance = dist; 
                    vertices[j].parent = min_index; 
                }
            }
        }
    }

    // output MST
    cout << MST_weight << "\n"; 
    for (auto i : MST_list){
        if (i.first > i.second){
            cout << i.second << " " << i.first << "\n"; 
        }
        else {
            cout << i.first << " " << i.second << "\n"; 
        }
    }
}

double find_FASTTSP(vector<Vertex> & vertices, bool output_dist){
    // furthest insertion algorithm that starts at vertex 0 

    // make best distance vector
    vector<size_t> ans = {0}; 
    vector<bool> unvisited(vertices.size(), true); 
    unvisited[0] = false; 
    for (size_t i = 1; i < vertices.size(); i++){
        vertices[i].distance = TSP_distance(vertices[i], vertices[0]); 
    }

    // creating TSP
    while (ans.size() != vertices.size()){
        size_t insert_index; 
        long long max_dist = -1; 
        // find next vertex to insert
        for (size_t i = 0; i < unvisited.size(); i++){
            if (unvisited[i] == true && vertices[i].distance > max_dist){
                max_dist = vertices[i].distance; 
                insert_index = i; 
            }
        }
        unvisited[insert_index] = false; 

        // insert index 
        ans.push_back(ans.front()); 
        size_t insert_pos = 0;
        long long min_dist_increase = LLONG_MAX; 
        for (size_t i = 0; i < ans.size()-1; i++){
            long long a_dist = TSP_distance(vertices[ans[i]], vertices[insert_index]); 
            long long b_dist = TSP_distance(vertices[insert_index], vertices[ans[i+1]]); 
            long long c_dist = TSP_distance(vertices[ans[i]], vertices[ans[i+1]]); 
            if (a_dist != LLONG_MAX && b_dist != LLONG_MAX && (a_dist + b_dist - c_dist) < min_dist_increase){
                min_dist_increase = a_dist + b_dist - c_dist; 
                insert_pos = i+1; 
            }
        }
        ans.insert(ans.begin() + static_cast<std::vector<size_t>::difference_type>(insert_pos), insert_index);
        ans.pop_back(); 
        

        // update distance for all unvisited 
        for (size_t i = 0; i < unvisited.size(); i++){
            if (unvisited[i] == true){
                long long dist = TSP_distance(vertices[i], vertices[insert_index]); 
                vertices[i].distance = min(vertices[i].distance, dist); 
            }
        }
    }

    // output answer
    ans.push_back(ans.front()); 
    double dist = 0;
    for (size_t i = 0; i < ans.size()-1; i++){
        dist += sqrt(TSP_distance(vertices[ans[i]], vertices[ans[i+1]])); 
    }
    if (output_dist == true){
        return dist; 
    }
    else {
        cout << dist << "\n"; 
        ans.pop_back(); 
        for (auto i : ans){
            cout << i << " "; 
        }
    }
    return 0; 
}

double MST_helper(vector<Vertex> & vertices){ 
    if (vertices.size() == 1) return 0; 
    for (auto &v : vertices) {
        v.distance = LLONG_MAX;
        v.visited = false;
    }
    double MST_weight = 0; 
    vertices[0].distance = 0; 
    size_t min_index = 0; 
    // loop v times
    for (size_t i = 0; i < vertices.size(); i++){
        // find next vertex to connect
        long long min_distance = LLONG_MAX;
        for (size_t j = 0; j < vertices.size(); j++){
            if (vertices[j].visited == false && min_distance > vertices[j].distance){
                min_index = j; 
                min_distance = vertices[j].distance; 
            }
        }
            
        // add vertex to MST
        if (i != 0){
            MST_weight += sqrt(vertices[min_index].distance); 
        }
        vertices[min_index].visited = true; 
        // shorten distances if possible 
        for (size_t j = 0; j < vertices.size(); j++){
            if (vertices[j].visited == false){
                long long dist = TSP_distance(vertices[min_index], vertices[j]); 
                if (vertices[j].distance > dist){
                    vertices[j].distance = dist; 
                }
            }
        }
    }
    return MST_weight; 
}


void genPerms(vector<Vertex> & vertices, vector<size_t> &path, size_t permLength, vector<size_t> & best_path, 
    double current_length, double & best_length, vector<vector<long long>> & distances){
    if (permLength == path.size()){
        double dist = sqrt(distances[path.back()][path.front()]);
        if (current_length + dist <= best_length){
            best_path = path; 
            best_length = current_length + dist;
        }
        return;
    }  // if ..complete path
    
    vector<Vertex> remaining;
    remaining.reserve(path.size()-permLength); 
    long long min_dist = LLONG_MAX; 
    long long min_dist_b = LLONG_MAX;
    for (size_t i = permLength; i < path.size(); i++){
        remaining.push_back(vertices[path[i]]); 
        min_dist = min(min_dist, distances[path[permLength-1]][path[i]]); 
        min_dist_b = min(min_dist_b, distances[path[0]][path[i]]);
    }
    if (current_length + sqrt(min_dist_b) + sqrt(min_dist) + MST_helper(remaining) > best_length){
        return;
    }  // if ..not promising
    
    
    
    for (size_t i = permLength; i < path.size(); ++i){
        swap(path[permLength], path[i]);
        genPerms(vertices, path, permLength + 1, best_path, 
                current_length + sqrt(distances[path[permLength-1]][path[permLength]]), best_length, distances);
        swap(path[permLength], path[i]);
    }  // for ..unpermuted elements
}  // genPerms()


void find_OPTTSP(vector<Vertex> & vertices){
    double best_so_far = find_FASTTSP(vertices, true);
    vector<size_t> path(vertices.size(), 0);
    for (size_t i = 0; i < vertices.size(); i++){
        path[i] = i; 
    }
    vector<size_t> best_path(vertices.size(), 0); 
    vector<vector<long long>> distances(vertices.size(), vector<long long> (vertices.size(), 0));
    for (size_t i = 0; i < vertices.size(); i++){
        for (size_t j = 0; j < vertices.size(); j++){
            if ( i != j) distances[i][j] = TSP_distance(vertices[i], vertices[j]); 
        }
    }
    vector<Vertex> remaining = vertices; 
    genPerms(vertices, path, 1, best_path, 0, best_so_far, distances); 

    cout << best_so_far << "\n"; 
    for (auto i : best_path){
        cout << i << " "; 
    }
}

