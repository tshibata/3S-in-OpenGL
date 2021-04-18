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

static Texture shadowMap(512, 256);
static Texture floorTexture("../Floor.png");
static Texture solidTexture("../SolidStar.png");
static Texture lucidTexture("../LucidStar.png");
static Texture digitTexture("../num8x8.png");

static SpacialFigure solidFigure(& solidTexture, "../Star.u-c.bin");
static SpacialFigure lucidFigure(& lucidTexture, "../Star.u-c.bin");
static SpacialFigure earthFigure(& floorTexture, "../Floor.u-c.bin");
SurficialFigure numFonts[] = {
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
DemoScene::DemoScene(float x, float y) : framing(20, 10, 5, 15), lighting(30, 15, 5, 20)
{
	star[0] = new SolidStar();
	star[1] = new SolidStar();
	star[2] = new LucidStar();
	star[3] = new LucidStar();

	framing.direction->next->next->dy = -10;
	framing.direction->next->next->dz = -1.5;
	framing.direction->next->angle = 3.14159 / 2;

	lighting.direction->dz = -7;
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
		framing.direction->next->next->dy += cosf(framing.direction->angle) * (y - prevY) * 0.01f;
		framing.direction->next->next->dx -= sinf(framing.direction->angle) * (y - prevY) * 0.01f;
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

