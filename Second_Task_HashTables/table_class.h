#pragma once

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <fstream>

#include "node_struct.h"
#include "hash_struct.h"
#include "variadic_table.h"

#define START_SIZE 10

template<typename K, typename V, typename Hash = HashFunction<K>>
class HashTable
{
    typedef HashNode<K, V> Node;

public:

    HashTable() {} // Basic constructor 

    HashTable(const size_t new_size) 
    {
        tables_.set_size(new_size);
    }

    bool insert(const K& key, const V& value)
    {
        check_capacity(); // Checking if rehashing is needed

        // Exit if there is already an element with similar key:
        try
        {
            if (find(key)) { return false; }
        }
        catch (const std::invalid_argument& invArg)
        {
            std::cout << invArg.what() << '\n';
        }

        size_t iterator = 1, index = get_hash_index(key), start = index;

        while (tables_.data_[index].state_ == OCCUPIED)
        {
            calc_new_index(iterator, index, start);
        }

        // New node initialization:
        tables_.data_[index].key_ = key;
        tables_.data_[index].value_ = value;
        tables_.data_[index].state_ = OCCUPIED;

        tables_.occupied_++;

        return true;
    }

    bool remove(const K& key, const string& txt_file_name)
    {
        Node* node = find(key);

        if (node) // Node was found
        {
            node->state_ = DELETED;
            tables_.occupied_--;

            std::ofstream out_file("temp.txt", std::ios::out);

            if (!out_file.is_open())
            {
                std::cerr << "Cannot open the file(s).\n";
                return false;
            }

            size_t occupied_counter = 0;

            for (int i = 0; i < tables_.size_; i++) 
            {   
                if (tables_.data_[i].state_ == OCCUPIED)
                {
                    if (tables_.data_[i].key_ != key)
                    {
                        out_file << tables_.data_[i].key_ << "_"
                            << tables_.data_[i].value_ << ";";

                        occupied_counter++;

                        if (occupied_counter != tables_.occupied_)
                        {
                            out_file << "\n";
                        }
                    }   
                }   
            }

            out_file.close();

            std::remove(txt_file_name.data()); // data() for c-style string
            std::rename("temp.txt", txt_file_name.data());

            return true;
        }
        else
        {
            throw std::invalid_argument("\nThe key is incorrect.\n");
        }
    }

    void print() 
    {
        VariadicTable<size_t, K, V> variadic_table({
            "Index", 
            "Key (birthday)", 
            "Value (name)"
        });

        for (size_t i = 0; i < tables_.size_; i++)
        {
            if (tables_.data_[i].state_ != DELETED)
            {
                variadic_table.addRow(
                    i, 
                    tables_.data_[i].key_, 
                    tables_.data_[i].value_
                );
            }
        }

        variadic_table.print(std::cout);
    }

    void printNode(const K& key)
    {
        Node* testNode = find(key);

        if (!testNode)
        {
            throw std::invalid_argument("\nThe key is incorrect.\n");
        }

        VariadicTable<K, V> variadic_table({ "Key (birthday)", "Value (name)" });
        variadic_table.addRow(testNode->key_, testNode->value_);
        variadic_table.print(std::cout);
    }

private:

    struct Tables
    {
        Node* data_;
        size_t size_, occupied_;

        Tables() : size_(START_SIZE), occupied_(0) 
        {
            data_ = new Node[START_SIZE];
        }

        void set_size(const size_t new_size) 
        {
            data_ = new Node[new_size];
            size_ = new_size;
            occupied_ = 0;
        }
    };

    Tables tables_;

    void check_capacity() 
    {
        if (tables_.occupied_ * 10 / tables_.size_ >= 7)
        {
            rehash(); // All nodes were removed or coef. is more than 0.7 (70%)
        }
    }

    size_t get_hash_index(const K& key)
    {
        // Index is in the range of the tab length:

        Hash hash_function;

        return hash_function(key) % tables_.size_;
    }

    void rehash()
    {
        size_t new_size = tables_.size_ * 2;

        if (new_size == 0 || tables_.occupied_ == 0) { new_size = START_SIZE; }

        HashTable<K, V, Hash> new_hash_table(new_size);

        for (size_t i = 0; i < tables_.size_; i++)
        {
            if (tables_.data_[i].state_ == OCCUPIED) // Insert only occupied nodes
            {
                new_hash_table.insert(
                    tables_.data_[i].key_, 
                    tables_.data_[i].value_
                );
            }
        }

        this->tables_ = new_hash_table.tables_;
    }

    void calc_new_index(size_t& iterator, size_t& index, size_t& start) 
    {
        index = start + iterator * iterator;
        index = index % tables_.size_; // Out of bounds prevention

        iterator++;
        index++; // Linear incrimentation

        if (index >= tables_.size_)
        {
            index = 0;
        }
    }

    Node* find(const K& key)
    {
        size_t iterator = 1, index = get_hash_index(key), start = index;

        while (tables_.data_[index].state_ != EMPTY)
        {
            if (tables_.data_[index].key_ == key)
            {
                if (tables_.data_[index].state_ == OCCUPIED)
                {
                    return &tables_.data_[index];
                }
                else
                {
                    throw std::invalid_argument("\nThe key is incorrect.\n");
                }
            }

            calc_new_index(iterator, index, start);
        }

        return NULL;
    }
};
