#include "MyDialog.h"


void DisplayMyDialog(mv _far *mV, LPEDATA edPtr)
{
	int result = MessageBox(mV->mvHMainWin, "Ultimate Fullscreen Object v1.1\nOriginally by Sebastian Janisz (Ultimate Walrus)\nUnofficial update by NaitorStudios\n\
For questions/comments/suggestions/bug reports, Discord: CesarFernandes#2737.\n\n\
To work properly, this object requires that the following application properties be set:\n\
\"Resize display to fill window size\" - YES\n\
\"Heading when maximized\" - NO\n\
\"Menu bar\" - NO\n\
Would you like these properties to be automatically set now?", "Ultimate Fullscreen", MB_YESNO);



	if(result == IDYES)
	{
		mvSetAppPropCheck(mV, edPtr, PROPID_APP_RESIZETOWINDOWSIZE, TRUE);
		mvSetAppPropCheck(mV, edPtr, PROPID_APP_HEADINGMAX, FALSE);
		mvSetAppPropCheck(mV, edPtr, PROPID_APP_MENUBAR, FALSE);
	}

}