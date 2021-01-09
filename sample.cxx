#include <stdio.h>
#include <math.h>
#include <time.h>
#include "geometry.h"

static Figure solidFigure("../solid.u-a.txt");
static Figure screenFigure("../screen.u-a.txt");
static Figure earthFigure("../earth.u-a.txt");

static Texture flatTexture("../flat.png");
static Texture bumpTexture("../bump.png");

class Star : public Being
{
public:
	RotY * rotY;
	Move * move;
	Star();
};
Star::Star() : Being::Being(rotY = new RotY(move = new Move(Direction::stop)))
{
}

class SolidStar : public Star
{
public:
	SolidStar();
	virtual BeingType getType();
	virtual Figure * getFigure();
	virtual Texture * getTexture();
};
SolidStar::SolidStar()
{
}
BeingType SolidStar::getType()
{
	return SOLID_BEING;
}
Figure * SolidStar::getFigure()
{
	return & solidFigure;
}
Texture * SolidStar::getTexture()
{
	return & bumpTexture;
}

class ScreenStar : public Star
{
public:
	ScreenStar();
	virtual BeingType getType();
	virtual Figure * getFigure();
	virtual Texture * getTexture();
};
ScreenStar::ScreenStar()
{
}
BeingType ScreenStar::getType()
{
	return SCREEN_BEING;
}
Figure * ScreenStar::getFigure()
{
	return & screenFigure;
}
Texture * ScreenStar::getTexture()
{
	return & flatTexture;
}

class Earth : public Being
{
public:
	Earth(Direction * direction);
	virtual BeingType getType();
	virtual Figure * getFigure();
	virtual Texture * getTexture();
};
Earth::Earth(Direction * direction) : Being::Being(direction)
{
}
BeingType Earth::getType()
{
	return SOLID_BEING;
}
Figure * Earth::getFigure()
{
	return & earthFigure;
}
Texture * Earth::getTexture()
{
	return & flatTexture;
}

class DemoScene : public Scene
{
private:
	Move * lightingMove;
	RotY * lightingRotY;
	RotX * lightingRotX;
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
	star[2] = new ScreenStar();
	star[3] = new ScreenStar();
	earth = new Earth(Direction::stop);

	framing = new Projection(Direction::stop);
	framing->width = 1;
	framing->height = 1;
	framing->depth = 3;

	lightingMove = new Move(lightingRotX = new RotX(lightingRotY = new RotY(Direction::stop)));
	lighting = new Projection(lightingMove);
	lightingMove->dx = - 2;
	lightingMove->dy = 0;
	lightingMove->dz = - 2;
	lightingRotX->angle = - 0.5;
	lightingRotY->angle = - 1;
	lighting->width = 1;
	lighting->height = 1;
	lighting->depth = 3;

	for (int i = 0; i < 4; i++)
	{
		star[i]->rotY->angle = i - angle1;
		star[i]->move->dx = cosf(i - angle2) * 2;
		star[i]->move->dy = 0;
		star[i]->move->dz = 3 + sinf(i - angle2) * 2;
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
		star[i]->rotY->angle = i - angle1;
		star[i]->move->dx = cosf(i - angle2) * 2;
		star[i]->move->dy = 0;
		star[i]->move->dz = 3 + sinf(i - angle2) * 2;
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

