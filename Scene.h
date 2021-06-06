class Hollow;

class Scene
{
protected:
	Hollow * hollow;
public:
	Scene(Hollow * hollow);
	virtual ~Scene();
	virtual void render() = 0;
	virtual Scene * rearrange(unsigned int dt, float x, float y) = 0;
};

Scene * arrange(float x, float y);

class Hollow
{
public:
	Plane * limits;
	Hollow * * adjacents;
	Scene * (* depiction)(float x, float y, Hollow * hollow);
	Hollow * transit(float x, float y, float & dx, float & dy);
};

template <typename T> Scene * cue(float x, float y, Hollow * hollow)
{
	return (Scene *) new T(x, y, hollow);
}

