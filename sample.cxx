#include <utility>
#include <cstdio>
#include <math.h>
#include <platform.h>
#include "sss/Basis.h"
#include "sss/Percipi.h"
#include "sss/common.h"
#include "sss/geometry.h"
#include "sss/renderer.h"
#include "Plane.h"
#include "Hollow.h"
#include "BackgroundRenderer.h"
#include "ShadowRenderer.h"
#include "SolidRenderer.h"
#include "LucidRenderer.h"
#include "Solid2DRenderer.h"

const int COUNTER_CAPACITY = 6;
const int MISSILE_CAPACITY = 5;

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

class Missile : public FinitePresence<RotZ<Move<Stop>>>
{
public:
	Hollow * hollow;
	Missile();
	virtual Figure * getFigure();
};
Missile::Missile() : FinitePresence::FinitePresence(lucid3D)
{
	visible = false;
}
Figure * Missile::getFigure()
{
	return & lucidFigure;
}

class Foe : public FinitePresence<RotZ<Move<Stop>>>
{
public:
	Hollow * hollow;
	Foe(Hollow * hollow);
	virtual Figure * getFigure();
};
Foe::Foe(Hollow * hollow) : FinitePresence::FinitePresence(solid3D), hollow(hollow)
{
}
Figure * Foe::getFigure()
{
	return & solidFigure;
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

static bool pressedHigh = false;
static bool pressedLow = false;
static float origY;
static float prevX;
static float prevY;

static PerspectiveProjection<RotX<RotZ<Move<Stop>>>> framing(100, 50, 0.5, 50);

static float slip;
static float yawing = 0.0;


extern Hollow cell1;
extern Hollow cell2;
extern Hollow cell3;


class Props0
{
public:
	Props0();
	Cuboid8E9 cuboid1;
	Cuboid8E9 cuboid2;
	Digit digit[COUNTER_CAPACITY];
	int count = 0;
	Missile missiles[MISSILE_CAPACITY];
	int missileIndex = 0;
	BackgroundRenderer backgroundRenderer;
	ShadowRenderer shadowRenderer;
	SolidRenderer solidRenderer;
	LucidRenderer lucidRenderer;
	Solid2DRenderer solid2DRenderer;
};
Props0::Props0() : shadowRenderer(shadowMap), solidRenderer(shadowMap)
{
	for (int i = 0; i < COUNTER_CAPACITY; i++)
	{
		digit[i].direction->scale = 2;
		digit[i].direction->next->dx = 0.9 + (i * 16) * - 2.0 / screenWidth;
		digit[i].direction->next->dy = 0.75;
	}

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
	Cuboid8E9 cuboid1;
	Cuboid243 cuboid2;
	Cuboid465 cuboid3;
	Cuboid465 cuboid4;
	Cuboid8E9 cuboid5;
	Cuboid465 cuboid6;
	Cuboid8E9 cuboid7;
	Cuboid243 cuboid8;
	Cuboid465 cuboid9;
	Cuboid8E9 cuboid10;
};
Props2::Props2()
{
	earth1.direction->dx = -30;
	earth1.direction->dy = 0;
	earth2.direction->dx = 0;
	earth2.direction->dy = 30;
	earth3.direction->dx = -30;
	earth3.direction->dy = 30;
	cuboid1.direction->angle = 3 * M_PI / 2;
	cuboid1.direction->next->dx = -22;
	cuboid1.direction->next->dy = -1;
	cuboid2.direction->angle = 1 * M_PI / 2;
	cuboid2.direction->next->dx = -31;
	cuboid2.direction->next->dy = 2;
	cuboid3.direction->angle = 0 * M_PI / 2;
	cuboid3.direction->next->dx = -35;
	cuboid3.direction->next->dy = 5;
	cuboid4.direction->angle = 1 * M_PI / 2;
	cuboid4.direction->next->dx = -36;
	cuboid4.direction->next->dy = 10;
	cuboid5.direction->angle = 2 * M_PI / 2;
	cuboid5.direction->next->dx = -37;
	cuboid5.direction->next->dy = 19;
	cuboid6.direction->angle = 2 * M_PI / 2;
	cuboid6.direction->next->dx = -35;
	cuboid6.direction->next->dy = 29;
	cuboid7.direction->angle = 0 * M_PI / 2;
	cuboid7.direction->next->dx = -37;
	cuboid7.direction->next->dy = 39;
	cuboid8.direction->angle = 1 * M_PI / 2;
	cuboid8.direction->next->dx = -31;
	cuboid8.direction->next->dy = 38;
	cuboid9.direction->angle = 1 * M_PI / 2;
	cuboid9.direction->next->dx = -26;
	cuboid9.direction->next->dy = 39;
	cuboid10.direction->angle = 1 * M_PI / 2;
	cuboid10.direction->next->dx = -16;
	cuboid10.direction->next->dy = 41;
}

class Props3
{
private:
	Percipi<Props0> props0;
public:
	Props3();
	void rearrange(float dt);
	Cuboid465 cuboid1;
	Cuboid465 cuboid2;
	Cuboid465 cuboid3;
	Cuboid8E9 cuboid4;
	Foe lazy;
};
Props3::Props3() : lazy(& cell3)
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

	lazy.direction->next->dx = 3;
	lazy.direction->next->dy = 33;
	lazy.direction->next->dz = -1;
}
void Props3::rearrange(float fdt)
{
	int x = controllers[0].x;
	int y = controllers[0].y;
	if (lazy.visible)
	{
		if (yawing == 0)
		{
			float dx = framing.direction->next->next->dx - lazy.direction->next->dx;
			float dy = framing.direction->next->next->dy - lazy.direction->next->dy;
			float r = sqrt(dx * dx + dy * dy);
			if (r < 1) // TBD: should have better collision detection?
			{
				slip = atan2(dx, dy);
				yawing = 1.0;
			}
			else
			{
				Hollow * hollow = transit(lazy.hollow, lazy.direction->next->dx, lazy.direction->next->dy, dx, dy, false);
				if (hollow != nullptr)
				{
					dx = (framing.direction->next->next->dx - lazy.direction->next->dx) * 0.5f / r;
					dy = (framing.direction->next->next->dy - lazy.direction->next->dy) * 0.5f / r;
					lazy.hollow = transit(lazy.hollow, lazy.direction->next->dx, lazy.direction->next->dy, dx, dy, false);
					lazy.direction->next->dx += dx;
					lazy.direction->next->dy += dy;
				}
			}
		}

		for (int i = 0; i < MISSILE_CAPACITY; i++)
		{
			if (props0->missiles[i].visible)
			{
				Missile * m = & props0->missiles[i];
				float dx = m->direction->next->dx - lazy.direction->next->dx;
				float dy = m->direction->next->dy - lazy.direction->next->dy;
				if (sqrt(dx * dx + dy * dy) < 1) // TBD: should have better collision detection?
				{
					m->visible = false;
					lazy.visible = false;
				}
			}
		}
	}
}

class DemoScene : public Scene
{
private:
	Percipi<Props0> props0;
	Background sky1;
	Background sky2;
protected:
	Hollow * hollow;
	ParallelProjection<Move<RotX<RotZ<Stop>>>> lighting;
public:
	DemoScene(Hollow * hollow);
	void render();
	Scene * rearrange(unsigned int dt);
};
DemoScene::DemoScene(Hollow * hollow) : hollow(hollow), lighting(60, 30, 10, 40)
{
	prevX = controllers[0].x;
	prevY = controllers[0].y;
}
void DemoScene::render()
{
	float sect = M_PI / (atan((framing.width / 2) / framing.far));
	float turn = - framing.direction->next->angle / (M_PI * 2);
	sky1.direction->scale = screenWidth * sect / sky1.getFigure()->texture->width;
	sky1.direction->next->dx = (turn - floor(turn) - 1) * (2 * sect);
	sky2.direction->scale = screenWidth * sect / sky2.getFigure()->texture->width;
	sky2.direction->next->dx = (turn - floor(turn)) * (2 * sect);

	props0->backgroundRenderer.process();

	Matrix4x4 lightingMatrix;
	lighting.getMatrix(lightingMatrix.elements);

	Matrix4x4 framingMatrix;
	framing.getMatrix(framingMatrix.elements);

	props0->shadowRenderer.lightingMatrix = lightingMatrix.elements;
	props0->shadowRenderer.process();

	props0->solidRenderer.framingMatrix = framingMatrix.elements;
	props0->solidRenderer.lightingMatrix = lightingMatrix.elements;
	props0->solidRenderer.process();

	props0->lucidRenderer.framingMatrix = framingMatrix.elements;
	props0->lucidRenderer.process();

	props0->solid2DRenderer.process();
}
Scene * DemoScene::rearrange(unsigned int dt)
{
	float x = controllers[0].x;
	float y = controllers[0].y;
	Hollow * next = hollow;

	float fdt = dt * 0.000001; // us -> s

	for (int i = 0; i < MISSILE_CAPACITY; i++)
	{
		if (props0->missiles[i].visible)
		{
			Missile * m = & props0->missiles[i];
			float dx = sinf(m->direction->angle) * fdt * 10;
			float dy = cosf(m->direction->angle) * fdt * 10;
			m->hollow = transit(m->hollow, m->direction->next->dx, m->direction->next->dy, dx, dy, false);
			m->direction->next->dx += dx;
			m->direction->next->dy += dy;
			if (m->hollow == nullptr)
			{
				m->visible = false;
			}
		}
	}

	if (yawing == 0)
	{
		if (pressedHigh && prevY < screenHeight / 2 && screenHeight / 2 <= y)
		{
			if (! props0->missiles[props0->missileIndex].visible)
			{
				float a = atan((framing.width / 2) / framing.far * (x - screenWidth / 2) / (screenWidth / 2));
				Missile * m = & props0->missiles[props0->missileIndex];
				m->visible = true;
				m->hollow = hollow;
				m->direction->angle = framing.direction->next->angle + a;
				m->direction->next->dx = framing.direction->next->next->dx;
				m->direction->next->dy = framing.direction->next->next->dy;
				m->direction->next->dz = -1.7;
				props0->missileIndex = (props0->missileIndex + 1) % MISSILE_CAPACITY;
			}
		}

		if (x < 0 || screenWidth <= x || y < screenHeight / 2 || screenHeight <= y)
		{
			pressedLow = false;
		}

		if (pressedLow
 && abs(x - prevX) < screenWidth / 4 /* work adound for Surface */
)
		{
			framing.direction->next->angle += (x - prevX) * -0.002f;

			float a = fdt * fminf(y - origY, screenHeight / 4) / (screenHeight / 4);
			float dx = sinf(framing.direction->next->angle) * a * 10;
			float dy = cosf(framing.direction->next->angle) * a * 10;
			// FIXME common process
			next = transit(hollow, framing.direction->next->next->dx, framing.direction->next->next->dy, dx, dy, true);
			framing.direction->next->next->dx += dx;
			framing.direction->next->next->dy += dy;
		}
	}
	else
	{
		float dx = cos(slip) * abs(yawing) * fdt * 10;
		float dy = sin(slip) * abs(yawing) * fdt * 10;
		// FIXME common process
		next = transit(hollow, framing.direction->next->next->dx, framing.direction->next->next->dy, dx, dy, true);
		framing.direction->next->next->dx += dx;
		framing.direction->next->next->dy += dy;

		framing.direction->next->angle += yawing * fdt * 10;
		if (yawing < 0)
		{
			yawing = fminf(0, yawing + fdt * 0.5);
		}
		else
		{
			yawing = fmaxf(0, yawing - fdt * 0.5);
		}
	}

	props0->count++;
	for (int i = 0, j = props0->count; i < COUNTER_CAPACITY; i++)
	{
		props0->digit[i].i = j % 10;
		j = j / 10;
	}

	prevX = x;
	prevY = y;

	return depict(next);
}

class DemoScene1 : public DemoScene
{
private:
	Percipi<Props1> props1;
	Percipi<Props2> props2;
public:
	DemoScene1(Hollow * hollow);
	Scene * rearrange(unsigned int dt);
};
DemoScene1::DemoScene1(Hollow * hollow) : DemoScene::DemoScene(hollow)
{
	lighting.direction->dy = 5;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}
Scene * DemoScene1::rearrange(unsigned int dt)
{
	Scene * next = DemoScene::rearrange(dt);
	float fdt = dt * 0.000001; // us -> s
	props1->rearrange(fdt);
	return next;
}

class DemoScene2 : public DemoScene
{
private:
	Percipi<Props1> props1;
	Percipi<Props2> props2;
	Percipi<Props3> props3;
public:
	DemoScene2(Hollow * hollow);
	Scene * rearrange(unsigned int dt);
};
DemoScene2::DemoScene2(Hollow * hollow) : DemoScene::DemoScene(hollow)
{
	lighting.direction->dx = -20;
	lighting.direction->dy = 2;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}
Scene * DemoScene2::rearrange(unsigned int dt)
{
	Scene * next = DemoScene::rearrange(dt);
	float fdt = dt * 0.000001; // us -> s
	props1->rearrange(fdt);
	props3->rearrange(fdt);
	return next;
}

class DemoScene3 : public DemoScene
{
private:
	Percipi<Props2> props2;
	Percipi<Props3> props3;
public:
	DemoScene3(Hollow * hollow);
	Scene * rearrange(unsigned int dt);
};
DemoScene3::DemoScene3(Hollow * hollow) : DemoScene::DemoScene(hollow)
{
	lighting.direction->dx = -20;
	lighting.direction->dy = 2;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}
Scene * DemoScene3::rearrange(unsigned int dt)
{
	Scene * next = DemoScene::rearrange(dt);
	float fdt = dt * 0.000001; // us -> s
	props3->rearrange(fdt);
	return next;
}

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

Scene * arrange()
{
	framing.direction->next->next->dy = -10;
	framing.direction->next->next->dz = -1.5;
	framing.direction->angle = M_PI / 2;

	return depict(& cell1);
}

void buttonPressed(int pos)
{
	if (controllers[0].y < screenHeight / 2)
	{
		pressedHigh = true;
	}
	if (screenHeight / 2 <= controllers[0].y)
	{
		pressedLow = true;
		origY = controllers[0].y;
	}
}

void buttonReleased(int pos)
{
	pressedHigh = false;
	pressedLow = false;
}

void wheelMoved(float d)
{
	std::printf("wheelMoved %f\n", d);
}

