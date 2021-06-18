class LucidRenderer
{
private:
	GLuint program;
public:
	float * framingMatrix;
	LucidRenderer();
	~LucidRenderer();
	void process();
};

