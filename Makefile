CXX 			:= g++
CXX_STANDARD 	:= -std=c++17
CXX_FLAGS 		:= -fPIC -Wall -Wextra

TOP_DIR 		:= $(shell pwd)
LIB_DIR 		:= $(TOP_DIR)/lib
OBJ_DIR         := $(TOP_DIR)/obj
SRC_DIR         := $(TOP_DIR)/src
INC_DIR         := $(TOP_DIR)/src

LIB_SO 			:= libstepper.so
TARGET 			:= $(LIB_DIR)/$(LIB_SO)

HDRS = $(wildcard $(SRC_DIR)/*.h)
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(notdir $(SRCS:.cpp=.o)))

all : $(TARGET)

$(TARGET) : $(OBJS) | $(LIB_DIR)
	$(CXX) -shared $^ -o $@

$(LIB_DIR) : ; mkdir -p $@ 

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(HDRS) | $(OBJ_DIR)
	$(CXX) -c $(CXX_FLAGS) $(CXX_STANDARD) -I$(INC_DIR) $< -o $@

$(OBJ_DIR) : ; mkdir -p $@

.PHONY : clean
clean :
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
