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

#include "crown_assert.h"
#include "socket.h"
#include "os.h"
#include "file.h"

namespace crown
{

/// Access file on a remote file server.
///
/// @ingroup Filesystem
class NetworkFile: public File
{
public:

	/// Reads the file named @a filename from the given @a socket.
	NetworkFile(const NetAddress& addr, uint16_t port, const char* filename);
	virtual ~NetworkFile();

	/// @copydoc File::seek()
	void seek(size_t position);

	/// @copydoc File::seek_to_end()
	void seek_to_end();

	/// @copydoc File::skip()
	void skip(size_t bytes);

	/// @copydoc File::read()
	void read(void* buffer, size_t size);

	/// @copydoc File::write()
	void write(const void* buffer, size_t size);

	/// @copydoc File::copy_to()
	bool copy_to(File& file, size_t size = 0);

	/// @copydoc File::flush()
	void flush();

	/// @copydoc File::end_of_file()
	bool end_of_file();

	/// @copydoc File::is_valid()
	bool is_valid();

	/// @copydoc File::size()
	size_t size();

	/// @copydoc File::position()
	size_t position();

	/// @copydoc File::can_read()
	bool can_read() const;

	/// @copydoc File::can_write()
	bool can_write() const;

	/// @copydoc File::can_seek()
	bool can_seek() const;

private:

	char m_filename[MAX_PATH_LENGTH];
	NetAddress m_address;
	uint16_t m_port;
	TCPSocket m_socket;
	size_t m_position;
};

} // namespace crown
