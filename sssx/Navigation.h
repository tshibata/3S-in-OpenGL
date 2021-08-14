#include <unordered_map>
#include <set>
#include <map>

namespace sssx
{

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

NavCell * settle(float x, float y);

NavCell * inbound(NavPoint* p1, NavPoint* p2);

float distance(float a, float b, float c, float x, float y);

NavPoint* furthest(NavPoint* p1, NavPoint* p2, NavCell * cell);

float traverse(NavCell * & cell, float & x, float & y, float dx, float dy, float size, float res);

float traverse(NavCell * cell, float x, float y, float dx, float dy, float size);

bool traversable(NavCell * cell, float x1, float y1, float x2, float y2, float size);

float resize(NavCell * & cell, float x, float y, float s0, float s1);

void navigation(std::unordered_map<NavPoint *, float> & d, std::unordered_map<NavPoint *, NavPoint *> * r, NavCell * cell, NavPoint & end, float size, float heed);

NavPoint * firstCorner(std::unordered_map<NavPoint *, float> & d, NavCell * cell, float x, float y, float size, float heed);

class Navigation : public Task
{
public:
	std::unordered_map<NavPoint *, float> d;
	std::unordered_map<NavPoint *, NavPoint *> r;
	NavCell * endCell;
	NavPoint endPoint;
	float size;
	float heed;
	bool done;
	void init(NavCell * cell, float x, float y, float size, float heed);
	virtual void execute();
};

}
