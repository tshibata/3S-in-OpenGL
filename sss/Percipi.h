#include <atomic>

namespace sss
{

template<typename T> T * & percipiInstance()
{
	static T * instance = nullptr;
	return instance;
}

template<typename T> std::atomic<int> & percipiCounter()
{
	static std::atomic<int> refc(0);
	return refc;
}

template<typename T> class Percipi
{
private:
	T * ptr;
	inline void tie()
	{
		if (ptr != nullptr)
		{
			int count = ++ percipiCounter<T>();
#ifdef DEBUG
			std::cout << "++ " << count << std::endl;
#endif
		}
	}
	inline void cut()
	{
		if (ptr != nullptr)
		{
			int count = -- percipiCounter<T>();
#ifdef DEBUG
			std::cout << "--" << count << std::endl;
#endif
			if (count <= 0)
			{
				delete ptr;
				ptr = nullptr;
				percipiInstance<T>() = nullptr;
			}
		}
	}

public:

	Percipi()
	{
#ifdef DEBUG
		std::cout << "Percipi()" << std::endl;
#endif
		if (percipiInstance<T>() == nullptr)
		{
			percipiInstance<T>() = new T();
		}
		ptr = percipiInstance<T>();
		tie();
	}

	T * operator -> () const
	{
#ifdef DEBUG
		std::cout << "operator -> ()" << std::endl;
#endif
		return ptr;
	}

	~Percipi()
	{
#ifdef DEBUG
		std::cout << "~Percipi()" << std::endl;
#endif
		cut();
	}
};

}
