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

#include "types.h"
#include "container_types.h"
#include "math_types.h"
#include "matrix4x4.h"
#include "quaternion.h"
#include "macros.h"
#include "resource.h"

namespace crown
{

class JSONParser;
class DynamicString;
class File;

/// Represents a JSON element.
/// The objects of this class are valid until the parser
/// which has generated them, will exist.
///
/// @ingroup JSON
class JSONElement
{
public:

	/// Construct the nil JSONElement.
	/// Used to forward-instantiate elements or as a special
	/// nil element.
	JSONElement();
	explicit JSONElement(const char* at);
	JSONElement(const JSONElement& other);

	JSONElement& operator=(const JSONElement& other);

	/// Returns the @a i -th item of the current array.
	JSONElement operator[](uint32_t i);

	/// @copydoc JSONElement::operator[]
	JSONElement index(uint32_t i);

	/// Returns the @a i -th item of the current array or
	/// the special nil JSONElement() if the index does not exist.
	JSONElement index_or_nil(uint32_t i);

	/// Returns the element corresponding to key @a k of the
	/// current object.
	/// @note
	/// If the key is not unique in the object scope, the last
	/// key in order of appearance will be selected.
	JSONElement key(const char* k);

	/// Returns the element corresponding to key @a k of the current
	/// object, or the special nil JSONElement() if the key does not exist.
	JSONElement key_or_nil(const char* k);

	/// Returns whether the element has the @a k key.
	bool has_key(const char* k) const;

	/// Returns true wheter the element is the JSON nil special value.
	bool is_nil() const;

	/// Returns true wheter the element is a JSON boolean (true or false).
	bool is_bool() const;

	/// Returns true wheter the element is a JSON number.
	bool is_number() const;

	/// Returns true whether the element is a JSON string.
	bool is_string() const;

	/// Returns true whether the element is a JSON array.
	bool is_array() const;

	/// Returns true whether the element is a JSON object.
	bool is_object() const;

	/// Returns the size of the element based on the
	/// element's type:
	/// * nil, bool, number: 1
	/// * string: length of the string
	/// * array: number of elements in the array
	/// * object: number of keys in the object
	uint32_t size() const;

	/// Returns the boolean value of the element.
	bool to_bool(bool def = false) const;

	/// Returns the integer value of the element.
	int32_t to_int(int32_t def = 0) const;

	/// Returns the float value of the element.
	float to_float(float def = 0) const;

	/// Returns the string value of the element.
	void to_string(DynamicString& str, const char* def = "") const;

	/// Returns the Vector2 value of the element.
	/// @note Vector2 = [x, y]
	Vector2 to_vector2(const Vector2& def = Vector2(0, 0)) const;

	/// Returns the Vector3 value of the element.
	/// @note Vector3 = [x, y, z]
	Vector3 to_vector3(const Vector3& def = Vector3(0, 0, 0)) const;

	/// Returns the Vector4 value of the element.
	/// @note Vector4 = [x, y, z, w]
	Vector4 to_vector4(const Vector4& def = Vector4(0, 0, 0, 0)) const;

	/// Returns the Quaternion value of the element.
	/// @note Quaternion = [x, y, z, w]
	Quaternion to_quaternion(const Quaternion& def = quaternion::IDENTITY) const;

	/// Returns the Matrix4x4 value of the element.
	/// @note Matrix4x4 = [x, x, x, x, y, y, y, y, z, z, z, z, t, t, t, t]
	Matrix4x4 to_matrix4x4(const Matrix4x4& def = matrix4x4::IDENTITY) const;

	/// Returns the string id value hashed to string::murmur2_32() of the element.
	StringId32 to_string_id(const StringId32 def = 0) const;

	/// Returns the resource id value of the element.
	/// If @a type is NULL then the string element is assumed to already contain extension.
	ResourceId to_resource_id(const char* type) const;

	/// Returns the array value of the element.
	/// @note
	/// Calling this function is way faster than accessing individual
	/// array elements by JSONElement::operator[] and it is the very preferred way
	/// for retrieving array elemets. However, you have to be sure that the array
	/// contains only items of the given @array type.
	void to_array(Array<bool>& array) const;

	/// @copydoc JSONElement::to_array(Array<bool>&)
	void to_array(Array<int16_t>& array) const;

	/// @copydoc JSONElement::to_array(Array<bool>&)
	void to_array(Array<uint16_t>& array) const;

	/// @copydoc JSONElement::to_array(Array<bool>&)
	void to_array(Array<int32_t>& array) const;

	/// @copydoc JSONElement::to_array(Array<bool>&)
	void to_array(Array<uint32_t>& array) const;

	/// @copydoc JSONElement::to_array(Array<bool>&)
	void to_array(Array<float>& array) const;

	/// @copydoc JSONElement::to_array(Array<bool>&)
	void to_array(Vector<DynamicString>& array) const;

	/// Returns all the keys of the element.
	void to_keys(Vector<DynamicString>& keys) const;

private:

	const char* m_at;

	friend class JSONParser;
};

/// Parses JSON documents.
///
/// @ingroup JSON
class JSONParser
{
public:

	/// Reads the JSON document contained in the non-null @a s string.
	/// @note
	/// The @a s string has to remain valid for the whole parser's
	/// existence scope.
	JSONParser(const char* s);

	/// Reads the JSON document from file @a f.
	JSONParser(File& f);

	~JSONParser();

	/// Returns the root element of the JSON document.
	JSONElement root();

private:

	bool m_file;
	const char* m_document;

private:

	// Disable copying
	JSONParser(const JSONParser&);
	JSONParser& operator=(const JSONParser&);
};

} // namespace crown
