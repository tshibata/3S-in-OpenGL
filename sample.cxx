#include <utility>
#include <stdio.h>
#include <math.h>
#include <platform.h>
#include "sss/Basis.h"
#include "sss/Percipi.h"
#include "sss/common.h"
#include "sss/geometry.h"
#include "sss/renderer.h"
#include "sss/Scene.h"
#include "Plane.h"
#include "Hollow.h"
#include "BackgroundRenderer.h"
#include "ShadowRenderer.h"
#include "SolidRenderer.h"
#include "LucidRenderer.h"
#include "Solid2DRenderer.h"

#define COUNTER_CAPACITY 6

const int screenWidth = 1024;
const int screenHeight = 512;

extern RenderingMode background;
extern RenderingMode solid2D;
extern RenderingMode solid3D;
extern RenderingMode lucid3D;

static Texture shadowMap(1024, 512);
static Texture backgroundTexture("Landscape.png");
static Texture floorTexture("Floor.png");
static Texture cuboid243Texture("Cuboid243.png");
static Texture cuboid465Texture("Cuboid465.png");
static Texture cuboid8E9Texture("Cuboid8E9.png");
static Texture solidTexture("SolidStar.png");
static Texture lucidTexture("LucidStar.png");
static Texture digitTexture("num8x8.png");

static SurficialFigure backgroundFigure = SurficialFigure(& backgroundTexture, 512, 64, 512, 64, 512, 64);
static SpacialFigure cuboid243Figure(& cuboid243Texture, "Cuboid243.u-c.bin");
static SpacialFigure cuboid465Figure(& cuboid465Texture, "Cuboid465.u-c.bin");
static SpacialFigure cuboid8E9Figure(& cuboid8E9Texture, "Cuboid8E9.u-c.bin");
static SpacialFigure solidFigure(& solidTexture, "Star.u-c.bin");
static SpacialFigure lucidFigure(& lucidTexture, "Star.u-c.bin");
static SpacialFigure earthFigure(& floorTexture, "Floor.u-c.bin");
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
static float origY;
static float prevX;
static float prevY;

static PerspectiveProjection<RotX<RotZ<Move<Stop>>>> framing(100, 50, 0.5, 50);


class Props0
{
public:
	Props0();
	Cuboid8E9 cuboid1;
	Cuboid8E9 cuboid2;
};
Props0::Props0()
{
	cuboid1.direction->angle = 2 * M_PI / 2;
	cuboid1.direction->next->dx = -9;
	cuboid1.direction->next->dy = 22;
	cuboid2.direction->angle = 1 * M_PI / 2;
	cuboid2.direction->next->dx = 2;
	cuboid2.direction->next->dy = 25;
}

class Props1
{
public:
	Props1();
	~Props1();
	void rearrange(float dt);
	Earth earth;
	Cuboid465 cuboid1;
	Cuboid8E9 cuboid2;
	Cuboid243 cuboid3;
	Cuboid8E9 cuboid4;
	Cuboid465 cuboid5;
	Cuboid465 cuboid6;
	Cuboid8E9 cuboid7;
	Cuboid465 cuboid8;
	Cuboid243 cuboid9;
	Cuboid465 cuboid10;
	Cuboid465 cuboid11;
	Cuboid243 cuboid12;
	Cuboid465 cuboid13;
	Cuboid8E9 cuboid14;
	Star * star[4];
	float angle1 = 0.0;
	float angle2 = 0.0;
	double speed = 0.0;
};
Props1::Props1()
{
	cuboid1.direction->angle = M_PI / 2;
	cuboid1.direction->next->dx = -2;
	cuboid1.direction->next->dy = 17;
	cuboid2.direction->angle = M_PI / 2;
	cuboid2.direction->next->dx = 8;
	cuboid2.direction->next->dy = 19;
	cuboid3.direction->angle = 0 * M_PI / 2;
	cuboid3.direction->next->dx = 16;
	cuboid3.direction->next->dy = 13;
	cuboid4.direction->angle = 0 * M_PI / 2;
	cuboid4.direction->next->dx = 19;
	cuboid4.direction->next->dy = 4;
	cuboid5.direction->angle = 0 * M_PI / 2;
	cuboid5.direction->next->dx = 17;
	cuboid5.direction->next->dy = -6;
	cuboid6.direction->angle = 2 * M_PI / 2;
	cuboid6.direction->next->dx = 17;
	cuboid6.direction->next->dy = -12;
	cuboid7.direction->angle = M_PI / 2;
	cuboid7.direction->next->dx = 9;
	cuboid7.direction->next->dy = -19;
	cuboid8.direction->angle = 0 * M_PI / 2;
	cuboid8.direction->next->dx = 0;
	cuboid8.direction->next->dy = -18;
	cuboid9.direction->angle = M_PI / 2;
	cuboid9.direction->next->dx = -4;
	cuboid9.direction->next->dy = -16;
	cuboid10.direction->angle = M_PI / 2;
	cuboid10.direction->next->dx = -9;
	cuboid10.direction->next->dy = -17;
	cuboid11.direction->angle = 2 * M_PI / 2;
	cuboid11.direction->next->dx = -14;
	cuboid11.direction->next->dy = -18;
	cuboid12.direction->angle = 0 * M_PI / 2;
	cuboid12.direction->next->dx = -16;
	cuboid12.direction->next->dy = -13;
	cuboid13.direction->angle = 0 * M_PI / 2;
	cuboid13.direction->next->dx = -17;
	cuboid13.direction->next->dy = -8;
	cuboid14.direction->angle = 3 * M_PI / 2;
	cuboid14.direction->next->dx = -22;
	cuboid14.direction->next->dy = -1;

	star[0] = new SolidStar();
	star[1] = new SolidStar();
	star[2] = new LucidStar();
	star[3] = new LucidStar();

	for (int i = 0; i < 4; i++)
	{
		star[i]->direction->angle = i - angle1;
		star[i]->direction->next->dx = cosf(i - angle2) * 2;
		star[i]->direction->next->dy = sinf(i - angle2) * 2;
		star[i]->direction->next->dz = -1;
	}
}
Props1::~Props1()
{
	delete star[0];
	delete star[1];
	delete star[2];
	delete star[3];
}
void Props1::rearrange(float fdt)
{
	float x = controllers[0].x;
	float y = controllers[0].y;
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
	angle1 += 0.029 * speed * fdt;
	angle2 += 0.005 * speed * fdt;
	for (int i = 0; i < 4; i++)
	{
		star[i]->direction->angle = i - angle1;
		star[i]->direction->next->dx = cosf(i - angle2) * 2;
		star[i]->direction->next->dy = sinf(i - angle2) * 2;
		star[i]->direction->next->dz = -1;
	}
}

class Props2
{
public:
	Props2();
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
};
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
}

class Props3
{
public:
	Props3();
	Cuboid465 cuboid1;
	Cuboid465 cuboid2;
	Cuboid465 cuboid3;
	Cuboid8E9 cuboid4;
};
Props3::Props3()
{
	cuboid1.direction->angle = 1 * M_PI / 2;
	cuboid1.direction->next->dx = -6;
	cuboid1.direction->next->dy = 39;

	cuboid2.direction->angle = 2 * M_PI / 2;
	cuboid2.direction->next->dx = -1;
	cuboid2.direction->next->dy = 40;

	cuboid3.direction->angle = 3 * M_PI / 2;
	cuboid3.direction->next->dx = 4;
	cuboid3.direction->next->dy = 39;

	cuboid4.direction->angle = 2 * M_PI / 2;
	cuboid4.direction->next->dx = 11;
	cuboid4.direction->next->dy = 36;
}

class DemoScene : public Scene
{
private:
	Background sky1;
	Background sky2;
	Digit digit[COUNTER_CAPACITY];
	int count = 0;
protected:
	ParallelProjection<Move<RotX<RotZ<Stop>>>> lighting;
public:
	DemoScene(Hollow * hollow);
	void render();
	Hollow * rearrange(unsigned int dt);
};
DemoScene::DemoScene(Hollow * hollow) : Scene::Scene(hollow), lighting(60, 30, 10, 40)
{
	for (int i = 0; i < COUNTER_CAPACITY; i++)
	{
		digit[i].direction->scale = 2;
		digit[i].direction->next->dx = 0.9 + (i * 16) * - 2.0 / screenWidth;
		digit[i].direction->next->dy = 0.75;
	}

	prevX = controllers[0].x;
	prevY = controllers[0].y;
}
void DemoScene::render()
{
	static BackgroundRenderer backgroundRenderer;
	static ShadowRenderer shadowRenderer(shadowMap);
	static SolidRenderer solidRenderer(shadowMap);
	static LucidRenderer lucidRenderer;
	static Solid2DRenderer solid2DRenderer;

	float sect = M_PI / (atan((framing.width / 2) / framing.far));
	float turn = - framing.direction->next->angle / (M_PI * 2);
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
Hollow * DemoScene::rearrange(unsigned int dt)
{
	float x = controllers[0].x;
	float y = controllers[0].y;
	Hollow * next = hollow;

	float fdt = dt * 0.000001; // us -> s

	if (x < 0 || screenWidth <= x || y < screenHeight / 2 || screenHeight <= y)
	{
		pressed = false;
	}

	if (pressed)
	{
		framing.direction->next->angle += (x - prevX) * -0.001f;

		float a = fdt * fminf(y - origY, screenHeight / 4) / (screenHeight / 4);
		float dx = sinf(framing.direction->next->angle) * a * 10;
		float dy = cosf(framing.direction->next->angle) * a * 10;
		next = transit(hollow, framing.direction->next->next->dx, framing.direction->next->next->dy, dx, dy, true);
		framing.direction->next->next->dx += dx;
		framing.direction->next->next->dy += dy;
	}

	count++;
	for (int i = 0, j = count; i < COUNTER_CAPACITY; i++)
	{
		digit[i].i = j % 10;
		j = j / 10;
	}

	prevX = x;
	prevY = y;

	return next;
}

class DemoScene1 : public DemoScene
{
private:
	Percipi<Props0> props0;
	Percipi<Props1> props1;
	Percipi<Props2> props2;
public:
	DemoScene1(Hollow * hollow);
	Hollow * rearrange(unsigned int dt);
};
DemoScene1::DemoScene1(Hollow * hollow) : DemoScene::DemoScene(hollow)
{
	lighting.direction->dy = 5;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}
Hollow * DemoScene1::rearrange(unsigned int dt)
{
	Hollow * next = DemoScene::rearrange(dt);
	float fdt = dt * 0.000001; // us -> s
	props1->rearrange(fdt);
	return next;
}

class DemoScene2 : public DemoScene
{
private:
	Percipi<Props0> props0;
	Percipi<Props1> props1;
	Percipi<Props2> props2;
	Percipi<Props3> props3;
public:
	DemoScene2(Hollow * hollow);
	Hollow * rearrange(unsigned int dt);
};
DemoScene2::DemoScene2(Hollow * hollow) : DemoScene::DemoScene(hollow)
{
	lighting.direction->dx = -20;
	lighting.direction->dy = 2;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}
Hollow * DemoScene2::rearrange(unsigned int dt)
{
	Hollow * next = DemoScene::rearrange(dt);
	float fdt = dt * 0.000001; // us -> s
	props1->rearrange(fdt);
	return next;
}

class DemoScene3 : public DemoScene
{
private:
	Percipi<Props0> props0;
	Percipi<Props2> props2;
	Percipi<Props3> props3;
public:
	DemoScene3(Hollow * hollow);
	Hollow * rearrange(unsigned int dt);
};
DemoScene3::DemoScene3(Hollow * hollow) : DemoScene::DemoScene(hollow)
{
	lighting.direction->dx = -20;
	lighting.direction->dy = 2;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}
Hollow * DemoScene3::rearrange(unsigned int dt)
{
	Hollow * next = DemoScene::rearrange(dt);
	return next;
}

extern Hollow cell1;
extern Hollow cell2;
extern Hollow cell3;

Hollow cell1 = {
(Plane []) {
	{1, 0, 0, -14},
	{-1, 0, 0, -14},
	{0, 1, 0, -14},
	{0, -1, 0, -14},
	{0, 0, 0, 0}
}, (Hollow * []) {
	& cell2,
	nullptr
}, & cue<DemoScene1> };
Hollow cell2 = { (Plane []) {
	{1, 0, 0, 14},
	{-1, 0, 0, -32},
	{0, 1, 0, -36},
	{0, -1, 0, 4},
	{0, 0, 0, 0}
}, (Hollow * []) {
	& cell1,
	& cell3,
	nullptr
}, & cue<DemoScene2> };
Hollow cell3 = {
(Plane []) {
	{1, 0, 0, -6},
	{-1, 0, 0, -14},
	{0, 1, 0, -36},
	{0, -1, 0, 30},
	{0, 0, 0, 0}
}, (Hollow * []) {
	& cell2,
	nullptr
}, & cue<DemoScene3> };

Hollow * arrange()
{
	framing.direction->next->next->dy = -10;
	framing.direction->next->next->dz = -1.5;
	framing.direction->angle = M_PI / 2;

	return & cell1;
}

void buttonPressed(int pos)
{
	if (screenHeight / 2 < controllers[0].y)
	{
		pressed = true;
		origY = controllers[0].y;
	}
	prevX = controllers[0].x;
	prevY = controllers[0].y;
}

void buttonReleased(int pos)
{
	pressed = false;
}

void wheelMoved(float d)
{
	printf("wheelMoved %f\n", d);
}

