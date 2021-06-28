namespace sss
{

template<typename T> T * & percipiInstance()
{
	static T * instance = nullptr;
	return instance;
}

template<typename T> int & percipiCounter()
{
	static int counter = 0;
	return counter;
}

template<typename T> class Percipi
{
private:
	T * ptr;
	inline void tie()
	{
		if (ptr != nullptr)
		{
#ifdef DEBUG
			std::cout << percipiCounter<T>() << "++" << std::endl;
#endif
			percipiCounter<T>()++;
		}
	}
	inline void cut()
	{
		if (ptr != nullptr)
		{
#ifdef DEBUG
			std::cout << percipiCounter<T>() << "--" << std::endl;
#endif
			percipiCounter<T>()--;
			if (percipiCounter<T>() <= 0)
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
