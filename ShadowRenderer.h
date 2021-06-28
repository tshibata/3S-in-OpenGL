class ShadowRenderer
{
private:
	GLuint program;
public:
	float * lightingMatrix;
	sss::Texture * shadowMap;
	ShadowRenderer(sss::Texture & shadowMap);
	~ShadowRenderer();
	void process();
};

