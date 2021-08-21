#include <atomic>

namespace sss
{

template<typename T> class ReferenceCounted
{
public:
private:
	T * ptr;
	inline void tie()
	{
		if (ptr != nullptr)
		{
			int count = ++ ptr->refc;
#ifdef DEBUG
			std::cout << "++ " << count << std::endl;
#endif
		}
	}
	inline void cut()
	{
		if (ptr != nullptr)
		{
			int count = -- ptr->refc;
#ifdef DEBUG
			std::cout << "--" << count << std::endl;
#endif
			if (count <= 0)
			{
				delete ptr;
				ptr = nullptr;
			}
		}
	}

public:

	ReferenceCounted() : ptr(nullptr)
	{
#ifdef DEBUG
		std::cout << "ReferenceCounted()" << std::endl;
#endif
	}

	ReferenceCounted(T * p) : ptr(p)
	{
#ifdef DEBUG
		std::cout << "ReferenceCounted(T * p)" << std::endl;
#endif
		tie();
	}

	ReferenceCounted(const ReferenceCounted<T> & r) : ptr(r.ptr)
	{
#ifdef DEBUG
		std::cout << "ReferenceCounted(const ReferenceCounted<T> & r)" << std::endl;
#endif
		tie();
	}

	template<typename U> ReferenceCounted(const ReferenceCounted<U> & r) : ptr((T *) r.ptr)
	{
#ifdef DEBUG
		std::cout << "ReferenceCounted(const ReferenceCounted<U> & r)" << std::endl;
#endif
		tie();
	}

	ReferenceCounted<T> & operator = (T * p)
	{
#ifdef DEBUG
		std::cout << "operator = (T * p)" << std::endl;
#endif
		cut();
		ptr = p;
		tie();
		return * this;
	}

	ReferenceCounted<T> & operator = (const ReferenceCounted<T> & r)
	{
#ifdef DEBUG
		std::cout << "operator = (const ReferenceCounted<T> & r)" << std::endl;
#endif
		cut();
		ptr = (T *) r;
		tie();
		return * this;
	}

	template<typename U> ReferenceCounted<T> & operator = (const ReferenceCounted<U> & r)
	{
#ifdef DEBUG
		std::cout << "operator = (const ReferenceCounted<U> & r)" << std::endl;
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

	bool operator == (const ReferenceCounted<T> & r)
	{
#ifdef DEBUG
		std::cout << "operator == (const ReferenceCounted<T> & r)" << std::endl;
#endif
		return ptr == (T *) r;
	}

	template<typename U> bool operator == (const ReferenceCounted<U> & r)
	{
#ifdef DEBUG
		std::cout << "operator == (const ReferenceCounted<U> & r)" << std::endl;
#endif
		return ptr == (T *) r;
	}

	~ReferenceCounted()
	{
#ifdef DEBUG
		std::cout << "~ReferenceCounted()" << std::endl;
#endif
		cut();
	}
};

template <typename T> using Basis = const ReferenceCounted<T>;

}
