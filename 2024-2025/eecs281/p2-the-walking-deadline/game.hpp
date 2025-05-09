// 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef GAME_HPP
#define GAME_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "P2random.h"

using namespace std; 

class Game{
private: 

    class Zombie {
        friend class Game; 
        friend class Zombie_comp; 
        private:
            string name; 
            uint32_t distance;
            uint32_t speed; 
            uint32_t health; 
            int spawn_round; 
            int active_rounds = 0; 

        public: 
            Zombie(string name, uint32_t distance, uint32_t speed, uint32_t health, int spawn_round) : name(name), distance(distance),
                    speed(speed), health(health), spawn_round(spawn_round) {}
    };

    size_t stats; 
    bool verbose; 
    bool median; 
    uint32_t quiver_capacity;
    int round = 1;
    vector<Zombie> zombies;
    vector<size_t> first_zombies_killed;


public: 
    class Zombie_comp{
        public: 
        Game & game; 

        Zombie_comp(Game & game) : game(game) {}

        bool operator()(size_t a, size_t b){
            uint32_t ETA1 = game.zombies[a].distance/game.zombies[a].speed;
            uint32_t ETA2 = game.zombies[b].distance/game.zombies[b].speed;
            if (ETA1 == ETA2){
                if (game.zombies[a].health == game.zombies[b].health){
                    return game.zombies[a].name > game.zombies[b].name; 
                }
                else return (game.zombies[a].health > game.zombies[b].health); 
            }
            else return ETA1 > ETA2;  
        }
    };

    class Least_active{
        public: 
        Game & game; 
        
        Least_active(Game & game) : game(game){}
        
        bool operator()(size_t a, size_t b){
            if (game.zombies[a].active_rounds == game.zombies[b].active_rounds){
                return game.zombies[a].name > game.zombies[b].name; 
            }
            else return (game.zombies[a].active_rounds > game.zombies[b].active_rounds); 
        }
    };

    class Most_active{
        public: 
        Game & game; 
        
        Most_active(Game & game) : game(game){}
        
        bool operator()(size_t a, size_t b){
            if (game.zombies[a].active_rounds == game.zombies[b].active_rounds){
                return game.zombies[a].name > game.zombies[b].name; 
            }
            else return (game.zombies[a].active_rounds < game.zombies[b].active_rounds); 
        }
    };

    priority_queue<size_t, vector<size_t>, Zombie_comp> pq;  
    priority_queue<size_t, vector<size_t>, Least_active> least_active; 
    priority_queue<size_t, vector<size_t>, Most_active> most_active; 

    Game(uint32_t stats, bool verbose, bool median, uint32_t quiver_capacity); 

    void init_random(uint32_t random_seed, uint32_t max_random_distance, uint32_t max_random_speed, uint32_t max_random_health){
        P2random::initialize(random_seed, max_random_distance, max_random_speed, max_random_health); 
    }  

    void play();

    size_t zombie_attack();

    void new_zombies(); 

    size_t player_shoot(); 
};

#endif