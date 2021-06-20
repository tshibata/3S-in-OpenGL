class Hollow
{
public:
	Plane * limits;
	Hollow * * adjacents;
	Scene * (* depiction)(float x, float y, Hollow * hollow);
};

template <typename T> Scene * cue(float x, float y, Hollow * hollow)
{
	return (Scene *) new T(x, y, hollow);
}

