CXX=g++
CXXFLAGS=-std=c++11 -Wall -fPIC
LDFLAGS=-shared

all: demo-application

parallel_scheduler.so: parallel_scheduler.o
	$(CXX) $(LDFLAGS) -o $@ $^

parallel_scheduler.o: parallel_scheduler.cpp
	$(CXX) $(CXXFLAGS) -c $<

demo-application: main.o parallel_scheduler.so
	$(CXX) -o $@ main.o -L. -lparallel_scheduler -pthread

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *.so demo-application

.PHONY: all clean
