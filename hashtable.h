#ifndef cop4530_hashtable_h
#define cop4530_hashtable_h
#include <iostream>
#include <vector>
#include <fstream>
#include <list>
#include <algorithm>
static const unsigned int default_capacity = 11;
static const unsigned int max_prime = 1301081;
namespace cop4530{
    template<typename T>
    class HashTable {
    public:
        HashTable(size_t size = 101);

        ~HashTable();

        bool contains(const T &x);

        bool insert(const T &x);

        bool insert(T &&x);

        bool remove(const T &x);

        void clear();

        bool load(const char *filename);

        void dump();

        bool write_to_file(const char *filename);
        //added operator and next prime based off the powerpoints

        bool operator==(const HashTable &rhs) const;

        bool operator!=(const HashTable &rhs) const;

        unsigned long nextPrime(unsigned long n);

        int getSize();

    private:
        std::vector<std::list<T>> theLists;
        int currentSize;

        void makeEmpty();

        void rehash();

        size_t myhash(const T &x);

        unsigned long prime_below(long);

        void setPrimes(std::vector<long> &);
    };
#include "hashtable.hpp"
}
#endif