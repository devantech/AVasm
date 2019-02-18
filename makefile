# Makefile for avalanche assembler
# By James Henderson, 2019
# Devantech Ltd
# www.robot-electronics.co.uk

vpath %.cpp src/utils
vpath %.cpp src/instruction
vpath %.cpp src/macro
vpath %.cpp src/data_types
vpath %.hpp src/includes
vpath %.cpp src

CXX := g++
LXX = g++

CXXFLAGS := -Os -std=c++11 -Isrc/includes -c
LXXFLAGS := -s -Os

# CXXFLAGS := -std=c++11 -Isrc/includes -c
# LXXFLAGS := -g

BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj

SRCS := $(notdir $(shell find -name '*.cpp'))
OBJS := $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

avalanche: builddir $(OBJS) $(SRCS) 
	$(LXX) $(LXXFLAGS) $(OBJS) -o $(BUILDDIR)/avasm

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: builddir
builddir:
	@mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	@rm -f -r build/obj/*.o
	@rm -f build/avalanche