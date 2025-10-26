CXX = mpic++
CXXFLAGS = -O2 -std=c++11

SRC_DIR = ../src
SRC = $(SRC_DIR)/mpi_vector.cpp 
EXE = mpi_vector

OSC: $(SRC)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(SRC)

clean:
	rm -f $(EXE) *.o