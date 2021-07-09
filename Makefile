PROJNAME = sample
OBJS = sample.o navigation.o ShadowRenderer.o SolidRenderer.o LucidRenderer.o Solid2DRenderer.o BackgroundRenderer.o
DATA = Floor.u-c.bin Star.u-c.bin Cuboid243.u-c.bin Cuboid465.u-c.bin Cuboid8E9.u-c.bin \
Cuboid243.png Cuboid465.png Cuboid8E9.png Floor.png Landscape.png LucidStar.png num8x8.png Screenshot.png SolidStar.png

run: $(PROJNAME).exe $(DATA)
	./$(PROJNAME).exe

%.png: ../../%.png
	cp $< $@

%.u-c.bin: %.u-c.txt
	ruby ../u-ctxt2bin.rb $<

Floor.u-c.txt: ../../Floor.dae
	ruby ../dae2u-c.rb ../../Floor.dae Floor

Star.u-c.txt: ../../star.dae
	ruby ../dae2u-c.rb ../../star.dae Star

Cuboid243.u-c.txt: ../../Cuboid243.dae
	ruby ../dae2u-c.rb ../../Cuboid243.dae Cuboid243

Cuboid465.u-c.txt: ../../Cuboid465.dae
	ruby ../dae2u-c.rb ../../Cuboid465.dae Cuboid465

Cuboid8E9.u-c.txt: ../../Cuboid8E9.dae
	ruby ../dae2u-c.rb ../../Cuboid8E9.dae Cuboid8E9


