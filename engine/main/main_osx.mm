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

#include "config.h"

#if CROWN_PLATFORM_OSX

#include "device.h"
#include "memory.h"
#include "os_event_queue.h"
#include "thread.h"
#include "main.h"
#include "command_line.h"
#include "disk_filesystem.h"
#include "crown.h"
#include "bundle_compiler.h"
#include "console_server.h"

namespace crown
{

struct MacOSXDevice
{
	MacOSXDevice()
	{ }

	int32_t run(Filesystem* fs, ConfigSettings* cs, CommandLineSettings* cls)
	{
		return 0;
	}

	OsEventQueue _queue;
};

static MacOSXDevice s_mdvc;

bool next_event(OsEvent& ev)
{
  return s_mdvc._queue.pop_event(ev);
}

} // namespace crown

int main(int argc, char** argv)
{
  using namespace crown;

  CommandLineSettings cls = parse_command_line(argc, argv);

  memory_globals::init();
  DiskFilesystem src_fs(cls.source_dir);
  ConfigSettings cs = parse_config_file(src_fs);

  console_server_globals::init();
  console_server_globals::console().init(cs.console_port, cls.wait_console);

  bundle_compiler_globals::init(cls.source_dir, cls.bundle_dir);

  bool do_continue = true;
  int exitcode = EXIT_SUCCESS;

  do_continue = bundle_compiler::main(cls);

  if (do_continue)
  {
    DiskFilesystem dst_fs(cls.bundle_dir);
    exitcode = crown::s_mdvc.run(&dst_fs, &cs, &cls);
  }

  bundle_compiler_globals::shutdown();
  console_server_globals::shutdown();
  memory_globals::shutdown();
  return exitcode;
}

#endif