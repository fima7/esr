# Hashtable<K, V> 
Implementation of Hash table with separate chaining method without using
any STL container classes.

Hash table inmplemented as a template class Hashtable<K, V>, K corresponds to
type of key, V corresponds to type of value.

Hashtable&lt;K, V> is implemented as an array of buckets. A bucket is a LinkedList&lt;K V> class.
LinkedList&lt;K, V> is an implementation of singly linked list to store keys and values.

## Runing times

### Runtime of Hashtable<K, V>
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

### Tests

### Results