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

Hollow * transit(Hollow * hollow, float x, float y, float & dx, float & dy, bool deflection);

