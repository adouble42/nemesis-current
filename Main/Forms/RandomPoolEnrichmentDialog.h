/*
 Copyright (c) 2008-2009 nemesis Developers Association. All rights reserved.

 Governed by the nemesis License 3.0 the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Main_Forms_RandomPoolEnrichmentDialog
#define TC_HEADER_Main_Forms_RandomPoolEnrichmentDialog

#include "Forms.h"
#include "Main/Main.h"

namespace nemesis
{
	class RandomPoolEnrichmentDialog : public RandomPoolEnrichmentDialogBase
	{
	public:
		RandomPoolEnrichmentDialog (wxWindow* parent);
		~RandomPoolEnrichmentDialog ();

	protected:
		void OnHashSelected (wxCommandEvent& event);
		void OnMouseMotion (wxMouseEvent& event);
		void OnShowRandomPoolCheckBoxClicked (wxCommandEvent& event);
		void ShowBytes (wxStaticText *textCtrl, const ConstBufferPtr &buffer);
		
		HashList Hashes;
	};
}

#endif // TC_HEADER_Main_Forms_RandomPoolEnrichmentDialog
