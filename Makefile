CXX = g++
LD = ld

MACROSES =
CCFLAGS = -I/home/bikineev/workspace/urdl-0.1/include \
	  -I/usr/local/ssl/include \
	  -std=c++1y -g
LDFLAGS = -L/home/bikineev/workspace/boost-dev/build/lib -lboost_system \
	  -L/home/bikineev/workspace/urdl-0.1/lib -lurdl \
	  -L/usr/local/ssl/lib -lssl -lcrypto \
	  -ldl -lboost_thread -lpthread -ltinyxml2

WAREQUEST_HO = request.hpp
TEST_BIN = test
TEST_BIN_FILES = test.cc

all: $(TEST_BIN)

$(TEST_BIN): $(TEST_BIN_FILES) $(WAREQUEST_HO)
	$(CXX) -o $(TEST_BIN) $(TEST_BIN_FILES) $(LDFLAGS) $(CCFLAGS)
