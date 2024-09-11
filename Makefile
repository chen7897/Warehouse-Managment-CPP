CXX = c++
SRC_DIR = ./src/
BIN_DIR = ./bin/
SRC = $(wildcard $(SRC_DIR)*.cpp)
OBJS = $(patsubst $(SRC_DIR)%.cpp,$(BIN_DIR)%.o,$(SRC))
EXEC = $(BIN_DIR)warehouse.out
INCLUDE= -I./include/
LIBS = -lm
RELEASE = -O3 -DNDEBUG
DEBUG = -O0 -g
CXXFLAGS = -std=c++11 -Wall -Wpedantic -Wextra -Weffc++ $(INCLUDE)

all: debug

release: CXXFLAGS += $(RELEASE)
release: $(EXEC)

debug: CXXFLAGS += $(DEBUG)
debug: $(EXEC)

$(EXEC): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)
	@echo "Build done."

$(BIN_DIR)%.o: $(SRC_DIR)%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully."

clean:
	@rm -f $(OBJS) $(EXEC)
	@echo "Build cleaned."
