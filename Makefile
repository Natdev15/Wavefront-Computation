MPI    = mpicxx -std=c++20

OPTFLAGS  = -O3 -DNDEBUG -ffast-math -g
CXXFLAGS += -w

INCLUDES = -I. -Ifastflow
LIBS     = -pthread

SOURCES = $(wildcard *.cpp)
TARGETS = $(SOURCES:.cpp=)

.PHONY: all clean cleanall

# Pattern rule: compile each .cpp file into an executable using the MPI compiler.
%: %.cpp
	$(MPI) $(INCLUDES) $(CXXFLAGS) $(OPTFLAGS) -o $@ $< $(LIBS)

all: $(TARGETS)

clean:
	-rm -fr *.o *~ $(TARGETS)

cleanall: clean
	-rm -fr $(TARGETS)


