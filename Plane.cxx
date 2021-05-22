#include <stdio.h>
#include <math.h>

#define NDEBUG
#include <cassert>

#include "Plane.h"

float Plane::distance(float x, float y, float z)
{
	assert(a != 0 || b != 0 || c != 0);
	float div = a * a + b * b + c * c;
	return (a * x + b * y + c * z + d) / div;
}
void Plane::intersection(float x, float y, float z, float dx, float dy, float dz, float & ti, float & to, Plane * & pi, Plane * & po)
{
	float dot =  a * dx + b * dy + c * dz;
	if (dot == 0)
	{
		// parallel
		float dis = distance(x, y, z);
		if (0 < dis)
		{
			// always outside
			to = - INFINITY;
			po = this;
		}
	}
	else
	{
		float t = (- a * x - b * y - c * z - d) / dot;
		if (dot < 0)
		{
			if (ti <= t)
			{
				ti = t;
				pi = this;
			}
		}
		else
		{
			assert(0 < dot);
			if (t <= to)
			{
				to = t;
				po = this;
			}
		}
	}
}

Plane * convexEntry(Plane * p, float x, float y, float z, float dx, float dy, float dz, float & t)
{
	float ti, to;
	Plane * pi, * po;
	ti = - INFINITY;
	to = INFINITY;
	pi = nullptr;
	po = nullptr;
	while (p->a != 0 || p->b != 0 || p->c != 0)
	{
		p->intersection(x, y, z, dx, dy, dz, ti, to, pi, po);
		p++;
	}
	//if (t < ti && ti < to)
	if (t <= ti && ti < to)
	{
		t = ti;
		return pi;
	}
	else
	{
		t = INFINITY;
		return nullptr;
	}
}

Plane * convexLeave(Plane * p, float x, float y, float z, float dx, float dy, float dz, float & t)
{
	float ti, to;
	Plane * pi, * po;
	ti = - INFINITY;
	to = INFINITY;
	pi = nullptr;
	po = nullptr;
	while (p->a != 0 || p->b != 0 || p->c != 0)
	{
		p->intersection(x, y, z, dx, dy, dz, ti, to, pi, po);
		p++;
	}
	if (t <= to && ti < to)
	{
		t = to;
		return po;
	}
	else
	{
		t = INFINITY;
		return nullptr;
	}
}

