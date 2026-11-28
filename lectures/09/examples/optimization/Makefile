CXX = c++
CPPFLAGS ?=
CXXFLAGS ?= -std=c++17 -O3 -Wall -Wextra -Wpedantic

EXEC = static loop_unrolling data_alignment

.PHONY: all clean distclean
all: $(EXEC)

$(EXEC): %: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC)
