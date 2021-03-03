// Copyright 2005-2007 WaveBreak Software

// r3dwin.cpp -- engine core

// includes //

#include"r3dwin.h"

// initialize globals //

// handle to application window
HWND g_hWnd=(HWND)INVALID_HANDLE_VALUE;
// handle to application instance
HINSTANCE g_hInst=(HINSTANCE)INVALID_HANDLE_VALUE;
// handle to duplication mutex
HANDLE g_hMutex=INVALID_HANDLE_VALUE;
// paused flag
BOOL g_bIdle=FALSE;
// closing flags
BOOL g_bClosing=FALSE;
#ifdef SHAREWARE_MODE
BOOL g_bPreClose=FALSE;
BOOL g_bCloseOK=FALSE;
BOOL g_bPurchPage=FALSE;
#endif
// display modes
DISPLAYMODE g_dmApp;
DISPLAYMODE g_dmSys;
DISPLAYMODE g_dmOrigSys;
// window size
int g_nXSize=0;
int g_nYSize=0;
// window position
int g_nXPos=0;
int g_nYPos=0;
// frame skip (must be 1)
DWORD g_dwFrameSkip=1;
// frames per second
DWORD g_dwFPS=0;
// updates per second
DWORD g_dwUPS=0;
// application timer
DWORD g_dwAppTimer=0;
// timer counters
DWORD g_dwNumTimer=0;
DWORD g_dwMaxTimer=0;
// timer data
LPDWORD g_lpdwTimerLast=NULL;
LPDWORD g_lpdwTimerInc=NULL;
// random number tables (disabled)
//float* g_lpfRand=NULL;
//int* g_lpnRand=NULL;
// resource counters
DWORD g_dwNumCOMRes=0;
DWORD g_dwNumWinRes=0;
DWORD g_dwNumMemRes=0;
DWORD g_dwNumClassRes=0;
// message strings
char g_lpMsg[MSG_SIZE];
char g_lpErrorMsg[MSG_SIZE*2];
WCHAR g_lpWMsg[MSG_SIZE];
// registry keys
HKEY g_hSoftKey=(HKEY)INVALID_HANDLE_VALUE;
HKEY g_hAppKey=(HKEY)INVALID_HANDLE_VALUE;
// script file handles
#ifdef DEBUG_MODE
HANDLE g_hScriptFile=INVALID_HANDLE_VALUE;
#endif
HANDLE g_hCScriptFile=INVALID_HANDLE_VALUE;
// script file data
LPBYTE g_lpCScriptData=NULL;
DWORD g_dwCScriptDataPtr=0;
// system mouse info
BOOL g_bWinMouse=FALSE;
int g_nMouseX=0;
int g_nMouseY=0;
int g_nMousePX=0;
int g_nMousePY=0;
int g_nMouseDX=0;
int g_nMouseDY=0;
BOOL g_bMouseBtnL=FALSE;
BOOL g_bMouseBtnR=FALSE;
BOOL g_bMousePBtnL=FALSE;
BOOL g_bMousePBtnR=FALSE;
BOOL g_bMouseClkL=FALSE;
BOOL g_bMouseClkR=FALSE;
#ifdef DEBUG_MODE
BOOL g_bMouseBtnS=FALSE;
BOOL g_bMouseBtnC=FALSE;
BOOL g_bMousePBtnS=FALSE;
BOOL g_bMousePBtnC=FALSE;
BOOL g_bMouseClkS=FALSE;
BOOL g_bMouseClkC=FALSE;
#endif
DWORD g_dwMouseSkip=0;
// dialog box counters
DWORD g_dwNumDlg=0;
DWORD g_dwMaxDlg=0;
// dialog box data
LPBYTE* g_lplpDlgData=NULL;
LPDWORD g_lpdwDlgDataSize=NULL;
LPDWORD g_lpdwDlgDataPtr=NULL;
HWND g_hCurrDlg=(HWND)INVALID_HANDLE_VALUE;
int* g_lpnDlgResID=NULL;
// accelerator keys
HACCEL g_hAccel=(HACCEL)INVALID_HANDLE_VALUE;
ACCEL g_acAltEnter;
// pause bitmap data
BITMAPINFOHEADER g_bih;
LPBYTE g_lpPauseBmpData=NULL;
// data file counters
DWORD g_dwNumDataFileRef=0;
DWORD g_dwMaxDataFileRef=0;
// data file data
HANDLE g_hDataFile=INVALID_HANDLE_VALUE;
LPBYTE g_lpDataFile=NULL;
#ifdef DEBUG_MODE
LPSTR* g_lplpDataFileRefName=NULL;
LPBOOL g_lpbDataFileRefUsed=NULL;
#endif
DWORD g_dwDataFileOffsetPos=0;
DWORD g_dwDataFileSizePos=0;
DWORD g_dwDataFileDataPos=0;
// COM flag
BOOL g_bCOMReady=FALSE;
// initialized flag
BOOL g_bAppInit=FALSE;
#ifdef SHAREWARE_MODE
// nybble codes
char g_lpNybCode[64];
#endif
// activity flags
BOOL g_bMouseMove=FALSE;
BOOL g_bKeyPress=FALSE;
// anti-hacking values
DWORD g_dwAHVal1=NULL;
DWORD g_dwAHVal2=NO_OBJECT;
DWORD g_dwAHVal3=NULL;
DWORD g_dwAHVal4=NULL;
// wait-cursor counter
int g_nWaitCursorCnt=0;
#ifdef DEBUG_MODE
// message locking flags
BOOL _g_bMsgLocked=FALSE;
#endif

// functions //

// WinMain()

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nShowCmd)
{
	// copy instance handle
	g_hInst=hInstance;

#ifdef SCREENSAVER_MODE
	// process screensaver flags
	if(!ProcessScreensaverFlags(lpCmdLine))
	{
		// confirm release of all resources
		ConfirmResourcesReleased();

		// return to os
		return(0);
	}
#endif

ANTIHACK_1

#ifndef ALLOW_MULTIPLE_INSTANCES
	LockGlobalMsg()

	// copy application name
	strcpy(g_lpMsg,ApplicationName());

#ifdef SHAREWARE_MODE
	// append "trial version" to name
	strcat(g_lpMsg," (Trial Version)");
#endif

	// prevent duplicate instances
	if(!LockApplication(NULL,
						g_lpMsg,
						(LPSTR)ApplicationID(),
						FALSE))
	{
		UnlockGlobalMsg()
		return(0);
	}

	UnlockGlobalMsg()
#endif

	// initialize & execute application
	if(InitializeAllModules())
	{
		// set initialized flag
		g_bAppInit=TRUE;

#ifdef SCREENSAVER_MODE
		// set mouse skip value
		g_dwMouseSkip=10;
#endif

		// execute application
		PrimaryMessageLoop();

		// reset initialized flag
		g_bAppInit=FALSE;
	}

#ifdef SHAREWARE_MODE
	// check purchase page flag
	if(g_bPurchPage)
		ApplicationLoadPurchasePage();
#endif

	// release all allocated resources
	ReleaseAllModules();

#ifndef ALLOW_MULTIPLE_INSTANCES
	// release duplication lock
	UnlockApplication();
#endif

	// confirm release of all resources
	ConfirmResourcesReleased();

	// return to os
	return(0);
}

// WindowProc()

LRESULT CALLBACK WindowProc(HWND hWnd,
							UINT Msg,
							WPARAM wParam,
							LPARAM lParam)
{
	// device context
	HDC hDC;

	// paint structure
	PAINTSTRUCT ps;
	
	// check for messages that we are interested in
	switch(Msg)
	{
	// process activate message
	case(WM_ACTIVATE):

		// ignore until initialized
		if(!g_bAppInit)
			return(NULL);

		// check new state
		if(LOWORD(wParam)==WA_INACTIVE)
#ifdef SCREENSAVER_MODE
			SendCloseMessage();
#else
			EnterIdleMode();
		else
			ExitIdleMode();
#endif
		
		// ok
		return(NULL);
		
	// process close message
	case(WM_CLOSE):

		// set flag
		g_bClosing=TRUE;
		
		// quit application
		PostMessage(g_hWnd,
					WM_QUIT,
					NULL,
					NULL);

		// ok
		return(NULL);

	// display mode changed
	case(WM_DISPLAYCHANGE):

		// ignore until initialized
		if(!g_bAppInit)
			return(NULL);

		// update mode if idle
		if(g_bIdle)
			SetSystemDisplayMode((DWORD)LOWORD(lParam),
								 (DWORD)HIWORD(lParam));
		
		// ok
		return(NULL);

	// process enable message
	case(WM_ENABLE):

		// ignore until initialized
		if(!g_bAppInit)
			return(NULL);

		// check flag
		if(!wParam)
#ifdef SCREENSAVER_MODE
			SendCloseMessage();
#else
			EnterIdleMode();
		else
			ExitIdleMode();
#endif
		
		// ok
		return(NULL);
		
	// user has ended session
	case(WM_ENDSESSION):

		// check flags
		if((BOOL)wParam==TRUE||
		   (BOOL)lParam==TRUE)
			ForceImmediateExit();
		
		// ok
		return(NULL);

	// focus is lost
	case(WM_ENTERMENULOOP):
	case(WM_ENTERSIZEMOVE):
	case(WM_KILLFOCUS):

		// ignore until initialized
		if(!g_bAppInit)
			return(NULL);

#ifdef SCREENSAVER_MODE
		// close screensaver
		SendCloseMessage();
#else
		// enter idle
		EnterIdleMode();
#endif

		// ok
		return(NULL);

	// system entering standby
	case(WM_POWERBROADCAST):

		// check flag
		if((DWORD)wParam==PBT_APMSUSPEND)
		{
#ifdef SCREENSAVER_MODE
			// close screensaver
			ForceImmediateExit();
#else
			// enter idle
			EnterIdleMode();
#endif
			
			// ok
			return(TRUE);
		}
		else if((DWORD)wParam==PBT_APMQUERYSUSPEND)
		{
			// ok
			return(TRUE);
		}

		// ok
		break;
		
	// focus is restored
	case(WM_EXITMENULOOP):
	case(WM_EXITSIZEMOVE):
	case(WM_SETFOCUS):

		// ignore until initialized
		if(!g_bAppInit)
			return(NULL);

		// exit idle
		ExitIdleMode();

		// ok
		return(NULL);

#ifdef FLASH_WHEN_MINIMIZED
	// timer message received
	case(WM_TIMER):

		// check id
		if((UINT)wParam==FLASH_TIMER_ID)
		{
			// flash window
			FlashWindow(g_hWnd,
						TRUE);
			
			// ok
			return(NULL);
		}
		
		// ok
		break;
#endif

	// command message
	case(WM_COMMAND):

		// check identifier
		switch(LOWORD(wParam))
		{
		// toggle full-screen command
		case(IDM_TOGGLEFULLSCREEN):
			
			// toggle if necessary
			if(ApplicationToggleFullScreen()&&
			   g_bWindowedOK)
				ResetRenderDevice();

			// ok
			return(NULL);
		}
		
		// ok
		break;

	// paint message
	case(WM_PAINT):

		// begin paint
		hDC=BeginPaint(g_hWnd,
					   &ps);

		// add resource
		AddWinResource;
		
		// check value
		if(hDC)
		{
			// update client area
			UpdateClientArea();
			
			// display bitmap if available
			if(g_lpPauseBmpData)
			{
				// copy bitmap
				SetDIBitsToDevice(hDC,
								  0,0,
								  g_rClient.dwX,
								  g_rClient.dwY,
								  0,0,
								  0,
								  g_rClient.dwY,
								  (LPVOID)g_lpPauseBmpData,
								  (LPBITMAPINFO)&g_bih,
								  DIB_RGB_COLORS);
			}
			else
			{
				// select black brush
				SelectObject(hDC,
							 GetStockObject(BLACK_BRUSH));

				// paint window
				PatBlt(hDC,
					   0,0,
					   (int)g_rClient.dwX,
					   (int)g_rClient.dwY,
					   PATCOPY);

				// de-select object
				SelectObject(hDC,
							 NULL);
			}
		}

		// end paint
		EndPaint(g_hWnd,
				 &ps);
		
		// remove resource
		RemoveWinResource;

		// ok
		return(NULL);

	// key press
	case(WM_KEYDOWN):
	case(WM_SYSKEYDOWN):
		
		// ignore until initialized
		if(!g_bAppInit)
			return(NULL);

		// set flag
		g_bKeyPress=TRUE;

#ifdef SCREENSAVER_MODE
		// scope for static variables
		{
			// code counters
			static DWORD dwSpeedCode=0;
			static DWORD dwFrameCode=0;
			
			// save key value
			int nVirtKey=(int)wParam;

			// check key value
			if     (dwSpeedCode==0&&nVirtKey=='S') dwSpeedCode=1;
			else if(dwSpeedCode==1&&nVirtKey=='P') dwSpeedCode=2;
			else if(dwSpeedCode==2&&nVirtKey=='E') dwSpeedCode=3;
			else if(dwSpeedCode==3&&nVirtKey=='E') dwSpeedCode=4;
			else if(dwSpeedCode==4&&nVirtKey=='D')
			{
				// reset counter
				dwSpeedCode=0;

				// toggle flag
				ApplicationProcessMessage(2); //swanlake.h::MSG_TOGGLESPEEDMODE
			}
			else if(dwFrameCode==0&&nVirtKey=='F') dwFrameCode=1;
			else if(dwFrameCode==1&&nVirtKey=='R') dwFrameCode=2;
			else if(dwFrameCode==2&&nVirtKey=='A') dwFrameCode=3;
			else if(dwFrameCode==3&&nVirtKey=='M') dwFrameCode=4;
			else if(dwFrameCode==4&&nVirtKey=='E')
			{
				// reset counter
				dwFrameCode=0;

				// toggle flag
				ApplicationProcessMessage(1); // swanlake.h::MSG_TOGGLEFRAMERATE
			}
			else
				SendCloseMessage();
		}
#endif

		// ok
		return(NULL);

	// mouse events
	case(WM_MOUSEMOVE):

		// set flag
		g_bMouseMove=TRUE;

		// process mouse if necessary
		if(g_bWinMouse)
		{
			// get positions
			float fX=(float)(LOWORD(lParam));
			float fY=(float)(HIWORD(lParam));

			// set scaled positions
			g_nMouseX=(int)((fX/g_rClient.fX)*640.0F);
			g_nMouseY=(int)((fY/g_rClient.fY)*480.0F);

			// ok
			return(NULL);
		}

		// ok
		break;

	// system command
	case(WM_SYSCOMMAND):

		// check command
		switch(wParam&0xFFF0)
		{
		// screensaver activate
		case(SC_SCREENSAVE):

			// prevent screensaver activation
			return(NULL);

#ifdef SCREENSAVER_MODE
		// alt-tab & close commands
		case(SC_NEXTWINDOW):
		case(SC_PREVWINDOW):
		case(SC_CLOSE):

			// prevent window change
			return(NULL);
#endif		
		}

		// ok
		break;
	}

	// let windows process all other messages
	return(DefWindowProc(hWnd,Msg,wParam,lParam));
}

// PrimaryMessageLoop()

void PrimaryMessageLoop(void)
{
	// frame target tick count
	DWORD dwFrameTickCount;

	// frame carry-over
	DWORD dwFrameCarryOver;
	
	// fps target tick count
	DWORD dwFPSTickCount;
	
	// fps frame counter
	DWORD dwFPSFrameCount;
	
	// ups frame counter
	DWORD dwUPSFrameCount;
	
	// timer refresh flag
	BOOL bTimerRefresh=TRUE;

	// current tick count
	DWORD dwTickCount;

	// loop until function returns
	while(TRUE)
	{
		// process pending messages
		if(!ProcessPendingMessages())
			return;
			
		// do nothing if device is lost
		if(FAILED(g_lpD3DDevice->TestCooperativeLevel()))
		{
			// attempt to recover device
			ResetRenderDevice();

			// reset timers
			bTimerRefresh=TRUE;
		}
		else
		{
			// check execution flags
			if(!g_bIdle&&
			   !g_bClosing)
			{
				// refresh timers if necessary
				if(bTimerRefresh)
				{
					// get tick count
					dwTickCount=GetTickCount();
					
					// refresh tick counts
					dwFrameTickCount=dwTickCount+MSEC_PER_FRAME;
					dwFPSTickCount=dwTickCount+MSEC_PER_SEC;

					// reset other counters
					dwFrameCarryOver=0;
					dwFPSFrameCount=0;
					dwUPSFrameCount=0;
					g_dwFrameSkip=1;
					g_dwFPS=0;
					g_dwUPS=0;

					// reset timer reset flag
					bTimerRefresh=FALSE;
				}
				
				// do not allow large frame skips
				if(g_dwFrameSkip>MAX_FRAME_SKIP)
					g_dwFrameSkip=MAX_FRAME_SKIP;

				// call application loop if necessary
				if(g_dwFrameSkip)
				{
					// process mouse
					ProcessMouseTracking();

					// execute application
					if(!ApplicationLoop())
						SendCloseMessage();

					// reset activity flags
					g_bMouseMove=FALSE;
					g_bKeyPress=FALSE;

#ifdef SCREENSAVER_MODE
					// check skip counter
					if(g_dwMouseSkip)
					{
						// decrement counter
						g_dwMouseSkip--;
					}
					else
					{
						// check for mouse movement
						if(abs(g_nMouseDX)>3||
						   abs(g_nMouseDY)>3||
						   g_bMouseBtnL||
						   g_bMouseBtnR)
							SendCloseMessage();
					}
#endif

					// increment counters
					dwFPSFrameCount++;
					dwUPSFrameCount+=g_dwFrameSkip;
				}

				// save current tick count
				dwTickCount=GetTickCount();
				
				// perform timing tasks
				if(dwTickCount<dwFrameTickCount)
				{
					// do not advance until frame tick
					g_dwFrameSkip=0;
				}
				else
				{
					// reset frame counter
					g_dwFrameSkip=1;

					// compute frame carry-over
					dwFrameCarryOver+=dwTickCount-dwFrameTickCount;

					// skip frames as needed
					while(dwFrameCarryOver>=MSEC_PER_FRAME)
					{
						// increment frame skip
						g_dwFrameSkip++;

						// update frame carry-over
						dwFrameCarryOver-=MSEC_PER_FRAME;
					}

					// increment counter
					dwFrameTickCount=dwTickCount+MSEC_PER_FRAME;
				}
				
				// perform frame rate tasks
				if(dwTickCount>dwFPSTickCount)
				{
					// store frame rates
					g_dwFPS=dwFPSFrameCount;
					g_dwUPS=dwUPSFrameCount;

					// reset frame counters
					dwFPSFrameCount=0;
					dwUPSFrameCount=0;

					// reset tick count
					dwFPSTickCount=dwTickCount+MSEC_PER_SEC;
				}

				// update timers
				UpdateTimers();
			}
			else
			{
				// make sure timers are reset after returning from idle
				bTimerRefresh=TRUE;
			}
		}
	}
}

// DisplayErrorMessage()

void DisplayErrorMessage(LPSTR lpError,
						 LPSTR lpSolution,
						 BOOL bExtInfo,
						 BOOL bFatal)
{
	// add error message
	strcpy(g_lpErrorMsg,
		   (lpError ? lpError : "Undefined error."));

	// check solution string
	if(lpSolution)
	{
		// add line break
		strcat(g_lpErrorMsg,"\n\n");

		// add possible solutions
		strcat(g_lpErrorMsg,lpSolution);
	}
		   
	// check last error
	if(GetLastError()==ERROR_SUCCESS)
	{
		// reset extended info flag
		bExtInfo=FALSE;
	}
	
	// check extended info flag
	if(bExtInfo)
	{
		// add line break
		strcat(g_lpErrorMsg,"\n\n");

		// add system message
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
					  NULL,
					  GetLastError(),
					  NULL,
					  g_lpErrorMsg+strlen(g_lpErrorMsg),
					  (MSG_SIZE-1)-strlen(g_lpErrorMsg),
					  NULL);
	}

	// display message box
	DisplayMessageBox(g_hWnd,
					  g_lpErrorMsg,
					  (LPSTR)ApplicationName(),
					  MB_OK|(bFatal ? MB_ICONSTOP : MB_ICONWARNING));
}

// DisplayNEErrorMessage()

void DisplayNEErrorMessage(LPSTR lpError,
						   LPSTR lpSolution,
						   BOOL bExtInfo,
						   BOOL bFatal)
{
#ifdef SHOW_NE_ERRORS
	// display error
	DisplayErrorMessage(lpError,
						lpSolution,
						bExtInfo,
						bFatal);
#else
	// only output if ctrl is down
	if(IsKeyDown(VK_CTRL))
		DisplayErrorMessage(lpError,
							lpSolution,
							bExtInfo,
							bFatal);
#endif
}

// DisplayFatalErrorMessage()

void DisplayFatalErrorMessage(LPSTR lpError,
							  LPSTR lpSolution,
							  BOOL bExtInfo)
{
	// display error message
	DisplayErrorMessage(lpError,
						lpSolution,
						bExtInfo,
						TRUE);

#ifdef DEBUG_MODE
	// uncomment to locate error
	//return;
#endif

	// end execution
	ForceImmediateExit();
}

// DisplayDXErrorMessage()

void DisplayDXErrorMessage(LPSTR lpError,
						   LPSTR lpSolution,
						   HRESULT hError,
						   BOOL bFatal)
{
	// add error message
	strcpy(g_lpErrorMsg,
		   (lpError ? lpError : "Undefined error."));

	// check solution string
	if(lpSolution)
	{
		// add line break
		strcat(g_lpErrorMsg,"\n\n");

		// add possible solutions
		strcat(g_lpErrorMsg,lpSolution);
	}
	
	// check for extended info
	if(FAILED(hError))
	{
		// add extended info
		strcat(g_lpErrorMsg,"\n\nSysInfo: ");

		// copy dx error description
		strncpy(g_lpErrorMsg+strlen(g_lpErrorMsg),
				DXGetErrorDescription8(hError),
				(MSG_SIZE-1)-strlen(g_lpErrorMsg));

		// set null terminator
		g_lpErrorMsg[MSG_SIZE-1]=0;
	}

	// display message box
	DisplayMessageBox(g_hWnd,
					  g_lpErrorMsg,
					  (LPSTR)ApplicationName(),
					  MB_OK|(bFatal ? MB_ICONSTOP : MB_ICONWARNING));
}

// DisplayNEDXErrorMessage()

void DisplayNEDXErrorMessage(LPSTR lpError,
						     LPSTR lpSolution,
						     HRESULT hError,
						     BOOL bFatal)
{
#ifdef SHOW_NE_ERRORS
	// display error
	DisplayDXErrorMessage(lpError,
						  lpSolution,
						  hError,
						  bFatal);
#else
	// only output if ctrl is down
	if(IsKeyDown(VK_CTRL))
		DisplayDXErrorMessage(lpError,
							  lpSolution,
							  hError,
							  bFatal);
#endif
}

// DisplayFatalDXErrorMessage()

void DisplayFatalDXErrorMessage(LPSTR lpError,
							    LPSTR lpSolution,
							    HRESULT hError)
{
	// display error message
	DisplayDXErrorMessage(lpError,
						  lpSolution,
						  hError,
						  TRUE);

#ifdef DEBUG_MODE
	// uncomment to locate error
	//return;
#endif

	// end execution
	ForceImmediateExit();
}

// DisplayInfoMessage()

void DisplayInfoMessage(LPSTR lpError)
{
	// display error message
	DisplayMessageBox(g_hWnd,
					  (lpError ? lpError : "Undefined message."),
					  (LPSTR)ApplicationName(),
					  MB_OK|MB_ICONINFORMATION);
}

// InitializeAllModules()

BOOL InitializeAllModules(void)
{
	LoadingOutput("InitializeAllModules");

	// begin exclusive mode
	BeginExclusiveMode();

#ifdef SHOW_LOADINGOUTPUT
	// add loading output font
	g_dwLOFont=AddFont("Courier New",16);

	// check for error
	if(g_dwLOFont==NO_OBJECT)
		return(FALSE);
#endif

#ifdef SCREENSAVER_MODE
	// dummy value
	BOOL bDummy;

	// set screen-saver-running flag
	SystemParametersInfo(SPI_SCREENSAVERRUNNING,
						 TRUE,
						 &bDummy,
						 0);
#endif

ANTIHACK_2

	// load application options
	if(!ApplicationLoadOptions())
		return(FALSE);

	// set data path
	if(!SetDataPathRel(ApplicationDataPath()))
		return(FALSE);

	// create application window
	if(!CreateApplicationWindow())
		return(FALSE);

	// initialize misc items
	if(!InitAcceleratorTable())
		return(FALSE);
	if(!InitRandomNumbers())
		return(FALSE);

	// initialize directx application
	if(!InitializeDirectXApplication())
		return(FALSE);

	// ok
	return(TRUE);
}

// ReleaseAllModules()

void ReleaseAllModules(void)
{
	// exit idle mode
	ExitIdleMode();

	// close data file (if open)
	CloseDataFile();

#ifdef USE_COLLDETECT
	// purge world objects
	PurgeWorldObjects();
#endif
	
	// release directx applciation
	ReleaseDirectXApplication();

	// release misc items
	ReleaseAcceleratorTable();
	ReleaseRandomNumbers();

	// purge objects
	PurgeSystemObjects();

	// destroy window & associated resources
	DestroyApplicationWindow();

	// end exclusive mode
	EndExclusiveMode();

#ifdef SCREENSAVER_MODE
	// dummy value
	BOOL bDummy;

	// reset screen-saver-running flag
	SystemParametersInfo(SPI_SCREENSAVERRUNNING,
						 FALSE,
						 &bDummy,
						 0);
#endif
}

// EnterIdleMode()

void EnterIdleMode(void)
{
	// check flags
	if(!g_bIdle&&
	   !g_bClosing)
	{
		// set flag
		g_bIdle=TRUE;

		// end exclusive mode
		EndExclusiveMode();

		// show cursor
		ShowWindowsCursor();

		// pause application
		D3DCOLOR clrFilter=ApplicationPause();

		// pause audio
		PauseAudioOutput();

		// capture pause bitmap
		CapturePauseBitmap(clrFilter);

#ifdef FLASH_WHEN_MINIMIZED
		// create timer for flashing window
		SetTimer(g_hWnd,
				 FLASH_TIMER_ID,
				 FLASH_TIMER_DURATION,
				 NULL);
#endif
	}
}

// ExitIdleMode()

void ExitIdleMode(void)
{
	// check flags
	if(g_bIdle)
	{
		// begin exclusive mode
		BeginExclusiveMode();

		// restore window
		RestoreApplicationWindow();

		// hide cursor
		HideWindowsCursor();

#ifdef FLASH_WHEN_MINIMIZED
		// disable flash timer
		KillTimer(g_hWnd,
				  FLASH_TIMER_ID);
		
		// un-flash window
		FlashWindow(g_hWnd,
					FALSE);
#endif

		// release pause bitmap
		ReleasePauseBitmap();

		// resume audio
		ResumeAudioOutput();

		// resume application
		ApplicationResume();

		// set flag
		g_bIdle=FALSE;
	}
}

// SendCloseMessage()

void SendCloseMessage(void)
{
#ifdef SHAREWARE_MODE
	// check initialized flag
	if(g_bAppInit)
	{
		// set pre-close flag
		g_bPreClose=TRUE;

		// check close-ok flag
		if(!g_bCloseOK)
			return;
	}
#endif

	// local closing flag
	static BOOL bClosing=FALSE;
	
	// check global closing flag
	if(g_bClosing)
		return;

	// check local closing flag
	if(bClosing)
		return;

	// set local closing flag
	bClosing=TRUE;

#ifdef SCREENSAVER_MODE
	// confirm screensaver password
	if(!ConfirmScreensaverPassword())
	{
		// reset local closing flag
		bClosing=FALSE;

		// ok
		return;
	}
#endif
	
	// send close message
	SendMessage(g_hWnd,WM_CLOSE,0,0);

	// reset local closing flag
	bClosing=FALSE;
}

// SwapPtrs()

void SwapPtrs(LPVOID* lplpPtr1,
 			  LPVOID* lplpPtr2)
{
	// swap pointers
	LPVOID lpTemp=(*lplpPtr2);
	(*lplpPtr2)=(*lplpPtr1);
	(*lplpPtr1)=lpTemp;
}

// AllocMem()

BOOL AllocMem(LPVOID* lplpPtr,
 			  DWORD dwSize)
{
	LoadingOutput("AllocMem");

	// allocate memory
	(*lplpPtr)=malloc(dwSize);

	// check for error
	if(!(*lplpPtr))
	{
		DisplayErrorMessage("Unable to allocate memory block.");
		return(FALSE);
	}
	else
		AddMemResource;

	// clear memory
	ZeroMemory((*lplpPtr),
			   dwSize);

	// ok
	return(TRUE);
}

// ExpandMem()

BOOL ExpandMem(LPVOID* lplpPtr,
 			   DWORD dwSize,
 			   DWORD dwExpAmt)
{
	LoadingOutput("ExpandMem");

	// allocate new block
	LPVOID lpVoid;
	if(!AllocMem(&lpVoid,dwSize+dwExpAmt))
		return(FALSE);

	// copy memory
	CopyMemory(lpVoid,(*lplpPtr),dwSize);

	// swap pointers
	SwapPtrs(&lpVoid,lplpPtr);

	// free old block
	FreeMem(&lpVoid);

	// ok
	return(TRUE);
}

// CompressMem()

BOOL CompressMem(LPVOID* lplpPtr,
 			     DWORD dwSize)
{
	LoadingOutput("CompressMem");

	// allocate new block
	LPVOID lpVoid;
	if(!AllocMem(&lpVoid,dwSize))
		return(FALSE);

	// copy memory
	CopyMemory(lpVoid,(*lplpPtr),dwSize);

	// swap pointers
	SwapPtrs(&lpVoid,lplpPtr);

	// free old block
	FreeMem(&lpVoid);

	// ok
	return(TRUE);
}

// FreeMem()

void FreeMem(LPVOID* lplpPtr)
{
	// check pointer
	if(*lplpPtr)
	{
		// free memory
		free(*lplpPtr);
		RemoveMemResource;

		// clear pointer
		*lplpPtr=NULL;
	}
}

// GetNextTimer()

DWORD GetNextTimer(void)
{
	LoadingOutput("GetNextTimer");

	// check counter
	if(g_dwNumTimer==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwTimerLast,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwTimerInc,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxTimer=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumTimer>=g_dwMaxTimer)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwTimerLast,
					  sizeof(DWORD)*g_dwMaxTimer,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwTimerInc,
					  sizeof(DWORD)*g_dwMaxTimer,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxTimer+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumTimer;

	// increment counter
	g_dwNumTimer++;

	// return index
	return(dwIndex);
}

// AddTimer()

DWORD AddTimer(DWORD dwFramesPerTick)
{
	LoadingOutput("AddTimer");

	// get return index
	DWORD dwIndex=GetNextTimer();

	// reset timer
	g_lpdwTimerLast[dwIndex]=g_dwAppTimer;

	// set increment
	g_lpdwTimerInc[dwIndex]=dwFramesPerTick;

	// return index
	return(dwIndex);
}

// UpdateTimers()

void UpdateTimers(void)
{
	// increment timer using frameskip
	g_dwAppTimer+=g_dwFrameSkip;
}

// CheckTimer()

DWORD CheckTimer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTimer)
	{
		DisplayFatalErrorMessage("Invalid timer index.",
								 "CheckTimer()",
								 FALSE);
		return(0);
	}
#endif

	// reset tick counter
	DWORD dwCount=0;
	
	// count number of ticks passed since last call
	while(g_lpdwTimerLast[dwIndex]<g_dwAppTimer)
	{
		// increment last timer
		g_lpdwTimerLast[dwIndex]+=g_lpdwTimerInc[dwIndex];

		// increment tick counter
		dwCount++;
	}

	// return number of ticks
	return(dwCount);
}

// PurgeTimers()

void PurgeTimers(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpdwTimerLast);
	FreeMem((LPVOID*)&g_lpdwTimerInc);
	
	// reset counters
	g_dwNumTimer=0;
	g_dwMaxTimer=0;
}

// InitRandomNumbers()

BOOL InitRandomNumbers(void)
{
	LoadingOutput("InitRandomNumbers");

ANTIHACK_3

	// random number lookup tables disabled

	// seed number generator
	srand(GetTickCount());

	/*// counter
	DWORD dwCount;

	// allocate tables
	if(!AllocMem((LPVOID*)&g_lpfRand,
				 sizeof(float)*RANDOM_NUMBERS))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpnRand,
				 sizeof(int)*RANDOM_NUMBERS))
		return(FALSE);

	// create tables
	for(dwCount=0;dwCount<RANDOM_NUMBERS;dwCount++)
	{
		// set float value
		g_lpfRand[dwCount]=((float)(rand()%10000))/9999.0F;
		
		// negate if necessary
		if(rand()%2)
			g_lpfRand[dwCount]=-g_lpfRand[dwCount];

		// set int value
		g_lpnRand[dwCount]=rand();
		
		// negate if necessary
		if(rand()%2)
			g_lpnRand[dwCount]=-g_lpnRand[dwCount];
	}*/

	// ok
	return(TRUE);
}

// GetRandomFloat()

float GetRandomFloat(BOOL bPrevious)
{
	// previous number
	static float fPrev=0.0F;

	// check previous flag
	if(bPrevious)
		return(fPrev);
	
	// set return value
	float fRet=((float)(rand()%10000))*(1.0F/9999.0F);
	
	// negate if necessary
	if(rand()%2)
		fRet=-fRet;

	// save as previous
	fPrev=fRet;

	// return final value
	return(fRet);

	// random number lookup tables disabled
			
	/*// current number
	static DWORD dwCur=0;
	
	// check flag
	if(!bPrevious)
	{
		// get next number
		dwCur++;

		// prevent overflow
		if(dwCur>=RANDOM_NUMBERS)
			dwCur=0;
	}

	// return value
	return(g_lpfRand[dwCur]);*/
}

// GetRandomInt()

int GetRandomInt(BOOL bPrevious)
{
	// previous number
	static int nPrev=0;

	// check previous flag
	if(bPrevious)
		return(nPrev);
	
	// set return value
	int nRet=rand();
	
	// negate if necessary
	if(rand()%2)
		nRet=-nRet;

	// save as previous
	nPrev=nRet;

	// return final value
	return(nRet);

	// random number lookup tables disabled

	/*// current number
	static DWORD dwCur=0;
	
	// check flag
	if(!bPrevious)
	{
		// get next number
		dwCur++;
		
		// prevent overflow
		if(dwCur>=RANDOM_NUMBERS)
			dwCur=0;
	}

	// return value
	return(g_lpnRand[dwCur]);*/
}

// ReleaseRandomNumbers()

void ReleaseRandomNumbers(void)
{
	// random number lookup tables disabled

	/*// free memory
	FreeMem((LPVOID*)&g_lpfRand);
	FreeMem((LPVOID*)&g_lpnRand);*/
}

// VerifyClassObject()

BOOL VerifyClassObject(LPVOID lpPtr)
{
	LoadingOutput("VerifyClassObject");

	// check pointer
	if(!lpPtr)
	{
		DisplayErrorMessage("Unable to allocate class object.",
							"Check available memory.",
							FALSE);
		return(FALSE);
	}
	
	// ok
	return(TRUE);
}

#ifdef DEBUG_MODE
// DisplayResourceUsage()

void DisplayResourceUsage(void)
{
	LockGlobalMsg()

	// display system resources
	sprintf(g_lpMsg,
			"System Resources\n\n"
			"System: %d\n"
			"Memory: %d\n"
			"Class: %d\n"
			"COM: %d\n"
			"Timers: %d\n"
			"Dialog Boxes: %d\n",
			g_dwNumWinRes,
			g_dwNumMemRes,
			g_dwNumClassRes,
			g_dwNumCOMRes,
			g_dwNumTimer,
			g_dwNumDlg);
	DisplayInfoMessage(g_lpMsg);

	// display graphical resources
	sprintf(g_lpMsg,
			"Graphical Resources\n\n"
			"Vertex Buffers: %d\n"
			"Index Buffers: %d\n"
			"Matrices: %d\n"
			"Materials: %d\n"
			"Textures: %d\n"
			"Lights: %d\n"
			"Meshes: %d\n"
			"Skin Meshes: %d\n"
			"Animations: %d\n"
			"Fonts: %d\n",
			g_dwNumVtxBuf,
			g_dwNumIdxBuf,
			g_dwNumMtx,
			g_dwNumMat,
			g_dwNumTex,
			g_dwNumLight,
			g_dwNumMesh,
			g_dwNumSkin,
			g_dwNumAnim,
			g_dwNumFont);
	DisplayInfoMessage(g_lpMsg);

	// display audio resources
	sprintf(g_lpMsg,
			"Audio Resources\n\n"
			"Segments: %d\n"
			"Audio Paths: %d\n",
			g_dwNumAudioSeg,
			g_dwNumAudioPath);
	DisplayInfoMessage(g_lpMsg);

#ifdef USE_DIRECTINPUT
	// display input resources
	sprintf(g_lpMsg,
			"Input Resources\n\n"
			"Buttons: %d\n"
			"Axes: %d\n"
			"Button Triggers: %d\n"
			"Axis Triggers: %d\n",
			g_dwNumButton,
			g_dwNumAxis,
			g_dwNumButtonTrig,
			g_dwNumAxisTrig);
	DisplayInfoMessage(g_lpMsg);
#endif

#ifdef USE_COLLDETECT
	// display collision resources
	sprintf(g_lpMsg,
			"Collision Resources\n\n"
			"SW Meshes: %d\n"
			"DW Meshes: %d\n"
			"Static Tris: %d\n"
			"SW Objects: %d\n"
			"DW Objects: %d\n",
			g_dwNumSWMesh,
			g_dwNumDWMesh,
			g_dwNumSTri,
			g_dwNumSWObj,
			g_dwNumDWObj);
	DisplayInfoMessage(g_lpMsg);
#endif

	UnlockGlobalMsg()
}
#endif

// ConfirmResourcesReleased()

void ConfirmResourcesReleased(void)
{
	// compute total resources
	DWORD dwTotalRes=g_dwNumWinRes+
					 g_dwNumMemRes+
					 g_dwNumClassRes+
					 g_dwNumCOMRes+
					 g_dwNumTimer+
					 g_dwNumDlg+
					 g_dwNumVtxBuf+
					 g_dwNumIdxBuf+
					 g_dwNumMtx+
					 g_dwNumMat+
					 g_dwNumTex+
					 g_dwNumLight+
					 g_dwNumMesh+
					 g_dwNumSkin+
					 g_dwNumAnim+
					 g_dwNumFont+
					 g_dwNumAudioSeg+
					 g_dwNumAudioPath;

#ifdef USE_DIRECTINPUT
	// add input resources
	dwTotalRes+=g_dwNumButton+
				g_dwNumAxis+
				g_dwNumButtonTrig+
				g_dwNumAxisTrig;
#endif

#ifdef USE_COLLDETECT
	// add collision resources
	dwTotalRes+=g_dwNumSWMesh+
				g_dwNumDWMesh+
				g_dwNumSTri+
				g_dwNumSWObj+
				g_dwNumDWObj;
#endif

	// if resources are left over, speak up!
	if(dwTotalRes!=0)
	{
		// removed: disconcerting to user!
		//DisplayInfoMessage("Some resources were not able to be released.\nYour system may become unstable.");
#ifdef DEBUG_MODE
		DisplayResourceUsage();
#endif
	}
}

// OpenRegistryRead()

BOOL OpenRegistryRead(BOOL bSystemWide)
{
	LoadingOutput("OpenRegistryRead");

ANTIHACK_4

	// error-shown flag
	static BOOL bErrorShown=FALSE;
	
	// key creation disposition
	DWORD dwCreationDisp;

	// base key
	HKEY hBaseKey;

	// set base key
	if(bSystemWide)
		hBaseKey=HKEY_LOCAL_MACHINE;
	else
		hBaseKey=HKEY_CURRENT_USER;
	
	// open software key
	if(RegCreateKeyEx(hBaseKey,
				      "Software",
				      NULL,
				      NULL,
				      REG_OPTION_NON_VOLATILE,
				      KEY_READ,
				      NULL,
				      &g_hSoftKey,
				      &dwCreationDisp)!=ERROR_SUCCESS)
	{
		if(!bErrorShown)
		{
			DisplayErrorMessage("Unable to open registry key.",
								"Configuration settings may be unavailable.",
								FALSE);
			bErrorShown=TRUE;
		}
		return(FALSE);
	}
	else
		AddWinResource;

	// open application key
	if(RegCreateKeyEx(g_hSoftKey,
				      ApplicationName(),
				      NULL,
				      NULL,
				      REG_OPTION_NON_VOLATILE,
				      KEY_READ,
				      NULL,
				      &g_hAppKey,
				      &dwCreationDisp)!=ERROR_SUCCESS)
	{
		CloseRegistry();
		if(!bErrorShown)
		{
			DisplayErrorMessage("Unable to open registry key.",
								"Configuration settings may be unavailable.",
								FALSE);
			bErrorShown=TRUE;
		}
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// OpenRegistryWrite()

BOOL OpenRegistryWrite(BOOL bSystemWide)
{
	LoadingOutput("OpenRegistryWrite");

ANTIHACK_4

	// error-shown flag
	static BOOL bErrorShown=FALSE;
	
	// key creation disposition
	DWORD dwCreationDisp;

	// base key
	HKEY hBaseKey;

	// set base key
	if(bSystemWide)
		hBaseKey=HKEY_LOCAL_MACHINE;
	else
		hBaseKey=HKEY_CURRENT_USER;
	
	// open software key
	if(RegCreateKeyEx(hBaseKey,
				      "Software",
				      NULL,
				      NULL,
				      REG_OPTION_NON_VOLATILE,
				      KEY_WRITE,
				      NULL,
				      &g_hSoftKey,
				      &dwCreationDisp)!=ERROR_SUCCESS)
	{
		if(!bErrorShown)
		{
			DisplayErrorMessage("Unable to open registry key.",
								"Configuration settings may be unavailable.",
								FALSE);
			bErrorShown=TRUE;
		}
		return(FALSE);
	}
	else
		AddWinResource;

	// open application key
	if(RegCreateKeyEx(g_hSoftKey,
				      ApplicationName(),
				      NULL,
				      NULL,
				      REG_OPTION_NON_VOLATILE,
				      KEY_WRITE,
				      NULL,
				      &g_hAppKey,
				      &dwCreationDisp)!=ERROR_SUCCESS)
	{
		CloseRegistry();
		if(!bErrorShown)
		{
			DisplayErrorMessage("Unable to open registry key.",
								"Configuration settings may be unavailable.",
								FALSE);
			bErrorShown=TRUE;
		}
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// CloseRegistry()

void CloseRegistry(void)
{
	// error-shown flag
	static BOOL bErrorShown=FALSE;
	
	// close application key
	if(g_hAppKey!=INVALID_HANDLE_VALUE)
	{
		// close key
		if(RegCloseKey(g_hAppKey)!=ERROR_SUCCESS)
		{
			if(!bErrorShown)
			{
				DisplayErrorMessage("Unable to close registry key.",
									"Configuration settings may not be stored.",
									FALSE);
				bErrorShown=TRUE;
			}
		}
		else
		{
			g_hAppKey=(HKEY)INVALID_HANDLE_VALUE;
			RemoveWinResource;
		}
	}
	
	// close software key
	if(g_hSoftKey!=INVALID_HANDLE_VALUE)
	{
		// close key
		if(RegCloseKey(g_hSoftKey)!=ERROR_SUCCESS)
		{
			if(!bErrorShown)
			{
				DisplayErrorMessage("Unable to close registry key.",
									"Configuration settings may not be stored.",
									FALSE);
				bErrorShown=TRUE;
			}
		}
		else
		{
			g_hSoftKey=(HKEY)INVALID_HANDLE_VALUE;
			RemoveWinResource;
		}
	}
}

// SetRegistryKey()

void SetRegistryKey(LPCSTR lpName,
					LPSTR lpData)
{
	// error-shown flag
	static BOOL bErrorShown=FALSE;
	
	// open registry
	if(OpenRegistryWrite())
	{
		// set value
		if(RegSetValueEx(g_hAppKey,
						 lpName,
						 NULL,
						 REG_SZ,
						 (CONST LPBYTE)lpData,
						 strlen(lpData)+1)!=ERROR_SUCCESS)
		{
			if(!bErrorShown)
			{
				DisplayErrorMessage("Unable to set registry key value.",
									"Configuration settings may not be saved.",
									FALSE);
				bErrorShown=TRUE;
			}
		}
	}

	// close registry
	CloseRegistry();
}

// GetRegistryKey()

LPSTR GetRegistryKey(LPCSTR lpName)
{
	LoadingOutput("GetRegistryKey");

	// pointer to return
	LPSTR lpData=NULL;

	// open registry
	if(OpenRegistryRead())
	{
		// get buffer size
		DWORD dwSize;
		DWORD dwType;
		RegQueryValueEx(g_hAppKey,
						lpName,
						NULL,
						&dwType,
						NULL,
						&dwSize);

		// check for zero or excessively large size
		if(!dwSize||
		   dwSize>128)
		{
			CloseRegistry();
			return(NULL);
		}
		
		// allocate buffer
		if(!AllocMem((LPVOID*)&lpData,
					 dwSize))
			return(NULL);

		// read value
		if(RegQueryValueEx(g_hAppKey,
						   lpName,
						   NULL,
						   &dwType,
						   (LPBYTE)lpData,
						   &dwSize)!=ERROR_SUCCESS)
			// key does not exist
			lpData=NULL;
	}

	// close registry
	CloseRegistry();

	// return value
	return(lpData);
}

// DeleteRegistryKey()

void DeleteRegistryKey(LPCSTR lpName)
{
	LoadingOutput("");

	// open registry
	if(OpenRegistryWrite())
	{
		// delete value
		RegDeleteValue(g_hAppKey,
					   lpName);
	}

	// close registry
	CloseRegistry();
}

// SetSystemWideRegistryKey()

void SetSystemWideRegistryKey(LPCSTR lpName,
							  LPSTR lpData)
{
	// error-shown flag
	static BOOL bErrorShown=FALSE;
	
	// open registry
	if(OpenRegistryWrite(TRUE))
	{
		// set value
		if(RegSetValueEx(g_hAppKey,
						 lpName,
						 NULL,
						 REG_SZ,
						 (CONST LPBYTE)lpData,
						 strlen(lpData)+1)!=ERROR_SUCCESS)
		{
			if(!bErrorShown)
			{
				DisplayErrorMessage("Unable to set system-wide registry key value.",
									"Configuration settings may not be saved.",
									FALSE);
				bErrorShown=TRUE;
			}
		}
	}

	// close registry
	CloseRegistry();
}

// GetSystemWideRegistryKey()

LPSTR GetSystemWideRegistryKey(LPCSTR lpName)
{
	LoadingOutput("GetRegistryKey");

ANTIHACK_5

	// pointer to return
	LPSTR lpData=NULL;

	// open registry
	if(OpenRegistryRead(TRUE))
	{
		// get buffer size
		DWORD dwSize;
		DWORD dwType;
		RegQueryValueEx(g_hAppKey,
						lpName,
						NULL,
						&dwType,
						NULL,
						&dwSize);

		// check for zero or excessively large size
		if(!dwSize||
		   dwSize>128)
		{
			CloseRegistry();
			return(NULL);
		}
		
		// allocate buffer
		if(!AllocMem((LPVOID*)&lpData,
					 dwSize))
			return(NULL);

		// read value
		if(RegQueryValueEx(g_hAppKey,
						   lpName,
						   NULL,
						   &dwType,
						   (LPBYTE)lpData,
						   &dwSize)!=ERROR_SUCCESS)
		{
			// key does not exist
			lpData=NULL;
		}
	}

	// close registry
	CloseRegistry();

	// return value
	return(lpData);
}

// DeleteSystemWideRegistryKey()

void DeleteSystemWideRegistryKey(LPCSTR lpName)
{
	LoadingOutput("");

	// open registry
	if(OpenRegistryWrite(TRUE))
	{
		// delete value
		RegDeleteValue(g_hAppKey,
					   lpName);
	}

	// close registry
	CloseRegistry();
}

// FreeRegistryKey()

void FreeRegistryKey(LPSTR lpData)
{
	// free previously allocated memory
	FreeMem((LPVOID*)&lpData);
}

#ifdef DEBUG_MODE
// OpenScriptCompiler()

BOOL OpenScriptCompiler(LPCSTR lpDestFile,
						LPCSTR lpSrcFile)
{
	LoadingOutput("OpenScriptCompiler");

	// check handles
	if(g_hScriptFile!=INVALID_HANDLE_VALUE||
	   g_hCScriptFile!=INVALID_HANDLE_VALUE)
	{
		DisplayFatalErrorMessage("Unable to open multiple script files.",
								 NULL,
								 FALSE);
		return(FALSE);
	}

	// open source file
	g_hScriptFile=CreateFile(lpSrcFile,
							 GENERIC_READ,
							 NULL,
							 NULL,
							 OPEN_EXISTING,
							 FILE_ATTRIBUTE_NORMAL|
							 FILE_FLAG_SEQUENTIAL_SCAN,
							 NULL);

	// check for error
	if(!g_hScriptFile)
	{
		g_hScriptFile=INVALID_HANDLE_VALUE;
		DisplayFatalErrorMessage("Unable to open script source file.",
								 NULL,
								 FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// open dest file
	g_hCScriptFile=CreateFile(lpDestFile,
							  GENERIC_WRITE,
							  NULL,
							  NULL,
							  CREATE_ALWAYS,
							  FILE_ATTRIBUTE_NORMAL|
							  FILE_FLAG_SEQUENTIAL_SCAN,
							  NULL);

	// check for error
	if(!g_hCScriptFile)
	{
		g_hScriptFile=INVALID_HANDLE_VALUE;
		DisplayFatalErrorMessage("Unable to open script destination file.",
								 NULL,
								 FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// OpenScriptCreator()

BOOL OpenScriptCreator(LPCSTR lpFile)
{
	LoadingOutput("OpenScriptCreator");

	// check handle
	if(g_hScriptFile!=INVALID_HANDLE_VALUE)
	{
		DisplayFatalErrorMessage("Unable to open multiple script files.",
								 NULL,
								 FALSE);
		return(FALSE);
	}

	// open source file
	g_hScriptFile=CreateFile(lpFile,
							 GENERIC_WRITE,
							 NULL,
							 NULL,
							 CREATE_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL|
							 FILE_FLAG_SEQUENTIAL_SCAN,
							 NULL);

	// check for error
	if(!g_hScriptFile)
	{
		g_hScriptFile=INVALID_HANDLE_VALUE;
		DisplayFatalErrorMessage("Unable to open script source file.",
								 NULL,
								 FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// ReadScriptChar()

char ReadScriptChar(void)
{
	// bytes read
	DWORD dwBytesRead;

	// return value
	char cVal;

	// read byte
	ReadFile(g_hScriptFile,
			 (LPVOID)&cVal,
			 sizeof(char),
			 &dwBytesRead,
			 NULL);

	// check for error
	if(dwBytesRead!=sizeof(char))
		return(NULL);

	// return value
	return(cVal);
}

// ReadScriptLong()

long ReadScriptLong(void)
{	
	// just to verify
	LockGlobalMsg()
	UnlockGlobalMsg()

	// read string
	ReadScriptString(g_lpMsg);

	// convert string to long
	return(atol(g_lpMsg));
}

// ReadScriptFloat()

float ReadScriptFloat(void)
{
	// just to verify
	LockGlobalMsg()
	UnlockGlobalMsg()

	// read string
	ReadScriptString(g_lpMsg);

	// convert string to float
	return((float)atof(g_lpMsg));
}

// ReadScriptString()

DWORD ReadScriptString(LPSTR lpStr)
{
	// character read
	char cData;
	
	// string position
	DWORD dwStrPos=0;

	// bytes read
	DWORD dwBytesRead=0;

	// read leading bytes
	while(TRUE)
	{
		// read character
		cData=ReadScriptChar(); dwBytesRead++;

		// check for end of file
		if(cData==NULL)
			{ dwBytesRead--; break; }

		// check for comment
		if(cData=='<')
			while(cData!='>')
				{ cData=ReadScriptChar(); dwBytesRead++; }
		
		// check for valid characters
		if(IsValidScriptChar(cData))
			break;
	}

	// read to end of data
	while(TRUE)
	{
		// store byte
		lpStr[dwStrPos++]=cData;

		// next byte
		cData=ReadScriptChar(); dwBytesRead++;

		// check for end of file
		if(cData==NULL)
			{ dwBytesRead--; break; }

		// check for comment
		if(cData=='<')
			while(cData!='>')
				{ cData=ReadScriptChar(); dwBytesRead++; }
		
		// check for valid characters
		if(!IsValidScriptChar(cData))
			break;
	}

	// set string terminator
	lpStr[dwStrPos]=NULL;

	// return number of bytes read
	return(dwBytesRead);
}

// SaveScriptChar()

BOOL SaveScriptChar(char cVal)
{
	// bytes written
	DWORD dwBytesWritten;
	
	// output data
	WriteFile(g_hCScriptFile,
			  (LPVOID)&cVal,
			  sizeof(char),
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=sizeof(char))
	{
		DisplayFatalErrorMessage("Unable to output BYTE to script.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SaveScriptLong()

BOOL SaveScriptLong(long lVal)
{
	// bytes written
	DWORD dwBytesWritten;
	
	// output data
	WriteFile(g_hCScriptFile,
			  (LPVOID)&lVal,
			  sizeof(long),
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=sizeof(long))
	{
		DisplayFatalErrorMessage("Unable to output DWORD to script.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SaveScriptFloat()

BOOL SaveScriptFloat(float fVal)
{
	// bytes written
	DWORD dwBytesWritten;
	
	// output data
	WriteFile(g_hCScriptFile,
			  (LPVOID)&fVal,
			  sizeof(float),
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=sizeof(float))
	{
		DisplayFatalErrorMessage("Unable to output FLOAT to script.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SaveScriptString()

BOOL SaveScriptString(LPSTR lpStr)
{
	// bytes written
	DWORD dwBytesWritten;

	// bytes to write
	DWORD dwBytesToWrite=strlen(lpStr)+1;

	// output data
	WriteFile(g_hCScriptFile,
			  (LPVOID)lpStr,
			  dwBytesToWrite,
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=dwBytesToWrite)
	{
		DisplayFatalErrorMessage("Unable to output STRING to script.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// CopyScriptChar()

BOOL CopyScriptChar(void)
{
	// copy value
	return(SaveScriptChar(ReadScriptChar()));
}

// CopyScriptBytes()

BOOL CopyScriptBytes(DWORD dwNumBytes)
{
	// counter
	DWORD dwCount;

	// copy bytes
	for(dwCount=0;dwCount<dwNumBytes;dwCount++)
		if(!CopyScriptChar())
			return(FALSE);

	// ok
	return(TRUE);
}

// CopyScriptLong()

BOOL CopyScriptLong(void)
{
	// copy value
	return(SaveScriptLong(ReadScriptLong()));
}

// CopyScriptFloat()

BOOL CopyScriptFloat(void)
{
	// copy value
	return(SaveScriptFloat(ReadScriptFloat()));
}

// CopyScriptString()

BOOL CopyScriptString(void)
{
	// just to verify
	LockGlobalMsg()
	UnlockGlobalMsg()

	// read string
	if(ReadScriptString(g_lpMsg))
	{
		// save string
		return(SaveScriptString(g_lpMsg));
	}
	else
		return(FALSE);
}

// CloseScriptCompiler()

void CloseScriptCompiler(void)
{
	// close source file
	if(!CloseHandle(g_hScriptFile))
	{
		DisplayFatalErrorMessage("Unable to close script source file.");
		return;
	}
	else
	{	
		// reset handle
		g_hScriptFile=INVALID_HANDLE_VALUE;
		
		// remove resource
		RemoveWinResource;
	}

	// close destination file
	if(!CloseHandle(g_hCScriptFile))
	{
		DisplayFatalErrorMessage("Unable to close script destination file.");
		return;
	}
	else
	{	
		// reset handle
		g_hCScriptFile=INVALID_HANDLE_VALUE;

		// remove resource
		RemoveWinResource;
	}
}

// ResetScriptCompilerSource()

BOOL ResetScriptCompilerSource(LPCSTR lpFile)
{
	// close old source file
	if(!CloseHandle(g_hScriptFile))
	{
		DisplayFatalErrorMessage("Unable to close script source file.");
		return(FALSE);
	}
	else
	{	
		// reset handle
		g_hScriptFile=INVALID_HANDLE_VALUE;
		
		// remove resource
		RemoveWinResource;
	}
	
	// open new source file
	g_hScriptFile=CreateFile(lpFile,
							 GENERIC_READ,
							 NULL,
							 NULL,
							 OPEN_EXISTING,
							 FILE_ATTRIBUTE_NORMAL|
							 FILE_FLAG_SEQUENTIAL_SCAN,
							 NULL);

	// check for error
	if(!g_hScriptFile)
	{
		g_hScriptFile=INVALID_HANDLE_VALUE;
		DisplayFatalErrorMessage("Unable to open script source file.",
								 NULL,
								 FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// CloseScriptCreator()

void CloseScriptCreator(void)
{
	// close source file
	if(!CloseHandle(g_hScriptFile))
	{
		DisplayFatalErrorMessage("Unable to close script source file.");
		return;
	}
	else
	{	
		// reset handle
		g_hScriptFile=INVALID_HANDLE_VALUE;

		// remove resource
		RemoveWinResource;
	}
}
#endif

// OpenCompiledScript()

BOOL OpenCompiledScript(LPCSTR lpFile)
{
	LoadingOutput("OpenCompiledScript");

	// check for data file reference
	DATAFILEREF dfr=FindDataFileRef((LPSTR)lpFile);

	// check value and load accordingly
	if(dfr.lpData)
	{
		// check memory block
		if(g_lpCScriptData)
		{
			DisplayErrorMessage("Unable to open multiple script files.",
								NULL,
								FALSE);
			return(FALSE);
		}

		// allocate memory for data
		if(!AllocMem((LPVOID*)&g_lpCScriptData,
					 dfr.dwSize))
			return(FALSE);

		// read data
		CopyMemory(g_lpCScriptData,
				   dfr.lpData,
				   dfr.dwSize);
	}
	else
	{
		// check memory block
		if(g_lpCScriptData)
		{
			DisplayErrorMessage("Unable to open multiple script files.",
								NULL,
								FALSE);
			return(FALSE);
		}

		// open file
		g_hCScriptFile=CreateFile(lpFile,
								  GENERIC_READ,
								  NULL,
								  NULL,
								  OPEN_EXISTING,
								  FILE_ATTRIBUTE_NORMAL|
								  FILE_FLAG_SEQUENTIAL_SCAN,
								  NULL);

		// check for error
		if(!g_hCScriptFile)
		{
			g_hCScriptFile=INVALID_HANDLE_VALUE;
			DisplayErrorMessage("Unable to open script file.",
								NULL,
								FALSE);
			return(FALSE);
		}
		else
			AddWinResource;

		// get size of file
		DWORD dwHSize;
		DWORD dwSize=GetFileSize(g_hCScriptFile,
								 &dwHSize);

		// check for error
		if(!dwSize||dwHSize)
		{
			DisplayErrorMessage("Unable to read script file.",
								NULL,
								FALSE);
			return(FALSE);
		}

		// allocate memory for data
		if(!AllocMem((LPVOID*)&g_lpCScriptData,
					 dwSize))
			return(FALSE);

		// read data
		DWORD dwBytesRead;
		ReadFile(g_hCScriptFile,
				 (LPVOID)g_lpCScriptData,
				 dwSize,
				 &dwBytesRead,
				 NULL);

		// check for error
		if(dwBytesRead!=dwSize)
		{
			DisplayErrorMessage("Unable to read script data.");
			return(FALSE);
		}
		
		// close destination file
		if(!CloseHandle(g_hCScriptFile))
		{
			DisplayErrorMessage("Unable to close script destination file.");
			return(FALSE);
		}
		else
		{	
			// reset handle
			g_hCScriptFile=INVALID_HANDLE_VALUE;

			// remove resource
			RemoveWinResource;
		}
	}

	// reset pointer
	g_dwCScriptDataPtr=0;

	// ok
	return(TRUE);
}

// ReadCScriptChar()

char ReadCScriptChar(void)
{
	// read value
	char* lpc=(char*)&g_lpCScriptData[g_dwCScriptDataPtr];

	// increment pointer
	g_dwCScriptDataPtr+=sizeof(char);
	
	// return value
	return(*lpc);
}

// ReadCScriptBytes()

void ReadCScriptBytes(LPVOID lpDest,
					  DWORD dwNumBytes)
{
	// copy data
	CopyMemory(lpDest,
			   &g_lpCScriptData[g_dwCScriptDataPtr],
			   dwNumBytes);

	// increment pointer
	g_dwCScriptDataPtr+=dwNumBytes;
}

// ReadCScriptLong()

long ReadCScriptLong(void)
{
	// read value
	long* lpl=(long*)&g_lpCScriptData[g_dwCScriptDataPtr];

	// increment pointer
	g_dwCScriptDataPtr+=sizeof(long);
	
	// return value
	return(*lpl);
}

// ReadCScriptFloat()

float ReadCScriptFloat(void)
{
	// read value
	float* lpf=(float*)&g_lpCScriptData[g_dwCScriptDataPtr];

	// increment pointer
	g_dwCScriptDataPtr+=sizeof(float);

	// return value
	return(*lpf);
}

// ReadCScriptString()

void ReadCScriptString(LPSTR lpStr)
{
	// string pointer
	DWORD dwStrPtr=0;
	
	// read bytes until end of string
	while(TRUE)
	{
		// read byte
		lpStr[dwStrPtr]=ReadCScriptChar();

		// check for null terminator
		if(!lpStr[dwStrPtr])
			break;

		// increment pointer
		dwStrPtr++;
	}
}

// CloseCompiledScript()

void CloseCompiledScript(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpCScriptData);
}

// EnableMouseTracking()

void EnableMouseTracking(void)
{
ANTIHACK_6

	// set flag
	g_bWinMouse=TRUE;
}

// DisableMouseTracking()

void DisableMouseTracking(void)
{
	// set flag
	g_bWinMouse=FALSE;
}

#ifdef DEBUG_MODE
// CreateScriptString()

BOOL CreateScriptString(LPSTR lpStr)
{
	LockGlobalMsg()

	// copy string to global & append new line
	strcpy(g_lpMsg,lpStr);
	strcat(g_lpMsg,"\r\n");
	
	// bytes written
	DWORD dwBytesWritten;

	// bytes to write
	DWORD dwBytesToWrite=strlen(g_lpMsg);

	// output data
	WriteFile(g_hScriptFile,
			  (LPVOID)g_lpMsg,
			  dwBytesToWrite,
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=dwBytesToWrite)
	{
		DisplayFatalErrorMessage("Unable to output STRING to script.");
		UnlockGlobalMsg()
		return(FALSE);
	}

	UnlockGlobalMsg()

	// ok
	return(TRUE);
}
#endif

// GetNextDialog()

DWORD GetNextDialog(void)
{
	LoadingOutput("GetNextDialog");

	// check counter
	if(g_dwNumDlg==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpDlgData,
					 sizeof(LPBYTE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDlgDataPtr,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDlgDataSize,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpnDlgResID,
					 sizeof(int)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDlg=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumDlg>=g_dwMaxDlg)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpDlgData,
					  sizeof(LPBYTE)*g_dwMaxDlg,
					  sizeof(LPBYTE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDlgDataPtr,
					  sizeof(DWORD)*g_dwMaxDlg,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDlgDataSize,
					  sizeof(DWORD)*g_dwMaxDlg,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpnDlgResID,
					  sizeof(int)*g_dwMaxDlg,
					  sizeof(int)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDlg+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumDlg;

	// increment counter
	g_dwNumDlg++;

	// return index
	return(dwIndex);
}

// AddDialog()

DWORD AddDialog(short sXSize,
			    short sYSize,
				LPCSTR lpTitle)
{
	LoadingOutput("AddDialog");

	// get return index
	DWORD dwIndex=GetNextDialog();
	
	// allocate memory for template
	if(!AllocMem((LPVOID*)&g_lplpDlgData[dwIndex],
				 sizeof(BYTE)*BLOCK_ALLOC_INC))
		return(NO_OBJECT);

	// set size
	g_lpdwDlgDataSize[dwIndex]=BLOCK_ALLOC_INC;

	// reset pointer
	g_lpdwDlgDataPtr[dwIndex]=0;

	// add header
	AddDialogDWORD(dwIndex,DS_CENTER|
						   DS_MODALFRAME|
						   DS_SETFOREGROUND|
						   WS_CAPTION); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogShort(dwIndex,0); // number of items
	AddDialogCoords(dwIndex,NULL,NULL); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // menu resource
	AddDialogWORD(dwIndex,NULL); // class id
	AddDialogString(dwIndex,(LPSTR)lpTitle); // title

	// reset resource id
	g_lpnDlgResID[dwIndex]=0;

	// return index
	return(dwIndex);
}

// AddDiallogFromResource()

DWORD AddDialogFromResource(int nResID)
{
	LoadingOutput("AddDialogFromResource");

	// get return index
	DWORD dwIndex=GetNextDialog();
	
	// reset unnecessary values
	g_lplpDlgData[dwIndex]=NULL;
	g_lpdwDlgDataSize[dwIndex]=0;
	g_lpdwDlgDataPtr[dwIndex]=0;

	// copy id
	g_lpnDlgResID[dwIndex]=nResID;

	// return index
	return(dwIndex);
}

// ExpandDialog()

void ExpandDialog(DWORD dwIndex)
{
	LoadingOutput("ExpandDialog");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "ExpandDialog()",
								 FALSE);
		return;
	}
#endif

	// expand template memory
	if(!ExpandMem((LPVOID*)&g_lplpDlgData[dwIndex],
				  sizeof(BYTE)*g_lpdwDlgDataSize[dwIndex],
				  sizeof(BYTE)*BLOCK_ALLOC_INC))
		return;

	// increment size
	g_lpdwDlgDataSize[dwIndex]+=BLOCK_ALLOC_INC;

	// ok
	return;
}

// AlignDialogWORD()

void AlignDialogWORD(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AlignDialogWORD()",
								 FALSE);
		return;
	}
#endif

	// add byte if necessary
	if(g_lpdwDlgDataPtr[dwIndex]&1)
		AddDialogBYTE(dwIndex,NULL);
}

// AlignDialogDWORD()

void AlignDialogDWORD(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AlignDialogDWORD()",
								 FALSE);
		return;
	}
#endif

	// align to word
	AlignDialogWORD(dwIndex);

	// add word if necessary
	if(g_lpdwDlgDataPtr[dwIndex]&2)
		AddDialogWORD(dwIndex,NULL);
}

// AddDialogDWORD()

void AddDialogDWORD(DWORD dwIndex,
					DWORD dwVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogDWORD()",
								 FALSE);
		return;
	}
#endif

	// expand storage if necessary
	if((g_lpdwDlgDataPtr[dwIndex]+sizeof(DWORD))>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   &dwVal,
			   sizeof(DWORD));

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=sizeof(DWORD);
}

// AddDialogWORD()

void AddDialogWORD(DWORD dwIndex,
				   WORD wVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogWORD()",
								 FALSE);
		return;
	}
#endif

	// expand storage if necessary
	if((g_lpdwDlgDataPtr[dwIndex]+sizeof(WORD))>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   &wVal,
			   sizeof(WORD));

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=sizeof(WORD);
}

// AddDialogBYTE()

void AddDialogBYTE(DWORD dwIndex,
				   BYTE cVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogBYTE)",
								 FALSE);
		return;
	}
#endif

	// expand storage if necessary
	if((g_lpdwDlgDataPtr[dwIndex]+sizeof(BYTE))>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   &cVal,
			   sizeof(BYTE));

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=sizeof(BYTE);
}

// AddDialogShort()

void AddDialogShort(DWORD dwIndex,
					short sVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogShort()",
								 FALSE);
		return;
	}
#endif

	// expand storage if necessary
	if((g_lpdwDlgDataPtr[dwIndex]+sizeof(short))>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   &sVal,
			   sizeof(short));

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=sizeof(short);
}

// AddDialogString()

void AddDialogString(DWORD dwIndex,
					 LPSTR lpStr)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog string.",
								 "AddDialogWORD()",
								 FALSE);
		return;
	}
#endif

	// compute number of bytes
	DWORD dwNumBytes=(strlen(lpStr)+1)<<1;

	// expand storage if necessary
	while((g_lpdwDlgDataPtr[dwIndex]+dwNumBytes)>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// convert string
	MakeWideString(g_lpWMsg,lpStr);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   (LPVOID)g_lpWMsg,
			   dwNumBytes);

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=dwNumBytes;
}

// AddDialogPtr()

void AddDialogPtr(DWORD dwIndex,
				  LPVOID lpPtr)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogPtr()",
								 FALSE);
		return;
	}
#endif

	// expand storage if necessary
	if((g_lpdwDlgDataPtr[dwIndex]+sizeof(LPVOID))>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   &lpPtr,
			   sizeof(LPVOID));

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=sizeof(LPVOID);
}

// ExecuteDialog()

int ExecuteDialog(DWORD dwIndex,
				  DLGPROC lpDlgProc,
				  HWND hWnd)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "ExecuteDialog()",
								 FALSE);
		return(0);
	}
#endif

	// return value
	int nVal;

	// check window handle
	if(hWnd==(HWND)INVALID_HANDLE_VALUE)
		hWnd=g_hWnd;

	// begin system output
	BOOL bFlag=BeginSystemOutput();
	
	// check resource name
	if(g_lpnDlgResID[dwIndex])
	{
		// execute dialog box
		nVal=DialogBox(g_hInst,
					   (LPCTSTR)g_lpnDlgResID[dwIndex],
					   hWnd,
					   lpDlgProc);
	}
	else
	{
		// execute dialog box
		nVal=DialogBoxIndirect(g_hInst,
							   (LPDLGTEMPLATE)g_lplpDlgData[dwIndex],
							   hWnd,
							   lpDlgProc);
	}

	// check for error
	if(nVal==-1)
		DisplayErrorMessage("Unable to execute dialog box");

	// end system output
	EndSystemOutput(bFlag);

	// return value
	return(nVal);
}

// PurgeDialogs()

void PurgeDialogs(void)
{
	// check counter
	if(g_dwNumDlg)
	{
		// counter
		DWORD dwCount;

		// release all data
		for(dwCount=0;dwCount<g_dwNumDlg;dwCount++)
			FreeMem((LPVOID*)&g_lplpDlgData[dwCount]);
	}

	// free memory
	FreeMem((LPVOID*)&g_lplpDlgData);
	FreeMem((LPVOID*)&g_lpdwDlgDataSize);
	FreeMem((LPVOID*)&g_lpdwDlgDataPtr);
	FreeMem((LPVOID*)&g_lpnDlgResID);

	// reset counters
	g_dwNumDlg=0;
	g_dwMaxDlg=0;
}

// AddDialogCoords()

void AddDialogCoords(DWORD dwIndex,
					 short sX,
					 short sY)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogShort()",
								 FALSE);
		return;
	}
#endif

	// add coordinates
	AddDialogShort(dwIndex,sX);
	AddDialogShort(dwIndex,sY);
}

// AddDialogControl()

void AddDialogControl(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogControl()",
								 FALSE);
		return;
	}
#endif

	// pointer to current number
	short* lpsNum=(short*)(g_lplpDlgData[dwIndex]+(sizeof(DWORD)*2));

	// increment value
	(*lpsNum)++;
}

// AddDialogPushButton()

void AddDialogPushButton(DWORD dwIndex,
						 short sXPos,
						 short sYPos,
						 short sXSize,
						 short sYSize,
						 LPCSTR lpText,
						 WORD wID,
						 BOOL bDefBtn)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogPushButton()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	if(bDefBtn)
		AddDialogDWORD(dwIndex,WS_CHILD|
							   WS_VISIBLE|
							   WS_TABSTOP|
							   BS_DEFPUSHBUTTON); // style
	else
		AddDialogDWORD(dwIndex,WS_CHILD|
							   WS_VISIBLE|
							   WS_TABSTOP|
							   BS_PUSHBUTTON); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0080); // button class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,(LPSTR)lpText); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogCheckBox()

void AddDialogCheckBox(DWORD dwIndex,
					   short sXPos,
					   short sYPos,
					   short sXSize,
					   short sYSize,
					   LPCSTR lpText,
					   WORD wID)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogCheckBox()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   WS_TABSTOP|
						   BS_AUTOCHECKBOX); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0080); // button class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,(LPSTR)lpText); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogRadioButton()

void AddDialogRadioButton(DWORD dwIndex,
						  short sXPos,
						  short sYPos,
						  short sXSize,
						  short sYSize,
						  LPCSTR lpText,
						  WORD wID)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogRadioButton()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   WS_TABSTOP|
						   BS_AUTORADIOBUTTON); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0080); // button class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,(LPSTR)lpText); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogGroupBox()

void AddDialogGroupBox(DWORD dwIndex,
					   short sXPos,
					   short sYPos,
					   short sXSize,
					   short sYSize,
					   LPCSTR lpText)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogGroupBox()",
								 FALSE);
		return;
	}
#endif

	// add group
	AddDialogGroup(dwIndex);
	
	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   BS_GROUPBOX); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,NULL); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0080); // button class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,(LPSTR)lpText); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogDropDownListBox()

void AddDialogDropDownListBox(DWORD dwIndex,
							  short sXPos,
							  short sYPos,
							  short sXSize,
							  short sYSize,
						      WORD wID)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogDropDownListBox()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   WS_TABSTOP|
						   CBS_SORT|
						   CBS_DROPDOWNLIST); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0085); // combo box class
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogEditControl()

void AddDialogEditControl(DWORD dwIndex,
						  short sXPos,
						  short sYPos,
						  short sXSize,
						  short sYSize,
						  WORD wID)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogEditControl()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   WS_TABSTOP|
						   WS_BORDER|
						   ES_AUTOHSCROLL); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0081); // edit control class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,""); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogListBox()

void AddDialogListBox(DWORD dwIndex,
					  short sXPos,
					  short sYPos,
					  short sXSize,
					  short sYSize,
					  WORD wID)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogListBox()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   WS_TABSTOP|
						   LBS_SORT); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0083); // list box class
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogStaticControl()

void AddDialogStaticControl(DWORD dwIndex,
							short sXPos,
							short sYPos,
							short sXSize,
							short sYSize,
							LPCSTR lpText)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogStaticControl()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   SS_CENTER); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,NULL); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0082); // static control class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,(LPSTR)lpText); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogGroup()

void AddDialogGroup(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogGroup()",
								 FALSE);
		return;
	}
#endif

	// add empty static control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,0,0); // position
	AddDialogCoords(dwIndex,0,0); // size
	AddDialogWORD(dwIndex,NULL); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0082); // static control class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,""); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// SetCurrentDialogCheckBox()

BOOL SetCurrentDialogCheckBox(WORD wID,
							  BOOL bVal)
{
ANTIHACK_7

	// set flag
	UINT nFlag;
	if(bVal)
		nFlag=BST_CHECKED;
	else
		nFlag=BST_UNCHECKED;
	
	// set button
	if(!CheckDlgButton(g_hCurrDlg,
					   (int)wID,
					   nFlag))
	{
		DisplayErrorMessage("Unable to set dialog button state.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// GetCurrentDialogCheckBox()

BOOL GetCurrentDialogCheckBox(WORD wID)
{
	// check button
	if(IsDlgButtonChecked(g_hCurrDlg,
						  (int)wID)==BST_CHECKED)
		return(TRUE);
	else
		return(FALSE);
}

// SetCurrentDialogEditText()

BOOL SetCurrentDialogEditText(WORD wID,
							  LPSTR lpText)
{
	// set value
	if(!SetDlgItemText(g_hCurrDlg,
					   (int)wID,
					   lpText))
	{
		DisplayErrorMessage("Unable to set dialog edit control text.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// GetCurrentDialogEditText()

BOOL GetCurrentDialogEditText(WORD wID,
							  LPSTR lpText,
							  int nMaxCount)
{
	// get value
	if(!GetDlgItemText(g_hCurrDlg,
					   (int)wID,
					   lpText,
					   nMaxCount))
		return(FALSE);

	// ok
	return(TRUE);
}

// SetCurrentDialogEditFloat()

BOOL SetCurrentDialogEditFloat(WORD wID,
							   float fVal)
{
	// just to verify
	LockGlobalMsg()
	UnlockGlobalMsg()

	// convert value to string
	sprintf(g_lpMsg,
			"%f",
			fVal);

	// set text
	return(SetCurrentDialogEditText(wID,
									g_lpMsg));
}

// GetCurrentDialogEditFloat()

float GetCurrentDialogEditFloat(WORD wID)
{
	// just to verify
	LockGlobalMsg()
	UnlockGlobalMsg()

	// get text
	if(GetCurrentDialogEditText(wID,
								g_lpMsg,
								_MAX_PATH))
		return((float)atof(g_lpMsg));
	else
		return(0);
}

// SetCurrentDialogBox()

void SetCurrentDialogBox(HWND hDlg)
{
	// save value
	g_hCurrDlg=hDlg;
}

// DivideString()

DWORD DivideString(LPSTR lpStr)
{
	// counter
	DWORD dwCount;

	// number of strings
	DWORD dwNumStrings=1;

	// get length of string
	DWORD dwLen=(DWORD)strlen(lpStr);

	// divide string at each "&"
	for(dwCount=0;dwCount<dwLen;dwCount++)
		if(lpStr[dwCount]=='&')
			{ lpStr[dwCount]=NULL; dwNumStrings++; }

	// return number of strings
	return(dwNumStrings);
}

// GetStringDivision()

LPSTR GetStringDivision(LPSTR lpStr,
					    DWORD dwIndex)
{
	ANTIHACK_8
		
	// position pointer
	DWORD dwPos=0;
	
	// loop until specified string
	while(dwIndex)
	{
		// find next string
		dwPos+=(DWORD)(strlen(&lpStr[dwPos])+1);
		
		// decrement index
		dwIndex--;
	}

	// return pointer
	return(&lpStr[dwPos]);
}

// DisplayMessageBox()

int DisplayMessageBox(HWND hWnd,
					  LPSTR lpText,
					  LPSTR lpCaption,
					  UINT uType)
{
	// return value
	int nVal;

	// extended caption string
	char lpXCaption[64];
	
	// begin system output
	BOOL bFlag=BeginSystemOutput();

	// add necessary flags
	uType=uType|MB_SETFOREGROUND|MB_TOPMOST;

	// copy caption string
	strcpy(lpXCaption,lpCaption);

#ifdef SHAREWARE_MODE
	// append "trial version" to caption
	strcat(lpXCaption," (Trial Version)");
#endif

	// ignore window handle & display message box
	nVal=MessageBox(NULL,
					lpText,
					lpXCaption,
					uType);

	// end system output
	EndSystemOutput(bFlag);

	// return value
	return(nVal);
}

#ifndef ALLOW_MULTIPLE_INSTANCES
// LockApplication()

BOOL LockApplication(LPCSTR lpID,
					 LPSTR lpWndTitle,
					 LPSTR lpWndClass,
					 BOOL bClose)
{
	// note: this function has been re-written as follows:
	//       no mutex-object lock is used. the function only
	//       checks for the requested window. if the window
	//       is found, it is either closed or activiated,
	//       depending on the bClose flag.

#ifdef DEBUG_MODE
	// verify strings
	if(!lpWndTitle||
	   !lpWndClass)
	{
		DisplayErrorMessage("Invalid string value.",
							"LockApplication()",
							FALSE);
		return(FALSE);
	}
#endif

	// window object
	HWND hWnd=(HWND)INVALID_HANDLE_VALUE;

	// find window
	hWnd=FindWindow((LPCSTR)lpWndClass,
					(LPCSTR)lpWndTitle);

	// check window
	if(hWnd==NULL)
		return(TRUE);

	// check close flag
	if(bClose)
	{
		// close other window
		SendMessage(hWnd,
					WM_CLOSE,
					NULL,
					NULL);

		// ok
		return(TRUE);
	}
	else
	{
		// set other to foreground
		SetForegroundWindow(hWnd);

		// close this app
		return(FALSE);
	}
}

// UnlockApplication()

void UnlockApplication(void)
{
	// see comments under LockApplication()
}
#endif

// AddApplicationWindowClass()

BOOL AddApplicationWindowClass(void)
{
	LoadingOutput("AddApplicationWindowClass");

	// create class for this window
	WNDCLASSEX wcx;
	wcx.cbSize=sizeof(WNDCLASSEX);
	wcx.style=CS_OWNDC;
	wcx.lpfnWndProc=WindowProc;
	wcx.cbClsExtra=NULL;
	wcx.cbWndExtra=NULL;
	wcx.hInstance=g_hInst;
	if(ApplicationIcon())
	{
		wcx.hIcon=LoadIcon(g_hInst,ApplicationIcon());
		wcx.hIconSm=LoadIcon(g_hInst,ApplicationIcon());
	}
	else
	{
		wcx.hIcon=LoadIcon(NULL,IDI_APPLICATION);
		wcx.hIconSm=LoadIcon(NULL,IDI_APPLICATION);
	}
	if(ApplicationCursor())
		wcx.hCursor=LoadCursor(g_hInst,ApplicationCursor());
	else
		wcx.hCursor=LoadCursor(NULL,IDC_ARROW);
	wcx.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName=ApplicationMenu();
	wcx.lpszClassName=ApplicationID();

	// register window class
	if(!RegisterClassEx(&wcx))
	{
		DisplayErrorMessage("Unable to register window class with operating system.");
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// RemoveApplicationWindowClass()

void RemoveApplicationWindowClass(void)
{
	// check handle & un-register class
	if(g_hInst!=INVALID_HANDLE_VALUE)
		if(UnregisterClass(ApplicationID(),g_hInst))
			RemoveWinResource;
}

// CreateApplicationWindow()

BOOL CreateApplicationWindow(void)
{
	LoadingOutput("CreateApplicationWindow");

ANTIHACK_9
	
	// add window class
	if(!AddApplicationWindowClass())
		return(NULL);

	// set system display mode
	SetSystemDisplayMode();

	LockGlobalMsg()

	// copy application name
	strcpy(g_lpMsg,ApplicationName());

#ifdef SHAREWARE_MODE
	// append "trial version" to name
	strcat(g_lpMsg," (Trial Version)");
#endif

	// create application window (invisible at first)
	g_hWnd=CreateWindowEx(NULL,
						  ApplicationID(),
						  g_lpMsg,
						  WS_BORDER|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
						  0,0,
						  g_dmSys.dwX,
						  g_dmSys.dwY,
						  NULL,
						  NULL,
						  g_hInst,
						  NULL);

	UnlockGlobalMsg()

	// check window handle
	if(g_hWnd==(HWND)INVALID_HANDLE_VALUE||
	   g_hWnd==NULL)
	{
		DisplayErrorMessage("Unable to create application window.");
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// DestroyApplicationWindow()

void DestroyApplicationWindow(void)
{
	// check handle & destroy window
	if(g_hWnd!=(HWND)INVALID_HANDLE_VALUE&&
	   g_hWnd!=NULL)
		if(DestroyWindow(g_hWnd))
		{
			// reset handle
			g_hWnd=(HWND)INVALID_HANDLE_VALUE;
			
			// remove resource
			RemoveWinResource;
		}

	// remove window class
	RemoveApplicationWindowClass();
}

// SetSystemDisplayMode()

void SetSystemDisplayMode(DWORD dwX,
						  DWORD dwY)
{
	// fill in structure
	if(dwX)
	{
		g_dmSys.dwX=dwX;
		g_dmSys.dwY=dwY;
	}
	else
	{
		g_dmSys.dwX=GetSystemMetrics(SM_CXSCREEN);
		g_dmSys.dwY=GetSystemMetrics(SM_CYSCREEN);
	}
	g_dmSys.fmt=GetSystemDisplayFormat();
	g_dmSys.bWindowed=TRUE;
	g_dmSys.bLockable=FALSE;
	g_dmSys.dwBackBufferCount=0;
}

// CalculateWindowMetrics()

void CalculateWindowMetrics(void)
{
	// get application display mode
	g_dmApp=ApplicationDisplayMode();

	// compute window size and position
	g_nXSize=g_dmApp.dwX+(GetSystemMetrics(SM_CXFIXEDFRAME)*2);
	g_nYSize=g_dmApp.dwY+(GetSystemMetrics(SM_CYFIXEDFRAME)*2)+GetSystemMetrics(SM_CYCAPTION);
	g_nXPos=(g_dmSys.dwX-g_nXSize)/2;
	g_nYPos=((g_dmSys.dwY-g_nYSize)/2)-GetSystemMetrics(SM_CYCAPTION);
}

// SetDataPathRel()

BOOL SetDataPathRel(LPCSTR lpDir,
					BOOL bSupressErrorMsg)
{
	LoadingOutput("SetDataPathRel");

	LockGlobalMsg()

	// get current directory
	if(!getcwd(g_lpMsg,_MAX_PATH-1))
	{
		if(!bSupressErrorMsg)
			DisplayErrorMessage("Unable to read current working directory.");
		return(FALSE);
	}

	// add directory to current path
	strcat(g_lpMsg,"\\");
	strcat(g_lpMsg,lpDir);

	// set directory
	if(chdir(g_lpMsg)!=0)
	{
		if(!bSupressErrorMsg)
			DisplayErrorMessage("Unable to set current working directory.");
		UnlockGlobalMsg()
		return(FALSE);
	}

	UnlockGlobalMsg()

	// ok
	return(TRUE);
}

// SetDataPathAbs()

BOOL SetDataPathAbs(LPCSTR lpDir,
					BOOL bSupressErrorMsg)
{
	LoadingOutput("SetDataPathAbs");

	// set directory
	if(chdir(lpDir)!=0)
	{
		if(!bSupressErrorMsg)
			DisplayErrorMessage("Unable to set current working directory.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// ProcessPendingMessages()

BOOL ProcessPendingMessages(void)
{
	// message storage
	MSG Msg;

	// retrieve message
	while(PeekMessage(&Msg,
					  NULL,
					  NULL,
					  NULL,
					  PM_REMOVE))
	{
		// check for quit message
		if(Msg.message==WM_QUIT)
			return(FALSE);
		
		// process accelerator keys
		if(!TranslateAccelerator(g_hWnd,
								 g_hAccel,
								 &Msg))
		{
			// process message
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}

	// check for idle mode
	if(g_bIdle)
		WaitMessage();

	// ok
	return(TRUE);
}

// ProcessMouseTracking()

void ProcessMouseTracking(void)
{
	// client area flag
	static BOOL bMouseWasInClient=FALSE;
	
	// check flag
	if(g_bWinMouse)
	{
		// compute delta-position values
		g_nMouseDX=g_nMouseX-g_nMousePX;
		g_nMouseDY=g_nMouseY-g_nMousePY;
		
		// store previous position values
		g_nMousePX=g_nMouseX;
		g_nMousePY=g_nMouseY;

		// check buttons
		g_bMouseBtnL=IsKeyDown(VK_LBUTTON);
		g_bMouseBtnR=IsKeyDown(VK_RBUTTON);
#ifdef DEBUG_MODE
		g_bMouseBtnS=IsKeyDown(VK_SHIFT);
		g_bMouseBtnC=IsKeyDown(VK_CONTROL);
#endif

		// set click flags
		if(!g_bMousePBtnL&&
		   g_bMouseBtnL)
			g_bMouseClkL=TRUE;
		else
			g_bMouseClkL=FALSE;
		if(!g_bMousePBtnR&&
		   g_bMouseBtnR)
			g_bMouseClkR=TRUE;
		else
			g_bMouseClkR=FALSE;
#ifdef DEBUG_MODE
		if(!g_bMousePBtnS&&
		   g_bMouseBtnS)
			g_bMouseClkS=TRUE;
		else
			g_bMouseClkS=FALSE;
		if(!g_bMousePBtnC&&
		   g_bMouseBtnC)
			g_bMouseClkC=TRUE;
		else
			g_bMouseClkC=FALSE;
#endif
		
		// store previous buttons
		g_bMousePBtnL=g_bMouseBtnL;
		g_bMousePBtnR=g_bMouseBtnR;
#ifdef DEBUG_MODE
		g_bMousePBtnS=g_bMouseBtnS;
		g_bMousePBtnC=g_bMouseBtnC;
#endif

		// check for windowed mode
		if(g_pp.Windowed)
		{
			// check mouse against client area
			if(g_nMouseX<0||
			   g_nMouseX>=640||
			   g_nMouseY<0||
			   g_nMouseY>=480)
			{
				// check flag
				if(bMouseWasInClient)
				{
					// show cursor
					ShowWindowsCursor();

					// set flag
					bMouseWasInClient=FALSE;
				}
			}
			else
			{
				// check flag
				if(!bMouseWasInClient)
				{
					// hide cursor
					HideWindowsCursor();

					// set flag
					bMouseWasInClient=TRUE;
				}
			}
		}
	}
}

// BeginExclusiveMode()

void BeginExclusiveMode(void)
{
#ifndef SCREENSAVER_MODE
	// prevent system interference
	SetThreadExecutionState(ES_SYSTEM_REQUIRED|
							ES_DISPLAY_REQUIRED|
							ES_CONTINUOUS);
#endif

	// check application flag
	if(ApplicationPriorityMode())
	{
		// set to high priority
		SetThreadPriority(GetCurrentThread(),
						  THREAD_PRIORITY_ABOVE_NORMAL);
	}
	else
	{
		// set to normal priority
		SetThreadPriority(GetCurrentThread(),
						  THREAD_PRIORITY_NORMAL);
	}
}

// EndExclusiveMode()

void EndExclusiveMode(void)
{
	// set to idle priority
	SetThreadPriority(GetCurrentThread(),
					  THREAD_PRIORITY_IDLE);

#ifndef SCREENSAVER_MODE
	// release execution state
	SetThreadExecutionState(ES_CONTINUOUS);
#endif
}

// MinimizeApplicationWindow()

void MinimizeApplicationWindow(void)
{
	// send minimize message
	if(g_hWnd!=(HWND)INVALID_HANDLE_VALUE&&
	   g_hWnd!=NULL)
		SendMessage(g_hWnd,
					WM_SYSCOMMAND,
					SC_MINIMIZE,
					NULL);
}

#ifdef DEBUG_MODE
// IsValidScriptChar()

BOOL IsValidScriptChar(char c)
{
	// compare with all valid characters
	if(c>='A'&&
	   c<='Z')
		return(TRUE);
	if(c>='a'&&
	   c<='z')
		return(TRUE);
	if(c>='0'&&
	   c<='9')
		return(TRUE);
	if(c=='_')
		return(TRUE);
	if(c=='-')
		return(TRUE);
	if(c=='.')
		return(TRUE);
	if(c=='#')
		return(TRUE);

	// no good
	return(FALSE);
}
#endif

// BeginSystemOutput()

BOOL BeginSystemOutput(void)
{
	// return flag
	BOOL bFlag=FALSE;

	// ensure direct3d cooperation
	if(g_lpD3DDevice)
	{
		// check scene
		if(g_bInScene)
		{
			// set flag
			bFlag=TRUE;

			// end scene
			EndScene();
		}

		// check full-screen flag
		if(!g_pp.Windowed)
		{
			// force device to reset
			ResetRenderDevice();

			// minimize window
			MinimizeApplicationWindow();
		}
	}

	// show cursor
	ShowWindowsCursor();

	// return flag
	return(bFlag);
}

// EndSystemOutput()

void EndSystemOutput(BOOL bFlag)
{
	// check initialized flag
	if(g_bAppInit)
	{
		// hide cursor
		HideWindowsCursor();

		// make sure window is restored
		RestoreApplicationWindow();

		// reset direct3d as needed
		if(g_lpD3DDevice)
		{
			// begin scene if necessary
			if(bFlag)
				BeginScene();
		}
	}
}

// UpdateWindowMetrics()

void UpdateWindowMetrics(void)
{
ANTIHACK_10

	// calculate new metrics
	CalculateWindowMetrics();

	// update window
	SetWindowPos(g_hWnd,
				 NULL,
				 g_nXPos,
				 g_nYPos,
				 g_nXSize,
				 g_nYSize,
				 SWP_NOZORDER);
}

// RestoreApplicationWindow()

void RestoreApplicationWindow(void)
{
	// send restore message
	if(g_hWnd!=(HWND)INVALID_HANDLE_VALUE&&
	   g_hWnd!=NULL)
		SendMessage(g_hWnd,
					WM_SYSCOMMAND,
					SC_RESTORE,
					NULL);
}

// InitAcceleratorTable()

BOOL InitAcceleratorTable(void)
{
	LoadingOutput("InitAcceleratorTable");

	// set alt+enter accelerator
	g_acAltEnter.fVirt=FALT|FVIRTKEY;
	g_acAltEnter.key=VK_RETURN;
	g_acAltEnter.cmd=IDM_TOGGLEFULLSCREEN;

	// create table
	g_hAccel=CreateAcceleratorTable(&g_acAltEnter,
									1);

	// check for error
	if(g_hAccel==INVALID_HANDLE_VALUE||
	   g_hAccel==NULL)
	{
		DisplayErrorMessage("Unable to create accelerator key table.");
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// ReleaseAcceleratorTable()

void ReleaseAcceleratorTable(void)
{
	// check handle and destroy table
	if(g_hAccel!=INVALID_HANDLE_VALUE&&
	   g_hAccel!=NULL)
		if(DestroyAcceleratorTable(g_hAccel))
		{
			// reset handle
			g_hAccel=(HACCEL)INVALID_HANDLE_VALUE;
			
			// remove resource
			RemoveWinResource;
		}
}

// ShowWindowsCursor()

void ShowWindowsCursor(void)
{
	// set cursor as needed
	if(ApplicationCursor())
		SetCursor(LoadCursor(g_hInst,ApplicationCursor()));
	else
		SetCursor(LoadCursor(NULL,IDC_ARROW));

	// show cursor
	while(ShowCursor(TRUE)<0)
	{}

	// release mouse
	ReleaseCapture();
}

// HideWindowsCursor()

void HideWindowsCursor(void)
{
	// hide cursor
	while(ShowCursor(FALSE)>=0)
	{}

	// set cursor to null
	SetCursor(NULL);

	// capture mouse
	SetCapture(g_hWnd);
}

// CapturePauseBitmap()

BOOL CapturePauseBitmap(D3DCOLOR clrFilter)
{
#ifndef SCREENSAVER_MODE
	// counters
	DWORD dwX,dwY;
	
	// check direct3d
	if(g_lpD3DDevice&&
	   g_dmApp.bWindowed)
	{
		// allocate memory for image
		if(!AllocMem((LPVOID*)&g_lpPauseBmpData,
					 sizeof(BYTE)*g_rClient.dwX*g_rClient.dwY*3))
			return(FALSE);

		// prepare bitmap header
		g_bih.biSize=sizeof(BITMAPINFOHEADER);
		g_bih.biWidth=(long)g_rClient.dwX;
		g_bih.biHeight=(long)g_rClient.dwY;
		g_bih.biPlanes=1;
		g_bih.biBitCount=24;
		g_bih.biCompression=BI_RGB;
		g_bih.biSizeImage=NULL;
		g_bih.biXPelsPerMeter=11811;
		g_bih.biYPelsPerMeter=11811;
		g_bih.biClrUsed=0;
		g_bih.biClrImportant=0;

		// get filter values
		float fFilterR=GetR888(clrFilter);
		float fFilterG=GetG888(clrFilter);
		float fFilterB=GetB888(clrFilter);
		float fFilterA=GetA888(clrFilter);
		fFilterR*=fFilterA;
		fFilterG*=fFilterA;
		fFilterB*=fFilterA;

		// lock back buffer
		D3DLOCKED_RECT lr=LockBackBuffer();
		LPWORD lpwData=(LPWORD)lr.pBits;
		LPDWORD lpdwData=(LPDWORD)lr.pBits;
		DWORD dwPitch=lr.Pitch;

		// check format
		switch(g_dmApp.fmt)
		{
		case(D3DFMT_R5G6B5):

			// set bits
			for(dwY=0;dwY<g_rClient.dwY;dwY++)
				for(dwX=0;dwX<g_rClient.dwX;dwX++)
				{
					// read color values
					WORD wC=lpwData[dwX+(((g_rClient.dwY-1)-dwY)*(dwPitch>>1))];
					DWORD dwC=lpdwData[dwX+(((g_rClient.dwY-1)-dwY)*(dwPitch>>2))];

					// color components
					float fR,fG,fB,fA;

					// exract components
					fR=GetR565(wC);
					fG=GetG565(wC);
					fB=GetB565(wC);

					// compute grayscale value
					fA=(fR+fG+fB)/3;
					
					// filter components
					fR=fFilterR*fA;
					fG=fFilterG*fA;
					fB=fFilterB*fA;

					// set bitmap bits
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)  ]=(BYTE)(fB*255);
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)+1]=(BYTE)(fG*255);
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)+2]=(BYTE)(fR*255);
				}
				
			// ok				
			break;

		case(D3DFMT_X1R5G5B5):

			// set bits
			for(dwY=0;dwY<g_rClient.dwY;dwY++)
				for(dwX=0;dwX<g_rClient.dwX;dwX++)
				{
					// read color values
					WORD wC=lpwData[dwX+(((g_rClient.dwY-1)-dwY)*(dwPitch>>1))];
					DWORD dwC=lpdwData[dwX+(((g_rClient.dwY-1)-dwY)*(dwPitch>>2))];

					// color components
					float fR,fG,fB,fA;

					// exract components
					fR=GetR555(wC);
					fG=GetG555(wC);
					fB=GetB555(wC);

					// compute grayscale value
					fA=(fR+fG+fB)/3;
					
					// filter components
					fR=fFilterR*fA;
					fG=fFilterG*fA;
					fB=fFilterB*fA;

					// set bitmap bits
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)  ]=(BYTE)(fB*255);
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)+1]=(BYTE)(fG*255);
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)+2]=(BYTE)(fR*255);
				}

			// ok
			break;

		case(D3DFMT_X4R4G4B4):

			// set bits
			for(dwY=0;dwY<g_rClient.dwY;dwY++)
				for(dwX=0;dwX<g_rClient.dwX;dwX++)
				{
					// read color values
					WORD wC=lpwData[dwX+(((g_rClient.dwY-1)-dwY)*(dwPitch>>1))];
					DWORD dwC=lpdwData[dwX+(((g_rClient.dwY-1)-dwY)*(dwPitch>>2))];

					// color components
					float fR,fG,fB,fA;

					// exract components
					fR=GetR444(wC);
					fG=GetG444(wC);
					fB=GetB444(wC);

					// compute grayscale value
					fA=(fR+fG+fB)/3;
					
					// filter components
					fR=fFilterR*fA;
					fG=fFilterG*fA;
					fB=fFilterB*fA;

					// set bitmap bits
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)  ]=(BYTE)(fB*255);
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)+1]=(BYTE)(fG*255);
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)+2]=(BYTE)(fR*255);
				}

			// ok
			break;

		case(D3DFMT_X8R8G8B8):

			// set bits
			for(dwY=0;dwY<g_rClient.dwY;dwY++)
				for(dwX=0;dwX<g_rClient.dwX;dwX++)
				{
					// read color values
					WORD wC=lpwData[dwX+(((g_rClient.dwY-1)-dwY)*(dwPitch>>1))];
					DWORD dwC=lpdwData[dwX+(((g_rClient.dwY-1)-dwY)*(dwPitch>>2))];

					// color components
					float fR,fG,fB,fA;

					// exract components
					fR=GetR888(dwC);
					fG=GetG888(dwC);
					fB=GetB888(dwC);
					break;

					// compute grayscale value
					fA=(fR+fG+fB)/3;
					
					// filter components
					fR=fFilterR*fA;
					fG=fFilterG*fA;
					fB=fFilterB*fA;

					// set bitmap bits
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)  ]=(BYTE)(fB*255);
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)+1]=(BYTE)(fG*255);
					g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)+2]=(BYTE)(fR*255);
				}

			// ok
			break;
		}

		// unlock back buffer
		UnlockBackBuffer();
	}	
#endif

	// redraw window
	if(!RedrawWindow(g_hWnd,
					 NULL,
					 NULL,
					 RDW_INTERNALPAINT))
		return(FALSE);

	// ok
	return(TRUE);
}

// ReleasePauseBitmap()

void ReleasePauseBitmap(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpPauseBmpData);
}

#ifdef DEBUG_MODE
// OpenDataCompiler()

BOOL OpenDataCompiler(LPCSTR lpFile)
{
	LoadingOutput("OpenDataCompiler");

	// check handle
	if(g_hDataFile!=INVALID_HANDLE_VALUE)
	{
		DisplayFatalErrorMessage("Unable to open multiple data files.",
								 NULL,
								 FALSE);
		return(FALSE);
	}

	// open output file
	g_hDataFile=CreateFile(lpFile,
						   GENERIC_WRITE,
						   NULL,
						   NULL,
						   CREATE_ALWAYS,
						   FILE_ATTRIBUTE_NORMAL|
						   FILE_FLAG_SEQUENTIAL_SCAN,
						   NULL);

	// check for error
	if(!g_hDataFile)
	{
		g_hScriptFile=INVALID_HANDLE_VALUE;
		DisplayFatalErrorMessage("Unable to create data file.",
								 (LPSTR)lpFile,
								 FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// AddDataFileRef()

DWORD AddDataFileRef(LPCSTR lpFile)
{
	LoadingOutput("AddDataFileRef");

	// check counter
	if(g_dwNumDataFileRef==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpDataFileRefName,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDataFileRef=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumDataFileRef>=g_dwMaxDataFileRef)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpDataFileRefName,
					  sizeof(LPSTR)*g_dwMaxDataFileRef,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDataFileRef+=BLOCK_ALLOC_INC;
	}

	// save index
	DWORD dwIndex=g_dwNumDataFileRef;

	// increment counter
	g_dwNumDataFileRef++;

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpDataFileRefName[dwIndex],
				 strlen(lpFile)+1))
		return(NO_OBJECT);

	// copy name
	strcpy(g_lplpDataFileRefName[dwIndex],
		   lpFile);
	
	// ok	
	return(dwIndex);
}

// CloseDataCompiler()

void CloseDataCompiler(LPCSTR lpFile)
{
	// number of bytes
	DWORD dwBytesRead;
	DWORD dwBytesWritten;

	// counter
	DWORD dwCount;

	// data file header
	DATAFILEHEADER dfh;

	// data file pointer
	DWORD dwPos;

	// compute data file size
	dfh.dwSize=0;
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
	{
		// add name
		dfh.dwSize+=strlen(g_lplpDataFileRefName[dwCount])+1;

		// add offset
		dfh.dwSize+=sizeof(DWORD);

		// add size of file
		dfh.dwSize+=sizeof(DWORD);

		// add file
		dfh.dwSize+=GetFileLength(g_lplpDataFileRefName[dwCount]);
	}

	// allocate data file memory
	if(!AllocMem((LPVOID*)&g_lpDataFile,
				 dfh.dwSize))
				 return;
	
	// add number of references
	dfh.dwNumRef=g_dwNumDataFileRef;
	
	// reset position
	dwPos=0;

	// add file names
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
	{
		// copy name
		strcpy((LPSTR)&g_lpDataFile[dwPos],
			   g_lplpDataFileRefName[dwCount]);

		// increment pointer
		dwPos+=strlen(g_lplpDataFileRefName[dwCount])+1;
	}

	// save position of offset table
	LPDWORD lpOffsetTable=(LPDWORD)&g_lpDataFile[dwPos];
	dfh.dwOffsetPos=dwPos;

	// skip offset table
	dwPos+=sizeof(DWORD)*g_dwNumDataFileRef;

	// save position of size table
	LPDWORD lpSizeTable=(LPDWORD)&g_lpDataFile[dwPos];
	dfh.dwSizePos=dwPos;

	// skip size table
	dwPos+=sizeof(DWORD)*g_dwNumDataFileRef;

	// save position of data table
	dfh.dwDataPos=dwPos;

	// add files
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
	{
		// set offset
		lpOffsetTable[dwCount]=dwPos;

		// set size
		lpSizeTable[dwCount]=GetFileLength(g_lplpDataFileRefName[dwCount]);

		// open file
		HANDLE hFile;
		hFile=CreateFile(g_lplpDataFileRefName[dwCount],
						 GENERIC_READ,
						 NULL,
						 NULL,
						 OPEN_EXISTING,
						 FILE_ATTRIBUTE_NORMAL|
						 FILE_FLAG_SEQUENTIAL_SCAN,
						 NULL);

		// check for error
		if(!hFile)
		{
			DisplayFatalErrorMessage("Unable to open reference file.",
									 g_lplpDataFileRefName[dwCount],
									 FALSE);
			return;
		}
		else
			AddWinResource;
		
		// read file
		ReadFile(hFile,
				 (LPVOID)&g_lpDataFile[dwPos],
				 lpSizeTable[dwCount],
				 &dwBytesRead,
				 NULL);

		// check for error
		if(dwBytesRead!=lpSizeTable[dwCount])
		{
			DisplayFatalErrorMessage("Unable to read data file.",
									 g_lplpDataFileRefName[dwCount]);
			return;
		}

		// increment pointer
		dwPos+=lpSizeTable[dwCount];

		// close file
		if(!CloseHandle(hFile))
		{
			DisplayFatalErrorMessage("Unable to reference file.",
									 g_lplpDataFileRefName[dwCount]);
			return;
		}
		else
			RemoveWinResource;
	}

	// output header
	WriteFile(g_hDataFile,
			  (LPVOID)&dfh,
			  sizeof(DATAFILEHEADER),
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=sizeof(DATAFILEHEADER))
	{
		DisplayFatalErrorMessage("Unable to output data file header.");
		return;
	}

	// output data
	WriteFile(g_hDataFile,
			  (LPVOID)g_lpDataFile,
			  dfh.dwSize,
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=dfh.dwSize)
	{
		DisplayFatalErrorMessage("Unable to output data file.");
		return;
	}

	// free data file memory
	FreeMem((LPVOID*)&g_lpDataFile);
	
	// close data file
	if(!CloseHandle(g_hDataFile))
	{
		DisplayFatalErrorMessage("Unable to close data file.");
		return;
	}
	else
	{	
		// reset handle
		g_hDataFile=INVALID_HANDLE_VALUE;

		// remove resource
		RemoveWinResource;
	}

	// free storage
	if(g_dwNumDataFileRef)
	{
		// release names
		for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
			FreeMem((LPVOID*)&g_lplpDataFileRefName[dwCount]);

		// free memory
		FreeMem((LPVOID*)&g_lplpDataFileRefName);
		
		// reset counters
		g_dwNumDataFileRef=0;
		g_dwMaxDataFileRef=0;
	}

	// compress file
	if(!CompressDataFile(lpFile))
		ForceImmediateExit();
}

// CompressDataFile()

BOOL CompressDataFile(LPCSTR lpFile)
{
	LockGlobalMsg()

	// create command line
	sprintf(g_lpMsg,
			"/r %s",
			lpFile);
	
	// compress file
	if(!ExecuteCommandLine("compress310.exe",
						   g_lpMsg))
	{
		UnlockGlobalMsg()
		return(FALSE);
	}

	// wait for user
	DisplayInfoMessage("Click OK when compression is complete.");

	// copy filename
	strcpy(g_lpMsg,lpFile);

	// replace last character
	g_lpMsg[strlen(g_lpMsg)-1]='_';

	// copy file
	if(!CopyFile(g_lpMsg,
				 lpFile,
				 FALSE))
	{
		DisplayErrorMessage("Unable to copy file.",
							"CompressDataFile()",
							TRUE);
		UnlockGlobalMsg()
		return(FALSE);
	}

	// delete old file
	if(!DeleteFile(g_lpMsg))
	{
		DisplayErrorMessage("Unable to delete file.",
							"CompressDataFile()",
							TRUE);
		UnlockGlobalMsg()
		return(FALSE);
	}

	// add timestamp bytes
	if(!AddTimeStampToDataFile(lpFile))
	{
		UnlockGlobalMsg()
		return(FALSE);
	}

	// wait for user
	DisplayInfoMessage("Click OK after transfering data file(s).");

	UnlockGlobalMsg()

	// ok
	return(TRUE);
}

// AddTimeStampToDataFile()

BOOL AddTimeStampToDataFile(LPCSTR lpFile)
{
	// now wait just a second!
	Sleep(1000);	

	// handle to file
	HANDLE hFile=INVALID_HANDLE_VALUE;

	// counter
	DWORD dwCount;

	// byte counter
	DWORD dwBytesWritten;

	LockGlobalMsg()

	// prepare output string
	g_lpMsg[0]='6';
	g_lpMsg[1]=NULL;

	// prepare remaining bytes
	for(dwCount=2;dwCount<32;dwCount++)
		g_lpMsg[dwCount]=(BYTE)(GetRandomInt()&0xFF);

	// open file
	hFile=CreateFile(lpFile,
					 GENERIC_WRITE,
					 NULL,
					 NULL,
					 OPEN_EXISTING,
					 NULL,
					 NULL);

	// check handle
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		AddWinResource;

		// set to end
		if(SetFilePointer(hFile,
						  0,
						  NULL,
						  FILE_END)==NO_OBJECT)
		{
			DisplayErrorMessage("Unable to set file pointer:",
								(LPSTR)lpFile,
								FALSE);
			if(CloseHandle(hFile))
				RemoveWinResource;
			UnlockGlobalMsg()
			return(FALSE);
		}

		// output bytes
		WriteFile(hFile,
				  g_lpMsg,
				  32,
				  &dwBytesWritten,
				  NULL);

		// close file
		if(CloseHandle(hFile))
			RemoveWinResource;

		// check for error
		if(dwBytesWritten!=32)
		{
			UnlockGlobalMsg()
			return(FALSE);
		}
	}
	else
	{
		DisplayErrorMessage("Unable to open file:",
							(LPSTR)lpFile,
							FALSE);
		UnlockGlobalMsg()
		return(FALSE);
	}

	UnlockGlobalMsg()

	// ok
	return(TRUE);
}
#endif

// OpenDataFile()

BOOL OpenDataFile(LPCSTR lpFile)
{
	LoadingOutput("OpenDataFile");

	// number of bytes
	int nBytesRead;

	// open-file structure
	OFSTRUCT ofs;
	
	// data file header
	DATAFILEHEADER dfh;

	// check handle
	if(g_hDataFile!=INVALID_HANDLE_VALUE)
	{
		DisplayErrorMessage("Unable to open multiple data files.",
							NULL,
							FALSE);
		return(FALSE);
	}

	// prepare open-file structure
	ofs.cBytes=sizeof(OFSTRUCT);

	// open file
	g_hDataFile=(HANDLE)LZOpenFile((LPTSTR)lpFile,
								   &ofs,
								   OF_READ);								  

	// check for error
	if(g_hDataFile<0)
	{
		g_hDataFile=INVALID_HANDLE_VALUE;
		DisplayErrorMessage("Unable to open data file.",
							(LPSTR)lpFile,
							FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// read header
	nBytesRead=LZRead((int)g_hDataFile,
					  (LPSTR)&dfh,
					  sizeof(DATAFILEHEADER));					  

	// check for error
	if(nBytesRead!=sizeof(DATAFILEHEADER))
	{
		DisplayErrorMessage("Unable to read data file header.",
							(LPSTR)lpFile);
		LZClose((int)g_hDataFile);
		g_hDataFile=INVALID_HANDLE_VALUE;
		RemoveWinResource;
		return(FALSE);
	}

	// allocate data file memory
	if(!AllocMem((LPVOID*)&g_lpDataFile,
				 dfh.dwSize))
	{
		LZClose((int)g_hDataFile);
		g_hDataFile=INVALID_HANDLE_VALUE;
		RemoveWinResource;
		return(FALSE);
	}

	// read header
	nBytesRead=LZRead((int)g_hDataFile,
					  (LPSTR)g_lpDataFile,
					  dfh.dwSize);

	// check for error
	if(nBytesRead!=(int)dfh.dwSize)
	{
		DisplayErrorMessage("Unable to read data file header.",
							(LPSTR)lpFile);
		LZClose((int)g_hDataFile);
		g_hDataFile=INVALID_HANDLE_VALUE;
		RemoveWinResource;
		return(FALSE);
	}

	// close data file
	LZClose((int)g_hDataFile);

	// reset handle
	g_hDataFile=INVALID_HANDLE_VALUE;

	// remove resource
	RemoveWinResource;

	// get position values
	g_dwNumDataFileRef=dfh.dwNumRef;
	g_dwDataFileOffsetPos=dfh.dwOffsetPos;
	g_dwDataFileSizePos=dfh.dwSizePos;
	g_dwDataFileDataPos=dfh.dwDataPos;

#ifdef DEBUG_MODE
	// counter
	DWORD dwCount;

	// allocate memory for used flags
	if(!AllocMem((LPVOID*)&g_lpbDataFileRefUsed,
				 sizeof(BOOL)*g_dwNumDataFileRef))
		return(FALSE);

	// reset used flags
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
		g_lpbDataFileRefUsed[dwCount]=FALSE;
#endif

	// ok
	return(TRUE);
}

// FindDataFileRef()

DATAFILEREF FindDataFileRef(LPSTR lpFile)
{
	LoadingOutput("FindDataFileRef");

	// return value
	DATAFILEREF dfr;
	InitStruct(dfr);
	
	// counter
	DWORD dwCount;

	// check data file
	if(!g_lpDataFile)
		return(dfr);

	// reset position holder
	DWORD dwPos=0;
	
	// reset found index
	DWORD dwIndex=NO_OBJECT;
		
	// search file names
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
	{
		// compare names
		if(stricmp((LPSTR)&g_lpDataFile[dwPos],
				   lpFile)==0)
		{
			// check for duplicates
			if(dwIndex!=NO_OBJECT)
				DisplayErrorMessage("Duplicate data file references found.",
								   lpFile,
								   FALSE);

			// save index
			dwIndex=dwCount;
			
#ifdef DEBUG_MODE
			// set used flag
			g_lpbDataFileRefUsed[dwCount]=TRUE;
#endif
		}

		// increment pointer
		dwPos+=strlen((LPSTR)&g_lpDataFile[dwPos])+1;
	}

	// check index
	if(dwIndex==NO_OBJECT)
	{
		DisplayErrorMessage("Data file reference not found.",
						   lpFile,
						   FALSE);
		return(dfr);
	}

	// get pointer to offset table
	LPDWORD lpdwOffset=(LPDWORD)&g_lpDataFile[g_dwDataFileOffsetPos];

	// set data pointer
	dfr.lpData=(LPVOID)&g_lpDataFile[lpdwOffset[dwIndex]];

	// get pointer to size table
	LPDWORD lpdwSize=(LPDWORD)&g_lpDataFile[g_dwDataFileSizePos];

	// set size
	dfr.dwSize=lpdwSize[dwIndex];

	// return structure
	return(dfr);
}

// CloseDataFile()

void CloseDataFile(void)
{
#ifdef DEBUG_MODE
#ifndef IGNORE_UNUSED_FILES
	// counter
	DWORD dwCount;

	// check used flags
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
		if(!g_lpbDataFileRefUsed[dwCount])
		{
			UnlockGlobalMsg()

			// create message
			sprintf(g_lpMsg,
					"Data file reference #%d not used.",
					dwCount);

			// display message
			DisplayInfoMessage(g_lpMsg);

			LockGlobalMsg()
		}
#endif

ANTIHACK_11

	// free used flag memory
	FreeMem((LPVOID*)&g_lpbDataFileRefUsed);
#endif

	// free data file memory
	FreeMem((LPVOID*)&g_lpDataFile);

	// reset number of references
	g_dwNumDataFileRef=0;
}

// GetFileLength()

DWORD GetFileLength(LPSTR lpFile)
{
	// file attributes
	WIN32_FILE_ATTRIBUTE_DATA fad;
	
	// get file attributes
	if(!GetFileAttributesEx(lpFile,
							GetFileExInfoStandard,
							(LPVOID)&fad))
	{
		DisplayErrorMessage("Unable to read file attributes.",
							lpFile);
		return(0);
	}

	// check high size
	if(fad.nFileSizeHigh)
	{
		DisplayErrorMessage("File is too large to read length.",
							lpFile);
		return(0);
	}

	// return low size
	return(fad.nFileSizeLow);
}

// GetCurrentDialogDropDownSelection()

BOOL GetCurrentDialogDropDownSelection(DWORD dwID,
									   LPSTR lpText)
{
	// get handle of list box
	HWND hList=GetDlgItem(g_hCurrDlg,
						  dwID);

	// check handle
	if(!hList)
	{
		DisplayErrorMessage("Unable to read list box selection.");
		return(FALSE);
	}
	
	// get index
	DWORD dwIndex=(DWORD)SendMessage(hList,
									 CB_GETCURSEL,
									 NULL,
									 NULL);

	// read string
	if(dwIndex!=CB_ERR)
		SendMessage(hList,
					CB_GETLBTEXT,
					(WPARAM)dwIndex,
					(LPARAM)lpText);
	else
		return(FALSE);

	// ok
	return(TRUE);
}

// ResetCurrentDialogDropDownSelections()

BOOL ResetCurrentDialogDropDownSelections(DWORD dwID)
{
	// get list box handle
	HWND hList=GetDlgItem(g_hCurrDlg,
						  dwID);

	// check handle
	if(!hList)
	{
		DisplayErrorMessage("Unable to read list box selection.");
		return(FALSE);
	}
	
	// reset list box
	SendMessage(hList,
				CB_RESETCONTENT,
				NULL,
				NULL);

	// ok
	return(TRUE);
}

// AddCurrentDialogDropDownSelection()

BOOL AddCurrentDialogDropDownSelection(DWORD dwID,
									   LPSTR lpText)
{
	// get list box handle
	HWND hList=GetDlgItem(g_hCurrDlg,
						  dwID);

	// check handle
	if(!hList)
	{
		DisplayErrorMessage("Unable to read list box selection.");
		return(FALSE);
	}
	
	// add string
	if(SendMessage(hList,
				   CB_ADDSTRING,
				   NULL,
				   (LPARAM)lpText)==CB_ERR)
		return(FALSE);

	// ok
	return(TRUE);
}

// RemoveCurrentDialogDropDownSelection()

BOOL RemoveCurrentDialogDropDownSelection(DWORD dwID)
{
	// get handle of list box
	HWND hList=GetDlgItem(g_hCurrDlg,
						  dwID);
	
	// check handle
	if(!hList)
	{
		DisplayErrorMessage("Unable to read list box selection.");
		return(FALSE);
	}
	
	// get index
	DWORD dwIndex=(DWORD)SendMessage(hList,
									 CB_GETCURSEL,
									 NULL,
									 NULL);

	// remove item
	if(dwIndex!=CB_ERR)
	{
		// send message
		if(SendMessage(hList,
					   CB_DELETESTRING,
					   (WPARAM)dwIndex,
					   NULL)==CB_ERR)
			return(FALSE);
	}
	else
		return(FALSE);

	// ok
	return(TRUE);
}

// HighlightCurrentDialogDropDownSelection()

BOOL HighlightCurrentDialogDropDownSelection(DWORD dwID,
											 LPSTR lpText)
{
	// get handle of list box
	HWND hList=GetDlgItem(g_hCurrDlg,
						  dwID);

	// check handle
	if(!hList)
	{
		DisplayErrorMessage("Unable to read list box selection.");
		return(FALSE);
	}
	
	// find index of selection
	DWORD dwIndex=(DWORD)SendMessage(hList,
									 CB_FINDSTRING,
									 (WPARAM)-1,
									 (LPARAM)lpText);

	// check index
	if(dwIndex==CB_ERR)
		return(FALSE);

	// highlight selection
	if(SendMessage(hList,
				   CB_SETCURSEL,
				   (WPARAM)dwIndex,
				   NULL)==-1)
		return(FALSE);

	// ok
	return(TRUE);
}

// SetCurrentDialogEditLong()

BOOL SetCurrentDialogEditLong(WORD wID,
							  long lVal)
{
	// just to verify
	LockGlobalMsg()
	UnlockGlobalMsg()

	// convert value to string
	sprintf(g_lpMsg,
			"%d",
			lVal);

	// set text
	return(SetCurrentDialogEditText(wID,
								    g_lpMsg));
}

// GetCurrentDialogEditLong()

long GetCurrentDialogEditLong(WORD wID)
{
	// just to verify
	LockGlobalMsg()
	UnlockGlobalMsg()

	// get text
	if(GetCurrentDialogEditText(wID,
								g_lpMsg,
								_MAX_PATH))
		return(atol(g_lpMsg));
	else
		return(0);
}

// PurgeSystemObjects()

void PurgeSystemObjects(void)
{
	// purge objects
	PurgeTimers();
	PurgeDialogs();
	PurgeFonts();
}

// ForceImmediateExit()

void ForceImmediateExit(void)
{
	// release all allocated resources
	ReleaseAllModules();

#ifndef ALLOW_MULTIPLE_INSTANCES
	// release duplication lock
	UnlockApplication();
#endif

	// confirm removal of all resources
	ConfirmResourcesReleased();

	// return to os
	ExitProcess(0);
}

// InitializeCOM()

BOOL InitializeCOM(void)
{
	LoadingOutput("InitializeCOM");

ANTIHACK_12
		
	// init com
	HRESULT hr=CoInitialize(NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayErrorMessage("Unable to initialize component object model library.",
							NULL,
							FALSE);
		return(FALSE);
	}
	else
		AddCOMResource;

	// set flag
	g_bCOMReady=TRUE;

	// ok
	return(TRUE);
}

// ReleaseCOM()

void ReleaseCOM(void)
{
	// check com flag
	if(g_bCOMReady)
	{
		// uninit com
		CoUninitialize();

		// remove resource
		RemoveCOMResource;

		// set flag
		g_bCOMReady=FALSE;
	}
}

// CapitalizeString()

void CapitalizeString(LPSTR lpStr)
{
	// reset current position
	DWORD dwPos=0;

	// scan string until finished
	while(lpStr[dwPos]!=0)
	{
		// get current character
		char cChar=lpStr[dwPos];

		// check character
		if(cChar>='a'&&
		   cChar<='z')
		{
			// check position
			if(dwPos==0)
			{
				// capitalize character
				lpStr[dwPos]=(cChar-'a')+'A';
			}
			else
			{
				// get previous character
				char cPrev=lpStr[dwPos-1];

				// check previous character
				if(!((cPrev>='A'&&
					  cPrev<='Z')||
					 (cPrev>='a'&&
					  cPrev<='z')))
				{
					// capitalize character
					lpStr[dwPos]=(cChar-'a')+'A';
				}
			}
		}
		
		// increment position
		dwPos++;
	}
}

#ifdef DEBUG_MODE
// BinaryToDWORD()

DWORD BinaryToDWORD(LPSTR lpStr)
{
	// counter
	DWORD dwCount;
	
	// reset return value
	DWORD dwRet=0;

	// get length of string
	DWORD dwLen=(DWORD)(strlen(lpStr));

	// reset multiplier
	DWORD dwMul=1<<(dwLen-1);

	// compute value
	for(dwCount=0;dwCount<dwLen;dwCount++)
	{
		// check byte
		if(lpStr[dwCount]=='1')
		{
			// add multiplier to value
			dwRet|=dwMul;

			// adjust multiplier
			dwMul=dwMul>>1;
		}
		else if(lpStr[dwCount]=='0')
		{
			// adjust multiplier
			dwMul=dwMul>>1;
		}
	}

	LockGlobalMsg()

	// display value
	sprintf(g_lpMsg,"%X",dwRet);
	DisplayInfoMessage(g_lpMsg);

	UnlockGlobalMsg()

	// return value
	return(dwRet);
}
#endif

// CNode::CNode()

CNode::CNode()
{
	// initialize data
	lpChild=NULL;
	lpSibling=NULL;
	lpNodeName=NULL;
	lpNodeData=NULL;
}

// CNode::AddChild()

CNode* CNode::AddChild(LPSTR lpName)
{
	// check pointer
	if(!lpChild)
	{
		// add new child
		AllocClassObject(lpChild,
						 CNode);

		// set child name
		lpChild->SetNodeName(lpName);

		// return pointer
		return(lpChild);
	}
	else
	{
		// add sibling to child
		return(lpChild->AddSibling(lpName));
	}
}

// CNode::AddSibling()

CNode* CNode::AddSibling(LPSTR lpName)
{
	// check pointer
	if(!lpSibling)
	{
		// add new sibling
		AllocClassObject(lpSibling,
						 CNode);

		// set sibling name
		lpSibling->SetNodeName(lpName);

		// return pointer
		return(lpSibling);
	}
	else
	{
		// add sibling to sibling
		return(lpSibling->AddSibling(lpName));
	}
}

/// CNode::GetChild()

CNode* CNode::GetChild(void)
{
	// return pointer
	return(lpChild);
}

// CNode::GetSibling()

CNode* CNode::GetSibling(void)
{
	// return pointer
	return(lpSibling);
}

// CNode::FindChild()

CNode* CNode::FindChild(LPSTR lpName)
{
	// check pointer
	if(lpChild)
		return(lpChild->FindNode(lpName));
	else
		return(NULL);
}

// CNode::FindSibling()

CNode* CNode::FindSibling(LPSTR lpName)
{
	// check pointer
	if(lpSibling)
		return(lpSibling->FindNode(lpName));
	else
		return(NULL);
}

// CNode::FindNode()

CNode* CNode::FindNode(LPSTR lpName)
{
	// pointer to node
	CNode* lpNode=NULL;
	
	// check this node
	if(lpNodeName)
		if(!strcmp(lpName,
				   lpNodeName))
			return(this);

	// check children
	lpNode=FindChild(lpName);
	if(lpNode)
		return(lpNode);

	// check siblings
	lpNode=FindSibling(lpName);
	if(lpNode)
		return(lpNode);

	// not found
	return(NULL);
}

// CNode::SetNodeData()

void CNode::SetNodeData(LPVOID lpData)
{
	// copy pointer
	lpNodeData=lpData;
}

// CNode::GetNodeData()

LPVOID CNode::GetNodeData(void)
{
	// return pointer
	return(lpNodeData);
}

// CNode::SetNodeName()

void CNode::SetNodeName(LPSTR lpName)
{
	// check for existing name
	if(lpNodeName)
		FreeMem((LPVOID*)&lpNodeName);

	// check name
	if(!lpName)
		return;

	// allocate memory for name
	if(!AllocMem((LPVOID*)&lpNodeName,
				 strlen(lpName)+1))
		return;

	// copy name
	strcpy(lpNodeName,
		   lpName);
}

// CNode::GetNodeName()

LPSTR CNode::GetNodeName(void)
{
	// return pointer
	return(lpNodeName);
}

// CNode::DeleteChild()

void CNode::DeleteChild(void)
{
	// check pointer
	if(lpChild)
	{
		// delete child node
		lpChild->DeleteNode();

		// free child object
		FreeClassObject(lpChild);
	}
}

// CNode::DeleteSibling()

void CNode::DeleteSibling(void)
{
	// check pointer
	if(lpSibling)
	{
		// delete sibling node
		lpSibling->DeleteNode();

		// free sibling object
		FreeClassObject(lpSibling);
	}
}

// CNode::DeleteNode()

void CNode::DeleteNode(void)
{
	// delete members
	DeleteChild();
	DeleteSibling();

	// free object name
	if(lpNodeName)
		FreeMem((LPVOID*)&lpNodeName);
}

// CNode::AllocNodeMem()

BOOL CNode::AllocNodeMem(DWORD dwSize)
{
	// allocate & return flag
	return(AllocMem(&lpNodeData,
					dwSize));
}

// CNode::FreeNodeMemChild()

void CNode::FreeNodeMemChild(void)
{
	// check pointer
	if(lpChild)
	{
		// free child node
		lpChild->FreeNodeMem();
	}
}

// CNode::FreeNodeMemSibling()

void CNode::FreeNodeMemSibling(void)
{
	// check pointer
	if(lpSibling)
	{
		// free sibling node
		lpSibling->FreeNodeMem();
	}
}

// CNode::FreeNodeMem()

void CNode::FreeNodeMem(void)
{
	// free members
	FreeNodeMemChild();
	FreeNodeMemSibling();

	// free this object
	if(lpNodeData)
		FreeMem(&lpNodeData);
}

#ifdef SCREENSAVER_MODE
// ProcessScreensaverFlags()

BOOL ProcessScreensaverFlags(LPSTR lpCmdLine)
{
	// mode flag
	DWORD dwMode;

	// parent window handle
	HWND hParentWnd;

	// process command line
	if(ProcessScreensaverCmdLine(lpCmdLine,
								 &dwMode,
								 &hParentWnd))
	{
		// check flag
		switch(dwMode)
		{
		// normal execution
		case(SSMODE_NORMAL):

			// execute screensaver
			return(TRUE);

		// configure
		case(SSMODE_CONFIG):
			
			// execute dialog box
			ApplicationConfigureScreensaver(hParentWnd);

			// do not execute screensaver
			return(FALSE);

		// preview window
		case(SSMODE_PREVIEW):
			
			// execute preview image window
			ExecutePreviewImageWindow(hParentWnd);

			// do not execute screensaver
			return(FALSE);

		// change password
		case(SSMODE_PASSWD):
			
			// execute password dialog box
			ChangeScreensaverPassword(hParentWnd);

			// do not execute screensaver
			return(FALSE);

		// WaveBreak special command
		case(SSMODE_WBS_CMD):
			
			// check command
			switch((int)hParentWnd)
			{
#ifdef SHAREWARE_MODE
			// purchase
			case(WBSCMD_PURCHASE):

				// execute purchase dialog
				ApplicationExecutePurchaseDlg();

				// ok
				break;
#endif
			
			// set as current screensaver
			case(WBSCMD_SETCURRENT):

				// set current screensaver
				SetCurrentScreensaver("Swan Lake.scr");

				LockGlobalMsg()

				// copy application name
				strcpy(g_lpMsg,ApplicationName());

#ifdef SHAREWARE_MODE
				// append "trial version" to name
				strcat(g_lpMsg," (Trial Version)");
#endif

				// query for options
				if(MessageBox(GetDesktopWindow(),
							  "Your current screensaver has been changed.\n\nWould you like to open Windows Display Properties?",
							  g_lpMsg,
							  MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL)==IDYES)
				{
					UnlockGlobalMsg()

					// open screensaver settings
					OpenScreensaverControlPanel();
				}

				UnlockGlobalMsg()

				// ok
				break;
			
			// load options
			case(WBSCMD_SSOPTIONS):

				// open screensaver settings
				OpenScreensaverControlPanel();

				// ok
				break;

			// invalid command
			default:

				// display message
				DisplayErrorMessage("Invalid screensaver startup command:",
									lpCmdLine,
									FALSE);

				// ok
				break;
			}

			// do not execute screensaver
			return(FALSE);
		}
	}
	
	// do not execute screensaver
	return(FALSE);
}

// ProcessScreensaverCmdLine()

BOOL ProcessScreensaverCmdLine(LPSTR lpCmdLine,
							   LPDWORD lpdwMode,
							   HWND* lphWnd)
{
	// counter
	DWORD dwCount;

	// reset return values
	(*lpdwMode)=SSMODE_INVALID;
	(*lphWnd)=(HWND)INVALID_HANDLE_VALUE;

ANTIHACK_3
	
	// check for configure mode
	if(strlen(lpCmdLine)==0)
		(*lpdwMode)=SSMODE_CONFIG;
	else
	{
		// check for WaveBreak special commands
		if(lpCmdLine[0]=='w'&&
		   lpCmdLine[1]=='~')
			(*lpdwMode)=SSMODE_WBS_CMD;

		// check for full-screen mode
		if(lpCmdLine[0]=='s'||
		   lpCmdLine[0]=='S'||
		   lpCmdLine[1]=='s'||
		   lpCmdLine[1]=='S')
			(*lpdwMode)=SSMODE_NORMAL;

		// check for configure mode
		if(lpCmdLine[0]=='c'||
		   lpCmdLine[0]=='C'||
		   lpCmdLine[1]=='c'||
		   lpCmdLine[1]=='C')
			(*lpdwMode)=SSMODE_CONFIG;
		
		// check for preview mode
		if(lpCmdLine[0]=='p'||
		   lpCmdLine[0]=='P'||
		   lpCmdLine[1]=='p'||
		   lpCmdLine[1]=='P')
			(*lpdwMode)=SSMODE_PREVIEW;
		
		// check for password mode
		if(lpCmdLine[0]=='a'||
		   lpCmdLine[0]=='A'||
		   lpCmdLine[1]=='a'||
		   lpCmdLine[1]=='A')
			(*lpdwMode)=SSMODE_PASSWD;
	}
	
	// check mode flag
	if((*lpdwMode)==SSMODE_INVALID)
	{
		// display message
		DisplayErrorMessage("Invalid screensaver startup option:",
							lpCmdLine,
							FALSE);

		// valid flag not found
		return(FALSE);
	}

	// convert non-numerical characters to '0'
	if(strlen(lpCmdLine))
		for(dwCount=0;dwCount<strlen(lpCmdLine);dwCount++)
			if(lpCmdLine[dwCount]<'0'||
			   lpCmdLine[dwCount]>'9')
				lpCmdLine[dwCount]='0';

	// extract window handle
	(*lphWnd)=(HWND)atoi(lpCmdLine);

	// set invalid value if necessary
	if(!(*lphWnd))
		(*lphWnd)=(HWND)INVALID_HANDLE_VALUE;
	
	// ok
	return(TRUE);
}

// ChangeScreensaverPassword()

void ChangeScreensaverPassword(HWND hParentWnd)
{
ANTIHACK_4
	
	// version info
	OSVERSIONINFO osvi;

	// set version info size
	osvi.dwOSVersionInfoSize=sizeof(osvi);

	// get version info
	if(!GetVersionEx(&osvi))
	{
		DisplayErrorMessage("Unable to read Windows version information.");
		return;
	}

	// do not continue if winnt
	if(osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
		return;

	// validate window handle
	if(hParentWnd==INVALID_HANDLE_VALUE)
		hParentWnd=GetDesktopWindow();

	// load mpr.dll
	HINSTANCE hMprDll=LoadLibrary("mpr.dll");

	// check handle
	if(!hMprDll)
		return;
	else
		AddWinResource;

	// cast prototype
	typedef void (WINAPI *PWDCHANGEPASSWORDA) (LPSTR,HWND,UINT,UINT);

	// cast pointer to funtion
	PWDCHANGEPASSWORDA PwdChangePasswordA=(PWDCHANGEPASSWORDA)GetProcAddress(hMprDll,
																			 "PwdChangePasswordA");

	// execute dialog box
	if(PwdChangePasswordA)
		PwdChangePasswordA("SCRSAVE",
						   hParentWnd,
						   NULL,
						   NULL);

	// free mpr.dll
	if(FreeLibrary(hMprDll))
		RemoveWinResource;
}

// ConfirmScreensaverPassword()

BOOL ConfirmScreensaverPassword(void)
{
	// version info
	OSVERSIONINFO osvi;

	// set version info size
	osvi.dwOSVersionInfoSize=sizeof(osvi);

	// get version info
	if(!GetVersionEx(&osvi))
	{
		DisplayErrorMessage("Unable to read Windows version information.");
		return(TRUE);
	}

	// do not continue if winnt
	if(osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
		return(TRUE);

	// registry key handle
	HKEY hKey;

	// key value data
	DWORD dwValType;
	DWORD dwValSize=sizeof(DWORD);

	// password flag
	DWORD dwUsePwd=NULL;

	// return value
	BOOL bResult=TRUE;

	// open password flag key
	if(RegOpenKeyEx(HKEY_CURRENT_USER,
					"Control Panel\\Desktop",
					0,
					KEY_READ,
					&hKey)==ERROR_SUCCESS)
	{
		// read value
		RegQueryValueEx(hKey,
						"ScreenSaveUsePassword",
						NULL,
						&dwValType,
						(LPBYTE)&dwUsePwd,
						&dwValSize);

		// close key
		RegCloseKey(hKey);
	}

	// check flag
	if(!dwUsePwd)
		return(TRUE);
	
	// load password.cpl
	HINSTANCE hPwdCpl=LoadLibrary("password.cpl");

	// check handle
	if(!hPwdCpl)
		return(TRUE);
	else
		AddWinResource;

	// cast prototype
	typedef BOOL (WINAPI *VERIFYSCREENSAVEPWD) (HWND);

	// cast pointer to funtion
	VERIFYSCREENSAVEPWD VerifyScreenSavePwd=(VERIFYSCREENSAVEPWD)GetProcAddress(hPwdCpl,
																			    "VerifyScreenSavePwd");

	// verify password
	if(VerifyScreenSavePwd)
	{
		// release directx application
		ReleaseDirectXApplication();

		// clear initialized flag
		g_bAppInit=FALSE;

		// reset window to fill display
		SetWindowPos(g_hWnd,
					 NULL,
					 0,0,
					 g_dmOrigSys.dwX,
					 g_dmOrigSys.dwY,
					 SWP_NOZORDER);

		// show window
		ShowWindow(g_hWnd,
				   SW_SHOW);

		// execute dialog box
		bResult=VerifyScreenSavePwd(g_hWnd);

		// check flag
		if(bResult==FALSE)
		{
			// set mouse skip value
			g_dwMouseSkip=10;

			// enable loading screen
			EnableLoadingScreen();

			// restore directx application
			if(!InitializeDirectXApplication())
			{
				// free password.cpl
				if(FreeLibrary(hPwdCpl))
					RemoveWinResource;

				// exit application
				ForceImmediateExit();
			}

			// disable loading screen
			DisableLoadingScreen();

			// set initialized flag
			g_bAppInit=TRUE;
		}
	}

	// free password.cpl
	if(FreeLibrary(hPwdCpl))
		RemoveWinResource;

	// return result
	return(bResult);
}

// ExecutePreviewImageWindow()

void ExecutePreviewImageWindow(HWND hParentWnd)
{
	// validate window handle
	if(hParentWnd==INVALID_HANDLE_VALUE)
		hParentWnd=GetDesktopWindow();

	// initialize preview window
	if(CreatePreviewImageWindow(hParentWnd))
	{
		// execute preview window
		ProcessScreensaverMessages();
	}

	// release preview window
	DestroyPreviewImageWindow();
}

// CreatePreviewImageWindow()

BOOL CreatePreviewImageWindow(HWND hParentWnd)
{
	// parent's client area
	RECT rClient;
	
	// add window class
	if(!AddPreviewImageWindowClass())
		return(FALSE);

	// get parent's client area
	if(!GetClientRect(hParentWnd,
					  &rClient))
		return(FALSE);
	
	// create application window
	g_hWnd=CreateWindowEx(NULL,
						  ApplicationScreensaverID(),
						  NULL,
						  WS_CHILD|WS_VISIBLE,
						  0,0,
						  rClient.right,
						  rClient.bottom,
						  hParentWnd,
						  NULL,
						  g_hInst,
						  NULL);

	// check window handle
	if(g_hWnd==(HWND)INVALID_HANDLE_VALUE||
	   g_hWnd==NULL)
		return(FALSE);
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// DestroyPreviewImageWindow()

void DestroyPreviewImageWindow(void)
{
	// check handle & destroy window
	if(g_hWnd!=(HWND)INVALID_HANDLE_VALUE&&
	   g_hWnd!=NULL)
		if(DestroyWindow(g_hWnd))
		{
			// reset handle
			g_hWnd=(HWND)INVALID_HANDLE_VALUE;
			
			// remove resource
			RemoveWinResource;
		}

	// remove window class
	RemovePreviewImageWindowClass();
}

// AddPreviewImageWindowClass()

BOOL AddPreviewImageWindowClass(void)
{
ANTIHACK_5

	// create class for this window
	WNDCLASSEX wcx;
	wcx.cbSize=sizeof(WNDCLASSEX);
	wcx.style=CS_PARENTDC;
	wcx.lpfnWndProc=PreviewImageWindowProc;
	wcx.cbClsExtra=NULL;
	wcx.cbWndExtra=NULL;
	wcx.hInstance=g_hInst;
	wcx.hIcon=NULL;
	wcx.hIconSm=NULL;
	wcx.hCursor=LoadCursor(NULL,IDC_ARROW);
	wcx.hbrBackground=(HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wcx.lpszMenuName=NULL;
	wcx.lpszClassName=ApplicationScreensaverID();

	// register window class
	if(!RegisterClassEx(&wcx))
		return(FALSE);
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// RemovePreviewImageWindowClass()

void RemovePreviewImageWindowClass(void)
{
	// check handle & un-register class
	if(g_hInst!=INVALID_HANDLE_VALUE)
		if(UnregisterClass(ApplicationScreensaverID(),g_hInst))
			RemoveWinResource;
}

// PreviewImageWindowProc()

LRESULT CALLBACK PreviewImageWindowProc(HWND hWnd,
										UINT Msg,
										WPARAM wParam,
										LPARAM lParam)
{
	// bitmap handle
	static HBITMAP hBmp=(HBITMAP)INVALID_HANDLE_VALUE;
	
	// device context
	HDC hDC;

	// pointer to create structure
	LPCREATESTRUCT lpCS;

	// paint structure
	PAINTSTRUCT ps;

	// check for messages that we are interested in
	switch(Msg)
	{
	// create window message
	case(WM_CREATE):

		// get pointer to create struct
		lpCS=(LPCREATESTRUCT)lParam;

		// load preview image
		hBmp=(HBITMAP)LoadImage(g_hInst,
								ApplicationScreensaverPreviewImageRes(),
								IMAGE_BITMAP,
								lpCS->cx,
								lpCS->cy,
								LR_DEFAULTCOLOR);

		// verify handle
		if(hBmp)
			AddWinResource;

		// ok
		return(NULL);

	// close message
	case(WM_CLOSE):

		// release preview image
		if(hBmp!=(HBITMAP)INVALID_HANDLE_VALUE&&
		   hBmp!=NULL)
			if(DeleteObject(hBmp))
				RemoveWinResource;

		// quit application
		PostMessage(g_hWnd,
					WM_QUIT,
					NULL,
					NULL);
		
		// ok
		return(NULL);

	// paint message
	case(WM_PAINT):

		// begin paint
		hDC=BeginPaint(g_hWnd,
					   &ps);

		// add resource
		AddWinResource;

		// check value
		if(hDC)
		{
			// display bitmap if available
			if(hBmp!=(HBITMAP)INVALID_HANDLE_VALUE&&
			   hBmp!=NULL)
			{
				// create compatible device context
				HDC hSrcDC=CreateCompatibleDC(hDC);

				// verify handle
				if(hSrcDC)
				{
					// add windows resource
					AddWinResource;
					
					// select bitmap as source
					SelectObject(hSrcDC,
								 hBmp);

					// copy image
					BitBlt(hDC,
						   ps.rcPaint.left,
						   ps.rcPaint.top,
						   ps.rcPaint.right,
						   ps.rcPaint.bottom,
						   hSrcDC,
						   ps.rcPaint.left,
						   ps.rcPaint.top,
						   SRCCOPY);

					// de-select object
					SelectObject(hSrcDC,
								 NULL);

					// delete device context
					if(DeleteDC(hSrcDC))
						RemoveWinResource;
				}
				else
				{
					// select black brush
					SelectObject(hDC,
								 GetStockObject(BLACK_BRUSH));

					// paint window
					PatBlt(hDC,
						   ps.rcPaint.left,
						   ps.rcPaint.top,
						   ps.rcPaint.right,
						   ps.rcPaint.bottom,
						   PATCOPY);

					// de-select object
					SelectObject(hDC,
								 NULL);
				}
			}
		}

		// end paint
		EndPaint(g_hWnd,
				 &ps); 
		
		// remove resource
		RemoveWinResource;

		// ok
		return(NULL);
	}

	// let windows process all other messages
	return(DefWindowProc(hWnd,Msg,wParam,lParam));
}

// ProcessScreensaverMessages()

void ProcessScreensaverMessages(void)
{
	// message storage
	MSG Msg;

	// retrieve message
	while(TRUE)
	{
		// process message
		if(PeekMessage(&Msg,
					   NULL,
					   NULL,
					   NULL,
					   PM_REMOVE))
		{
			// check for quit message
			if(Msg.message==WM_QUIT)
				return;
			
			// process message
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else
		{
			// wait for message
			WaitMessage();
		}			
	}
}
#endif

// InitializeDirectXApplication()

BOOL InitializeDirectXApplication(void)
{
	LoadingOutput("InitializeDirectXApplication");

	// verify current installation
	if(!VerifyDirectXVersion())
		return(FALSE);
	
	// initialize all modules
	if(!DXGraphicsInit())
		return(FALSE);
	if(!DXAudioInit())
		return(FALSE);
#ifdef USE_DIRECTINPUT
	if(!DXInputInit())
		return(FALSE);
#endif
	if(!ApplicationInit())
		return(FALSE);

	// ok
	return(TRUE);
}

// ReleaseDirectXApplication()

void ReleaseDirectXApplication(void)
{
	// perform audio pre-shutdown
	AudioPreShutdown();

	// release all modules
	ApplicationRelease();
#ifdef USE_DIRECTINPUT
	DXInputRelease();
#endif
	DXAudioRelease();
	DXGraphicsRelease();
}

// ResetMousePosition()

void ResetMousePosition(void)
{
#ifdef DEBUG_MODE
	// only valid in full-screen mode!
	if(g_pp.Windowed)
	{
		static BOOL bErrorFlag=FALSE;
		if(!bErrorFlag)
		{
			DisplayErrorMessage("Cannot reset mouse position in windowed mode.",
								"ResetMousePosition()",
								FALSE);
			bErrorFlag=TRUE;
		}
	}
#endif

	// set cursor position
	SetCursorPos(g_dmApp.dwX>>1,
				 g_dmApp.dwY>>1);

	// store position values
	g_nMousePX=g_dmApp.dwX>>1;
	g_nMousePY=g_dmApp.dwY>>1;
}

// SetFont()

LOGFONT SetFont(LPSTR FaceName,
				LONG Height,
				LONG Width,
				LONG Angle,
				LONG Weight,
				BYTE Italic,
				BYTE Underline,
				BYTE Strikeout)
{
	// structure to return
	LOGFONT lf;

	// set up structure
	lf.lfHeight=Height;
	lf.lfWidth=Width;
	lf.lfEscapement=Angle;
	lf.lfOrientation=Angle;
	lf.lfWeight=Weight;
	lf.lfItalic=Italic;
	lf.lfUnderline=Underline;
	lf.lfStrikeOut=Strikeout;
	lf.lfCharSet=DEFAULT_CHARSET;
	lf.lfOutPrecision=OUT_TT_PRECIS;
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lf.lfQuality=PROOF_QUALITY;
	lf.lfPitchAndFamily=DEFAULT_PITCH|FF_DONTCARE;
	if(FaceName)
		strcpy(lf.lfFaceName,
			   FaceName);
	else
		lf.lfFaceName[0]=0;
	
	// return structure
	return(lf);
}

// AddFont()

DWORD AddFont(LPSTR FaceName,
			  LONG Height,
			  LONG Width,
			  LONG Angle,
			  LONG Weight,
			  BYTE Italic,
			  BYTE Underline,
			  BYTE Strikeout)
{
	LoadingOutput("AddFont");

	// set up structure
	LOGFONT lf=SetFont(FaceName,
					   Height,
					   Width,
					   Angle,
					   Weight,
					   Italic,
					   Underline,
					   Strikeout);

	// add font
	return(AddFont(&lf));
}

// GetNextFont()

DWORD GetNextFont(void)
{
	LoadingOutput("GetNextFont");

	// check counter
	if(g_dwNumFont==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lphFont,
					 sizeof(HFONT*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxFont=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumFont>=g_dwMaxFont)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lphFont,
					  sizeof(HFONT*)*g_dwMaxFont,
					  sizeof(HFONT*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxFont+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumFont;

	// increment counter
	g_dwNumFont++;

	// return index
	return(dwIndex);
}

// AddFont()

DWORD AddFont(LOGFONT* lplf)
{
	LoadingOutput("AddFont");

	// get return index
	DWORD dwIndex=GetNextFont();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// create font
	g_lphFont[dwIndex]=CreateFontIndirect(lplf);

	// check for error
	if(!g_lphFont[dwIndex])
	{
		DisplayErrorMessage("Unable to load font.",
						    lplf->lfFaceName);
		return(NO_OBJECT);
	}
	else
		AddWinResource;

	// return index
	return(dwIndex);
}

// BeginText()

BOOL BeginText(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumFont)
	{
		DisplayFatalErrorMessage("Invalid font index.",
								 "BeginText()",
								 FALSE);
		return(FALSE);
	}
#endif

	// error-shown flag
	static BOOL bErrorShown=FALSE;

	// check window handle
	if(g_hWnd==INVALID_HANDLE_VALUE||
	   g_hWnd==NULL)
		return(FALSE);

	// init font device context
	g_hFontDC=GetDC(g_hWnd);

	// check for error
	if(!g_hFontDC)
	{
		if(!bErrorShown)
		{
			DisplayErrorMessage("Unable to retrieve device context.",
								"Some information may not display properly.");
			bErrorShown=TRUE;
		}
		return(FALSE);
	}
	else
		AddWinResource;

	// select font into device context
	SelectObject(g_hFontDC,g_lphFont[dwIndex]);

	// ok
	return(TRUE);
}

// OutputText()

void OutputText(LPSTR lpText,
			    int nX,
			    int nY,
			    COLORREF crForeground,
			    COLORREF crBackground,
			    DWORD dwFormat)
{
	// set alignment
	SetTextAlign(g_hFontDC,dwFormat);

	// set foreground color
	SetTextColor(g_hFontDC,crForeground);

	// set background color
	SetBkColor(g_hFontDC,crBackground);

	// output text
	TextOut(g_hFontDC,nX,nY,lpText,strlen(lpText));
}

// EndText()

void EndText(void)
{
	// select font out of device context
	SelectObject(g_hFontDC,NULL);

	// delete device context
	DeleteDC(g_hFontDC);
	
	// remove resource
	RemoveWinResource;
}

// PurgeFonts()

void PurgeFonts(void)
{
	// check counter
	if(g_dwNumFont)
	{
		// counter
		DWORD dwCount;

		// release all fonts
		for(dwCount=0;dwCount<g_dwNumFont;dwCount++)
		{
			DeleteObject(g_lphFont[dwCount]);
			RemoveWinResource;
		}
	}

	// free memory
	FreeMem((LPVOID*)&g_lphFont);
	
	// reset counters
	g_dwNumFont=0;
	g_dwMaxFont=0;
}

// SetCurrentDialogScrollInfo()

BOOL SetCurrentDialogScrollInfo(WORD wID,
								int nMin,
								int nMax,
								UINT nPage,
								int nPos)
{
	// handle to scroll bar
	HWND hScroll;
	
	// scroll bar info
	SCROLLINFO si;

	// get handle to scroll bar
	hScroll=GetDlgItem(g_hCurrDlg,
					   (int)wID);

	// check for error
	if(!hScroll)
	{
		DisplayErrorMessage("Unable to get scroll bar window handle.");
		return(FALSE);
	}

	// prepare info structure
	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_PAGE|SIF_POS|SIF_RANGE;
	si.nMin=nMin;
	si.nMax=nMax;
	si.nPage=nPage;
	si.nPos=nPos;
	si.nTrackPos=NULL;

	// set scroll bar info
	SetScrollInfo(hScroll,
				  SB_CTL,
				  &si,
				  TRUE);

	// ok
	return(TRUE);
}

// GetCurrentDialogScrollPos()

int GetCurrentDialogScrollPos(WORD wID)
{
	// handle to scroll bar
	HWND hScroll;
	
	// scroll bar info
	SCROLLINFO si;

	// get handle to scroll bar
	hScroll=GetDlgItem(g_hCurrDlg,
					   (int)wID);

	// check for error
	if(!hScroll)
	{
		DisplayErrorMessage("Unable to get scroll bar window handle.");
		return(-1);
	}

	// prepare info structure
	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_POS;

	// get scroll bar info
	if(!GetScrollInfo(hScroll,
					  SB_CTL,
					  &si))
	{
		DisplayErrorMessage("Unable to get current scroll bar position.");
		return(-1);
	}

	// ok
	return(si.nPos);
}

// GetRegistryConfigValue()

int GetRegistryConfigValue(LPCSTR lpName,
						   int nMin,
						   int nMax,
						   int nDef)
{
	// get registry key
	LPSTR lpVal=GetRegistryKey(lpName);

	// check for key
	if(!lpVal)
	{
		// return default value
		return(nDef);
	}

	// convert value to int
	int nVal=atoi(lpVal);

	// free key value
	FreeRegistryKey(lpVal);

	// clip value if needed
	if(nMin>=0)
		if(nVal<nMin)
			nVal=nDef;
	if(nMax>=0)
		if(nVal>nMax)
			nVal=nDef;

	// return value
	return(nVal);
}

// SetRegistryConfigValue()

void SetRegistryConfigValue(LPCSTR lpName,
							int nVal)
{
	LockGlobalMsg()

	// convert value to string
	sprintf(g_lpMsg,
			"%d",
			nVal);

	// set key
	SetRegistryKey(lpName,
				   g_lpMsg);

	UnlockGlobalMsg()
}

// DisableCurrentDialogCheckBox()

BOOL DisableCurrentDialogCheckBox(WORD wID)
{
	// set button
	if(!CheckDlgButton(g_hCurrDlg,
					   (int)wID,
					   BST_INDETERMINATE))
	{
		DisplayErrorMessage("Unable to set dialog button state.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// ExecuteExternalApp()

BOOL ExecuteExternalApp(LPTSTR lpCmdLine,
						LPCTSTR lpWorkingDir)
{
	// startup info
	STARTUPINFO si;

	// process information
	PROCESS_INFORMATION pi;
	
	// prepare startup info
	si.cb=sizeof(STARTUPINFO);
	si.lpReserved=NULL;
	si.lpDesktop=NULL;
	si.lpTitle=NULL;
	si.dwFlags=NULL;
	si.cbReserved2=NULL;
	si.lpReserved2=NULL;

	// create new process
	if(!CreateProcess(NULL,
					  lpCmdLine,
					  NULL,
					  NULL,
					  FALSE,
					  NULL,
					  NULL,
					  lpWorkingDir,
					  &si,
					  &pi))
	{
		DisplayErrorMessage("Unable to start external application.",
							(LPSTR)lpCmdLine,
							TRUE);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// OpenExternalDoc()

BOOL OpenExternalDoc(LPCTSTR lpFileName,
					 LPCTSTR lpWorkingDir)
{
	// execute shell command
	if(ShellExecute(NULL,
					"open",
					lpFileName,
					NULL,
					lpWorkingDir,
					SW_SHOW)<=(HINSTANCE)32)
	{
		DisplayErrorMessage("Unable to open document.",
						    (LPSTR)lpFileName,
						    FALSE);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// PrintExternalDoc()

BOOL PrintExternalDoc(LPCTSTR lpFileName,
					  LPCTSTR lpWorkingDir)
{
	// execute shell command
	if(ShellExecute(NULL,
					"print",
					lpFileName,
					NULL,
					lpWorkingDir,
					SW_SHOW)<=(HINSTANCE)32)
	{
		DisplayErrorMessage("Unable to print document.",
						    (LPSTR)lpFileName,
						    FALSE);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// HighlihghtCurrentDialogEditText()

void HighlightCurrentDialogEditText(WORD wID,
									int nStart,
									int nEnd)
{
	// get handle of edit control
	HWND hEdit=GetDlgItem(g_hCurrDlg,
						  wID);

	// check handle
	if(!hEdit)
		return;

	// set selection
	SendMessage(hEdit,
				EM_SETSEL,
				(WPARAM)nStart,
				(LPARAM)nEnd);	
}

// UpdateScrollBarPos()

BOOL UpdateScrollBarPos(HWND hScroll,
						int nPosInc)
{
ANTIHACK_6

	// scroll info
	SCROLLINFO si;

	// prepare structure
	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_POS|SIF_RANGE;

	// get scroll info
	if(!GetScrollInfo(hScroll,
					  SB_CTL,
					  &si))
		return(FALSE);

	// increment position
	si.nPos+=nPosInc;

	// clip value
	if(si.nPos<si.nMin)
		si.nPos=si.nMin;
	if(si.nPos>si.nMax)
		si.nPos=si.nMax;

	// set new info
	SetScrollInfo(hScroll,
				  SB_CTL,
				  &si,
				  TRUE);
	
	// ok
	return(TRUE);
}

// SetScrollBarPos()

BOOL SetScrollBarPos(HWND hScroll,
					 int nPos)
{
	// scroll info
	SCROLLINFO si;

	// prepare structure
	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_POS;

	// set position
	si.nPos=nPos;

	// set scroll info
	SetScrollInfo(hScroll,
				  SB_CTL,
				  &si,
				  TRUE);
	
	// ok
	return(TRUE);
}

// VerifyDirectXVersion()

BOOL VerifyDirectXVersion(void)
{
	// The file 'dpnhpast.dll' was introducted in DirectX 8.1.
	// Presence of this file verifies that DirectX 8.1 or higher is present.
	
	// load dpnhpast.dll
	HINSTANCE hDpnhPastDll=LoadLibrary("dpnhpast.dll");

	// check handle
	if(!hDpnhPastDll)
	{
		// query user
		if(DisplayMessageBox(g_hWnd,
							 "This program requires DirectX version 8.1 or higher.  Your computer does not have the correct version installed.\n\nWould you like to connect to the Internet and download the latest version of DirectX?",
							 (LPSTR)ApplicationName(),
							 MB_YESNO|MB_ICONQUESTION)==IDYES)
		{
			// begin wait cursor
			BeginWaitCursor();

			// open download page
			OpenExternalDoc("http://www.wb-software.com/directx.htm");

			// end wait cursor
			EndWaitCursor();
		}

		// cannot continue
		return(FALSE);
	}
	else
		AddWinResource;

	// free dpnhpast.dll
	if(FreeLibrary(hDpnhPastDll))
		RemoveWinResource;

	// ok
	return(TRUE);
}

// SetCurrentScreensaver()

BOOL SetCurrentScreensaver(LPCSTR lpScr)
{
	LockGlobalMsg()

	// copy windows path
	if(!GetWindowsDirectory(g_lpMsg,
							_MAX_PATH))
	{
		DisplayErrorMessage("Unable to read Windows directory.",
						    NULL,
						    TRUE);
		UnlockGlobalMsg()
		return(FALSE);
	}

	// append '\'
	strcat(g_lpMsg,
		   "\\");

	// append name
	strcat(g_lpMsg,
		   lpScr);

	// convert to short form
	GetShortPathName(g_lpMsg,
					 g_lpMsg,
					 _MAX_PATH);
	
	// save setting
	if(!WritePrivateProfileString("boot",
								  "SCRNSAVE.EXE",
								  g_lpMsg,
								  "system.ini"))
	{
		DisplayErrorMessage("Unable to set current screensaver.",
						    NULL,
						    TRUE);
		UnlockGlobalMsg()
		return(FALSE);
	}

	UnlockGlobalMsg()
	
	// ok
	return(TRUE);
}

// OpenScreensaverControlPanel()

BOOL OpenScreensaverControlPanel(void)
{
	// execute control panel/display/screensaver
	if(WinExec("control.exe desk.cpl Display,@ScreenSaver",
			   SW_SHOW)<32)
	{
		DisplayErrorMessage("Unable to open Windows Control Panel.",
						    NULL,
						    FALSE);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// ExecuteCommandLine()

BOOL ExecuteCommandLine(LPCTSTR lpExeFile,
						LPCTSTR lpCmdLine,
						LPCTSTR lpWorkingDir)
{
	// execute shell command
	if(ShellExecute(NULL,
					"open",
					lpExeFile,
					lpCmdLine,
					lpWorkingDir,
					SW_SHOW)<=(HINSTANCE)32)
	{
		// command line storage
		LPSTR lpCmd;
		
		// allocate memory for command line
		if(AllocMem((LPVOID*)&lpCmd,
					strlen(lpExeFile)+1+strlen(lpCmdLine)+1))
		{
			// copy exe file
			strcpy(lpCmd,lpExeFile);

			// add ' '
			strcat(lpCmd," ");

			// add command line
			strcat(lpCmd,lpCmdLine);

			// display error message
			DisplayErrorMessage("Unable to execute command line.",
								lpCmd,
								FALSE);

			// free memory
			FreeMem((LPVOID*)&lpCmd);
		}

		// no good
		return(FALSE);
	}

	// ok
	return(TRUE);
}

#ifdef SHAREWARE_MODE
// GetHDDSerialNumber()

DWORD GetHDDSerialNumber(void)
{
	// set default return value
	DWORD dwRet=NO_OBJECT;

	// counter
	DWORD dwCount;
	
ANTIHACK_7

	LockGlobalMsg()

	// get windows directory
	if(GetWindowsDirectory((LPTSTR)g_lpMsg,
						   _MAX_PATH))
	{
		// truncate directory after first '\'
		for(dwCount=0;dwCount<strlen(g_lpMsg);dwCount++)
			if(g_lpMsg[dwCount]=='\\')
				g_lpMsg[dwCount+1]=0;

		// get volume information
		if(!GetVolumeInformation((LPCSTR)g_lpMsg,
								 NULL,
								 0,
								 &dwRet,
								 NULL,
								 NULL,
								 NULL,
								 0))
		{
ANTIHACK_8
			
			// reset default return value
			DWORD dwRet=NO_OBJECT;
		}
	}

	UnlockGlobalMsg()

	// return value
	return(dwRet);
}

// EncodeTimeStamp()

void EncodeTimeStamp(TIMESTAMP tsSrc,
					 LPSTR lpStr)
{
	// destination timestamp
	TIMESTAMP tsDest;

	// randomize upper 3 bytes of days left
	tsSrc.dwDaysLeft=(tsSrc.dwDaysLeft&0x000000FFL)|(GetRandomInt()&0xFFFFFF00L);

	// encode source elements
	EncodeDWORD(&tsSrc.dwID);
ANTIHACK_9
	EncodeDWORD(&tsSrc.dwInstTimeLow);
	EncodeDWORD(&tsSrc.dwInstTimeHigh);
ANTIHACK_10
	EncodeDWORD(&tsSrc.dwDaysLeft);

	// cast byte pointers
	LPBYTE lpcSrc=(LPBYTE)&tsSrc;
	LPBYTE lpcDest=(LPBYTE)&tsDest;

	// swap bytes
	lpcDest[0] =lpcSrc[4];
	lpcDest[1] =lpcSrc[0];
	lpcDest[2] =lpcSrc[5];
	lpcDest[3] =lpcSrc[7];
	lpcDest[4] =lpcSrc[11];
ANTIHACK_11
	lpcDest[5] =lpcSrc[6];
	lpcDest[6] =lpcSrc[9];
	lpcDest[7] =lpcSrc[2];
ANTIHACK_12
	lpcDest[8] =lpcSrc[8];
	lpcDest[9] =lpcSrc[1];
	lpcDest[10]=lpcSrc[14];
	lpcDest[11]=lpcSrc[13];
	lpcDest[12]=lpcSrc[3];
	lpcDest[13]=lpcSrc[12];
ANTIHACK_3
	lpcDest[14]=lpcSrc[15];
	lpcDest[15]=lpcSrc[10];

	// encode destination timestamp
	EncodeBytesAsString(lpcDest,
						16,
						lpStr);
}

// DecodeTimeStamp()

TIMESTAMP DecodeTimeStamp(LPSTR lpStr)
{
	// working timestamps
	TIMESTAMP tsSrc;
	TIMESTAMP tsDest;

	// check length of string
	if(strlen(lpStr)!=32)
	{
		// set default timestamp
		tsDest.dwDaysLeft=0;
		tsDest.dwID=NO_OBJECT;
ANTIHACK_4
		tsDest.dwInstTimeHigh=NULL;
		tsDest.dwInstTimeLow=NULL;

		// return timestamp
		return(tsDest);
	}

	// decode source timestamp
	DecodeBytesFromString(lpStr,
						  (LPBYTE)&tsSrc,
						  16);

	// cast byte pointers
	LPBYTE lpcSrc=(LPBYTE)&tsSrc;
	LPBYTE lpcDest=(LPBYTE)&tsDest;

	// swap bytes
	lpcDest[0] =lpcSrc[1];
	lpcDest[1] =lpcSrc[9];
	lpcDest[2] =lpcSrc[7];
	lpcDest[3] =lpcSrc[12];
	lpcDest[4] =lpcSrc[0];
ANTIHACK_5
	lpcDest[5] =lpcSrc[2];
	lpcDest[6] =lpcSrc[5];
	lpcDest[7] =lpcSrc[3];
	lpcDest[8] =lpcSrc[8];
	lpcDest[9] =lpcSrc[6];
_asm // anti-hack
{
	mov eax,1
	cpuid
	mov eax,ebx
}
	lpcDest[10]=lpcSrc[15];
	lpcDest[11]=lpcSrc[4];
	lpcDest[12]=lpcSrc[13];
	lpcDest[13]=lpcSrc[11];
ANTIHACK_6
	lpcDest[14]=lpcSrc[10];
	lpcDest[15]=lpcSrc[14];

	// decode timestamp elements
	DecodeDWORD(&tsDest.dwID);
	DecodeDWORD(&tsDest.dwInstTimeLow);
	DecodeDWORD(&tsDest.dwInstTimeHigh);
	DecodeDWORD(&tsDest.dwDaysLeft);

	// clear upper 3 bytes of days left
	tsDest.dwDaysLeft=(tsDest.dwDaysLeft&0x000000FFL);

	// return destination timestamp
	return(tsDest);
}

// EncodeDWORD()

void EncodeDWORD(LPDWORD lpDW)
{
	// get high/low words
	WORD wHigh=HIWORD(*lpDW);
	WORD wLow =LOWORD(*lpDW);

	// encode bits
	_asm
	{
		//	-->	dummy instructions
	
		// copy high/low words
		mov ax,wHigh
		mov bx,wLow
				mov cx,ax
				mov dx,wLow

		// encode bits
		inc ah
				add cx,dx
		dec al
		dec bh
		inc bl
				inc dx
		neg ax
		not bx
				rol dx,7
				ror cl,4
		rol ah,3
		ror al,2
		ror bh,5
				and cx,dx
		rol bl,4
		xchg ah,bl
		xchg ax,bx
				mov dh,cl
		add ax,0x43A6
		sub bx,0xF430

		// store new words
				mov wLow,dx
		mov wHigh,ax
		mov wLow,bx
	}

	// set new dword value
	(*lpDW)=MAKELONG(wLow,wHigh);
}

// DecodeDWORD()

void DecodeDWORD(LPDWORD lpDW)
{
	// get high/low words
	WORD wHigh=HIWORD(*lpDW);
	WORD wLow =LOWORD(*lpDW);

	// encode bits
	_asm
	{
		//	-->	dummy instructions

		// copy high/low words
		mov ax,wHigh
		mov bx,wLow
				mov cx,ax

		// decode bits
		add bx,0xF430
				mov dx,ax
		sub ax,0x43A6
		xchg ax,bx
		xchg ah,bl
		ror bl,4
				sub dx,bx
		rol bh,5
		rol al,2
				mov cl,dh
				mov ch,dl
		ror ah,3
		not bx
		neg ax
				inc cx
		dec bl
		inc bh
				dec dx
		inc al
		dec ah

		// store new words
				add cx,ax
		mov wHigh,ax
		mov wLow,bx
	}
	
	// set new dword value
	(*lpDW)=MAKELONG(wLow,wHigh);
}

// EncodeBytesAsString()

void EncodeBytesAsString(LPBYTE lpcBytes,
						 DWORD dwNumBytes,
						 LPSTR lpStr)
{
	// counter
	DWORD dwCount;

	// loop through bytes
	for(dwCount=0;dwCount<dwNumBytes;dwCount++)
	{
		// extract nybbles
		BYTE cNyb1=(lpcBytes[dwCount]   )&0x0F;
		BYTE cNyb2=(lpcBytes[dwCount]>>4)&0x0F;

ANTIHACK_12

		// convert nybbles to bytes
		BYTE cByte1=EncodeNybble(cNyb1);
		BYTE cByte2=EncodeNybble(cNyb2);

		// copy bytes
		lpStr[(dwCount<<1)  ]=cByte1;
		lpStr[(dwCount<<1)+1]=cByte2;
	}

	// place null terminator
	lpStr[dwNumBytes<<1]=0;
}

// DecodeBytesFromString()

void DecodeBytesFromString(LPSTR lpStr,
						   LPBYTE lpcBytes,
						   DWORD dwNumBytes)
{
	// counter
	DWORD dwCount;

	// loop through bytes
	for(dwCount=0;dwCount<dwNumBytes;dwCount++)
	{
		// copy bytes
		BYTE cByte1=lpStr[(dwCount<<1)  ];
		BYTE cByte2=lpStr[(dwCount<<1)+1];

		// extract nybbles
		BYTE cNyb1=DecodeNybble(cByte1);
		BYTE cNyb2=DecodeNybble(cByte2);

ANTIHACK_3
		
		// copy byte
		lpcBytes[dwCount]=cNyb1|(cNyb2<<4);
	}
}

// EncodeNybble()

BYTE EncodeNybble(BYTE cNybble)
{
	// init codes
	InitNybbleCodes();

	// return encoded byte
	return(g_lpNybCode[((cNybble&0x0F)<<2)+(abs(GetRandomInt())%4)]);
}

// DecodeNybble()

BYTE DecodeNybble(BYTE cByte)
{
	// counter
	DWORD dwCount;

	// init codes
	InitNybbleCodes();

	// search for code
	for(dwCount=0;dwCount<64;dwCount++)
		if(g_lpNybCode[dwCount]==cByte)
			return((BYTE)((dwCount>>2)&0x0F));

ANTIHACK_4

	// problem
	return(NULL);
}

// InitNybbleCodes()

void InitNybbleCodes(void)
{
	// initialized flag
	static BOOL bInit=FALSE;

	// check initialized flag
	if(bInit)
		return;

	// set values
	g_lpNybCode[21]='y';
	g_lpNybCode[39]='S';
	g_lpNybCode[22]='X';
ANTIHACK_5
	g_lpNybCode[23]='7';
	g_lpNybCode[36]='M';
	g_lpNybCode[40]='O';
	g_lpNybCode[13]='C';
	g_lpNybCode[60]='f';
	g_lpNybCode[10]='2';
	g_lpNybCode[46]='s';
	g_lpNybCode[2] ='w';
ANTIHACK_6
	g_lpNybCode[61]='5';
	g_lpNybCode[49]='0';
	g_lpNybCode[41]='v';
	g_lpNybCode[5] ='P';
	g_lpNybCode[59]='u';
	g_lpNybCode[54]='4';
	g_lpNybCode[27]='R';
	g_lpNybCode[12]='E';
	g_lpNybCode[63]='Y';
	g_lpNybCode[4] ='o';
	g_lpNybCode[0] ='e';
	g_lpNybCode[1] ='n';
	g_lpNybCode[26]='D';
	g_lpNybCode[9] ='h';
ANTIHACK_7
	g_lpNybCode[14]='c';
	g_lpNybCode[50]='G';
	g_lpNybCode[28]='A';
	g_lpNybCode[24]='9';
	g_lpNybCode[17]='F';
	g_lpNybCode[53]='Q';
	g_lpNybCode[45]='z';
	g_lpNybCode[43]='k';
	g_lpNybCode[58]='L';
	g_lpNybCode[44]='3';
	g_lpNybCode[20]='6';
	g_lpNybCode[37]='g';
	g_lpNybCode[25]='1';
	g_lpNybCode[8] ='_';
	g_lpNybCode[29]='a';
	g_lpNybCode[62]='d';
	g_lpNybCode[52]='-';
	g_lpNybCode[3] ='H';
	g_lpNybCode[51]='m';
	g_lpNybCode[38]='B';
	g_lpNybCode[33]='j';
	g_lpNybCode[57]='K';
ANTIHACK_8
	g_lpNybCode[48]='T';
	g_lpNybCode[34]='t';
	g_lpNybCode[55]='q';
	g_lpNybCode[32]='J';
	g_lpNybCode[18]='8';
	g_lpNybCode[30]='V';
	g_lpNybCode[35]='r';
	g_lpNybCode[16]='l';
_asm // anti-hack
{
	mov eax,2
	cpuid
	mov eax,edx
}
	g_lpNybCode[7] ='I';
	g_lpNybCode[19]='b';
	g_lpNybCode[15]='U';
	g_lpNybCode[31]='x';
ANTIHACK_11
	g_lpNybCode[11]='Z';
	g_lpNybCode[56]='N';
	g_lpNybCode[6] ='W';
	g_lpNybCode[47]='i';
	g_lpNybCode[42]='p';

	// set initialized flag
	bInit=TRUE;
}
#endif

// SetTimeStampStringToFile()

void SetTimeStampStringToFile(LPSTR lpEnc,
							  LPSTR lpFile)
{
	// file handle
	HANDLE hFile=INVALID_HANDLE_VALUE;

	// counters
	DWORD dwCount;
	DWORD dwByteCount;

	// bytes written
	DWORD dwBytesWritten;

	// output byte
	BYTE cOut;

	LockGlobalMsg()

	// get windows directory
	if(!GetWindowsDirectory(g_lpMsg,
							_MAX_PATH))
	{
		UnlockGlobalMsg()
		return;
	}

	// append "\" and filename
	strcat(g_lpMsg,"\\");
	strcat(g_lpMsg,lpFile);

ANTIHACK_5

	// open file
	hFile=CreateFile(g_lpMsg,
					 GENERIC_WRITE,
					 NULL,
					 NULL,
					 CREATE_ALWAYS,
					 NULL,
					 NULL);

	// check handle
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		AddWinResource;

		// reset byte counter
		dwByteCount=0;

		// output 52 random bytes
		for(dwCount=0;dwCount<52;dwCount++)
		{
			// set random output byte
			cOut=(BYTE)(GetRandomInt()%0xFF);

			// output byte
			WriteFile(hFile,
					  &cOut,
					  1,
					  &dwBytesWritten,
					  NULL);

			// increment byte counter
			dwByteCount++;
		}

		// output string (every 3rd byte)
		for(dwCount=0;dwCount<strlen(lpEnc);dwCount++)
		{
			// set output byte
			cOut=(BYTE)(lpEnc[dwCount]);

			// output byte
			WriteFile(hFile,
					  &cOut,
					  1,
					  &dwBytesWritten,
					  NULL);

			// set random output byte
			cOut=(BYTE)(GetRandomInt()%0xFF);

			// output byte
			WriteFile(hFile,
					  &cOut,
					  1,
					  &dwBytesWritten,
					  NULL);

			// set random output byte
			cOut=(BYTE)(GetRandomInt()%0xFF);

ANTIHACK_7

			// output byte
			WriteFile(hFile,
					  &cOut,
					  1,
					  &dwBytesWritten,
					  NULL);

			// increment byte counter
			dwByteCount+=3;
		}

		// output remaining bytes
		while(dwByteCount<256)
		{
			// set random output byte
			cOut=(BYTE)(GetRandomInt()%0xFF);

			// output byte
			WriteFile(hFile,
					  &cOut,
					  1,
					  &dwBytesWritten,
					  NULL);

			// increment byte counter
			dwByteCount++;
		}

ANTIHACK_4

		// close file
		if(CloseHandle(hFile))
			RemoveWinResource;
	}

	UnlockGlobalMsg()
}
							 
// SetTimeStampStringToDataFile()

void SetTimeStampStringToDataFile(LPSTR lpEnc,
								  LPSTR lpFile)
{
	// file handle
	HANDLE hFile=INVALID_HANDLE_VALUE;

	// byte counters
	DWORD dwBytesWritten;
	DWORD dwBytesToCopy;

	// open file
	hFile=CreateFile(lpFile,
					 GENERIC_WRITE,
					 NULL,
					 NULL,
					 OPEN_EXISTING,
					 NULL,
					 NULL);

	// check handle
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		AddWinResource;

		// set file pointer
		if(SetFilePointer(hFile,
						  -32,
						  NULL,
						  FILE_END)==NO_OBJECT)
		{
			if(CloseHandle(hFile))
				RemoveWinResource;
			return;
		}

		// set bytes to copy
		if(strlen(lpEnc)>=31)
			dwBytesToCopy=32;
		else
			dwBytesToCopy=strlen(lpEnc)+1;
			
		// output string
		WriteFile(hFile,
				  lpEnc,
				  dwBytesToCopy,
				  &dwBytesWritten,
				  NULL);

		// close file
		if(CloseHandle(hFile))
			RemoveWinResource;
	}
}

#ifdef SHAREWARE_MODE
// GetTimeStampStringFromFile()

void GetTimeStampStringFromFile(LPSTR lpEnc,
								LPSTR lpFile)
{
	// file handle
	HANDLE hFile=INVALID_HANDLE_VALUE;

	// counters
	DWORD dwCount;

	// bytes read
	DWORD dwBytesRead;

	// set terminator in case of error
	lpEnc[0]=NULL;

	LockGlobalMsg()
	
	// get windows directory
	if(!GetWindowsDirectory(g_lpMsg,
							_MAX_PATH))
	{
		UnlockGlobalMsg()
		return;
	}

	// append "\" and filename
	strcat(g_lpMsg,"\\");
	strcat(g_lpMsg,lpFile);
	
	// open file
	hFile=CreateFile(g_lpMsg,
					 GENERIC_READ,
					 NULL,
					 NULL,
					 OPEN_EXISTING,
					 NULL,
					 NULL);

	// check handle
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		AddWinResource;

		// set file pointer
		if(SetFilePointer(hFile,
						  52,
						  NULL,
						  FILE_BEGIN)==NO_OBJECT)
		{
			if(CloseHandle(hFile))
				RemoveWinResource;
			UnlockGlobalMsg()
			return;
		}
		
		// read string (every 3rd byte)
		for(dwCount=0;dwCount<32;dwCount++)
		{
			// read 3 bytes
			ReadFile(hFile,
					 g_lpMsg,
					 3,
					 &dwBytesRead,
					 NULL);

			// check for error
			if(dwBytesRead!=3)
			{
				if(CloseHandle(hFile))
					RemoveWinResource;
				UnlockGlobalMsg()
				return;
			}

			// copy first byte
			lpEnc[dwCount]=g_lpMsg[0];

			// set null terminator
			lpEnc[dwCount+1]=NULL;
		}

		// close file
		if(CloseHandle(hFile))
			RemoveWinResource;
	}

	UnlockGlobalMsg()
}
							 
// GetTimeStampStringFromDataFile()

void GetTimeStampStringFromDataFile(LPSTR lpEnc,
									LPSTR lpFile)
{
	// file handle
	HANDLE hFile=INVALID_HANDLE_VALUE;

	// byte counter
	DWORD dwBytesRead;

	// set terminator in case of error
	lpEnc[0]=NULL;

	// open file
	hFile=CreateFile(lpFile,
					 GENERIC_READ,
					 NULL,
					 NULL,
					 OPEN_EXISTING,
					 NULL,
					 NULL);

	// check handle
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		AddWinResource;

		// set file pointer
		if(SetFilePointer(hFile,
						  -32,
						  NULL,
						  FILE_END)==NO_OBJECT)
		{
			if(CloseHandle(hFile))
				RemoveWinResource;
			return;
		}

		// read string
		ReadFile(hFile,
				 lpEnc,
				 32,
				 &dwBytesRead,
				 NULL);

		// set final terminator
		lpEnc[32]=NULL;

		// close file
		if(CloseHandle(hFile))
			RemoveWinResource;
	}
}
#endif

// BeginWaitCursor()

int BeginWaitCursor(void)
{
	// check counter
	if(g_nWaitCursorCnt==0)
	{
		// begin wait cursor
		SetCursor(LoadCursor(NULL,IDC_WAIT));
	}

	// increment counter
	g_nWaitCursorCnt++;

	// return counter
	return(g_nWaitCursorCnt);
}

// EndWaitCursor()

int EndWaitCursor(void)
{
	// decrement counter
	g_nWaitCursorCnt--;

	// check counter
	if(g_nWaitCursorCnt<=0)
	{
		// end wait cursor
		SetCursor(LoadCursor(NULL,IDC_ARROW));

		// clip value
		g_nWaitCursorCnt=0;
	}

	// return counter
	return(g_nWaitCursorCnt);
}

#ifdef DEBUG_MODE
// LockGlobalMessageString()

void LockGlobalMessageString(void)
{
	// check flag
	if(_g_bMsgLocked)
		DisplayErrorMessage("Global string collision! (g_lpMsg)",
							g_lpMsg,
							FALSE);

	// set flag
	_g_bMsgLocked=TRUE;
}

// UnlockGlobalMessageString()

void UnlockGlobalMessageString(void)
{
	// reset flag
	_g_bMsgLocked=FALSE;
}
#endif

// eof //

// Copyright 2005-2007 WaveBreak Software
