class Hollow
{
public:
	Plane * limits;
	Hollow * * adjacents;
	sss::Scene * (* depiction)(Hollow * hollow);
};

template <typename T> sss::Scene * cue(Hollow * hollow)
{
	return (sss::Scene *) new T(hollow);
}

Hollow * transit(Hollow * hollow, float x, float y, float & dx, float & dy, bool deflection);

sss::Scene * depict(Hollow * hollow);

