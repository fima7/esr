COMPILE = g++ -pipe -O2 -std=c++11 -c
LINK = g++ 
CL = g++ -pipe -O2 -std=c++11
INCLUDE = ./ 

all: tiny_test linkedlist_test cities correctness_test performance_test

linkedlist_test: linkedlist_test.cpp esr/linkedlist.hpp
	$(CL) -I$(INCLUDE) linkedlist_test.cpp -o linkedlist_test 

tiny_test: tiny_test.cpp esr/hashtable.hpp esr/hasher.hpp esr/linkedlist.hpp
	$(CL) -I$(INCLUDE) tiny_test.cpp -o tiny_test

correctness_test: correctness_test.cpp esr/hashtest.hpp esr/hashtable.hpp esr/hasher.hpp esr/linkedlist.hpp
	$(CL) -I$(INCLUDE) correctness_test.cpp -o correctness_test 

performance_test: performance_test.cpp esr/hashtest.hpp esr/hashtable.hpp esr/hasher.hpp esr/linkedlist.hpp
	$(CL) -I$(INCLUDE) performance_test.cpp -o performance_test 

cities: cities.cpp esr/hashtable.hpp esr/hasher.hpp esr/linkedlist.hpp
	$(CL) -I$(INCLUDE) cities.cpp -o cities 


doc: cities.cpp performance_test.cpp esr/hashtest.hpp esr/hashtable.hpp esr/hasher.hpp esr/linkedlist.hpp
	doxygen ./Doxyfile

clean:
	rm -f cities correctness_test linkedlist_test performance_test tiny_test *.o 
