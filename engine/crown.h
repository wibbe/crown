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

#include "filesystem.h"

namespace crown
{
	struct Platform
	{
		enum Enum
		{
			LINUX = 0,
			WINDOWS = 1,
			ANDROID = 2,
			OSX = 3,

			COUNT
		};
	};

	struct ConfigSettings
	{
		uint16_t console_port;
		StringId64 boot_package;
		StringId64 boot_script;
		uint16_t window_width;
		uint16_t window_height;
	};

	struct CommandLineSettings
	{
		const char* source_dir;
		const char* bundle_dir;
		Platform::Enum platform;
		bool wait_console;
		bool do_compile;
		bool do_continue;
		uint32_t parent_window;
	};

	CommandLineSettings parse_command_line(int argc, char** argv);

	/// Read configuration file from @a fs.
	ConfigSettings parse_config_file(Filesystem& fs);

	/// Initializes the engine.
	bool init(Filesystem& fs, const ConfigSettings& cs);

	/// Updates all the subsystems.
	void update();

	/// Shutdowns the engine.
	void shutdown();
} // namespace crown
