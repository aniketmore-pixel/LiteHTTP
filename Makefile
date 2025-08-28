CXX=g++
CXXFLAGS=-std=c++17 -pthread -O2
SRC=src/main.cpp src/http.cpp
OBJ=$(SRC:.cpp=.o)
OUT=server

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	mkdir -p www
	./$(OUT) 8080 www

clean:
	rm -f $(OUT) *.o
