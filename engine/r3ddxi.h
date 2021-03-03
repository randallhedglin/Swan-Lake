// Copyright 2005-2007 WaveBreak Software

/////////////////////////////////////////////////////////////////////
// perform step-by-step walkthrough with COMPLETE error checking!! //
/////////////////////////////////////////////////////////////////////

// r3ddxi.h -- DirectInput wrapper header

// globals //

// directinput interface
extern IDirectInput8* g_lpDI;
// input devices
extern IDirectInputDevice8* g_lpKeyboard;
extern IDirectInputDevice8** g_lplpJoystick;
// joystick counter
extern DWORD g_dwNumJoystick;
extern LPSTR* g_lplpJoystickName;
extern LPGUID g_lpguidJoystickInst;
extern LPDIJOYSTATE g_lpJoystickState;
extern float* g_lplJoystickXMid;
extern float* g_lplJoystickXRange;
extern float* g_lplJoystickYMid;
extern float* g_lplJoystickYRange;
extern LPDIDEVCAPS g_lpJoystickCaps;
// button counters
extern DWORD g_dwNumButton;
extern DWORD g_dwMaxButton;
// button data
extern LPBOOL g_lpbButtonDown;
extern LPBOOL g_lpbButtonDownP;
extern LPBOOL g_lpbButtonHit;
// axis counters
extern DWORD g_dwNumAxis;
extern DWORD g_dwMaxAxis;
// axis data
extern float* g_lpfAxisPos;
extern LPBOOL g_lpbAxisMinDownP;
extern LPBOOL g_lpbAxisMinHit;
extern LPBOOL g_lpbAxisMaxDownP;
extern LPBOOL g_lpbAxisMaxHit;
// button trigger counters
extern DWORD g_dwNumButtonTrig;
extern DWORD g_dwMaxButtonTrig;
// button trigger data
extern LPDWORD g_lpdwButtonTarget;
extern LPDWORD g_lpdwButtonTrig;
// axis trigger counters
extern DWORD g_dwNumAxisTrig;
extern DWORD g_dwMaxAxisTrig;
// axis trigger data
extern LPDWORD g_lpdwAxisTarget;
extern LPDWORD g_lpdwAxisTrig1;
extern LPDWORD g_lpdwAxisTrig2;
// keyboard data
extern BYTE g_lpcKeys[];
// default input settings
extern DWORD g_dwDefXAxis;
extern DWORD g_dwDefYAxis;
extern DWORD g_dwDefCancelBtn;
extern DWORD g_dwDefSelectBtn;
extern DWORD g_dwDefHelpBtn;

// prototypes //

// object handling functions

extern BOOL DXInputInit(void);

extern void DXInputRelease(void);

extern BOOL InitDIInterface(void);

extern void ReleaseDIInterface(void);

extern BOOL InitKeyboard(void);

extern void ReleaseKeyboard(void);

extern BOOL InitJoysticks(void);

extern void ReleaseJoysticks(void);

extern BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE lpddi,
										   LPVOID lpRefData);

extern void OpenControlPanel(void);

extern void OpenInputDeviceControlPanel(DWORD dwIndex=NO_OBJECT);

extern BOOL AcquireKeyboard(void);

extern BOOL UnacquireKeyboard(void);

extern BOOL AcquireJoystick(DWORD dwIndex);

extern BOOL UnacquireJoystick(DWORD dwIndex);

extern void AcquireAllInputDevices(void);

extern void UnacquireAllInputDevices(void);

extern BOOL SetJoystickDeadZone(DWORD dwIndex,
								float fVal=0.2F);

extern BOOL SetJoystickSaturation(DWORD dwIndex,
								  float fVal=0.8F);

extern BOOL ReadKeyboard(void);

extern BOOL ScanKeyboard(LPDWORD lpdwKeyID,
						 LPSTR lpKeyName);

extern BOOL ReadJoystick(DWORD dwIndex);

extern BOOL ReadJoysticks(void);

extern BOOL ScanJoystick(DWORD dwJoyIndex,
						 LPDWORD lpdwBtnIndex);

extern BOOL ScanJoysticks(LPDWORD lpdwJoyIndex,
						  LPDWORD lpdwBtnIndex);

extern DWORD FindJoystick(GUID guid);

// user input functions

extern void AddDefaultInputSettings(BOOL bNoJoystick=FALSE);

extern void PurgeInputSettings(void);

extern DWORD GetNextInputButton(void);

extern DWORD AddInputButton(void);

extern void PurgeInputButtons(void);

extern DWORD GetNextInputAxis(void);

extern DWORD AddInputAxis(void);

extern void PurgeInputAxes(void);

extern DWORD GetNextInputButtonTrigger(void);

extern DWORD AddInputButtonTrigger(DWORD dwBtnIndex,
								   DWORD dwTrigger);

extern void EditInputButtonTrigger(DWORD dwTrigIndex,
								   DWORD dwTrigger);

extern void PurgeInputButtonTriggers(void);

extern DWORD GetNextInputAxisTrigger(void);

extern DWORD AddInputAxisTrigger(DWORD dwAxisIndex,
								 DWORD dwTrigger1,
								 DWORD dwTrigger2=NULL);

extern void EditInputAxisTrigger(DWORD dwTrigIndex,
								 DWORD dwTrigger1,
								 DWORD dwTrigger2=NULL);

extern void PurgeInputAxisTriggers(void);

extern BOOL ReadUserInput(void);

extern void ResetUserInput(void);

#ifdef DEBUG_MODE
extern void RenderUserInput(void);
#endif

// misc functions //

#ifdef DEBUG_MODE
extern void DisplayJoystickDeviceSettings(DWORD dwIndex);
#endif

// macros //

// joystick trigger macros
#define SetTriggerXAxis(j)    (0x80000000|(((DWORD)(j))<<16)                   )
#define SetTriggerYAxis(j)    (0x40000000|(((DWORD)(j))<<16)                   )
#define SetTriggerButton(j,b) (0x20000000|(((DWORD)(j))<<16)|(((DWORD)(b))<<24))
#define IsTriggerJoystick(t)  (((DWORD)(t))&0xE0000000)
#define IsTriggerXAxis(t)     (((DWORD)(t))&0x80000000)
#define IsTriggerYAxis(t)     (((DWORD)(t))&0x40000000)
#define IsTriggerButton(t)    (((DWORD)(t))&0x20000000)
#define GetTriggerJoystick(t) ((((DWORD)(t))>>16)&0x000000FF)
#define GetTriggerButton(t)   ((((DWORD)(t))>>24)&0x0000001F)

// user input macros
#define IsButtonDown(n)  (g_lpbButtonDown[n])
#define IsButtonHit(n)   (g_lpbButtonHit[n])
#define GetAxisPos(n)    (g_lpfAxisPos[n])
#define IsAxisMinDown(n) (g_lpfAxisPos[n]<-0.5F)
#define IsAxisMinHit(n)  (g_lpbAxisMinHit[n])
#define IsAxisMaxDown(n) (g_lpfAxisPos[n]>0.5F)
#define IsAxisMaxHit(n)  (g_lpbAxisMaxHit[n])

// eof //

// Copyright 2005-2007 WaveBreak Software
