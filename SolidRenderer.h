class SolidRenderer
{
private:
	GLuint program;
public:
	float * framingMatrix;
	float * lightingMatrix;
	sss::Texture * shadowMap;
	SolidRenderer(sss::Texture & shadowMap);
	~SolidRenderer();
	void process();
};

