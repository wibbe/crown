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

// Adapted from Branimir Karadžić's config.h (https://github.com/bkaradzic/bx)

#pragma once

#include "platform.h"

#ifndef CE_PIXELS_PER_METER
	#define CE_PIXELS_PER_METER 32
#endif // CE_PIXELS_PER_METER

#ifndef CROWN_DEFAULT_WINDOW_WIDTH
	#define CROWN_DEFAULT_WINDOW_WIDTH 1280
#endif // CROWN_DEFAULT_WINDOW_WIDTH

#ifndef CROWN_DEFAULT_WINDOW_HEIGHT
	#define CROWN_DEFAULT_WINDOW_HEIGHT 720
#endif // CROWN_DEFAULT_WINDOW_HEIGHT

#ifndef CE_MAX_WORLDS
	#define CE_MAX_WORLDS 1024
#endif // CE_MAX_WORLDS

#ifndef CE_MAX_UNITS
	#define CE_MAX_UNITS 65000 // Per world
#endif // CE_MAX_UNITS

#ifndef CE_MAX_CAMERAS
	#define CE_MAX_CAMERAS 16 // Per world
#endif // CE_MAX_CAMERAS

#ifndef CE_MAX_ACTORS
	#define CE_MAX_ACTORS 1024 // Per world
#endif // CE_MAX_ACTORS

#ifndef CE_MAX_CONTROLLERS
	#define CE_MAX_CONTROLLERS 16 // Per world
#endif // CE_MAX

#ifndef CE_MAX_TRIGGERS
	#define CE_MAX_TRIGGERS 1024 // Per world
#endif // CE_MAX

#ifndef CE_MAX_JOINTS
	#define CE_MAX_JOINTS 512 // Per world
#endif // CE_MAX

#ifndef CE_MAX_SOUND_INSTANCES
	#define CE_MAX_SOUND_INSTANCES 64 // Per world
#endif // CE_MAX

#ifndef CE_MAX_RAYCASTS
	#define CE_MAX_RAYCASTS 8 // Per World
#endif // CE_MAX

#ifndef CE_MAX_RAY_INTERSECTIONS
	#define CE_MAX_RAY_INTERSECTIONS 16
#endif // CE_MAX

#ifndef CE_MAX_CAMERA_COMPONENTS
	#define CE_MAX_CAMERA_COMPONENTS 16 // Per unit
#endif // CE_MAX

#ifndef CE_MAX_MESH_COMPONENTS
	#define CE_MAX_MESH_COMPONENTS 16 // Per unit
#endif // CE_MAX

#ifndef CE_MAX_SPRITE_COMPONENTS
	#define CE_MAX_SPRITE_COMPONENTS 16 // Per unit
#endif // CE_MAX

#ifndef CE_MAX_ACTOR_COMPONENTS
	#define CE_MAX_ACTOR_COMPONENTS 16 // Per unit
#endif // CE_MAX

#ifndef CE_MAX_MATERIAL_COMPONENTS
	#define CE_MAX_MATERIAL_COMPONENTS 16 // Per unit
#endif // CE_MAX

#ifndef CE_MAX_CONSOLE_CLIENTS
	#define CE_MAX_CONSOLE_CLIENTS 32
#endif // CE_MAX

#ifndef CE_MAX_GUI_RECTS
	#define CE_MAX_GUI_RECTS 64 // Per Gui
#endif // CE_MAX

#ifndef CE_MAX_GUI_TRIANGLES
	#define CE_MAX_GUI_TRIANGLES 64 // Per Gui
#endif // CE_MAX

#ifndef CE_MAX_GUI_IMAGES
	#define CE_MAX_GUI_IMAGES 64 // Per Gui
#endif // CE_MAX

#ifndef CE_MAX_GUI_TEXTS
	#define CE_MAX_GUI_TEXTS 64 // Per Gui
#endif // CE_MAX

#ifndef CE_MAX_DEBUG_LINES
	#define CE_MAX_DEBUG_LINES 2 * 1024 // Per DebugLine
#endif // CE_MAX

#ifndef CE_MAX_LUA_VECTOR2
	#define CE_MAX_LUA_VECTOR2 4096
#endif // CE_MAX

#ifndef CE_MAX_LUA_VECTOR3
	#define CE_MAX_LUA_VECTOR3 4096
#endif // CE_MAX

#ifndef CE_MAX_LUA_MATRIX4X4
	#define CE_MAX_LUA_MATRIX4X4 4096
#endif // CE_MAX

#ifndef CE_MAX_LUA_QUATERNION
	#define CE_MAX_LUA_QUATERNION 4096
#endif // CE_MAX

#ifndef CROWN_SOUND_OPENAL
	#define CROWN_SOUND_OPENAL (CROWN_PLATFORM_LINUX || CROWN_PLATFORM_WINDOWS)
#endif // CROWN_SOUND_OPENAL

#ifndef CROWN_SOUND_OPENSLES
	#define CROWN_SOUND_OPENSLES (CROWN_PLATFORM_ANDROID)
#endif // CROWN_SOUND_OPENSLES
