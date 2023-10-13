#include "SmartAllocator.h"

#define NEW new ( __FILE__, __LINE__ )

class Memorytracking
{
public:

	// Standard Implementierung of operator new
	// cannot be used in conjunction with memory tracking operator new
	// since we only can use the standard implementation of operator delete
	// which assumes that the object to destroy has additional memory tracking information.
	// if used anyway, it will produce meory leaks that can't be detected
	static void* operator new (size_t size)
	{
		return malloc(size);
	}

	// memory Tracking operator new
	static void* operator new (	size_t size, 
									const char * file, 
									unsigned int line )
	{
		return SmartAllocator::alloc ( size, file, line );
	}

	// Placement operator new
	static void * operator new ( size_t size, void* mem_pointer )
	{
		return mem_pointer;
	}

	// only applicable operator delete
	static void operator delete ( void* pobject )
	{
		SmartAllocator::free ( pobject );
	}

	// illegal overload of operator new with multiple arguments
	inline static void operator delete ( void *, void * )
	{

	}
	
};