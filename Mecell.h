class Mecell;

template <typename T> using Creator = T * (*)(float x, float y, Mecell * mecell);

class Mecell
{
public:
	Plane * limits;
	Mecell * * adjacents;
	Creator<Scene> creator;
	Mecell * transit(float x, float y, float & dx, float & dy);
};

template <typename T, typename U> T * create(float x, float y, Mecell * mecell)
{
	return (T *) new U(x, y, mecell);
}

