#include <stdio.h>
#include <math.h>
#include <time.h>
#include "ref.h"
#include "common.h"
#include "geometry.h"

const int screenWidth = 512;
const int screenHeight = 256;

static Texture flatTexture("../flat.png");
static Texture bumpTexture("../bump.png");

static Figure solidFigure(& bumpTexture, "../solid.u-c.bin");
static Figure lucidFigure(& flatTexture, "../lucid.u-c.bin");
static Figure earthFigure(& flatTexture, "../earth.u-c.bin");

class Star : public DirectedBeing<RotY<Move<Stop>>>
{
public:
	Star();
};
Star::Star() : DirectedBeing<RotY<Move<Stop>>>::DirectedBeing()
{
}

class SolidStar : public Star
{
public:
	SolidStar();
	virtual BlendMode getBlendMode();
	virtual Figure * getFigure();
};
SolidStar::SolidStar()
{
}
BlendMode SolidStar::getBlendMode()
{
	return SOLID_BLEND;
}
Figure * SolidStar::getFigure()
{
	return & solidFigure;
}

class LucidStar : public Star
{
public:
	LucidStar();
	virtual BlendMode getBlendMode();
	virtual Figure * getFigure();
};
LucidStar::LucidStar()
{
}
BlendMode LucidStar::getBlendMode()
{
	return LUCID_BLEND;
}
Figure * LucidStar::getFigure()
{
	return & lucidFigure;
}

class Earth : public DirectedBeing<Stop>
{
public:
	Earth();
	virtual BlendMode getBlendMode();
	virtual Figure * getFigure();
};
Earth::Earth() : DirectedBeing<Stop>::DirectedBeing()
{
}
BlendMode Earth::getBlendMode()
{
	return SOLID_BLEND;
}
Figure * Earth::getFigure()
{
	return & earthFigure;
}

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
	prevX = x;
	prevY = y;
}
Scene * DemoScene::rearrange(unsigned int dt, float x, float y)
{
	float fdt = dt * 0.000001; // us -> s
	if (0 <= prevX && prevX < 512 && 0 <= x && x < 512
	 && 0 <= prevY && prevY < 512 && 0 <= y && y < 512
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

