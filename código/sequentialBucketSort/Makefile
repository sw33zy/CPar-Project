# source files.
SRC = bucketSortSeq.c sorting.c main.c

OBJ = $(SRC:.cpp=.o)

OUT = sequentialBucketSort

# include directories
INCLUDES = -I.
 
# C compiler flags 
CCFLAGS = -O2 -Wall -I/share/apps/papi/5.4.1/include -funroll-loops -fopenmp

# compiler
CCC = gcc 

# libraries
LIBS = -L/share/apps/papi/5.4.1/lib -lm -lpapi

.SUFFIXES: .cpp .c 


default: $(OUT)

.cpp.o:
	$(CCC) $(CCFLAGS) $(INCLUDES)  -c $< -o $@

.c.o:
	$(CCC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

$(OUT): $(OBJ)
	$(CCC) -o $(OUT) $(CCFLAGS) $(OBJ) $(LIBS) 

depend:  dep
#
#dep:
#	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f *.o .a *~ Makefile.bak $(OUT)
