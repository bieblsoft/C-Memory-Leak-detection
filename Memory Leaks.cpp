#include <iostream>
using namespace std;
#include <vector>
#include "Memorytracking.h"

class A : public Memorytracking
{
	const int Number = 4711;
};

class B : public A
{
};

class C : public Memorytracking
{
	const int Number = 1234;
};

void main ()
{

	A* pA = NEW A();
	B* pB = NEW B();
	C* pC = NEW C();

	// using a vector will only be valid if we overload the placement operator new
	// ( depending on the Compiler manufacturer, since some use the placement operator new 
	//	 to put new objects into the vector, since the vector already may hold some memory )
	std::vector<C> v;
	v.reserve(10 * sizeof(C));
	size_t cap = v.capacity();
	size_t max = v.max_size();
	v.push_back(*pC);

	delete pA;
	delete pB;

	/*
		using the standard implementation of operator new results in a memory leak
		which we cannot detect using our smartallocator since we only have one single implementation
		of the delete operator. 
		using our overloaded delete operator results in a problem since objects created using the 
		standard operator new do not have the expected file/line information 
	*/
	A* pX = new A();

	cout << endl;
	cout << "-----------------" << endl;
	cout << "Memory leak Info: " << endl;
	SmartAllocator::report();
	cout << "-----------------" << endl;

}

