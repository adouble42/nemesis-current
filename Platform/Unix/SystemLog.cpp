/*
 Copyright (c) 2008 nemesis Developers Association. All rights reserved.

 Governed by the nemesis License 3.0 the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
*/

#include <syslog.h>
#include "Platform/SystemLog.h"

namespace nemesis
{
	void SystemLog::WriteDebug (const string &debugMessage)
	{
		openlog ("truecrypt", LOG_PID, LOG_USER);
		syslog (LOG_DEBUG, "%s", debugMessage.c_str());
		closelog();
	}

	void SystemLog::WriteError (const string &errorMessage)
	{
		openlog ("truecrypt", LOG_PID, LOG_USER);
		syslog (LOG_ERR, "%s", errorMessage.c_str());
		closelog();
	}
}
