// Copyright 2005-2011 WaveBreak Software

// swanlake.h -- swan lake screensaver header

// structures //

typedef struct tagSOUNDPOSDATA
{
	// segment index
	DWORD dwSFXIndex;
	// audiopath id
	DWORD dwAudioPathID;
	// repeat value	
	DWORD dwRepeatVal;
	// priority level
	DWORD dwSFXPriVal;
	// max volume
	float fMaxVolVal;
	// distance values
	float fMinDist;
	float fMaxDist;
	// number of targets
	DWORD dwNumTarget;
	// pointer to target matrices
	LPDWORD lpdwTargetMtxArray;
	// target matrix
	DWORD dwTargetMtxIndex;
	// playback flag
	BOOL bPlayback;
	// optimization
	DWORD dwDummy[5];
} SOUNDPOSDATA,*LPSOUNDPOSDATA;

// globals //

// water surface data
extern DWORD g_dwWaterVtxBuf;
extern DWORD g_dwWaterIdxBuf;
extern DWORD g_dwWaterMat;
extern DWORD g_dwWaterTex;
// terrain mesh data
extern DWORD g_dwTerrainMesh;
extern DWORD g_dwTerrainMtx;
// sky mesh data
extern DWORD g_dwSkyMesh;
extern DWORD g_dwSkyMtx;
#ifdef DEBUG_MODE
// top-view data
extern DWORD g_dwTopViewMtx;
extern DWORD g_dwTopViewBtn;
#endif
// initial updates flag
extern BOOL g_bInitUpdates;
// creature animations & meshes
extern DWORD g_dwSwanAnim;
extern DWORD g_dwBBSwanAnim;
extern DWORD g_dwTurtleAnim;
extern DWORD g_dwDragonflyMesh;
extern DWORD g_dwDragonflyWingAnim;
extern DWORD g_dwFishTex;
extern DWORD g_dwBirdAnim;
// swan counters
extern DWORD g_dwNumSwan;
extern DWORD g_dwMaxSwan;
// swan data
extern LPDWORD g_lpdwSwanMtx;
extern LPDWORD g_lpdwSwanPriState;
extern LPDWORD g_lpdwSwanActState;
extern LPDWORD g_lpdwSwanNavState;
extern LPDWORD g_lpdwSwanKeyframe1;
extern LPDWORD g_lpdwSwanKeyframe2;
extern float* g_lpfSwanTweenVal;
extern float* g_lpfSwanTailPos;
extern float* g_lpfSwanTailDir;
extern float* g_lpfSwanTransPos;
extern float* g_lpfSwanTransSpeed;
extern LPDWORD g_lpdwSwanTransKey1;
extern LPDWORD g_lpdwSwanTransKey2;
extern float* g_lpfSwanNavSpeed;
extern LPDWORD g_lpdwSwanNavPos;
extern LPDWORD g_lpdwSwanNavMax;
extern LPDWORD g_lpdwSwanMateIdx;
// baby swan counters
extern DWORD g_dwNumBBSwan;
extern DWORD g_dwMaxBBSwan;
// baby swan data
extern LPDWORD g_lpdwBBSwanMtx;
extern LPDWORD g_lpdwBBSwanPriState;
extern LPDWORD g_lpdwBBSwanActState;
extern LPDWORD g_lpdwBBSwanNavState;
extern LPDWORD g_lpdwBBSwanKeyframe1;
extern LPDWORD g_lpdwBBSwanKeyframe2;
extern float* g_lpfBBSwanTweenVal;
extern float* g_lpfBBSwanTailPos;
extern float* g_lpfBBSwanTailDir;
extern float* g_lpfBBSwanTransPos;
extern float* g_lpfBBSwanTransSpeed;
extern LPDWORD g_lpdwBBSwanTransKey1;
extern LPDWORD g_lpdwBBSwanTransKey2;
extern float* g_lpfBBSwanNavSpeed;
extern LPDWORD g_lpdwBBSwanNavPos;
extern LPDWORD g_lpdwBBSwanNavMax;
extern LPDWORD g_lpdwBBSwanParent1Idx;
extern LPDWORD g_lpdwBBSwanParent2Idx;
// turtle counters
extern DWORD g_dwNumTurtle;
extern DWORD g_dwMaxTurtle;
// turtle data
extern LPDWORD g_lpdwTurtleMtx;
extern LPDWORD g_lpdwTurtlePriState;
extern LPDWORD g_lpdwTurtleActState;
extern LPDWORD g_lpdwTurtleNavState;
extern LPDWORD g_lpdwTurtleKeyframe1;
extern LPDWORD g_lpdwTurtleKeyframe2;
extern float* g_lpfTurtleTweenVal;
extern float* g_lpfTurtleTransPos;
extern float* g_lpfTurtleTransSpeed;
extern float* g_lpfTurtleNavSpeed;
extern LPDWORD g_lpdwTurtleNavPos;
extern LPDWORD g_lpdwTurtleNavMax;
// dragonfly counters
extern DWORD g_dwNumDFly;
extern DWORD g_dwMaxDFly;
// dragonfly data
extern LPDWORD g_lpdwDFlyMtx;
extern LPDWORD g_lpdwDFlyPriState;
extern LPDWORD g_lpdwDFlyNavState;
extern float* g_lpfDFlyNavSpeed;
extern LPDWORD g_lpdwDFlyNavPos;
extern LPDWORD g_lpdwDFlyNavMax;
extern LPD3DXVECTOR3 g_lpvDFlyDartVec;
extern LPDWORD g_lpdwDFlyMateIdx;
extern LPDWORD g_lpdwDFlyAPathID;
extern LPDWORD g_lpdwDFlyBuzzIdx;
extern LPBOOL g_lpbDFlyPlayback;
// fish counters
extern DWORD g_dwNumFish;
extern DWORD g_dwMaxFish;
// fish data
extern LPD3DXVECTOR3 g_lpvFishPos;
extern float* g_lpfFishScale;
extern LPDWORD g_lpdwFishPriState;
extern LPDWORD g_lpdwFishAnimFrame;
extern LPDWORD g_lpdwFishAPathID;
extern LPDWORD g_lpdwFishSplashIdx;
extern LPBOOL g_lpbFishPlayback;
// bird counters
extern DWORD g_dwNumBird;
extern DWORD g_dwMaxBird;
// bird data
extern LPDWORD g_lpdwBirdMtx;
extern float* g_lpfBirdWingPos;
extern float* g_lpfBirdWingDir;
extern LPDWORD g_lpdwBirdNavState;
extern float* g_lpfBirdNavSpeed;
extern LPDWORD g_lpdwBirdNavPos;
extern LPDWORD g_lpdwBirdNavMax;
extern LPDWORD g_lpdwBirdKeyframe1;
extern LPDWORD g_lpdwBirdKeyframe2;
extern float* g_lpfBirdTweenVal;
// lens flare data
extern LENSFLAREDATA g_lfd;
// ripple rendering data
extern DWORD g_dwRippleVtxBuf;
extern DWORD g_dwRippleIdxBuf;
extern DWORD g_dwRippleMat;
extern DWORD g_lpdwRippleTex[4];
extern DWORD g_dwRippleMtx;
// ripple counters
extern DWORD g_dwNumRipple;
extern DWORD g_dwMaxRipple;
// ripple data
extern float* g_lpfRippleX;
extern float* g_lpfRippleZ;
extern float* g_lpfRipplePos;
extern float* g_lpfRippleSpeed;
// creature ripple indices
extern LPDWORD g_lpdwSwanRipple1Idx;
extern LPDWORD g_lpdwSwanRipple2Idx;
extern LPDWORD g_lpdwBBSwanRipple1Idx;
extern LPDWORD g_lpdwBBSwanRipple2Idx;
extern LPDWORD g_lpdwTurtleRipple1Idx;
extern LPDWORD g_lpdwTurtleRipple2Idx;
extern LPDWORD g_lpdwFishRipple1Idx;
extern LPDWORD g_lpdwFishRipple2Idx;
// camera data
extern DWORD g_dwCamMtx;
extern DWORD g_dwCamOutputMtx;
extern DWORD g_dwCamPriState;
extern DWORD g_dwCamNavState;
extern float g_fCamNavSpeed;
extern DWORD g_dwCamNavPos;
extern DWORD g_dwCamNavMax;
extern D3DXVECTOR3 g_vCamAhead;
extern float g_fCamTurnInc;
extern float g_fCamClimbInc;
extern DWORD g_dwCamTargetMtx;
// sound data
extern DWORD g_dwLeCygneMus;
extern DWORD g_dwEagleSFX;
extern DWORD g_dwBirds1SFX;
extern DWORD g_dwBirds2SFX;
extern DWORD g_dwSwan1SFX;
extern DWORD g_dwSwan2SFX;
extern DWORD g_dwSplashSFX;
extern DWORD g_dwBBSwan1SFX;
extern DWORD g_dwBBSwan2SFX;
extern DWORD g_dwBuzzSFX;
extern SOUNDPOSDATA g_spdEagle;
extern SOUNDPOSDATA g_spdBirds1;
extern SOUNDPOSDATA g_spdBirds2;
extern SOUNDPOSDATA g_spdSwan1;
extern SOUNDPOSDATA g_spdSwan2;
extern SOUNDPOSDATA g_spdSwan3;
extern SOUNDPOSDATA g_spdSplash;
extern SOUNDPOSDATA g_spdBBSwan1;
extern SOUNDPOSDATA g_spdBBSwan2;
extern SOUNDPOSDATA g_spdBuzz;
// sound source matrices
extern DWORD g_lpdwSndSrcMtx[];
// frame rate font
extern DWORD g_dwFPSFont;
// screensaver options
extern BOOL g_bPriorityMode;
extern DWORD g_dwHorzRes;
extern DWORD g_dwVertRes;
extern DWORD g_dwBufferCnt;
extern BOOL g_bFogMode;
extern float g_fMasterVol;
extern float g_fPrimaryVol;
extern float g_fSecondaryVol;
extern BOOL g_bFrameRate;
extern BOOL g_bSpeedMode;
extern BOOL g_bTexSmooth;
extern BOOL g_bShadeSmooth;
extern DWORD g_dwSceneNumSwans;
extern BOOL g_bSceneBBSwans;
extern DWORD g_dwSceneNumTurtles;
extern DWORD g_dwSceneNumDFlies;
extern DWORD g_dwSceneNumFish;
extern DWORD g_dwSceneNumBirds;
extern BOOL g_bLensFlareMode;
extern BOOL g_bUseRipples;
extern BOOL g_bTerrainHiRes;
extern BOOL g_bUseMusic;
extern BOOL g_bUseSFX;
extern BOOL g_bUse3DSound;
extern BOOL g_bShowClock;
// clock digits
extern DWORD g_dwClockDigit[10];
extern DWORD g_dwClockAM;
extern DWORD g_dwClockPM;
extern DWORD g_dwClockColon;
// fade-in data
extern float g_fFadeIn;
// "phantom" swan flag
extern BOOL g_bPhantomSwan;
// dialog boxes
extern DWORD g_dwConfigDlg;
extern DWORD g_dwAboutDlg;
#ifdef SHAREWARE_MODE
extern DWORD g_dwPurchaseDlg;
#endif
#ifdef SHAREWARE_MODE
// days remaining in trial period
extern DWORD g_dwTrialDays;
// nag screen textures
extern DWORD g_dwNagMain;
extern DWORD g_dwNagTFlash;
extern DWORD g_dwNagClose;
extern DWORD g_dwNagRedX;
extern DWORD g_dwNagDigits[];
extern DWORD g_dwNagCursor;
extern DWORD g_dwNagBlank;
// nag screen sound fx
extern DWORD g_dwNagSnd;
// nag screen active flag
extern BOOL g_bNagScreen;
// misc nag screen values
extern DWORD g_dwNagFreqCur;
extern DWORD g_dwNagFreqMax;
extern DWORD g_dwNagDelayCur;
extern DWORD g_dwNagDelayMax;
extern DWORD g_dwNagAnimPos;
#endif

// defines //

// reflection optimization flag
#define REFLECTION_OPTIMIZATION

// water surface vertex type
#define D3DFVF_WATERVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

typedef struct tagWATERVERTEX
{
	float x,y,z;
	float tu,tv;
} WATERVERTEX,*LPWATERVERTEX;

#define SET_WATERVERTEX(vtx,X,Y,Z,TU,TV) { vtx.x=X; vtx.y=Y; vtx.z=Z; vtx.tu=TU; vtx.tv=TV; }

// ripple vertex type
#define D3DFVF_RIPPLEVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

typedef struct tagRIPPLEVERTEX
{
	float x,y,z;
	float tu,tv;
} RIPPLEVERTEX,*LPRIPPLEVERTEX;

#define SET_RIPPLEVERTEX(vtx,X,Y,Z,TU,TV) { vtx.x=X; vtx.y=Y; vtx.z=Z; vtx.tu=TU; vtx.tv=TV; }

// constants //

// skybox values
#define SKYBOX_RAD 25000

// water surface values
#define WSURF_RAD      2500
#define WSURF_SIZE     125
#define WSURF_NUMTEX   64
#define WSURF_MOVEINC  0.01F
#define WSURF_SLOWDOWN 2

// scale factors
#define TERRAIN_SCALE   100
#define SKY_SCALE       100
#define SWAN_SCALE      1.0F
#define SWAN_SCALEVAR   0.1F
#define BBSWAN_SCALE    0.30F
#define BBSWAN_SCALEVAR 0.03F
#define TURTLE_SCALE    0.44F
#define TURTLE_SCALEVAR 0.08F
#define DRGFLY_SCALE    0.100F
#define DRGFLY_SCALEVAR 0.015F
#define FISH_SCALE      10.0F
#define FISH_SCALEVAR   1.0F
#define BIRD_SCALE      3.2F
#define BIRD_SCALEVAR   0.4F

// fog values
#define FOG_START 20
#define FOG_END   60
#define FOG_COLOR (D3DCOLOR_RGBA(186,227,255,255))

// ambient light
#define AMBIENT_LIGHT (D3DCOLOR_COLORVALUE(0.9F,0.9F,1,1))

// initial number of updates
#define NUM_INITUPDATES 500

// number of lens flare rings
#define NUM_FLARERINGS 8

// ripple values
#define MAX_RIPPLEDIA   20.0F
#define MIN_RIPPLESPEED 0.0008F
#define MAX_RIPPLESPEED 0.0016F

// misc. terrain values
#define TERRAIN_CENTERX 300
#define TERRAIN_CENTERZ -100
#define TERRAIN_XRANGE  350
#define TERRAIN_ZRANGE  350

// sun position
#define SUN_XPOS 7500
#define SUN_YPOS 3800
#define SUN_ZPOS -7500

// swan animation keyframes
#define KEYF_SWAN_BASE   0
#define KEYF_SWAN_HCTR   1
#define KEYF_SWAN_HCTL   2
#define KEYF_SWAN_HUTR   3
#define KEYF_SWAN_HUTL   4
#define KEYF_SWAN_HDTR   5
#define KEYF_SWAN_HDTL   6
#define KEYF_SWAN_HR1TR  7
#define KEYF_SWAN_HR1TL  8
#define KEYF_SWAN_HR2TR  9
#define KEYF_SWAN_HR2TL  10
#define KEYF_SWAN_HL1TR  11
#define KEYF_SWAN_HL1TL  12
#define KEYF_SWAN_HL2TR  13
#define KEYF_SWAN_HL2TL  14
#define KEYF_SWAN_HDR1TR 15
#define KEYF_SWAN_HDR1TL 16
#define KEYF_SWAN_HDR2TR 17
#define KEYF_SWAN_HDR2TL 18
#define KEYF_SWAN_HDL1TR 19
#define KEYF_SWAN_HDL1TL 20
#define KEYF_SWAN_HDL2TR 21
#define KEYF_SWAN_HDL2TL 22

// swan primary states
#define PRST_SWAN_LEAD     0
#define PRST_SWAN_FOLLOW   1
#define PRST_SWAN_REST     2

// swan action states
#define ACST_SWAN_HEADUP         0
#define ACST_SWAN_HEADDOWN       1
#define ACST_SWAN_HEADLEFT       2
#define ACST_SWAN_HEADRIGHT      3
#define ACST_SWAN_HEADLEFT2      4
#define ACST_SWAN_HEADRIGHT2     5
#define ACST_SWAN_HEADDOWNLEFT   6
#define ACST_SWAN_HEADDOWNRIGHT  7
#define ACST_SWAN_HEADDOWNLEFT2  8
#define ACST_SWAN_HEADDOWNRIGHT2 9
#define ACST_SWAN_HEADCENTER     10
#define ACST_SWAN_JUSTSWIM       11
#define ACST_SWAN_TRANSITION     12

// swan navigation states
#define NVST_SWAN_SPEEDUP    0
#define NVST_SWAN_SLOWDOWN   1
#define NVST_SWAN_TURNLEFT   2
#define NVST_SWAN_TURNRIGHT  3
#define NVST_SWAN_HARDLEFT   4
#define NVST_SWAN_HARDRIGHT  5
#define NVST_SWAN_JUSTSWIM   6

// misc. swan values
#define SWAN_ACTIONCHANGERATIO     15
#define SWAN_NUMACTIONSTATES       11
#define SWAN_TRANSITIONSPEED       0.0035F
#define SWAN_TAILSPEED             0.0150F
#define SWAN_NAVIGATIONCHANGERATIO 20
#define SWAN_NUMNAVIGATIONSTATES   6
#define SWAN_NAVIGATIONMAXINC      200
#define SWAN_SPEEDINC              0.0001F
#define SWAN_TURNINC               0.0006F
#define SWAN_HARDTURNINC           0.0012F
#define SWAN_MAXSPEED              0.0280F
#define SWAN_NUMPRIMARYSTATES      3
#define SWAN_PRIMARYCHANGERATIO    250
#define SWAN_SEPARATIONDISTANCE    50.0F
#define SWAN_SPEEDTOLERANCE        0.0010F
#define SWAN_MINSWANDISTANCE       10.0F
#define SWAN_MAXFOLLOWDISTANCE     15.0F
#define SWAN_FOLLOWTRACKINGVAL     0.2F
#define SWAN_TRACKINGSPEEDVAL      1.25F
#define SWAN_TRACKINGSPEEDINC      0.0020F
#define SWAN_TRACKINGSPEEDVAL1     0.75F
#define SWAN_TRACKINGSPEEDINC1     0.0020F
#define SWAN_TRACKINGSPEEDVAL2     1.5F
#define SWAN_TRACKINGSPEEDINC2     0.0040F
#define SWAN_MAXRESTSPEED          0.0020F
#define SWAN_MAXRESTDISTANCE       20.0F

// baby swan animation keyframes
#define KEYF_BBSWAN_BASE  0
#define KEYF_BBSWAN_HCTR  1
#define KEYF_BBSWAN_HCTL  2
#define KEYF_BBSWAN_HUTR  3
#define KEYF_BBSWAN_HUTL  4
#define KEYF_BBSWAN_HDTR  5
#define KEYF_BBSWAN_HDTL  6
#define KEYF_BBSWAN_HR1TR 7
#define KEYF_BBSWAN_HR1TL 8
#define KEYF_BBSWAN_HR2TR 9
#define KEYF_BBSWAN_HR2TL 10
#define KEYF_BBSWAN_HL1TR 11
#define KEYF_BBSWAN_HL1TL 12
#define KEYF_BBSWAN_HL2TR 13
#define KEYF_BBSWAN_HL2TL 14

// baby swan primary states
#define PRST_BBSWAN_FOLLOW 0
#define PRST_BBSWAN_WANDER 1
#define PRST_BBSWAN_PANIC  2
#define PRST_BBSWAN_REST   3

// baby swan action states
#define ACST_BBSWAN_HEADUP     0
#define ACST_BBSWAN_HEADDOWN   1
#define ACST_BBSWAN_HEADLEFT   2
#define ACST_BBSWAN_HEADRIGHT  3
#define ACST_BBSWAN_HEADLEFT2  4
#define ACST_BBSWAN_HEADRIGHT2 5
#define ACST_BBSWAN_HEADCENTER 6
#define ACST_BBSWAN_JUSTSWIM   7
#define ACST_BBSWAN_TRANSITION 8

// baby swan navigation states
#define NVST_BBSWAN_SPEEDUP   0
#define NVST_BBSWAN_SLOWDOWN  1
#define NVST_BBSWAN_TURNLEFT  2
#define NVST_BBSWAN_TURNRIGHT 3
#define NVST_BBSWAN_HARDLEFT  4
#define NVST_BBSWAN_HARDRIGHT 5
#define NVST_BBSWAN_JUSTSWIM  6

// misc. baby swan values
#define BBSWAN_ACTIONCHANGERATIO     12
#define BBSWAN_NUMACTIONSTATES       7
#define BBSWAN_TRANSITIONSPEED       0.0040F
#define BBSWAN_TAILSPEED             0.0250F
#define BBSWAN_NAVIGATIONCHANGERATIO 16
#define BBSWAN_NUMNAVIGATIONSTATES   6
#define BBSWAN_NAVIGATIONMAXINC      150
#define BBSWAN_SPEEDINC              0.00012F
#define BBSWAN_TURNINC               0.00080F
#define BBSWAN_HARDTURNINC           0.00140F
#define BBSWAN_MAXSPEED              0.03200F
#define BBSWAN_NUMPRIMARYSTATES      4
#define BBSWAN_PRIMARYCHANGERATIO    150
#define BBSWAN_MINSWANDISTANCE       6.0F
#define BBSWAN_MINBBSWANDISTANCE     3.0F
#define BBSWAN_MAXFOLLOWDISTANCE     17.5F
#define BBSWAN_PANICDISTANCE         25.0F
#define BBSWAN_MAXRESTSPEED          0.0040F
#define BBSWAN_MAXRESTDISTANCE       20.0F
#define BBSWAN_PANICSPEEDFACTOR      1.5F
#define BBSWAN_PANICTURNFACTOR       2.0F

// turtle animation keyframes
#define KEYF_TURTLE_BASE 0
#define KEYF_TURTLE_HC   0
#define KEYF_TURTLE_HU   1
#define KEYF_TURTLE_HD   2
#define KEYF_TURTLE_HL   3
#define KEYF_TURTLE_HR   4

// turtle primary states
#define PRST_TURTLE_SURFACE  0
#define PRST_TURTLE_DIVE     1
#define PRST_TURTLE_TRANSFER 2
#define PRST_TURTLE_JUSTSWIM 3

// turtle action states
#define ACST_TURTLE_HEADUP     0
#define ACST_TURTLE_HEADDOWN   1
#define ACST_TURTLE_HEADLEFT   2
#define ACST_TURTLE_HEADRIGHT  3
#define ACST_TURTLE_JUSTSWIM   4
#define ACST_TURTLE_TRANSITION 5

// turtle navigation states
#define NVST_TURTLE_SPEEDUP   0
#define NVST_TURTLE_SLOWDOWN  1
#define NVST_TURTLE_TURNLEFT  2
#define NVST_TURTLE_TURNRIGHT 3
#define NVST_TURTLE_JUSTSWIM  4

// misc. turtle values
#define TURTLE_ACTIONCHANGERATIO     24
#define TURTLE_NUMACTIONSTATES       5
#define TURTLE_TRANSITIONSPEED       0.0030F
#define TURTLE_NAVIGATIONCHANGERATIO 28
#define TURTLE_NUMNAVIGATIONSTATES   5
#define TURTLE_NAVIGATIONMAXINC      200
#define TURTLE_SPEEDINC              0.00008F
#define TURTLE_TURNINC               0.00060F
#define TURTLE_MINSPEED              0.00200F
#define TURTLE_MAXSPEED              0.01000F
#define TURTLE_NUMPRIMARYSTATES      4
#define TURTLE_MINTURTLEDISTANCE     25.0F
#define TURTLE_MINSWANDISTANCE       25.0F
#define TURTLE_MINBBSWANDISTANCE     25.0F
#define TURTLE_DIVEINC               0.0042F
#define TURTLE_MAXDIVEDEPTH          -1.0F
#define TURTLE_TRANSFERRATIO         300

// dragonfly primary states
#define PRST_DFLY_LEAD   0
#define PRST_DFLY_FOLLOW 1
#define PRST_DFLY_REST   2

// dragonfly navigation states
#define NVST_DFLY_SPEEDUP   0
#define NVST_DFLY_SLOWDOWN  1
#define NVST_DFLY_TURNLEFT  2
#define NVST_DFLY_TURNRIGHT 3
#define NVST_DFLY_HARDLEFT  4
#define NVST_DFLY_HARDRIGHT 5
#define NVST_DFLY_CLIMB     6
#define NVST_DFLY_DIVE      7
#define NVST_DFLY_DART      8
#define NVST_DFLY_JUSTFLY   9

// misc. dragonfly values
#define DFLY_NAVIGATIONCHANGERATIO 30
#define DFLY_NUMNAVIGATIONSTATES   10
#define DFLY_NAVIGATIONMAXINC      50
#define DFLY_SPEEDINC              0.0075F
#define DFLY_TURNINC               0.0050F
#define DFLY_HARDTURNINC           0.0120F
#define DFLY_MAXSPEED              0.1400F
#define DFLY_DARTINC               0.1600F
#define DFLY_MAXDARTINC            20
#define DFLY_MAXRESTSPEED          0.0500F
#define DFLY_NUMPRIMARYSTATES      3
#define DFLY_PRIMARYCHANGERATIO    300
#define DFLY_SEPARATIONDISTANCE    10.0F
#define DFLY_SPEEDTOLERANCE        0.0350F
#define DFLY_MINDFLYDISTANCE       1.0F
#define DFLY_MAXFOLLOWDISTANCE     4.0F
#define DFLY_FOLLOWTRACKINGVAL     0.2F
#define DFLY_TRACKINGSPEEDVAL      1.25F
#define DFLY_TRACKINGSPEEDINC      0.0200F
#define DFLY_TRACKINGSPEEDVAL1     0.75F
#define DFLY_TRACKINGSPEEDINC1     0.0200F
#define DFLY_TRACKINGSPEEDVAL2     1.5F
#define DFLY_TRACKINGSPEEDINC2     0.0400F
#define DFLY_MAXRESTDISTANCE       6.0F
#define DFLY_MINHEIGHT             0.5F
#define DFLY_MAXHEIGHT             6.5F
#define DFLY_WINGALPHA             0.35F
#define DFLY_MINSWANDISTANCE       10.0F
#define DFLY_MINBBSWANDISTANCE     10.0F
#define DFLY_MINTURTLEDISTANCE     10.0F
#define DFLY_MINSWANHEIGHT         6.0F
#define DFLY_MINBBSWANHEIGHT       3.0F
#define DFLY_MINTURTLEHEIGHT       2.5F
#define DFLY_HEIGHTINCFACTOR       0.75F

// fish primary states
#define PRST_FISH_LURK  0
#define PRST_FISH_JUMPL 1
#define PRST_FISH_JUMPR 2

// misc. fish values
#define FISH_NUMTEX             30
#define FISH_INITSPLASHTEX      17
#define FISH_SLOWDOWN           2
#define FISH_TRANSCOLOR         D3DCOLOR_RGBA(68,68,182,255)
#define FISH_MINCAMERADISTANCE  60.0F
#define FISH_MINSWANDISTANCE    20.0F
#define FISH_MINBBSWANDISTANCE  20.0F
#define FISH_MINTURTLEDISTANCE  20.0F
#define FISH_MINDFLYDISTANCE    30.0F
#define FISH_MINFISHDISTANCE    20.0F
#define FISH_NUMPRIMARYSTATES   3
#define FISH_PRIMARYCHANGERATIO 300

// bird animation keyframes
#define KEYF_BIRD_BASE 0
#define KEYF_BIRD_WU   1
#define KEYF_BIRD_WD   2

// bird navigation states
#define NVST_BIRD_SPEEDUP   0
#define NVST_BIRD_SLOWDOWN  1
#define NVST_BIRD_TURNLEFT  2
#define NVST_BIRD_TURNRIGHT 3
#define NVST_BIRD_HARDLEFT  4
#define NVST_BIRD_HARDRIGHT 5
#define NVST_BIRD_JUSTFLY   6

// misc. bird values
#define BIRD_WINGSPEED             0.033F
#define BIRD_MAXWINGPOS            2.5F
#define BIRD_NAVIGATIONCHANGERATIO 20
#define BIRD_NUMNAVIGATIONSTATES   7
#define BIRD_NAVIGATIONMAXINC      100
#define BIRD_SPEEDINC              0.005F
#define BIRD_TURNINC               0.004F
#define BIRD_HARDTURNINC           0.007F
#define BIRD_MINSPEED              0.500F
#define BIRD_MAXSPEED              0.750F
#define BIRD_BASEALTITUDE          400.0F
#define BIRD_ALTITUDEINC           20.0F

// camera primary states
#define PRST_CAMERA_FOLLOW 0

// camera navigation states
#define NVST_CAMERA_SPEEDUP   0
#define NVST_CAMERA_SLOWDOWN  1
#define NVST_CAMERA_TURNLEFT  2
#define NVST_CAMERA_TURNRIGHT 3
#define NVST_CAMERA_HARDLEFT  4
#define NVST_CAMERA_HARDRIGHT 5
#define NVST_CAMERA_CLIMB     6
#define NVST_CAMERA_DIVE      7
#define NVST_CAMERA_JUSTFLY   8

// misc. camera values
#define CAMERA_MINHEIGHT             6.0F
#define CAMERA_MAXHEIGHT             12.0F
#define CAMERA_PRIMARYCHANGERATIO    500
#define CAMERA_NUMPRIMARYSTATES      1
#define CAMERA_NAVIGATIONCHANGERATIO 100
#define CAMERA_NUMNAVIGATIONSTATES   9
#define CAMERA_NAVIGATIONMAXINC      200
#define CAMERA_SPEEDINC              0.002F
#define CAMERA_TURNINC               0.00003F
#define CAMERA_HARDTURNINC           0.00008F
#define CAMERA_MAXTURNINC            0.0009F
#define CAMERA_MINSPEED              0.03F
#define CAMERA_MAXSPEED              0.16F
#define CAMERA_MINSWANDISTANCE       45.0F
#define CAMERA_MINBBSWANDISTANCE     40.0F
#define CAMERA_MINTURTLEDISTANCE     50.0F
#define CAMERA_MINDFLYDISTANCE       10.0F
#define CAMERA_CLIMBINC              0.00008F
#define CAMERA_MAXCLIMBINC           0.008F
#define CAMERA_TARGETYOFFSET         6.0F
#define CAMERA_MAXAHEADYVALUE        0.06F
#define CAMERA_TRACKINGINC           0.0020F
#define CAMERA_TRACKINGINC2          0.0040F
#define CAMERA_OBSERVATIONDISTANCE   65.0F
#define CAMERA_FOLLOWTRACKINGVAL     0.2F
#define CAMERA_OOBSPEEDFACTOR        0.8F
#define CAMERA_TARGETDISTTOL         10.0F
#define CAMERA_MINSOUNDDISTANCE      120.0F
#define CAMERA_FADEININC             0.008F

// sound source values
#define SNDSRC_NUMSOURCES 16
#define SNDSRC_OOBFACTOR  1.25F

// sound effect max volume values
#define MAXVOL_EAGLE   1.000F
#define MAXVOL_BIRDS1  0.443F
#define MAXVOL_BIRDS2  0.592F
#define MAXVOL_SWAN1   0.958F
#define MAXVOL_SWAN2   0.877F
#define MAXVOL_SWAN3   0.958F
#define MAXVOL_SPLASH  0.625F
#define MAXVOL_BBSWAN1 0.765F
#define MAXVOL_BBSWAN2 0.765F
#define MAXVOL_BUZZ    0.659F

// sound effect repeat values
#define REPEAT_EAGLE   1280
#define REPEAT_BIRDS1  360
#define REPEAT_BIRDS2  360
#define REPEAT_SWAN1   320
#define REPEAT_SWAN2   320
#define REPEAT_SWAN3   320
#define REPEAT_BBSWAN1 440
#define REPEAT_BBSWAN2 440

// sound effect priorities
#define SFXPRI_EAGLE   90
#define SFXPRI_BIRDS1  30
#define SFXPRI_BIRDS2  40
#define SFXPRI_SWAN1   50
#define SFXPRI_SWAN2   60
#define SFXPRI_SWAN3   70
#define SFXPRI_SPLASH  100
#define SFXPRI_BBSWAN1 10
#define SFXPRI_BBSWAN2 20
#define SFXPRI_BUZZ    80

// sound effect minimum distance values
#define MINDIST_EAGLE   350.0F
#define MINDIST_BIRDS1  250.0F
#define MINDIST_BIRDS2  250.0F
#define MINDIST_SWAN1   100.0F
#define MINDIST_SWAN2   100.0F
#define MINDIST_SWAN3   100.0F
#define MINDIST_SPLASH  150.0F
#define MINDIST_BBSWAN1 100.0F
#define MINDIST_BBSWAN2 100.0F
#define MINDIST_BUZZ    12.0F

// sound effect maximum distance values
#define MAXDIST_EAGLE   5000.0F
#define MAXDIST_BIRDS1  2500.0F
#define MAXDIST_BIRDS2  2500.0F
#define MAXDIST_SWAN1   1000.0F
#define MAXDIST_SWAN2   1000.0F
#define MAXDIST_SWAN3   1000.0F
#define MAXDIST_SPLASH  500.0F
#define MAXDIST_BBSWAN1 1000.0F
#define MAXDIST_BBSWAN2 1000.0F
#define MAXDIST_BUZZ    50.0F

// number of managed audiopaths
#define NUM_MANAGEDAUDIOPATHS 8

// message identifiers
#define MSG_TOGGLEFRAMERATE  1
#define MSG_TOGGLESPEEDMODE  2

// configuration values
#define CFG_XRES_KEY "HorizRes"
#define CFG_XRES_MIN -1
#define CFG_XRES_MAX -1
#define CFG_XRES_DEF 640

#define CFG_YRES_KEY "VertRes"
#define CFG_YRES_MIN -1
#define CFG_YRES_MAX -1
#define CFG_YRES_DEF 480

#define CFG_RES_DEF "640x480"

#define CFG_STEX_KEY "SmoothTex"
#define CFG_STEX_MIN 0
#define CFG_STEX_MAX 1
#define CFG_STEX_DEF 1

#define CFG_SSHD_KEY "SmoothShade"
#define CFG_SSHD_MIN 0
#define CFG_SSHD_MAX 1
#define CFG_SSHD_DEF 1

#define CFG_MUSIC_KEY "MusicOn"
#define CFG_MUSIC_MIN 0
#define CFG_MUSIC_MAX 1
#define CFG_MUSIC_DEF 1

#define CFG_SOUND_KEY "SoundOn"
#define CFG_SOUND_MIN 0
#define CFG_SOUND_MAX 1
#define CFG_SOUND_DEF 1

#define CFG_3DSND_KEY "3DSound"
#define CFG_3DSND_MIN 0
#define CFG_3DSND_MAX 1
#define CFG_3DSND_DEF 1

#define CFG_MVOL_KEY "MasterVol"
#define CFG_MVOL_MIN 50
#define CFG_MVOL_MAX 100
#define CFG_MVOL_DEF 100

#define CFG_PVOL_KEY "MusicVol"
#define CFG_PVOL_MIN 50
#define CFG_PVOL_MAX 100
#define CFG_PVOL_DEF 90

#define CFG_SVOL_KEY "SoundVol"
#define CFG_SVOL_MIN 50
#define CFG_SVOL_MAX 100
#define CFG_SVOL_DEF 100

#define CFG_NUMSWANS_KEY "NumSwans"
#define CFG_NUMSWANS_MIN 0
#define CFG_NUMSWANS_MAX 36
#define CFG_NUMSWANS_DEF 24

#define CFG_NUMTURTLES_KEY "NumTurtles"
#define CFG_NUMTURTLES_MIN 0
#define CFG_NUMTURTLES_MAX 8
#define CFG_NUMTURTLES_DEF 3

#define CFG_NUMDFLIES_KEY "NumDFlies"
#define CFG_NUMDFLIES_MIN 0
#define CFG_NUMDFLIES_MAX 16
#define CFG_NUMDFLIES_DEF 8

#define CFG_NUMFISH_KEY "NumFish"
#define CFG_NUMFISH_MIN 0
#define CFG_NUMFISH_MAX 8
#define CFG_NUMFISH_DEF 3

#define CFG_NUMBIRDS_KEY "NumEagles"
#define CFG_NUMBIRDS_MIN 0
#define CFG_NUMBIRDS_MAX 8
#define CFG_NUMBIRDS_DEF 2

#define CFG_FOG_KEY "FogMode"
#define CFG_FOG_MIN 0
#define CFG_FOG_MAX 1
#define CFG_FOG_DEF 1

#define CFG_LFLARE_KEY "LensFlares"
#define CFG_LFLARE_MIN 0
#define CFG_LFLARE_MAX 1
#define CFG_LFLARE_DEF 1

#define CFG_RIPPLES_KEY "Ripples"
#define CFG_RIPPLES_MIN 0
#define CFG_RIPPLES_MAX 1
#define CFG_RIPPLES_DEF 1

#define CFG_CLOCK_KEY "ClockMode"
#define CFG_CLOCK_MIN 0
#define CFG_CLOCK_MAX 1
#define CFG_CLOCK_DEF 0

#define CFG_PRIORITY_KEY "PriorityMode"
#define CFG_PRIORITY_MIN 0
#define CFG_PRIORITY_MAX 1
#define CFG_PRIORITY_DEF 0

#define CFG_BUFFERCNT_KEY "BufferCnt"
#define CFG_BUFFERCNT_MIN 1
#define CFG_BUFFERCNT_MAX 2
#define CFG_BUFFERCNT_DEF 1

// logo button sizes
#define LOGOA_BTN_X 185
#define LOGOA_BTN_Y 60
#define LOGOB_BTN_X 185
#define LOGOB_BTN_Y 52
#define LOGOC_BTN_X 100
#define LOGOC_BTN_Y 52

#ifdef SHAREWARE_MODE
// purchase box image sizes
#define CCARDS_BTN_X   54
#define CCARDS_BTN_Y   99
#define SWANLAKE_BTN_X 173
#define SWANLAKE_BTN_Y 78

// trial day values
#define TRIALDAYS_MAX  7
#define TRIALDAYS_WARN 3

// nag screen frequency values
#define NAG_FREQ1 10800
#define NAG_FREQ2 7200
#define NAG_FREQ3 3600
#define NAG_FREQ4 600

// nag screen delay values
#define NAG_DELAY1 540
#define NAG_DELAY2 540
#define NAG_DELAY3 540
#define NAG_DELAY4 540

// nag screen closing value
#define NAG_CLOSEVAL 0xFFFFFF00L

// nag screen max idle time
#define NAG_MAXIDLE 3600
#endif

// prototypes //

extern BOOL SwanLakeInit(void);

extern BOOL SwanLakeExec(void);

extern void SwanLakeRelease(void);

#ifdef DEBUG_MODE
extern void AddTopViewFunctionality(void);
#endif

extern BOOL InitLakeScene(void);

extern BOOL InitEnvironment(void);

extern BOOL InitWaterSurface(void);


extern BOOL InitLandscape(void);

extern BOOL InitCreatures(void);

extern BOOL InitLensFlare(void);

extern void SwanLakeUpdate(void);

extern void SwanLakeRender(void);

extern void RenderClock(void);

extern BOOL InitClockDigits(void);

extern void SwanLakeConfigure(HWND hParentWnd);

extern BOOL SwanLakeProcessMessage(DWORD dwMsg,
								   LPVOID lpData);

#ifdef DEBUG_MODE
extern BOOL CreateWaterTextures(DWORD dwTexSize=64,
								DWORD dwNumTex=WSURF_NUMTEX,
								DWORD dwSkip1=300,
								DWORD dwSkip2=150,
								float fR1=0.9F,
								float fG1=0.9F,
								float fB1=0.7F,
								float fR2=0.3F,
								float fG2=0.3F,
								float fB2=0.7F,
								float fAMin=0.55F,
								float fAMax=0.77F,
								int nDropRatio=5,
								float fMaxWaveHeight=25.0F,
								float fDecayFactor=0.97F,
								float fAccelFactor=0.1F,
								float fTransferFactor=0.25F,
								LPCSTR lpFileName="water%d.dds");

extern void UpdateWaterSurface(float* lpfCur,
							   float* lpfNew,
							   float* lpfSpd,
							   DWORD dwTexSize,
							   int nDropRatio,
							   float fMaxWaveHeight,
							   float fDecayFactor,
							   float fAccelFactor,
							   float fTransferFactor);

extern void RenderWaterTexture(float* lpfCur,
							   DWORD dwTexIndex,
							   DWORD dwTexSize,
							   float fR1,
							   float fG1,
							   float fB1,
							   float fR2,
							   float fG2,
							   float fB2,
							   float fAMin,
							   float fAMax,
							   float fBlend);
#endif

extern DWORD AddWaterTextures(DWORD dwNumTex=WSURF_NUMTEX,
							  LPCSTR lpFileName="water%d.dds");

extern DWORD AddWaterVertexBuffer(void);

extern DWORD AddWaterIndexBuffer(void);

extern DWORD GetWaterTexture(void);

extern float GetWaterPosition(void);

extern void RenderWaterSurface(void);

#ifdef DEBUG_MODE
extern void CreateDataFiles(void);

extern BOOL CreateLensFlareTextures(void);
#endif

extern BOOL AddCreatures(void);

extern BOOL SwanLakeSetDataPath(void);

extern BOOL APIENTRY SwanLakeConfigDlgProc(HWND hDlg,
										   UINT Msg,
										   WPARAM wParam,
										   LPARAM lParam);

extern BOOL APIENTRY SwanLakeAboutDlgProc(HWND hDlg,
										  UINT Msg,
										  WPARAM wParam,
										  LPARAM lParam);

#ifdef SHAREWARE_MODE
extern BOOL APIENTRY SwanLakePurchaseDlgProc(HWND hDlg,
										     UINT Msg,
										     WPARAM wParam,
										     LPARAM lParam);
extern BOOL SwanLakeExecutePurchaseDlg(void);

extern BOOL SwanLakeLoadPurchasePage(void);
#endif

// swan functions

extern DWORD GetNextSwan(void);

extern DWORD AddSwan(void);

extern void SetSwanMatrix(DWORD dwIndex,
						  LPD3DXMATRIX lpMtx);

extern void UpdateSwans(void);

extern void RenderSwans(void);

extern void PurgeSwans(void);

extern DWORD AddSwanLoner(void);

extern DWORD AddSwanCouple(void);

extern DWORD AddSwanSPFamily(DWORD dwNumBBSwans);

extern DWORD AddSwanDPFamily(DWORD dwNumBBSwans);

extern void RandomizeSwanPrimaryState(DWORD dwIndex);

// misc. functions

extern BOOL IsPointOutOfUpperBounds(float fX,
									float fZ);

extern BOOL IsPointOutOfCenterBounds(float fX,
									 float fZ);

extern BOOL IsPointOutOfLowerBounds(float fX,
								    float fZ);

extern BOOL IsPointOutOfBounds(float fX,
							   float fZ);

extern BOOL SwanLakeLoadOptions(void);

extern BOOL SwanLakeSaveOptions(void);

extern DWORD GetResolutionWidth(LPSTR lpText);

extern DWORD GetResolutionHeight(LPSTR lpText);

#ifdef SHAREWARE_MODE
extern DWORD GetTrialDaysRemaining(void);
#endif

// baby swan functions

extern DWORD GetNextBBSwan(void);

extern DWORD AddBBSwan(void);

extern void UpdateBBSwans(void);

extern void RenderBBSwans(void);

extern void PurgeBBSwans(void);

// turtle functions

extern DWORD GetNextTurtle(void);

extern DWORD AddTurtle(void);

extern void UpdateTurtles(void);

extern void RenderTurtles(void);

extern void PurgeTurtles(void);

// dragonfly functions

extern DWORD GetNextDragonfly(void);

extern DWORD AddDragonfly(void);

extern void SetDragonflyMatrix(DWORD dwIndex,
							   LPD3DXMATRIX lpMtx);

extern void UpdateDragonflies(void);

extern void RenderDragonfliesA(void);

extern void RenderDragonfliesB(void);

extern void PurgeDragonflies(void);

extern DWORD AddDragonflyLoner(void);

extern DWORD AddDragonflyCouple(void);

extern void RandomizeDragonflyPrimaryState(DWORD dwIndex);

// fish functions

#ifdef DEBUG_MODE
extern BOOL CreateFishTextures(DWORD dwNumTex=FISH_NUMTEX,
							   LPCSTR lpFileNameIn="jfish%d.jpg",
							   LPCSTR lpFileNameOut="jfish%d.dds");
#endif

extern DWORD AddFishTextures(DWORD dwNumTex=FISH_NUMTEX,
							 LPCSTR lpFileName="jfish%d.dds");

extern DWORD GetNextFish(void);

extern DWORD AddFish(void);

extern void UpdateFish(void);

extern void RenderFish(void);

extern void PurgeFish(void);

// bird functions

extern DWORD GetNextBird(void);

extern DWORD AddBird(void);

extern void UpdateBirds(void);

extern void RenderBirds(void);

extern void PurgeBirds(void);

// ripple functions

#ifdef DEBUG_MODE
extern BOOL CreateRippleTextures(void);
#endif

extern DWORD AddRippleVertexBuffer(void);

extern DWORD AddRippleIndexBuffer(void);

extern BOOL InitRipples(void);

extern DWORD GetNextRipple(void);

extern DWORD AddRipple(void);

extern void StartRipple(DWORD dwIndex,
					    float fX,
					    float fZ,
						float fSpeed);

extern BOOL IsRippleComplete(DWORD dwIndex);

extern void UpdateRipples(void);

extern void RenderRipples(void);

extern void PurgeRipples(void);

extern float GetRandomRippleSpeed(void);

// camera functions

extern BOOL InitCameraObject(void);

extern void UpdateCameraObject(void);

extern void SetCameraObjectToViewMatrix(void);

extern DWORD GetNewCameraObjectTarget(void);

// sound functions

extern BOOL InitSoundFX(void);

extern void UpdateSoundFX(void);

extern void RenderSoundFX(void);

extern void ReleaseSoundFX(void);

extern void UpdateSoundPosition(LPSOUNDPOSDATA lpSPD);

extern BOOL InitSoundPositionData(void);

extern void SetSoundPositionData(LPSOUNDPOSDATA lpSPD,
								 DWORD dwSFXIndex,
								 DWORD dwRepeatVal,
								 DWORD dwSFXPriVal,
								 float fMaxVolVal,
								 float fMinDist,
								 float fMaxDist,
								 DWORD dwNumTarget,
								 LPDWORD lpdwTargetMtxArray);

extern BOOL InitSoundSources(void);

extern BOOL InitDragonflySFX(void);

extern BOOL InitFishSFX(void);

#ifdef SHAREWARE_MODE
// nag screen functions

extern BOOL InitNagScreen(void);

extern void OpenNagScreen(void);

extern void CloseNagScreen(void);

extern void UpdateNagScreen(void);

extern void RenderNagScreen(void);
#endif

// timestamp functions

extern void GetTimeStampLocations(LPSTR lpStr);

extern void ClearTimeStampLocations(LPSTR lpStr,
									DWORD dwBytes);

#ifdef SHAREWARE_MODE
extern void SetTimeStamp(LPTIMESTAMP lpTS,
						 DWORD dwLocID,
						 LPSTR lpTSLoc);

extern void GetTimeStamp(LPTIMESTAMP lpTS,
						 DWORD dwLocID,
						 DWORD dwUniID,
						 LPSTR lpTSLoc);

extern void SortTimeStamps(LPTIMESTAMP lpOut,
						   LPTIMESTAMP lpTS,
						   DWORD dwNumTS);
#else
extern void ResetTimeStamps(void);
#endif

// eof //

// Copyright 2005-2011 WaveBreak Software
