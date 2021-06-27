CXX = clang++
CXXFLAGS = -O3 -I.
LDFLAGS = -lpng -lGL -lGLEW -lglfw

include ../../Makefile

$(PROJNAME).exe: platform.o geometry.o common.o files.o $(OBJS)
	${CXX} -o $@ $+ ${CXXFLAGS} ${LDFLAGS}

platform.o: platform.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

geometry.o: ../geometry.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

common.o: ../common.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

files.o: ../files.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

%.o : ../../%.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

clean:
	rm -f $(PROJNAME).exe *.o *.u-c.bin *.u-c.txt *.png

