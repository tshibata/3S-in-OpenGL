namespace sss
{

class Scene
{
public:
	Scene();
	virtual ~Scene();
	virtual void render() = 0;
	virtual Scene * rearrange(unsigned int dt) = 0;
};

Scene * arrange();

}

