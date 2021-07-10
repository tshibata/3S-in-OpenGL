namespace sss
{

extern const int screenWidth;
extern const int screenHeight;

bool initiate();
bool update(unsigned int dt);
void terminate();

void buttonPressed(int pos);
void buttonReleased(int pos);
void wheelMoved(float d);

GLuint initProgram(const GLchar * * vertSource, const GLchar * * fragSource);

unsigned char pixelLabel(float x, float y);

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

