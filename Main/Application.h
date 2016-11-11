/*
 Copyright (c) 2008 nemesis Developers Association. All rights reserved.

 Governed by the nemesis License 3.0 the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Main_AppMain
#define TC_HEADER_Main_AppMain

#include "Main.h"
#include "UserInterface.h"
#include "UserInterfaceType.h"

namespace nemesis
{
	class Application
	{
	public:
		static wxApp* CreateConsoleApp ();
		static wxApp* CreateGuiApp ();
		static FilePath GetConfigFilePath (const wxString &configFileName, bool createConfigDir = false);
		static DirectoryPath GetExecutableDirectory ();
		static FilePath GetExecutablePath ();
		static int GetExitCode () { return ExitCode; }
		static wstring GetName () { return L"nemesis"; }
		static UserInterface *GetUserInterface () { return mUserInterface; }
		static UserInterfaceType::Enum GetUserInterfaceType () { return mUserInterfaceType; }
		static void Initialize (UserInterfaceType::Enum type);
		static void SetExitCode (int code) { ExitCode = code; }

	protected:
		static int ExitCode;
		static UserInterface *mUserInterface;
		static UserInterfaceType::Enum mUserInterfaceType;
	};
}

#endif // TC_HEADER_Main_AppMain
