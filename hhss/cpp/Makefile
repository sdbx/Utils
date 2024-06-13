CXX = g++
CXXFLAGS = -O -Wall -W -pedantic

TARGET = hhss

SOURCES = $(wildcard ./*.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
