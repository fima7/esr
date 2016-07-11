# Hashtable<K, V> 
Implementation of Hash Table with separate chaining method without using
any STL container classes.

Hash table is implemented as a template class Hashtable&lt;K, V>. K is a type of key,
V is a type of value.

Hashtable&lt;K, V> structure is an array of buckets. A bucket is an object of
LinkedList&lt;K, V> class. LinkedList&lt;K, V> is an implementation of singly linked
list customized to store values with unique keys.

## Runing times
_Provided for keys and values of fixed length._

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
* Hashtable::iterator++() : worst O(n), amortized O(1)
* Hashtable::iterator*() : O(1)
* Hashtable::iterator->() : O(1)
* Hashtable::iterator!=() : O(1)
* Hashtable::iterator==() : O(1)
* Hashtable::begin() : worst O(n), amortized O(1)
* Hashtable::end() : O(1)
* Hashtable::resize(size_t bucket_count) : worst O(n), amortized O(1)

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

### Example
```
#include <esr/hashtable.hpp>

int main(int argc, char *argv[]) {
  esr::Hashtable<std::string, std::string> phone_book;
  phone_book.add("Fima", "01034900103");
  phone_book.add("Chicha", "01070300103");
  phone_book.add("Kit", "01070300777");

  auto record = phone_book.find("Fima");
  if (record != phone_book.end())
    std::cout << record->key() << ", " << record->value()  << '\n';

  for (auto& record : phone_book)
    std::cout << record.key() << ", " << record.value()  << '\n';

  return 0;
}
```
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
* __cities.cpp__ : Illustrative application which uses Hashtable to process the real world data
with custom key type.
* __tiny_test.cpp__ : My sandbox to make some quick tests.

### Compile and Run 
```
make
./testall
```
### Performance evaluation
* _HT_ stands for Hash Table.
* _UM_ stands for std::unordered_map.
* _ADD()_ insertion operation.
* _FIND()_ retrieval operation.
First column contains a number of elements in Hash Table.
Next four columns contain CPU time in miliseconds consumed by one
operation. 

#### Integer Keys
| Input Size | HT ADD() | UM ADD()   | HT FIND() | UM FIND() |
| ---------- | -------- | ---------- | --------- | --------- |
|  2	     | 0.003000	| 0.003000   |  0.000500 |  0.000000 |	       	 
|  4 	     | 0.000500	| 0.000500   |  0.000000 |  0.000250 |		
|  8	     | 0.00025	| 0 0.000625 |  0.000125 |  0.000125 |		
|  16	     | 0.000250 | 0.000250   |  0.000000 |  0.000000 |		
|  32	     | 0.000313 | 0.000188   |  0.000031 |  0.000031 |		
|  64	     | 0.000234 | 0.000188   |  0.000031 |  0.000016 |		
|  128	     | 0.000227 | 0.000141   |  0.000031 |  0.000023 |		
|  256	     | 0.000203 | 0.000148   |  0.000027 |  0.000023 |		
|  512	     | 0.000199 | 0.000133   |  0.000027 |  0.000021 |		
|  1024	     | 0.000190 | 0.000135   |  0.000026 |  0.000020 |		
|  2048	     | 0.000188 | 0.000130   |  0.000026 |  0.000020 |		
|  4096      | 0.000189 | 0.000133   |  0.000026 |  0.000020 |		
|  8192	     | 0.000200 | 0.000135   |  0.000026 |  0.000020 |		
|  16384     | 0.000211 | 0.000125   |  0.000025 |  0.000018 |		
|  32768     | 0.000211 | 0.000121   |  0.000025 |  0.000018 |		
|  65536     | 0.000198 | 0.000110   |  0.000021 |  0.000015 |		
|  131072    | 0.000170 | 0.000097   |  0.000018 |  0.000013 |		
|  262144    | 0.000144 | 0.000078   |  0.000015 |  0.000011 |		
|  524288    | 0.000112 | 0.000062   |  0.000012 |  0.000009 |		
|  1048576   | 0.000101 | 0.000049   |  0.000012 |  0.000009 |		

#### Fixed Length String Keys
| Input Size | HT ADD() | UM ADD()   | HT FIND() | UM FIND() |
| ---------- | -------- | ---------- | --------- | --------- |
| 2 	     |0.002000 	|  0.001500  | 0.000500  |0.000000   | 
| 4 	     |0.000500 	|  0.000000  | 0.000250  |0.000250   |
| 8 	     |0.001250 	|  0.000125  | 0.000125  |0.000000   |
| 16 	     |0.000250 	|  0.000125  | 0.000063  |0.000063   |
| 32 	     |0.000219 	|  0.000156  | 0.000031  |0.000031   |
| 64 	     |0.000219 	|  0.000141  | 0.000047  |0.000031   |
| 128 	     |0.000234 	|  0.000117  | 0.000055	 |0.000031   |
| 256 	     |0.000234 	|  0.000113  | 0.000047	 |0.000031   |
| 512 	     |0.000223 	|  0.000109  | 0.000047  |0.000031   |
| 1024 	     |0.000242 	|  0.000111  | 0.000049  |0.000031   |
| 2048 	     |0.000220 	|  0.000114  | 0.000050  |0.000033   |
| 4096 	     |0.000234 	|  0.000125  | 0.000055  |0.000034   |
| 8192 	     |0.000231 	|  0.000127  | 0.000056  |0.000036   |

#### Variable Length String Keys
| Input Size | HT ADD() | UM ADD()   | HT FIND() | UM FIND() |
| ---------- | -------- | ---------- | --------- | --------- |
| 20	     | 0.000322 |0.000120    |0.000057 	 |0.000038   | 
| 40 	     | 0.000401 |0.000151    |0.000139 	 |0.000050   |
| 80 	     | 0.000553 |0.000128    |0.000167 	 |0.000123   |
| 160 	     | 0.000962 |0.000157    |0.000392 	 |0.000136   |
| 320 	     | 0.001515 |0.000207    |0.000508 	 |0.000164   |
| 640 	     | 0.002710 |0.000281    |0.000921 	 |0.000258   |
| 1280 	     | 0.005008 |0.000539    |0.001832 	 |0.000410   |
| 2560 	     | 0.010335 |0.001124    |0.003556 	 |0.000790   |
| 5120 	     | 0.020706 |0.001954    |0.007135 	 |0.001278   |
| 10240      | 0.041167 |0.003666    |0.014515 	 |0.002303   |
| 20480      | 0.083014 |0.007015    |0.028690 	 |0.004283   |
| 40960      | 0.169421 |0.013148    |0.058229 	 |0.008421   |
