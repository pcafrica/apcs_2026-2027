CXX = c++
CPPFLAGS ?=
CXXFLAGS ?= -std=c++17 -O0 -g -Wall -Wextra -Wpedantic

EXEC = assert1 assert2 debug_macro defensive pause print student1 student2 undefined_behaviour

.PHONY: all clean distclean
all: $(EXEC)

$(EXEC): %: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@
debug_macro: debug_macro.hpp

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC)
