ifndef CXX
CXX := nvcc
endif
CXXFLAGS := -O3 -Wall -Wno-pedantic
ifeq ($(CXX), nvcc)
CXXFLAGS := -Xcompiler "$(CXXFLAGS)"
endif

TARGET := nsy

SOURCES := $(wildcard ./*.cu)
OBJECTS := $(patsubst %.cu, %.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

ifeq ($(CXX), hipcc)
_SOURCES := $(SOURCES)
SOURCES := $(patsubst %.cu, %.cu.hip, $(SOURCES))

$(SOURCES): $(_SOURCES)
	hipify-clang $<
endif

$(OBJECTS): $(SOURCES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f nsy
	rm -f nsy.o
	rm -f nsy.cu.hip