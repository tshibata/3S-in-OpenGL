class SolidRenderer
{
private:
	GLuint program;
public:
	float * framingMatrix;
	float * lightingMatrix;
	Texture * shadowMap;
	SolidRenderer(Texture & shadowMap);
	~SolidRenderer();
	void process();
};

