class Plane
{
public:
	float a, b, c, d;
	float distance(float x, float y, float z);
	void intersection(float x, float y, float z, float dx, float dy, float dz, float & ti, float & to, Plane * & pi, Plane * & po);
};

Plane * convexEntry(Plane * p, float x, float y, float z, float dx, float dy, float dz, float & t);

Plane * convexLeave(Plane * p, float x, float y, float z, float dx, float dy, float dz, float & t);

