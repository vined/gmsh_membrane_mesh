CXX=g++
CXXFLAGS=-std=c++11

objects = main.o

mesh: $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -o mesh -Llib -lgmsh

#main.o: gmsh.h
#intervals.o: intervals.h

clean:
	-rm mesh *.o
