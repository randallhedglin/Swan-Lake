// Copyright 2005-2009 WaveBreak Software

// r3dapp.h -- application info header

// see "r3dwin.h" for debug flag

// globals //

// full-screen flag
extern BOOL g_bFullscreen;

// prototypes //

// application info functions

extern LPCSTR ApplicationName(void);

extern LPCSTR ApplicationID(void);

extern LPCSTR ApplicationIcon(void);

extern LPCSTR ApplicationMenu(void);

extern LPCSTR ApplicationCursor(void);

extern DISPLAYMODE ApplicationDisplayMode(void);

extern LPCSTR ApplicationDataPath(void);

extern float ApplicationMasterVolume(void);

extern float ApplicationPrimaryVolume(void);

extern float ApplicationSecondaryVolume(void);

extern BOOL ApplicationAlphaRequired(void);

extern BOOL ApplicationFogRequired(void);

extern BOOL ApplicationPriorityMode(void);

// application execution functions

extern BOOL ApplicationLoadOptions(void);

extern BOOL ApplicationInit(void);

extern BOOL ApplicationLoop(void);

extern D3DCOLOR ApplicationPause(void);

extern void ApplicationResume(void);

extern void ApplicationRelease(void);

extern BOOL ApplicationToggleFullScreen(void);

#ifdef SCREENSAVER_MODE
extern void ApplicationConfigureScreensaver(HWND hParentWnd);

extern LPCSTR ApplicationScreensaverPreviewImageRes(void);

extern LPCSTR ApplicationScreensaverID(void);
#endif

extern BOOL ApplicationProcessMessage(DWORD dwMsg,
									  LPVOID lpData=NULL);

#ifdef SHAREWARE_MODE
extern BOOL ApplicationExecutePurchaseDlg(void);

extern BOOL ApplicationLoadPurchasePage(void);
#endif

// eof //

// 3D Canvas username: Randall Hedglin
// 3D Canvas reg code: VLJHCVAICQGZ

// SI_id (200003041): V4urMEi4 
// Comodo CC: oI855Xa0jzlRzBub

// WinZIP username: Randall Hedglin 
// WinZIP reg code: 11158953

// WinZIP SE username: WaveBreak Software 
// WinZIP SE reg code: 164351

// Copyright 2005-2009 WaveBreak Software
