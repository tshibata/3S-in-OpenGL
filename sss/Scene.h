class Hollow;

class Scene
{
protected:
	Hollow * hollow;
public:
	Scene(Hollow * hollow);
	virtual ~Scene();
	virtual void render() = 0;
	virtual Hollow * rearrange(unsigned int dt) = 0;
};

Hollow * arrange();

Scene * depict(Hollow * hollow);

