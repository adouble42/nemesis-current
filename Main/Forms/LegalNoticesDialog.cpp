/*
 Copyright (c) 2008 nemesis Developers Association. All rights reserved.

 Governed by the nemesis License 3.0 the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
*/

#include "System.h"
#include "LegalNoticesDialog.h"
#include "Main/GraphicUserInterface.h"
#include "Main/Resources.h"

namespace nemesis
{
	LegalNoticesDialog::LegalNoticesDialog (wxWindow* parent) : LegalNoticesDialogBase (parent)
	{
		LegalNoticesTextCtrl->SetMinSize (wxSize (
			Gui->GetCharWidth (LegalNoticesTextCtrl) * 88,
			Gui->GetCharHeight (LegalNoticesTextCtrl) * 28));
		
		Layout();
		Fit();
		Center();

		LegalNoticesTextCtrl->ChangeValue (StringConverter::ToWide (Resources::GetLegalNotices()));
	}
}
