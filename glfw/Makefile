
CFLAGS = -I. -lstdc++ -lstdc++fs -lpng -lGL -lGLEW -lglfw

run: sample.exe ../earth.u-c.bin ../solid.u-c.bin ../screen.u-c.bin
	./sample.exe

../earth.u-c.bin: ../earth.u-c.txt
	ruby ../u-ctxt2bin.rb ../earth.u-c.txt

../solid.u-c.bin: ../solid.u-c.txt
	ruby ../u-ctxt2bin.rb ../solid.u-c.txt

../screen.u-c.bin: ../screen.u-c.txt
	ruby ../u-ctxt2bin.rb ../screen.u-c.txt

sample.exe: platform.c ../sample.cxx ../geometry.cxx ../common.cxx ../files.cxx
	g++ -o sample.exe platform.c ../sample.cxx ../geometry.cxx ../common.cxx ../files.cxx ${CFLAGS}

clean:
	rm -f sample.exe ../*.u-c.bin

