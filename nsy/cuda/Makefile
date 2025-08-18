ifndef CXX
CXX := nvcc
endif
CXXFLAGS := -O3 -Wall -Wno-pedantic
ifeq ($(CXX), nvcc)
CXXFLAGS := -Xcompiler "$(CXXFLAGS)"
endif
LDFLAGS :=

TARGET := nsy

SOURCES := $(wildcard ./*.cu)
OBJECTS := $(patsubst %.cu, %.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

ifneq (,$(filter $(CXX),clang clang++))
CXXFLAGS += -march=native -std=c++17
LDFLAGS += -ltbb

_SOURCES := $(SOURCES)
SOURCES := $(patsubst %.cu, %.cpp, $(SOURCES))

$(SOURCES): $(_SOURCES)
	hipify-clang --hip-kernel-execution-syntax -o $@ $<
endif

ifeq ($(CXX), hipcc)
_SOURCES := $(SOURCES)
SOURCES := $(patsubst %.cu, %.cu.hip, $(SOURCES))

$(SOURCES): $(_SOURCES)
	hipify-clang -o $@ $<
endif

$(OBJECTS): $(SOURCES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f nsy
	rm -f nsy.o
	rm -f nsy.cpp
	rm -f nsy.cu.hip