CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = library_system
SRC_DIR = src
INC_DIR = include

SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/Book.cpp \
       $(SRC_DIR)/Member.cpp \
       $(SRC_DIR)/Transaction.cpp \
       $(SRC_DIR)/Library.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
