CXX=g++
CXXFLAGS=-std=c++17 -Wall -O2
LDFLAGS=-lcpr -lcurl -lssl -lcrypto -lz
TARGET=login_nosis
SRC=main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
