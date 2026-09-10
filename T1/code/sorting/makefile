CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17
ALGO_DIR = algorithms

all: sorting_sort sorting_mergesort sorting_quicksort sorting_patiencesort

sorting_sort: sorting.cpp $(ALGO_DIR)/sort.cpp
	$(CXX) $(CXXFLAGS) sorting.cpp $(ALGO_DIR)/sort.cpp -o sorting_sort

sorting_mergesort: sorting.cpp $(ALGO_DIR)/mergesort.cpp
	$(CXX) $(CXXFLAGS) sorting.cpp $(ALGO_DIR)/mergesort.cpp -o sorting_mergesort

sorting_quicksort: sorting.cpp $(ALGO_DIR)/quicksort.cpp
	$(CXX) $(CXXFLAGS) sorting.cpp $(ALGO_DIR)/quicksort.cpp -o sorting_quicksort

sorting_patiencesort: sorting.cpp $(ALGO_DIR)/patiencesort.cpp
	$(CXX) $(CXXFLAGS) sorting.cpp $(ALGO_DIR)/patiencesort.cpp -o sorting_patiencesort

clean:
	rm -f sorting_sort sorting_mergesort sorting_quicksort sorting_patiencesort
run_all: all
	./sorting_mergesort data/array_input/ MergeSort data/measurements/mediciones.txt
	./sorting_quicksort data/array_input/ QuickSort data/measurements/mediciones.txt
	./sorting_patiencesort data/array_input/ PatienceSort data/measurements/mediciones.txt
	./sorting_sort data/array_input/ std::sort data/measurements/mediciones.txt