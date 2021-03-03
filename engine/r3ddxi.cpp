// Copyright 2005-2007 WaveBreak Software

/////////////////////////////////////////////////////////////////////
// perform step-by-step walkthrough with COMPLETE error checking!! //
/////////////////////////////////////////////////////////////////////

// r3ddxi.cpp -- DirectInput wrapper

// includes //

#include"r3dwin.h"

// globals //

// directinput interface
IDirectInput8* g_lpDI=NULL;
// input devices
IDirectInputDevice8* g_lpKeyboard=NULL;
IDirectInputDevice8** g_lplpJoystick=NULL;
// joystick counter
DWORD g_dwNumJoystick=0;
// joystick data
LPSTR* g_lplpJoystickName=NULL;
LPGUID g_lpguidJoystickInst=NULL;
LPDIJOYSTATE g_lpJoystickState=NULL;
float* g_lpfJoystickXMid=NULL;
float* g_lpfJoystickXRange=NULL;
float* g_lpfJoystickYMid=NULL;
float* g_lpfJoystickYRange=NULL;
LPDIDEVCAPS g_lpJoystickCaps=NULL;
// button counters
DWORD g_dwNumButton=0;
DWORD g_dwMaxButton=0;
// button data
LPBOOL g_lpbButtonDown=NULL;
LPBOOL g_lpbButtonDownP=NULL;
LPBOOL g_lpbButtonHit=NULL;
// axis counters
DWORD g_dwNumAxis=0;
DWORD g_dwMaxAxis=0;
// axis data
float* g_lpfAxisPos=NULL;
LPBOOL g_lpbAxisMinDownP=NULL;
LPBOOL g_lpbAxisMinHit=NULL;
LPBOOL g_lpbAxisMaxDownP=NULL;
LPBOOL g_lpbAxisMaxHit=NULL;
// button trigger counters
DWORD g_dwNumButtonTrig=0;
DWORD g_dwMaxButtonTrig=0;
// button trigger data
LPDWORD g_lpdwButtonTarget=NULL;
LPDWORD g_lpdwButtonTrig=NULL;
// axis trigger counters
DWORD g_dwNumAxisTrig=0;
DWORD g_dwMaxAxisTrig=0;
// axis trigger data
LPDWORD g_lpdwAxisTarget=NULL;
LPDWORD g_lpdwAxisTrig1=NULL;
LPDWORD g_lpdwAxisTrig2=NULL;
// keyboard data
BYTE g_lpcKeys[256];
// default input settings
DWORD g_dwDefXAxis=NO_OBJECT;
DWORD g_dwDefYAxis=NO_OBJECT;
DWORD g_dwDefCancelBtn=NO_OBJECT;
DWORD g_dwDefSelectBtn=NO_OBJECT;
DWORD g_dwDefHelpBtn=NO_OBJECT;

// functions //

// DXInputInit()

BOOL DXInputInit(void)
{
	LoadingOutput("DXInputInit");

	// create directinput interface
	if(!InitDIInterface())
		return(FALSE);

	// initialize keyboard
	if(!InitKeyboard())
		return(FALSE);

	// initialize joysticks
	if(!InitJoysticks())
		return(FALSE);

	// ok
	return(TRUE);
}

// DXInputRelease()

void DXInputRelease(void)
{
	// purge input settings
	PurgeInputSettings();
	
	// unacquire devices
	UnacquireAllInputDevices();
	
	// release all devices
	ReleaseKeyboard();
	ReleaseJoysticks();
	
	// release directinput interface
	ReleaseDIInterface();
}

// InitDIInterface()

BOOL InitDIInterface(void)
{
	LoadingOutput("InitDIInterface");

	// create directinput interface
	HRESULT hr=DirectInput8Create(g_hInst,
								  DIRECTINPUT_VERSION,
								  IID_IDirectInput8A,
								  (LPVOID*)&g_lpDI,
								  NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to initialize DirectInput.",
							  NULL,
							  hr);
		return(FALSE);
	}
	else
		AddCOMResource;

	// ok
	return(TRUE);
}

// ReleaseDIInterface()

void ReleaseDIInterface(void)
{
	// release interface
	ReleaseCOMPtr(g_lpDI);
}

// InitKeyboard()

BOOL InitKeyboard(void)
{
	LoadingOutput("InitKeyboard");

	// used for error checking
	HRESULT hr;

	// create keyboard interface
	hr=g_lpDI->CreateDevice(GUID_SysKeyboard,
							&g_lpKeyboard,
							NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create keyboard interface.",
							  NULL,
							  hr);
		return(FALSE);
	}
	else
		AddCOMResource;

	// set data format
	hr=g_lpKeyboard->SetDataFormat(&c_dfDIKeyboard);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set keyboard data format.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// set cooperative level
	hr=g_lpKeyboard->SetCooperativeLevel(g_hWnd,
										 DISCL_FOREGROUND|
										 DISCL_NONEXCLUSIVE);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set keyboard cooperative level.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// acquire device
	if(!AcquireKeyboard())
		return(FALSE);

	// ok
	return(TRUE);
}

// ReleaseKeyboard()

void ReleaseKeyboard(void)
{
	// release interface
	ReleaseCOMPtr(g_lpKeyboard);
}

// InitJoysticks()

BOOL InitJoysticks(void)
{
	LoadingOutput("InitJoysticks");

	// used for error checking
	HRESULT hr;

	// enumerate joystick devices
	hr=g_lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL,
						   (LPDIENUMDEVICESCALLBACK)EnumJoysticksCallback,
						   NULL,
						   DIEDFL_ATTACHEDONLY);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to enumerate game control devices.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// ReleaseJoysitcks()

void ReleaseJoysticks(void)
{
	// counter
	DWORD dwCount;
	
	// check counter
	if(g_dwNumJoystick)
	{
		// release internal data
		for(dwCount=0;dwCount<g_dwNumJoystick;dwCount++)
		{
			// release interface
			ReleaseCOMPtr(g_lplpJoystick[dwCount]);

			// free name
			FreeMem((LPVOID*)&g_lplpJoystickName[dwCount]);
		}

		// free memory
		FreeMem((LPVOID*)&g_lplpJoystick);
		FreeMem((LPVOID*)&g_lplpJoystickName);
		FreeMem((LPVOID*)&g_lpguidJoystickInst);
		FreeMem((LPVOID*)&g_lpJoystickState);
		FreeMem((LPVOID*)&g_lpfJoystickXMid);
		FreeMem((LPVOID*)&g_lpfJoystickXRange);
		FreeMem((LPVOID*)&g_lpfJoystickYMid);
		FreeMem((LPVOID*)&g_lpfJoystickYRange);
		FreeMem((LPVOID*)&g_lpJoystickCaps);
		
		// reset counter
		g_dwNumJoystick=0;
	}
}

// EnumJoysticksCallback()

BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE lpddi,
									LPVOID lpRefData)
{
	LoadingOutput("InitJoysticksCallback");

	// used for error checking
	HRESULT hr;

	// create device
	IDirectInputDevice8* lpDev;
	hr=g_lpDI->CreateDevice(lpddi->guidInstance,
							&lpDev,
							NULL);

	// check for error
	if(FAILED(hr))
		return(DIENUM_CONTINUE);
	else
		AddCOMResource;

	// set data format
	hr=lpDev->SetDataFormat(&c_dfDIJoystick);

	// check for error
	if(FAILED(hr))
		return(DIENUM_CONTINUE);

	// set cooperative level
	hr=lpDev->SetCooperativeLevel(g_hWnd,
								  DISCL_FOREGROUND|
								  DISCL_EXCLUSIVE);

	// check for error
	if(FAILED(hr))
		return(DIENUM_CONTINUE);

	// allocate or expand joystick data as needed
	if(g_dwNumJoystick==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpJoystick,
					 sizeof(IDirectInputDevice*)))
			return(DIENUM_STOP);
		if(!AllocMem((LPVOID*)&g_lplpJoystickName,
					 sizeof(LPSTR)))
			return(DIENUM_STOP);
		if(!AllocMem((LPVOID*)&g_lpguidJoystickInst,
					 sizeof(GUID)))
			return(DIENUM_STOP);
		if(!AllocMem((LPVOID*)&g_lpJoystickState,
					 sizeof(DIJOYSTATE)))
			return(DIENUM_STOP);
		if(!AllocMem((LPVOID*)&g_lpfJoystickXMid,
					 sizeof(float)))
			return(DIENUM_STOP);
		if(!AllocMem((LPVOID*)&g_lpfJoystickXRange,
					 sizeof(float)))
			return(DIENUM_STOP);
		if(!AllocMem((LPVOID*)&g_lpfJoystickYMid,
					 sizeof(float)))
			return(DIENUM_STOP);
		if(!AllocMem((LPVOID*)&g_lpfJoystickYRange,
					 sizeof(float)))
			return(DIENUM_STOP);
		if(!AllocMem((LPVOID*)&g_lpJoystickCaps,
					 sizeof(DIDEVCAPS)))
			return(DIENUM_STOP);
	}
	else
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpJoystick,
					  sizeof(IDirectInputDevice*)*g_dwNumJoystick,
					  sizeof(IDirectInputDevice*)))
			return(DIENUM_STOP);
		if(!ExpandMem((LPVOID*)&g_lplpJoystickName,
					  sizeof(LPSTR)*g_dwNumJoystick,
					  sizeof(LPSTR)))
			return(DIENUM_STOP);
		if(!ExpandMem((LPVOID*)&g_lpguidJoystickInst,
					  sizeof(GUID)*g_dwNumJoystick,
					  sizeof(GUID)))
			return(DIENUM_STOP);
		if(!ExpandMem((LPVOID*)&g_lpJoystickState,
					  sizeof(DIJOYSTATE)*g_dwNumJoystick,
					  sizeof(DIJOYSTATE)))
			return(DIENUM_STOP);
		if(!ExpandMem((LPVOID*)&g_lpfJoystickXMid,
					  sizeof(float)*g_dwNumJoystick,
					  sizeof(float)))
			return(DIENUM_STOP);
		if(!ExpandMem((LPVOID*)&g_lpfJoystickXRange,
					  sizeof(float)*g_dwNumJoystick,
					  sizeof(float)))
			return(DIENUM_STOP);
		if(!ExpandMem((LPVOID*)&g_lpfJoystickYMid,
					  sizeof(float)*g_dwNumJoystick,
					  sizeof(float)))
			return(DIENUM_STOP);
		if(!ExpandMem((LPVOID*)&g_lpfJoystickYRange,
					  sizeof(float)*g_dwNumJoystick,
					  sizeof(float)))
			return(DIENUM_STOP);
		if(!ExpandMem((LPVOID*)&g_lpJoystickCaps,
					  sizeof(DIDEVCAPS)*g_dwNumJoystick,
					  sizeof(DIDEVCAPS)))
			return(DIENUM_STOP);
	}
							
	// save index
	DWORD dwIndex=g_dwNumJoystick;

	// increment counter
	g_dwNumJoystick++;

	// copy interface
	g_lplpJoystick[dwIndex]=lpDev;

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpJoystickName[dwIndex],
				 strlen(lpddi->tszProductName)+1))
		return(DIENUM_STOP);

	// copy name
	strcpy(g_lplpJoystickName[dwIndex],
		   lpddi->tszProductName);

	// capitalize name
	CapitalizeString(g_lplpJoystickName[dwIndex]);

	// copy guid
	g_lpguidJoystickInst[dwIndex]=lpddi->guidInstance;
	
	// prepare range structure
	DIPROPRANGE pr;
	pr.diph.dwSize=sizeof(DIPROPRANGE);
	pr.diph.dwHeaderSize=sizeof(DIPROPHEADER);
	pr.diph.dwObj=DIDFT_ABSAXIS|DIDFT_MAKEINSTANCE(0);
	pr.diph.dwHow=DIPH_BYID;
	pr.lMin=0;
	pr.lMax=10000;

	// attempt to set axis range
	g_lplpJoystick[dwIndex]->SetProperty(DIPROP_RANGE,
										 &pr.diph);

	// get axis range
	hr=g_lplpJoystick[dwIndex]->GetProperty(DIPROP_RANGE,
											&pr.diph);

	// check for error
	if(FAILED(hr))
		return(DIENUM_CONTINUE);

	// save axis data
	g_lpfJoystickXRange[dwIndex]=1/(((float)(pr.lMax-pr.lMin))*0.5F);
	g_lpfJoystickXMid[dwIndex]=((float)pr.lMin)+(1/g_lpfJoystickXRange[dwIndex]);

	// prepare range structure for y-axis
	pr.diph.dwObj=DIDFT_ABSAXIS|DIDFT_MAKEINSTANCE(1);
	pr.lMin=0;
	pr.lMax=10000;

	// attempt to set axis range
	g_lplpJoystick[dwIndex]->SetProperty(DIPROP_RANGE,
										 &pr.diph);

	// get axis range
	hr=g_lplpJoystick[dwIndex]->GetProperty(DIPROP_RANGE,
											&pr.diph);

	// check for error
	if(FAILED(hr))
		return(DIENUM_CONTINUE);

	// save axis data
	g_lpfJoystickYRange[dwIndex]=1/(((float)(pr.lMax-pr.lMin))*0.5F);
	g_lpfJoystickYMid[dwIndex]=((float)pr.lMin)+(1/g_lpfJoystickYRange[dwIndex]);

	// set default dead zone and saturation
	SetJoystickDeadZone(dwIndex);
	SetJoystickSaturation(dwIndex);

	// initialize structure
	InitStruct(g_lpJoystickCaps[dwIndex]);
	g_lpJoystickCaps[dwIndex].dwSize=sizeof(g_lpJoystickCaps[dwIndex]);
	
	// get device caps
	hr=g_lplpJoystick[dwIndex]->GetCapabilities(&g_lpJoystickCaps[dwIndex]);

	// check for error
	if(FAILED(hr))
		return(DIENUM_CONTINUE);

	// acquire device
	if(!AcquireJoystick(dwIndex))
		return(DIENUM_CONTINUE);

	// ok
	return(DIENUM_CONTINUE);
}

// OpenControlPanel()

void OpenControlPanel(void)
{
	// minimize this application
	MinimizeApplicationWindow();

	// run control panel
	HRESULT hr=g_lpDI->RunControlPanel(g_hWnd,
									   NULL);

	// check for error
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to open Control Panel window.",
							  "You will need to open it manually.",
							  hr);
}

// OpenInputDeviceControlPanel()

void OpenInputDeviceControlPanel(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumJoystick&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid joystick index.",
								 "OpenInputDeviceControlPanel()",
								 FALSE);
		return;
	}
#endif

	// minimize this application
	MinimizeApplicationWindow();

	// get device interface
	IDirectInputDevice8* lpDev;
	if(dwIndex==NO_OBJECT)
		lpDev=g_lpKeyboard;
	else
		lpDev=g_lplpJoystick[dwIndex];

	// run control panel
	HRESULT hr=lpDev->RunControlPanel(g_hWnd,
									  NULL);

	// check for error
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to open Control Panel window.",
							  "You will need to open it manually.",
							  hr);
}

// AcquireKeyboard()

BOOL AcquireKeyboard(void)
{
	// acquire device
	HRESULT hr=g_lpKeyboard->Acquire();

	// check for error
	if(FAILED(hr))
		return(FALSE);
	
	// ok
	return(TRUE);
}

// UnacquireKeyboard()

BOOL UnacquireKeyboard(void)
{
	// check pointer
	if(!g_lpKeyboard)
		return(FALSE);
	
	// unacquire device
	HRESULT hr=g_lpKeyboard->Unacquire();

	// check for error
	if(FAILED(hr))
		return(FALSE);
	
	// ok
	return(TRUE);
}

// AcquireJoystick()

BOOL AcquireJoystick(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumJoystick)
	{
		DisplayFatalErrorMessage("Invalid joystick index.",
								 "AcquireJoystick()",
								 FALSE);
		return(FALSE);
	}
#endif
	
	// acquire device
	HRESULT hr=g_lplpJoystick[dwIndex]->Acquire();

	// check for error
	if(FAILED(hr))
		return(FALSE);
	
	// ok
	return(TRUE);
}

// UnacquireJoystick()

BOOL UnacquireJoystick(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumJoystick)
	{
		DisplayFatalErrorMessage("Invalid joystick index.",
								 "UnacquireJoystick()",
								 FALSE);
		return(FALSE);
	}
#endif
	
	// check pointer
	if(!g_lplpJoystick[dwIndex])
		return(FALSE);
	
	// unacquire device
	HRESULT hr=g_lplpJoystick[dwIndex]->Unacquire();

	// check for error
	if(FAILED(hr))
		return(FALSE);
	
	// ok
	return(TRUE);
}

// AcquireAllInputDevices()

void AcquireAllInputDevices(void)
{
	// counter
	DWORD dwCount;

	// acquire keyboard
	AcquireKeyboard();

	// acquire joysticks
	if(g_dwNumJoystick)
		for(dwCount=0;dwCount<g_dwNumJoystick;dwCount++)
			AcquireJoystick(dwCount);
}

// UnacquireAllInputDevices()

void UnacquireAllInputDevices(void)
{
	// counter
	DWORD dwCount;
	
	// unacquire keyboard
	UnacquireKeyboard();

	// unacquire joysticks
	if(g_dwNumJoystick)
		for(dwCount=0;dwCount<g_dwNumJoystick;dwCount++)
			UnacquireJoystick(dwCount);
}

// SetJoystickDeadZone()

BOOL SetJoystickDeadZone(DWORD dwIndex,
						 float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumJoystick)
	{
		DisplayFatalErrorMessage("Invalid joystick index.",
								 "SetJoystickDeadZone()",
								 FALSE);
		return(FALSE);
	}
#endif

	// prepare structure
	DIPROPDWORD pdw;
	InitStruct(pdw);
	pdw.diph.dwSize=sizeof(DIPROPDWORD);
	pdw.diph.dwHeaderSize=sizeof(DIPROPHEADER);
	pdw.diph.dwObj=0;
	pdw.diph.dwHow=DIPH_DEVICE;
	pdw.dwData=(DWORD)(fVal*10000);
	
	// set dead zone
	HRESULT hr=g_lplpJoystick[dwIndex]->SetProperty(DIPROP_DEADZONE,
													&pdw.diph);

	// check for error
	if(FAILED(hr))
		return(FALSE);

	// ok
	return(TRUE);
}

// SetJoystickSaturation()

BOOL SetJoystickSaturation(DWORD dwIndex,
						   float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumJoystick)
	{
		DisplayFatalErrorMessage("Invalid joystick index.",
								 "SetJoystickSaturation()",
								 FALSE);
		return(FALSE);
	}
#endif

	// prepare structure
	DIPROPDWORD pdw;
	InitStruct(pdw);
	pdw.diph.dwSize=sizeof(DIPROPDWORD);
	pdw.diph.dwHeaderSize=sizeof(DIPROPHEADER);
	pdw.diph.dwObj=0;
	pdw.diph.dwHow=DIPH_DEVICE;
	pdw.dwData=(DWORD)(fVal*10000);
	
	// set dead zone
	HRESULT hr=g_lplpJoystick[dwIndex]->SetProperty(DIPROP_SATURATION,
													&pdw.diph);

	// check for error
	if(FAILED(hr))
		return(FALSE);

	// ok
	return(TRUE);
}

// PurgeInputSettings()

void PurgeInputSettings(void)
{
	// purge input objects
	PurgeInputButtons();
	PurgeInputAxes();
	PurgeInputButtonTriggers();
	PurgeInputAxisTriggers();
}

// ReadKeyboard()

BOOL ReadKeyboard(void)
{
	// get keyboard state
	HRESULT hr=g_lpKeyboard->GetDeviceState(256,
											(LPVOID)g_lpcKeys);

	// check for error
	if(FAILED(hr))
	{
		// attempt to recover device
		if(AcquireKeyboard())
			hr=g_lpKeyboard->GetDeviceState(256,
											(LPVOID)g_lpcKeys);

		// check for error
		if(FAILED(hr))
		{
			// reset keyboard data
			ZeroMemory(g_lpcKeys,
					   sizeof(BYTE)*256);
			
			// data is not valid
			return(FALSE);
		}
	}

	// ok
	return(TRUE);
}

// ScanKeyboard()

BOOL ScanKeyboard(LPDWORD lpdwKeyID,
				  LPSTR lpKeyName)
{
	// read keyboard
	if(!ReadKeyboard())
		return(FALSE);

	// macro for checking values
	#define ScanKey(id,name) { if(g_lpcKeys[(id)]&0x80) { (*lpdwKeyID)=(DWORD)(id); strcpy(lpKeyName,(name)); return(TRUE); } }

	// scan each key
	ScanKey(DIK_0,"0");
	ScanKey(DIK_1,"1");
	ScanKey(DIK_2,"2");
	ScanKey(DIK_3,"3");
	ScanKey(DIK_4,"4");
	ScanKey(DIK_5,"5");
	ScanKey(DIK_6,"6");
	ScanKey(DIK_7,"7");
	ScanKey(DIK_8,"8");
	ScanKey(DIK_9,"9");
	ScanKey(DIK_A,"A");
	ScanKey(DIK_B,"B");
	ScanKey(DIK_C,"C");
	ScanKey(DIK_D,"D");
	ScanKey(DIK_E,"E");
	ScanKey(DIK_F,"F");
	ScanKey(DIK_G,"G");
	ScanKey(DIK_H,"H");
	ScanKey(DIK_I,"I");
	ScanKey(DIK_J,"J");
	ScanKey(DIK_K,"K");
	ScanKey(DIK_L,"L");
	ScanKey(DIK_M,"M");
	ScanKey(DIK_N,"N");
	ScanKey(DIK_O,"O");
	ScanKey(DIK_P,"P");
	ScanKey(DIK_Q,"Q");
	ScanKey(DIK_R,"R");
	ScanKey(DIK_S,"S");
	ScanKey(DIK_T,"T");
	ScanKey(DIK_U,"U");
	ScanKey(DIK_V,"V");
	ScanKey(DIK_W,"W");
	ScanKey(DIK_X,"X");
	ScanKey(DIK_Y,"Y");
	ScanKey(DIK_Z,"Z");
	ScanKey(DIK_ADD,"Keypad +");
	ScanKey(DIK_APOSTROPHE,"'");
	//ScanKey(DIK_APPS,"App Menu");
	ScanKey(DIK_BACK,"Backspace");
	ScanKey(DIK_BACKSLASH,"\\");
	//ScanKey(DIK_CAPITAL,"Caps Lock");
	ScanKey(DIK_COLON,":");
	ScanKey(DIK_COMMA,",");
	ScanKey(DIK_DECIMAL,"Keypad .");
	ScanKey(DIK_DELETE,"Delete");
	ScanKey(DIK_DIVIDE,"Keypad /");
	ScanKey(DIK_DOWN,"Down Arrow");
	ScanKey(DIK_END,"End");
	ScanKey(DIK_EQUALS,"=");
	ScanKey(DIK_ESCAPE,"Escape");
	ScanKey(DIK_F1,"F1");
	ScanKey(DIK_F2,"F2");
	ScanKey(DIK_F3,"F3");
	ScanKey(DIK_F4,"F4");
	ScanKey(DIK_F5,"F5");
	ScanKey(DIK_F6,"F6");
	ScanKey(DIK_F7,"F7");
	ScanKey(DIK_F8,"F8");
	ScanKey(DIK_F9,"F9");
	ScanKey(DIK_F10,"F10");
	//ScanKey(DIK_F11,"F11");
	//ScanKey(DIK_F12,"F12");
	ScanKey(DIK_GRAVE,"`");
	ScanKey(DIK_HOME,"Home");
	ScanKey(DIK_INSERT,"Insert");
	ScanKey(DIK_LBRACKET,"[");
	ScanKey(DIK_LCONTROL,"Left Ctrl");
	ScanKey(DIK_LEFT,"Left Arrow");
	//ScanKey(DIK_LMENU,"Left Alt");
	ScanKey(DIK_LSHIFT,"Left Shift");
	//ScanKey(DIK_LWIN,"Left Win");
	ScanKey(DIK_MINUS,"-");
	ScanKey(DIK_MULTIPLY,"Keypad *");
	ScanKey(DIK_NEXT,"Page Down");
	//ScanKey(DIK_NUMLOCK,"Num Lock");
	ScanKey(DIK_NUMPAD0,"Keypad 0");
	ScanKey(DIK_NUMPAD1,"Keypad 1");
	ScanKey(DIK_NUMPAD2,"Keypad 2");
	ScanKey(DIK_NUMPAD3,"Keypad 3");
	ScanKey(DIK_NUMPAD4,"Keypad 4");
	ScanKey(DIK_NUMPAD5,"Keypad 5");
	ScanKey(DIK_NUMPAD6,"Keypad 6");
	ScanKey(DIK_NUMPAD7,"Keypad 7");
	ScanKey(DIK_NUMPAD8,"Keypad 8");
	ScanKey(DIK_NUMPAD9,"Keypad 9");
	ScanKey(DIK_NUMPADENTER,"Keypad Enter");
	//ScanKey(DIK_PAUSE,"Pause");
	ScanKey(DIK_PERIOD,".");
	ScanKey(DIK_PRIOR,"Page Up");
	ScanKey(DIK_RBRACKET,"]");
	ScanKey(DIK_RCONTROL,"Right Ctrl");
	ScanKey(DIK_RETURN,"Enter");
	ScanKey(DIK_RIGHT,"Right Arrow");
	//ScanKey(DIK_RMENU,"Right Alt");
	ScanKey(DIK_RSHIFT,"Right Shift");
	//ScanKey(DIK_RWIN,"Right Win");
	//ScanKey(DIK_SCROLL,"Scroll Lock");
	ScanKey(DIK_SEMICOLON,";");
	ScanKey(DIK_SLASH,"/");
	ScanKey(DIK_SPACE,"Space Bar");
	ScanKey(DIK_SUBTRACT,"Keypad -");
	//ScanKey(DIK_SYSRQ,"System Request");
	ScanKey(DIK_TAB,"Tab");
	ScanKey(DIK_UP,"Up Arrow");

	// key not found
	return(FALSE);
}

// ReadJoystick()

BOOL ReadJoystick(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumJoystick)
	{
		DisplayFatalErrorMessage("Invalid joystick index.",
								 "ReadJoystick()",
								 FALSE);
		return(FALSE);
	}
#endif

	// used for error checking
	HRESULT hr;
	
	// poll joystick
	hr=g_lplpJoystick[dwIndex]->Poll();
	
	// get joystick state
	if(SUCCEEDED(hr))
		hr=g_lplpJoystick[dwIndex]->GetDeviceState(sizeof(DIJOYSTATE),
												   (LPVOID)&g_lpJoystickState[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		// attempt to recover device
		if(AcquireJoystick(dwIndex))
		{
			// poll joystick
			hr=g_lplpJoystick[dwIndex]->Poll();
			
			// get joystick state
			if(SUCCEEDED(hr))
				hr=g_lplpJoystick[dwIndex]->GetDeviceState(sizeof(DIJOYSTATE),
														   (LPVOID)&g_lpJoystickState[dwIndex]);
		}

		// check for error
		if(FAILED(hr))
		{
			// reset data
			ZeroMemory(&g_lpJoystickState[dwIndex],
					   sizeof(DIJOYSTATE));
			
			// data is not valid
			return(FALSE);
		}
	}

	// ok
	return(TRUE);
}

// ReadJoysticks()

BOOL ReadJoysticks(void)
{
	// counter
	DWORD dwCount;

	// reset return value
	BOOL bRet=TRUE;

	// read each joystick
	if(g_dwNumJoystick)
		for(dwCount=0;dwCount<g_dwNumJoystick;dwCount++)
			if(!ReadJoystick(dwCount))
				bRet=FALSE;;

	// ok
	return(bRet);
}

#ifdef DEBUG_MODE
// RenderUserInput()

void RenderUserInput(void)
{
	// reset line counter
	int nLine=32;
	
	LockGlobalMsg()

	// begin text
	BeginText(0);

	// render mouse info
	strcpy(g_lpMsg,"");
	if(g_bMouseBtnL) strcat(g_lpMsg,"<|"); else strcat(g_lpMsg,"<-");
	if(g_bMouseClkL) strcat(g_lpMsg,"|>"); else strcat(g_lpMsg,"->");
	if(g_bMouseBtnR) strcat(g_lpMsg,"<|"); else strcat(g_lpMsg,"<-");
	if(g_bMouseClkR) strcat(g_lpMsg,"|>"); else strcat(g_lpMsg,"->");
	if(g_bMouseBtnS) strcat(g_lpMsg,"<|"); else strcat(g_lpMsg,"<-");
	if(g_bMouseClkS) strcat(g_lpMsg,"|>"); else strcat(g_lpMsg,"->");
	if(g_bMouseBtnC) strcat(g_lpMsg,"<|"); else strcat(g_lpMsg,"<-");
	if(g_bMouseClkC) strcat(g_lpMsg,"|>"); else strcat(g_lpMsg,"->");
	OutputText(g_lpMsg,0,nLine); nLine+=16;
	sprintf(g_lpMsg,"(%d,%d)",g_nMouseX,g_nMouseY);
	OutputText(g_lpMsg,0,nLine);
	sprintf(g_lpMsg,"(%d,%d)",g_nMouseDX,g_nMouseDY);
	OutputText(g_lpMsg,72,nLine); nLine+=16;

	// render keyboard info
	DWORD dwID;
	char lpName[32];
	if(ScanKeyboard(&dwID,lpName)) sprintf(g_lpMsg,"Key: <%s>",lpName); else strcpy(g_lpMsg,"Key: <>");
	OutputText(g_lpMsg,0,nLine); nLine+=16;

	// render joystick info
	DWORD dwJoyIndex;
	DWORD dwBtnIndex;
	if(ScanJoysticks(&dwJoyIndex,&dwBtnIndex)) sprintf(g_lpMsg,"Joy: (%d) (%d)",dwJoyIndex,dwBtnIndex); else strcpy(g_lpMsg,"Joy: (-) (-)");
	OutputText(g_lpMsg,0,nLine); nLine+=16;
	DWORD dwCount;
	if(g_dwNumJoystick)
		for(dwCount=0;dwCount<g_dwNumJoystick;dwCount++)
		{
			// read joystick
			if(ReadJoystick(dwCount))
			{
				// copy name & position
				sprintf(g_lpMsg,
						"%s: (%f,%f) ",
						g_lplpJoystickName[dwCount],
						(((float)g_lpJoystickState[dwCount].lX)-g_lpfJoystickXMid[dwCount])*g_lpfJoystickXRange[dwCount],
						(((float)g_lpJoystickState[dwCount].lY)-g_lpfJoystickYMid[dwCount])*g_lpfJoystickYRange[dwCount]);

				// add buttons
				DWORD dwCount2;
				for(dwCount2=0;dwCount2<8;dwCount2++)
				{
					if(g_lpJoystickState[dwCount].rgbButtons[dwCount2]&0x80)
						strcat(g_lpMsg,"|");
					else
						strcat(g_lpMsg,"-");
				}
				
				// output text	
				OutputText(g_lpMsg,0,nLine); nLine+=16;
			}
		}

	// render input axes
	nLine+=16;
	if(g_dwNumAxis)
		for(dwCount=0;dwCount<g_dwNumAxis;dwCount++)
		{
			// add axis info
			sprintf(g_lpMsg,
					"Axis: (%f) ",
					GetAxisPos(dwCount));

			// add hit info
			if(IsAxisMinHit(dwCount)) strcat(g_lpMsg,"|"); else strcat(g_lpMsg,"-");
			if(IsAxisMaxHit(dwCount)) strcat(g_lpMsg,"|"); else strcat(g_lpMsg,"-");

			// output text	
			OutputText(g_lpMsg,0,nLine); nLine+=16;
		}

	// render input buttons
	if(g_dwNumButton)
	{
		// add button info
		strcpy(g_lpMsg,"Buttons: ");

		// add each button
		for(dwCount=0;dwCount<g_dwNumButton;dwCount++)
		{
			if(IsButtonDown(dwCount)) strcat(g_lpMsg,"<|"); else strcat(g_lpMsg,"<-");
			if(IsButtonHit(dwCount))  strcat(g_lpMsg,"|>"); else strcat(g_lpMsg,"->");
		}

		// output text	
		OutputText(g_lpMsg,0,nLine); nLine+=16;
	}

	// end text
	EndText();

	UnlockGlobalMsg()
}
#endif

// AddDefaultInputSettings()

void AddDefaultInputSettings(BOOL bNoJoystick)
{
	LoadingOutput("AddDefaultInputSettings");

	// counters
	DWORD dwCount1;
	DWORD dwCount2;
	
	// purge previous settings
	PurgeInputSettings();
	
	// add default axes
	g_dwDefXAxis=AddInputAxis();
	g_dwDefYAxis=AddInputAxis();

	// add default buttons
	g_dwDefCancelBtn=AddInputButton();
	g_dwDefSelectBtn=AddInputButton();
	g_dwDefHelpBtn=AddInputButton();

	// add keyboard triggers
	AddInputAxisTrigger(g_dwDefXAxis,DIK_LEFT,DIK_RIGHT);
	AddInputAxisTrigger(g_dwDefXAxis,DIK_NUMPAD4,DIK_NUMPAD6);
	AddInputAxisTrigger(g_dwDefYAxis,DIK_UP,DIK_DOWN);
	AddInputAxisTrigger(g_dwDefYAxis,DIK_NUMPAD8,DIK_NUMPAD2);
	AddInputButtonTrigger(g_dwDefCancelBtn,DIK_ESCAPE);
	AddInputButtonTrigger(g_dwDefSelectBtn,DIK_SPACE);
	AddInputButtonTrigger(g_dwDefSelectBtn,DIK_RETURN);
	AddInputButtonTrigger(g_dwDefSelectBtn,DIK_NUMPADENTER);
	AddInputButtonTrigger(g_dwDefHelpBtn,DIK_F1);

	// add joystick triggers
	if(!bNoJoystick)
		if(g_dwNumJoystick)
			for(dwCount1=0;dwCount1<g_dwNumJoystick;dwCount1++)
			{
				// add axis triggers
				AddInputAxisTrigger(g_dwDefXAxis,SetTriggerXAxis(dwCount1));
				AddInputAxisTrigger(g_dwDefYAxis,SetTriggerYAxis(dwCount1));

				// check number of buttons
				if(g_lpJoystickCaps[dwCount1].dwButtons>32)
					g_lpJoystickCaps[dwCount1].dwButtons=32;
				
				// add button triggers
				for(dwCount2=0;dwCount2<g_lpJoystickCaps[dwCount1].dwButtons;dwCount2++)
					AddInputButtonTrigger(g_dwDefSelectBtn,SetTriggerButton(dwCount1,dwCount2));
			}
}

// GetNextInputButton()

DWORD GetNextInputButton(void)
{
	LoadingOutput("GetNextInputButton");

	// check counter
	if(g_dwNumButton==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpbButtonDown,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbButtonDownP,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbButtonHit,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxButton=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumButton>=g_dwMaxButton)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpbButtonDown,
					  sizeof(BOOL)*g_dwMaxButton,
					  sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbButtonDownP,
					  sizeof(BOOL)*g_dwMaxButton,
					  sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbButtonHit,
					  sizeof(BOOL)*g_dwMaxButton,
					  sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxButton+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumButton;

	// increment counter
	g_dwNumButton++;

	// return index
	return(dwIndex);
}

// AddInputButton()

DWORD AddInputButton(void)
{
	LoadingOutput("AddInputButton");

	// get return index
	DWORD dwIndex=GetNextInputButton();

	// reset flags
	g_lpbButtonDown[dwIndex]=FALSE;
	g_lpbButtonDownP[dwIndex]=FALSE;
	g_lpbButtonHit[dwIndex]=FALSE;
	
	// return index
	return(dwIndex);
}

// PurgeInputButtons()

void PurgeInputButtons(void)
{
	// free memory objects
	FreeMem((LPVOID*)&g_lpbButtonDown);
	FreeMem((LPVOID*)&g_lpbButtonDownP);
	FreeMem((LPVOID*)&g_lpbButtonHit);

	// reset counters
	g_dwNumButton=0;
	g_dwMaxButton=0;
}

// GetNextInputAxis()

DWORD GetNextInputAxis(void)
{
	LoadingOutput("GetNextInputAxis");

	// check counter
	if(g_dwNumAxis==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpfAxisPos,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbAxisMinDownP,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbAxisMinHit,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbAxisMaxDownP,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbAxisMaxHit,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAxis=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumAxis>=g_dwMaxAxis)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpfAxisPos,
					  sizeof(float)*g_dwMaxAxis,
					  sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbAxisMinDownP,
					  sizeof(BOOL)*g_dwMaxAxis,
					  sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbAxisMinHit,
					  sizeof(BOOL)*g_dwMaxAxis,
					  sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbAxisMaxDownP,
					  sizeof(BOOL)*g_dwMaxAxis,
					  sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbAxisMaxHit,
					  sizeof(BOOL)*g_dwMaxAxis,
					  sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAxis+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumAxis;

	// increment counter
	g_dwNumAxis++;

	// return index
	return(dwIndex);
}

// AddInputAxis()

DWORD AddInputAxis(void)
{
	LoadingOutput("AddInputAxis");

	// get return index
	DWORD dwIndex=GetNextInputAxis();

	// reset position
	g_lpfAxisPos[dwIndex]=0;
	
	// reset flags
	g_lpbAxisMinDownP[dwIndex]=FALSE;
	g_lpbAxisMinHit[dwIndex]=FALSE;
	g_lpbAxisMaxDownP[dwIndex]=FALSE;
	g_lpbAxisMaxHit[dwIndex]=FALSE;
	
	// return index
	return(dwIndex);
}

// PurgeInputAxes()

void PurgeInputAxes(void)
{
	// free memory objects
	FreeMem((LPVOID*)&g_lpfAxisPos);
	FreeMem((LPVOID*)&g_lpbAxisMinDownP);
	FreeMem((LPVOID*)&g_lpbAxisMinHit);
	FreeMem((LPVOID*)&g_lpbAxisMaxDownP);
	FreeMem((LPVOID*)&g_lpbAxisMaxHit);

	// reset counters
	g_dwNumAxis=0;
	g_dwMaxAxis=0;
}

// GetNextInputButtonTrigger()

DWORD GetNextInputButtonTrigger(void)
{
	LoadingOutput("GetNextInputButtonTrigger");

	// check counter
	if(g_dwNumButtonTrig==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwButtonTarget,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwButtonTrig,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxButtonTrig=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumButtonTrig>=g_dwMaxButtonTrig)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwButtonTarget,
					  sizeof(DWORD)*g_dwMaxButtonTrig,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwButtonTrig,
					  sizeof(DWORD)*g_dwMaxButtonTrig,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxButtonTrig+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumButtonTrig;

	// increment counter
	g_dwNumButtonTrig++;

	// return index
	return(dwIndex);
}

// AddInputButtonTrigger()

DWORD AddInputButtonTrigger(DWORD dwBtnIndex,
							DWORD dwTrigger)
{
	LoadingOutput("AddInputButtonTrigger");

#ifdef DEBUG_MODE
	// verify index
	if(dwBtnIndex>=g_dwNumButton)
	{
		DisplayFatalErrorMessage("Invalid button index.",
								 "AddInputButtonTrigger()",
								 FALSE);
		return(FALSE);
	}
#endif

	// get return index
	DWORD dwIndex=GetNextInputButtonTrigger();

	// set values
	g_lpdwButtonTarget[dwIndex]=dwBtnIndex;
	g_lpdwButtonTrig[dwIndex]=dwTrigger;
	
	// return index
	return(dwIndex);
}

// EditInputButtonTrigger()

void EditInputButtonTrigger(DWORD dwTrigIndex,
							DWORD dwTrigger)
{
	LoadingOutput("EditInputButtonTrigger");

#ifdef DEBUG_MODE
	// verify index
	if(dwTrigIndex>=g_dwNumButtonTrig)
	{
		DisplayFatalErrorMessage("Invalid button trigger index.",
								 "EditInputButtonTrigger()",
								 FALSE);
		return;
	}
#endif

	// set value
	g_lpdwButtonTrig[dwTrigIndex]=dwTrigger;
}

// PurgeInputButtonTriggers()

void PurgeInputButtonTriggers(void)
{
	// free memory objects
	FreeMem((LPVOID*)&g_lpdwButtonTarget);
	FreeMem((LPVOID*)&g_lpdwButtonTrig);

	// reset counters
	g_dwNumButtonTrig=0;
	g_dwMaxButtonTrig=0;
}

// GetNextInputAxisTrigger()

DWORD GetNextInputAxisTrigger(void)
{
	LoadingOutput("GetNextInputAxisTrigger");

	// check counter
	if(g_dwNumAxisTrig==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwAxisTarget,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwAxisTrig1,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwAxisTrig2,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAxisTrig=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumAxisTrig>=g_dwMaxAxisTrig)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwAxisTarget,
					  sizeof(DWORD)*g_dwMaxAxisTrig,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwAxisTrig1,
					  sizeof(DWORD)*g_dwMaxAxisTrig,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwAxisTrig2,
					  sizeof(DWORD)*g_dwMaxAxisTrig,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAxisTrig+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumAxisTrig;

	// increment counter
	g_dwNumAxisTrig++;

	// return index
	return(dwIndex);
}

// AddInputAxisTrigger()

DWORD AddInputAxisTrigger(DWORD dwAxisIndex,
						  DWORD dwTrigger1,
						  DWORD dwTrigger2)
{
	LoadingOutput("AddInputAxisTrigger");

#ifdef DEBUG_MODE
	// verify index
	if(dwAxisIndex>=g_dwNumAxis)
	{
		DisplayFatalErrorMessage("Invalid axis index.",
								 "AddInputAxisTrigger()",
								 FALSE);
		return(FALSE);
	}
#endif

	// get return index
	DWORD dwIndex=GetNextInputAxisTrigger();

	// set values
	g_lpdwAxisTarget[dwIndex]=dwAxisIndex;
	g_lpdwAxisTrig1[dwIndex]=dwTrigger1;
	g_lpdwAxisTrig2[dwIndex]=dwTrigger2;
	
	// return index
	return(dwIndex);
}

// EditInputAxisTrigger()

void EditInputAxisTrigger(DWORD dwTrigIndex,
						  DWORD dwTrigger1,
						  DWORD dwTrigger2)
{
	LoadingOutput("EditInputAxisTrigger");

#ifdef DEBUG_MODE
	// verify index
	if(dwTrigIndex>=g_dwNumAxisTrig)
	{
		DisplayFatalErrorMessage("Invalid axis trigger index.",
								 "EditInputAxisTrigger()",
								 FALSE);
		return;
	}
#endif

	// set values
	g_lpdwAxisTrig1[dwTrigIndex]=dwTrigger1;
	g_lpdwAxisTrig2[dwTrigIndex]=dwTrigger2;
}

// PurgeInputAxisTriggers()

void PurgeInputAxisTriggers(void)
{
	// free memory objects
	FreeMem((LPVOID*)&g_lpdwAxisTarget);
	FreeMem((LPVOID*)&g_lpdwAxisTrig1);
	FreeMem((LPVOID*)&g_lpdwAxisTrig2);

	// reset counters
	g_dwNumAxisTrig=0;
	g_dwMaxAxisTrig=0;
}

// ScanJoystick()

BOOL ScanJoystick(DWORD dwJoyIndex,
				  LPDWORD lpdwBtnIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwJoyIndex>=g_dwNumJoystick)
	{
		DisplayFatalErrorMessage("Invalid joystick index.",
								 "ScanJoystick()",
								 FALSE);
		return(FALSE);
	}
#endif

	// counter
	DWORD dwCount;

	// read joystick
	ReadJoystick(dwJoyIndex);
	
	// check each button
	for(dwCount=0;dwCount<32;dwCount++)
		if(g_lpJoystickState[dwJoyIndex].rgbButtons[dwCount]&0x80)
		{
			// copy button index
			(*lpdwBtnIndex)=dwCount;

			// ok
			return(TRUE);
		}

	// no buttons found
	return(FALSE);
}

// ScanJoysticks()

BOOL ScanJoysticks(LPDWORD lpdwJoyIndex,
				   LPDWORD lpdwBtnIndex)
{
	// check number of joysticks
	if(g_dwNumJoystick==0)
		return(FALSE);

	// reset counter
	DWORD dwCount=0;

	// scan each joystick
	while(dwCount<g_dwNumJoystick)
	{
		// scan joystick
		if(ScanJoystick(dwCount,
						lpdwBtnIndex))
		{
			// set index
			(*lpdwJoyIndex)=dwCount;

			// ok
			return(TRUE);
		}

		// increment counter
		dwCount++;
	}

	// no buttons found
	return(FALSE);
}

// FindJoystick()

DWORD FindJoystick(GUID guid)
{
	LoadingOutput("FindJoystick");

	// counter
	DWORD dwCount;

	// search joysticks
	if(g_dwNumJoystick)
		for(dwCount=0;dwCount<g_dwNumJoystick;dwCount++)
			if(guid==g_lpguidJoystickInst[dwCount])
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// ResetUserInput()

void ResetUserInput(void)
{
	// reset each button
	if(g_dwNumButton)
	{
		ZeroMemory(g_lpbButtonDown,
				   sizeof(BOOL)*g_dwNumButton);
		ZeroMemory(g_lpbButtonHit,
				   sizeof(BOOL)*g_dwNumButton);
	}

	// reset each axis
	if(g_dwNumAxis)
	{
		ZeroMemory(g_lpfAxisPos,
				   sizeof(float)*g_dwNumAxis);
		ZeroMemory(g_lpbAxisMinHit,
				   sizeof(BOOL)*g_dwNumAxis);
		ZeroMemory(g_lpbAxisMaxHit,
				   sizeof(BOOL)*g_dwNumAxis);
	}
}

// ReadUserInput()

BOOL ReadUserInput(void)
{
	// counter
	DWORD dwCount;
	
	// reset user input
	ResetUserInput();
	
	// read keyboard
	ReadKeyboard();

	// read joysticks
	ReadJoysticks();
	
	// check axis triggers
	if(g_dwNumAxisTrig)
		for(dwCount=0;dwCount<g_dwNumAxisTrig;dwCount++)
		{
			// get target
			DWORD dwTarget=g_lpdwAxisTarget[dwCount];
			
			// get triggers
			DWORD dwTrigger1=g_lpdwAxisTrig1[dwCount];
			DWORD dwTrigger2=g_lpdwAxisTrig2[dwCount];

			// check for joystick trigger
			if(IsTriggerJoystick(dwTrigger1))
			{
				// get joystick number
				DWORD dwJoystick=GetTriggerJoystick(dwTrigger1);

				// check trigger type
				if(IsTriggerXAxis(dwTrigger1))
					g_lpfAxisPos[dwTarget]+=(((float)g_lpJoystickState[dwJoystick].lX)-g_lpfJoystickXMid[dwJoystick])*g_lpfJoystickXRange[dwJoystick];
				else if(IsTriggerYAxis(dwTrigger1))
					g_lpfAxisPos[dwTarget]+=(((float)g_lpJoystickState[dwJoystick].lY)-g_lpfJoystickYMid[dwJoystick])*g_lpfJoystickYRange[dwJoystick];
				else
				{
					// get button numbers
					DWORD dwButton1=GetTriggerButton(dwTrigger1);
					DWORD dwButton2=GetTriggerButton(dwTrigger2);

					// check button 1
					if(g_lpJoystickState[dwJoystick].rgbButtons[dwButton1]&0x80)
						g_lpfAxisPos[dwTarget]-=1;

					// check button 2
					if(g_lpJoystickState[dwJoystick].rgbButtons[dwButton2]&0x80)
						g_lpfAxisPos[dwTarget]+=1;
				}
			}
			else
			{
				// check key 1
				if(g_lpcKeys[dwTrigger1]&0x80)
					g_lpfAxisPos[dwTarget]-=1;

				// check key 2
				if(g_lpcKeys[dwTrigger2]&0x80)
					g_lpfAxisPos[dwTarget]+=1;
			}
		}
		
	// complete processing on axes
	if(g_dwNumAxis)
		for(dwCount=0;dwCount<g_dwNumAxis;dwCount++)
		{
			// check for overflow
			if(g_lpfAxisPos[dwCount]<-1)
				g_lpfAxisPos[dwCount]=-1;
			if(g_lpfAxisPos[dwCount]>1)
				g_lpfAxisPos[dwCount]=1;
			
			// set axis hit flags
			if(!g_lpbAxisMinDownP[dwCount]&&
			   IsAxisMinDown(dwCount))
				g_lpbAxisMinHit[dwCount]=TRUE;
			if(!g_lpbAxisMaxDownP[dwCount]&&
			   IsAxisMaxDown(dwCount))
				g_lpbAxisMaxHit[dwCount]=TRUE;
			
			// set previous axis flags
			g_lpbAxisMinDownP[dwCount]=IsAxisMinDown(dwCount);
			g_lpbAxisMaxDownP[dwCount]=IsAxisMaxDown(dwCount);
		}

	// check button triggers
	if(g_dwNumButtonTrig)
		for(dwCount=0;dwCount<g_dwNumButtonTrig;dwCount++)
		{
			// get target
			DWORD dwTarget=g_lpdwButtonTarget[dwCount];
			
			// get trigger
			DWORD dwTrigger=g_lpdwButtonTrig[dwCount];

			// check for joystick trigger
			if(IsTriggerJoystick(dwTrigger))
			{
				// get joystick number
				DWORD dwJoystick=GetTriggerJoystick(dwTrigger);

				// check trigger type
				if(IsTriggerButton(dwTrigger))
				{
					// get button number
					DWORD dwButton=GetTriggerButton(dwTrigger);

					// check button
					if(g_lpJoystickState[dwJoystick].rgbButtons[dwButton]&0x80)
						g_lpbButtonDown[dwTarget]=TRUE;
				}
			}
			else
			{
				// check key
				if(g_lpcKeys[dwTrigger]&0x80)
					g_lpbButtonDown[dwTarget]=TRUE;
			}
		}
	
	// complete processing on buttons
	if(g_dwNumButton)
		for(dwCount=0;dwCount<g_dwNumButton;dwCount++)
		{
			// set button hit flag
			if(!g_lpbButtonDownP[dwCount]&&
			   g_lpbButtonDown[dwCount])
				g_lpbButtonHit[dwCount]=TRUE;
			
			// set previous button flag
			g_lpbButtonDownP[dwCount]=g_lpbButtonDown[dwCount];
		}

	// ok
	return(TRUE);
}

#ifdef DEBUG_MODE
// DisplayJoystickDeviceSettings()

void DisplayJoystickDeviceSettings(DWORD dwIndex)
{
	// verify index
	if(dwIndex>=g_dwNumJoystick)
	{
		DisplayFatalErrorMessage("Invalid joystick index.",
								 "DisplayJoystickDeviceSettings()",
								 FALSE);
		return;
	}

	// buffer for output
	LPSTR lpOutput;
	
	// allocate buffer for data
	if(!AllocMem((LPVOID*)&lpOutput,
				 1024))
		return;

	LockGlobalMsg()

	// output data
	strcpy(lpOutput,"");
	sprintf(g_lpMsg,"JoystickIndex=%d\n",dwIndex); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"Alias="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_ALIAS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"Attached="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_ATTACHED) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DeadBand="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_DEADBAND) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"Emulated="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_EMULATED) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ForceFeedback="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_FORCEFEEDBACK) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"FFFade="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_FFFADE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"FFAttack="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_FFATTACK) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"Hidden="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_HIDDEN) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"Phantom="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_PHANTOM) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"PolledDataFormat="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_POLLEDDATAFORMAT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"PosNegCoefficients="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_POSNEGCOEFFICIENTS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"PosNegSaturation="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_POSNEGSATURATION) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"Saturation="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_SATURATION) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"StartDelay="); strcat(lpOutput,g_lpMsg);
		if(g_lpJoystickCaps[dwIndex].dwFlags&DIDC_STARTDELAY) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DevType=%d\n",g_lpJoystickCaps[dwIndex].dwDevType); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"Axes=%d\n",g_lpJoystickCaps[dwIndex].dwAxes); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"Buttons=%d\n",g_lpJoystickCaps[dwIndex].dwButtons); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"POVs=%d\n",g_lpJoystickCaps[dwIndex].dwPOVs); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"FFSamplePeriod=%d\n",g_lpJoystickCaps[dwIndex].dwFFSamplePeriod); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"FFMinTimeResolution=%d\n",g_lpJoystickCaps[dwIndex].dwFFMinTimeResolution); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"FirmwareRevision=%d\n",g_lpJoystickCaps[dwIndex].dwFirmwareRevision); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"HardwareRevision=%d\n",g_lpJoystickCaps[dwIndex].dwHardwareRevision); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"FFDriverVersion=%d\n",g_lpJoystickCaps[dwIndex].dwFFDriverVersion); strcat(lpOutput,g_lpMsg);

	UnlockGlobalMsg()

	// display message
	DisplayInfoMessage(lpOutput);
	
	// free output buffer
	FreeMem((LPVOID*)&lpOutput);
}
#endif

// eof //

// Copyright 2005-2007 WaveBreak Software
