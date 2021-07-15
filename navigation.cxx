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

NavPoint* furthest(NavPoint* p1, NavPoint* p2, NavCell* cell)
{
	if (cell == nullptr)
	{
		return nullptr;
	}
	if (cell->points[0] == p1)
	{
		assert(cells[index].points[1] == p2);
		return cell->points[2];
	}
	else if (cell->points[1] == p1)
	{
		assert(cells[index].points[2] == p2);
		return cell->points[0];
	}
	else
	{
		assert(cells[index].points[2] == p1);
		assert(cells[index].points[0] == p2);
		return cell->points[1];
	}
}

float invasion(float a, float b, float c, float x, float y, float dx, float dy)
{
	float div = a * dx + b * dy;
	if (div <= 0)
	{
		return INFINITY;
	}
	float t = (-a * x - b * y - c) / div;
	return t;
}

float invasion(NavPoint* p1, NavPoint* p2, float x, float y, float dx, float dy, float size)
{
	float x1 = p1->x + size * p1->dx;
	float y1 = p1->y + size * p1->dy;
	float x2 = p2->x + size * p2->dx;
	float y2 = p2->y + size * p2->dy;
	float a = y1 - y2;
	float b = x2 - x1;
	float c = x1 * y2 - x2 * y1;
	return invasion(a, b, c, x, y, dx, dy);
}

float traverse(NavCell * & cell, float & x, float & y, float dx, float dy, float size, float res)
{
	NavPoint * p1, * p2;
	float t, t1, t2, t3;

	t1 = invasion(cell->points[0], cell->points[1], x, y, dx, dy, size);
	t2 = invasion(cell->points[1], cell->points[2], x, y, dx, dy, size);
	t3 = invasion(cell->points[2], cell->points[0], x, y, dx, dy, size);
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
		if (next == nullptr)
		{
			if (std::isnan(res))
			{
				x += t * dx;
				y += t * dy;
				return t;
			}
			else
			{
				float x1 = p1->x + size * p1->dx;
				float y1 = p1->y + size * p1->dy;
				float x2 = p2->x + size * p2->dx;
				float y2 = p2->y + size * p2->dy;
				float a = y1 - y2;
				float b = x2 - x1;
				float c = x1 * y2 - x2 * y1;
				float d = distance(a, b, c, x + dx, y + dy);
				x += t * dx;
				y += t * dy;
				dx = dx * (1 - t) - (1 + res) * d * a;
				dy = dy * (1 - t) - (1 + res) * d * b;
				res = NAN;
				NavPoint * p = p1;
				p1 = p2;
				p2 = p;
				next = inbound(p2, p1);
			}
		}
		cell = next;

		NavPoint* p = furthest(p2, p1, cell);

		t1 = invasion(p1, p, x, y, dx, dy, size);
		t2 = invasion(p, p2, x, y, dx, dy, size);
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

float traverse(NavCell * cell, float x, float y, float dx, float dy, float size)
{
	return traverse(cell, x, y, dx, dy, size, NAN);
}

bool traversable(NavCell * cell, float x1, float y1, float x2, float y2, float size)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	return 1 <= traverse(cell, x1, y1, dx, dy, size);
}

float annexation(float x, float y, float x1, float y1, float dx1, float dy1, float x2, float y2, float dx2, float dy2, float t0)
{
	float a = dy1*dx2 - dx1*dy2;
	float b = dy2*x - dy1*x + dx1*y - dx2*y + dy1*x2 + y1*dx2 - dx1*y2 - x1*dy2;
	float c = y2*x - y1*x + x1*y - x2*y + y1*x2 - x1*y2;
	if (a == 0)
	{
		// t*b + c = 0
		if (b == 0)
		{
			return INFINITY;
		}
		float t = - c / b;
		if (t < t0)
		{
			return INFINITY;
		}
		return t;
	}

	float s = b*b - 4*a*c;
	if (s <= 0)
	{
		return INFINITY;
	}

	float r = sqrt(s);

	float t1 = (- b + r) / (2*a);
	float t2 = (- b - r) / (2*a);

	if (t1 < t2)
	{
		if (t2 < t0)
		{
			return INFINITY;
		}
		if (t1 < t0)
		{
			return t2;
		}
		return t1;
	}
	else
	{
		if (t1 < t0)
		{
			return INFINITY;
		}
		if (t2 < t0)
		{
			return t1;
		}
		return t2;
	}
}

float annexation(float x, float y, NavPoint * p1, NavPoint * p2, float s0, float s1, float t0)
{
	float x1 = p1->x + s0 * p1->dx;
	float y1 = p1->y + s0 * p1->dy;
	float dx1 = (s1 - s0) * p1->dx;
	float dy1 = (s1 - s0) * p1->dy;
	float x2 = p2->x + s0 * p2->dx;
	float y2 = p2->y + s0 * p2->dy;
	float dx2 = (s1 - s0) * p2->dx;
	float dy2 = (s1 - s0) * p2->dy;
	return annexation(x, y, x1, y1, dx1, dy1, x2, y2, dx2, dy2, t0);
}

float resize(NavCell * & cell, float x, float y, float s0, float s1)
{
	float t, t1, t2, t3;
	NavPoint * p1, * p2;

	t1 = annexation(x, y, cell->points[0], cell->points[1], s0, s1, 0);
	t2 = annexation(x, y, cell->points[1], cell->points[2], s0, s1, 0);
	t3 = annexation(x, y, cell->points[2], cell->points[0], s0, s1, 0);

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
			return 1.0;
		}

		NavCell * next = inbound(p2, p1);
		if (next == nullptr)
		{
			return t;
		}
		cell = next;

		NavPoint* p = furthest(p2, p1, cell);

		t1 = annexation(x, y, p1, p, s0, s1, t);
		t2 = annexation(x, y, p, p2, s0, s1, t);
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

void hop(std::map<NavPoint*, std::map<NavPoint*, float>>& m, NavCell* c1, NavPoint* p1, NavCell* c2, NavPoint* p2, float size, float heed)
{
	float x1 = p1->x + size * p1->dx;
	float y1 = p1->y + size * p1->dy;
	traverse(c1, x1, y1, heed * p1->dx, heed * p1->dy, size, 0.01);

	float x2 = p2->x + size * p2->dx;
	float y2 = p2->y + size * p2->dy;
	traverse(c2, x2, y2, heed * p2->dx, heed * p2->dy, size, 0.01);

	float dx = x2 - x1;
	float dy = y2 - y1;

	float t = traverse(c1, x1, y1, dx, dy, size, NAN);
	if (1.0 <= t && c1 == c2)
	{
		m[p1][p2] = sqrt(dx * dx + dy * dy);
	}
}

void navigation(std::unordered_map<NavPoint *, float> & d, std::unordered_map<NavPoint *, NavPoint *> * r, NavCell * cell, NavPoint & end, float size, float heed)
{
	std::set<std::pair<float, NavPoint*>> q;
	std::map<NavPoint *, std::map<NavPoint *, float>> edges;
	for (int i = 0; i < expanse; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			hop(edges, cell, & end, & cells[i], cells[i].points[j], size, heed);
			d[cells[i].points[j]] = INFINITY;
			for (int k = 0; k < expanse; k++)
			{
				for (int l = 0; l < 3; l++)
				{
					if (cells[i].points[j] != cells[k].points[l])
					{
						hop(edges, & cells[i], cells[i].points[j], &cells[k], cells[k].points[l], size, heed);
					}
				}
			}
		}
	}

	for (auto p1 : edges)
	{
		d[p1.first] = INFINITY;
	}
	d[& end] = 0;
	for (auto p1 : edges)
	{
		q.insert(std::make_pair(d[p1.first], p1.first));
	}
	q.insert(std::make_pair(0, & end));
	while (! q.empty())
	{
		auto begin = q.begin();
		float min = begin->first; // FIXME: better name?
		NavPoint* p = begin->second;
		q.erase(begin);

		for (auto next: edges[p])
		{
			float tmp = d[next.first]; // FIXME: better name?
			if (min + next.second < tmp)
			{
				q.erase(std::make_pair(tmp, next.first));
				d[next.first] = tmp = min + next.second;
				q.insert(std::make_pair(tmp, next.first));
				if (r != nullptr)
				{
					(* r)[next.first] = p;
				}
			}
		}
	}
}

NavPoint * firstCorner(std::unordered_map<NavPoint *, float> & d, NavCell * cell, float x, float y, float size, float heed)
{
	NavPoint * dst = nullptr;
	float min = INFINITY;
	for (auto p : d)
	{
		float dx = p.first->x + (size + heed) * p.first->dx - x;
		float dy = p.first->y + (size + heed) * p.first->dy - y;
		if (1 <= traverse(cell, x, y, dx, dy, size))
		{
			float r = sqrt(dx * dx + dy * dy);
			if (p.second + r < min)
			{
				dst = p.first;
				min = p.second + r;
			}
		}
	}
	return dst;
}

