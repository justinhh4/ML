#ifndef HASHTABLE_H
#define HASHTABLE_H

// Project Identifier: 2C4A3C53CD5AD45A7BEA3AE5356A3B1622C9D04B

// INSTRUCTIONS:
// fill out the methods in the class below.

// You may assume that the key and value types can be copied and have default
// constructors.

// You can also assume that the key type has (==) and (!=) methods.

// You may assume that Hasher is a functor type with a
// size_t operator()(const Key&) overload.

// The key/value types aren't guaranteed to support any other operations.

// Do not add, remove, or change any of the class's member variables.
// The num_deleted counter is *optional*. You might find it helpful, but it
// is not required to pass the lab assignment.

// Do not change the Bucket type.

// SUBMISSION INSTRUCTIONS:
// Submit this file, by itself, in a .tar.gz.
// Other files will be ignored.

#include <cstdint>
#include <functional> // where std::hash lives
#include <vector>
#include <cassert> // useful for debugging!

// A bucket's status tells you whether it's empty, occupied, 
// or contains a deleted element.
enum class Status : uint8_t {
    Empty,
    Occupied,
    Deleted
};

template<typename K, typename V, typename Hasher = std::hash<K>>
class HashTable {
    // used by the autograder; do not change/remove.
    friend class Verifier;
public:
    // A bucket has a status, a key, and a value.
    struct Bucket {
        // Do not modify Bucket.
        Status status = Status::Empty;
        K key;
        V val;
    };

    HashTable() {
        // TODO: a default constructor (possibly empty).
        buckets.resize(20); 
        for (size_t i = 0; i < 20; i++){
            buckets[i].status = Status::Empty; 
        }
        // You can use the following to avoid implementing rehash_and_grow().
        // However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
        //    buckets.resize(10000);
    }

    size_t size() const {
        return num_elements;
    }

    // returns a reference to the value in the bucket with the key, if it
    // already exists. Otherwise, insert it with a default value, and return
    // a reference to the resulting bucket.
    V& operator[](const K& key) {
        // TODO
        Hasher hasher; 
        size_t i = hasher(key) % buckets.size(); 
        V val = V{}; 
        insert(key, val); 
        while (true){
            if (buckets[i].status == Status::Occupied && buckets[i].key == key){
                return buckets[i].val; 
            }
            i = (i+1) % buckets.size(); 
        }
    }

    // insert returns whether inserted successfully
    // (if the key already exists in the table, do nothing and return false).
    bool insert(const K& key, const V& val) {
        //TODO
        Hasher hasher; 
        size_t i = hasher(key) % buckets.size(); 
        size_t first_deleted = SIZE_MAX; 
        size_t num_iterations = 0; 
        while (buckets[i].status != Status::Empty && num_iterations < buckets.size()){
            num_iterations++; 
            if (buckets[i].status == Status::Deleted && first_deleted == SIZE_MAX){
                first_deleted = i; 
            }
            if (buckets[i].status == Status::Occupied && buckets[i].key == key){
                return false; 
            }
            i = (i + 1) % buckets.size();
        }
        num_elements++; 
        if (first_deleted != SIZE_MAX) i = first_deleted; 
        buckets[i].status = Status::Occupied; 
        buckets[i].key = key; 
        buckets[i].val = val; 
        // CHECK FOR INCRESING SIZE
        if (num_elements * 2 > buckets.size()){
            rehash_and_grow(); 
        }
        return true; 
    }
    // erase returns the number of items remove (0 or 1)
    size_t erase(const K& key) {
        // TODO
        Hasher hasher; 
        size_t i = hasher(key) % buckets.size(); 
        for (size_t j = 0; j < buckets.size(); j++){
            if (buckets[i].status == Status::Occupied && buckets[i].key == key){
                buckets[i].status = Status::Deleted; 
                num_elements--; 
                return 1; 
            }
            else if (buckets[i].status == Status::Empty){
                return 0; 
            }
            i = (i+1) % buckets.size(); 
        }
        return 0; 
    }

private:
    size_t num_elements = 0;
    size_t num_deleted = 0; // OPTIONAL: you don't need to use num_deleted to pass
    std::vector<Bucket> buckets;

    void rehash_and_grow() {
        std::vector<Bucket> temp;
        temp.resize(buckets.size() * 2); 
        for (size_t i = 0; i < temp.size(); i++){
            temp[i].status = Status::Empty;
        }
        Hasher hasher; 
        for (size_t i = 0; i < buckets.size(); i++){
            if (buckets[i].status == Status::Occupied){
                size_t new_index = hasher(buckets[i].key) % temp.size(); 
                while (temp[new_index].status == Status::Occupied){
                    new_index = (new_index + 1) % temp.size(); 
                }
                temp[new_index] = buckets[i]; 
            }
        }
        buckets = temp; 
        // You can avoid implementing rehash_and_grow() by calling
        //    buckets.resize(10000);
        // in the constructor. However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
    }

    // You can add methods here if you like.
    // TODO
};

#endif // HASHTABLE_H