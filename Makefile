COMPILE = g++ -pipe -O2 -std=c++11 -c
LINK = g++ 
CL = g++ -pipe -O2 -std=c++11
INCLUDE = ./ 
TESTS = -DENABLE_INT_INT_TESTS \
	-DENABLE_INT_INT_BASIC_TESTS \
	-DENABLE_INT_INT_RANDOM_ACCESS_TEST # -DENABLE_INT_INT_HAVY_TEST 

all: hashtable cities

cities: cities.cpp esr/hashtable.hpp
	$(CL) -I$(INCLUDE) -DFIMA_LOCAL cities.cpp -o cities 

hashtable: hashtable_tests.cpp esr/hashtable.hpp
	$(CL) -I$(INCLUDE) $(TESTS) -DFIMA_LOCAL hashtable_tests.cpp -o hashtable 

submission: hashtable.cpp
	$(CL) hashtable.cpp -o hashtable

clean:
	rm -f cities hashtable *.o 
