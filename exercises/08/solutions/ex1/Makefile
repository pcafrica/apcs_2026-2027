CXX = c++
CPPFLAGS ?= -I.
CXXFLAGS ?= -std=c++17 -O0 -g -Wall -Wextra -Wpedantic
LDFLAGS ?=
LDLIBS ?=

EXEC = ex1
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean distclean
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.cpp list.hpp node.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC)
