#include <cstdio>
#include <cmath>
#include <climits>
#include <cassert>
#include <utility>
#include <unordered_map>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <platform.h>
#include "sss/Basis.h"
#include "sss/Percipi.h"
#include "sss/common.h"
#include "sss/geometry.h"
#include "sss/renderer.h"
#include "Task.h"
#include "Navigation.h"
#include "BackgroundRenderer.h"
#include "ShadowRenderer.h"
#include "SolidRenderer.h"
#include "LucidRenderer.h"
#include "Solid2DRenderer.h"

const int COUNTER_CAPACITY = 6;
const int MISSILE_CAPACITY = 5;

const int sss::screenWidth = 1024;
const int sss::screenHeight = 512;

extern sss::RenderingMode background;
extern sss::RenderingMode solid2D;
extern sss::RenderingMode solid3D;
extern sss::RenderingMode lucid3D;

static sss::Texture shadowMap(1024, 512);
static sss::Texture backgroundTexture("Landscape.png");
static sss::Texture floorTexture("Floor.png");
static sss::Texture cuboid243Texture("Cuboid243.png");
static sss::Texture cuboid465Texture("Cuboid465.png");
static sss::Texture cuboid8E9Texture("Cuboid8E9.png");
static sss::Texture solidTexture("SolidStar.png");
static sss::Texture lucidTexture("LucidStar.png");
static sss::Texture digitTexture("num8x8.png");
static sss::Texture inst1Texture("Inst1.png");
static sss::Texture inst2Texture("Inst2.png");
static sss::Texture inst3Texture("Inst3.png");

static sss::SurficialFigure backgroundFigure = sss::SurficialFigure(& backgroundTexture, 512, 64, 512, 64, 512, 64);
static sss::SpacialFigure cuboid243Figure(& cuboid243Texture, "Cuboid243.u-c.bin");
static sss::SpacialFigure cuboid465Figure(& cuboid465Texture, "Cuboid465.u-c.bin");
static sss::SpacialFigure cuboid8E9Figure(& cuboid8E9Texture, "Cuboid8E9.u-c.bin");
static sss::SpacialFigure solidFigure(& solidTexture, "Star.u-c.bin");
static sss::SpacialFigure lucidFigure(& lucidTexture, "Star.u-c.bin");
static sss::SpacialFigure earthFigure(& floorTexture, "Floor.u-c.bin");
static sss::SpacialFigure inst1Figure(& inst1Texture, "Floor.u-c.bin");
static sss::SpacialFigure inst2Figure(& inst2Texture, "Floor.u-c.bin");
static sss::SpacialFigure inst3Figure(& inst3Texture, "Floor.u-c.bin");
static sss::SurficialFigure numFonts[] = {
	sss::SurficialFigure(& digitTexture, 0 * 8, 8, 0, 8, 8, 0),
	sss::SurficialFigure(& digitTexture, 1 * 8, 8, 0, 8, 8, 0),
	sss::SurficialFigure(& digitTexture, 2 * 8, 8, 0, 8, 8, 0),
	sss::SurficialFigure(& digitTexture, 3 * 8, 8, 0, 8, 8, 0),
	sss::SurficialFigure(& digitTexture, 4 * 8, 8, 0, 8, 8, 0),
	sss::SurficialFigure(& digitTexture, 5 * 8, 8, 0, 8, 8, 0),
	sss::SurficialFigure(& digitTexture, 6 * 8, 8, 0, 8, 8, 0),
	sss::SurficialFigure(& digitTexture, 7 * 8, 8, 0, 8, 8, 0),
	sss::SurficialFigure(& digitTexture, 8 * 8, 8, 0, 8, 8, 0),
	sss::SurficialFigure(& digitTexture, 9 * 8, 8, 0, 8, 8, 0),
};

class Background : public sss::FinitePresence<sss::Expand<sss::Move<sss::Stop>>>
{
public:
	Background();
	virtual sss::Figure * getFigure();
};
Background::Background() : FinitePresence::FinitePresence(background)
{
}
sss::Figure * Background::getFigure()
{
	return & backgroundFigure;
}

class Missile : public sss::FinitePresence<sss::RotZ<sss::Move<sss::Stop>>>
{
public:
	NavCell * cell;
	Missile();
	virtual sss::Figure * getFigure();
};
Missile::Missile() : FinitePresence::FinitePresence(lucid3D)
{
	visible = false;
}
sss::Figure * Missile::getFigure()
{
	return & lucidFigure;
}

class Foe : public sss::FinitePresence<sss::RotZ<sss::Move<sss::Stop>>>
{
public:
	Foe();
	virtual sss::Figure * getFigure();
	bool check(float x, float y, float r, bool damage);
};
Foe::Foe() : FinitePresence::FinitePresence(solid3D)
{
}
sss::Figure * Foe::getFigure()
{
	return & solidFigure;
}
bool Foe::check(float x, float y, float r, bool damage)
{
	if (visible)
	{
		float dx = x - direction->next->dx;
		float dy = y - direction->next->dy;
		if (sqrt(dx * dx + dy * dy) < r)
		{
			if (damage)
			{
				visible = false;
			}
			return true;
		}
	}
	return false;
}

class Saggy : public Foe
{
public:
	Saggy();
};
Saggy::Saggy()
{
}

class Hasty : public Foe
{
public:
	NavCell * cell;
	Hasty(NavCell * cell);
};
Hasty::Hasty(NavCell * cell) : cell(cell)
{
}

class Star : public sss::FinitePresence<sss::RotZ<sss::Move<sss::Stop>>>
{
public:
	Star(sss::RenderingMode & mode);
};
Star::Star(sss::RenderingMode & mode) : FinitePresence::FinitePresence(mode)
{
}

class SolidStar : public Star
{
public:
	SolidStar();
	virtual sss::Figure * getFigure();
};
SolidStar::SolidStar() : Star::Star(solid3D)
{
	label = 1;
}
sss::Figure * SolidStar::getFigure()
{
	return & solidFigure;
}

class LucidStar : public Star
{
public:
	LucidStar();
	virtual sss::Figure * getFigure();
};
LucidStar::LucidStar() : Star::Star(lucid3D)
{
}
sss::Figure * LucidStar::getFigure()
{
	return & lucidFigure;
}

class Earth : public sss::FinitePresence<sss::Move<sss::Stop>>
{
public:
	Earth();
	virtual sss::Figure * getFigure();
};
Earth::Earth() : FinitePresence::FinitePresence(solid3D)
{
}
sss::Figure * Earth::getFigure()
{
	return & earthFigure;
}

class Cuboid243 : public sss::FinitePresence<sss::RotZ<sss::Move<sss::Stop>>>
{
public:
	Cuboid243();
	virtual sss::Figure * getFigure();
};
Cuboid243::Cuboid243() : FinitePresence::FinitePresence(solid3D)
{
}
sss::Figure * Cuboid243::getFigure()
{
	return & cuboid243Figure;
}

class Cuboid465 : public sss::FinitePresence<sss::RotZ<sss::Move<sss::Stop>>>
{
public:
	Cuboid465();
	virtual sss::Figure * getFigure();
};
Cuboid465::Cuboid465() : FinitePresence::FinitePresence(solid3D)
{
}
sss::Figure * Cuboid465::getFigure()
{
	return & cuboid465Figure;
}

class Cuboid8E9 : public sss::FinitePresence<sss::RotZ<sss::Move<sss::Stop>>>
{
public:
	Cuboid8E9();
	virtual sss::Figure * getFigure();
};
Cuboid8E9::Cuboid8E9() : FinitePresence::FinitePresence(solid3D)
{
}
sss::Figure * Cuboid8E9::getFigure()
{
	return & cuboid8E9Figure;
}

class Digit : public sss::FinitePresence<sss::Expand<sss::Move<sss::Stop>>>
{
public:
	int i = 0;
	Digit();
	virtual sss::Figure * getFigure();
};
Digit::Digit() : FinitePresence::FinitePresence(solid2D)
{
}
sss::Figure * Digit::getFigure()
{
	return & numFonts[i];
}

class Inst : public sss::FinitePresence<sss::Expand<sss::RotX<sss::RotZ<sss::Move<sss::Stop>>>>>
{
private:
	sss::Figure * figure;
public:
	Inst(sss::Figure * figure);
	virtual sss::Figure * getFigure();
};
Inst::Inst(sss::Figure * figure) : FinitePresence::FinitePresence(lucid3D), figure(figure)
{
}
sss::Figure * Inst::getFigure()
{
	return figure;
}

static bool pressedHigh = false;
static bool pressedLow = false;
static int sincePressed = 0;
static float origY;
static float prevX;
static float prevY;

static sss::PerspectiveProjection<sss::RotX<sss::RotZ<sss::Move<sss::Stop>>>> framing(100, 50, 0.5, 50);

static float slip;
static float yawing = 0.0;


class Props0
{
public:
	Props0();
	Cuboid8E9 cuboid1;
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
		digit[i].direction->next->dx = 0.9 + (i * 16) * - 2.0 / sss::screenWidth;
		digit[i].direction->next->dy = 0.75;
	}

	cuboid1.direction->angle = 2 * M_PI / 2;
	cuboid1.direction->next->dx = -11;
	cuboid1.direction->next->dy = 22;
}

class Props1
{
public:
	Props1();
	~Props1();
	void rearrange(float dt);
	Earth earth;
	Cuboid8E9 cuboid1;
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
	cuboid1.direction->next->dx = 0;
	cuboid1.direction->next->dy = 19;
	cuboid2.direction->angle = M_PI / 2;
	cuboid2.direction->next->dx = 14;
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
	float x = sss::controllers[0].x;
	float y = sss::controllers[0].y;
	if (0 <= x && x < sss::screenWidth && 0 <= y && y < sss::screenHeight && sss::pixelLabel(x, y) != 0)
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
	sss::Percipi<Props0> props0;
public:
	Props3();
	void rearrange(float dt, NavCell * cell);
	Cuboid465 cuboid1;
	Cuboid465 cuboid2;
	Cuboid465 cuboid3;
	Cuboid8E9 cuboid4;
	Cuboid465 cuboid5;
	Cuboid8E9 cuboid6;
	Cuboid8E9 cuboid7;
	Cuboid465 cuboid8;
	Cuboid465 cuboid9;

	Inst inst1;
	Inst inst2;
	Inst inst3;

	TaskQueue queue;
	Navigation nav[2];
	int navCurr;
	Saggy saggy;
	Hasty hasty;
};
Props3::Props3() : queue(2), inst1(& inst1Figure), inst2(& inst2Figure), inst3(& inst3Figure), hasty(& cells[8])
{
	inst1.direction->scale = 0.2;
	inst1.direction->next->next->angle = M_PI / 2;
	inst1.direction->next->next->next->dx = 10;
	inst1.direction->next->next->next->dy = 33;
	inst1.direction->next->next->next->dz = - 0.1;

	inst2.direction->scale = 0.2;
	inst2.direction->next->angle = M_PI / 2;
	inst2.direction->next->next->angle = M_PI / 2;
	inst2.direction->next->next->next->dx = 12.9;
	inst2.direction->next->next->next->dy = 33;
	inst2.direction->next->next->next->dz = - 3;
	inst2.visible = false;

	inst3.direction->scale = 0.2;
	inst3.direction->next->angle = M_PI / 2;
	inst3.direction->next->next->next->dx = 10;
	inst3.direction->next->next->next->dy = 48.9;
	inst3.direction->next->next->next->dz = - 3;
	inst3.visible = false;

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
	cuboid4.direction->next->dx = 17;
	cuboid4.direction->next->dy = 34;

	cuboid5.direction->angle = 1 * M_PI / 2;
	cuboid5.direction->next->dx = -4;
	cuboid5.direction->next->dy = 27;

	cuboid6.direction->angle = 1 * M_PI / 2;
	cuboid6.direction->next->dx = 6;
	cuboid6.direction->next->dy = 25;

	cuboid7.direction->angle = 3 * M_PI / 2;
	cuboid7.direction->next->dx = 10;
	cuboid7.direction->next->dy = 53;

	cuboid8.direction->angle = 3 * M_PI / 2;
	cuboid8.direction->next->dx = 4;
	cuboid8.direction->next->dy = 43;

	cuboid9.direction->angle = 3 * M_PI / 2;
	cuboid9.direction->next->dx = 16;
	cuboid9.direction->next->dy = 43;

	saggy.direction->next->dx = 10;
	saggy.direction->next->dy = 48;
	saggy.direction->next->dz = -1.5;

	hasty.direction->next->dx = 3;
	hasty.direction->next->dy = 33;
	hasty.direction->next->dz = -1;

	nav[0].init(& cells[0] /* FIXME: it's not cool to have this hard-coded */, framing.direction->next->next->dx, framing.direction->next->next->dy, 1.0, 0.5);
	nav[0].execute();
	navCurr = 0;
	nav[1].init(& cells[0] /* FIXME: it's not cool to have this hard-coded */, framing.direction->next->next->dx, framing.direction->next->next->dy, 1.0, 0.5);
	queue.push(nav[1]);
}

void Props3::rearrange(float fdt, NavCell * cell)
{
	int navNext = (navCurr + 1) % 2;
	if (nav[navNext].done)
	{
		nav[navCurr].init(cell, framing.direction->next->next->dx, framing.direction->next->next->dy, 1.0, 0.5);
		queue.push(nav[navCurr]);
		navCurr = navNext;
	}

	for (int i = 0; i < MISSILE_CAPACITY; i++)
	{
		if (props0->missiles[i].visible)
		{
			Missile * m = & props0->missiles[i];
			if (saggy.check(m->direction->next->dx, m->direction->next->dy, 1, true))
			{
				m->visible = false;
				inst2.visible = false;
				inst3.visible = false;
			}
			else if (hasty.check(m->direction->next->dx, m->direction->next->dy, 1, true))
			{
				m->visible = false;
			}
		}
	}

	int x = sss::controllers[0].x;
	int y = sss::controllers[0].y;
	if (hasty.visible)
	{
		NavPoint end{framing.direction->next->next->dx, framing.direction->next->next->dy, 0, 0};
		NavPoint * dst;
		if (traversable(hasty.cell, hasty.direction->next->dx, hasty.direction->next->dy, end.x, end.y, 1.0))
		{
			// go direct if possible
			dst = & end;
		}
		else
		{
			dst = firstCorner(nav[navCurr].d, hasty.cell, hasty.direction->next->dx, hasty.direction->next->dy, 1.0, 0.5);
			NavPoint * next = nav[navCurr].r[dst];
			if (next != nullptr && traversable(hasty.cell, hasty.direction->next->dx, hasty.direction->next->dy, next->x + 1.5 * next->dx, next->y + 1.5 * next->dy, 1.0))
			{
				dst = next;
			}
		}
		if (dst != nullptr)
		{
			float dx = dst->x + 1.5 * dst->dx - hasty.direction->next->dx;
			float dy = dst->y + 1.5 * dst->dy - hasty.direction->next->dy;
			float r = sqrt(dx * dx + dy * dy);
			if (fdt < r)
			{
				dx = dx * fdt / r;
				dy = dy * fdt / r;
			}
			traverse(hasty.cell, hasty.direction->next->dx, hasty.direction->next->dy, dx, dy, 1.0, 0.1);
			hasty.direction->angle = atan2(dx, dy);
		}

		if (yawing == 0)
		{
			float dx = framing.direction->next->next->dx - hasty.direction->next->dx;
			float dy = framing.direction->next->next->dy - hasty.direction->next->dy;
			float r = sqrt(dx * dx + dy * dy);
			if (r < 1) // TBD: should have better collision detection?
			{
				slip = atan2(dx, dy);
				yawing = 1.0;
			}
		}

	}
}

class Props4
{
private:
	Earth earth1;
	Earth earth2;
	Earth earth3;
	Cuboid243 cuboid1;
	Cuboid243 cuboid2;
	Cuboid465 cuboid3;
	Cuboid8E9 cuboid4;
	Cuboid465 cuboid5;
	Cuboid465 cuboid6;
	Cuboid8E9 cuboid7;
	Cuboid465 cuboid8;
	Cuboid8E9 cuboid9;
	Cuboid8E9 cuboid10;
	Cuboid465 cuboid11;
public:
	Props4();
};
Props4::Props4()
{
	earth1.direction->dx = 0;
	earth1.direction->dy = 30;
	earth2.direction->dx = 0;
	earth2.direction->dy = 60;
	earth3.direction->dx = 30;
	earth3.direction->dy = 60;

	cuboid1.direction->angle = 1 * M_PI / 2;
	cuboid1.direction->next->dx = -1;
	cuboid1.direction->next->dy = 44;

	cuboid2.direction->angle = 1 * M_PI / 2;
	cuboid2.direction->next->dx = -5;
	cuboid2.direction->next->dy = 44;

	cuboid3.direction->angle = 0 * M_PI / 2;
	cuboid3.direction->next->dx = -7;
	cuboid3.direction->next->dy = 48;

	cuboid4.direction->angle = 2 * M_PI / 2;
	cuboid4.direction->next->dx = -9;
	cuboid4.direction->next->dy = 58;

	cuboid5.direction->angle = 1 * M_PI / 2;
	cuboid5.direction->next->dx = -3;
	cuboid5.direction->next->dy = 67;

	cuboid6.direction->angle = 1 * M_PI / 2;
	cuboid6.direction->next->dx = 3;
	cuboid6.direction->next->dy = 67;

	cuboid7.direction->angle = 1 * M_PI / 2;
	cuboid7.direction->next->dx = 13;
	cuboid7.direction->next->dy = 69;

	cuboid8.direction->angle = 1 * M_PI / 2;
	cuboid8.direction->next->dx = 23;
	cuboid8.direction->next->dy = 67;

	cuboid9.direction->angle = 2 * M_PI / 2;
	cuboid9.direction->next->dx = 29;
	cuboid9.direction->next->dy = 58;

	cuboid10.direction->angle = 0 * M_PI / 2;
	cuboid10.direction->next->dx = 29;
	cuboid10.direction->next->dy = 44;

	cuboid11.direction->angle = 1 * M_PI / 2;
	cuboid11.direction->next->dx = 22;
	cuboid11.direction->next->dy = 43;
}

class DemoScene : public sss::Scene
{
private:
	sss::Percipi<Props0> props0;
	Background sky1;
	Background sky2;
protected:
	NavCell * cell;
	sss::ParallelProjection<sss::Move<sss::RotX<sss::RotZ<sss::Stop>>>> lighting;
public:
	DemoScene(NavCell * cell);
	void render();
	sss::Scene * rearrange(unsigned int dt);
};
DemoScene::DemoScene(NavCell * cell) : cell(cell), lighting(60, 30, 10, 40)
{
	prevX = sss::controllers[0].x;
	prevY = sss::controllers[0].y;
}
void DemoScene::render()
{
	float sect = M_PI / (atan((framing.width / 2) / framing.far));
	float turn = - framing.direction->next->angle / (M_PI * 2);
	sky1.direction->scale = sss::screenWidth * sect / sky1.getFigure()->texture->width;
	sky1.direction->next->dx = (turn - floor(turn) - 1) * (2 * sect);
	sky2.direction->scale = sss::screenWidth * sect / sky2.getFigure()->texture->width;
	sky2.direction->next->dx = (turn - floor(turn)) * (2 * sect);

	props0->backgroundRenderer.process();

	sss::Matrix4x4 lightingMatrix;
	lighting.getMatrix(lightingMatrix.elements);

	sss::Matrix4x4 framingMatrix;
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
sss::Scene * DemoScene::rearrange(unsigned int dt)
{
	float x = sss::controllers[0].x;
	float y = sss::controllers[0].y;
	NavCell * next = cell;

	float fdt = dt * 0.000001; // us -> s

	for (int i = 0; i < MISSILE_CAPACITY; i++)
	{
		if (props0->missiles[i].visible)
		{
			Missile * m = & props0->missiles[i];
			float dx = sinf(m->direction->angle) * fdt * 10;
			float dy = cosf(m->direction->angle) * fdt * 10;
			float t = traverse(m->cell, m->direction->next->dx, m->direction->next->dy, dx, dy, 0, NAN);
			if (t < 1.0)
			{
				m->visible = false;
			}
		}
	}

	if (yawing == 0)
	{
		if (pressedHigh && prevY < sss::screenHeight / 2 && sss::screenHeight / 2 <= y)
		{
			if (! props0->missiles[props0->missileIndex].visible)
			{
				float a = atan((framing.width / 2) / framing.far * (x - sss::screenWidth / 2) / (sss::screenWidth / 2));
				Missile * m = & props0->missiles[props0->missileIndex];
				m->visible = true;
				m->cell = cell;
				m->direction->angle = framing.direction->next->angle + a;
				m->direction->next->dx = framing.direction->next->next->dx;
				m->direction->next->dy = framing.direction->next->next->dy;
				m->direction->next->dz = -1.7;
				props0->missileIndex = (props0->missileIndex + 1) % MISSILE_CAPACITY;
			}
		}

		if (x < 0 || sss::screenWidth <= x || y < sss::screenHeight / 2 || sss::screenHeight <= y)
		{
			pressedLow = false;
		}

		sincePressed++;
		if (pressedLow
 && 1 < sincePressed /* work adound for Surface */
)
		{
			framing.direction->next->angle += (x - prevX) * -0.002f;

			float a = fdt * fminf(y - origY, sss::screenHeight / 4) / (sss::screenHeight / 4);
			float dx = sinf(framing.direction->next->angle) * a * 10;
			float dy = cosf(framing.direction->next->angle) * a * 10;
			traverse(next, framing.direction->next->next->dx, framing.direction->next->next->dy, dx, dy, 1.0, 0.1);
		}
	}
	else
	{
		float dx = cos(slip) * std::abs(yawing) * fdt * 10;
		float dy = sin(slip) * std::abs(yawing) * fdt * 10;
		traverse(next, framing.direction->next->next->dx, framing.direction->next->next->dy, dx, dy, 1.0, 0.1);

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

	return (* next->depiction)(next);
}

class DemoScene1 : public DemoScene
{
private:
	sss::Percipi<Props1> props1;
	sss::Percipi<Props2> props2;
	sss::Percipi<Props3> props3;
public:
	DemoScene1(NavCell * cell);
	sss::Scene * rearrange(unsigned int dt);
};
DemoScene1::DemoScene1(NavCell * cell) : DemoScene::DemoScene(cell)
{
	lighting.direction->dy = 5;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}
sss::Scene * DemoScene1::rearrange(unsigned int dt)
{
	sss::Scene * next = DemoScene::rearrange(dt);
	float fdt = dt * 0.000001; // us -> s
	props1->rearrange(fdt);
	props3->rearrange(fdt, cell);
	return next;
}

class DemoScene2 : public DemoScene
{
private:
	sss::Percipi<Props1> props1;
	sss::Percipi<Props2> props2;
	sss::Percipi<Props3> props3;
public:
	DemoScene2(NavCell * cell);
	sss::Scene * rearrange(unsigned int dt);
};
DemoScene2::DemoScene2(NavCell * cell) : DemoScene::DemoScene(cell)
{
	lighting.direction->dx = -20;
	lighting.direction->dy = 2;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}
sss::Scene * DemoScene2::rearrange(unsigned int dt)
{
	sss::Scene * next = DemoScene::rearrange(dt);
	float fdt = dt * 0.000001; // us -> s
	props1->rearrange(fdt);
	props3->rearrange(fdt, cell);
	return next;
}

class DemoScene3 : public DemoScene
{
private:
	sss::Percipi<Props2> props2;
	sss::Percipi<Props3> props3;
	sss::Percipi<Props4> props4;
public:
	DemoScene3(NavCell * cell);
	sss::Scene * rearrange(unsigned int dt);
};
DemoScene3::DemoScene3(NavCell * cell) : DemoScene::DemoScene(cell)
{
	lighting.direction->dx = -20;
	lighting.direction->dy = 2;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}
sss::Scene * DemoScene3::rearrange(unsigned int dt)
{
	props3->inst1.visible = false;
	if (props3->saggy.visible)
	{
		props3->inst2.visible = true;
		props3->inst3.visible = true;
	}
	sss::Scene * next = DemoScene::rearrange(dt);
	float fdt = dt * 0.000001; // us -> s
	props3->rearrange(fdt, cell);
	return next;
}

class DemoScene4 : public DemoScene
{
private:
	sss::Percipi<Props3> props3;
	sss::Percipi<Props4> props4;
public:
	DemoScene4(NavCell * cell);
	sss::Scene * rearrange(unsigned int dt);
};
DemoScene4::DemoScene4(NavCell * cell) : DemoScene::DemoScene(cell)
{
	lighting.direction->dx = -20;
	lighting.direction->dy = 2;
	lighting.direction->dz = -15;
	lighting.direction->next->angle = M_PI / 3;
	lighting.direction->next->next->angle = M_PI / 4;
}
sss::Scene * DemoScene4::rearrange(unsigned int dt)
{
	sss::Scene * next = DemoScene::rearrange(dt);
	float fdt = dt * 0.000001; // us -> s
	props3->rearrange(fdt, cell);
	return next;
}

NavPoint roundaboutNE = { 25, 65, -1, -1 };
NavPoint roundaboutNW = { -5, 65, +1, -1 };
NavPoint roundaboutSE = { 25, 45, -1, +1 };
NavPoint roundaboutSW = { -5, 45, +1, +1 };

NavPoint islandNE = { 17, 57, +1, +1 };
NavPoint islandNW = { 3, 57, -1, +1 };
NavPoint islandSE = { 17, 49, +1, -1 };
NavPoint islandSW = { 3, 49, -1, -1 };

NavPoint avenueNE = { 13, 45, -1, +1 };
NavPoint avenueNW = { 7, 45, +1, +1 };
NavPoint avenueSE = { 13, 39, -1, 0 };
NavPoint avenueSW = { 7, 39, +1, 0 };

NavPoint cornerSE = { 13, 29, -1, +1 };
NavPoint cornerNW = { 7, 37, +1, -1 };

NavPoint gatanNE = { 5, 37, 0, -1 };
NavPoint gatanSE = { 5, 29, 0, +1 };
NavPoint gatanSW = { -15, 29, -1, +1 };

NavPoint platzNE = { -15, 37, -1, -1 };
NavPoint platzNW = { -33, 37, +1, -1 };
NavPoint platzSE = { -15, 3, +1, +1 };
NavPoint platzSW = { -33, 3, +1, +1 };

NavPoint stortorgetNE = { 15, 15, -1, -1 };
NavPoint stortorgetNW = { -15, 15, -1, -1 };
NavPoint stortorgetSE = { 15, -15, -1, +1 };
NavPoint stortorgetSW = { -15, -15, +1, +1 };

NavCell cells[] =
{
	{ & platzSE, & stortorgetNW, & stortorgetSE, & cue<DemoScene1> },
	{ & stortorgetSW, & platzSE, & stortorgetSE, & cue<DemoScene1> },
	{ & stortorgetSE, & stortorgetNW, & stortorgetNE, & cue<DemoScene1> },
	{ & stortorgetNW, & platzSE, & platzSW, & cue<DemoScene2> },
	{ & platzNW, & stortorgetNW, & platzSW, & cue<DemoScene2> },
	{ & platzNW, & gatanSW, & stortorgetNW, & cue<DemoScene2> },
	{ & platzNW, & platzNE, & gatanSW, & cue<DemoScene2> },
	{ & platzNE, & gatanNE, & gatanSW, & cue<DemoScene2> },
	{ & gatanNE, & gatanSE, & gatanSW, & cue<DemoScene2> },
	{ & gatanNE, & cornerNW, & gatanSE, & cue<DemoScene3> },
	{ & cornerNW, & cornerSE, & gatanSE, & cue<DemoScene3> },
	{ & cornerNW, & avenueSE, & cornerSE, & cue<DemoScene3> },
	{ & cornerNW, & avenueSW, & avenueSE, & cue<DemoScene3> },
	{ & avenueSW, & avenueNW, & avenueSE, & cue<DemoScene4> },
	{ & avenueNW, & avenueNE, & avenueSE, & cue<DemoScene4> },
	{ & islandSE, & avenueNE, & avenueNW, & cue<DemoScene4> },
	{ & islandSW, & islandSE, & avenueNW, & cue<DemoScene4> },
	{ & islandSW, & avenueNW, & roundaboutSW, & cue<DemoScene4> },
	{ & roundaboutSW, & roundaboutNW, & islandSW, & cue<DemoScene4> },
	{ & roundaboutNW, & islandNW, & islandSW, & cue<DemoScene4> },
	{ & roundaboutNW, & roundaboutNE, & islandNW, & cue<DemoScene4> },
	{ & roundaboutNE, & islandNE, & islandNW, & cue<DemoScene4> },
	{ & roundaboutNE, & roundaboutSE, & islandNE, & cue<DemoScene4> },
	{ & roundaboutSE, & islandSE, & islandNE, & cue<DemoScene4> },
	{ & islandSE, & roundaboutSE, & avenueNE, & cue<DemoScene4> },
};

int expanse = sizeof(cells) / sizeof(NavCell); // FIXME rename


sss::Scene * sss::arrange()
{
	// check if the cells are clockwise.
	for (int i = 0; i < expanse; i++)
	{
		float a1 = cells[i].points[0]->y - cells[i].points[1]->y;
		float b1 = cells[i].points[1]->x - cells[i].points[0]->x;

		float dx2 = cells[i].points[2]->x - cells[i].points[1]->x;
		float dy2 = cells[i].points[2]->y - cells[i].points[1]->y;

		if  (0 < a1 * dx2 + b1 * dy2)
		{
			std::printf("Not clockwise: ");
			std::printf("(%f, %f) -> ", cells[i].points[0]->x, cells[i].points[0]->y);
			std::printf("(%f, %f) -> ", cells[i].points[1]->x, cells[i].points[1]->y);
			std::printf("(%f, %f)\n", cells[i].points[2]->x, cells[i].points[2]->y);
		}

		clockwise[cells[i].points[0]][cells[i].points[1]] = i;
		clockwise[cells[i].points[1]][cells[i].points[2]] = i;
		clockwise[cells[i].points[2]][cells[i].points[0]] = i;
	}

	framing.direction->next->next->dy = -10;
	framing.direction->next->next->dz = -1.5;
	framing.direction->angle = M_PI / 2;

	return (* cells[0].depiction)(& cells[0]);
}

void sss::buttonPressed(int pos)
{
	sincePressed = 0;
	if (sss::controllers[0].y < sss::screenHeight / 2)
	{
		pressedHigh = true;
	}
	if (sss::screenHeight / 2 <= sss::controllers[0].y)
	{
		pressedLow = true;
		origY = sss::controllers[0].y;
	}
}

void sss::buttonReleased(int pos)
{
	pressedHigh = false;
	pressedLow = false;
}

void sss::wheelMoved(float d)
{
	std::printf("wheelMoved %f\n", d);
}

