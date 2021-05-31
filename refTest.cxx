#include <cassert>
#include <iostream>
#include "Basis.h"

static int count = 0;

class Foo
{
friend ReferenceCounted<Foo>;
protected:
	int refc = 0;
public:
	Foo();
	~Foo();
};

Foo::Foo()
{
	count++;
}

Foo::~Foo()
{
	count--;
}

class Bar : public Foo
{
friend ReferenceCounted<Bar>;
};

void simpleTest()
{
	{
		ReferenceCounted<Foo> a = new Foo();
	}
	assert(count == 0);
}

void renewTest()
{
	{
		ReferenceCounted<Foo> a = new Foo();
		a = new Foo();
	}
	assert(count == 0);
}

void assignTest()
{
	{
		ReferenceCounted<Foo> a;
		ReferenceCounted<Foo> b = new Foo();
		a = b;
	}
	assert(count == 0);
}

void assignUpTest()
{
	{
		ReferenceCounted<Foo> a;
		ReferenceCounted<Bar> b = new Bar();
		a = b;
	}
	assert(count == 0);
}

void reassignTest()
{
	{
		ReferenceCounted<Foo> a = new Foo();
		ReferenceCounted<Foo> b = new Foo();
		a = b;
	}
	assert(count == 0);
}

void reassignUpTest()
{
	{
		ReferenceCounted<Foo> a = new Foo();
		ReferenceCounted<Bar> b = new Bar();
		a = b;
	}
	assert(count == 0);
}

int main(int argc, char * * argv)
{
	simpleTest();
	renewTest();
	assignTest();
	assignUpTest();
	reassignTest();
	reassignUpTest();
	// FIXME not enough
}

