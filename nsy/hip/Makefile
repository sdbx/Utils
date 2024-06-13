HIP_PATH ?= $(wildcard /opt/rocm)
CXX = $(HIP_PATH)/bin/hipcc
CXXFLAGS = -O -Wall -W -pedantic

TARGET = nsy_hip

ifeq ($(OS), Windows_NT)
	TARGET = nsy_hip.exe
endif

SOURCES = $(wildcard ./*.cu)
OBJECTS = $(patsubst %.cu, %.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cu
	$(CXX) $(CXXFLAGS) -c -o $@ $<
