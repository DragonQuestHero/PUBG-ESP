#pragma once

#include <windows.h>
#include "XorStr.h"

class Memory
{
public:
	Memory()
	{
		_This = this;
	}
	~Memory() = default;
public:
	bool Init();
	bool Read_Memory(ULONG64 Addr, void *Buffer, ULONG64 Size);
	bool Write_Memory(ULONG64 Addr, void *Buffer, ULONG64 Size);
	void* Alloc_Memory(ULONG64 Size);
public:
	static Memory *_This;
public:
	template<typename T>
	T Read_Memory(ULONG64 Addr, T t)
	{
		T t2;
		return t2;
	}
private:

};

