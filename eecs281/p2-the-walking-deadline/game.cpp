// 9504853406CBAC39EE89AA3AD238AA12CA198043

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "game.hpp"
#include <limits>
#include <queue>

using namespace std; 

Game::Game(uint32_t stats, bool verbose, bool median, uint32_t quiver_capacity) : stats(stats), verbose(verbose), median(median),
        quiver_capacity(quiver_capacity), pq(Zombie_comp(*this)), least_active(Least_active(*this)), most_active(Most_active(*this)){}

void Game::play(){
    int next_round; 
    string ignore; 
    cin >> ignore; 
    cin >> ignore; 
    cin >> next_round; 
    size_t dead = SIZE_MAX; 
    size_t last_zombie = SIZE_MAX; 
    while ((next_round != -1) || !pq.empty()){
        if (verbose){
            cout << "Round: " << round << "\n"; 
        }
        //zombies move
        dead = zombie_attack(); 

        //if player is dead
        if (dead != SIZE_MAX){
            break; 
        }

        //potentially spawn new zombies
        if (round == next_round){
            new_zombies(); 
            if (cin >> ignore){
                cin >> ignore;
                cin >> next_round;
            }
            else {
                next_round = -1; 
            }
        }

        //player shoots zombies
        last_zombie = player_shoot(); 

        // display median 
        if (median){
            if (!most_active.empty() && most_active.size() == least_active.size()){
                int total = zombies[most_active.top()].active_rounds + zombies[least_active.top()].active_rounds; 
                cout << "At the end of round " << round << ", the median zombie lifetime is " << total / 2 << "\n"; 
            }
            else if (!most_active.empty()){
                cout << "At the end of round " << round << ", the median zombie lifetime is " << zombies[most_active.top()].active_rounds << "\n"; 
            }
        }

        round++; 
    }
    // endgame output
    if (dead != SIZE_MAX){
        cout << "DEFEAT IN ROUND " << round << "! " << zombies[dead].name << " ate your brains!\n"; 
    }
    else {
        cout << "VICTORY IN ROUND " << round - 1 << "! " << zombies[last_zombie].name << " was the last zombie.\n"; 
    }

    // stats
    if (stats != 0){
        cout << "Zombies still active: " << pq.size() << "\n";
        cout << "First zombies killed:\n";
        size_t stats_zombies_killed = min(stats, first_zombies_killed.size()); 
        for (size_t i = 0; i < stats_zombies_killed; i++){
            cout << zombies[first_zombies_killed[i]].name << " " << i + 1 << "\n";
        }
        cout << "Last zombies killed:\n";
        for (size_t i = first_zombies_killed.size() - 1; i >= first_zombies_killed.size() - stats_zombies_killed && i != SIZE_MAX; i--){
            cout << zombies[first_zombies_killed[i]].name << " " << stats_zombies_killed + i - (first_zombies_killed.size() - 1) << "\n";
        }
        cout << "Most active zombies:\n";
        priority_queue<size_t, vector<size_t>, Least_active> least_active_pq(Least_active(*this)); 
        priority_queue<size_t, vector<size_t>, Most_active> most_active_pq(Most_active(*this)); 
        while (!most_active.empty()){
            least_active_pq.push(most_active.top()); 
            most_active_pq.push(most_active.top()); 
            most_active.pop(); 
        }
        while (!least_active.empty()){
            least_active_pq.push(least_active.top()); 
            most_active_pq.push(least_active.top()); 
            least_active.pop();             
        }
        // pushing alive zombies
        for (size_t i = 0; i < zombies.size(); i++){
            if (zombies[i].active_rounds == 0){
                zombies[i].active_rounds = round - zombies[i].spawn_round + 1; 
                least_active_pq.push(i);
                most_active_pq.push(i); 
            }
        }
        stats = min(stats, zombies.size());
        
        for (size_t i = 0; i < stats; i++){
            cout << zombies[most_active_pq.top()].name << " " << zombies[most_active_pq.top()].active_rounds << "\n"; 
            most_active_pq.pop(); 
        }

        cout << "Least active zombies:\n"; 
        for (size_t i = 0; i < stats; i++){
            cout << zombies[least_active_pq.top()].name << " " << zombies[least_active_pq.top()].active_rounds << "\n"; 
            least_active_pq.pop(); 
        }
    }
}

size_t Game::zombie_attack(){
    size_t last_zombie = SIZE_MAX; 
    for (size_t i = 0; i < zombies.size(); i++){
        if (zombies[i].health != 0){
            zombies[i].distance -= min(zombies[i].distance, zombies[i].speed);
            if (verbose){
                cout << "Moved: " << zombies[i].name << " (distance: " << zombies[i].distance << ", speed: " 
                        << zombies[i].speed << ", health: " << zombies[i].health << ")\n"; 
            }
            if (last_zombie == SIZE_MAX && zombies[i].distance == 0) {
                last_zombie = i; 
            }
        }
    }
    return last_zombie; 
}

void Game::new_zombies(){
    // random zombies
    string ignore; 
    cin >> ignore; 
    size_t num_zombies; 
    cin >> num_zombies; 
    for (size_t i = 0; i < num_zombies; i++){
        string name  = P2random::getNextZombieName();
	    uint32_t distance = P2random::getNextZombieDistance();
	    uint32_t speed    = P2random::getNextZombieSpeed();
	    uint32_t health   = P2random::getNextZombieHealth();
        zombies.push_back(Zombie(name, distance, speed, health, round)); 
        pq.push(zombies.size() - 1); 
        if (verbose){
            cout << "Created: " << name << " (distance: " << distance << ", speed: " << speed << ", health: " << health << ")\n"; 
        }
    }
    //named zombies
    cin >> ignore;
    cin >> num_zombies; 
    string name; 
    uint32_t distance;
    uint32_t speed; 
    uint32_t health; 
    for (size_t i = 0; i < num_zombies; i++){
        cin >> name; 
        cin >> ignore; 
        cin >> distance; 
        cin >> ignore; 
        cin >> speed; 
        cin >> ignore; 
        cin >> health; 
        zombies.push_back(Zombie(name, distance, speed, health, round)); 
        pq.push(zombies.size() - 1); 
        if (verbose){
            cout << "Created: " << name << " (distance: " << distance << ", speed: " << speed << ", health: " << health << ")\n"; 
        }
    }
}

size_t Game::player_shoot(){
    uint32_t num_arrows = quiver_capacity; 
    size_t last_zombie = SIZE_MAX; 
    while (!pq.empty() && num_arrows > 0){
        Zombie & zombie = zombies[pq.top()];
        last_zombie = pq.top(); 
        // shoot arrow at zombie
        uint32_t damage = min(zombie.health, num_arrows);
        num_arrows -= damage; 
        zombie.health -= damage; 
        if (zombie.health == 0){
            zombie.active_rounds = round - zombie.spawn_round + 1;
            first_zombies_killed.push_back(pq.top());
            
            //for stats and median
            size_t a = pq.top(); 
            if (most_active.empty() || zombies[a].active_rounds <= zombies[most_active.top()].active_rounds){
                most_active.push(a); 
            }
            else {
                least_active.push(a); 
            }
            if (most_active.size() > least_active.size() + 1){
                least_active.push(most_active.top()); 
                most_active.pop(); 
            }
            else if (least_active.size() > most_active.size()){
                most_active.push(least_active.top());
                least_active.pop(); 
            }
            

            // for verbose
            if (verbose){
                cout << "Destroyed: " << zombie.name << " (distance: " << zombie.distance << ", speed: " 
                        << zombie.speed << ", health: " << zombie.health << ")\n";
            }
            pq.pop(); 
        }
    }
    return last_zombie; 
}
