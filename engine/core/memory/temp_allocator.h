/*
Copyright (c) 2013 Daniele Bartolini, Michele Rossi
Copyright (c) 2012 Daniele Bartolini, Simone Boscaratto

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "allocator.h"
#include "memory.h"

namespace crown
{

/// Allocates from a fixed-size buffer.
/// When the internal memory is exhausted or the allocation size exceeds
/// the remainig internal storage, the backing allocator is used instead.
/// The memory is automatically freed when the allocator is destroyed.
///
/// @ingroup Memory
template <size_t SIZE>
class TempAllocator : public Allocator
{
public:

	/// Uses the @a backing allocator when internal memory is exahusted
	/// or the allocation size exceeds the remaining storage.
	TempAllocator(Allocator& backing = default_allocator());
	~TempAllocator();

	/// @copydoc Allocator::allocate()
	void* allocate(size_t size, size_t align = Allocator::DEFAULT_ALIGN);

	/// Does nothing, the memory is automatically freed when the
	/// allocator is destroyed.
	void deallocate(void* data);

	/// @copydoc Allocator::allocated_size()
	size_t allocated_size();

private:

	Allocator&	_backing;
	
	char* _begin;
	char* _end;
	char* _cur;
	size_t _chunk_size;
	char _buffer[SIZE];
};

typedef TempAllocator<64> TempAllocator64;
typedef TempAllocator<128> TempAllocator128;
typedef TempAllocator<256> TempAllocator256;
typedef TempAllocator<512> TempAllocator512;
typedef TempAllocator<1024> TempAllocator1024;
typedef TempAllocator<2048> TempAllocator2048;
typedef TempAllocator<4096> TempAllocator4096;

template <size_t SIZE>
inline TempAllocator<SIZE>::TempAllocator(Allocator& backing)
	: _backing(backing)
	, _begin(&_buffer[0])
	, _end(&_buffer[SIZE - 1])
	, _cur(&_buffer[0])
	, _chunk_size(4 * 1024)
{
	*(void**) _begin = 0;
	_cur += sizeof(void*);
}

template <size_t SIZE>
inline TempAllocator<SIZE>::~TempAllocator()
{
	union { char* as_char; void** as_dvoid; };
	as_char = _buffer;

	void *p = *(void **)as_dvoid;
	while (p)
	{
		void *next = *(void **)p;
		_backing.deallocate(p);
		p = next;
	}
}

template <size_t SIZE>
inline void* TempAllocator<SIZE>::allocate(size_t size, size_t align)
{
	_cur = (char*) memory::align_top(_cur, align);

	if (size > size_t(_end - _cur))
	{
		uint32_t to_allocate = sizeof(void*) + size + align;

		if (to_allocate < _chunk_size)
		{
			to_allocate = _chunk_size;
		}

		_chunk_size *= 2;

		void *p = _backing.allocate(to_allocate);
		*(void **)_begin = p;
		_cur = _begin = (char*) p;
		_end = _begin + to_allocate;
		*(void**) _begin = 0;
		_cur += sizeof(void*);
		memory::align_top(p, align);
	}

	void *result = _cur;
	_cur += size;
	return result;
}

template <size_t SIZE>
inline void TempAllocator<SIZE>::deallocate(void* /*data*/)
{
}

template <size_t SIZE>
inline size_t TempAllocator<SIZE>::allocated_size()
{
	return 0;
}

} // namespace crown
