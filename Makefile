CXX = g++
CXXFLAGS = -g -Wall -std=c++11

all: d_base rank mult ns_base
d_base: src/d_base.cpp
	$(CXX) $(CXXFLAGS) -o bin/d_base src/d_base.cpp

rank: src/rank.cpp
	$(CXX) $(CXXFLAGS) -o bin/rank src/filters/rank.cpp

mult: src/mult.cpp
	$(CXX) $(CXXFLAGS) -o bin/mult src/mult.cpp

ns_base: src/ns_base.cpp
	$(CXX) $(CXXFLAGS) -o bin/ns_base src/ns_base.cpp
