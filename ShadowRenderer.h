class ShadowRenderer
{
private:
	GLuint program;
public:
	float * lightingMatrix;
	Texture * shadowMap;
	ShadowRenderer(Texture & shadowMap);
	~ShadowRenderer();
	void process();
};

