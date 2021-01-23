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
	Direction();
	virtual void getMatrix(float * matrix) = 0;
	virtual void getInvertedMatrix(float * matrix) = 0;
};

class Stop : public Direction
{
public:
	Stop();
	virtual void getMatrix(float * matrix);
	virtual void getInvertedMatrix(float * matrix);
};

template <typename T> class Move : public Direction
{
public:
	T * const next;
	float dx, dy, dz;
	Move() : next(new T())
	{
	}
	virtual void getMatrix(float * matrix)
	{
		move(matrix, dx, dy, dz);
		next->getMatrix(matrix);
	}
	virtual void getInvertedMatrix(float * matrix)
	{
		next->getInvertedMatrix(matrix);
		move(matrix, - dx, - dy, - dz);
	}
};

template <typename T> class RotX : public Direction
{
public:
	T * const next;
	float angle;
	RotX() : next(new T())
	{
	}
	virtual void getMatrix(float * matrix)
	{
		rotX(matrix, angle);
		next->getMatrix(matrix);
	}
	virtual void getInvertedMatrix(float * matrix)
	{
		next->getInvertedMatrix(matrix);
		rotX(matrix, - angle);
	}
};

template <typename T> class RotY : public Direction
{
public:
	T * const next;
	float angle;
	RotY() : next(new T())
	{
	}
	virtual void getMatrix(float * matrix)
	{
		rotY(matrix, angle);
		next->getMatrix(matrix);
	}
	virtual void getInvertedMatrix(float * matrix)
	{
		next->getInvertedMatrix(matrix);
		rotY(matrix, - angle);
	}
};

template <typename T> class RotZ : public Direction
{
public:
	T * const next;
	float angle;
	RotZ() : next(new T())
	{
	}
	virtual void getMatrix(float * matrix)
	{
		rotZ(matrix, angle);
		next->getMatrix(matrix);
	}
	virtual void getInvertedMatrix(float * matrix)
	{
		next->getInvertedMatrix(matrix);
		rotZ(matrix, - angle);
	}
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

class AbstractBeing
{
private:
	AbstractBeing * prev;
	AbstractBeing * next;
public:
	static AbstractBeing * getFirst();
	AbstractBeing * getNext();
	AbstractBeing();
	void getMatrix(float * matrix);
	virtual Direction * getDirection() = 0;
	virtual BeingType getType() = 0;
	virtual Figure * getFigure() = 0;
	virtual Texture * getTexture() = 0;
	virtual ~AbstractBeing();
};

template <typename T> class DirectedBeing : public AbstractBeing
{
public:
	T * direction;
	DirectedBeing() : direction(new T())
	{
	}
	virtual Direction * getDirection()
	{
		return direction;
	}
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

