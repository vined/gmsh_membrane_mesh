CXX=g++
CXXFLAGS=-std=c++11

objects = main.o \
    parameters-utils.o

mesh: $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -o mesh -Llib -lgmsh

main.o: parameters-utils.h
parameters-utils.o: parameters-utils.h

clean:
	-rm mesh *.o
