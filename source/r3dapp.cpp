// Copyright 2005-2011 WaveBreak Software

// r3dapp.cpp -- application info

// includes //

#include"..\\engine\\r3dwin.h"
#include"swanlake.h"

// globals //

// full-screen flag
BOOL g_bFullscreen=TRUE;

// functions //

// ApplicationName()

LPCSTR ApplicationName(void)
{
	// return name
	return("Swan Lake Screensaver");
}

// ApplicationID()

LPCSTR ApplicationID(void)
{
	// return id
	return("74ABE3D7-BAF5-4ac2-B848-C655118023E5");
}

#ifdef SCREENSAVER_MODE
// ApplicationScreensaverID()

LPCSTR ApplicationScreensaverID(void)
{
	// return id-ss
	return("74ABE3D7-BAF5-4ac2-B848-C655118023E5-ss");
}
#endif

// ApplicationIcon()

LPCSTR ApplicationIcon(void)
{
	// return icon resource
	return(MAKEINTRESOURCE(IDI_APPICON));
}

// ApplicationMenu()

LPCSTR ApplicationMenu(void)
{
	// return menu resource
	return(NULL);
}

// ApplicationCursor()

LPCSTR ApplicationCursor(void)
{
	// return cursor resource
	return(NULL);
}

// ApplicationDisplayMode()

DISPLAYMODE ApplicationDisplayMode(void)
{
	// prepare display mode structure
	DISPLAYMODE dm;
	dm.dwX=g_dwHorzRes;
	dm.dwY=g_dwVertRes;
	dm.fmt=(D3DFORMAT)NULL;
	dm.bWindowed=!g_bFullscreen;
	dm.bLockable=FALSE;
	dm.dwBackBufferCount=g_dwBufferCnt;
	
	// return structure
	return(dm);
}

// ApplicationDataPath()

LPCSTR ApplicationDataPath(void)
{
	// return path
#ifdef COMPILE_DATAFILES
	return("data");
#else
	return("");
#endif
}

// ApplicationMasterVolume()

float ApplicationMasterVolume(void)
{
	// return value
	return(g_fMasterVol);
}

// ApplicationPrimaryVolume()

float ApplicationPrimaryVolume(void)
{
	// return value
	return(g_fPrimaryVol);
}

// ApplicationSecondaryVolume()

float ApplicationSecondaryVolume(void)
{
	// return value
	return(g_fSecondaryVol);
}

// ApplicationAlphaRequired()

BOOL ApplicationAlphaRequired(void)
{
	// return value
	return(TRUE);
}

// ApplicationFogRequired()

BOOL ApplicationFogRequired(void)
{
	// return value
	return(g_bFogMode);
}

// ApplicationPriorityMode()

BOOL ApplicationPriorityMode(void)
{
	// return value
	return(g_bPriorityMode);
}

// ApplicationLoadOptions()

BOOL ApplicationLoadOptions(void)
{
	LoadingOutput("ApplicationLoadOptions");

	// load swan lake screensaver options
	return(SwanLakeLoadOptions());
}

// ApplicationInit()

BOOL ApplicationInit(void)
{
	LoadingOutput("ApplicationInit");

	// init swan lake screensaver
	return(SwanLakeInit());
}

// ApplicationLoop()

BOOL ApplicationLoop(void)
{
	// execute swan lake screensaver
	return(SwanLakeExec());
}

// ApplicationPause()

D3DCOLOR ApplicationPause(void)
{
// NOTE: back buffer must be lockable for pause bitmap to function!
	// return filter color for pause bitmap
	return(D3DCOLOR_RGBA(64,128,255,255));
}

// ApplicationResume()

void ApplicationResume(void)
{
}

// ApplicationRelease()

void ApplicationRelease(void)
{
	// release swan lake screensaver
	SwanLakeRelease();
}

// ApplicationToggleFullScreen()

BOOL ApplicationToggleFullScreen(void)
{
#ifndef SCREENSAVER_MODE
	// toggle value
	g_bFullscreen=!g_bFullscreen;

	// allow update
	return(TRUE);
#else
	// no update
	return(FALSE);
#endif
}

#ifdef SCREENSAVER_MODE
// ApplicationConfigureScreensaver()

void ApplicationConfigureScreensaver(HWND hParentWnd)
{
	// call configuration dialog
	SwanLakeConfigure(hParentWnd);
}

// ApplicationScreensaverPreviewImageRes()

LPCSTR ApplicationScreensaverPreviewImageRes(void)
{
	// return resource name
	return(MAKEINTRESOURCE(IDB_SCREENSHOT));
}
#endif

// ApplicationProcessMessage()

BOOL ApplicationProcessMessage(DWORD dwMsg,
							   LPVOID lpData)
{
	// process message
	return(SwanLakeProcessMessage(dwMsg,
								  lpData));
}

#ifdef SHAREWARE_MODE
// ApplicationExecutePurchaseDlg()

BOOL ApplicationExecutePurchaseDlg(void)
{
	// execute dialog
	return(SwanLakeExecutePurchaseDlg());
}

// ApplicationLoadPurchasePage()

BOOL ApplicationLoadPurchasePage(void)
{
	// load page
	return(SwanLakeLoadPurchasePage());
}
#endif

// eof //

// Copyright 2005-2011 WaveBreak Software
