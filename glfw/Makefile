
CXX = clang++
CXXFLAGS = -I.
LDFLAGS = -lpng -lGL -lGLEW -lglfw

run: sample.exe Floor.u-c.bin Star.u-c.bin Cuboid243.u-c.bin Cuboid465.u-c.bin Cuboid8E9.u-c.bin
	./sample.exe

%.u-c.bin: %.u-c.txt
	ruby ../u-ctxt2bin.rb $<

Floor.u-c.txt: ../Floor.dae
	ruby ../dae2u-c.rb ../Floor.dae Floor

Star.u-c.txt: ../star.dae
	ruby ../dae2u-c.rb ../star.dae Star

Cuboid243.u-c.txt: ../Cuboid243.dae
	ruby ../dae2u-c.rb ../Cuboid243.dae Cuboid243

Cuboid465.u-c.txt: ../Cuboid465.dae
	ruby ../dae2u-c.rb ../Cuboid465.dae Cuboid465

Cuboid8E9.u-c.txt: ../Cuboid8E9.dae
	ruby ../dae2u-c.rb ../Cuboid8E9.dae Cuboid8E9

sample.exe: platform.o sample.o geometry.o common.o files.o renderer.o ShadowRenderer.o SolidRenderer.o LucidRenderer.o Solid2DRenderer.o BackgroundRenderer.o Plane.o Scene.o
	${CXX} -o $@ $+ ${CXXFLAGS} ${LDFLAGS}

platform.o: platform.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

%.o : ../%.cxx
	${CXX} -c -o $@ $< ${CXXFLAGS}

clean:
	rm -f sample.exe *.o *.u-c.bin *.u-c.txt

