#pragma once

using std::string;

template<typename K>
struct HashFunction // Hash function for evetything that is not a string (basic)
{
    size_t operator()(const K& key) // Overriding the () operator
    {
        return key; // If char - return ASCII code, else returns initial value
    }
};

template<>
struct HashFunction<string> // Specialization of the hash function for string key
{
    static size_t get_hash(const char* str)
    {
        unsigned int seed = 121; // Initial seed
        unsigned int hash = 0; // Result hash

        while (*str) // While string isn't empty
        {
            hash = hash * seed + (*str++); // Hash formula (str is moving to the next char)
        }

        return hash;
    }

    size_t operator()(const string& key) // Overriding the () operator
    {
        return get_hash(key.c_str()); // Transition to c-style string
    }
};
