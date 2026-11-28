CXX = c++
CPPFLAGS ?= -I.
CXXFLAGS ?= -std=c++17 -O2 -g -Wall -Wextra -Wpedantic
LDFLAGS ?=
LDLIBS ?=

EXEC = ex3
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean distclean
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.cpp matrix.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC)
