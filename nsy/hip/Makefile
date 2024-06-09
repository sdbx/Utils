HIP_PATH?= $(wildcard /opt/rocm)
HIPCC=$(HIP_PATH)/bin/hipcc

SOURCES = nsy_hip.hip
OBJECTS = $(SOURCES:.hip=.o)

EXECUTABLE=./nsy_hip

all: $(EXECUTABLE)

CXXFLAGS =-g
CXX=$(HIPCC)

$(EXECUTABLE): $(OBJECTS)
	$(HIPCC) $(OBJECTS) -o $@

$(OBJECTS): $(SOURCES)
	$(HIPCC) $(CXXFLAGS) -c $< -o $@

test: $(EXECUTABLE)
	$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
