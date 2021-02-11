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
friend Ref<Direction>;
protected:
	unsigned int refc;
public:
	Direction();
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

class Projection
{
private:
	Ref<Direction> direction;
public:
	float width, height, depth;
	Projection(Direction * direction, float widht, float height, float depth);
	void getMatrix(float * matrix);
};

class Texture
{
public:
	static Texture * last;
	const char * path;
	int id;
	int width, height;
	Texture * next;
	Texture(const char * path);
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

enum BlendMode
{
	SOLID_BLEND, LUCID_BLEND
};

class SpacialFigure : public Figure
{
public:
	BlendMode mode;
	const char * path;
	SpacialFigure(Texture * texture, const char * path, BlendMode mode);
	virtual void getData(unsigned char * * data, size_t * size);
};

class SurficialFigure : public Figure
{
public:
	int x, y, left, top, right, bottom;
	SurficialFigure(Texture * texture, int x, int y, int left, int top, int right, int bottom);
	virtual void getData(unsigned char * * data, size_t * size);
};

template <typename T> class AbstractBeing
{
private:
	static AbstractBeing<T> sentinel;
	AbstractBeing<T> * prev;
	AbstractBeing<T> * next;
	AbstractBeing(AbstractBeing<T> * prev, AbstractBeing<T> * next) : prev(prev), next(next) {}
public:
	static AbstractBeing<T> * getFirst() { return sentinel.next; }
	AbstractBeing * getNext() { if (next == & sentinel) { return nullptr; } else { return next; } }
	AbstractBeing() : AbstractBeing(sentinel.prev, & sentinel) {
		// FIXME make this thread safe
		next->prev = this;
		prev->next = this;
	}
	void getMatrix(float * matrix) { getDirection()->getMatrix(matrix); }
	virtual Direction * getDirection() { return nullptr; }
	virtual T * getFigure() { return nullptr; }
	virtual ~AbstractBeing() {
		// FIXME make this thread safe
		next->prev = prev;
		prev->next = next;
	}
};
template <typename T> AbstractBeing<T> AbstractBeing<T>::sentinel(& sentinel, & sentinel);

template <typename T, typename U> class FiniteBeing : public AbstractBeing<T>
{
public:
	Ref<U> direction;
	FiniteBeing() : direction(new U())
	{
	}
	virtual Direction * getDirection()
	{
		return direction;
	}
};

template <typename T> using SpacialBeing = FiniteBeing<SpacialFigure, T>;

template <typename T> using SurficialBeing = FiniteBeing<SurficialFigure, T>;

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

