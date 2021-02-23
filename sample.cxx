#include <stdio.h>
#include <math.h>
#include <time.h>
#include "ref.h"
#include "common.h"
#include "geometry.h"

#define COUNTER_CAPACITY 6

const int screenWidth = 512;
const int screenHeight = 256;

static Texture flatTexture("../flat.png");
static Texture bumpTexture("../bump.png");
static Texture digitTexture("../num8x8.png");

static SpacialFigure solidFigure(& bumpTexture, "../solid.u-c.bin", SOLID_BLEND);
static SpacialFigure lucidFigure(& flatTexture, "../lucid.u-c.bin", LUCID_BLEND);
static SpacialFigure earthFigure(& flatTexture, "../earth.u-c.bin", SOLID_BLEND);
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

class Star : public SpacialBeing<RotY<Move<Stop>>>
{
public:
	Star();
};
Star::Star()
{
}

class SolidStar : public Star
{
public:
	SolidStar();
	virtual SpacialFigure * getFigure();
};
SolidStar::SolidStar()
{
}
SpacialFigure * SolidStar::getFigure()
{
	return & solidFigure;
}

class LucidStar : public Star
{
public:
	LucidStar();
	virtual SpacialFigure * getFigure();
};
LucidStar::LucidStar()
{
}
SpacialFigure * LucidStar::getFigure()
{
	return & lucidFigure;
}

class Earth : public SpacialBeing<Stop>
{
public:
	Earth();
	virtual SpacialFigure * getFigure();
};
Earth::Earth()
{
}
SpacialFigure * Earth::getFigure()
{
	return & earthFigure;
}

class Digit : public SurficialBeing<Expand<Move<Stop>>>
{
public:
	int i = 0;
	virtual SurficialFigure * getFigure()
	{
		return & numFonts[i];
	}
};

class DemoScene : public Scene
{
private:
	Move<RotX<RotY<Stop>>> * lightningDirection;
	Star * star[4];
	Earth * earth;
	float angle1 = 0.0;
	float angle2 = 0.0;
	float prevX = -1.0;
	float prevY = -1.0;
	double speed = 0.0;
	Digit digit[COUNTER_CAPACITY];
	int count = 0;
public:
	DemoScene(float x, float y);
	Scene * rearrange(unsigned int dt, float x, float y);
};
DemoScene::DemoScene(float x, float y)
{
	star[0] = new SolidStar();
	star[1] = new SolidStar();
	star[2] = new LucidStar();
	star[3] = new LucidStar();
	earth = new Earth();

	framing = new Projection(new Stop(), 1, 0.5, 3);

	lightningDirection = new Move<RotX<RotY<Stop>>>();
	lightningDirection->dx = - 2;
	lightningDirection->dy = 0;
	lightningDirection->dz = - 2;
	lightningDirection->next->angle = - 0.5;
	lightningDirection->next->next->angle = - 1;
	lighting = new Projection(lightningDirection, 1, 1, 3);

	for (int i = 0; i < 4; i++)
	{
		star[i]->direction->angle = i - angle1;
		star[i]->direction->next->dx = cosf(i - angle2) * 2;
		star[i]->direction->next->dy = 0;
		star[i]->direction->next->dz = 3 + sinf(i - angle2) * 2;
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
Scene * DemoScene::rearrange(unsigned int dt, float x, float y)
{
	float fdt = dt * 0.000001; // us -> s
	if (0 <= prevX && prevX < screenWidth && 0 <= x && x < screenWidth
	 && 0 <= prevY && prevY < screenHeight && 0 <= y && y < screenHeight
	 && speed < prevX - x)
	{ // when mouse pointer moved inside the area, faster than stars
		speed = (prevX - x) / fdt;
		if (1000 < speed)
		{
			speed = 1000; // speed limit
		}
	}
	else
	{
		speed = speed - fdt * 500;
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
		star[i]->direction->next->dy = 0;
		star[i]->direction->next->dz = 3 + sinf(i - angle2) * 2;
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
	printf("buttonPressed %d\n", pos);
}

void buttonReleased(int pos, float x, float y)
{
	printf("buttonReleased %d\n", pos);
}

void wheelMoved(float d, float x, float y)
{
	printf("wheelMoved %f\n", d);
}

