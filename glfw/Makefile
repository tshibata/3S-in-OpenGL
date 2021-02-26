
CXX = clang++
CXXFLAGS = -I.
LDFLAGS = -lpng -lGL -lGLEW -lglfw

run: sample.exe ../earth.u-c.bin ../solid.u-c.bin ../lucid.u-c.bin
	./sample.exe

../%.u-c.bin: ../%.u-c.txt
	ruby ../u-ctxt2bin.rb $<

sample.exe: platform.o sample.o geometry.o common.o files.o ShadowRenderer.o SolidRenderer.o LucidRenderer.o Solid2DRenderer.o
	${CXX} -o $@ $+ ${CXXFLAGS} ${LDFLAGS}

platform.o: platform.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

%.o : ../%.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

clean:
	rm -f sample.exe *.o ../*.u-c.bin

