// Copyright 2005-2007 WaveBreak Software

// r3dwin.h -- engine core header

// flags //

// omit from release version
//#define DEBUG_MODE

// compile data files flag
//#define COMPILE_DATAFILES

// ignore unused data files flag
#define IGNORE_UNUSED_FILES

// screensaver flag
#define SCREENSAVER_MODE

// collision detection flag
//#define USE_COLLDETECT

// DirectInput flag
//#define USE_DIRECTINPUT

// transitions flag
//#define USE_TRANSITIONS

// force aspect ratio flag
//#define FORCE_ASPECTRATIO

// loading output flag
//#define SHOW_LOADINGOUTPUT

// loading screen flag
#define USE_LOADINGSCREEN

// audio/video warnings flag
//#define USE_AVWARNINGS

// shareware mode flag
//#define SHAREWARE_MODE

// flash when minimized flag
//#define FLASH_WHEN_MINIMIZED

// allow multiple instances flag
#define ALLOW_MULTIPLE_INSTANCES

// non-essential errors flag
#define SHOW_NE_ERRORS

// check recompile flag
#ifdef COMPILE_DATAFILES
	#ifndef DEBUG_MODE
		#define DEBUG_MODE
	#endif
#endif

// defines //

#define STRICT
#define WIN32_LEAN_AND_MEAN
#ifdef USE_DIRECTINPUT
	#define DIRECTINPUT_VERSION 0x0800
#endif

// system includes //

#include<objbase.h>
#include<windows.h>
#include<windowsx.h>
#include<winuser.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
#include<direct.h>
#include<shellapi.h>
#include<lzexpand.h>
#include<imagehlp.h>

// directx includes

#include<dxerr8.h>
#include<d3d8.h>
#include<d3d8types.h>
#include<d3dx8math.h>
#include<dmusici.h>
#ifdef USE_DIRECTINPUT
	#include<dinput.h>
#endif

// data types //

typedef unsigned __int64 QWORD;

// structures //

typedef struct tagDISPLAYMODE
{
	// resolution
	DWORD dwX,dwY;
	// format
	D3DFORMAT fmt;
	// windowed flag
	BOOL bWindowed;
	// lockable flag
	BOOL bLockable;
	// back buffer count
	DWORD dwBackBufferCount;
	// optimization
	DWORD dwDummy[2];
} DISPLAYMODE,*LPDISPLAYMODE;

typedef struct tagDATAFILEHEADER
{
	// size of data block
	DWORD dwSize;
	// number of references
	DWORD dwNumRef;
	// position of offset table
	DWORD dwOffsetPos;
	// position of size table
	DWORD dwSizePos;
	// position of data table
	DWORD dwDataPos;
	// optimization
	DWORD dwDummy[3];
} DATAFILEHEADER,*LPDATAFILEHEADER;

#ifdef SHAREWARE_MODE
typedef struct tagTIMESTAMP
{
	// persistent id
	DWORD dwID;
	// install time
	DWORD dwInstTimeLow;
	DWORD dwInstTimeHigh;
	// days left
	DWORD dwDaysLeft;
} TIMESTAMP,*LPTIMESTAMP;
#endif

typedef struct tagDATAFILEREF
{
	// pointer to data
	LPVOID lpData;
	// size of data
	DWORD dwSize;
} DATAFILEREF,*LPDATAFILEREF;

// classes //

class CNode
{
public:
	CNode();
	CNode* AddChild(LPSTR lpName=NULL);
	CNode* AddSibling(LPSTR lpName=NULL);
	CNode* GetChild(void);
	CNode* GetSibling(void);
	CNode* FindChild(LPSTR lpName);
	CNode* FindSibling(LPSTR lpName);
	CNode* FindNode(LPSTR lpName);
	void SetNodeData(LPVOID lpData);
	LPVOID GetNodeData(void);
	void SetNodeName(LPSTR lpName);
	LPSTR GetNodeName(void);
	void DeleteChild(void);
	void DeleteSibling(void);
	void DeleteNode(void);
	BOOL AllocNodeMem(DWORD dwSize);
	void FreeNodeMemChild(void);
	void FreeNodeMemSibling(void);
	void FreeNodeMem(void);

private:
	// pointers to members
	CNode* lpChild;
	CNode* lpSibling;
	// pointer to name
	LPSTR lpNodeName;
	// pointer to data
	LPVOID lpNodeData;
};

// constants //

#define MSG_SIZE 512

#define EPSILON 0.000001F

#define PI 3.141592653F

#define BLOCK_ALLOC_INC 32

// random number lookup has been disabled
//#define RANDOM_NUMBERS 4096

#define MAX_FRAME_SKIP 15

#define KEY_BUFFER_SIZE 32

// standard dialog pushbuttons
#define BTN_OK		0xFFFF
#define BTN_CANCEL	0xFFFE

#ifdef FLASH_WHEN_MINIMIZED
// flash timer constants
#define FLASH_TIMER_ID		 1
#define FLASH_TIMER_DURATION 1000
#endif

// timing constants
#define MSEC_PER_FRAME 17
#define MSEC_PER_SEC   1000

#ifdef SCREENSAVER_MODE
// screensaver mode flags
#define SSMODE_INVALID 0
#define SSMODE_NORMAL  1
#define SSMODE_CONFIG  2
#define SSMODE_PREVIEW 3
#define SSMODE_PASSWD  4
#define SSMODE_WBS_CMD 5

// WaveBreak Software special commands
#define WBSCMD_PURCHASE   1000
#define WBSCMD_SETCURRENT 2000
#define WBSCMD_SSOPTIONS  3000
#endif

#ifdef USE_LOADINGSCREEN
// loading screen update increment
#define LS_UPDATEINC 80
#endif

// 100-nanosecond intervals per day
#define CNSEC_PER_DAY 0xC92A69C000I64

// engine includes //

#include"r3ddxg.h" // DirectX Graphics wrapper
#include"r3ddxa.h" // DirectX Audio wrapper
#ifdef USE_DIRECTINPUT
	#include"r3ddxi.h" // DirectInput wrapper
#endif
#ifdef USE_COLLDETECT
	#include"r3dcol.h" // collision detection
#endif
#include"r3dxfa.h" // x-file animation support
#include"r3dapp.h" // primary application
#include"..\\source\\resource.h" // resource header

// globals //

// handle to application window
extern HWND g_hWnd;
// handle to application instance
extern HINSTANCE g_hInst;
// handle to duplication mutex
extern HANDLE g_hMutex;
// paused flag
extern BOOL g_bIdle;
// closing flags
extern BOOL g_bClosing;
#ifdef SHAREWARE_MODE
extern BOOL g_bPreClose;
extern BOOL g_bCloseOK;
extern BOOL g_bPurchPage;
#endif
// display modes
extern DISPLAYMODE g_dmApp;
extern DISPLAYMODE g_dmSys;
extern DISPLAYMODE g_dmOrigSys;
// window size
extern int g_nXSize;
extern int g_nYSize;
// window position
extern int g_nXPos;
extern int g_nYPos;
// frame skip
extern DWORD g_dwFrameSkip;
// frames per second
extern DWORD g_dwFPS;
// updates per second
extern DWORD g_dwUPS;
// application timer
extern DWORD g_dwAppTimer;
// timer counters
extern DWORD g_dwNumTimer;
extern DWORD g_dwMaxTimer;
// timer data
extern LPDWORD g_lpdwTimerLast;
extern LPDWORD g_lpdwTimerInc;
// random number tables (disabled)
//extern float* g_lpfRand;
//extern int* g_lpnRand;
// resource counters
extern DWORD g_dwNumCOMRes;
extern DWORD g_dwNumWinRes;
extern DWORD g_dwNumMemRes;
extern DWORD g_dwNumClassRes;
// message strings
extern char g_lpMsg[];
extern char g_lpErrorMsg[];
extern WCHAR g_lpWMsg[];
// registry keys
extern HKEY g_hSoftKey;
extern HKEY g_hAppKey;
// script file handles
#ifdef DEBUG_MODE
extern HANDLE g_hScriptFile;
#endif
extern HANDLE g_hCScriptFile;
// script file data
extern LPBYTE g_lpCScriptData;
extern DWORD g_dwCScriptDataPtr;
// system mouse info
extern BOOL g_bWinMouse;
extern int g_nMouseX;
extern int g_nMouseY;
extern int g_nMousePX;
extern int g_nMousePY;
extern int g_nMouseDX;
extern int g_nMouseDY;
extern BOOL g_bMouseBtnL;
extern BOOL g_bMouseBtnR;
extern BOOL g_bMousePBtnL;
extern BOOL g_bMousePBtnR;
extern BOOL g_bMouseClkL;
extern BOOL g_bMouseClkR;
#ifdef DEBUG_MODE
extern BOOL g_bMouseBtnS;
extern BOOL g_bMouseBtnC;
extern BOOL g_bMousePBtnS;
extern BOOL g_bMousePBtnC;
extern BOOL g_bMouseClkS;
extern BOOL g_bMouseClkC;
#endif
extern DWORD g_dwMouseSkip;
// dialog box counters
extern DWORD g_dwNumDlg;
extern DWORD g_dwMaxDlg;
// dialog box data
extern LPBYTE* g_lplpDlgData;
extern LPDWORD g_lpdwDlgDataSize;
extern LPDWORD g_lpdwDlgDataPtr;
extern HWND g_hCurrDlg;
extern int* g_lpnDlgResID;
// accelerator keys
extern HACCEL g_hAccel;
extern ACCEL g_acAltEnter;
// pause bitmap data
extern BITMAPINFOHEADER g_bih;
extern LPBYTE g_lpPauseBmpData;
// data file counters
extern DWORD g_dwNumDataFileRef;
extern DWORD g_dwMaxDataFileRef;
// data file data
extern HANDLE g_hDataFile;
extern LPBYTE g_lpDataFile;
#ifdef DEBUG_MODE
extern LPSTR* g_lplpDataFileRefName;
extern LPBOOL g_lpbDataFileRefUsed;
#endif
extern DWORD g_dwDataFileOffsetPos;
extern DWORD g_dwDataFileSizePos;
extern DWORD g_dwDataFileDataPos;
// COM flag
extern BOOL g_bCOMReady;
// initialized flag
extern BOOL g_bAppInit;
#ifdef SHAREWARE_MODE
// nybble codes
char g_lpNybCode[];
#endif
// activity flags
extern BOOL g_bMouseMove;
extern BOOL g_bKeyPress;
// anti-hacking values
extern DWORD g_dwAHVal1;
extern DWORD g_dwAHVal2;
extern DWORD g_dwAHVal3;
extern DWORD g_dwAHVal4;
// wait-cursor counter
extern int g_nWaitCursorCnt;
#ifdef DEBUG_MODE
// message locking flags
extern BOOL _g_bMsgLocked;
#endif

// prototypes //

// window handling functions

extern LRESULT CALLBACK WindowProc(HWND hWnd,
								   UINT Msg,
								   WPARAM wParam,
								   LPARAM lParam);

extern void PrimaryMessageLoop(void);

extern void EnterIdleMode(void);

extern void ExitIdleMode(void);

extern void SendCloseMessage(void);

#ifndef ALLOW_MULTIPLE_INSTANCES
extern BOOL LockApplication(LPCSTR lpID,
							LPSTR lpWndTitle,
							LPSTR lpWndClass,
							BOOL bClose);

extern void UnlockApplication(void);
#endif

extern BOOL AddApplicationWindowClass(void);

extern void RemoveApplicationWindowClass(void);

extern BOOL CreateApplicationWindow(void);

extern void DestroyApplicationWindow(void);

extern DISPLAYMODE GetSystemDisplayMode(void);

extern void SetSystemDisplayMode(DWORD dwX=0,
								 DWORD dwY=0);

extern void CalculateWindowMetrics(void);

extern BOOL SetDataPathRel(LPCSTR lpDir,
						   BOOL bSupressErrorMsg=FALSE);

extern BOOL SetDataPathAbs(LPCSTR lpDir,
						   BOOL bSupressErrorMsg=FALSE);

extern BOOL ProcessPendingMessages(void);

extern void ProcessMouseTracking(void);

extern void ResetMousePosition(void);

extern void BeginExclusiveMode(void);

extern void EndExclusiveMode(void);

extern void MinimizeApplicationWindow(void);

extern void UpdateWindowMetrics(void);

extern void RestoreApplicationWindow(void);

extern BOOL InitAcceleratorTable(void);

extern void ReleaseAcceleratorTable(void);

extern void ShowWindowsCursor(void);

extern void HideWindowsCursor(void);

extern BOOL CapturePauseBitmap(D3DCOLOR clrFilter);

extern void ReleasePauseBitmap(void);

extern int BeginWaitCursor(void);

extern int EndWaitCursor(void);

// object handling functions

extern BOOL InitializeAllModules(void);

extern void ReleaseAllModules(void);

extern BOOL InitializeCOM(void);

extern void ReleaseCOM(void);

extern BOOL InitializeDirectXApplication(void);

extern BOOL VerifyDirectXVersion(void);

extern void ReleaseDirectXApplication(void);

// error handling functions

extern void DisplayErrorMessage(LPSTR lpError,
								LPSTR lpSolution=NULL,
								BOOL bExtInfo=TRUE,
								BOOL bFatal=FALSE);

extern void DisplayNEErrorMessage(LPSTR lpError,
								  LPSTR lpSolution=NULL,
								  BOOL bExtInfo=TRUE,
								  BOOL bFatal=FALSE);

extern void DisplayFatalErrorMessage(LPSTR lpError,
									 LPSTR lpSolution=NULL,
									 BOOL bExtInfo=TRUE);

extern void DisplayDXErrorMessage(LPSTR lpError,
								  LPSTR lpSolution=NULL,
								  HRESULT hError=NULL,
								  BOOL bFatal=FALSE);

extern void DisplaNEDXErrorMessage(LPSTR lpError,
								   LPSTR lpSolution=NULL,
								   HRESULT hError=NULL,
								   BOOL bFatal=FALSE);

extern void DisplayFatalDXErrorMessage(LPSTR lpError,
									   LPSTR lpSolution=NULL,
									   HRESULT hError=NULL);

extern void DisplayInfoMessage(LPSTR lpError);

// misc functions

extern void SwapPtrs(LPVOID* lplpPtr1,
					 LPVOID* lplpPtr2);

extern BOOL AllocMem(LPVOID* lplpPtr,
					 DWORD dwSize);

extern BOOL ExpandMem(LPVOID* lplpPtr,
	 			      DWORD dwSize,
 				      DWORD dwExpAmt);

extern BOOL CompressMem(LPVOID* lplpPtr,
	 			        DWORD dwSize);

extern void FreeMem(LPVOID* lplpPtr);

extern BOOL InitRandomNumbers(void);

extern float GetRandomFloat(BOOL bPrevious=FALSE);

extern int GetRandomInt(BOOL bPrevious=FALSE);

extern void ReleaseRandomNumbers(void);

extern BOOL VerifyClassObject(LPVOID lpPtr);

#ifdef DEBUG_MODE
extern void DisplayResourceUsage(void);
#endif

extern void ConfirmResourcesReleased(void);

extern DWORD DivideString(LPSTR lpStr);

extern LPSTR GetStringDivision(LPSTR lpStr,
							   DWORD dwIndex);

extern int DisplayMessageBox(HWND hWnd,
							 LPSTR lpText,
							 LPSTR lpCaption,
							 UINT uType);

extern BOOL BeginSystemOutput(void);

extern void EndSystemOutput(BOOL bFlag);

extern DWORD GetFileLength(LPSTR lpFile);

extern void PurgeSystemObjects(void);

extern void ForceImmediateExit(void);

extern void CapitalizeString(LPSTR lpStr);

extern DWORD BinaryToDWORD(LPSTR lpStr);

#ifdef SCREENSAVER_MODE
extern BOOL ProcessScreensaverFlags(LPSTR lpCmdLine);

extern BOOL ProcessScreensaverCmdLine(LPSTR lpCmdLine,
									  LPDWORD lpdwMode,
									  HWND* lphWnd);

extern void ChangeScreensaverPassword(HWND hParentWnd);

extern BOOL ConfirmScreensaverPassword(void);

extern void ExecutePreviewImageWindow(HWND hParentWnd);

extern BOOL CreatePreviewImageWindow(HWND hParentWnd);

extern void DestroyPreviewImageWindow(void);

extern BOOL AddPreviewImageWindowClass(void);

extern void RemovePreviewImageWindowClass(void);

extern LRESULT CALLBACK PreviewImageWindowProc(HWND hWnd,
											   UINT Msg,
											   WPARAM wParam,
											   LPARAM lParam);

extern void ProcessScreensaverMessages(void);
#endif

extern BOOL ExecuteExternalApp(LPTSTR lpCmdLine,
							   LPCTSTR lpWorkingDir=NULL);

extern BOOL OpenExternalDoc(LPCTSTR lpFileName,
							LPCTSTR lpWorkingDir=NULL);

extern BOOL PrintExternalDoc(LPCTSTR lpFileName,
							 LPCTSTR lpWorkingDir=NULL);

extern BOOL ExecuteCommandLine(LPCTSTR lpExeFile,
							   LPCTSTR lpCmdLine,
							   LPCTSTR lpWorkingDir=NULL);

extern BOOL SetCurrentScreensaver(LPCSTR lpScr);

extern BOOL OpenScreensaverControlPanel(void);

#ifdef DEBUG_MODE
extern void LockGlobalMessageString(void);

extern void UnlockGlobalMessageString(void);
#endif

// registry functions

extern BOOL OpenRegistryRead(BOOL bSystemWide=FALSE);

extern BOOL OpenRegistryWrite(BOOL bSystemWide=FALSE);

extern void CloseRegistry(void);

extern void SetRegistryKey(LPCSTR lpName,
						   LPSTR lpData);

extern LPSTR GetRegistryKey(LPCSTR lpName);

extern void DeleteRegistryKey(LPCSTR lpName);

extern void SetSystemWideRegistryKey(LPCSTR lpName,
									 LPSTR lpData);

extern LPSTR GetSystemWideRegistryKey(LPCSTR lpName);

extern void DeleteSystemWideRegistryKey(LPCSTR lpName);

extern void FreeRegistryKey(LPSTR lpData);

extern int GetRegistryConfigValue(LPCSTR lpName,
								  int nMin,
								  int nMax,
								  int nDef);

extern void SetRegistryConfigValue(LPCSTR lpName,
								   int nVal);

// system mouse functions

extern void EnableMouseTracking(void);

extern void DisableMouseTracking(void);

// timer functions

extern DWORD GetNextTimer(void);

extern DWORD AddTimer(DWORD dwFramesPerTick);

extern void UpdateTimers(void);

extern DWORD CheckTimer(DWORD dwIndex);

extern void PurgeTimers(void);

// script functions

#ifdef DEBUG_MODE
extern BOOL OpenScriptCompiler(LPCSTR lpDestFile,
							   LPCSTR lpSrcFile);

extern char ReadScriptChar(void);

extern long ReadScriptLong(void);

extern float ReadScriptFloat(void);

extern DWORD ReadScriptString(LPSTR lpStr);

extern BOOL SaveScriptChar(char cVal);

extern BOOL SaveScriptLong(long lVal);

extern BOOL SaveScriptFloat(float fVal);

extern BOOL SaveScriptString(LPSTR lpStr);

extern BOOL CopyScriptChar(void);

extern BOOL CopyScriptBytes(DWORD dwNumBytes);

extern BOOL CopyScriptLong(void);

extern BOOL CopyScriptFloat(void);

extern BOOL CopyScriptString(void);

extern void CloseScriptCompiler(void);

extern BOOL ResetScriptCompilerSource(LPCSTR lpFile);

extern BOOL OpenScriptCreator(LPCSTR lpFile);

extern BOOL CreateScriptString(LPSTR lpStr);

extern void CloseScriptCreator(void);

extern BOOL IsValidScriptChar(char c);
#endif

extern BOOL OpenCompiledScript(LPCSTR lpFile);

extern char ReadCScriptChar(void);

extern void ReadCScriptBytes(LPVOID lpDest,
							 DWORD dwNumBytes);

extern long ReadCScriptLong(void);

extern float ReadCScriptFloat(void);

extern void ReadCScriptString(LPSTR lpStr);

extern void CloseCompiledScript(void);

// dialog box functions

extern DWORD GetNextDialog(void);

extern DWORD AddDialog(short sXSize,
					   short sYSize,
					   LPCSTR lpTitle);

extern void ExpandDialog(DWORD dwIndex);

extern void AlignDialogWORD(DWORD dwIndex);

extern void AlignDialogDWORD(DWORD dwIndex);

extern void AddDialogDWORD(DWORD dwIndex,
						   DWORD dwVal);

extern void AddDialogWORD(DWORD dwIndex,
						  WORD wVal);

extern void AddDialogBYTE(DWORD dwIndex,
						  BYTE cVal);

extern void AddDialogShort(DWORD dwIndex,
						   short sVal);

extern void AddDialogCoords(DWORD dwIndex,
						    short sX,
							short sY);

extern void AddDialogString(DWORD dwIndex,
						    LPSTR lpStr);

extern void AddDialogPtr(DWORD dwIndex,
						 LPVOID lpPtr);

extern int ExecuteDialog(DWORD dwIndex,
						 DLGPROC lpDlgProc,
						 HWND hWnd=(HWND)INVALID_HANDLE_VALUE);

extern void PurgeDialogs(void);

extern void AddDialogControl(DWORD dwIndex);

extern void AddDialogPushButton(DWORD dwIndex,
								short sXPos,
								short sYPos,
								short sXSize,
								short sYSize,
								LPCSTR lpText,
								WORD wID,
								BOOL bDefBtn=FALSE);

extern void AddDialogCheckBox(DWORD dwIndex,
							  short sXPos,
							  short sYPos,
							  short sXSize,
							  short sYSize,
							  LPCSTR lpText,
							  WORD wID);

extern void AddDialogRadioButton(DWORD dwIndex,
								 short sXPos,
								 short sYPos,
								 short sXSize,
								 short sYSize,
								 LPCSTR lpText,
								 WORD wID);

extern void AddDialogGroupBox(DWORD dwIndex,
							  short sXPos,
							  short sYPos,
							  short sXSize,
							  short sYSize,
							  LPCSTR lpText);

extern void AddDialogDropDownListBox(DWORD dwIndex,
									 short sXPos,
									 short sYPos,
									 short sXSize,
									 short sYSize,
									 WORD wID);

extern void AddDialogEditControl(DWORD dwIndex,
								 short sXPos,
								 short sYPos,
								 short sXSize,
								 short sYSize,
								 WORD wID);

extern void AddDialogListBox(DWORD dwIndex,
							 short sXPos,
							 short sYPos,
							 short sXSize,
							 short sYSize,
							 WORD wID);

extern void AddDialogStaticControl(DWORD dwIndex,
								   short sXPos,
								   short sYPos,
								   short sXSize,
								   short sYSize,
								   LPCSTR lpText);

extern void AddDialogGroup(DWORD dwIndex);

extern BOOL SetCurrentDialogCheckBox(WORD wID,
									 BOOL bVal);

extern BOOL GetCurrentDialogCheckBox(WORD wID);

extern BOOL DisableCurrentDialogCheckBox(WORD wID);

extern BOOL SetCurrentDialogEditText(WORD wID,
									 LPSTR lpText);

extern BOOL GetCurrentDialogEditText(WORD wID,
									 LPSTR lpText,
									 int nMaxCount);

extern BOOL SetCurrentDialogEditFloat(WORD wID,
									  float fVal);

extern float GetCurrentDialogEditFloat(WORD wID);

extern BOOL SetCurrentDialogEditLong(WORD wID,
									 long lVal);

extern long GetCurrentDialogEditLong(WORD wID);

extern void SetCurrentDialogBox(HWND hDlg);

extern BOOL GetCurrentDialogDropDownSelection(DWORD dwID,
											  LPSTR lpText);

extern BOOL ResetCurrentDialogDropDownSelections(DWORD dwID);

extern BOOL AddCurrentDialogDropDownSelection(DWORD dwID,
											  LPSTR lpText);

extern BOOL RemoveCurrentDialogDropDownSelection(DWORD dwID);

extern BOOL HighlightCurrentDialogDropDownSelection(DWORD dwID,
													LPSTR lpText);

extern DWORD AddDialogFromResource(int nResID);

extern BOOL SetCurrentDialogScrollInfo(WORD wID,
									   int nMin,
									   int nMax,
									   UINT nPage,
									   int nPos);

extern int GetCurrentDialogScrollPos(WORD wID);

extern void HighlightCurrentDialogEditText(WORD wID,
										   int nStart=0,
										   int nEnd=-1);

extern BOOL UpdateScrollBarPos(HWND hScroll,
							   int nPosInc);

extern BOOL SetScrollBarPos(HWND hScroll,
							int nPos);

// data file functions

#ifdef DEBUG_MODE
extern BOOL OpenDataCompiler(LPCSTR lpFile);

extern DWORD AddDataFileRef(LPCSTR lpFile);

extern void CloseDataCompiler(LPCSTR lpFile);

extern BOOL CompressDataFile(LPCSTR lpFile);

extern BOOL AddTimeStampToDataFile(LPCSTR lpFile);
#endif

extern BOOL OpenDataFile(LPCSTR lpFile);

extern DATAFILEREF FindDataFileRef(LPSTR lpFile);

extern void CloseDataFile(void);

extern void SetTimeStampStringToFile(LPSTR lpEnc,
									 LPSTR lpFile);
							 
extern void SetTimeStampStringToDataFile(LPSTR lpEnc,
										 LPSTR lpFile);

#ifdef SHAREWARE_MODE
extern void GetTimeStampStringFromFile(LPSTR lpEnc,
									   LPSTR lpFile);
							 
extern void GetTimeStampStringFromDataFile(LPSTR lpEnc,
										   LPSTR lpFile);
#endif

// font functions

extern LOGFONT SetFont(LPSTR FaceName,
					   LONG Height,
					   LONG Width=0,
					   LONG Angle=0,
					   LONG Weight=FW_DONTCARE,
					   BYTE Italic=FALSE,
					   BYTE Underline=FALSE,
					   BYTE Strikeout=FALSE);

extern DWORD AddFont(LPSTR FaceName,
					 LONG Height,
					 LONG Width=0,
					 LONG Angle=0,
					 LONG Weight=FW_NORMAL,
					 BYTE Italic=FALSE,
					 BYTE Underline=FALSE,
					 BYTE Strikeout=FALSE);

extern DWORD GetNextFont(void);

extern DWORD AddFont(LOGFONT* lplf);

extern BOOL BeginText(DWORD dwIndex);

extern void OutputText(LPSTR lpText,
					   int nX,
					   int nY,
					   COLORREF crForeground=0x00FFFFFF,
					   COLORREF crBackground=0x00000000,
					   DWORD dwFormat=DT_LEFT|DT_TOP);

extern void EndText(void);

extern void PurgeFonts(void);

#ifdef SHAREWARE_MODE
// trial time limit functions

extern DWORD GetHDDSerialNumber(void);

extern void EncodeTimeStamp(TIMESTAMP tsSrc,
							LPSTR lpStr);

extern TIMESTAMP DecodeTimeStamp(LPSTR lpStr);

extern void EncodeDWORD(LPDWORD lpDW);

extern void DecodeDWORD(LPDWORD lpDW);

extern BYTE EncodeNybble(BYTE cNybble);

extern BYTE DecodeNybble(BYTE cByte);

extern void InitNybbleCodes(void);

extern void EncodeBytesAsString(LPBYTE lpcBytes,
								DWORD dwNumBytes,
								LPSTR lpStr);

extern void DecodeBytesFromString(LPSTR lpStr,
								  LPBYTE lpcBytes,
								  DWORD dwNumBytes);
#endif

// macros //

#define AddCOMResource   { g_dwNumCOMRes++; }
#define AddWinResource   { g_dwNumWinRes++; }
#define AddMemResource   { g_dwNumMemRes++; }
#define AddClassResource { g_dwNumClassRes++; }

#define AddCOMResources(n) { g_dwNumCOMRes+=n; }

#define RemoveCOMResource   { g_dwNumCOMRes--; }
#define RemoveWinResource   { g_dwNumWinRes--; }
#define RemoveMemResource   { g_dwNumMemRes--; }
#define RemoveClassResource { g_dwNumClassRes--; }

#define ReleaseCOMPtr(a) { if(a) { a->Release(); a=NULL; RemoveCOMResource; } }

#define AllocClassObject(a,b) { a=new b; if(VerifyClassObject((LPVOID)(a))) AddClassResource; }
#define FreeClassObject(a)    { if(a) { delete a; a=NULL; RemoveClassResource; } }

#define InitStruct(a) { ZeroMemory(&a,sizeof(a)); }

#define IsNearEqual(a,b) (fabsf(((float)(a))-((float)(b)))<EPSILON)
#define IsNearZero(a)    (fabsf((float)(a))<EPSILON)

#define DegToRad(a) ((float)((((float)(a))/360.0F)*(2.0F*PI)))
#define RadToDeg(a) ((float)((((float)(a))/(2.0F*PI))*360.0F))

#define Squared(a) ((a)*(a))

#define IsKeyDown(a) (GetAsyncKeyState(a)&0x8000)

#define IsMouseInRect(r)           (g_nMouseX>=r.left&&g_nMouseX<=r.right&&g_nMouseY>=r.top&&g_nMouseY<=r.bottom)
#define IsMouseInArea(x1,y1,x2,y2) (g_nMouseX>=x1    &&g_nMouseX<=x2     &&g_nMouseY>=y1   &&g_nMouseY<=y2)

#define MakeWideString(d,s) { MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,s,-1,d,_MAX_PATH); }

#define IsAlphabeticChar(c) (((c)>='A'&&(c)<='Z')||((c)>='a'&&(c)<='z'))

#ifdef SHOW_LOADINGOUTPUT
	#ifdef USE_LOADINGSCREEN
		#define LoadingOutput(a) { UpdateLoadingScreen(); if(g_dwLOFont!=NO_OBJECT) { if(BeginText(g_dwLOFont)) { OutputText("                                       ",0,0); OutputText(a,0,0); EndText(); Sleep(1); } } }
	#endif
#endif

#ifdef SHOW_LOADINGOUTPUT
	#ifndef USE_LOADINGSCREEN
		#define LoadingOutput(a) { if(g_dwLOFont!=NO_OBJECT) { if(BeginText(g_dwLOFont)) { OutputText("                                       ",0,0); OutputText(a,0,0); EndText(); Sleep(1); } } }
	#endif
#endif

#ifndef SHOW_LOADINGOUTPUT
	#ifdef USE_LOADINGSCREEN
		#define LoadingOutput(a) { UpdateLoadingScreen(); }
	#endif
#endif

#ifndef SHOW_LOADINGOUTPUT
	#ifndef USE_LOADINGSCREEN
		#define LoadingOutput(a) {}
	#endif
#endif

#ifdef DEBUG_MODE
	#define LockGlobalMsg()   LockGlobalMessageString();
	#define UnlockGlobalMsg() UnlockGlobalMessageString();
#else
	#define LockGlobalMsg()   {}
	#define UnlockGlobalMsg() {}
#endif

// anti-hacking macros

typedef BOOL (WINAPI *ISDEBUGGERPRESENT) (void);

#define ANTIHACK_1  { HINSTANCE hKernel32Dll=LoadLibrary("kernel32.dll"); \
					if(!hKernel32Dll) return(FALSE); else AddWinResource; \
					ISDEBUGGERPRESENT IsDebuggerPresent=(ISDEBUGGERPRESENT)GetProcAddress(hKernel32Dll,"IsDebuggerPresent"); \
					g_dwAHVal3=NULL; \
					if(IsDebuggerPresent) if(IsDebuggerPresent()) g_dwAHVal3=NO_OBJECT; \
					if(FreeLibrary(hKernel32Dll)) RemoveWinResource; }
#define ANTIHACK_2  MapFileAndCheckSum("swanla~1.scr",&g_dwAHVal4,&g_dwAHVal2);
#define ANTIHACK_3  g_dwAHVal1=(DWORD)(GetRandomInt()%0xFFFF00FF)|((g_dwAHVal4&0xFF)<<8);
#define ANTIHACK_4  g_dwAHVal3&=g_dwAHVal1;
#define ANTIHACK_5  g_dwAHVal4++;
#define ANTIHACK_6  g_dwAHVal3=!g_dwAHVal1;;
#define ANTIHACK_7  g_dwAHVal2=0xFF00FF;;
#define ANTIHACK_8  g_dwAHVal3=g_dwAHVal2%17;
#define ANTIHACK_9  g_dwAHVal1=g_dwAHVal3;
#define ANTIHACK_10 g_dwAHVal2=g_dwAHVal1&0xFFFF0000;
#define ANTIHACK_11 g_dwAHVal3=g_dwAHVal4&0x0000FFFF;
#define ANTIHACK_12 g_dwAHVal4=g_dwAHVal2&g_dwAHVal3;

// messages //

#define IDM_TOGGLEFULLSCREEN WM_USER+100
	
// eof //

// Copyright 2005-2007 WaveBreak Software
