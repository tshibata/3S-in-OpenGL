#include <cassert>
#include <iostream>
#include "ref.h"

static int count = 0;

class Foo
{
friend Ref<Foo>;
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
friend Ref<Bar>;
};

void simpleTest()
{
	{
		Ref<Foo> a = new Foo();
	}
	assert(count == 0);
}

void renewTest()
{
	{
		Ref<Foo> a = new Foo();
		a = new Foo();
	}
	assert(count == 0);
}

void assignTest()
{
	{
		Ref<Foo> a;
		Ref<Foo> b = new Foo();
		a = b;
	}
	assert(count == 0);
}

void assignUpTest()
{
	{
		Ref<Foo> a;
		Ref<Bar> b = new Bar();
		a = b;
	}
	assert(count == 0);
}

void reassignTest()
{
	{
		Ref<Foo> a = new Foo();
		Ref<Foo> b = new Foo();
		a = b;
	}
	assert(count == 0);
}

void reassignUpTest()
{
	{
		Ref<Foo> a = new Foo();
		Ref<Bar> b = new Bar();
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

