/*
 Copyright (c) 2008 nemesis Developers Association. All rights reserved.

 Governed by the nemesis License 3.0 the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Platform_TextReader
#define TC_HEADER_Platform_TextReader

#include "PlatformBase.h"
#include "FileStream.h"
#include "FilesystemPath.h"
#include "SharedPtr.h"
#include "Stream.h"

namespace nemesis
{
	class TextReader
	{
	public:
		TextReader (const FilePath &path);
		TextReader (shared_ptr <Stream> stream) : InputStream (stream) { }
		virtual ~TextReader () { }

		virtual bool ReadLine (string &outputString);

	protected:
		shared_ptr <File> InputFile;
		shared_ptr <Stream> InputStream;
	};
}

#endif // TC_HEADER_Platform_TextReader
