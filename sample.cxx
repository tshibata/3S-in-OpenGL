#include <utility>
#include <stdio.h>
#include <math.h>
#include <platform.h>
#include "ref.h"
#include "common.h"
#include "geometry.h"
#include "renderer.h"

#define COUNTER_CAPACITY 6

const int screenWidth = 1024;
const int screenHeight = 512;

static Texture shadowMap(1024, 512);
static Texture floorTexture("../Floor.png");
static Texture cuboid243Texture("../Cuboid243.png");
static Texture cuboid465Texture("../Cuboid465.png");
static Texture cuboid8E9Texture("../Cuboid8E9.png");
static Texture solidTexture("../SolidStar.png");
static Texture lucidTexture("../LucidStar.png");
static Texture digitTexture("../num8x8.png");

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

class Earth : public FinitePresence<Stop>
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

class DemoScene : public Scene
{
private:
	PerspectiveProjection<RotY<RotX<Move<Stop>>>> framing;
	ParallelProjection<Move<RotX<RotZ<Stop>>>> lighting;
	Star * star[4];
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
	Cuboid465 cuboid16;
	Earth earth;
	float angle1 = 0.0;
	float angle2 = 0.0;
	double speed = 0.0;
	Digit digit[COUNTER_CAPACITY];
	int count = 0;
public:
	DemoScene(float x, float y);
	void render();
	Scene * rearrange(unsigned int dt, float x, float y);
};
DemoScene::DemoScene(float x, float y) : framing(50, 25, 0.5, 50), lighting(60, 30, 10, 40)
{
	cuboid1.direction->angle = 3 * M_PI / 2;
	cuboid1.direction->next->dx = -11;
	cuboid1.direction->next->dy = 19;
	cuboid2.direction->angle = M_PI / 2;
	cuboid2.direction->next->dx = -1;
	cuboid2.direction->next->dy = 17;
	cuboid3.direction->angle = M_PI / 2;
	cuboid3.direction->next->dx = 9;
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
	cuboid15.direction->angle = 2 * M_PI / 2;
	cuboid15.direction->next->dx = -19;
	cuboid15.direction->next->dy = 2;
	cuboid16.direction->angle = 0 * M_PI / 2;
	cuboid16.direction->next->dx = -17;
	cuboid16.direction->next->dy = 12;

	star[0] = new SolidStar();
	star[1] = new SolidStar();
	star[2] = new LucidStar();
	star[3] = new LucidStar();

	framing.direction->next->next->dy = -10;
	framing.direction->next->next->dz = -1.5;
	framing.direction->next->angle = 3.14159 / 2;

	lighting.direction->dy = 5;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = 3.14159 / 3;
	lighting.direction->next->next->angle = 3.14159 / 4;

	for (int i = 0; i < 4; i++)
	{
		star[i]->direction->angle = i - angle1;
		star[i]->direction->next->dx = cosf(i - angle2) * 2;
		star[i]->direction->next->dy = sinf(i - angle2) * 2;
		star[i]->direction->next->dz = -1;
	}

	for (int i = 0; i < COUNTER_CAPACITY; i++)
	{
		digit[i].direction->scale = 2;
		digit[i].direction->next->dx = 0.9 + (i * 16) * - 2.0 / screenWidth;
		digit[i].direction->next->dy = 0.75;
	}

	prevX = x;
	prevY = y;
}
void DemoScene::render()
{
	static ShadowRenderer shadowRenderer(shadowMap);
	static SolidRenderer solidRenderer(shadowMap);
	static LucidRenderer lucidRenderer;
	static Solid2DRenderer solid2DRenderer;

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
		framing.direction->next->next->dy += cosf(framing.direction->angle) * (y - prevY) * 0.02f;
		if (framing.direction->next->next->dy < -14)
		{
			framing.direction->next->next->dy = -14;
		}
		if (14 < framing.direction->next->next->dy)
		{
			framing.direction->next->next->dy = 14;
		}
		framing.direction->next->next->dx -= sinf(framing.direction->angle) * (y - prevY) * 0.01f;
		if (framing.direction->next->next->dx < -14)
		{
			framing.direction->next->next->dx = -14;
		}
		if (14 < framing.direction->next->next->dx)
		{
			framing.direction->next->next->dx = 14;
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

	count++;
	for (int i = 0, j = count; i < COUNTER_CAPACITY; i++)
	{
		digit[i].i = j % 10;
		j = j / 10;
	}

	prevX = x;
	prevY = y;

	return this;
}

Scene * arrange(float x, float y)
{
	return new DemoScene(x, y);
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

