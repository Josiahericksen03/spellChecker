#include "hashtable.h"
using namespace cop4530;
using namespace std;
// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename T>
unsigned long HashTable<T>::prime_below (long n)
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
		std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	return n;
      --n;
    }

  return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n) 
template <typename T>
void HashTable<T>::setPrimes(std::vector<long>& vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}

template<typename T>
HashTable<T>::HashTable(size_t size) : currentSize(prime_below(size)){
    theLists.resize(prime_below(size));
    //output for both with and without parameters auto set size to prime below users input, so just
    //set it up in constructor
}

template<typename T>
HashTable<T>::~HashTable(){
    makeEmpty();
}

template<typename T>
bool HashTable<T>::contains(const T &x) {
    auto & whichList = theLists[myhash(x)];
    //hashes x to find index of which list in vector to search through
    return find(begin(whichList), end(whichList), x) != end(whichList);
    //find funct with that lists size
}

template<typename T>
bool HashTable<T>::insert(const T &x) {
    auto & whichList = theLists[myhash(x)];
    //hashes x to find index of which list in vector to search through
    auto it = find(begin(whichList),end(whichList), x);
    if(it != end(whichList))
        return false;

    whichList.push_back(x);

    //  rehash, see Section 5.5  < your function
    if (++currentSize > theLists.size())
        rehash();

    return true;

}

template<typename T>
bool HashTable<T>::insert(T &&x) {
    auto & whichList = theLists[myhash(x)];
    auto it = find(begin(whichList),end(whichList), x);
    if(it != end(whichList))
        return false;

    whichList.push_back(move(x));

    //  rehash, see Section 5.5 < your function
    if (++currentSize > theLists.size())
        rehash();

    return true;
}

template<typename T>
bool HashTable<T>::remove(const T &x) {
    auto & whichList = theLists[myhash(x)];
    auto itr = find(begin(whichList), end(whichList), x);

    if (itr == end(whichList))
        return false;
    whichList.erase(itr);
    --currentSize;
    return true;
    //self expplainatory. looks through, erases at itr, minus 1 size
}

template<typename T>
void HashTable<T>::clear() {
    makeEmpty();
}

template<typename T>
bool HashTable<T>::load(const char *filename) {
    ifstream inFile(filename);
    if (!inFile) {
        return false; //makes sure file opne or not
    }

    makeEmpty(); //clear table

    T value;
    while (inFile >> value) {
        insert(value); //insert into hash
    }

    inFile.close();
    return true;
}

template<typename T>
void HashTable<T>::dump() {
    for (size_t i = 0; i < theLists.size(); ++i) {
        cout <<"v["<<i<<"]"<< ": "; //outting content of table with formatting

        for (const auto &value : theLists[i]) {
            cout << value << "\t"; //each element in the linked list separated by tab
        }

        cout << endl;
    }
}

template<typename T>
bool HashTable<T>::write_to_file(const char *filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
        return false;
    }

    for (const auto &bucket : theLists) {
        for (const auto &value : bucket) {
            outFile << value << endl; //each element to file on a new line
        }
    }

    outFile.close();
    return true;
}

template<typename T>
bool HashTable<T>::operator!=(const HashTable &rhs) const {
    return !(*this == rhs);
}

template<typename T>
bool HashTable<T>::operator==(const HashTable &rhs) const {
    if (this == &rhs)
        return true;

    if (currentSize != rhs.currentSize)
        return false;

    for (const auto &bucket : theLists) {
        for (const auto &value : bucket) {
            if (!rhs.contains(value))
                //loop through to see if not in there
                return false;
        }
    }

    return true;
}

template<typename T>
void HashTable<T>::makeEmpty() {
    for(auto & thisList : theLists){
        thisList.clear();
    }
    currentSize = 0;
    //self explainatory
}

template<typename T>
void HashTable<T>::rehash() {
    vector<list<T>> oldLists = theLists;

    theLists.resize(nextPrime(2 * theLists.size()));
    for (auto & thisList: theLists)
        thisList.clear();

    // copy table over
    currentSize = 0;
    for (auto & thisList : oldLists)
        for (auto & x : thisList)
            insert(std::move(x));

}

template<typename T>
size_t HashTable<T>::myhash(const T &x) {
    static hash<T> hf;
    return hf(x) % theLists.size();
    //hash you gave
}

template<typename T>
unsigned long HashTable<T>::nextPrime(unsigned long n) {
    if (n <= 1)
        return 2;
    //implementing next prime that was in your rehash function
    while (true) {
        bool isPrime = true;
        for (unsigned long i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
            return n;
        //looping until prime found, if so then output
        ++n;
    }
}

template<typename T>
int HashTable<T>::getSize() {
    return currentSize;
    //funct i made for easier in main
}
