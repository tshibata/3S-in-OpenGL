void prod(float * a, float * b); // a <- a * b
void prod(float * a, float * b, float * c); // c <- a * b
void rotX(float * mat, float a);
void rotY(float * mat, float a);
void rotZ(float * mat, float a);
void move(float * mat, float x, float y, float z);
void proj(float * mat, float width, float height, float depth);

class Matrix4x4
{
public:
	Matrix4x4();
	float elements[16];
};

class Direction
{
public:
	static Direction * stop;
	Direction();
	virtual void getMatrix(float * matrix) = 0;
	virtual void getInvertedMatrix(float * matrix) = 0;
};

class Move : public Direction
{
private:
	Direction * next;
public:
	float dx, dy, dz;
	Move(Direction * next);
	virtual void getMatrix(float * matrix);
	virtual void getInvertedMatrix(float * matrix);
};

class RotX : public Direction
{
private:
	Direction * next;
public:
	float angle;
	RotX(Direction * next);
	virtual void getMatrix(float * matrix);
	virtual void getInvertedMatrix(float * matrix);
};

class RotY : public Direction
{
private:
	Direction * next;
public:
	float angle;
	RotY(Direction * next);
	virtual void getMatrix(float * matrix);
	virtual void getInvertedMatrix(float * matrix);
};

class RotZ : public Direction
{
private:
	Direction * next;
public:
	float angle;
	RotZ(Direction * next);
	virtual void getMatrix(float * matrix);
	virtual void getInvertedMatrix(float * matrix);
};

class Projection
{
private:
	Direction * direction;
public:
	float width, height, depth;
	Projection(Direction * direction);
	void getMatrix(float * matrix);
};

class Texture
{
public:
	static Texture * last;
	const char * path;
	int id;
	Texture * next;
	Texture(const char * path);
};

class Figure
{
public:
	static Figure * last;
	const char * path;
	int id;
	Figure * next;
	Figure(const char * path);
};

enum BeingType
{
	SOLID_BEING, SCREEN_BEING
};

class Being
{
private:
	Direction * direction;
	Being * prev;
	Being * next;
public:
	static Being * getFirst();
	Being * getNext();
	Being(Direction * direction);
	void getMatrix(float * matrix);
	virtual BeingType getType() = 0;
	virtual Figure * getFigure() = 0;
	virtual Texture * getTexture() = 0;
	virtual ~Being();
};

class Scene
{
public:
	Projection * framing;
	Projection * lighting;
	Scene();
	virtual Scene * rearrange(unsigned int dt, float x, float y) = 0;
	virtual ~Scene();
};

Scene * arrange(float x, float y);

