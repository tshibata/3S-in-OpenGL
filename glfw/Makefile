
CFLAGS = -I. -lstdc++ -lstdc++fs -lpng -lGL -lGLEW -lglfw

run: sample.exe
	./sample.exe

sample.exe: platform.c ../sample.cxx ../geometry.cxx
	g++ -o sample.exe platform.c ../sample.cxx ../geometry.cxx ${CFLAGS}

clean:
	rm -f sample.exe

