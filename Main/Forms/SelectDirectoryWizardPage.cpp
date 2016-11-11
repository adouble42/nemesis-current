/*
 Copyright (c) 2008 nemesis Developers Association. All rights reserved.

 Governed by the nemesis License 3.0 the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
*/

#include "System.h"
#include "Main/GraphicUserInterface.h"
#include "SelectDirectoryWizardPage.h"

namespace nemesis
{
	bool SelectDirectoryWizardPage::IsValid ()
	{
		if (!DirectoryTextCtrl->IsEmpty())
		{
			return FilesystemPath (DirectoryTextCtrl->GetValue().wc_str()).IsDirectory();
		}

		return false;
	}
	
	void SelectDirectoryWizardPage::OnBrowseButtonClick (wxCommandEvent& event)
	{
		DirectoryPath dir = Gui->SelectDirectory (this);

		if (!dir.IsEmpty())
			DirectoryTextCtrl->SetValue (wstring (dir));
	}
}
