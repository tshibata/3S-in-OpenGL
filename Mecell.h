class Mecell;

class Mecell
{
public:
	Plane * limits;
	Mecell * * adjacents;
	Scene * (* depction)(float x, float y, Mecell * mecell);
	Mecell * transit(float x, float y, float & dx, float & dy);
};

template <typename T> Scene * cue(float x, float y, Mecell * mecell)
{
	return (Scene *) new T(x, y, mecell);
}

