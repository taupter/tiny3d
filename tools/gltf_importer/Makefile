CXXFLAGS += -O3 -std=c++20 -I./src/lib
OBJDIR = build
SRCDIR = src
INSTALLDIR = $(N64_INST)

OBJ = build/parser.o build/main.o build/lib/lodepng.o \
	build/parser/materialParser.o build/parser/boneParser.o build/parser/nodeParser.o \
	build/optimizer/meshOptimizer.o \
	build/optimizer/meshBVH.o \
	build/parser/animParser.o \
	build/converter/meshConverter.o \
	build/converter/animConverter.o \
	build/lib/meshopt/allocator.o \
	build/lib/meshopt/indexcodec.o \
	build/lib/meshopt/indexgenerator.o \
	build/lib/meshopt/simplifier.o \
	build/lib/meshopt/stripifier.o \
	build/lib/meshopt/spatialorder.o \
	build/lib/meshopt/vcacheanalyzer.o \
	build/lib/meshopt/vcacheoptimizer.o \
	build/lib/tristrip/connectivity_graph.o \
	build/lib/tristrip/policy.o \
	build/lib/tristrip/tri_stripper.o

all: gltf_to_t3d

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

gltf_to_t3d: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $ $(LINKFLAGS)

install:
	mkdir -p $(INSTALLDIR)/bin
	install -Cv -m 0755 gltf_to_t3d $(INSTALLDIR)/bin/

clean:
	rm -rf ./build ./gltf_to_t3d
