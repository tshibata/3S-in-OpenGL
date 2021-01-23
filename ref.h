template<typename T> class Ref
{
public:
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
			}
		}
	}

public:

	Ref() : ptr(nullptr)
	{
#ifdef DEBUG
		std::cout << "Ref()" << std::endl;
#endif
	}

	Ref(T * p) : ptr(p)
	{
#ifdef DEBUG
		std::cout << "Ref(T * p)" << std::endl;
#endif
		tie();
	}

	Ref(const Ref<T> & r) : ptr(r.ptr)
	{
#ifdef DEBUG
		std::cout << "Ref(const Ref<T> & r)" << std::endl;
#endif
		tie();
	}

	template<typename U> Ref(const Ref<U> & r) : ptr((T *) r.ptr)
	{
#ifdef DEBUG
		std::cout << "Ref(const Ref<U> & r)" << std::endl;
#endif
		tie();
	}

	Ref<T> & operator = (T * p)
	{
#ifdef DEBUG
		std::cout << "operator = (T * p)" << std::endl;
#endif
		cut();
		ptr = p;
		tie();
		return * this;
	}

	Ref<T> & operator = (const Ref<T> & r)
	{
#ifdef DEBUG
		std::cout << "operator = (const Ref<T> & r)" << std::endl;
#endif
		cut();
		ptr = (T *) r;
		tie();
		return * this;
	}

	template<typename U> Ref<T> & operator = (const Ref<U> & r)
	{
#ifdef DEBUG
		std::cout << "operator = (const Ref<U> & r)" << std::endl;
#endif
		cut();
		ptr = (T *) r;
		tie();
		return * this;
	}

	T * operator -> () const
	{
#ifdef DEBUG
		std::cout << "operator -> ()" << std::endl;
#endif
		return ptr;
	}

	operator T * () const
	{
#ifdef DEBUG
		std::cout << "T * ()" << std::endl;
#endif
		return ptr;
	}

	bool operator == (T * p)
	{
#ifdef DEBUG
		std::cout << "operator == (T * p)" << std::endl;
#endif
		return ptr == p;
	}

	bool operator == (const Ref<T> & r)
	{
#ifdef DEBUG
		std::cout << "operator == (const Ref<T> & r)" << std::endl;
#endif
		return ptr == (T *) r;
	}

	template<typename U> bool operator == (const Ref<U> & r)
	{
#ifdef DEBUG
		std::cout << "operator == (const Ref<U> & r)" << std::endl;
#endif
		return ptr == (T *) r;
	}

	~Ref()
	{
#ifdef DEBUG
		std::cout << "~Ref()" << std::endl;
#endif
		cut();
	}
};

