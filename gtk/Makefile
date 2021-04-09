
CXX = clang++
CXXFLAGS = -I. `pkg-config --cflags gtk+-3.0 epoxy libpng`
LDFLAGS = `pkg-config --libs gtk+-3.0 epoxy libpng`

run: sample.exe ../Floor.u-c.bin ../Star.u-c.bin
	./sample.exe

../%.u-c.bin: ../%.u-c.txt
	ruby ../u-ctxt2bin.rb $<

../Floor.u-c.txt: ../floor.dae
	ruby ../dae2u-c.rb ../floor.dae

../Star.u-c.txt: ../star.dae
	ruby ../dae2u-c.rb ../star.dae

sample.exe: platform.o sample.o geometry.o common.o files.o renderer.o ShadowRenderer.o SolidRenderer.o LucidRenderer.o Solid2DRenderer.o
	${CXX} -o $@ $+ ${CXXFLAGS} ${LDFLAGS}

platform.o: platform.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

%.o : ../%.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

clean:
	rm -f sample.exe *.o ../*.u-c.bin ../*.u-c.txt

