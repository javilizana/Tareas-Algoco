CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17
ALGO_DIR = algorithms

all: matrix_multiplication

matrix_multiplication: matrix_multiplication.cpp $(ALGO_DIR)/naive.cpp $(ALGO_DIR)/strassen.cpp
	$(CXX) $(CXXFLAGS) matrix_multiplication.cpp $(ALGO_DIR)/naive.cpp $(ALGO_DIR)/strassen.cpp -o matrix_multiplication

clean:
	rm -f matrix_multiplication