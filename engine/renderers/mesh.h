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

#include "matrix4x4.h"

namespace crown
{

struct SceneGraph;
struct MeshResource;
struct Quaternion;
struct Unit;
struct Vector3;

struct Mesh
{
	Mesh(SceneGraph& sg, int32_t node, const MeshResource* mr);

	/// Returns the local position of the mesh.
	Vector3 local_position() const;

	/// Returns the local rotation of the mesh.	
	Quaternion local_rotation() const;

	/// Returns the local pose of the mesh.
	Matrix4x4 local_pose() const;

	/// Returns the world position of the mesh.
	Vector3 world_position() const;

	/// Returns the world rotation of the mesh.
	Quaternion world_rotation() const;

	/// Returns the world pose of the mesh.
	Matrix4x4 world_pose() const;

	/// Sets the local position of the mesh.
	void set_local_position(Unit* unit, const Vector3& pos);

	/// Sets the local rotation of the mesh.
	void set_local_rotation(Unit* unit, const Quaternion& rot);

	/// Sets the local pose of the mesh.
	void set_local_pose(Unit* unit, const Matrix4x4& pose);

public:

	SceneGraph& m_scene_graph;
	int32_t m_node;
	const MeshResource* m_resource;
};

} // namespace crown
