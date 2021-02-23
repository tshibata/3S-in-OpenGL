
CFLAGS = -I. -lpng `pkg-config --cflags --libs gtk+-3.0 epoxy`

run: sample.exe ../earth.u-c.bin ../solid.u-c.bin ../lucid.u-c.bin
	./sample.exe

../earth.u-c.bin: ../earth.u-c.txt
	ruby ../u-ctxt2bin.rb ../earth.u-c.txt

../solid.u-c.bin: ../solid.u-c.txt
	ruby ../u-ctxt2bin.rb ../solid.u-c.txt

../lucid.u-c.bin: ../lucid.u-c.txt
	ruby ../u-ctxt2bin.rb ../lucid.u-c.txt

sample.exe: platform.c ../sample.cxx ../geometry.cxx ../common.cxx ../files.cxx
	g++ -o sample.exe platform.c ../sample.cxx ../geometry.cxx ../common.cxx ../files.cxx ${CFLAGS}

clean:
	rm -f sample.exe ../*.u-c.bin

