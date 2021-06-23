class Hollow
{
public:
	Plane * limits;
	Hollow * * adjacents;
	Scene * (* depiction)(Hollow * hollow);
};

template <typename T> Scene * cue(Hollow * hollow)
{
	return (Scene *) new T(hollow);
}

Hollow * transit(Hollow * hollow, float x, float y, float & dx, float & dy, bool deflection);


