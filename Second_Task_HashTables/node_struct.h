#pragma once

enum States
{
    EMPTY = 1, // The cell is empty
    OCCUPIED = 2, // The cell is occupied
    DELETED = 3, // The cell was "deleted"
};

template<typename K, typename V>
struct HashNode
{
    K key_;
    V value_;
    States state_;

    HashNode(const K& key = K(), const V& value = V())
        : key_(key)
        , value_(value)
        , state_(EMPTY)
    {}
};
