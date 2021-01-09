
CFLAGS = -I. -lstdc++ -lstdc++fs -lpng `pkg-config --cflags --libs gtk+-3.0 epoxy`

run: sample.exe ../earth.u-a.txt ../solid.u-a.txt ../screen.u-a.txt
	./sample.exe

../earth.u-a.txt: ../earth.u-z.txt
	ruby ../norm.rb ../earth.u-z.txt ../earth.u-a.txt

../solid.u-a.txt: ../solid.u-z.txt
	ruby ../norm.rb ../solid.u-z.txt ../solid.u-a.txt

../screen.u-a.txt: ../screen.u-z.txt
	ruby ../norm.rb ../screen.u-z.txt ../screen.u-a.txt

sample.exe: platform.c ../sample.cxx ../geometry.cxx ../common.cxx
	g++ -o sample.exe platform.c ../sample.cxx ../geometry.cxx ../common.cxx ${CFLAGS}

clean:
	rm -f sample.exe

