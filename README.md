# Hashtable<K, V> 
Implementation of Hash table with separate chaining method without using
any STL container classes.

Hash table is implemented as a template class Hashtable&lt;K, V>. K is a type of key,
V is a type of value.

Hashtable&lt;K, V> structure is an array of buckets. A bucket is an object of
LinkedList&lt;K V> class. LinkedList&lt;K, V> is an implementation of singly linked
list customized to store values with unique keys.

## Runing times

### Runtime of Hash Table
n is a number of elements in Hashtable
* Hashtable::Hashtable() : O(1)
* Hashtable::Hashtable(const Hashtable& other) : O(n)
* Hashtable::~Hashtable() : O(n)
* Hashtable::operator=(Hashtable other) : O(n)
* Hashtable::add(const K& key, const V& value) : worst O(n), amortized O(1)
* Hashtable::remove(const K& key) : worst O(n), amortized O(1)
* Hashtable::set(const K& key, const V& value) : worst O(n), amortized O(1)
* Hashtable::get(const K& key) : worst O(n), amortized O(1)
* Hashtable::find(const K& key): worst O(n), amortized O(1)
* Hashtable::size() : O(1)
* Hashtable::load_factor() : O(1)
* Hashtable::iterator() consrtructor : O(1)
* Hashtable::iterator++() : O(n)
* Hashtable::iterator*() : O(1)
* Hashtable::iterator->() : O(1)
* Hashtable::iterator!=() : O(1)
* Hashtable::iterator==() : O(1)
* Hashtable::begin() : worst O(n), amortized O(1)
* Hashtable::end() : O(1)
* Hashtable::resize(size_t bucket_count) : O(n)

### Runtime of Linked List
n is a number of elements in List.
* linkedlist::linkedlist() : O(1)
* linkedlist::linkedlist(const linkedlist& other) : O(n)
* linkedlist::~linkedlist() : O(n)
* linkedlist::operator=(linkedlist other) : O(n)
* linkedlist::push_back(const K& key, const V& value) : O(n)
* linkedlist::erase(const K& key) : O(n)
* linkedlist::find(const K& key) : O(n)
* linkedlist::front() : O(1)
* linkedlist::empty() : O(1)
* linkedlist::size() : O(1)

### Code example

### Files
#### Hash Table
* esr/
  * __hashtable.hpp__ : Implementation of Hashtable.
  * __hasher.hpp__ : Provides hash functions for some basic types.
  * __linkedlist.hpp__ : Linked List implementation.
  * __hashexcept.hpp__ : Hash Table exceptions.

#### Tests
* __esr/hashtest.hpp__ : Generic test classes to check the functional correctness of Hash Table.
* __likedlist_test.hpp__ : Runs functional correctness tests for Linked List.
* __correctness_test.cpp__ : Runs a number of correctness tests for basic types.
* __performance_test.cpp__ : Runs perfomance tests using integer and std::string keys for both Hashtable and std::unordered_map.
* __cities.cpp__ : Illustrative application which uses Hashtable to process the real world data.
* __tiny_test.cpp__ : Developers sandbox to make some quick tests.

### Tests
1. > make
2. > testall

### Performance evaluation
#### Integer Keys
| Input Size | dfsfs    | dfsf       | HT FIND   | UM FIND   |
| ---------- | -------- | ---------- | --------- | --------- |
|  2	     | 0.003000	| 0.003000   |  0.000500 |  0.000000 |	       	 

