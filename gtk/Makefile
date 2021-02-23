
CXX = clang++
CXXFLAGS = -I. `pkg-config --cflags gtk+-3.0 epoxy libpng`
LDFLAGS = `pkg-config --libs gtk+-3.0 epoxy libpng`

run: sample.exe ../earth.u-c.bin ../solid.u-c.bin ../lucid.u-c.bin
	./sample.exe

../%.u-c.bin: ../%.u-c.txt
	ruby ../u-ctxt2bin.rb $<

sample.exe: platform.o sample.o geometry.o common.o files.o
	${CXX} -o $@ $+ ${CXXFLAGS} ${LDFLAGS}

platform.o: platform.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

%.o : ../%.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

clean:
	rm -f sample.exe *.o ../*.u-c.bin

