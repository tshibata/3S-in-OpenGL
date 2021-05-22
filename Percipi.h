template<typename T> class Res
{
private:
	T * ptr;
	inline void tie()
	{
		if (ptr != nullptr)
		{
#ifdef DEBUG
			std::cout << ptr->refc << "++" << std::endl;
#endif
			ptr->refc++;
		}
	}
	inline void cut()
	{
		if (ptr != nullptr)
		{
#ifdef DEBUG
			std::cout << ptr->refc << "--" << std::endl;
#endif
			ptr->refc--;
#ifdef DEBUG
			if (ptr->refc <= 0)
#else
			if (ptr->refc == 0) // refc can be unsigned
#endif
			{
				delete ptr;
				ptr = nullptr;
				T::singleton = nullptr;
			}
		}
	}

public:

	Res()
	{
#ifdef DEBUG
		std::cout << "Res()" << std::endl;
#endif
		if (T::singleton == nullptr)
		{
			T::singleton = new T();
		}
		ptr = T::singleton;
		tie();
	}

	T * operator -> () const
	{
#ifdef DEBUG
		std::cout << "operator -> ()" << std::endl;
#endif
		return ptr;
	}

	~Res()
	{
#ifdef DEBUG
		std::cout << "~Res()" << std::endl;
#endif
		cut();
	}
};

