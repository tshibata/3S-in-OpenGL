
CFLAGS = -I. `pkg-config --cflags --libs gtk+-3.0 epoxy`

run: sample.exe
	./sample.exe

sample.exe: platform.c ../sample.cxx ../geometry.cxx
	g++ -o sample.exe platform.c ../sample.cxx ../geometry.cxx ${CFLAGS}

clean:
	rm -f sample.exe

