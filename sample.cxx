#include <utility>
#include <stdio.h>
#include <math.h>
#include <platform.h>
#include "ref.h"
#include "Percipi.h"
#include "common.h"
#include "geometry.h"
#include "renderer.h"
#include "Plane.h"
#include "Mecell.h"

#define COUNTER_CAPACITY 6

const int screenWidth = 1024;
const int screenHeight = 512;

static Texture shadowMap(1024, 512);
static Texture backgroundTexture("../Landscape.png");
static Texture floorTexture("../Floor.png");
static Texture cuboid243Texture("../Cuboid243.png");
static Texture cuboid465Texture("../Cuboid465.png");
static Texture cuboid8E9Texture("../Cuboid8E9.png");
static Texture solidTexture("../SolidStar.png");
static Texture lucidTexture("../LucidStar.png");
static Texture digitTexture("../num8x8.png");

static SurficialFigure backgroundFigure = SurficialFigure(& backgroundTexture, 512, 64, 512, 64, 512, 64);
static SpacialFigure cuboid243Figure(& cuboid243Texture, "../Cuboid243.u-c.bin");
static SpacialFigure cuboid465Figure(& cuboid465Texture, "../Cuboid465.u-c.bin");
static SpacialFigure cuboid8E9Figure(& cuboid8E9Texture, "../Cuboid8E9.u-c.bin");
static SpacialFigure solidFigure(& solidTexture, "../Star.u-c.bin");
static SpacialFigure lucidFigure(& lucidTexture, "../Star.u-c.bin");
static SpacialFigure earthFigure(& floorTexture, "../Floor.u-c.bin");
static SurficialFigure numFonts[] = {
	SurficialFigure(& digitTexture, 0 * 8, 8, 0, 8, 8, 0),
	SurficialFigure(& digitTexture, 1 * 8, 8, 0, 8, 8, 0),
	SurficialFigure(& digitTexture, 2 * 8, 8, 0, 8, 8, 0),
	SurficialFigure(& digitTexture, 3 * 8, 8, 0, 8, 8, 0),
	SurficialFigure(& digitTexture, 4 * 8, 8, 0, 8, 8, 0),
	SurficialFigure(& digitTexture, 5 * 8, 8, 0, 8, 8, 0),
	SurficialFigure(& digitTexture, 6 * 8, 8, 0, 8, 8, 0),
	SurficialFigure(& digitTexture, 7 * 8, 8, 0, 8, 8, 0),
	SurficialFigure(& digitTexture, 8 * 8, 8, 0, 8, 8, 0),
	SurficialFigure(& digitTexture, 9 * 8, 8, 0, 8, 8, 0),
};

class Background : public FinitePresence<Expand<Move<Stop>>>
{
public:
	Background();
	virtual Figure * getFigure();
};
Background::Background() : FinitePresence::FinitePresence(background)
{
}
Figure * Background::getFigure()
{
	return & backgroundFigure;
}

class Star : public FinitePresence<RotZ<Move<Stop>>>
{
public:
	Star(RenderingMode & mode);
};
Star::Star(RenderingMode & mode) : FinitePresence::FinitePresence(mode)
{
}

class SolidStar : public Star
{
public:
	SolidStar();
	virtual Figure * getFigure();
};
SolidStar::SolidStar() : Star::Star(solid3D)
{
	label = 1;
}
Figure * SolidStar::getFigure()
{
	return & solidFigure;
}

class LucidStar : public Star
{
public:
	LucidStar();
	virtual Figure * getFigure();
};
LucidStar::LucidStar() : Star::Star(lucid3D)
{
}
Figure * LucidStar::getFigure()
{
	return & lucidFigure;
}

class Earth : public FinitePresence<Move<Stop>>
{
public:
	Earth();
	virtual Figure * getFigure();
};
Earth::Earth() : FinitePresence::FinitePresence(solid3D)
{
}
Figure * Earth::getFigure()
{
	return & earthFigure;
}

class Cuboid243 : public FinitePresence<RotZ<Move<Stop>>>
{
public:
	Cuboid243();
	virtual Figure * getFigure();
};
Cuboid243::Cuboid243() : FinitePresence::FinitePresence(solid3D)
{
}
Figure * Cuboid243::getFigure()
{
	return & cuboid243Figure;
}

class Cuboid465 : public FinitePresence<RotZ<Move<Stop>>>
{
public:
	Cuboid465();
	virtual Figure * getFigure();
};
Cuboid465::Cuboid465() : FinitePresence::FinitePresence(solid3D)
{
}
Figure * Cuboid465::getFigure()
{
	return & cuboid465Figure;
}

class Cuboid8E9 : public FinitePresence<RotZ<Move<Stop>>>
{
public:
	Cuboid8E9();
	virtual Figure * getFigure();
};
Cuboid8E9::Cuboid8E9() : FinitePresence::FinitePresence(solid3D)
{
}
Figure * Cuboid8E9::getFigure()
{
	return & cuboid8E9Figure;
}

class Digit : public FinitePresence<Expand<Move<Stop>>>
{
public:
	int i = 0;
	Digit() : FinitePresence::FinitePresence(solid2D) {}
	virtual Figure * getFigure()
	{
		return & numFonts[i];
	}
};

static bool pressed = false;
static float prevX;
static float prevY;

static PerspectiveProjection<RotY<RotX<Move<Stop>>>> framing(100, 50, 0.5, 50);

class Props1
{
friend Res<Props1>;
private:
	static Props1 * singleton;
	int refc = 0;
public:
	Props1();
	~Props1();
	Earth earth;
	Cuboid8E9 cuboid1;
	Cuboid465 cuboid2;
	Cuboid8E9 cuboid3;
	Cuboid243 cuboid4;
	Cuboid8E9 cuboid5;
	Cuboid465 cuboid6;
	Cuboid465 cuboid7;
	Cuboid8E9 cuboid8;
	Cuboid465 cuboid9;
	Cuboid243 cuboid10;
	Cuboid465 cuboid11;
	Cuboid465 cuboid12;
	Cuboid243 cuboid13;
	Cuboid465 cuboid14;
	Cuboid8E9 cuboid15;
	Star * star[4];
	Digit digit[COUNTER_CAPACITY];
};
Props1 * Props1::singleton = nullptr;
Props1::Props1()
{
	cuboid1.direction->angle = 2 * M_PI / 2;
	cuboid1.direction->next->dx = -9;
	cuboid1.direction->next->dy = 22;
	cuboid2.direction->angle = M_PI / 2;
	cuboid2.direction->next->dx = -2;
	cuboid2.direction->next->dy = 17;
	cuboid3.direction->angle = M_PI / 2;
	cuboid3.direction->next->dx = 8;
	cuboid3.direction->next->dy = 19;
	cuboid4.direction->angle = 0 * M_PI / 2;
	cuboid4.direction->next->dx = 16;
	cuboid4.direction->next->dy = 13;
	cuboid5.direction->angle = 0 * M_PI / 2;
	cuboid5.direction->next->dx = 19;
	cuboid5.direction->next->dy = 4;
	cuboid6.direction->angle = 0 * M_PI / 2;
	cuboid6.direction->next->dx = 17;
	cuboid6.direction->next->dy = -6;
	cuboid7.direction->angle = 2 * M_PI / 2;
	cuboid7.direction->next->dx = 17;
	cuboid7.direction->next->dy = -12;
	cuboid8.direction->angle = M_PI / 2;
	cuboid8.direction->next->dx = 9;
	cuboid8.direction->next->dy = -19;
	cuboid9.direction->angle = 0 * M_PI / 2;
	cuboid9.direction->next->dx = 0;
	cuboid9.direction->next->dy = -18;
	cuboid10.direction->angle = M_PI / 2;
	cuboid10.direction->next->dx = -4;
	cuboid10.direction->next->dy = -16;
	cuboid11.direction->angle = M_PI / 2;
	cuboid11.direction->next->dx = -9;
	cuboid11.direction->next->dy = -17;
	cuboid12.direction->angle = 2 * M_PI / 2;
	cuboid12.direction->next->dx = -14;
	cuboid12.direction->next->dy = -18;
	cuboid13.direction->angle = 0 * M_PI / 2;
	cuboid13.direction->next->dx = -16;
	cuboid13.direction->next->dy = -13;
	cuboid14.direction->angle = 0 * M_PI / 2;
	cuboid14.direction->next->dx = -17;
	cuboid14.direction->next->dy = -8;
	cuboid15.direction->angle = 3 * M_PI / 2;
	cuboid15.direction->next->dx = -22;
	cuboid15.direction->next->dy = -1;

	star[0] = new SolidStar();
	star[1] = new SolidStar();
	star[2] = new LucidStar();
	star[3] = new LucidStar();
}
Props1::~Props1()
{
	delete star[0];
	delete star[1];
	delete star[2];
	delete star[3];
}

class Props2
{
friend Res<Props2>;
private:
	static Props2 * singleton;
	int refc = 0;
public:
	Props2();
	~Props2();
	Earth earth1;
	Earth earth2;
	Earth earth3;
	Cuboid243 cuboid1;
	Cuboid465 cuboid2;
	Cuboid465 cuboid3;
	Cuboid8E9 cuboid4;
	Cuboid465 cuboid5;
	Cuboid8E9 cuboid6;
	Cuboid243 cuboid7;
	Cuboid465 cuboid8;
	Cuboid8E9 cuboid9;
	Cuboid8E9 cuboid10;
};
Props2 * Props2::singleton = nullptr;
Props2::Props2()
{
	earth1.direction->dx = -30;
	earth1.direction->dy = 0;
	earth2.direction->dx = 0;
	earth2.direction->dy = 30;
	earth3.direction->dx = -30;
	earth3.direction->dy = 30;
	cuboid1.direction->angle = 1 * M_PI / 2;
	cuboid1.direction->next->dx = -31;
	cuboid1.direction->next->dy = 2;
	cuboid2.direction->angle = 0 * M_PI / 2;
	cuboid2.direction->next->dx = -35;
	cuboid2.direction->next->dy = 5;
	cuboid3.direction->angle = 1 * M_PI / 2;
	cuboid3.direction->next->dx = -36;
	cuboid3.direction->next->dy = 10;
	cuboid4.direction->angle = 2 * M_PI / 2;
	cuboid4.direction->next->dx = -37;
	cuboid4.direction->next->dy = 19;
	cuboid5.direction->angle = 2 * M_PI / 2;
	cuboid5.direction->next->dx = -35;
	cuboid5.direction->next->dy = 29;
	cuboid6.direction->angle = 0 * M_PI / 2;
	cuboid6.direction->next->dx = -37;
	cuboid6.direction->next->dy = 39;
	cuboid7.direction->angle = 1 * M_PI / 2;
	cuboid7.direction->next->dx = -31;
	cuboid7.direction->next->dy = 38;
	cuboid8.direction->angle = 1 * M_PI / 2;
	cuboid8.direction->next->dx = -26;
	cuboid8.direction->next->dy = 39;
	cuboid9.direction->angle = 1 * M_PI / 2;
	cuboid9.direction->next->dx = -16;
	cuboid9.direction->next->dy = 41;
	cuboid10.direction->angle = 2 * M_PI / 2;
	cuboid10.direction->next->dx = -9;
	cuboid10.direction->next->dy = 36;
}
Props2::~Props2()
{
}

class MecellScene : public Scene
{
public:
	Mecell * mecell;
	MecellScene(Mecell * mecell);
};
MecellScene::MecellScene(Mecell * mecell) : mecell(mecell)
{
}

class DemoScene : public MecellScene
{
private:
	Background sky1;
	Background sky2;
	Res<Props1> props1;
	Res<Props2> props2;
	float angle1 = 0.0;
	float angle2 = 0.0;
	double speed = 0.0;
	int count = 0;
protected:
	ParallelProjection<Move<RotX<RotZ<Stop>>>> lighting;
public:
	DemoScene(float x, float y, Mecell * mecell);
	void render();
	Scene * rearrange(unsigned int dt, float x, float y);
};
DemoScene::DemoScene(float x, float y, Mecell * mecell) : MecellScene::MecellScene(mecell), lighting(60, 30, 10, 40)
{
	for (int i = 0; i < 4; i++)
	{
		props1->star[i]->direction->angle = i - angle1;
		props1->star[i]->direction->next->dx = cosf(i - angle2) * 2;
		props1->star[i]->direction->next->dy = sinf(i - angle2) * 2;
		props1->star[i]->direction->next->dz = -1;
	}

	for (int i = 0; i < COUNTER_CAPACITY; i++)
	{
		props1->digit[i].direction->scale = 2;
		props1->digit[i].direction->next->dx = 0.9 + (i * 16) * - 2.0 / screenWidth;
		props1->digit[i].direction->next->dy = 0.75;
	}

	prevX = x;
	prevY = y;
}
void DemoScene::render()
{
	static BackgroundRenderer backgroundRenderer;
	static ShadowRenderer shadowRenderer(shadowMap);
	static SolidRenderer solidRenderer(shadowMap);
	static LucidRenderer lucidRenderer;
	static Solid2DRenderer solid2DRenderer;

	float sect = M_PI / (atan((framing.width / 2) / framing.far));
	float turn = framing.direction->angle / (M_PI * 2);
	sky1.direction->scale = screenWidth * sect / sky1.getFigure()->texture->width;
	sky1.direction->next->dx = (turn - floor(turn) - 1) * (2 * sect);
	sky2.direction->scale = screenWidth * sect / sky2.getFigure()->texture->width;
	sky2.direction->next->dx = (turn - floor(turn)) * (2 * sect);

	backgroundRenderer.process();

	Matrix4x4 lightingMatrix;
	lighting.getMatrix(lightingMatrix.elements);

	Matrix4x4 framingMatrix;
	framing.getMatrix(framingMatrix.elements);

	shadowRenderer.lightingMatrix = lightingMatrix.elements;
	shadowRenderer.process();

	solidRenderer.framingMatrix = framingMatrix.elements;
	solidRenderer.lightingMatrix = lightingMatrix.elements;
	solidRenderer.process();

	lucidRenderer.framingMatrix = framingMatrix.elements;
	lucidRenderer.process();

	solid2DRenderer.process();
}
Scene * DemoScene::rearrange(unsigned int dt, float x, float y)
{
	Scene * next = this;

	float fdt = dt * 0.000001; // us -> s
	if (0 <= x && x < screenWidth && 0 <= y && y < screenHeight && pixelLabel(x, y) != 0)
	{
		speed = 1000;
	}
	else
	{
		speed = speed - fdt * 1000;
		if (speed < 0)
		{
			speed = 0; // no rewind
		}
	}

	if (pressed)
	{
		framing.direction->angle += (x - prevX) * 0.001f;

		float dx = sinf(framing.direction->angle) * (y - prevY) * -0.01f;
		float dy = cosf(framing.direction->angle) * (y - prevY) * 0.02f;
		Mecell * dst = mecell->transit(framing.direction->next->next->dx, framing.direction->next->next->dy, dx, dy);
		framing.direction->next->next->dx += dx;
		framing.direction->next->next->dy += dy;
		if (dst != mecell)
		{
			next = (* dst->depction)(x, y, dst);
		}
	}

	angle1 += 0.029 * speed * fdt;
	angle2 += 0.005 * speed * fdt;
	for (int i = 0; i < 4; i++)
	{
		props1->star[i]->direction->angle = i - angle1;
		props1->star[i]->direction->next->dx = cosf(i - angle2) * 2;
		props1->star[i]->direction->next->dy = sinf(i - angle2) * 2;
		props1->star[i]->direction->next->dz = -1;
	}

	count++;
	for (int i = 0, j = count; i < COUNTER_CAPACITY; i++)
	{
		props1->digit[i].i = j % 10;
		j = j / 10;
	}

	prevX = x;
	prevY = y;

	return next;
}

class DemoScene1 : public DemoScene
{
public:
	DemoScene1(float x, float y, Mecell * mecell);
};
DemoScene1::DemoScene1(float x, float y, Mecell * mecell) : DemoScene::DemoScene(x, y, mecell)
{
	lighting.direction->dy = 5;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}

class DemoScene2 : public DemoScene
{
public:
	DemoScene2(float x, float y, Mecell * mecell);
};
DemoScene2::DemoScene2(float x, float y, Mecell * mecell) : DemoScene::DemoScene(x, y, mecell)
{
	lighting.direction->dx = -20;
	lighting.direction->dy = 2;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}

extern Mecell cell1;
extern Mecell cell2;

Mecell cell1 = {
(Plane []) {
	{1, 0, 0, -14},
	{-1, 0, 0, -14},
	{0, 1, 0, -14},
	{0, -1, 0, -14},
	{0, 0, 0, 0}
}, (Mecell * []) {
	& cell2,
	nullptr
}, & cue<DemoScene1> };
Mecell cell2 = { (Plane []) {
	{1, 0, 0, 14},
	{-1, 0, 0, -32},
	{0, 1, 0, -36},
	{0, -1, 0, 4},
	{0, 0, 0, 0}
}, (Mecell * []) {
	& cell1,
	nullptr
}, & cue<DemoScene2> };

Scene * arrange(float x, float y)
{
	framing.direction->next->next->dy = -10;
	framing.direction->next->next->dz = -1.5;
	framing.direction->next->angle = M_PI / 2;

	return new DemoScene1(x, y, & cell1);
}

void buttonPressed(int pos, float x, float y)
{
	pressed = true;
	prevX = x;
	prevY = y;
}

void buttonReleased(int pos, float x, float y)
{
	pressed = false;
}

void wheelMoved(float d, float x, float y)
{
	printf("wheelMoved %f\n", d);
}

