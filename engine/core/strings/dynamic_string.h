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

#include <algorithm>
#include <cstring>

#include "crown_assert.h"
#include "memory.h"
#include "string_utils.h"
#include "array.h"
#include "string_utils.h"

namespace crown
{

///
class DynamicString
{
public:

	DynamicString(Allocator& allocator = default_allocator());
	DynamicString(const char* s, Allocator& allocator = default_allocator());

	~DynamicString();

	DynamicString& operator+=(const DynamicString& s);
	DynamicString& operator+=(const char* s);
	DynamicString& operator+=(const char c);
	///
	DynamicString& operator=(const DynamicString& s);
	DynamicString& operator=(const char* s);
	DynamicString& operator=(const char c);

	bool operator<(const DynamicString& s) const;
	bool operator==(const DynamicString& s) const;
	bool operator==(const char* s) const;

	// Returns the length of the string.
	uint32_t length() const;

	/// Removes the leading string @a s.
	/// @note
	/// The string must start with @a s.
	void strip_leading(const char* s);

	/// Removes the trailing string @a s.
	/// @note
	/// The string must end with @a s.
	void strip_trailing(const char* s);

	/// Returns whether the string starts with the given @a s string.
	bool starts_with(const char* s) const;

	/// Returns wheterh the string ends with the given @a s string.
	bool ends_with(const char* s) const;

	/// Returns the string hashed to string::murmur2_32.
	StringId32 to_string_id() const;

	///
	const char* c_str() const;

private:

	Array<char> m_string;
};

//-----------------------------------------------------------------------------
inline DynamicString::DynamicString(Allocator& allocator)
	: m_string(allocator)
{
	array::push_back(m_string, '\0');
}

//-----------------------------------------------------------------------------
inline DynamicString::DynamicString(const char* s, Allocator& allocator)
	: m_string(allocator)
{
	if (s != NULL)
	{
		array::push(m_string, s, string::strlen(s));
	}
	array::push_back(m_string, '\0');
}

//-----------------------------------------------------------------------------
inline DynamicString::~DynamicString()
{
}

//-----------------------------------------------------------------------------
inline DynamicString& DynamicString::operator+=(const DynamicString& s)
{
	return *this += s.c_str();
}

//-----------------------------------------------------------------------------
inline DynamicString& DynamicString::operator+=(const char* s)
{
	CE_ASSERT_NOT_NULL(s);
	array::pop_back(m_string);
	array::push(m_string, s, string::strlen(s));
	array::push_back(m_string, '\0');
	return *this;
}

//-----------------------------------------------------------------------------
inline DynamicString& DynamicString::operator+=(const char c)
{
	array::pop_back(m_string);
	array::push_back(m_string, c);
	array::push_back(m_string, '\0');
	return *this;
}

//-----------------------------------------------------------------------------
inline DynamicString& DynamicString::operator=(const DynamicString& s)
{
	m_string = s.m_string;
	return *this;
}

//-----------------------------------------------------------------------------
inline DynamicString& DynamicString::operator=(const char* s)
{
	CE_ASSERT_NOT_NULL(s);
	array::clear(m_string);
	array::push(m_string, s, string::strlen(s));
	array::push_back(m_string, '\0');
	return *this;
}

//-----------------------------------------------------------------------------
inline DynamicString& DynamicString::operator=(const char c)
{
	array::clear(m_string);
	array::push_back(m_string, c);

	return *this;
}

//-----------------------------------------------------------------------------
inline bool DynamicString::operator<(const DynamicString& s) const
{
	return string::strcmp(c_str(), s.c_str()) < 0;
}

//-----------------------------------------------------------------------------
inline bool DynamicString::operator==(const DynamicString& s) const
{
	return string::strcmp(c_str(), s.c_str()) == 0;
}

//-----------------------------------------------------------------------------
inline bool DynamicString::operator==(const char* s) const
{
	CE_ASSERT_NOT_NULL(s);

	return string::strcmp(c_str(), s) == 0;
}

//-----------------------------------------------------------------------------
inline uint32_t DynamicString::length() const
{
	return string::strlen(this->c_str());
}

//-----------------------------------------------------------------------------
inline void DynamicString::strip_leading(const char* s)
{
	CE_ASSERT_NOT_NULL(s);
	CE_ASSERT(starts_with(s), "String does not start with %s", s);

	const size_t my_len = string::strlen(c_str());
	const size_t s_len = string::strlen(s);

	memmove(array::begin(m_string), array::begin(m_string) + s_len, (my_len - s_len));
	array::resize(m_string, my_len - s_len);
	array::push_back(m_string, '\0');
}

//-----------------------------------------------------------------------------
inline void DynamicString::strip_trailing(const char* s)
{
	CE_ASSERT_NOT_NULL(s);
	CE_ASSERT(ends_with(s), "String does not end with %s", s);

	const size_t my_len = string::strlen(c_str());
	const size_t s_len = string::strlen(s);
	array::resize(m_string, my_len - s_len);
	array::push_back(m_string, '\0');
}

//-----------------------------------------------------------------------------
inline bool DynamicString::starts_with(const char* s) const
{
	CE_ASSERT_NOT_NULL(s);
	return string::strncmp(c_str(), s, string::strlen(s)) == 0;
}

//-----------------------------------------------------------------------------
inline bool DynamicString::ends_with(const char* s) const
{
	CE_ASSERT_NOT_NULL(s);

	const size_t my_len = string::strlen(c_str());
	const size_t s_len = string::strlen(s);

	if (my_len >= s_len)
	{
		return string::strncmp(array::begin(m_string) + (my_len - s_len), s, s_len) == 0;
	}

	return false;
}

//-----------------------------------------------------------------------------
inline StringId32 DynamicString::to_string_id() const
{
	return string::murmur2_32(c_str(), length());
}

//-----------------------------------------------------------------------------
inline const char* DynamicString::c_str() const
{
	return array::begin(m_string);
}

} // namespace crown
