#define NDEBUG
#include <cassert>
#include <cmath>

#include <platform.h>
#include "sss/common.h"
#include "navigation.h"

std::unordered_map<NavPoint*, std::unordered_map<NavPoint*, int>> clockwise;

NavCell * inbound(NavPoint* p1, NavPoint* p2)
{
	if (clockwise.find(p1) == clockwise.end())
	{
		return nullptr;
	}
	auto a = clockwise[p1];
	if (a.find(p2) == a.end())
	{
		return nullptr;
	}
	return & cells[a[p2]];
}

float distance(float a, float b, float c, float x, float y)
{
	assert(a != 0 || b != 0);
	float div = a * a + b * b;
	float t = (a * x + b * y + c) / div;
	return t;
}

float distance(NavPoint * p1, NavPoint * p2, float x, float y)
{
	float a = p1->y - p2->y;
	float b = p2->x - p1->x;
	float c = p1->x * p2->y - p2->x * p1->y;
	return distance(a, b, c, x, y);
}

NavPoint* furthest(NavPoint* p1, NavPoint* p2, NavCell * cell)
{
	// FIXME assert
	if (cell == nullptr)
	{
		return nullptr;
	}
	if (cell->points[0] == p1)// && cells[index].points[1] == p2)
	{
		return cell->points[2];
	}
	else if (cell->points[1] == p1)// && cells[index].points[2] == p2)
	{
		return cell->points[0];
	}
	else //if (cells[index].points[2] == p1)// && cells[index].points[0] == p2)
	{
		return cell->points[1];
	}
}

float leaving(float a, float b, float c, float x, float y, float dx, float dy)
{
	float div = a * dx + b * dy;
	if (div <= 0)
	{
		return INFINITY;
	}
	float t = (- a * x - b * y - c) / div;
	return t;
}

float leaving(NavPoint * p1, NavPoint * p2, float x, float y, float dx, float dy)
{
	float a = p1->y - p2->y;
	float b = p2->x - p1->x;
	float c = p1->x * p2->y - p2->x * p1->y;
	return leaving(a, b, c, x, y, dx, dy);
}

float traverse(NavCell * & cell, float & x, float & y, float dx, float dy, float res)
{
	NavPoint * p1, * p2;
	float t, t1, t2, t3;

	t1 = leaving(cell->points[0], cell->points[1], x, y, dx, dy);
	t2 = leaving(cell->points[1], cell->points[2], x, y, dx, dy);
	t3 = leaving(cell->points[2], cell->points[0], x, y, dx, dy);
	if (t1 < t2)
	{
		if (t1 < t3)
		{
			t = t1;
			p1 = cell->points[0];
			p2 = cell->points[1];
		}
		else
		{
			t = t3;
			p1 = cell->points[2];
			p2 = cell->points[0];
		}
	}
	else
	{
		if (t2 < t3)
		{
			t = t2;
			p1 = cell->points[1];
			p2 = cell->points[2];
		}
		else
		{
			t = t3;
			p1 = cell->points[2];
			p2 = cell->points[0];
		}
	}

	while (true)
	{
		if (1 <= t)
		{
			x += dx;
			y += dy;
			return 1.0;
		}

		NavCell * next = inbound(p2, p1);
		if (next != nullptr)
		{
			cell = next;
		}
		else
		{
			if (std::isnan(res))
			{
				x += t * dx;
				y += t * dy;
				return t;
			}
			else
			{
				float a = p1->y - p2->y;
				float b = p2->x - p1->x;
				float c = p1->x * p2->y - p2->x * p1->y;
				float d = distance(a, b, c, x + dx, y + dy);
				x += t * dx;
				y += t * dy;
				dx = dx * (1 - t) - (1 + res) * d * a;
				dy = dy * (1 - t) - (1 + res) * d * b;
				res = NAN;
				NavPoint * p = p1;
				p1 = p2;
				p2 = p;
				cell = inbound(p2, p1);
			}
		}
		NavPoint* p = furthest(p2, p1, cell);

		float t0 = t;
		float t1 = leaving(p1, p, x, y, dx, dy);
		float t2 = leaving(p, p2, x, y, dx, dy);
		if (t1 < t2)
		{
			p2 = p;
			t = t1;
		}
		else
		{
			p1 = p;
			t = t2;
		}
	}
}

float traverse(NavCell * cell, float x, float y, float dx, float dy)
{
	return traverse(cell, x, y, dx, dy, NAN);
}
