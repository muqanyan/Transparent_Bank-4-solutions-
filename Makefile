CXX = g++
CXXFLAGS = -std=c++17 -pthread
DEPS = Bank.h
OBJ = main.o Bank.o Server.o Client.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

all: bank_local bank_server bank_client

bank_local: main.o Bank.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

bank_server: Server.o Bank.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

bank_client: Client.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f *.o bank_local bank_server bank_client
