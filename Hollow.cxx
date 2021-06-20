#define NDEBUG
#include <cassert>

#include "Plane.h"
#include "Scene.h"
#include "Hollow.h"

Hollow * transit(Hollow * curr, float x, float y, float & dx, float & dy, bool deflection)
{
	float t = 0;
	while (true)
	{
		Plane * p = convexLeave(curr->limits, x, y, 0, dx, dy, 0, t);
		if (1 <= t)
		{
			return curr;
		}
		else
		{
			assert(p != nullptr);
			for (int i = 0;; i++)
			{
				Hollow * next = curr->adjacents[i];

				if (next == nullptr)
				{
					// no adjacent to go
					if (! deflection)
					{
						return nullptr;
					}
					float u = p->distance(x + dx, y + dy, 0);
					dx -= 1.01f * u * p->a;
					dy -= 1.01f * u * p->b;
					t = 0;
					convexLeave(curr->limits, x, y, 0, dx, dy, 0, t);
					if (t < 1)
					{
						return transit(curr, x, y, dx, dy, deflection);
					}
					return curr;
				}

				float t2;
				t2 = t;
				Plane * p2 = convexEntry(next->limits, x, y, 0, dx, dy, 0, t2);
				if (t2 == t)
				{
					// go into one of adjacents
					curr = next;
					break;
				}
			}
		}
	}
}

