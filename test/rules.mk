TOP_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
INC_DIR := $(TOP_DIR)/../src
LINKAGE := -L$(TOP_DIR)/../lib -lstepper

CXX 		:= g++
CXXFLAGS 	:= -std=c++17

SRC 	:= $(wildcard *.cpp)
TARGET 	:= $(patsubst %.cpp,%,$(SRC))

$(TARGET) : $(SRC)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) $< $(LINKAGE) -o $@

.PHONY : clean
clean :
	rm -f $(TARGET)
