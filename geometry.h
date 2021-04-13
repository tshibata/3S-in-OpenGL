void prod(float * a, float * b); // a <- a * b
void prod(float * a, float * b, float * c); // c <- a * b
void rotX(float * mat, float a);
void rotY(float * mat, float a);
void rotZ(float * mat, float a);
void move(float * mat, float x, float y, float z);
void expand(float * mat, float sx, float sy, float sz);
void proj(float * mat, float width, float height, float near, float far);

class Matrix4x4
{
public:
	Matrix4x4();
	float elements[16];
};

class Direction
{
friend Ref<Direction>;
protected:
	unsigned int refc;
public:
	Direction();
	virtual ~Direction();
	virtual void getMatrix(float * matrix) = 0;
	virtual void getInvertedMatrix(float * matrix) = 0;
};

class Stop : public Direction
{
friend Ref<Stop>;
public:
	Stop();
	virtual void getMatrix(float * matrix);
	virtual void getInvertedMatrix(float * matrix);
};

template <typename T> class Move : public Direction
{
friend Ref<Move>;
public:
	Ref<T> const next;
	float dx, dy, dz;
	Move() : next(new T()), dx(0), dy(0), dz(0)
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
friend Ref<RotX>;
public:
	Ref<T> const next;
	float angle;
	RotX() : next(new T()), angle(0)
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
friend Ref<RotY>;
public:
	Ref<T> const next;
	float angle;
	RotY() : next(new T()), angle(0)
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
friend Ref<RotZ>;
public:
	Ref<T> const next;
	float angle;
	RotZ() : next(new T()), angle(0)
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

template <typename T> class Expand : public Direction
{
friend Ref<Expand>;
public:
	Ref<T> const next;
	float scale, sx, sy, sz;
	Expand() : next(new T()), scale(1), sx(1), sy(1), sz(1)
	{
	}
	virtual void getMatrix(float * matrix)
	{
		expand(matrix, scale * sx, scale * sy, scale * sz);
		next->getMatrix(matrix);
	}
	virtual void getInvertedMatrix(float * matrix)
	{
		next->getInvertedMatrix(matrix);
		expand(matrix, 1 / (scale * sx), 1 / (scale * sy), 1 / (scale * sz));
	}
};

template <typename T> class Projection
{
friend Ref<Projection>;
private:
	unsigned int refc;
public:
	Ref<T> const direction;
	float width, height, near, far;
	Projection(float width, float height, float near, float far)
	 : direction(new T()), width(width), height(height), near(near), far(far)
	{
	}
	void getMatrix(float * matrix)
	{
		direction->getInvertedMatrix(matrix);
		proj(matrix, width, height, near, far);
	}
};

class Texture
{
public:
	static Texture * last;
	const char * path;
	const int id;
	unsigned int frameBuffer;
	int width, height;
	Texture * next;
	Texture(const char * path);
	Texture(int witdh, int height);
	static int nextId();
};

class Figure
{
public:
	static Figure * last;
	Texture * texture;
	int id;
	Figure * next;
	Figure(Texture * texuture);
	virtual void getData(unsigned char * * data, size_t * size) = 0;
};

class SpacialFigure : public Figure
{
public:
	const char * path;
	SpacialFigure(Texture * texture, const char * path);
	virtual void getData(unsigned char * * data, size_t * size);
};

class SurficialFigure : public Figure
{
public:
	int x, y, left, top, right, bottom;
	SurficialFigure(Texture * texture, int x, int y, int left, int top, int right, int bottom);
	virtual void getData(unsigned char * * data, size_t * size);
};

class RenderingMode;

class AbstractPresence
{
friend RenderingMode;
private:
	RenderingMode * const mode; // TBD no need to be const?
	AbstractPresence * prev;
	AbstractPresence * next;
	AbstractPresence();
public:
	unsigned char label = 0;
	AbstractPresence(RenderingMode & mode);
	virtual ~AbstractPresence();
	AbstractPresence * getNext();
	void getMatrix(float * matrix);
	virtual Direction * getDirection();
	virtual Figure * getFigure();
};

class RenderingMode
{
friend AbstractPresence;
private:
	AbstractPresence sentinel;
public:
	AbstractPresence * getFirst();
};
extern RenderingMode solid2D;
extern RenderingMode solid3D;
extern RenderingMode lucid3D;

template <typename U> class FinitePresence : public AbstractPresence
{
public:
	Ref<U> direction;
	FinitePresence(RenderingMode & mode) : AbstractPresence::AbstractPresence(mode), direction(new U())
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
	Scene();
	virtual ~Scene();
	virtual void render() = 0;
	virtual Scene * rearrange(unsigned int dt, float x, float y) = 0;
};

Scene * arrange(float x, float y);

