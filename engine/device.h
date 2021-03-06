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
#include "config.h"
#include "os.h"
#include "linear_allocator.h"
#include "resource.h"
#include "world_types.h"

namespace crown
{

class Bundle;
class Filesystem;
struct LuaEnvironment;
class ResourceManager;
struct ResourcePackage;
class World;
class WorldManager;
struct Camera;

/// @defgroup Device Device

/// Holds data for a display mode.
///
/// @ingroup Device
struct DisplayMode
{
	uint32_t id;
	uint16_t width;
	uint16_t height;
};

/// This is the place where to look for accessing all of
/// the engine subsystems and related stuff.
///
/// @ingroup Device
struct Device
{
	Device(Filesystem& fs, StringId64 boot_package, StringId64 boot_script);

	void init();

	/// Shutdowns the engine freeing all the allocated resources
	void shutdown();

	/// Returns a string identifying what platform the engine is running on.
	const char* platform() const { return CROWN_PLATFORM_NAME; }

	/// Returns a string identifying what architecture the engine is running on.
	const char* architecture() const { return CROWN_ARCH_NAME; }

	/// Returns a string identifying the engine version.
	const char* version() const { return CROWN_VERSION_MAJOR "." CROWN_VERSION_MINOR "." CROWN_VERSION_MICRO; }

	/// Returns wheter the engine is running (i.e. it is advancing
	/// the simulation).
	bool is_running() const;

	/// Return the number of frames rendered from the first
	/// call to Device::start()
	uint64_t frame_count() const;

	/// Returns the time in seconds needed to render the last frame
	float last_delta_time() const;

	/// Returns the time in seconds since the first call to start().
	double time_since_start() const;

	/// Quits the application.
	void quit();

	/// Pauses the engine
	void pause();

	/// Unpauses the engine
	void unpause();

	void update_resolution(uint16_t width, uint16_t height)
	{
		_width = width;
		_height = height;
	}

	/// Returns the main window resolution.
	void resolution(uint16_t& width, uint16_t& height)
	{
		width = _width;
		height = _height;
	}

	/// Updates all the subsystems
	void update();

	/// Renders the given @a world from the point of view of the given @a camera.
	void render_world(World* world, Camera* camera);

	/// Creates a new world.
	WorldId create_world();

	/// Destroys the given @a world.
	void destroy_world(WorldId world);

	/// Returns the resource package with the given @a package_name name.
	ResourcePackage* create_resource_package(const char* name);
	ResourcePackage* create_resource_package(StringId64 id);

	/// Destroy a previously created resource @a package.
	/// @note
	/// To unload the resources loaded by the package, you have to call
	/// ResourcePackage::unload() first.
	void destroy_resource_package(ResourcePackage* package);

	void reload(const char* type, const char* name);

	ResourceManager* resource_manager();
	LuaEnvironment* lua_environment();
	WorldManager* world_manager() { return _world_manager; }

private:

	// Used to allocate all subsystems
	LinearAllocator _allocator;

	uint16_t _width;
	uint16_t _height;

	bool _is_init		: 1;
	bool _is_running	: 1;
	bool _is_paused		: 1;

	uint64_t _frame_count;
	int64_t _last_time;
	int64_t _current_time;
	float _last_delta_time;
	double _time_since_start;

	Filesystem& _fs;
	StringId64 _boot_package_id;
	StringId64 _boot_script_id;
	ResourcePackage* _boot_package;

	LuaEnvironment* _lua_environment;
	ResourceManager* _resource_manager;
	Bundle* _resource_bundle;
	WorldManager* _world_manager;

private:

	// Disable copying
	Device(const Device&);
	Device& operator=(const Device&);
};

namespace device_globals
{
	void init(Filesystem& fs, StringId64 boot_package, StringId64 boot_script);
	void shutdown();
} // namespace device_globals

Device* device();

} // namespace crown
