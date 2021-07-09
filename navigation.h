#include <unordered_map>
#include <set>
#include <map>

class NavPoint
{
public:
	float x;
	float y;
	float dx;
	float dy;
};

class NavCell
{
public:
	NavPoint * points[3];
	sss::Scene * (* depiction)(NavCell * cell);
};

template <typename T> sss::Scene * cue(NavCell * cell)
{
	return (sss::Scene *) new T(cell);
}

extern NavCell cells[];

extern int expanse;

extern std::unordered_map<NavPoint*, std::unordered_map<NavPoint*, int>> clockwise;

NavCell * inbound(NavPoint* p1, NavPoint* p2);

float distance(float a, float b, float c, float x, float y);

float distance(NavPoint * p1, NavPoint * p2, float x, float y);

NavPoint* furthest(NavPoint* p1, NavPoint* p2, NavCell * cell);

float leaving(float a, float b, float c, float x, float y, float dx, float dy);

float leaving(NavPoint * p1, NavPoint * p2, float x, float y, float dx, float dy);

float traverse(NavCell * & cell, float & x, float & y, float dx, float dy, float res);

float traverse(NavCell * cell, float x, float y, float dx, float dy);

NavCell * transit(NavCell * curr, float x, float y, float & dx, float & dy, bool deflection);

