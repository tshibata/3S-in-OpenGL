
CFLAGS = -I. -lGL -lGLEW -lglfw

run: sample.exe
	./sample.exe

sample.exe: platform.c ../sample.cxx ../geometry.cxx
	g++ -o sample.exe platform.c ../sample.cxx ../geometry.cxx ${CFLAGS}

# -std=c++11

clean:
	rm -f sample.exe

