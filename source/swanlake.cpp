// Copyright 2005-2011 WaveBreak Software

// swanlake.cpp -- swan lake screensaver functions

// includes //

#include"..\\engine\\r3dwin.h"
#include"htmlhelp\\htmlhelp.h"
#include"swanlake.h"

#ifdef DEBUG_MODE
// work-arounds for debug mode (due to absence of DirectInput)
BOOL AddDefaultInputSettings(BOOL b) { return(TRUE); }
BOOL IsButtonDown(DWORD dw) { return(FALSE); }
DWORD g_dwDefSelectBtn=0;
DWORD AddInputButton(void) { return(0); }
DWORD AddInputButtonTrigger(DWORD dw1,DWORD dw2) { return(0); }
DWORD DIK_T=0;
void ReadUserInput(void) {}
float GetAxisPos(DWORD dw) { return(0); }
DWORD g_dwDefXAxis=0;
DWORD g_dwDefYAxis=0;
#endif

// globals //

// water surface buffers
DWORD g_dwWaterVtxBuf=NO_OBJECT;
DWORD g_dwWaterIdxBuf=NO_OBJECT;
DWORD g_dwWaterMat=NO_OBJECT;
DWORD g_dwWaterTex=NO_OBJECT;
// terrain mesh data
DWORD g_dwTerrainMesh=NO_OBJECT;
DWORD g_dwTerrainMtx=NO_OBJECT;
// sky mesh data
DWORD g_dwSkyMesh=NO_OBJECT;
DWORD g_dwSkyMtx=NO_OBJECT;
#ifdef DEBUG_MODE
// top view data
DWORD g_dwTopViewMtx=NO_OBJECT;
DWORD g_dwTopViewBtn=NO_OBJECT;
#endif
// initial updates flag
BOOL g_bInitUpdates=FALSE;
// creature animations & meshes
DWORD g_dwSwanAnim=NO_OBJECT;
DWORD g_dwBBSwanAnim=NO_OBJECT;
DWORD g_dwTurtleAnim=NO_OBJECT;
DWORD g_dwDragonflyMesh=NO_OBJECT;
DWORD g_dwDragonflyWingAnim=NO_OBJECT;
DWORD g_dwFishTex=NO_OBJECT;
DWORD g_dwBirdAnim=NO_OBJECT;
// swan counters
DWORD g_dwNumSwan=0;
DWORD g_dwMaxSwan=0;
// swan data
LPDWORD g_lpdwSwanMtx=NULL;
LPDWORD g_lpdwSwanPriState=NULL;
LPDWORD g_lpdwSwanActState=NULL;
LPDWORD g_lpdwSwanNavState=NULL;
LPDWORD g_lpdwSwanKeyframe1=NULL;
LPDWORD g_lpdwSwanKeyframe2=NULL;
float* g_lpfSwanTweenVal=NULL;
float* g_lpfSwanTailPos=NULL;
float* g_lpfSwanTailDir=NULL;
float* g_lpfSwanTransPos=NULL;
float* g_lpfSwanTransSpeed=NULL;
LPDWORD g_lpdwSwanTransKey1=NULL;
LPDWORD g_lpdwSwanTransKey2=NULL;
float* g_lpfSwanNavSpeed=NULL;
LPDWORD g_lpdwSwanNavPos=NULL;
LPDWORD g_lpdwSwanNavMax=NULL;
LPDWORD g_lpdwSwanMateIdx=NULL;
// baby swan counters
DWORD g_dwNumBBSwan=0;
DWORD g_dwMaxBBSwan=0;
// baby swan data
LPDWORD g_lpdwBBSwanMtx=NULL;
LPDWORD g_lpdwBBSwanPriState=NULL;
LPDWORD g_lpdwBBSwanActState=NULL;
LPDWORD g_lpdwBBSwanNavState=NULL;
LPDWORD g_lpdwBBSwanKeyframe1=NULL;
LPDWORD g_lpdwBBSwanKeyframe2=NULL;
float* g_lpfBBSwanTweenVal=NULL;
float* g_lpfBBSwanTailPos=NULL;
float* g_lpfBBSwanTailDir=NULL;
float* g_lpfBBSwanTransPos=NULL;
float* g_lpfBBSwanTransSpeed=NULL;
LPDWORD g_lpdwBBSwanTransKey1=NULL;
LPDWORD g_lpdwBBSwanTransKey2=NULL;
float* g_lpfBBSwanNavSpeed=NULL;
LPDWORD g_lpdwBBSwanNavPos=NULL;
LPDWORD g_lpdwBBSwanNavMax=NULL;
LPDWORD g_lpdwBBSwanParent1Idx=NULL;
LPDWORD g_lpdwBBSwanParent2Idx=NULL;
// turtle counters
DWORD g_dwNumTurtle=0;
DWORD g_dwMaxTurtle=0;
// turtle data
LPDWORD g_lpdwTurtleMtx=NULL;
LPDWORD g_lpdwTurtlePriState=NULL;
LPDWORD g_lpdwTurtleActState=NULL;
LPDWORD g_lpdwTurtleNavState=NULL;
LPDWORD g_lpdwTurtleKeyframe1=NULL;
LPDWORD g_lpdwTurtleKeyframe2=NULL;
float* g_lpfTurtleTweenVal=NULL;
float* g_lpfTurtleTransPos=NULL;
float* g_lpfTurtleTransSpeed=NULL;
float* g_lpfTurtleNavSpeed=NULL;
LPDWORD g_lpdwTurtleNavPos=NULL;
LPDWORD g_lpdwTurtleNavMax=NULL;
// dragonfly counters
DWORD g_dwNumDFly=0;
DWORD g_dwMaxDFly=0;
// dragonfly data
LPDWORD g_lpdwDFlyMtx=NULL;
LPDWORD g_lpdwDFlyPriState=NULL;
LPDWORD g_lpdwDFlyNavState=NULL;
float* g_lpfDFlyNavSpeed=NULL;
LPDWORD g_lpdwDFlyNavPos=NULL;
LPDWORD g_lpdwDFlyNavMax=NULL;
LPD3DXVECTOR3 g_lpvDFlyDartVec=NULL;
LPDWORD g_lpdwDFlyMateIdx=NULL;
LPDWORD g_lpdwDFlyAPathID=NULL;
LPDWORD g_lpdwDFlyBuzzIdx=NULL;
LPBOOL g_lpbDFlyPlayback=NULL;
// fish counters
DWORD g_dwNumFish=0;
DWORD g_dwMaxFish=0;
// fish data
LPD3DXVECTOR3 g_lpvFishPos=NULL;
float* g_lpfFishScale=0;
LPDWORD g_lpdwFishPriState=NULL;
LPDWORD g_lpdwFishAnimFrame=NULL;
LPDWORD g_lpdwFishAPathID=NULL;
LPDWORD g_lpdwFishSplashIdx=NULL;
LPBOOL g_lpbFishPlayback=NULL;
// bird counters
DWORD g_dwNumBird=0;
DWORD g_dwMaxBird=0;
// bird data
LPDWORD g_lpdwBirdMtx=NULL;
float* g_lpfBirdWingPos=NULL;
float* g_lpfBirdWingDir=NULL;
LPDWORD g_lpdwBirdNavState=NULL;
float* g_lpfBirdNavSpeed=NULL;
LPDWORD g_lpdwBirdNavPos=NULL;
LPDWORD g_lpdwBirdNavMax=NULL;
LPDWORD g_lpdwBirdKeyframe1=NULL;
LPDWORD g_lpdwBirdKeyframe2=NULL;
float* g_lpfBirdTweenVal=NULL;
// lens flare data
LENSFLAREDATA g_lfd;
// ripple rendering data
DWORD g_dwRippleVtxBuf;
DWORD g_dwRippleIdxBuf;
DWORD g_dwRippleMat;
DWORD g_lpdwRippleTex[4];
DWORD g_dwRippleMtx;
// ripple counters
DWORD g_dwNumRipple=0;
DWORD g_dwMaxRipple=0;
// ripple data
float* g_lpfRippleX=NULL;
float* g_lpfRippleZ=NULL;
float* g_lpfRipplePos=NULL;
float* g_lpfRippleSpeed=NULL;
// creature ripple indices
LPDWORD g_lpdwSwanRipple1Idx=NULL;
LPDWORD g_lpdwSwanRipple2Idx=NULL;
LPDWORD g_lpdwBBSwanRipple1Idx=NULL;
LPDWORD g_lpdwBBSwanRipple2Idx=NULL;
LPDWORD g_lpdwTurtleRipple1Idx=NULL;
LPDWORD g_lpdwTurtleRipple2Idx=NULL;
LPDWORD g_lpdwFishRipple1Idx=NULL;
LPDWORD g_lpdwFishRipple2Idx=NULL;
// camera data
DWORD g_dwCamMtx=NO_OBJECT;
DWORD g_dwCamOutputMtx=NO_OBJECT;
DWORD g_dwCamPriState=NULL;
DWORD g_dwCamNavState=NULL;
float g_fCamNavSpeed=NULL;
DWORD g_dwCamNavPos=NULL;
DWORD g_dwCamNavMax=NULL;
D3DXVECTOR3 g_vCamAhead=D3DXVECTOR3(0,0,0);
float g_fCamTurnInc=NULL;
float g_fCamClimbInc=NULL;
DWORD g_dwCamTargetMtx=NO_OBJECT;
// sound data
BOOL g_bUse3DSound=FALSE;
DWORD g_dwLeCygneMus=NO_OBJECT;
DWORD g_dwEagleSFX=NO_OBJECT;
DWORD g_dwBirds1SFX=NO_OBJECT;
DWORD g_dwBirds2SFX=NO_OBJECT;
DWORD g_dwSwan1SFX=NO_OBJECT;
DWORD g_dwSwan2SFX=NO_OBJECT;
DWORD g_dwSwan3SFX=NO_OBJECT;
DWORD g_dwSplashSFX=NO_OBJECT;
DWORD g_dwBBSwan1SFX=NO_OBJECT;
DWORD g_dwBBSwan2SFX=NO_OBJECT;
DWORD g_dwBuzzSFX=NO_OBJECT;
SOUNDPOSDATA g_spdEagle;
SOUNDPOSDATA g_spdBirds1;
SOUNDPOSDATA g_spdBirds2;
SOUNDPOSDATA g_spdSwan1;
SOUNDPOSDATA g_spdSwan2;
SOUNDPOSDATA g_spdSwan3;
SOUNDPOSDATA g_spdSplash;
SOUNDPOSDATA g_spdBBSwan1;
SOUNDPOSDATA g_spdBBSwan2;
SOUNDPOSDATA g_spdBuzz;
// sound source matrices
DWORD g_lpdwSndSrcMtx[SNDSRC_NUMSOURCES];
// frame rate font
DWORD g_dwFPSFont=NO_OBJECT;
// screensaver options
BOOL g_bPriorityMode=FALSE;
DWORD g_dwHorzRes=0;
DWORD g_dwVertRes=0;
DWORD g_dwBufferCnt=0;
BOOL g_bFogMode=FALSE;
float g_fMasterVol=0;
float g_fPrimaryVol=0;
float g_fSecondaryVol=0;
BOOL g_bFrameRate=FALSE;
BOOL g_bSpeedMode=FALSE;
BOOL g_bTexSmooth=FALSE;
BOOL g_bShadeSmooth=FALSE;
DWORD g_dwSceneNumSwans=0;
BOOL g_bSceneBBSwans=FALSE;
DWORD g_dwSceneNumTurtles=0;
DWORD g_dwSceneNumDFlies=0;
DWORD g_dwSceneNumFish=0;
DWORD g_dwSceneNumBirds=0;
BOOL g_bLensFlareMode=FALSE;
BOOL g_bUseRipples=FALSE;
BOOL g_bTerrainHiRes=FALSE;
BOOL g_bUseMusic=FALSE;
BOOL g_bUseSFX=FALSE;
BOOL g_bShowClock=FALSE;
// clock digits
DWORD g_dwClockDigit[10];
DWORD g_dwClockAM=NO_OBJECT;
DWORD g_dwClockPM=NO_OBJECT;
DWORD g_dwClockColon=NO_OBJECT;
// fade-in data
float g_fFadeIn=0.0F;
// "phantom" swan flag
BOOL g_bPhantomSwan=FALSE;
// dialog boxes
DWORD g_dwConfigDlg=NO_OBJECT;
DWORD g_dwAboutDlg=NO_OBJECT;
#ifdef SHAREWARE_MODE
DWORD g_dwPurchaseDlg=NO_OBJECT;
#endif
#ifdef SHAREWARE_MODE
// days remaining in trial period
DWORD g_dwTrialDays=0;
// nag screen textures
DWORD g_dwNagMain=NO_OBJECT;
DWORD g_dwNagTFlash=NO_OBJECT;
DWORD g_dwNagClose=NO_OBJECT;
DWORD g_dwNagRedX=NO_OBJECT;
DWORD g_dwNagDigits[10];
DWORD g_dwNagCursor=NO_OBJECT;
DWORD g_dwNagBlank=NO_OBJECT;
// nag screen sound fx
DWORD g_dwNagSnd=NO_OBJECT;
// nag screen active flag
BOOL g_bNagScreen=FALSE;
// misc nag screen values
DWORD g_dwNagFreqCur=0;
DWORD g_dwNagFreqMax=0;
DWORD g_dwNagDelayCur=0;
DWORD g_dwNagDelayMax=0;
DWORD g_dwNagAnimPos=0;
#endif

// functions //

// SwanLakeInit()

BOOL SwanLakeInit(void)
{
	LoadingOutput("SwanLakeInit");

	// counter
	DWORD dwCount;

#ifdef DEBUG_MODE
	// create data files
	CreateDataFiles();
#endif

	// enable mouse tracking
	EnableMouseTracking();

	// set data path
	if(!SwanLakeSetDataPath())
		return(FALSE);
	
// this section must be after SetDataPath!
#ifdef SHAREWARE_MODE
	// get days left
	g_dwTrialDays=GetTrialDaysRemaining();
#else
	// reset leftover timestamps
	ResetTimeStamps();
#endif

	// open data file
	if(!OpenDataFile("swanlake.dat"))
		return(FALSE);

	// initialize lake scene
	if(!InitLakeScene())
	{
		CloseDataFile();
		return(FALSE);
	}

	// initialize camera object
	if(!InitCameraObject())
	{
		CloseDataFile();
		return(FALSE);
	}

	// initialize sound effects
	if(!InitSoundFX())
	{
		CloseDataFile();
		return(FALSE);
	}

	// initialize clock digits
	if(!InitClockDigits())
	{
		CloseDataFile();
		return(FALSE);
	}

#ifdef SHAREWARE_MODE
	// initialize nag screen
	if(!InitNagScreen())
		return(FALSE);
#endif

	// close data file
	CloseDataFile();

#ifdef DEBUG_MODE
	// add default input
	AddDefaultInputSettings(TRUE);

	// add top view functionality
	AddTopViewFunctionality();
#endif

	// add font for fps info
	g_dwFPSFont=AddFont("Courier New",16);

	// check font
	if(g_dwFPSFont==NO_OBJECT)
		return(FALSE);

#ifdef SHAREWARE_MODE
	// check days left
	if(g_dwTrialDays!=0)
#endif
	{
		// set initial updates flag
		g_bInitUpdates=TRUE;

		// perform initial updates
		for(dwCount=0;dwCount<NUM_INITUPDATES;dwCount++)
			SwanLakeUpdate();

		// reset initial updates flag
		g_bInitUpdates=FALSE;

		// start music
		if(g_bUseMusic)
			PlayPrimaryAudioSegment(g_dwLeCygneMus);
	}

	// set fade-in value
	g_fFadeIn=1.5F;

	// ok
	return(TRUE);
}

#ifdef DEBUG_MODE
// CreateDataFiles()

void CreateDataFiles(void)
{
	LoadingOutput("CreateDataFiles");
/*
	// create water surface textures
	if(!CreateWaterTextures())
		return;
*/
/*
	// compress sky model
	CompressX("sky.x");

	// create swan animation
	CreateXFAFromX("swan.xfa",
				   "swan.x");

	// create baby swan animation
	CreateXFAFromX("bbswan.xfa",
				   "bbswan.x");

	// create turtle animation
	CreateXFAFromX("turtle.xfa",
				   "turtle.x");

	// compress dragonfly model
	CompressX("drgfly.x");
	
	// create dragonfly wing animation
	CreateXFAFromX("dflywngs.xfa",
				   "dflywngs.x");

	// create bird animation
	CreateXFAFromX("bird.xfa",
				   "bird.x");

	// create lens flare textures
	CreateLensFlareTextures();

	// create ripple texture
	CreateRippleTextures();
*/
/*
	// create fish textures
	if(!CreateFishTextures())
		return;
*/
/*
	// create cursor texture
	CreateDDSFromImage("n_cursor.dds",
					   "n_cursor.bmp",
					   TRUE,
					   D3DCOLOR_RGBA(255,0,0,255));
*/	
#ifdef COMPILE_DATAFILES
	// counter
	DWORD dwCount;

	// create data file
	OpenDataCompiler("..\\swanlake.dat");

	LockGlobalMsg()

	// add data
	for(dwCount=0;dwCount<9;dwCount++)
	{
		sprintf(g_lpMsg,"flare%d.dds",dwCount);
		AddDataFileRef(g_lpMsg);
	}
	for(dwCount=1;dwCount<31;dwCount++)
	{
		sprintf(g_lpMsg,"jfish%d.dds",dwCount);
		AddDataFileRef(g_lpMsg);
	}
	for(dwCount=0;dwCount<4;dwCount++)
	{
		sprintf(g_lpMsg,"ripple%d.dds",dwCount);
		AddDataFileRef(g_lpMsg);
	}
	for(dwCount=0;dwCount<64;dwCount++)
	{
		sprintf(g_lpMsg,"water%d.dds",dwCount);
		AddDataFileRef(g_lpMsg);
	}

	UnlockGlobalMsg()

	AddDataFileRef("bbswan.jpg");
	AddDataFileRef("bird.jpg");
	AddDataFileRef("dflywngs.jpg");
	AddDataFileRef("drgfly.jpg");
	AddDataFileRef("sky.jpg");
	AddDataFileRef("swan.jpg");
	AddDataFileRef("trnmap.jpg");
	AddDataFileRef("trnmap2.jpg");
	AddDataFileRef("turtle.jpg");

	AddDataFileRef("bbswan1.wav");
	AddDataFileRef("bbswan2.wav");
	AddDataFileRef("birds1.wav");
	AddDataFileRef("birds2.wav");
	AddDataFileRef("buzz1.wav");
	AddDataFileRef("eagle1.wav");
	AddDataFileRef("splash1.wav");
	AddDataFileRef("swan1.wav");
	AddDataFileRef("swan2.wav");
	AddDataFileRef("swan3.wav");
	AddDataFileRef("lecygne2.mid");

	AddDataFileRef("drgfly.x");
	AddDataFileRef("sky.x");
	AddDataFileRef("terrain.x");
	AddDataFileRef("bbswan.xfa");
	AddDataFileRef("bird.xfa");
	AddDataFileRef("dflywngs.xfa");
	AddDataFileRef("swan.xfa");
	AddDataFileRef("turtle.xfa");

	AddDataFileRef("clock0.jpg");
	AddDataFileRef("clock1.jpg");
	AddDataFileRef("clock2.jpg");
	AddDataFileRef("clock3.jpg");
	AddDataFileRef("clock4.jpg");
	AddDataFileRef("clock5.jpg");
	AddDataFileRef("clock6.jpg");
	AddDataFileRef("clock7.jpg");
	AddDataFileRef("clock8.jpg");
	AddDataFileRef("clock9.jpg");
	AddDataFileRef("clockam.jpg");
	AddDataFileRef("clockpm.jpg");
	AddDataFileRef("clockcln.jpg");

	AddDataFileRef("n_thanks.jpg");
	AddDataFileRef("n_expsn.jpg");
	AddDataFileRef("n_expd.jpg");
	AddDataFileRef("n_tflash.jpg");
	AddDataFileRef("n_close.jpg");
	AddDataFileRef("n_redx.jpg");
	AddDataFileRef("n_blank.bmp");
	AddDataFileRef("n_digit0.jpg");
	AddDataFileRef("n_digit1.jpg");
	AddDataFileRef("n_digit2.jpg");
	AddDataFileRef("n_digit3.jpg");
	AddDataFileRef("n_digit4.jpg");
	AddDataFileRef("n_digit5.jpg");
	AddDataFileRef("n_digit6.jpg");
	AddDataFileRef("n_digit7.jpg");
	AddDataFileRef("n_digit8.jpg");
	AddDataFileRef("n_digit9.jpg");
	AddDataFileRef("n_cursor.dds");
	AddDataFileRef("n_chord1.wav");

	// close data file
	CloseDataCompiler("..\\swanlake.dat");
#endif
}
#endif

// SwanLakeExec()

BOOL SwanLakeExec(void)
{
	// update scene
	SwanLakeUpdate();

/*#ifdef DEBUG_MODE
	// speed up world if necessary
	if(IsButtonDown(g_dwDefSelectBtn))
		g_bSpeedMode=TRUE;
	else
		g_bSpeedMode=FALSE;
#endif*/

	// speed up world if necessary
	if(g_bSpeedMode)
		for(int n=0;n<9;n++)
			SwanLakeUpdate();

	// render scene
	SwanLakeRender();

#ifdef DEBUG_MODE
	// check for exit
	if(IsKeyDown(VK_ESCAPE))
		return(FALSE);
#endif

#ifdef SHAREWARE_MODE
	// check for close
	if(g_bCloseOK)
		return(FALSE);
#endif

	// ok
	return(TRUE);
}

// SwanLakeRelease()

void SwanLakeRelease(void)
{
	// purge creatures
	PurgeSwans();
	PurgeBBSwans();
	PurgeTurtles();
	PurgeDragonflies();
	PurgeFish();
	PurgeBirds();
	PurgeRipples();

	// release sound effects
	ReleaseSoundFX();
}

#ifdef DEBUG_MODE
// AddTopViewFunctionality()

void AddTopViewFunctionality(void)
{
	LoadingOutput("AddTopViewFunctionality");

	// add top view matrix
	g_dwTopViewMtx=AddMatrix(1,0,0,0,
							 0,0,1,0,
							 0,-1,0,0,
							 TERRAIN_CENTERX,500,TERRAIN_CENTERZ,1);

	// add top view button
	g_dwTopViewBtn=AddInputButton();

	// set button trigger
	AddInputButtonTrigger(g_dwTopViewBtn,DIK_T);
}
#endif

// InitLakeScene()

BOOL InitLakeScene(void)
{
	LoadingOutput("InitLakeScene");

	// initialize environment
	if(!InitEnvironment())
		return(FALSE);
	
	// initialize water surface	
	if(!InitWaterSurface())
		return(FALSE);

	// initialize landscape
	if(!InitLandscape())
		return(FALSE);

	// initialize creatures
	if(!InitCreatures())
		return(FALSE);

	// initialize lens flare
	if(!InitLensFlare())
		return(FALSE);

	// initialize ripples
	if(!InitRipples())
		return(FALSE);

	// ok
	return(TRUE);
}

// InitWaterSurface()

BOOL InitWaterSurface(void)
{
	LoadingOutput("InitWaterSurface");

	// add water material
	g_dwWaterMat=AddMaterial(0,0,0,0,
							 0,0,0,0,
							 0,0,0,0,
							 1,1,1,1,
							 0);

	// check material
	if(g_dwWaterMat==NO_OBJECT)
		return(FALSE);
	
	// add water textures
	g_dwWaterTex=AddWaterTextures();

	// check texture
	if(g_dwWaterTex==NO_OBJECT)
		return(FALSE);

	// add water vertex buffer
	g_dwWaterVtxBuf=AddWaterVertexBuffer();

	// check vertex buffer
	if(g_dwWaterVtxBuf==NO_OBJECT)
		return(FALSE);

	// add water index buffer
	g_dwWaterIdxBuf=AddWaterIndexBuffer();

	// check index buffer
	if(g_dwWaterIdxBuf==NO_OBJECT)
		return(FALSE);

	// ok
	return(TRUE);
}

// InitLandscape()

BOOL InitLandscape(void)
{
	LoadingOutput("InitLandscape");

	// add terrain mesh
	g_dwTerrainMesh=AddMesh("terrain.x",NULL,TRUE,FALSE);

	// check mesh
	if(g_dwTerrainMesh==NO_OBJECT)
		return(FALSE);

	// check for low-res terrain
	if(g_bTerrainHiRes==FALSE||
	   g_lpDevCaps.MaxTextureWidth<1024||
	   g_lpDevCaps.MaxTextureHeight<1024)
	{
		// cast subset info pointer
		LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[g_dwTerrainMesh];
	
		// release hi-res texture
		ReleaseCOMPtr(g_lplpTex[lpMeshSubsetInfo[0].dwTex]);

		// load low-res texture
		lpMeshSubsetInfo[0].dwTex=AddTexture("trnmap2.jpg");

		// check texture
		if(lpMeshSubsetInfo[0].dwTex==NO_OBJECT)
			return(FALSE);
	}

	// add terrain matrix
	g_dwTerrainMtx=AddMatrix();

	// check matrix
	if(g_dwTerrainMtx==NO_OBJECT)
		return(FALSE);

	// scale terrain matrix
	MatrixScale(g_dwTerrainMtx,TERRAIN_SCALE);

	// add sky mesh
	g_dwSkyMesh=AddMesh("sky.x",NULL,TRUE,FALSE);

	// check mesh
	if(g_dwSkyMesh==NO_OBJECT)
		return(FALSE);

	// add sky matrix
	g_dwSkyMtx=AddMatrix();

	// check matrix
	if(g_dwSkyMtx==NO_OBJECT)
		return(FALSE);

	// scale sky matrix
	MatrixScale(g_dwSkyMtx,SKY_SCALE);

	// ok
	return(TRUE);
}

// InitEnvironment()

BOOL InitEnvironment(void)
{
	LoadingOutput("InitEnvironment");

	// set texturing mode
	if(g_bTexSmooth)
		EnableLinearFilter();
	else
		DisableLinearFilter();

	// enable vertical billboards
	EnableVerticalBillboards();

	// set fog parameters
	if(g_bFogMode)
	{
		SetFogStart(FOG_START*TERRAIN_SCALE);
		SetFogEnd(FOG_END*TERRAIN_SCALE);
		SetFogColor(FOG_COLOR);
	}

	// set ambient light
	SetAmbientLight(AMBIENT_LIGHT);

	// set projection matrix
	SetDefaultProjectionMatrix(1,SKYBOX_RAD*1.25F);

	// ok
	return(TRUE);
}

// InitCreatures()

BOOL InitCreatures(void)
{
	LoadingOutput("InitCreatures");

	// add swan animation
	g_dwSwanAnim=AddAnimation("swan.xfa");

	// check animation
	if(g_dwSwanAnim==NO_OBJECT)
		return(FALSE);

	// add baby swan animation
	g_dwBBSwanAnim=AddAnimation("bbswan.xfa");

	// check animation
	if(g_dwBBSwanAnim==NO_OBJECT)
		return(FALSE);

	// add turtle animation
	g_dwTurtleAnim=AddAnimation("turtle.xfa");

	// check animation
	if(g_dwTurtleAnim==NO_OBJECT)
		return(FALSE);

	// add dragonfly body
	g_dwDragonflyMesh=AddMesh("drgfly.x");

	// check mesh
	if(g_dwDragonflyMesh==NO_OBJECT)
		return(FALSE);

	// add dragonfly wing animation
	g_dwDragonflyWingAnim=AddAnimation("dflywngs.xfa");

	// check animation
	if(g_dwDragonflyWingAnim==NO_OBJECT)
		return(FALSE);

	// add fish textures
	g_dwFishTex=AddFishTextures();

	// check texture
	if(g_dwFishTex==NO_OBJECT)
		return(FALSE);

	// add bird animation
	g_dwBirdAnim=AddAnimation("bird.xfa");

	// check animation
	if(g_dwBirdAnim==NO_OBJECT)
		return(FALSE);

	// add creatures
	if(!AddCreatures())
		return(FALSE);

	// ok
	return(TRUE);
}

// InitLensFlare()

BOOL InitLensFlare(void)
{
	LoadingOutput("InitLensFlare");

	// init lens flare data
	InitLensFlareData(&g_lfd,
					  "flare%d.dds",
					  NUM_FLARERINGS,
					  4.0F,
					  128,
					  32,
					  96,
					  1,1,1,
					  1.2F,
					  0.7F,
					  1.0F);

	// ok
	return(TRUE);
}

#ifdef DEBUG_MODE
// CreateWaterTextures()

BOOL CreateWaterTextures(DWORD dwTexSize,
						 DWORD dwNumTex,
						 DWORD dwSkip1,
						 DWORD dwSkip2,
						 float fR1,
						 float fG1,
						 float fB1,
						 float fR2,
						 float fG2,
						 float fB2,
						 float fAMin,
						 float fAMax,
						 int nDropRatio,
					     float fMaxWaveHeight,
					     float fDecayFactor,
					     float fAccelFactor,
					     float fTransferFactor,
						 LPCSTR lpFileName)
{
	// counters
	DWORD dwCount;

	// water surface values
	float* lpfCur;
	float* lpfNew;
	float* lpfSpd;

	// compute memory size
	DWORD dwMemSize=Squared(dwTexSize)*sizeof(float);

	// allocate water surface values
	if(!AllocMem((LPVOID*)&lpfCur,
				 dwMemSize))
		return(FALSE);
	if(!AllocMem((LPVOID*)&lpfNew,
				 dwMemSize))
		return(FALSE);
	if(!AllocMem((LPVOID*)&lpfSpd,
				 dwMemSize))
		return(FALSE);

	// init water surface values
	ZeroMemory((LPVOID)lpfCur,
			   dwMemSize);
	ZeroMemory((LPVOID)lpfNew,
			   dwMemSize);
	ZeroMemory((LPVOID)lpfSpd,
			   dwMemSize);

	// add first rendering texture
	DWORD dwTexIndex=AddTexture(dwTexSize,
								dwTexSize,
								TRUE,
								TRUE);

	// add remaining rendering textures
	for(dwCount=1;dwCount<dwNumTex;dwCount++)
		AddTexture(dwTexSize,
				   dwTexSize,
				   TRUE,
				   TRUE);

	// update to first skip value
	for(dwCount=0;dwCount<dwSkip1;dwCount++)
		UpdateWaterSurface(lpfCur,
						   lpfNew,
						   lpfSpd,
						   dwTexSize,
						   nDropRatio,
					       fMaxWaveHeight,
					       fDecayFactor,
					       fAccelFactor,
					       fTransferFactor);

	// render textures
	for(dwCount=0;dwCount<dwNumTex;dwCount++)
	{
		// update surface
		UpdateWaterSurface(lpfCur,
						   lpfNew,
						   lpfSpd,
						   dwTexSize,
						   nDropRatio,
					       fMaxWaveHeight,
					       fDecayFactor,
					       fAccelFactor,
					       fTransferFactor);

		// render surface
		RenderWaterTexture(lpfCur,
						   dwTexIndex+dwCount,
						   dwTexSize,
						   fR1,
						   fG1,
						   fB1,
						   fR2,
						   fG2,
						   fB2,
						   fAMin,
						   fAMax,
						   1);
	}

	// compute number of textures/2
	DWORD dwNumTex2=dwNumTex>>1;
	
	// rotate textures
	for(dwCount=0;dwCount<dwNumTex2;dwCount++)
		SwapTextureSurfaces(dwTexIndex+dwCount,
							dwTexIndex+dwCount+dwNumTex2);

	// update to second skip value
	for(dwCount=0;dwCount<dwSkip2;dwCount++)
		UpdateWaterSurface(lpfCur,
						   lpfNew,
						   lpfSpd,
						   dwTexSize,
						   nDropRatio,
					       fMaxWaveHeight,
					       fDecayFactor,
					       fAccelFactor,
					       fTransferFactor);

	// render textures
	for(dwCount=0;dwCount<dwNumTex;dwCount++)
	{
		// update surface
		UpdateWaterSurface(lpfCur,
						   lpfNew,
						   lpfSpd,
						   dwTexSize,
						   nDropRatio,
					       fMaxWaveHeight,
					       fDecayFactor,
					       fAccelFactor,
					       fTransferFactor);

		// blend value
		float fBlend;

		// compute blend value
		if(dwCount<dwNumTex2)
			fBlend=((float)((dwNumTex2-1)-dwCount))/((float)(dwNumTex2-1));
		else
			fBlend=((float)(dwCount-dwNumTex2))/((float)(dwNumTex2-1));
		
		// render surface
		RenderWaterTexture(lpfCur,
						   dwTexIndex+dwCount,
						   dwTexSize,
						   fR1,
						   fG1,
						   fB1,
						   fR2,
						   fG2,
						   fB2,
						   fAMin,
						   fAMax,
						   1-fBlend);
	}

	// release water surface memory
	FreeMem((LPVOID*)&lpfCur);
	FreeMem((LPVOID*)&lpfNew);
	FreeMem((LPVOID*)&lpfSpd);

	// output water textures
	for(dwCount=0;dwCount<dwNumTex;dwCount++)
	{
		// generate filename
		char lpDestFile[16];
		sprintf(lpDestFile,
				lpFileName,
				dwCount);

		// output image
		CreateDDSFromTexture(lpDestFile,
							 dwTexIndex+dwCount);
	}
	
	// ok
	return(TRUE);
}

// UpdateWaterSurface()						 

void UpdateWaterSurface(float* lpfCur,
						float* lpfNew,
						float* lpfSpd,
						DWORD dwTexSize,
						int nDropRatio,
					    float fMaxWaveHeight,
					    float fDecayFactor,
					    float fAccelFactor,
					    float fTransferFactor)
{
	// counters
	DWORD dwX;
	DWORD dwY;

	// compute texture size mask
	DWORD dwTexSizeMask=dwTexSize-1;

	// compute accel/transfer factor
	float fAccTransFactor=fAccelFactor*fTransferFactor;

	// add drop if necessary
	if((abs(GetRandomInt())%nDropRatio)==1)
	{
		// get random position
		dwX=GetRandomInt()&dwTexSizeMask;
		dwY=GetRandomInt()&dwTexSizeMask;

		// add drop
		lpfCur[dwX+(dwY*dwTexSize)]=GetRandomFloat()*fMaxWaveHeight;
	}

	// update water surface
	for(dwX=0;dwX<dwTexSize;dwX++)
		for(dwY=0;dwY<dwTexSize;dwY++)
		{
			// compute offset values
			DWORD dwXP1=(dwX+1)&dwTexSizeMask;
			DWORD dwXM1=(dwX-1)&dwTexSizeMask;
			DWORD dwYP1=(dwY+1)&dwTexSizeMask;
			DWORD dwYM1=(dwY-1)&dwTexSizeMask;

			// compute position value
			DWORD dwCurPos=dwX+(dwY*dwTexSize);

			// check height of current position
			if(lpfCur[dwCurPos]<0)
			{
				// update speed table
				lpfSpd[dwCurPos]+=fAccelFactor;
				lpfSpd[dwXP1+(dwY*dwTexSize)]+=fAccTransFactor;
				lpfSpd[dwXM1+(dwY*dwTexSize)]+=fAccTransFactor;
				lpfSpd[dwX+(dwYP1*dwTexSize)]+=fAccTransFactor;
				lpfSpd[dwX+(dwYM1*dwTexSize)]+=fAccTransFactor;
			}
			else if(lpfCur[dwX+(dwY*dwTexSize)]>0)
			{
				// update speed table
				lpfSpd[dwCurPos]-=fAccelFactor;
				lpfSpd[dwXP1+(dwY*dwTexSize)]-=fAccTransFactor;
				lpfSpd[dwXM1+(dwY*dwTexSize)]-=fAccTransFactor;
				lpfSpd[dwX+(dwYP1*dwTexSize)]-=fAccTransFactor;
				lpfSpd[dwX+(dwYM1*dwTexSize)]-=fAccTransFactor;
			}

			// compute new height
			lpfNew[dwCurPos]=(lpfCur[dwCurPos]+lpfSpd[dwCurPos])*fDecayFactor;
		}

	// copy new table to current table
	CopyMemory(lpfCur,
			   lpfNew,
			   Squared(dwTexSize)*sizeof(float));
}

// RenderWaterTexture()	

void RenderWaterTexture(float* lpfCur,
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
						float fBlend)
{
	// counters
	DWORD dwX;
	DWORD dwY;

	// compute texture size mask
	DWORD dwTexSizeMask=dwTexSize-1;

	// compute alpha range
	float fARange=fAMax-fAMin;

	// do not allow zero blend
	if(fBlend<EPSILON)
		fBlend=EPSILON;

	// lock texture
	D3DLOCKED_RECT lr=LockTexture(dwTexIndex);

	// extract data pointer & pitch
	LPWORD lpData=(LPWORD)lr.pBits;
	DWORD dwPitch=lr.Pitch>>1;

	// render surface
	for(dwX=0;dwX<dwTexSize;dwX++)
		for(dwY=0;dwY<dwTexSize;dwY++)
		{
			// get original color
			WORD wOldColor=lpData[dwX+(dwY*dwPitch)];

			// extract components
			float fROld=GetR444(wOldColor);
			float fGOld=GetG444(wOldColor);
			float fBOld=GetB444(wOldColor);
			float fAOld=GetA444(wOldColor);

			// compute offset values
			DWORD dwXP1=(dwX+1)&dwTexSizeMask;
			DWORD dwXM1=(dwX-1)&dwTexSizeMask;
			DWORD dwYP1=(dwY+1)&dwTexSizeMask;
			DWORD dwYM1=(dwY-1)&dwTexSizeMask;

			// compute vertices
			D3DXVECTOR3 vC=D3DXVECTOR3(0,lpfCur[dwX+(dwY*dwTexSize)],0);
			D3DXVECTOR3 vL=D3DXVECTOR3(-1,lpfCur[dwXM1+(dwY*dwTexSize)],0);
			D3DXVECTOR3 vR=D3DXVECTOR3(1,lpfCur[dwXP1+(dwY*dwTexSize)],0);
			D3DXVECTOR3 vT=D3DXVECTOR3(0,lpfCur[dwX+(dwYM1*dwTexSize)],-1);
			D3DXVECTOR3 vB=D3DXVECTOR3(0,lpfCur[dwX+(dwYP1*dwTexSize)],1);
			
			// compute plane normals
			D3DXVECTOR3 vN1,vN2,vN3,vN4;
			GetPlaneNormal(&vN1,&vT,&vC,&vR);
			GetPlaneNormal(&vN2,&vR,&vC,&vB);
			GetPlaneNormal(&vN3,&vB,&vC,&vL);
			GetPlaneNormal(&vN4,&vL,&vC,&vT);
			
			// compute average normal
			D3DXVECTOR3 vN=(vN1+vN2+vN3+vN4)*0.25F;
			
			// compute distance from vertical
			float fDV=sqrtf(Squared(vN.x)+Squared(vN.y));

			// compute color values
			float fR=fR1+((fR2-fR1)*fDV);
			float fG=fG1+((fG2-fG1)*fDV);
			float fB=fB1+((fB2-fB1)*fDV);

			// compute alpha value
			float fA=((1-fDV)*fARange)+fAMin;
			
			// interpolate components
			float fRNew=fROld+((fR-fROld)*fBlend);
			float fGNew=fGOld+((fG-fGOld)*fBlend);
			float fBNew=fBOld+((fB-fBOld)*fBlend);
			float fANew=fAOld+((fA-fAOld)*fBlend);

			// set new color
			SetRGBA444(lpData[dwX+(dwY*dwPitch)],
					   fRNew,fGNew,fBNew,fANew);
		}

	// unlock texture
	UnlockTexture(dwTexIndex);
}
#endif

// AddWaterTextures()

DWORD AddWaterTextures(DWORD dwNumTex,
					   LPCSTR lpFileName)
{
	LoadingOutput("AddWaterTextures");

	// counter
	DWORD dwCount;
	
	// texture index
	DWORD dwTexIndex=NO_OBJECT;
	
	// add water textures
	for(dwCount=0;dwCount<dwNumTex;dwCount++)
	{
		// generate filename
		char lpDestFile[16];
		sprintf(lpDestFile,
				lpFileName,
				dwCount);

		// load image
		DWORD dwIndex=AddTexture(lpDestFile);

		// check texture
		if(dwIndex==NO_OBJECT)
			return(NO_OBJECT);

		// check for first texture
		if(dwCount==0)
			dwTexIndex=dwIndex;
	}

	// return index
	return(dwTexIndex);
}

// AddWaterVertexBuffer()

DWORD AddWaterVertexBuffer(void)
{
	LoadingOutput("AddWaterVertexBuffer");

	// water vertex data
	LPWATERVERTEX lpWaterVtx;
	
	// create water vertex buffer
	DWORD dwVtxBuf=AddVertexBuffer(D3DFVF_WATERVERTEX,
								   sizeof(WATERVERTEX),
								   4);

	// check vertex buffer
	if(dwVtxBuf==NO_OBJECT)
		return(NO_OBJECT);

	// lock water vertex buffer
	lpWaterVtx=(LPWATERVERTEX)LockVertexBuffer(dwVtxBuf);

	// check for error
	if(!lpWaterVtx)
		return(NO_OBJECT);

	// fill in vertices
	SET_WATERVERTEX(lpWaterVtx[0],
					-WSURF_RAD,0,-WSURF_RAD,
					-(WSURF_RAD/WSURF_SIZE),-(WSURF_RAD/WSURF_SIZE));
	SET_WATERVERTEX(lpWaterVtx[1],
					WSURF_RAD,0,-WSURF_RAD,
					(WSURF_RAD/WSURF_SIZE),-(WSURF_RAD/WSURF_SIZE));
	SET_WATERVERTEX(lpWaterVtx[2],
					-WSURF_RAD,0,WSURF_RAD,
					-(WSURF_RAD/WSURF_SIZE),(WSURF_RAD/WSURF_SIZE));
	SET_WATERVERTEX(lpWaterVtx[3],
					WSURF_RAD,0,WSURF_RAD,
					(WSURF_RAD/WSURF_SIZE),(WSURF_RAD/WSURF_SIZE));

	// unlock vertex buffer
	UnlockVertexBuffer(dwVtxBuf);

	// return index
	return(dwVtxBuf);
}

// AddWaterIndexBuffer()

DWORD AddWaterIndexBuffer(void)
{
	LoadingOutput("AddWaterIndexBuffer");

	// water index data
	LPWORD lpwWaterIdx;
	
	// create water index buffer
	DWORD dwIdxBuf=AddIndexBuffer(D3DPT_TRIANGLESTRIP,
								  2);

	// check index buffer
	if(dwIdxBuf==NO_OBJECT)
		return(NO_OBJECT);

	// lock water index buffer
	lpwWaterIdx=(LPWORD)LockIndexBuffer(dwIdxBuf);
	
	// check for error
	if(!lpwWaterIdx)
		return(NO_OBJECT);

	// fill in indices
	lpwWaterIdx[0]=0;
	lpwWaterIdx[1]=2;
	lpwWaterIdx[2]=1;
	lpwWaterIdx[3]=3;

	// unlock index buffer
	UnlockIndexBuffer(dwIdxBuf);

	// return index
	return(dwIdxBuf);
}

// GetWaterTexture()

DWORD GetWaterTexture(void)
{
	// texture counter
	static DWORD dwTexCount=0;

#ifdef SHAREWARE_MODE
	// check for nag screen
	if(!g_bNagScreen)
#endif
	{
		// update texture counter
		dwTexCount+=g_dwFrameSkip;
	}

	// compute texture number
	DWORD dwTexNum=(dwTexCount>>WSURF_SLOWDOWN)&(WSURF_NUMTEX-1);

	// return texture number
	return(dwTexNum);
}

// GetWaterPosition()

float GetWaterPosition(void)
{
	// water position
	static float fWaterPos=0;
	
#ifdef SHAREWARE_MODE
	// check for nag screen
	if(!g_bNagScreen)
#endif
	{
		// update water position
		fWaterPos+=(WSURF_MOVEINC*((float)g_dwFrameSkip));
	}
	
	// check for overflow
	while(fWaterPos>WSURF_SIZE)
		fWaterPos-=WSURF_SIZE;

	// return position
	return(fWaterPos);
}

// RenderWaterSurface()

void RenderWaterSurface(void)
{
	// get water texture
	DWORD dwTexNum=GetWaterTexture();

	// get water position
	float fWaterPos=GetWaterPosition();

	// prepare matrix
	D3DXMATRIX mtx=D3DXMATRIX(1,0,0,0,
							  0,1,0,0,
							  0,0,1,0,
							  fWaterPos,0,fWaterPos,1);

	// set matrix
	SetWorldMatrix(&mtx);

	// set material
	SetMaterial(g_dwWaterMat);

	// set texture
	SetTexture(g_dwWaterTex+dwTexNum);

	// set vertex buffer
	SetVertexBuffer(g_dwWaterVtxBuf);

	// set index buffer
	SetIndexBuffer(g_dwWaterIdxBuf);

	// render water
	RenderPrimitive();
}

// SwanLakeUpdate()

void SwanLakeUpdate(void)
{
	// counter
	DWORD dwCount;

#ifdef SHAREWARE_MODE
	// update nag screen
	if(!g_bInitUpdates)
		for(dwCount=0;dwCount<g_dwFrameSkip;dwCount++)
			UpdateNagScreen();

	// check nag screen flag
	if(g_bNagScreen)
		return;

	// check days left
	if(g_dwTrialDays==0)
		return;
#endif

	// update based on frame skip
	for(dwCount=0;dwCount<g_dwFrameSkip;dwCount++)
	{
		// update creatures
		UpdateSwans();
		UpdateBBSwans();
		UpdateTurtles();
		UpdateDragonflies();
		UpdateFish();
		UpdateBirds();

		// update ripples
		UpdateRipples();

		// update camera
		UpdateCameraObject();

		// update sound effects
		if(!g_bInitUpdates)
			UpdateSoundFX();

		// update fade-in value
		if(g_fFadeIn>0.0F)
			g_fFadeIn-=CAMERA_FADEININC;
	}
}

// SwanLakeRender()

void SwanLakeRender(void)
{
	// frame rate display buffer
	char lpFPS[12];

	// clear z-buffer
	ClearZBuffer();

	// set camera to view matrix
	SetCameraObjectToViewMatrix();

	// begin render
	if(BeginScene())
	{
		// render sky
		RenderMesh(g_dwSkyMesh,g_dwSkyMtx);

		// enable fog
		if(g_bFogMode)
			EnableFog();

		// enable smooth shading
		if(g_bShadeSmooth)
			EnableGouraud();

		// render terrain
		RenderMesh(g_dwTerrainMesh,g_dwTerrainMtx);

		// render swans, baby swans, birds & dragonfly bodies
		RenderSwans();
		RenderBBSwans();
		RenderBirds();
		RenderDragonfliesA();

		// enable alpha blend
		EnableAlphaBlend();

		// render turtles
		RenderTurtles();

		// disable fog
		if(g_bFogMode)
			DisableFog();

		// disable smooth shading
		if(g_bShadeSmooth)
			DisableGouraud();

		// enable alpha textures
		EnableAlphaTextures();

		// enable alpha testing
		EnableAlphaTest();

		// render fish
		RenderFish();

		// render ripples
		if(g_bUseRipples)
			RenderRipples();

		// disable alpha testing
		DisableAlphaTest();
		
		// render water surface
		RenderWaterSurface();

		// render lens flare
		if(g_bLensFlareMode)
			RenderLensFlare(&g_lfd,
							SUN_XPOS,
							SUN_YPOS,
							SUN_ZPOS);

		// disable alpha textures
		DisableAlphaTextures();

		// render dragonfly wings
		RenderDragonfliesB();

// uncomment to render boundaries
/*		
static DWORD m=NO_OBJECT;
static DWORD m1=NO_OBJECT;
static DWORD m2=NO_OBJECT;
if(m==NO_OBJECT)
{
	m=AddMeshSphere(TERRAIN_XRANGE,8,8,AddMaterial(1,0,0,0.3F,
												   1,0,0,0.3F,
												   0,0,0,0,
												   0,0,0,0,
												   0));
	m1=AddMatrix(1,0,0,0,
				 0,1,0,0,
				 0,0,1,0,
				 TERRAIN_CENTERX,0,TERRAIN_CENTERZ+TERRAIN_ZRANGE,1);
	m2=AddMatrix(1,0,0,0,
				 0,1,0,0,
				 0,0,1,0,
				 TERRAIN_CENTERX,0,TERRAIN_CENTERZ-TERRAIN_ZRANGE,1);
}
RenderMesh(m,m1);
RenderMesh(m,m2);
*/
		
		// render clock if necessary
		if(g_bShowClock)
			RenderClock();

		// fade screen if necessary
		if(g_fFadeIn>0.0F)
			if(g_fFadeIn>1.0F)
				FadeScreen(D3DCOLOR_COLORVALUE(0.0F,0.0F,0.0F,1.0F));
			else
				FadeScreen(D3DCOLOR_COLORVALUE(0.0F,0.0F,0.0F,g_fFadeIn));

		// disable alpha blend
		DisableAlphaBlend();

#ifdef SHAREWARE_MODE
		// check nag screen flag
		if(g_bNagScreen)
		{
			// render nag screen
			RenderNagScreen();

			// render mouse cursor
			RenderMouseCursor();
		}
#endif

		// end scene
		EndScene();
	}

	// render sound effects
	RenderSoundFX();

	// present scene
	PresentScene();

	// display fps info if necessary
	if(g_bFrameRate)
		if(BeginText(g_dwFPSFont))
		{
			sprintf(lpFPS,"%d",g_dwFPS);
			OutputText(lpFPS,0,0);
#ifdef DEBUG_MODE
			sprintf(lpFPS,"%d",g_dwUPS);
			OutputText(lpFPS,0,16);
#endif
			EndText();
		}
}

// GetNextSwan()

DWORD GetNextSwan(void)
{
	LoadingOutput("GetNextSwan");

	// check counter
	if(g_dwNumSwan==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwSwanMtx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanPriState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanActState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanNavState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanKeyframe1,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanKeyframe2,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfSwanTweenVal,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfSwanTailPos,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfSwanTailDir,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfSwanTransPos,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfSwanTransSpeed,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanTransKey1,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanTransKey2,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfSwanNavSpeed,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanNavPos,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanNavMax,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanMateIdx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanRipple1Idx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSwanRipple2Idx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxSwan=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumSwan>=g_dwMaxSwan)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwSwanMtx,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanPriState,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanActState,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanNavState,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanKeyframe1,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanKeyframe2,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfSwanTweenVal,
					   sizeof(float)*g_dwMaxSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfSwanTailPos,
					   sizeof(float)*g_dwMaxSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfSwanTailDir,
					   sizeof(float)*g_dwMaxSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfSwanTransPos,
					   sizeof(float)*g_dwMaxSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfSwanTransSpeed,
					   sizeof(float)*g_dwMaxSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanTransKey1,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanTransKey2,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfSwanNavSpeed,
					   sizeof(float)*g_dwMaxSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanNavPos,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanNavMax,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanMateIdx,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanRipple1Idx,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSwanRipple2Idx,
					   sizeof(DWORD)*g_dwMaxSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxSwan+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumSwan;

	// increment counter
	g_dwNumSwan++;

	// return index
	return(dwIndex);
}

// AddSwan()

DWORD AddSwan(void)
{
	LoadingOutput("AddSwan");

	// get return index
	DWORD dwIndex=GetNextSwan();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// add swan matrix
	g_lpdwSwanMtx[dwIndex]=AddMatrix();

	// check matrix
	if(g_lpdwSwanMtx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// scale matrix by swan scale
	MatrixScale(g_lpdwSwanMtx[dwIndex],
				SWAN_SCALE+(GetRandomFloat()*SWAN_SCALEVAR));

	// set default values
	g_lpdwSwanPriState[dwIndex]=PRST_SWAN_REST;
	g_lpdwSwanActState[dwIndex]=ACST_SWAN_JUSTSWIM;
	g_lpdwSwanNavState[dwIndex]=NVST_SWAN_JUSTSWIM;
	g_lpdwSwanKeyframe1[dwIndex]=KEYF_SWAN_HCTR;
	g_lpdwSwanKeyframe2[dwIndex]=KEYF_SWAN_HCTL;
	g_lpfSwanTweenVal[dwIndex]=0;
	g_lpfSwanTailPos[dwIndex]=0;
	g_lpfSwanTailDir[dwIndex]=SWAN_TAILSPEED;
	g_lpfSwanTransPos[dwIndex]=0;
	g_lpfSwanTransSpeed[dwIndex]=0;
	g_lpdwSwanTransKey1[dwIndex]=NO_OBJECT;
	g_lpdwSwanTransKey2[dwIndex]=NO_OBJECT;
	g_lpfSwanNavSpeed[dwIndex]=0;
	g_lpdwSwanNavPos[dwIndex]=0;
	g_lpdwSwanNavMax[dwIndex]=0;
	g_lpdwSwanMateIdx[dwIndex]=NO_OBJECT;
	
	// add ripple #1
	g_lpdwSwanRipple1Idx[dwIndex]=AddRipple();

	// check ripple #1
	if(g_lpdwSwanRipple1Idx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// add ripple #2
	g_lpdwSwanRipple2Idx[dwIndex]=AddRipple();

	// check ripple #2
	if(g_lpdwSwanRipple2Idx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);


	// return index
	return(dwIndex);
}

// SetSwanMatrix()

void SetSwanMatrix(DWORD dwIndex,
				   LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumSwan)
	{
		DisplayFatalErrorMessage("Invalid swan index.",
								 "SetSwanMatrix()",
								 FALSE);
		return;
	}
#endif

	// set matrix
	SetMatrix(g_lpdwSwanMtx[dwIndex],
			  lpMtx);
}

// PurgeSwans()

void PurgeSwans(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpdwSwanMtx);
	FreeMem((LPVOID*)&g_lpdwSwanPriState);
	FreeMem((LPVOID*)&g_lpdwSwanActState);
	FreeMem((LPVOID*)&g_lpdwSwanNavState);
	FreeMem((LPVOID*)&g_lpdwSwanKeyframe1);
	FreeMem((LPVOID*)&g_lpdwSwanKeyframe2);
	FreeMem((LPVOID*)&g_lpfSwanTweenVal);
	FreeMem((LPVOID*)&g_lpfSwanTailPos);
	FreeMem((LPVOID*)&g_lpfSwanTailDir);
	FreeMem((LPVOID*)&g_lpfSwanTransPos);
	FreeMem((LPVOID*)&g_lpfSwanTransSpeed);
	FreeMem((LPVOID*)&g_lpdwSwanTransKey1);
	FreeMem((LPVOID*)&g_lpdwSwanTransKey2);
	FreeMem((LPVOID*)&g_lpfSwanNavSpeed);
	FreeMem((LPVOID*)&g_lpdwSwanNavPos);
	FreeMem((LPVOID*)&g_lpdwSwanNavMax);
	FreeMem((LPVOID*)&g_lpdwSwanMateIdx);
	FreeMem((LPVOID*)&g_lpdwSwanRipple1Idx);
	FreeMem((LPVOID*)&g_lpdwSwanRipple2Idx);

	// reset counters
	g_dwNumSwan=0;
	g_dwMaxSwan=0;
}

// RenderSwans()

void RenderSwans(void)
{
	// swan index
	DWORD dwIndex;

	// check "phantom" swan flag
	if(g_bPhantomSwan)
		return;

#ifndef REFLECTION_OPTIMIZATION
	// render swans
	for(dwIndex=0;dwIndex<g_dwNumSwan;dwIndex++)
	{
		// set keyframe
		UpdateRenderFrames(g_dwSwanAnim,
						   g_lpdwSwanKeyframe1[dwIndex],
						   g_lpdwSwanKeyframe2[dwIndex],
						   g_lpfSwanTweenVal[dwIndex]);

		// render swan
		RenderAnimation(g_dwSwanAnim,
						g_lpdwSwanMtx[dwIndex]);
	}

	// enable frontface cull
	EnableFrontfaceCull();
	
	// render swan reflections
	for(dwIndex=0;dwIndex<g_dwNumSwan;dwIndex++)
	{
		// invert y-axis
		g_lpMtx[g_lpdwSwanMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwSwanMtx[dwIndex]].fYScale;

		// set keyframe
		UpdateRenderFrames(g_dwSwanAnim,
						   g_lpdwSwanKeyframe1[dwIndex],
						   g_lpdwSwanKeyframe2[dwIndex],
						   g_lpfSwanTweenVal[dwIndex]);
		
		// render swan
		RenderAnimation(g_dwSwanAnim,
						g_lpdwSwanMtx[dwIndex]);
		
		// reset y-axis
		g_lpMtx[g_lpdwSwanMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwSwanMtx[dwIndex]].fYScale;
	}
	
	// enable backface cull
	EnableBackfaceCull();
#else
	// render swans & reflections
	for(dwIndex=0;dwIndex<g_dwNumSwan;dwIndex++)
	{
		// set keyframe
		UpdateRenderFrames(g_dwSwanAnim,
						   g_lpdwSwanKeyframe1[dwIndex],
						   g_lpdwSwanKeyframe2[dwIndex],
						   g_lpfSwanTweenVal[dwIndex]);

		// render swan
		RenderAnimation(g_dwSwanAnim,
						g_lpdwSwanMtx[dwIndex]);

		// enable frontface cull
		EnableFrontfaceCull();
		
		// invert y-axis
		g_lpMtx[g_lpdwSwanMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwSwanMtx[dwIndex]].fYScale;
		
		// render swan
		RenderAnimation(g_dwSwanAnim,
						g_lpdwSwanMtx[dwIndex]);
		
		// reset y-axis
		g_lpMtx[g_lpdwSwanMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwSwanMtx[dwIndex]].fYScale;

		// enable backface cull
		EnableBackfaceCull();
	}
#endif
}

// AddSwanLoner()

DWORD AddSwanLoner(void)
{
	LoadingOutput("AddSwanLoner");

	// swan index
	DWORD dwIndex;

	// polar coordinate values
	float fAngle;
	float fDist;
	
	// position values
	float fX;
	float fZ;

	// counter
	DWORD dwCount;

	// done flag
	BOOL bDone;
	
	// add swan
	dwIndex=AddSwan();

	// check swan
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// reset done flag
	bDone=FALSE;

	// loop until suitable position is found
	while(!bDone)
	{
		// create random position
		switch(abs(GetRandomInt())%3)
		{
		// upper round portion of boundary
		case(0):
			// set polar coordinates
			fAngle=fabsf(GetRandomFloat()*PI);
			fDist=fabsf(GetRandomFloat()*TERRAIN_XRANGE);
			
			// set position
			fX=TERRAIN_CENTERX-(cosf(fAngle)*fDist);
			fZ=(TERRAIN_CENTERZ-TERRAIN_ZRANGE)-(sinf(fAngle)*fDist);

			// ok
			break;

		// center rectangular portion of boundary
		case(1):
			// set position
			fX=TERRAIN_CENTERX+(GetRandomFloat()*TERRAIN_XRANGE);
			fZ=TERRAIN_CENTERZ+(GetRandomFloat()*TERRAIN_ZRANGE);

			// ok
			break;

		// lower round portion of boundary
		case(2):
			// set polar coordinates
			fAngle=fabsf(GetRandomFloat()*PI);
			fDist=fabsf(GetRandomFloat()*TERRAIN_XRANGE);
			
			// set position
			fX=TERRAIN_CENTERX+(cosf(fAngle)*fDist);
			fZ=(TERRAIN_CENTERZ+TERRAIN_ZRANGE)+(sinf(fAngle)*fDist);

			// ok
			break;
		}

		// assume that we will be done
		bDone=TRUE;
		
		// compare position with other swans
		for(dwCount=0;dwCount<g_dwNumSwan;dwCount++)
		{
			// skip this swan
			if(dwCount!=dwIndex)
			{
				// get swan position
				float fSwanX=g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._41;
				float fSwanZ=g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._43;

				// compare distance with separation distance
				if(SquareMagnitudeXZ(fSwanX-fX,fSwanZ-fZ)<Squared(SWAN_SEPARATIONDISTANCE))
				{
					// reset done flag
					bDone=FALSE;

					// end loop
					dwCount=g_dwNumSwan;
				}
			}
		}
	}

	// set swan position
	g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._41=fX;
	g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._43=fZ;

	// randomize yaw
	for(dwCount=0;dwCount<8;dwCount++)
		MatrixYaw(g_lpdwSwanMtx[dwIndex],
				  fabsf(GetRandomFloat()));

	// set primary state
	g_lpdwSwanPriState[dwIndex]=(DWORD)(abs(GetRandomInt())%SWAN_NUMPRIMARYSTATES);

	// return index
	return(dwIndex);
}

// AddSwanCouple()

DWORD AddSwanCouple(void)
{
	LoadingOutput("AddSwanCouple");

	// swan indices
	DWORD dwIndex1;
	DWORD dwIndex2;

	// counter
	DWORD dwCount;

	// add swan #1
	dwIndex1=AddSwanLoner();

	// check swan #1
	if(dwIndex1==NO_OBJECT)
		return(NO_OBJECT);

	// add swan #1
	dwIndex2=AddSwan();

	// check swan #2
	if(dwIndex2==NO_OBJECT)
		return(NO_OBJECT);

	// set mate values
	g_lpdwSwanMateIdx[dwIndex1]=dwIndex2;
	g_lpdwSwanMateIdx[dwIndex2]=dwIndex1;

	// set primary state & position of swan #2
	switch(g_lpdwSwanPriState[dwIndex1])
	{
	// swan #1 leading
	case(PRST_SWAN_LEAD):
		// set swan #2 to follow
		g_lpdwSwanPriState[dwIndex2]=PRST_SWAN_FOLLOW;

		// copy swan #1's position to swan #2
		g_lpMtx[g_lpdwSwanMtx[dwIndex2]].mtx._41=g_lpMtx[g_lpdwSwanMtx[dwIndex1]].mtx._41;
		g_lpMtx[g_lpdwSwanMtx[dwIndex2]].mtx._42=g_lpMtx[g_lpdwSwanMtx[dwIndex1]].mtx._42;
		g_lpMtx[g_lpdwSwanMtx[dwIndex2]].mtx._43=g_lpMtx[g_lpdwSwanMtx[dwIndex1]].mtx._43;

		// randomize yaw of swan #2
		for(dwCount=0;dwCount<8;dwCount++)
			MatrixYaw(g_lpdwSwanMtx[dwIndex2],
					  fabsf(GetRandomFloat()));

		// move swan #2 back by follow distance
		MatrixAhead(g_lpdwSwanMtx[dwIndex2],
					SWAN_MAXFOLLOWDISTANCE);

		// ok
		break;

	// swan #1 following
	case(PRST_SWAN_FOLLOW):
		// set swan #2 to lead
		g_lpdwSwanPriState[dwIndex2]=PRST_SWAN_LEAD;

		// copy swan #1's position to swan #2
		g_lpMtx[g_lpdwSwanMtx[dwIndex2]].mtx._41=g_lpMtx[g_lpdwSwanMtx[dwIndex1]].mtx._41;
		g_lpMtx[g_lpdwSwanMtx[dwIndex2]].mtx._42=g_lpMtx[g_lpdwSwanMtx[dwIndex1]].mtx._42;
		g_lpMtx[g_lpdwSwanMtx[dwIndex2]].mtx._43=g_lpMtx[g_lpdwSwanMtx[dwIndex1]].mtx._43;

		// randomize yaw of swan #2
		for(dwCount=0;dwCount<8;dwCount++)
			MatrixYaw(g_lpdwSwanMtx[dwIndex2],
					  fabsf(GetRandomFloat()));

		// move swan #1 back by follow distance
		MatrixAhead(g_lpdwSwanMtx[dwIndex1],
					SWAN_MAXFOLLOWDISTANCE);

		// ok
		break;

	// swan #1 resting
	case(PRST_SWAN_REST):
		// set swan #2 to rest
		g_lpdwSwanPriState[dwIndex2]=PRST_SWAN_REST;

		// copy swan #1's position to swan #2
		g_lpMtx[g_lpdwSwanMtx[dwIndex2]].mtx._41=g_lpMtx[g_lpdwSwanMtx[dwIndex1]].mtx._41;
		g_lpMtx[g_lpdwSwanMtx[dwIndex2]].mtx._42=g_lpMtx[g_lpdwSwanMtx[dwIndex1]].mtx._42;
		g_lpMtx[g_lpdwSwanMtx[dwIndex2]].mtx._43=g_lpMtx[g_lpdwSwanMtx[dwIndex1]].mtx._43;

		// randomize yaw of swan #2
		for(dwCount=0;dwCount<8;dwCount++)
			MatrixYaw(g_lpdwSwanMtx[dwIndex2],
					  fabsf(GetRandomFloat()));

		// move swan #2 back by follow distance
		MatrixAhead(g_lpdwSwanMtx[dwIndex2],
					SWAN_MAXFOLLOWDISTANCE);

		// ok
		break;
	}

	// return index #1
	return(dwIndex1);
}

// RandomizeSwanPrimaryState()

void RandomizeSwanPrimaryState(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumSwan)
	{
		DisplayFatalErrorMessage("Invalid swan index.",
								 "RandomizeSwanPrimaryState()",
								 FALSE);
		return;
	}
#endif

	// set new state
	g_lpdwSwanPriState[dwIndex]=(DWORD)(abs(GetRandomInt())%SWAN_NUMPRIMARYSTATES);

	// check for mate
	if(g_lpdwSwanMateIdx[dwIndex]!=NO_OBJECT)
	{
		// set mate's state accordingly
		switch(g_lpdwSwanPriState[dwIndex])
		{
		// this swan leading
		case(PRST_SWAN_LEAD):
			// set mate to follow
			g_lpdwSwanPriState[g_lpdwSwanMateIdx[dwIndex]]=PRST_SWAN_FOLLOW;

			// ok
			break;

		// this swan following
		case(PRST_SWAN_FOLLOW):
			// set mate to lead
			g_lpdwSwanPriState[g_lpdwSwanMateIdx[dwIndex]]=PRST_SWAN_LEAD;

			// ok
			break;

		// this swan resting
		case(PRST_SWAN_REST):
			// set mate to rest
			g_lpdwSwanPriState[g_lpdwSwanMateIdx[dwIndex]]=PRST_SWAN_REST;

			// ok
			break;
		}
	}
}

// IsPointOutOfUpperBounds()

BOOL IsPointOutOfUpperBounds(float fX,
							 float fZ)
{
	// distance value
	float fDist;
	
	// check z value
	if(fZ<=(TERRAIN_CENTERZ-TERRAIN_ZRANGE))
	{
		// compute distance to center of boundary
		fDist=SquareMagnitudeXZ(fX-TERRAIN_CENTERX,
							    fZ-(TERRAIN_CENTERZ-TERRAIN_ZRANGE));

		// check distance
		if(fDist>Squared(TERRAIN_XRANGE))
			return(TRUE);
	}

	// ok
	return(FALSE);
}

// IsPointOutOfCenterBounds()

BOOL IsPointOutOfCenterBounds(float fX,
							  float fZ)
{
	// check z value
	if(fZ>=(TERRAIN_CENTERZ-TERRAIN_ZRANGE)&&
	   fZ<=(TERRAIN_CENTERZ+TERRAIN_ZRANGE))
	{
		// check x value
		if(fX<=(TERRAIN_CENTERX-TERRAIN_XRANGE)||
		   fX>=(TERRAIN_CENTERX+TERRAIN_XRANGE))
			return(TRUE);
	}

	// ok
	return(FALSE);
}

// IsPointOutOfLowerBounds()

BOOL IsPointOutOfLowerBounds(float fX,
 							 float fZ)
{
	// distance value
	float fDist;
	
	// check z value
	if(fZ>=(TERRAIN_CENTERZ+TERRAIN_ZRANGE))
	{
		// compute distance to center of boundary
		fDist=SquareMagnitudeXZ(fX-TERRAIN_CENTERX,
							    fZ-(TERRAIN_CENTERZ+TERRAIN_ZRANGE));

		// check distance
		if(fDist>Squared(TERRAIN_XRANGE))
			return(TRUE);
	}

	// ok
	return(FALSE);
}

// IsPointOutOfBounds()

BOOL IsPointOutOfBounds(float fX,
 						float fZ)
{
	// check each boundary
	if(IsPointOutOfUpperBounds(fX,fZ))
		return(TRUE);
	if(IsPointOutOfLowerBounds(fX,fZ))
		return(TRUE);
	if(IsPointOutOfCenterBounds(fX,fZ))
		return(TRUE);
	
	// ok
	return(FALSE);
}

// UpdateSwans()

void UpdateSwans(void)
{
	// alternating counter
	static DWORD dwAltCount=0;

	// swan index
	DWORD dwIndex;

	// mate index
	DWORD dwMateIndex;

	// navigation update flag
	BOOL bNavFlag;

	// vectors for comparisons
	D3DXVECTOR3 vV1;
	D3DXVECTOR3 vV2;
	D3DXVECTOR3 vV3;

	// distance value
	float fDist;

	// tracking value
	float fTrackingVal;

	// coordinate values
	float fX;
	float fZ;

	// boundary flags
	BOOL bUpper;
	BOOL bLower;
	BOOL bCenter;

	// counter
	DWORD dwCount;

	// update alternating counter
	dwAltCount=(dwAltCount+1)&1;

	// update swans
	for(dwIndex=0;dwIndex<g_dwNumSwan;dwIndex++)
	{
		// get mate index
		dwMateIndex=g_lpdwSwanMateIdx[dwIndex];

		// set coordinate values
		fX=g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._41;
		fZ=g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._43;

		// compare swan position with other swans
		for(dwCount=0;dwCount<g_dwNumSwan;dwCount++)
		{
			// skip this swan
			if(dwCount!=dwIndex)
			{
				// skip this swan's mate
				if(dwCount!=dwMateIndex)
				{
					// set up vector
					vV1.x=fX-g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._41;
					vV1.z=fZ-g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._43;

					// save distance
					fDist=SquareMagnitudeXZ(&vV1);

					// check distance
					if(fDist<Squared(SWAN_SEPARATIONDISTANCE))
					{
						// compute distance
						float fInvDist=1.0F/sqrtf(fDist);
						
						// normalize vector
						vV2.x=vV1.x*fInvDist;
						vV2.y=0;
						vV2.z=vV1.z*fInvDist;

						// set new position
						g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._41+=vV2.x*g_lpfSwanNavSpeed[dwCount];
						g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._43+=vV2.z*g_lpfSwanNavSpeed[dwCount];
					}
				}
			}
		}

		// check primary state
		switch(g_lpdwSwanPriState[dwIndex])
		{
		// lead
		case(PRST_SWAN_LEAD):
			// get boundary flags
			bUpper=IsPointOutOfUpperBounds(fX,fZ);
			bLower=IsPointOutOfLowerBounds(fX,fZ);
			bCenter=IsPointOutOfCenterBounds(fX,fZ);

			// check boundaries
			if(bUpper||bLower||bCenter)
			{
				// get vector from center of boundary to swan
				if(bUpper)
				{
					// center of upper boundary
					vV2=D3DXVECTOR3(fX-TERRAIN_CENTERX,
									0,
									fZ-(TERRAIN_CENTERZ-TERRAIN_ZRANGE));
				}
				else if(bLower)
				{
					// center of lower boundary
					vV2=D3DXVECTOR3(fX-TERRAIN_CENTERX,
									0,
									fZ-(TERRAIN_CENTERZ+TERRAIN_ZRANGE));
				}
				else
				{
					// center of center boundary
					vV2=D3DXVECTOR3(fX-TERRAIN_CENTERX,
									0,
									fZ-TERRAIN_CENTERZ);
				}

				// normalize vector
				Normalize(&vV1,&vV2);

				// get perpendicular vector
				vV2.x=vV1.z;
				vV2.y=0;
				vV2.z=-vV1.x;

				// get swan ahead vector
				vV3=D3DXVECTOR3(g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._31,
								0,
								g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._33);

				// check cosine of angle
				if(DotProduct(&vV2,&vV3)>0)
				{
					// set state to hard right
					g_lpdwSwanNavState[dwIndex]=NVST_SWAN_HARDRIGHT;
				}
				else
				{
					// set state to hard left
					g_lpdwSwanNavState[dwIndex]=NVST_SWAN_HARDLEFT;
				}

				// check cosine of angle
				if(DotProduct(&vV1,&vV3)<0)
				{
					// check speed
					if(g_lpfSwanNavSpeed[dwIndex]>0)
					{
						// check alternating counter
						if(dwAltCount)
						{
							// set state to slow down
							g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SLOWDOWN;
						}
					}
				}
				else
				{
					// check speed
					if(g_lpfSwanNavSpeed[dwIndex]<(SWAN_MAXSPEED*0.25F))
					{
						// check alternating counter
						if(dwAltCount)
						{
							// set state to speed up
							g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SPEEDUP;
						}
					}
				}

				// set navigation values
				g_lpdwSwanNavPos[dwIndex]=0;
				g_lpdwSwanNavMax[dwIndex]=2;
			}
			else
			{
				// check speed
				if(g_lpfSwanNavSpeed[dwIndex]<(SWAN_MAXSPEED*0.25F))
				{
					// set state to speed up
					g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SPEEDUP;

					// set navigation values
					g_lpdwSwanNavPos[dwIndex]=0;
					g_lpdwSwanNavMax[dwIndex]=2;
				}
				else
				{
					// check navigation state
					if(g_lpdwSwanNavState[dwIndex]==NVST_SWAN_JUSTSWIM)
					{
						// change navigation state if necessary
						if((abs(GetRandomInt())%SWAN_NAVIGATIONCHANGERATIO)==1)
						{
							// set new state
							g_lpdwSwanNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%SWAN_NUMNAVIGATIONSTATES);

							// set navigation values
							g_lpdwSwanNavPos[dwIndex]=0;
							g_lpdwSwanNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%SWAN_NAVIGATIONMAXINC)+1);
						}
					}

					// check for mate
					if(dwMateIndex!=NO_OBJECT)
					{
						// get swan position
						vV1=D3DXVECTOR3(g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._41,
										0,
										g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._43);

						// get mate position
						vV2=D3DXVECTOR3(g_lpMtx[g_lpdwSwanMtx[dwMateIndex]].mtx._41,
										0,
										g_lpMtx[g_lpdwSwanMtx[dwMateIndex]].mtx._43);

						// get distance to mate
						fDist=SquareMagnitudeXZ(vV1.x-vV2.x,
											    vV1.z-vV2.z);

						// check distance
						if(fDist>Squared(SWAN_MAXFOLLOWDISTANCE))
						{
							// check speed
							if(g_lpfSwanNavSpeed[dwIndex]>(SWAN_MAXSPEED*0.5F))
							{
								// set state to slow down
								g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SLOWDOWN;

								// set navigation values
								g_lpdwSwanNavPos[dwIndex]=0;
								g_lpdwSwanNavMax[dwIndex]=2;
							}
						}
					}
				}

				// change primary state if necessary
				if((abs(GetRandomInt())%SWAN_PRIMARYCHANGERATIO)==1)
					RandomizeSwanPrimaryState(dwIndex);
			}

			// ok
			break;

		// follow
		case(PRST_SWAN_FOLLOW):
			// single swans must be leaders
			if(dwMateIndex==NO_OBJECT)
				g_lpdwSwanPriState[dwIndex]=PRST_SWAN_LEAD;
			else
			{
				// get swan position
				vV1=D3DXVECTOR3(g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._41,
								0,
								g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._43);

				// get mate position
				vV2=D3DXVECTOR3(g_lpMtx[g_lpdwSwanMtx[dwMateIndex]].mtx._41,
								0,
								g_lpMtx[g_lpdwSwanMtx[dwMateIndex]].mtx._43);

				// get distance to mate
				fDist=SquareMagnitudeXZ(vV1.x-vV2.x,
									    vV1.z-vV2.z);

				// check distance to mate
				if(fDist<Squared(SWAN_MAXFOLLOWDISTANCE))
				{
					// check distance to mate
					if(fDist<Squared(SWAN_MINSWANDISTANCE))
					{
						// get vector from mate to swan
						vV3=vV1-vV2;

						// normalize vector
						Normalize(&vV1,&vV3);

						// set new position
						g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._41+=vV1.x*g_lpfSwanNavSpeed[dwMateIndex];
						g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._43+=vV1.z*g_lpfSwanNavSpeed[dwMateIndex];
					}
					else
					{
						// check speed
						if(g_lpfSwanNavSpeed[dwIndex]>(g_lpfSwanNavSpeed[dwMateIndex]*SWAN_TRACKINGSPEEDVAL)+SWAN_TRACKINGSPEEDINC)
						{
							// set state to slow down
							g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SLOWDOWN;

							// set navigation values
							g_lpdwSwanNavPos[dwIndex]=0;
							g_lpdwSwanNavMax[dwIndex]=2;
						}
						else if(g_lpfSwanNavSpeed[dwIndex]<(g_lpfSwanNavSpeed[dwMateIndex]*SWAN_TRACKINGSPEEDVAL1)-SWAN_TRACKINGSPEEDINC1)
						{
							// set state to speed up
							g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SPEEDUP;

							// set navigation values
							g_lpdwSwanNavPos[dwIndex]=0;
							g_lpdwSwanNavMax[dwIndex]=2;
						}
						else
						{
							// check navigation state
							if(g_lpdwSwanNavState[dwIndex]==NVST_SWAN_JUSTSWIM)
							{
								// change navigation state if necessary
								if((abs(GetRandomInt())%SWAN_NAVIGATIONCHANGERATIO)==1)
								{
									// set new state
									g_lpdwSwanNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%SWAN_NUMNAVIGATIONSTATES);

									// set navigation values
									g_lpdwSwanNavPos[dwIndex]=0;
									g_lpdwSwanNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%SWAN_NAVIGATIONMAXINC)+1);
								}
							}
						}
  					}
				}
				else
				{
					// get vector from swan to mate
					vV3=vV2-vV1;

					// normalize vector
					Normalize(&vV2,&vV3);
					
					// get swan ahead vector
					vV1=D3DXVECTOR3(g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._31,
									0,
									g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._33);

					// check cosine of angle
					if(DotProduct(&vV1,&vV2)>0)
					{
						// get vector perpendicular to ahead vector
						vV3.x=vV1.z;
						vV3.y=0;
						vV3.z=-vV1.x;
 
						// check cosine of angle
						if(DotProduct(&vV3,&vV2)>0)
						{
							// set state to hard right
							g_lpdwSwanNavState[dwIndex]=NVST_SWAN_HARDRIGHT;
						}
						else
						{
							// set state to hard left
							g_lpdwSwanNavState[dwIndex]=NVST_SWAN_HARDLEFT;
						}

						// check speed
						if(g_lpfSwanNavSpeed[dwIndex]>0)
						{
							// check alternating counter
							if(dwAltCount)
							{
								// set state to slow down
								g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SLOWDOWN;
							}
						}

						// set navigation values
						g_lpdwSwanNavPos[dwIndex]=0;
						g_lpdwSwanNavMax[dwIndex]=2;
					}
					else
					{
						// get vector perpendicular to ahead vector
						vV3.x=vV1.z;
						vV3.y=0;
						vV3.z=-vV1.x;

						// save tracking value
						fTrackingVal=DotProduct(&vV3,&vV2);

						// check tracking value
						if(fabsf(fTrackingVal)>SWAN_FOLLOWTRACKINGVAL)
						{
							// check tracking value
							if(fTrackingVal>0)
							{
								// set state to hard right
								g_lpdwSwanNavState[dwIndex]=NVST_SWAN_HARDRIGHT;
							}
							else
							{
								// set state to hard left
								g_lpdwSwanNavState[dwIndex]=NVST_SWAN_HARDLEFT;
							}

							// check speed
							if(g_lpfSwanNavSpeed[dwIndex]<((g_lpfSwanNavSpeed[dwMateIndex]*SWAN_TRACKINGSPEEDVAL)+SWAN_TRACKINGSPEEDINC)-SWAN_SPEEDTOLERANCE)
							{
								// check alternating counter
								if(dwAltCount)
								{
									// set state to speed up
									g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SPEEDUP;
								}
							}
							else if(g_lpfSwanNavSpeed[dwIndex]>((g_lpfSwanNavSpeed[dwMateIndex]*SWAN_TRACKINGSPEEDVAL)+SWAN_TRACKINGSPEEDINC)+SWAN_SPEEDTOLERANCE)
							{
								// check alternating counter
								if(dwAltCount)
								{
									// set state to slow down
									g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SLOWDOWN;
								}
							}

							// set navigation values
							g_lpdwSwanNavPos[dwIndex]=0;
							g_lpdwSwanNavMax[dwIndex]=2;
						}
						else
						{
							// check distance to mate
							if(fDist>Squared(SWAN_MAXFOLLOWDISTANCE))
							{
								// check speed
								if(g_lpfSwanNavSpeed[dwIndex]<((g_lpfSwanNavSpeed[dwMateIndex]*SWAN_TRACKINGSPEEDVAL2)+SWAN_TRACKINGSPEEDINC2)-SWAN_SPEEDTOLERANCE)
								{
									// set state to speed up
									g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SPEEDUP;

									// set navigation values
									g_lpdwSwanNavPos[dwIndex]=0;
									g_lpdwSwanNavMax[dwIndex]=2;
								}
								else if(g_lpfSwanNavSpeed[dwIndex]>((g_lpfSwanNavSpeed[dwMateIndex]*SWAN_TRACKINGSPEEDVAL2)+SWAN_TRACKINGSPEEDINC2)+SWAN_SPEEDTOLERANCE)
								{
									// set state to slow down
									g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SLOWDOWN;

									// set navigation values
									g_lpdwSwanNavPos[dwIndex]=0;
									g_lpdwSwanNavMax[dwIndex]=2;
								}
							}
						}
					}
				}
			}

			// ok
			break;

		// rest
		case(PRST_SWAN_REST):
			// check for mate
			if(dwMateIndex!=NO_OBJECT)
			{
				// get swan position
				vV1=D3DXVECTOR3(g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._41,
								0,
								g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._43);

				// get mate position
				vV2=D3DXVECTOR3(g_lpMtx[g_lpdwSwanMtx[dwMateIndex]].mtx._41,
								0,
								g_lpMtx[g_lpdwSwanMtx[dwMateIndex]].mtx._43);

				// get distance to mate
				fDist=SquareMagnitudeXZ(vV1.x-vV2.x,
									    vV1.z-vV2.z);

				// check distance to mate
				if(fDist<Squared(SWAN_MINSWANDISTANCE))
				{
					// get vector from mate to swan
					vV3=vV1-vV2;

					// normalize vector
					Normalize(&vV1,&vV3);

					// set new position
					g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._41+=vV1.x*g_lpfSwanNavSpeed[dwMateIndex];
					g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._43+=vV1.z*g_lpfSwanNavSpeed[dwMateIndex];
				}
				else if(fDist>Squared(SWAN_MAXRESTDISTANCE))
				{
					// set swan to lead
					g_lpdwSwanPriState[dwIndex]=PRST_SWAN_LEAD;

					// set mate to follow if necessary
					if(dwMateIndex!=NO_OBJECT)
						g_lpdwSwanPriState[dwMateIndex]=PRST_SWAN_FOLLOW;
				}
			}

			// check boundaries
			if(IsPointOutOfBounds(fX,fZ))
			{
				// set swan to lead
				g_lpdwSwanPriState[dwIndex]=PRST_SWAN_LEAD;

				// set mate to follow if necessary
				if(dwMateIndex!=NO_OBJECT)
					g_lpdwSwanPriState[dwMateIndex]=PRST_SWAN_FOLLOW;
			}
			else
			{
				// check speed
				if(g_lpfSwanNavSpeed[dwIndex]>SWAN_MAXRESTSPEED)
				{
					// set state to slow down
					g_lpdwSwanNavState[dwIndex]=NVST_SWAN_SLOWDOWN;

					// set navigation values
					g_lpdwSwanNavPos[dwIndex]=0;
					g_lpdwSwanNavMax[dwIndex]=2;
				}
				else
				{
					// check navigation state
					if(g_lpdwSwanNavState[dwIndex]==NVST_SWAN_JUSTSWIM)
					{
						// change navigation state if necessary
						if((abs(GetRandomInt())%SWAN_NAVIGATIONCHANGERATIO)==1)
						{
							// set new state
							g_lpdwSwanNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%SWAN_NUMNAVIGATIONSTATES);

							// set navigation values
							g_lpdwSwanNavPos[dwIndex]=0;
							g_lpdwSwanNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%SWAN_NAVIGATIONMAXINC)+1);
						}
					}
				}

				// change primary state if necessary
				if((abs(GetRandomInt())%SWAN_PRIMARYCHANGERATIO)==1)
					RandomizeSwanPrimaryState(dwIndex);
			}

			// ok
			break;
		}

		// check navigation state
		if(g_lpdwSwanNavState[dwIndex]!=NVST_SWAN_JUSTSWIM)
		{
			// check navigation position
			if(g_lpdwSwanNavPos[dwIndex]<g_lpdwSwanNavMax[dwIndex])
			{
				// set flag
				bNavFlag=TRUE;

				// increment navigation position
				g_lpdwSwanNavPos[dwIndex]++;
			}
			else
			{
				// reset flag
				bNavFlag=FALSE;
			}
		}

		// check navigation state
		switch(g_lpdwSwanNavState[dwIndex])
		{
		// speed up
		case(NVST_SWAN_SPEEDUP):
			// increase speed
			g_lpfSwanNavSpeed[dwIndex]+=SWAN_SPEEDINC;

			// check for overflow
			if(g_lpfSwanNavSpeed[dwIndex]>SWAN_MAXSPEED)
				g_lpfSwanNavSpeed[dwIndex]=SWAN_MAXSPEED;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwSwanNavState[dwIndex]=NVST_SWAN_JUSTSWIM;
			}

			// ok
			break;

		// slow down
		case(NVST_SWAN_SLOWDOWN):
			// decrease speed
			g_lpfSwanNavSpeed[dwIndex]-=SWAN_SPEEDINC;

			// check for overflow
			if(g_lpfSwanNavSpeed[dwIndex]<0)
				g_lpfSwanNavSpeed[dwIndex]=0;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwSwanNavState[dwIndex]=NVST_SWAN_JUSTSWIM;
			}

			// ok
			break;

		// turn left
		case(NVST_SWAN_TURNLEFT):
			// turn left
			MatrixYaw(g_lpdwSwanMtx[dwIndex],
					  SWAN_TURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwSwanNavState[dwIndex]=NVST_SWAN_JUSTSWIM;
			}

			// ok
			break;

		// turn right
		case(NVST_SWAN_TURNRIGHT):
			// turn right
			MatrixYaw(g_lpdwSwanMtx[dwIndex],
					  -SWAN_TURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwSwanNavState[dwIndex]=NVST_SWAN_JUSTSWIM;
			}

			// ok
			break;

		// hard left
		case(NVST_SWAN_HARDLEFT):
			// turn left
			MatrixYaw(g_lpdwSwanMtx[dwIndex],
					  SWAN_HARDTURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwSwanNavState[dwIndex]=NVST_SWAN_JUSTSWIM;
			}

			// ok
			break;

		// hard right
		case(NVST_SWAN_HARDRIGHT):
			// turn right
			MatrixYaw(g_lpdwSwanMtx[dwIndex],
					  -SWAN_HARDTURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwSwanNavState[dwIndex]=NVST_SWAN_JUSTSWIM;
			}

			// ok
			break;
		}

		// move swan
		MatrixBack(g_lpdwSwanMtx[dwIndex],
				   g_lpfSwanNavSpeed[dwIndex]);

		// update tail
		g_lpfSwanTailPos[dwIndex]+=g_lpfSwanTailDir[dwIndex]+g_lpfSwanNavSpeed[dwIndex];

		// check for overflow
		if(g_lpfSwanTailPos[dwIndex]>=1.0F)
		{
			// clip position
			g_lpfSwanTailPos[dwIndex]=1.0F;

			// reverse direction
			g_lpfSwanTailDir[dwIndex]=-g_lpfSwanTailDir[dwIndex];
		}
		else if(g_lpfSwanTailPos[dwIndex]<=-1.0F)
		{
			// clip position
			g_lpfSwanTailPos[dwIndex]=-1.0F;

			// reverse direction
			g_lpfSwanTailDir[dwIndex]=-g_lpfSwanTailDir[dwIndex];
		}

		// check action state
		switch(g_lpdwSwanActState[dwIndex])
		{
		// just swim
		case(ACST_SWAN_JUSTSWIM):
			// set tween value
			g_lpfSwanTweenVal[dwIndex]=GetNaturalizedTweenValue((g_lpfSwanTailPos[dwIndex]+1.0F)*0.5F);

			// change states if necessary
			if((abs(GetRandomInt())%SWAN_ACTIONCHANGERATIO)==1)
			{
				switch(abs(GetRandomInt())%SWAN_NUMACTIONSTATES)
				{
				// head up
				case(ACST_SWAN_HEADUP):
					// set transition values
					g_lpdwSwanTransKey1[dwIndex]=KEYF_SWAN_HUTR;
					g_lpdwSwanTransKey2[dwIndex]=KEYF_SWAN_HUTL;
					g_lpfSwanTransSpeed[dwIndex]=SWAN_TRANSITIONSPEED+(GetRandomFloat()*SWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head down
				case(ACST_SWAN_HEADDOWN):
					// set transition values
					g_lpdwSwanTransKey1[dwIndex]=KEYF_SWAN_HDTR;
					g_lpdwSwanTransKey2[dwIndex]=KEYF_SWAN_HDTL;
					g_lpfSwanTransSpeed[dwIndex]=SWAN_TRANSITIONSPEED+(GetRandomFloat()*SWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head left
				case(ACST_SWAN_HEADLEFT):
					// set transition values
					g_lpdwSwanTransKey1[dwIndex]=KEYF_SWAN_HL1TR;
					g_lpdwSwanTransKey2[dwIndex]=KEYF_SWAN_HL1TL;
					g_lpfSwanTransSpeed[dwIndex]=SWAN_TRANSITIONSPEED+(GetRandomFloat()*SWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head right
				case(ACST_SWAN_HEADRIGHT):
					// set transition values
					g_lpdwSwanTransKey1[dwIndex]=KEYF_SWAN_HR1TR;
					g_lpdwSwanTransKey2[dwIndex]=KEYF_SWAN_HR1TL;
					g_lpfSwanTransSpeed[dwIndex]=SWAN_TRANSITIONSPEED+(GetRandomFloat()*SWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head left #2
				case(ACST_SWAN_HEADLEFT2):
					// set transition values
					g_lpdwSwanTransKey1[dwIndex]=KEYF_SWAN_HL2TR;
					g_lpdwSwanTransKey2[dwIndex]=KEYF_SWAN_HL2TL;
					g_lpfSwanTransSpeed[dwIndex]=SWAN_TRANSITIONSPEED+(GetRandomFloat()*SWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head right #2
				case(ACST_SWAN_HEADRIGHT2):
					// set transition values
					g_lpdwSwanTransKey1[dwIndex]=KEYF_SWAN_HR2TR;
					g_lpdwSwanTransKey2[dwIndex]=KEYF_SWAN_HR2TL;
					g_lpfSwanTransSpeed[dwIndex]=SWAN_TRANSITIONSPEED+(GetRandomFloat()*SWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head down-left
				case(ACST_SWAN_HEADDOWNLEFT):
					// set transition values
					g_lpdwSwanTransKey1[dwIndex]=KEYF_SWAN_HDL1TR;
					g_lpdwSwanTransKey2[dwIndex]=KEYF_SWAN_HDL1TL;
					g_lpfSwanTransSpeed[dwIndex]=SWAN_TRANSITIONSPEED+(GetRandomFloat()*SWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head down-right
				case(ACST_SWAN_HEADDOWNRIGHT):
					// set transition values
					g_lpdwSwanTransKey1[dwIndex]=KEYF_SWAN_HDR1TR;
					g_lpdwSwanTransKey2[dwIndex]=KEYF_SWAN_HDR1TL;
					g_lpfSwanTransSpeed[dwIndex]=SWAN_TRANSITIONSPEED+(GetRandomFloat()*SWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head down-left #2
				case(ACST_SWAN_HEADDOWNLEFT2):
					// set transition values
					g_lpdwSwanTransKey1[dwIndex]=KEYF_SWAN_HDL2TR;
					g_lpdwSwanTransKey2[dwIndex]=KEYF_SWAN_HDL2TL;
					g_lpfSwanTransSpeed[dwIndex]=SWAN_TRANSITIONSPEED+(GetRandomFloat()*SWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head down-right #2
				case(ACST_SWAN_HEADDOWNRIGHT2):
					// set transition values
					g_lpdwSwanTransKey1[dwIndex]=KEYF_SWAN_HDR2TR;
					g_lpdwSwanTransKey2[dwIndex]=KEYF_SWAN_HDR2TL;
					g_lpfSwanTransSpeed[dwIndex]=SWAN_TRANSITIONSPEED+(GetRandomFloat()*SWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// look ahead
				default:
					// set transition values
					g_lpdwSwanTransKey1[dwIndex]=KEYF_SWAN_HCTR;
					g_lpdwSwanTransKey2[dwIndex]=KEYF_SWAN_HCTL;
					g_lpfSwanTransSpeed[dwIndex]=SWAN_TRANSITIONSPEED+(GetRandomFloat()*SWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;
				}

				// set state to transition
				g_lpdwSwanActState[dwIndex]=ACST_SWAN_TRANSITION;

				// set transition position flag
				g_lpfSwanTransPos[dwIndex]=-1.0F;
			}
		
			// ok
			break;
		
		// perform transition
		case(ACST_SWAN_TRANSITION):
			// check position flag
			if(g_lpfSwanTransPos[dwIndex]<0)
			{
				// set tween value
				g_lpfSwanTweenVal[dwIndex]=GetNaturalizedTweenValue((g_lpfSwanTailPos[dwIndex]+1.0F)*0.5F);

				// continue until keyframe is reached
				if(IsNearEqual(g_lpfSwanTweenVal[dwIndex],1))
					g_lpfSwanTransPos[dwIndex]=2;
			}
			else if(g_lpfSwanTransPos[dwIndex]>1)
			{
				// set tween value
				g_lpfSwanTweenVal[dwIndex]=0;

				// set new keyframes
				g_lpdwSwanKeyframe1[dwIndex]=g_lpdwSwanKeyframe2[dwIndex];
				g_lpdwSwanKeyframe2[dwIndex]=g_lpdwSwanTransKey1[dwIndex];

				// set position value
				g_lpfSwanTransPos[dwIndex]=0;
			}
			else
			{
				// increment transition position
				g_lpfSwanTransPos[dwIndex]+=g_lpfSwanTransSpeed[dwIndex];

				// check for overflow
				if(g_lpfSwanTransPos[dwIndex]>1)
				{
					// set tween value
					g_lpfSwanTweenVal[dwIndex]=0;

					// reset tail position & direction
					g_lpfSwanTailPos[dwIndex]=-1;
					g_lpfSwanTailDir[dwIndex]=fabsf(g_lpfSwanTailDir[dwIndex]);

					// set final keyframes
					g_lpdwSwanKeyframe1[dwIndex]=g_lpdwSwanTransKey1[dwIndex];
					g_lpdwSwanKeyframe2[dwIndex]=g_lpdwSwanTransKey2[dwIndex];

					// set state to swim
					g_lpdwSwanActState[dwIndex]=ACST_SWAN_JUSTSWIM;
				}
				else
				{
					// set tween value
					g_lpfSwanTweenVal[dwIndex]=GetNaturalizedTweenValue(g_lpfSwanTransPos[dwIndex]);
				}
			}
			
			// ok
			break;
		}

		// start ripples if necessary
		if(IsRippleComplete(g_lpdwSwanRipple1Idx[dwIndex]))
			StartRipple(g_lpdwSwanRipple1Idx[dwIndex],
						g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._41,
						g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._43,
						GetRandomRippleSpeed());
		if(IsRippleComplete(g_lpdwSwanRipple2Idx[dwIndex]))
			StartRipple(g_lpdwSwanRipple2Idx[dwIndex],
						g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._41,
						g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._43,
						GetRandomRippleSpeed());
	}
}

// AddSwanSPFamily()

DWORD AddSwanSPFamily(DWORD dwNumBBSwans)
{
	LoadingOutput("AddSwanSPFamily");

	// parent index
	DWORD dwIndex;
	
	// counters
	DWORD dwCount1;
	DWORD dwCount2;

	// baby index
	DWORD dwBBIndex;

	// matrix
	D3DXMATRIX mtx;
	
	// add parent & save index
	dwIndex=AddSwanLoner();

	// check swan
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// add baby swans
	for(dwCount1=0;dwCount1<dwNumBBSwans;dwCount1++)
	{
		// add baby swan
		dwBBIndex=AddBBSwan();

		// check baby swan
		if(dwBBIndex==NO_OBJECT)
			return(NO_OBJECT);

		// copy parent's position to baby swan
		g_lpMtx[g_lpdwBBSwanMtx[dwBBIndex]].mtx._41=g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._41;
		g_lpMtx[g_lpdwBBSwanMtx[dwBBIndex]].mtx._42=g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._42;
		g_lpMtx[g_lpdwBBSwanMtx[dwBBIndex]].mtx._43=g_lpMtx[g_lpdwSwanMtx[dwIndex]].mtx._43;

		// randomize yaw of baby swan
		for(dwCount2=0;dwCount2<8;dwCount2++)
			MatrixYaw(g_lpdwBBSwanMtx[dwBBIndex],
					  fabsf(GetRandomFloat()));

		// move baby swan back to ensure no others occupy this same position
		MatrixAhead(g_lpdwBBSwanMtx[dwBBIndex],
					(BBSWAN_PANICDISTANCE*0.5F)+((float)(dwCount1)));

		// set parent indices
		g_lpdwBBSwanParent1Idx[dwBBIndex]=dwIndex;
		g_lpdwBBSwanParent2Idx[dwBBIndex]=NO_OBJECT;
	}

	// return index
	return(dwIndex);
}

// AddSwanDPFamily()

DWORD AddSwanDPFamily(DWORD dwNumBBSwans)
{
	LoadingOutput("AddSwanDPFamily");

	// parent indices
	DWORD dwIndex1;
	DWORD dwIndex2;
	
	// counters
	DWORD dwCount1;
	DWORD dwCount2;

	// baby index
	DWORD dwBBIndex;

	// matrices
	D3DXMATRIX mtx1;
	D3DXMATRIX mtx2;
	
	// add parents & save index
	dwIndex1=AddSwanCouple();

	// check index
	if(dwIndex1==NO_OBJECT)
		return(NO_OBJECT);

	// save mate's index
	dwIndex2=g_lpdwSwanMateIdx[dwIndex1];

	// get parent #1's matrix
	GetMatrix(g_lpdwSwanMtx[dwIndex1],
			  &mtx1);

	// get parent #2's matrix
	GetMatrix(g_lpdwSwanMtx[dwIndex2],
			  &mtx2);

	// compute average position
	mtx1._41=(mtx1._41+mtx2._41)*0.5F;
	mtx1._42=(mtx1._42+mtx2._42)*0.5F;
	mtx1._43=(mtx1._43+mtx2._43)*0.5F;

	// add baby swans
	for(dwCount1=0;dwCount1<dwNumBBSwans;dwCount1++)
	{
		// add baby swan
		dwBBIndex=AddBBSwan();

		// check baby swan
		if(dwBBIndex==NO_OBJECT)
			return(NO_OBJECT);

		// copy parent's position to baby swan
		g_lpMtx[g_lpdwBBSwanMtx[dwBBIndex]].mtx._41=mtx1._41;
		g_lpMtx[g_lpdwBBSwanMtx[dwBBIndex]].mtx._42=mtx1._42;
		g_lpMtx[g_lpdwBBSwanMtx[dwBBIndex]].mtx._43=mtx1._43;

		// randomize yaw of baby swan
		for(dwCount2=0;dwCount2<8;dwCount2++)
			MatrixYaw(g_lpdwBBSwanMtx[dwBBIndex],
					  fabsf(GetRandomFloat()));

		// move baby swan back to ensure no others occupy this same position
		MatrixAhead(g_lpdwBBSwanMtx[dwBBIndex],
					(BBSWAN_PANICDISTANCE*0.5F)+((float)(dwCount1)));

		// set parent indices
		g_lpdwBBSwanParent1Idx[dwBBIndex]=dwIndex1;
		g_lpdwBBSwanParent2Idx[dwBBIndex]=dwIndex2;
	}

	// return index
	return(dwIndex1);
}

// GetNextBBSwan()

DWORD GetNextBBSwan(void)
{
	LoadingOutput("GetNextBBSwan");

	// check counter
	if(g_dwNumBBSwan==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanMtx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanPriState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanActState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanNavState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanKeyframe1,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanKeyframe2,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfBBSwanTweenVal,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfBBSwanTailPos,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfBBSwanTailDir,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfBBSwanTransPos,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfBBSwanTransSpeed,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanTransKey1,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanTransKey2,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfBBSwanNavSpeed,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanNavPos,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanNavMax,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanParent1Idx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanParent2Idx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanRipple1Idx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBBSwanRipple2Idx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxBBSwan=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumBBSwan>=g_dwMaxBBSwan)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanMtx,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanPriState,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanActState,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanNavState,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanKeyframe1,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanKeyframe2,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfBBSwanTweenVal,
					   sizeof(float)*g_dwMaxBBSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfBBSwanTailPos,
					   sizeof(float)*g_dwMaxBBSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfBBSwanTailDir,
					   sizeof(float)*g_dwMaxBBSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfBBSwanTransPos,
					   sizeof(float)*g_dwMaxBBSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfBBSwanTransSpeed,
					   sizeof(float)*g_dwMaxBBSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanTransKey1,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanTransKey2,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfBBSwanNavSpeed,
					   sizeof(float)*g_dwMaxBBSwan,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanNavPos,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanNavMax,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanParent1Idx,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanParent2Idx,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanRipple1Idx,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBBSwanRipple2Idx,
					   sizeof(DWORD)*g_dwMaxBBSwan,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxBBSwan+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumBBSwan;

	// increment counter
	g_dwNumBBSwan++;

	// return index
	return(dwIndex);
}

// AddBBSwan()

DWORD AddBBSwan(void)
{
	LoadingOutput("AddBBSwan");

	// get return index
	DWORD dwIndex=GetNextBBSwan();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// add baby swan matrix
	g_lpdwBBSwanMtx[dwIndex]=AddMatrix();

	// check matrix
	if(g_lpdwBBSwanMtx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// scale matrix by baby swan scale
	MatrixScale(g_lpdwBBSwanMtx[dwIndex],
				BBSWAN_SCALE+(GetRandomFloat()*BBSWAN_SCALEVAR));

	// set default values
	g_lpdwBBSwanPriState[dwIndex]=PRST_BBSWAN_REST;
	g_lpdwBBSwanActState[dwIndex]=ACST_BBSWAN_JUSTSWIM;
	g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_JUSTSWIM;
	g_lpdwBBSwanKeyframe1[dwIndex]=KEYF_BBSWAN_HCTR;
	g_lpdwBBSwanKeyframe2[dwIndex]=KEYF_BBSWAN_HCTL;
	g_lpfBBSwanTweenVal[dwIndex]=0;
	g_lpfBBSwanTailPos[dwIndex]=0;
	g_lpfBBSwanTailDir[dwIndex]=BBSWAN_TAILSPEED;
	g_lpfBBSwanTransPos[dwIndex]=0;
	g_lpfBBSwanTransSpeed[dwIndex]=0;
	g_lpdwBBSwanTransKey1[dwIndex]=NO_OBJECT;
	g_lpdwBBSwanTransKey2[dwIndex]=NO_OBJECT;
	g_lpfBBSwanNavSpeed[dwIndex]=0;
	g_lpdwBBSwanNavPos[dwIndex]=0;
	g_lpdwBBSwanNavMax[dwIndex]=0;
	g_lpdwBBSwanParent1Idx[dwIndex]=NO_OBJECT;
	g_lpdwBBSwanParent2Idx[dwIndex]=NO_OBJECT;

	// add ripple #1
	g_lpdwBBSwanRipple1Idx[dwIndex]=AddRipple();

	// check ripple #1
	if(g_lpdwBBSwanRipple1Idx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// add ripple #2
	g_lpdwBBSwanRipple2Idx[dwIndex]=AddRipple();

	// check ripple #2
	if(g_lpdwBBSwanRipple2Idx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// return index
	return(dwIndex);
}

// PurgeBBSwans()

void PurgeBBSwans(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpdwBBSwanMtx);
	FreeMem((LPVOID*)&g_lpdwBBSwanPriState);
	FreeMem((LPVOID*)&g_lpdwBBSwanActState);
	FreeMem((LPVOID*)&g_lpdwBBSwanNavState);
	FreeMem((LPVOID*)&g_lpdwBBSwanKeyframe1);
	FreeMem((LPVOID*)&g_lpdwBBSwanKeyframe2);
	FreeMem((LPVOID*)&g_lpfBBSwanTweenVal);
	FreeMem((LPVOID*)&g_lpfBBSwanTailPos);
	FreeMem((LPVOID*)&g_lpfBBSwanTailDir);
	FreeMem((LPVOID*)&g_lpfBBSwanTransPos);
	FreeMem((LPVOID*)&g_lpfBBSwanTransSpeed);
	FreeMem((LPVOID*)&g_lpdwBBSwanTransKey1);
	FreeMem((LPVOID*)&g_lpdwBBSwanTransKey2);
	FreeMem((LPVOID*)&g_lpfBBSwanNavSpeed);
	FreeMem((LPVOID*)&g_lpdwBBSwanNavPos);
	FreeMem((LPVOID*)&g_lpdwBBSwanNavMax);
	FreeMem((LPVOID*)&g_lpdwBBSwanParent1Idx);
	FreeMem((LPVOID*)&g_lpdwBBSwanParent2Idx);
	FreeMem((LPVOID*)&g_lpdwBBSwanRipple1Idx);
	FreeMem((LPVOID*)&g_lpdwBBSwanRipple2Idx);

	// reset counters
	g_dwNumBBSwan=0;
	g_dwMaxBBSwan=0;
}

// RenderBBSwans()

void RenderBBSwans(void)
{
	// baby swan index
	DWORD dwIndex;

#ifndef REFLECTION_OPTIMIZATION
	// render baby swans
	for(dwIndex=0;dwIndex<g_dwNumBBSwan;dwIndex++)
	{
		// set keyframe
		UpdateRenderFrames(g_dwBBSwanAnim,
						   g_lpdwBBSwanKeyframe1[dwIndex],
						   g_lpdwBBSwanKeyframe2[dwIndex],
						   g_lpfBBSwanTweenVal[dwIndex]);

		// render swan
		RenderAnimation(g_dwBBSwanAnim,
						g_lpdwBBSwanMtx[dwIndex]);
	}

	// enable frontface cull
	EnableFrontfaceCull();
	
	// render baby swan reflections
	for(dwIndex=0;dwIndex<g_dwNumBBSwan;dwIndex++)
	{
		// invert y-axis
		g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].fYScale;

		// set keyframe
		UpdateRenderFrames(g_dwBBSwanAnim,
						   g_lpdwBBSwanKeyframe1[dwIndex],
						   g_lpdwBBSwanKeyframe2[dwIndex],
						   g_lpfBBSwanTweenVal[dwIndex]);
		
		// render swan
		RenderAnimation(g_dwBBSwanAnim,
						g_lpdwBBSwanMtx[dwIndex]);
		
		// reset y-axis
		g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].fYScale;
	}
	
	// enable backface cull
	EnableBackfaceCull();
#else
	// render baby swans & reflections
	for(dwIndex=0;dwIndex<g_dwNumBBSwan;dwIndex++)
	{
		// set keyframe
		UpdateRenderFrames(g_dwBBSwanAnim,
						   g_lpdwBBSwanKeyframe1[dwIndex],
						   g_lpdwBBSwanKeyframe2[dwIndex],
						   g_lpfBBSwanTweenVal[dwIndex]);

		// render swan
		RenderAnimation(g_dwBBSwanAnim,
						g_lpdwBBSwanMtx[dwIndex]);
		// enable frontface cull
		EnableFrontfaceCull();
	
		// invert y-axis
		g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].fYScale;

		// render swan
		RenderAnimation(g_dwBBSwanAnim,
						g_lpdwBBSwanMtx[dwIndex]);
		
		// reset y-axis
		g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].fYScale;
		
		// enable backface cull
		EnableBackfaceCull();
	}
#endif
}

// UpdateBBSwans()

void UpdateBBSwans(void)
{
	// alternating counter
	static DWORD dwAltCount=0;

	// baby swan index
	DWORD dwIndex;

	// parent indices
	DWORD dwParent1Index;
	DWORD dwParent2Index;

	// average parent position coordinates
	float fParentX;
	float fParentZ;

	// navigation update flag
	BOOL bNavFlag;

	// vectors for comparisons
	D3DXVECTOR3 vV1;
	D3DXVECTOR3 vV2;
	D3DXVECTOR3 vV3;

	// distance value
	float fDist;

	// tracking value
	float fTrackingVal;

	// coordinate values
	float fX;
	float fZ;

	// counter
	DWORD dwCount;

	// update alternating counter
	dwAltCount=(dwAltCount+1)&1;

	// update baby swans
	for(dwIndex=0;dwIndex<g_dwNumBBSwan;dwIndex++)
	{
		// get parent indices
		dwParent1Index=g_lpdwBBSwanParent1Idx[dwIndex];
		dwParent2Index=g_lpdwBBSwanParent2Idx[dwIndex];

		// check for parent #2
		if(dwParent2Index!=NO_OBJECT)
		{
			// compute average parent position
			fParentX=(g_lpMtx[g_lpdwSwanMtx[dwParent1Index]].mtx._41+g_lpMtx[g_lpdwSwanMtx[dwParent2Index]].mtx._41)*0.5F;
			fParentZ=(g_lpMtx[g_lpdwSwanMtx[dwParent1Index]].mtx._43+g_lpMtx[g_lpdwSwanMtx[dwParent2Index]].mtx._43)*0.5F;

			// set baby swan to rest if necessary (unless panicking)
			if(g_lpdwBBSwanPriState[dwIndex]!=PRST_BBSWAN_PANIC)
				if(g_lpdwSwanPriState[dwParent1Index]==PRST_SWAN_REST||
				   g_lpdwSwanPriState[dwParent2Index]==PRST_SWAN_REST)
					g_lpdwBBSwanPriState[dwIndex]=PRST_BBSWAN_REST;
		}
		else
		{
			// set parent #1 position
			fParentX=g_lpMtx[g_lpdwSwanMtx[dwParent1Index]].mtx._41;
			fParentZ=g_lpMtx[g_lpdwSwanMtx[dwParent1Index]].mtx._43;

			// set baby swan to rest if necessary (unless panicking)
			if(g_lpdwBBSwanPriState[dwIndex]!=PRST_BBSWAN_PANIC)
				if(g_lpdwSwanPriState[dwParent1Index]==PRST_SWAN_REST)
					g_lpdwBBSwanPriState[dwIndex]=PRST_BBSWAN_REST;
		}

		// set coordinate values
		fX=g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._41;
		fZ=g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._43;

		// compare baby swan position with adult swans
		for(dwCount=0;dwCount<g_dwNumSwan;dwCount++)
		{
			// set up vector
			vV1.x=fX-g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._41;
			vV1.z=fZ-g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._43;

			// save distance
			fDist=SquareMagnitudeXZ(&vV1);

			// check distance
			if(fDist<Squared(BBSWAN_MINSWANDISTANCE))
			{
				// compute inverse distance
				float fInvDist=1.0F/sqrtf(fDist);
				
				// normalize vector
				vV2.x=vV1.x*fInvDist;
				vV2.y=0;
				vV2.z=vV1.z*fInvDist;

				// set new position
				g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._41+=vV2.x*g_lpfSwanNavSpeed[dwCount];
				g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._43+=vV2.z*g_lpfSwanNavSpeed[dwCount];
			}
		}

		// compare baby swan position with other baby swans
		for(dwCount=0;dwCount<g_dwNumBBSwan;dwCount++)
		{
			// skip this swan
			if(dwCount!=dwIndex)
			{
				// set up vector
				vV1.x=fX-g_lpMtx[g_lpdwBBSwanMtx[dwCount]].mtx._41;
				vV1.z=fZ-g_lpMtx[g_lpdwBBSwanMtx[dwCount]].mtx._43;

				// save distance
				fDist=SquareMagnitudeXZ(&vV1);

				// check distance
				if(fDist<Squared(BBSWAN_MINBBSWANDISTANCE))
				{
					// compute distance
					float fInvDist=1.0F/sqrtf(fDist);
					
					// normalize vector
					vV2.x=vV1.x*fInvDist;
					vV2.y=0;
					vV2.z=vV1.z*fInvDist;

					// set new position
					g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._41+=vV2.x*g_lpfBBSwanNavSpeed[dwCount];
					g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._43+=vV2.z*g_lpfBBSwanNavSpeed[dwCount];
				}
			}
		}

		// check primary state
		switch(g_lpdwBBSwanPriState[dwIndex])
		{
		// follow
		case(PRST_BBSWAN_FOLLOW):
			// get baby swan position
			vV1=D3DXVECTOR3(g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._41,
							0,
							g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._43);

			// get average parent position
			vV2=D3DXVECTOR3(fParentX,0,fParentZ);

			// get distance to parents
			fDist=SquareMagnitudeXZ(vV1.x-vV2.x,
									vV1.z-vV2.z);

			// check distance to parents
			if(fDist<Squared(BBSWAN_MAXFOLLOWDISTANCE))
			{
				// check speed
				if(g_lpfBBSwanNavSpeed[dwIndex]>(g_lpfSwanNavSpeed[dwParent1Index]*SWAN_TRACKINGSPEEDVAL)+SWAN_TRACKINGSPEEDINC)
				{
					// set state to slow down
					g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_SLOWDOWN;

					// set navigation values
					g_lpdwBBSwanNavPos[dwIndex]=0;
					g_lpdwBBSwanNavMax[dwIndex]=2;
				}
				else if(g_lpfBBSwanNavSpeed[dwIndex]<(g_lpfSwanNavSpeed[dwParent1Index]*SWAN_TRACKINGSPEEDVAL1)-SWAN_TRACKINGSPEEDINC1)
				{
					// set state to speed up
					g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_SPEEDUP;

					// set navigation values
					g_lpdwBBSwanNavPos[dwIndex]=0;
					g_lpdwBBSwanNavMax[dwIndex]=2;
				}
				else
				{
					// check navigation state
					if(g_lpdwBBSwanNavState[dwIndex]==NVST_BBSWAN_JUSTSWIM)
					{
						// change navigation state if necessary
						if((abs(GetRandomInt())%BBSWAN_NAVIGATIONCHANGERATIO)==1)
						{
							// set new state
							g_lpdwBBSwanNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%BBSWAN_NUMNAVIGATIONSTATES);

							// set navigation values
							g_lpdwBBSwanNavPos[dwIndex]=0;
							g_lpdwBBSwanNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%BBSWAN_NAVIGATIONMAXINC)+1);
						}
					}
  				}
			}
			else
			{
				// get vector from swan to parent
				vV3=vV2-vV1;

				// normalize vector
				Normalize(&vV2,&vV3);
				
				// get baby swan ahead vector
				vV1=D3DXVECTOR3(g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._31,
								0,
								g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._33);

				// check cosine of angle
				if(DotProduct(&vV1,&vV2)>0)
				{
					// get vector perpendicular to ahead vector
					vV3.x=vV1.z;
					vV3.y=0;
					vV3.z=-vV1.x;

					// check cosine of angle
					if(DotProduct(&vV3,&vV2)>0)
					{
						// set state to hard right
						g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_HARDRIGHT;
					}
					else
					{
						// set state to hard left
						g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_HARDLEFT;
					}

					// check speed
					if(g_lpfBBSwanNavSpeed[dwIndex]>0)
					{
						// check alternating counter
						if(dwAltCount)
						{
							// set state to slow down
							g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_SLOWDOWN;
						}
					}

					// set navigation values
					g_lpdwBBSwanNavPos[dwIndex]=0;
					g_lpdwBBSwanNavMax[dwIndex]=2;
				}
				else
				{
					// get vector perpendicular to ahead vector
					vV3.x=vV1.z;
					vV3.y=0;
					vV3.z=-vV1.x;

					// save tracking value
					fTrackingVal=DotProduct(&vV3,&vV2);

					// check tracking value
					if(fabsf(fTrackingVal)>SWAN_FOLLOWTRACKINGVAL)
					{
						// check tracking value
						if(fTrackingVal>0)
						{
							// set state to hard right
							g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_HARDRIGHT;
						}
						else
						{
							// set state to hard left
							g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_HARDLEFT;
						}

						// check speed
						if(g_lpfBBSwanNavSpeed[dwIndex]<((g_lpfSwanNavSpeed[dwParent1Index]*SWAN_TRACKINGSPEEDVAL)+SWAN_TRACKINGSPEEDINC)-SWAN_SPEEDTOLERANCE)
						{
							// check alternating counter
							if(dwAltCount)
							{
								// set state to speed up
								g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_SPEEDUP;
							}
						}
						else if(g_lpfBBSwanNavSpeed[dwIndex]>((g_lpfSwanNavSpeed[dwParent1Index]*SWAN_TRACKINGSPEEDVAL)+SWAN_TRACKINGSPEEDINC)+SWAN_SPEEDTOLERANCE)
						{
							// check alternating counter
							if(dwAltCount)
							{
								// set state to slow down
								g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_SLOWDOWN;
							}
						}

						// set navigation values
						g_lpdwBBSwanNavPos[dwIndex]=0;
						g_lpdwBBSwanNavMax[dwIndex]=2;
					}
					else
					{
						// check distance to parents
						if(fDist>Squared(BBSWAN_PANICDISTANCE))
						{
							// set state to panic
							g_lpdwBBSwanPriState[dwIndex]=PRST_BBSWAN_PANIC;
						}
						else if(fDist>Squared(BBSWAN_MAXFOLLOWDISTANCE))
						{
							// check speed
							if(g_lpfBBSwanNavSpeed[dwIndex]<((g_lpfSwanNavSpeed[dwParent1Index]*SWAN_TRACKINGSPEEDVAL2)+SWAN_TRACKINGSPEEDINC2)-SWAN_SPEEDTOLERANCE)
							{
								// set state to speed up
								g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_SPEEDUP;

								// set navigation values
								g_lpdwBBSwanNavPos[dwIndex]=0;
								g_lpdwBBSwanNavMax[dwIndex]=2;
							}
							else if(g_lpfBBSwanNavSpeed[dwIndex]>((g_lpfSwanNavSpeed[dwParent1Index]*SWAN_TRACKINGSPEEDVAL2)+SWAN_TRACKINGSPEEDINC2)+SWAN_SPEEDTOLERANCE)
							{
								// set state to slow down
								g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_SLOWDOWN;

								// set navigation values
								g_lpdwBBSwanNavPos[dwIndex]=0;
								g_lpdwBBSwanNavMax[dwIndex]=2;
							}
						}
					}
				}
			}

			// change primary state if necessary
			if((abs(GetRandomInt())%BBSWAN_PRIMARYCHANGERATIO)==1)
			{
				// set new state
				g_lpdwBBSwanPriState[dwIndex]=(DWORD)(abs(GetRandomInt())%BBSWAN_NUMPRIMARYSTATES);
			}

			// ok
			break;

		// wander
		case(PRST_BBSWAN_WANDER):
			// get baby swan position
			vV1=D3DXVECTOR3(g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._41,
							0,
							g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._43);

			// compare position with boundaries
			if(IsPointOutOfBounds(vV1.x,vV1.z))
			{
				// set state to panic
				g_lpdwBBSwanPriState[dwIndex]=PRST_BBSWAN_PANIC;
			}
			else
			{
				// get average parent position
				vV2=D3DXVECTOR3(fParentX,0,fParentZ);

				// get distance to parents
				fDist=SquareMagnitudeXZ(vV1.x-vV2.x,
										vV1.z-vV2.z);

				// check distance to parents
				if(fDist>Squared(BBSWAN_PANICDISTANCE))
				{
					// set state to panic
					g_lpdwBBSwanPriState[dwIndex]=PRST_BBSWAN_PANIC;
				}
				else
				{
					// check navigation state
					if(g_lpdwBBSwanNavState[dwIndex]==NVST_BBSWAN_JUSTSWIM)
					{
						// change navigation state if necessary
						if((abs(GetRandomInt())%BBSWAN_NAVIGATIONCHANGERATIO)==1)
						{
							// set new state
							g_lpdwBBSwanNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%BBSWAN_NUMNAVIGATIONSTATES);

							// set navigation values
							g_lpdwBBSwanNavPos[dwIndex]=0;
							g_lpdwBBSwanNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%BBSWAN_NAVIGATIONMAXINC)+1);
						}
					}

					// change primary state if necessary
					if((abs(GetRandomInt())%BBSWAN_PRIMARYCHANGERATIO)==1)
					{
						// set new state
						g_lpdwBBSwanPriState[dwIndex]=(DWORD)(abs(GetRandomInt())%BBSWAN_NUMPRIMARYSTATES);
					}
				}
			}

			// ok
			break;

		// panic
		case(PRST_BBSWAN_PANIC):
			// get baby swan position
			vV1=D3DXVECTOR3(g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._41,
							0,
							g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._43);

			// get average parent position
			vV2=D3DXVECTOR3(fParentX,0,fParentZ);

			// get distance to parents
			fDist=SquareMagnitudeXZ(vV1.x-vV2.x,
								    vV1.z-vV2.z);

			// check distance to parents
			if(fDist<Squared(BBSWAN_MAXFOLLOWDISTANCE))
			{
				// no need to panic!
				g_lpdwBBSwanPriState[dwIndex]=PRST_BBSWAN_FOLLOW;
			}
			else
			{
				// get vector from swan to parent
				vV3=vV2-vV1;

				// normalize vector
				Normalize(&vV2,&vV3);
				
				// get baby swan ahead vector
				vV1=D3DXVECTOR3(g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._31,
								0,
								g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._33);

				// check cosine of angle
				if(DotProduct(&vV1,&vV2)>0)
				{
					// get vector perpendicular to ahead vector
					vV3.x=vV1.z;
					vV3.y=0;
					vV3.z=-vV1.x;

					// check cosine of angle
					if(DotProduct(&vV3,&vV2)>0)
					{
						// set state to hard right
						g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_HARDRIGHT;
					}
					else
					{
						// set state to hard left
						g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_HARDLEFT;
					}

					// check speed
					if(g_lpfBBSwanNavSpeed[dwIndex]<BBSWAN_MAXSPEED)
					{
						// check alternating counter
						if(dwAltCount)
						{
							// set state to speed up
							g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_SPEEDUP;
						}
					}

					// set navigation values
					g_lpdwBBSwanNavPos[dwIndex]=0;
					g_lpdwBBSwanNavMax[dwIndex]=2;
				}
				else
				{
					// get vector perpendicular to ahead vector
					vV3.x=vV1.z;
					vV3.y=0;
					vV3.z=-vV1.x;

					// save tracking value
					fTrackingVal=DotProduct(&vV3,&vV2);

					// check tracking value
					if(fabsf(fTrackingVal)>SWAN_FOLLOWTRACKINGVAL)
					{
						// check tracking value
						if(fTrackingVal>0)
						{
							// set state to hard right
							g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_HARDRIGHT;
						}
						else
						{
							// set state to hard left
							g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_HARDLEFT;
						}

						// check speed
						if(g_lpfBBSwanNavSpeed[dwIndex]<BBSWAN_MAXSPEED)
						{
							// check alternating counter
							if(dwAltCount)
							{
								// set state to speed up
								g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_SPEEDUP;
							}
						}

						// set navigation values
						g_lpdwBBSwanNavPos[dwIndex]=0;
						g_lpdwBBSwanNavMax[dwIndex]=2;
					}
				}
			}

			// ok
			break;

		// rest
		case(PRST_BBSWAN_REST):
			// swan cannot rest unless parents are resting
			if(dwParent2Index!=NO_OBJECT)
			{
				// set baby swan to follow if necessary
				if(g_lpdwSwanPriState[dwParent1Index]!=PRST_SWAN_REST||
				   g_lpdwSwanPriState[dwParent2Index]!=PRST_SWAN_REST)
					g_lpdwBBSwanPriState[dwIndex]=PRST_BBSWAN_FOLLOW;
			}
			else
			{
				// set baby swan to follow if necessary
				if(g_lpdwSwanPriState[dwParent1Index]!=PRST_SWAN_REST)
					g_lpdwBBSwanPriState[dwIndex]=PRST_BBSWAN_FOLLOW;
			}

			// get baby swan position
			vV1=D3DXVECTOR3(g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._41,
							0,
							g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._43);

			// get average parent position
			vV2=D3DXVECTOR3(fParentX,0,fParentZ);

			// get distance to parents
			fDist=SquareMagnitudeXZ(vV1.x-vV2.x,
								    vV1.z-vV2.z);

			// check distance to parents & boundaries
			if(fDist>Squared(BBSWAN_MAXRESTDISTANCE)||
			   IsPointOutOfBounds(fX,fZ))
			{
				// set parent #1 to lead
				g_lpdwSwanPriState[dwParent1Index]=PRST_SWAN_LEAD;

				// set parent #2 to follow if necessary
				if(dwParent2Index!=NO_OBJECT)
					g_lpdwSwanPriState[dwParent2Index]=PRST_SWAN_FOLLOW;
			}
			else
			{
				// check speed
				if(g_lpfBBSwanNavSpeed[dwIndex]>BBSWAN_MAXRESTSPEED)
				{
					// set state to slow down
					g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_SLOWDOWN;

					// set navigation values
					g_lpdwBBSwanNavPos[dwIndex]=0;
					g_lpdwBBSwanNavMax[dwIndex]=2;
				}
				else
				{
					// check navigation state
					if(g_lpdwBBSwanNavState[dwIndex]==NVST_BBSWAN_JUSTSWIM)
					{
						// change navigation state if necessary
						if((abs(GetRandomInt())%BBSWAN_NAVIGATIONCHANGERATIO)==1)
						{
							// set new state
							g_lpdwBBSwanNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%BBSWAN_NUMNAVIGATIONSTATES);

							// set navigation values
							g_lpdwBBSwanNavPos[dwIndex]=0;
							g_lpdwBBSwanNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%BBSWAN_NAVIGATIONMAXINC)+1);
						}
					}
				}
			}
			
			// ok
			break;
		}

		// check navigation state
		if(g_lpdwBBSwanNavState[dwIndex]!=NVST_BBSWAN_JUSTSWIM)
		{
			// check navigation position
			if(g_lpdwBBSwanNavPos[dwIndex]<g_lpdwBBSwanNavMax[dwIndex])
			{
				// set flag
				bNavFlag=TRUE;

				// increment navigation position
				g_lpdwBBSwanNavPos[dwIndex]++;
			}
			else
			{
				// reset flag
				bNavFlag=FALSE;
			}
		}

		// check navigation state
		switch(g_lpdwBBSwanNavState[dwIndex])
		{
		// speed up
		case(NVST_SWAN_SPEEDUP):
			// increase speed
			g_lpfBBSwanNavSpeed[dwIndex]+=BBSWAN_SPEEDINC;

			// check for overflow
			if(g_lpfBBSwanNavSpeed[dwIndex]>BBSWAN_MAXSPEED)
				g_lpfBBSwanNavSpeed[dwIndex]=BBSWAN_MAXSPEED;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_JUSTSWIM;
			}

			// ok
			break;

		// slow down
		case(NVST_BBSWAN_SLOWDOWN):
			// decrease speed
			g_lpfBBSwanNavSpeed[dwIndex]-=BBSWAN_SPEEDINC;

			// check for overflow
			if(g_lpfBBSwanNavSpeed[dwIndex]<0)
				g_lpfBBSwanNavSpeed[dwIndex]=0;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_JUSTSWIM;
			}

			// ok
			break;

		// turn left
		case(NVST_BBSWAN_TURNLEFT):
			// check for panic mode & turn as needed
			if(g_lpdwBBSwanNavState[dwIndex]==PRST_BBSWAN_PANIC)
				MatrixYaw(g_lpdwBBSwanMtx[dwIndex],BBSWAN_TURNINC*BBSWAN_PANICTURNFACTOR);
			else
				MatrixYaw(g_lpdwBBSwanMtx[dwIndex],BBSWAN_TURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_JUSTSWIM;
			}

			// ok
			break;

		// turn right
		case(NVST_BBSWAN_TURNRIGHT):
			// check for panic mode & turn as needed
			if(g_lpdwBBSwanNavState[dwIndex]==PRST_BBSWAN_PANIC)
				MatrixYaw(g_lpdwBBSwanMtx[dwIndex],-(BBSWAN_TURNINC*BBSWAN_PANICTURNFACTOR));
			else
				MatrixYaw(g_lpdwBBSwanMtx[dwIndex],-BBSWAN_TURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_JUSTSWIM;
			}

			// ok
			break;

		// hard left
		case(NVST_BBSWAN_HARDLEFT):
			// check for panic mode & turn as needed
			if(g_lpdwBBSwanNavState[dwIndex]==PRST_BBSWAN_PANIC)
				MatrixYaw(g_lpdwBBSwanMtx[dwIndex],BBSWAN_HARDTURNINC*BBSWAN_PANICTURNFACTOR);
			else
				MatrixYaw(g_lpdwBBSwanMtx[dwIndex],BBSWAN_HARDTURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_JUSTSWIM;
			}

			// ok
			break;

		// hard right
		case(NVST_BBSWAN_HARDRIGHT):
			// check for panic mode & turn as needed
			if(g_lpdwBBSwanNavState[dwIndex]==PRST_BBSWAN_PANIC)
				MatrixYaw(g_lpdwBBSwanMtx[dwIndex],-(BBSWAN_HARDTURNINC*BBSWAN_PANICTURNFACTOR));
			else
				MatrixYaw(g_lpdwBBSwanMtx[dwIndex],-BBSWAN_HARDTURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwBBSwanNavState[dwIndex]=NVST_BBSWAN_JUSTSWIM;
			}

			// ok
			break;
		}

		// check for panic mode & move baby swan
		if(g_lpdwBBSwanNavState[dwIndex]==PRST_BBSWAN_PANIC)
			MatrixBack(g_lpdwBBSwanMtx[dwIndex],g_lpfBBSwanNavSpeed[dwIndex]*BBSWAN_PANICSPEEDFACTOR);
		else
			MatrixBack(g_lpdwBBSwanMtx[dwIndex],g_lpfBBSwanNavSpeed[dwIndex]);

		// check for panic mode & update tail
		if(g_lpdwBBSwanNavState[dwIndex]==PRST_BBSWAN_PANIC)
			g_lpfBBSwanTailPos[dwIndex]+=g_lpfBBSwanTailDir[dwIndex]+(g_lpfBBSwanNavSpeed[dwIndex]*BBSWAN_PANICSPEEDFACTOR);
		else
			g_lpfBBSwanTailPos[dwIndex]+=g_lpfBBSwanTailDir[dwIndex]+g_lpfBBSwanNavSpeed[dwIndex];

		// check for overflow
		if(g_lpfBBSwanTailPos[dwIndex]>=1.0F)
		{
			// clip position
			g_lpfBBSwanTailPos[dwIndex]=1.0F;

			// reverse direction
			g_lpfBBSwanTailDir[dwIndex]=-g_lpfBBSwanTailDir[dwIndex];
		}
		else if(g_lpfBBSwanTailPos[dwIndex]<=-1.0F)
		{
			// clip position
			g_lpfBBSwanTailPos[dwIndex]=-1.0F;

			// reverse direction
			g_lpfBBSwanTailDir[dwIndex]=-g_lpfBBSwanTailDir[dwIndex];
		}

		// check action state
		switch(g_lpdwBBSwanActState[dwIndex])
		{
		// just swim
		case(ACST_BBSWAN_JUSTSWIM):
			// set tween value
			g_lpfBBSwanTweenVal[dwIndex]=GetNaturalizedTweenValue((g_lpfBBSwanTailPos[dwIndex]+1.0F)*0.5F);

			// change states if necessary
			if((abs(GetRandomInt())%BBSWAN_ACTIONCHANGERATIO)==1)
			{
				switch(abs(GetRandomInt())%BBSWAN_NUMACTIONSTATES)
				{
				// head up
				case(ACST_BBSWAN_HEADUP):
					// set transition values
					g_lpdwBBSwanTransKey1[dwIndex]=KEYF_BBSWAN_HUTR;
					g_lpdwBBSwanTransKey2[dwIndex]=KEYF_BBSWAN_HUTL;
					g_lpfBBSwanTransSpeed[dwIndex]=BBSWAN_TRANSITIONSPEED+(GetRandomFloat()*BBSWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head down
				case(ACST_BBSWAN_HEADDOWN):
					// set transition values
					g_lpdwBBSwanTransKey1[dwIndex]=KEYF_BBSWAN_HDTR;
					g_lpdwBBSwanTransKey2[dwIndex]=KEYF_BBSWAN_HDTL;
					g_lpfBBSwanTransSpeed[dwIndex]=BBSWAN_TRANSITIONSPEED+(GetRandomFloat()*BBSWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head left
				case(ACST_BBSWAN_HEADLEFT):
					// set transition values
					g_lpdwBBSwanTransKey1[dwIndex]=KEYF_BBSWAN_HL1TR;
					g_lpdwBBSwanTransKey2[dwIndex]=KEYF_BBSWAN_HL1TL;
					g_lpfBBSwanTransSpeed[dwIndex]=BBSWAN_TRANSITIONSPEED+(GetRandomFloat()*BBSWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head right
				case(ACST_BBSWAN_HEADRIGHT):
					// set transition values
					g_lpdwBBSwanTransKey1[dwIndex]=KEYF_BBSWAN_HR1TR;
					g_lpdwBBSwanTransKey2[dwIndex]=KEYF_BBSWAN_HR1TL;
					g_lpfBBSwanTransSpeed[dwIndex]=BBSWAN_TRANSITIONSPEED+(GetRandomFloat()*BBSWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head left #2
				case(ACST_BBSWAN_HEADLEFT2):
					// set transition values
					g_lpdwBBSwanTransKey1[dwIndex]=KEYF_BBSWAN_HL2TR;
					g_lpdwBBSwanTransKey2[dwIndex]=KEYF_BBSWAN_HL2TL;
					g_lpfBBSwanTransSpeed[dwIndex]=BBSWAN_TRANSITIONSPEED+(GetRandomFloat()*BBSWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head right #2
				case(ACST_BBSWAN_HEADRIGHT2):
					// set transition values
					g_lpdwBBSwanTransKey1[dwIndex]=KEYF_BBSWAN_HR2TR;
					g_lpdwBBSwanTransKey2[dwIndex]=KEYF_BBSWAN_HR2TL;
					g_lpfBBSwanTransSpeed[dwIndex]=BBSWAN_TRANSITIONSPEED+(GetRandomFloat()*BBSWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// look ahead
				default:
					// set transition values
					g_lpdwBBSwanTransKey1[dwIndex]=KEYF_BBSWAN_HCTR;
					g_lpdwBBSwanTransKey2[dwIndex]=KEYF_BBSWAN_HCTL;
					g_lpfBBSwanTransSpeed[dwIndex]=BBSWAN_TRANSITIONSPEED+(GetRandomFloat()*BBSWAN_TRANSITIONSPEED*0.5F);
					
					// ok
					break;
				}

				// set state to transition
				g_lpdwBBSwanActState[dwIndex]=ACST_BBSWAN_TRANSITION;

				// set transition position flag
				g_lpfBBSwanTransPos[dwIndex]=-1.0F;
			}
		
			// ok
			break;
		
		// perform transition
		case(ACST_BBSWAN_TRANSITION):
			// check position flag
			if(g_lpfBBSwanTransPos[dwIndex]<0)
			{
				// set tween value
				g_lpfBBSwanTweenVal[dwIndex]=GetNaturalizedTweenValue((g_lpfBBSwanTailPos[dwIndex]+1.0F)*0.5F);

				// continue until keyframe is reached
				if(IsNearEqual(g_lpfBBSwanTweenVal[dwIndex],1))
					g_lpfBBSwanTransPos[dwIndex]=2;
			}
			else if(g_lpfBBSwanTransPos[dwIndex]>1)
			{
				// set tween value
				g_lpfBBSwanTweenVal[dwIndex]=0;

				// set new keyframes
				g_lpdwBBSwanKeyframe1[dwIndex]=g_lpdwBBSwanKeyframe2[dwIndex];
				g_lpdwBBSwanKeyframe2[dwIndex]=g_lpdwBBSwanTransKey1[dwIndex];

				// set position value
				g_lpfBBSwanTransPos[dwIndex]=0;
			}
			else
			{
				// check for panic mode & increment transition position
				if(g_lpdwBBSwanNavState[dwIndex]==PRST_BBSWAN_PANIC)
					g_lpfBBSwanTransPos[dwIndex]+=(g_lpfBBSwanTransSpeed[dwIndex]*BBSWAN_PANICSPEEDFACTOR);
				else
					g_lpfBBSwanTransPos[dwIndex]+=g_lpfBBSwanTransSpeed[dwIndex];
				
				// check for overflow
				if(g_lpfBBSwanTransPos[dwIndex]>1)
				{
					// set tween value
					g_lpfBBSwanTweenVal[dwIndex]=0;

					// reset tail position & direction
					g_lpfBBSwanTailPos[dwIndex]=-1;
					g_lpfBBSwanTailDir[dwIndex]=fabsf(g_lpfBBSwanTailDir[dwIndex]);

					// set final keyframes
					g_lpdwBBSwanKeyframe1[dwIndex]=g_lpdwBBSwanTransKey1[dwIndex];
					g_lpdwBBSwanKeyframe2[dwIndex]=g_lpdwBBSwanTransKey2[dwIndex];

					// set state to swim
					g_lpdwBBSwanActState[dwIndex]=ACST_BBSWAN_JUSTSWIM;
				}
				else
				{
					// set tween value
					g_lpfBBSwanTweenVal[dwIndex]=GetNaturalizedTweenValue(g_lpfBBSwanTransPos[dwIndex]);
				}
			}
			
			// ok
			break;
		}

		// start ripples if necessary
		if(IsRippleComplete(g_lpdwBBSwanRipple1Idx[dwIndex]))
			StartRipple(g_lpdwBBSwanRipple1Idx[dwIndex],
						g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._41,
						g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._43,
						GetRandomRippleSpeed());
		if(IsRippleComplete(g_lpdwBBSwanRipple2Idx[dwIndex]))
			StartRipple(g_lpdwBBSwanRipple2Idx[dwIndex],
						g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._41,
						g_lpMtx[g_lpdwBBSwanMtx[dwIndex]].mtx._43,
						GetRandomRippleSpeed());
	}
}

// GetNextTurtle()

DWORD GetNextTurtle(void)
{
	LoadingOutput("GetNextTurtle");

	// check counter
	if(g_dwNumTurtle==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwTurtleMtx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwTurtlePriState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwTurtleActState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwTurtleNavState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwTurtleKeyframe1,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwTurtleKeyframe2,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfTurtleTweenVal,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfTurtleTransPos,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfTurtleTransSpeed,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfTurtleNavSpeed,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwTurtleNavPos,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwTurtleNavMax,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwTurtleRipple1Idx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwTurtleRipple2Idx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxTurtle=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumTurtle>=g_dwMaxTurtle)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwTurtleMtx,
					   sizeof(DWORD)*g_dwMaxTurtle,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwTurtlePriState,
					   sizeof(DWORD)*g_dwMaxTurtle,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwTurtleActState,
					   sizeof(DWORD)*g_dwMaxTurtle,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwTurtleNavState,
					   sizeof(DWORD)*g_dwMaxTurtle,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwTurtleKeyframe1,
					   sizeof(DWORD)*g_dwMaxTurtle,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwTurtleKeyframe2,
					   sizeof(DWORD)*g_dwMaxTurtle,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfTurtleTweenVal,
					   sizeof(float)*g_dwMaxTurtle,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfTurtleTransPos,
					   sizeof(float)*g_dwMaxTurtle,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfTurtleTransSpeed,
					   sizeof(float)*g_dwMaxTurtle,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfTurtleNavSpeed,
					   sizeof(float)*g_dwMaxTurtle,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwTurtleNavPos,
					   sizeof(DWORD)*g_dwMaxTurtle,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwTurtleNavMax,
					   sizeof(DWORD)*g_dwMaxTurtle,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwTurtleRipple1Idx,
					   sizeof(DWORD)*g_dwMaxTurtle,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwTurtleRipple2Idx,
					   sizeof(DWORD)*g_dwMaxTurtle,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxTurtle+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumTurtle;

	// increment counter
	g_dwNumTurtle++;

	// return index
	return(dwIndex);
}

// AddTurtle()

DWORD AddTurtle(void)
{
	LoadingOutput("AddTurtle");

	// get return index
	DWORD dwIndex=GetNextTurtle();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// add turtle matrix
	g_lpdwTurtleMtx[dwIndex]=AddMatrix(1,0,0,0,
									   0,1,0,0,
									   0,0,1,0,
									   0,TURTLE_MAXDIVEDEPTH,0,1);

	// check matrix
	if(g_lpdwTurtleMtx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// scale matrix by turtle scale
	MatrixScale(g_lpdwTurtleMtx[dwIndex],
				TURTLE_SCALE+(GetRandomFloat()*TURTLE_SCALEVAR));

	// set default values
	g_lpdwTurtlePriState[dwIndex]=PRST_TURTLE_TRANSFER;
	g_lpdwTurtleActState[dwIndex]=ACST_TURTLE_JUSTSWIM;
	g_lpdwTurtleNavState[dwIndex]=NVST_TURTLE_JUSTSWIM;
	g_lpdwTurtleKeyframe1[dwIndex]=KEYF_TURTLE_HC;
	g_lpdwTurtleKeyframe2[dwIndex]=KEYF_TURTLE_HC;
	g_lpfTurtleTweenVal[dwIndex]=0;
	g_lpfTurtleTransPos[dwIndex]=0;
	g_lpfTurtleTransSpeed[dwIndex]=0;
	g_lpfTurtleNavSpeed[dwIndex]=TURTLE_MINSPEED;
	g_lpdwTurtleNavPos[dwIndex]=0;
	g_lpdwTurtleNavMax[dwIndex]=0;

	// add ripple #1
	g_lpdwTurtleRipple1Idx[dwIndex]=AddRipple();

	// check ripple #1
	if(g_lpdwTurtleRipple1Idx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// add ripple #2
	g_lpdwTurtleRipple2Idx[dwIndex]=AddRipple();

	// check ripple #2
	if(g_lpdwTurtleRipple2Idx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// return index
	return(dwIndex);
}

// UpdateTurtles()

void UpdateTurtles(void)
{
	// turtle index
	DWORD dwIndex;

	// navigation update flag
	BOOL bNavFlag;

	// coordinate values
	float fX;
	float fZ;

	// comparison vector
	D3DXVECTOR3 vV1;

	// counter
	DWORD dwCount;

	// polar coordinates
	float fAngle;
	float fDist;

	// update turtles
	for(dwIndex=0;dwIndex<g_dwNumTurtle;dwIndex++)
	{
		// set coordinate values
		fX=g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._41;
		fZ=g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._43;

		// check for transfer state
		if(g_lpdwTurtlePriState[dwIndex]!=PRST_TURTLE_TRANSFER)
		{
			// compare turtle position with other turtles
			for(dwCount=0;dwCount<g_dwNumTurtle;dwCount++)
			{
				// skip this turtle
				if(dwCount!=dwIndex)
				{
					// set up vector
					vV1.x=fX-g_lpMtx[g_lpdwTurtleMtx[dwCount]].mtx._41;
					vV1.z=fZ-g_lpMtx[g_lpdwTurtleMtx[dwCount]].mtx._43;

					// check distance
					if(SquareMagnitudeXZ(&vV1)<Squared(TURTLE_MINTURTLEDISTANCE))
					{
						// set primary state to dive
						g_lpdwTurtlePriState[dwIndex]=PRST_TURTLE_DIVE;
					}
				}
			}

			// compare turtle position with swans
			for(dwCount=0;dwCount<g_dwNumSwan;dwCount++)
			{
				// set up vector
				vV1.x=fX-g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._41;
				vV1.z=fZ-g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._43;

				// check distance
				if(SquareMagnitudeXZ(&vV1)<Squared(TURTLE_MINSWANDISTANCE))
				{
					// set primary state to dive
					g_lpdwTurtlePriState[dwIndex]=PRST_TURTLE_DIVE;
				}
			}

			// compare turtle position with baby swans
			for(dwCount=0;dwCount<g_dwNumBBSwan;dwCount++)
			{
				// set up vector
				vV1.x=fX-g_lpMtx[g_lpdwBBSwanMtx[dwCount]].mtx._41;
				vV1.z=fZ-g_lpMtx[g_lpdwBBSwanMtx[dwCount]].mtx._43;

				// check distance
				if(SquareMagnitudeXZ(&vV1)<Squared(TURTLE_MINBBSWANDISTANCE))
				{
					// set primary state to dive
					g_lpdwTurtlePriState[dwIndex]=PRST_TURTLE_DIVE;
				}
			}

			// check boundaries
			if(IsPointOutOfBounds(fX,fZ))
			{
				// set primary state to dive
				g_lpdwTurtlePriState[dwIndex]=PRST_TURTLE_DIVE;
			}
		}

		// check primary state
		switch(g_lpdwTurtlePriState[dwIndex])
		{
		// surface
		case(PRST_TURTLE_SURFACE):
			// check navigation state
			if(g_lpdwTurtleNavState[dwIndex]==NVST_TURTLE_JUSTSWIM)
			{
				// change navigation state if necessary
				if((abs(GetRandomInt())%TURTLE_NAVIGATIONCHANGERATIO)==1)
				{
					// set new state
					g_lpdwTurtleNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%TURTLE_NUMNAVIGATIONSTATES);

					// set navigation values
					g_lpdwTurtleNavPos[dwIndex]=0;
					g_lpdwTurtleNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%TURTLE_NAVIGATIONMAXINC)+1);
				}
			}

			// surface
			g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._42+=TURTLE_DIVEINC;

			// check for overflow
			if(g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._42>0)
			{
				// clip value
				g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._42=0;

				// set state to swim
				g_lpdwTurtlePriState[dwIndex]=PRST_TURTLE_JUSTSWIM;
			}

			// ok
			break;

		// dive
		case(PRST_TURTLE_DIVE):
			// check navigation state
			if(g_lpdwTurtleNavState[dwIndex]==NVST_TURTLE_JUSTSWIM)
			{
				// change navigation state if necessary
				if((abs(GetRandomInt())%TURTLE_NAVIGATIONCHANGERATIO)==1)
				{
					// set new state
					g_lpdwTurtleNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%TURTLE_NUMNAVIGATIONSTATES);

					// set navigation values
					g_lpdwTurtleNavPos[dwIndex]=0;
					g_lpdwTurtleNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%TURTLE_NAVIGATIONMAXINC)+1);
				}
			}

			// dive
			g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._42-=TURTLE_DIVEINC;

			// check for overflow
			if(g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._42<TURTLE_MAXDIVEDEPTH)
			{
				// clip value
				g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._42=TURTLE_MAXDIVEDEPTH;

				// set state to transfer
				g_lpdwTurtlePriState[dwIndex]=PRST_TURTLE_TRANSFER;
			}

			// ok
			break;

		// transfer
		case(PRST_TURTLE_TRANSFER):
			// find new position if necessary
			if((abs(GetRandomInt())%TURTLE_TRANSFERRATIO)==1)
			{
				// create random position
				switch(abs(GetRandomInt())%3)
				{
				// upper round portion of boundary
				case(0):
					// set polar coordinates
					fAngle=fabsf(GetRandomFloat()*PI);
					fDist=fabsf(GetRandomFloat()*TERRAIN_XRANGE);
					
					// set position
					fX=TERRAIN_CENTERX-(cosf(fAngle)*fDist);
					fZ=(TERRAIN_CENTERZ-TERRAIN_ZRANGE)-(sinf(fAngle)*fDist);

					// ok
					break;

				// center rectangular portion of boundary
				case(1):
					// set position
					fX=TERRAIN_CENTERX+(GetRandomFloat()*TERRAIN_XRANGE);
					fZ=TERRAIN_CENTERZ+(GetRandomFloat()*TERRAIN_ZRANGE);

					// ok
					break;

				// lower round portion of boundary
				case(2):
					// set polar coordinates
					fAngle=fabsf(GetRandomFloat()*PI);
					fDist=fabsf(GetRandomFloat()*TERRAIN_XRANGE);
					
					// set position
					fX=TERRAIN_CENTERX+(cosf(fAngle)*fDist);
					fZ=(TERRAIN_CENTERZ+TERRAIN_ZRANGE)+(sinf(fAngle)*fDist);

					// ok
					break;
				}

				// set turtle position
				g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._41=fX;
				g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._43=fZ;

				// randomize yaw
				for(dwCount=0;dwCount<8;dwCount++)
					MatrixYaw(g_lpdwTurtleMtx[dwIndex],
							  fabsf(GetRandomFloat()));

				// set primary state to surface
				g_lpdwTurtlePriState[dwIndex]=PRST_TURTLE_SURFACE;
			}

			// ok
			break;

		// just swim
		case(PRST_TURTLE_JUSTSWIM):
			// check navigation state
			if(g_lpdwTurtleNavState[dwIndex]==NVST_TURTLE_JUSTSWIM)
			{
				// change navigation state if necessary
				if((abs(GetRandomInt())%TURTLE_NAVIGATIONCHANGERATIO)==1)
				{
					// set new state
					g_lpdwTurtleNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%TURTLE_NUMNAVIGATIONSTATES);

					// set navigation values
					g_lpdwTurtleNavPos[dwIndex]=0;
					g_lpdwTurtleNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%TURTLE_NAVIGATIONMAXINC)+1);
				}
			}

			// ok
			break;
		}

		// check navigation state
		if(g_lpdwTurtleNavState[dwIndex]!=NVST_TURTLE_JUSTSWIM)
		{
			// check navigation position
			if(g_lpdwTurtleNavPos[dwIndex]<g_lpdwTurtleNavMax[dwIndex])
			{
				// set flag
				bNavFlag=TRUE;

				// increment navigation position
				g_lpdwTurtleNavPos[dwIndex]++;
			}
			else
			{
				// reset flag
				bNavFlag=FALSE;
			}
		}

		// check navigation state
		switch(g_lpdwTurtleNavState[dwIndex])
		{
		// speed up
		case(NVST_TURTLE_SPEEDUP):
			// increase speed
			g_lpfTurtleNavSpeed[dwIndex]+=TURTLE_SPEEDINC;

			// check for overflow
			if(g_lpfTurtleNavSpeed[dwIndex]>TURTLE_MAXSPEED)
				g_lpfTurtleNavSpeed[dwIndex]=TURTLE_MAXSPEED;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwTurtleNavState[dwIndex]=NVST_TURTLE_JUSTSWIM;
			}

			// ok
			break;

		// slow down
		case(NVST_TURTLE_SLOWDOWN):
			// decrease speed
			g_lpfTurtleNavSpeed[dwIndex]-=TURTLE_SPEEDINC;

			// check for overflow
			if(g_lpfTurtleNavSpeed[dwIndex]<TURTLE_MINSPEED)
				g_lpfTurtleNavSpeed[dwIndex]=TURTLE_MINSPEED;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwTurtleNavState[dwIndex]=NVST_TURTLE_JUSTSWIM;
			}

			// ok
			break;

		// turn left
		case(NVST_TURTLE_TURNLEFT):
			// turn left
			MatrixYaw(g_lpdwTurtleMtx[dwIndex],
					  TURTLE_TURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwTurtleNavState[dwIndex]=NVST_TURTLE_JUSTSWIM;
			}

			// ok
			break;

		// turn right
		case(NVST_TURTLE_TURNRIGHT):
			// turn right
			MatrixYaw(g_lpdwTurtleMtx[dwIndex],
					  -TURTLE_TURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to swim
				g_lpdwTurtleNavState[dwIndex]=NVST_TURTLE_JUSTSWIM;
			}

			// ok
			break;
		}

		// move turtle
		MatrixBack(g_lpdwTurtleMtx[dwIndex],
				   g_lpfTurtleNavSpeed[dwIndex]);

		// check action state
		switch(g_lpdwTurtleActState[dwIndex])
		{
		// just swim
		case(ACST_TURTLE_JUSTSWIM):
			// change states if necessary
			if((abs(GetRandomInt())%TURTLE_ACTIONCHANGERATIO)==1)
			{
				// copy current keyframe to first keyframe
				g_lpdwTurtleKeyframe1[dwIndex]=g_lpdwTurtleKeyframe2[dwIndex];

				// reset tween value
				g_lpfTurtleTweenVal[dwIndex]=0;

				switch(abs(GetRandomInt())%TURTLE_NUMACTIONSTATES)
				{
				// head up
				case(ACST_TURTLE_HEADUP):
					// set transition values
					g_lpdwTurtleKeyframe2[dwIndex]=KEYF_TURTLE_HU;
					g_lpfTurtleTransSpeed[dwIndex]=TURTLE_TRANSITIONSPEED+(GetRandomFloat()*TURTLE_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head down
				case(ACST_TURTLE_HEADDOWN):
					// set transition values
					g_lpdwTurtleKeyframe2[dwIndex]=KEYF_TURTLE_HD;
					g_lpfTurtleTransSpeed[dwIndex]=TURTLE_TRANSITIONSPEED+(GetRandomFloat()*TURTLE_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head left
				case(ACST_TURTLE_HEADLEFT):
					// set transition values
					g_lpdwTurtleKeyframe2[dwIndex]=KEYF_TURTLE_HL;
					g_lpfTurtleTransSpeed[dwIndex]=TURTLE_TRANSITIONSPEED+(GetRandomFloat()*TURTLE_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// head right
				case(ACST_TURTLE_HEADRIGHT):
					// set transition values
					g_lpdwTurtleKeyframe2[dwIndex]=KEYF_TURTLE_HR;
					g_lpfTurtleTransSpeed[dwIndex]=TURTLE_TRANSITIONSPEED+(GetRandomFloat()*TURTLE_TRANSITIONSPEED*0.5F);
					
					// ok
					break;

				// look ahead
				default:
					// set transition values
					g_lpdwTurtleKeyframe2[dwIndex]=KEYF_TURTLE_HC;
					g_lpfTurtleTransSpeed[dwIndex]=TURTLE_TRANSITIONSPEED+(GetRandomFloat()*TURTLE_TRANSITIONSPEED*0.5F);
					
					// ok
					break;
				}

				// set state to transition
				g_lpdwTurtleActState[dwIndex]=ACST_TURTLE_TRANSITION;

				// set transition position
				g_lpfTurtleTransPos[dwIndex]=0;
			}
		
			// ok
			break;
		
		// perform transition
		case(ACST_TURTLE_TRANSITION):
			// increment transition position
			g_lpfTurtleTransPos[dwIndex]+=g_lpfTurtleTransSpeed[dwIndex];

			// check for overflow
			if(g_lpfTurtleTransPos[dwIndex]>1)
			{
				// set tween value
				g_lpfTurtleTweenVal[dwIndex]=1;

				// set state to swim
				g_lpdwTurtleActState[dwIndex]=ACST_TURTLE_JUSTSWIM;
			}
			else
			{
				// set tween value
				g_lpfTurtleTweenVal[dwIndex]=GetNaturalizedTweenValue(g_lpfTurtleTransPos[dwIndex]);
			}
			
			// ok
			break;
		}

		// start ripples if necessary
		if(IsRippleComplete(g_lpdwTurtleRipple1Idx[dwIndex]))
			StartRipple(g_lpdwTurtleRipple1Idx[dwIndex],
						g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._41,
						g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._43,
						GetRandomRippleSpeed());
		if(IsRippleComplete(g_lpdwTurtleRipple2Idx[dwIndex]))
			StartRipple(g_lpdwTurtleRipple2Idx[dwIndex],
						g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._41,
						g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._43,
						GetRandomRippleSpeed());
	}
}

// RenderTurtles()

void RenderTurtles(void)
{
	// turtle index
	DWORD dwIndex;

	// y-value
	float fY;

	// alpha value
	float fAlpha;

#ifndef REFLECTION_OPTIMIZATION
	// render turtles
	for(dwIndex=0;dwIndex<g_dwNumTurtle;dwIndex++)
	{
		// set keyframe
		UpdateRenderFrames(g_dwTurtleAnim,
						   g_lpdwTurtleKeyframe1[dwIndex],
						   g_lpdwTurtleKeyframe2[dwIndex],
						   g_lpfTurtleTweenVal[dwIndex]);

		// get y-value
		fY=g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._42;
		
		// compute alpha value
		if(fY>(TURTLE_MAXDIVEDEPTH*0.5F))
			fAlpha=1;
		else
			fAlpha=1-((fY-(TURTLE_MAXDIVEDEPTH*0.5F))/(TURTLE_MAXDIVEDEPTH*0.5F));
		
		// render turtle
		RenderAnimationAlpha(g_dwTurtleAnim,
							 g_lpdwTurtleMtx[dwIndex],
							 fAlpha);
	}

	// enable frontface cull
	EnableFrontfaceCull();
	
	// render turtle reflections
	for(dwIndex=0;dwIndex<g_dwNumTurtle;dwIndex++)
	{
		// invert y-axis
		g_lpMtx[g_lpdwTurtleMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwTurtleMtx[dwIndex]].fYScale;

		// set keyframe
		UpdateRenderFrames(g_dwTurtleAnim,
						   g_lpdwTurtleKeyframe1[dwIndex],
						   g_lpdwTurtleKeyframe2[dwIndex],
						   g_lpfTurtleTweenVal[dwIndex]);
		
		// get y-value
		fY=g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._42;
		
		// compute alpha value
		if(fY>(TURTLE_MAXDIVEDEPTH*0.5F))
			fAlpha=1-(fY/(TURTLE_MAXDIVEDEPTH*0.5F));
		else
			fAlpha=0;

		
		// render turtle
		RenderAnimationAlpha(g_dwTurtleAnim,
							 g_lpdwTurtleMtx[dwIndex],
							 fAlpha);
		
		// reset y-axis
		g_lpMtx[g_lpdwTurtleMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwTurtleMtx[dwIndex]].fYScale;
	}
	
	// enable backface cull
	EnableBackfaceCull();
#else
	// render turtles & reflections
	for(dwIndex=0;dwIndex<g_dwNumTurtle;dwIndex++)
	{
		// set keyframe
		UpdateRenderFrames(g_dwTurtleAnim,
						   g_lpdwTurtleKeyframe1[dwIndex],
						   g_lpdwTurtleKeyframe2[dwIndex],
						   g_lpfTurtleTweenVal[dwIndex]);

		// get y-value
		fY=g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._42;
		
		// compute alpha value
		if(fY>(TURTLE_MAXDIVEDEPTH*0.5F))
			fAlpha=1;
		else
			fAlpha=1-((fY-(TURTLE_MAXDIVEDEPTH*0.5F))/(TURTLE_MAXDIVEDEPTH*0.5F));
		
		// render turtle
		RenderAnimationAlpha(g_dwTurtleAnim,
							 g_lpdwTurtleMtx[dwIndex],
							 fAlpha);
		// enable frontface cull
		EnableFrontfaceCull();
	
		// invert y-axis
		g_lpMtx[g_lpdwTurtleMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwTurtleMtx[dwIndex]].fYScale;

		// get y-value
		fY=g_lpMtx[g_lpdwTurtleMtx[dwIndex]].mtx._42;
		
		// compute alpha value
		if(fY>(TURTLE_MAXDIVEDEPTH*0.5F))
			fAlpha=1-(fY/(TURTLE_MAXDIVEDEPTH*0.5F));
		else
			fAlpha=0;

		// render turtle
		RenderAnimationAlpha(g_dwTurtleAnim,
							 g_lpdwTurtleMtx[dwIndex],
							 fAlpha);
		
		// reset y-axis
		g_lpMtx[g_lpdwTurtleMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwTurtleMtx[dwIndex]].fYScale;
	
		// enable backface cull
		EnableBackfaceCull();
	}
#endif
}

// PurgeTurtles()

void PurgeTurtles(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpdwTurtleMtx);
	FreeMem((LPVOID*)&g_lpdwTurtlePriState);
	FreeMem((LPVOID*)&g_lpdwTurtleActState);
	FreeMem((LPVOID*)&g_lpdwTurtleNavState);
	FreeMem((LPVOID*)&g_lpdwTurtleKeyframe1);
	FreeMem((LPVOID*)&g_lpdwTurtleKeyframe2);
	FreeMem((LPVOID*)&g_lpfTurtleTweenVal);
	FreeMem((LPVOID*)&g_lpfTurtleTransPos);
	FreeMem((LPVOID*)&g_lpfTurtleTransSpeed);
	FreeMem((LPVOID*)&g_lpfTurtleNavSpeed);
	FreeMem((LPVOID*)&g_lpdwTurtleNavPos);
	FreeMem((LPVOID*)&g_lpdwTurtleNavMax);
	FreeMem((LPVOID*)&g_lpdwTurtleRipple1Idx);
	FreeMem((LPVOID*)&g_lpdwTurtleRipple2Idx);

	// reset counters
	g_dwNumTurtle=0;
	g_dwMaxTurtle=0;
}

// GetNextDragonfly()

DWORD GetNextDragonfly(void)
{
	LoadingOutput("GetNextDragonfly");

	// check counter
	if(g_dwNumDFly==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwDFlyMtx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDFlyPriState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDFlyNavState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfDFlyNavSpeed,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDFlyNavPos,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDFlyNavMax,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpvDFlyDartVec,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDFlyMateIdx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDFlyAPathID,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDFlyBuzzIdx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbDFlyPlayback,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDFly=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumDFly>=g_dwMaxDFly)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwDFlyMtx,
					   sizeof(DWORD)*g_dwMaxDFly,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDFlyPriState,
					   sizeof(DWORD)*g_dwMaxDFly,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDFlyNavState,
					   sizeof(DWORD)*g_dwMaxDFly,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfDFlyNavSpeed,
					   sizeof(float)*g_dwMaxDFly,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDFlyNavPos,
					   sizeof(DWORD)*g_dwMaxDFly,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDFlyNavMax,
					   sizeof(DWORD)*g_dwMaxDFly,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpvDFlyDartVec,
					   sizeof(D3DXVECTOR3)*g_dwMaxDFly,
					   sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDFlyMateIdx,
					   sizeof(DWORD)*g_dwMaxDFly,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDFlyAPathID,
					   sizeof(DWORD)*g_dwMaxDFly,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDFlyBuzzIdx,
					   sizeof(DWORD)*g_dwMaxDFly,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbDFlyPlayback,
					   sizeof(BOOL)*g_dwMaxDFly,
					   sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDFly+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumDFly;

	// increment counter
	g_dwNumDFly++;

	// return index
	return(dwIndex);
}

// AddDragonfly()

DWORD AddDragonfly(void)
{
	LoadingOutput("AddDragonfly");

	// get return index
	DWORD dwIndex=GetNextDragonfly();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// add dragonfly matrix
	g_lpdwDFlyMtx[dwIndex]=AddMatrix();

	// check matrix
	if(g_lpdwDFlyMtx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// scale matrix by dragonfly scale
	MatrixScale(g_lpdwDFlyMtx[dwIndex],
				DRGFLY_SCALE+(GetRandomFloat()*DRGFLY_SCALEVAR));

	// set height of dragonfly
	MatrixUp(g_lpdwDFlyMtx[dwIndex],
			 DFLY_MINHEIGHT+((DFLY_MAXHEIGHT-DFLY_MINHEIGHT)*fabsf(GetRandomFloat())));

	// set default values
	g_lpdwDFlyPriState[dwIndex]=PRST_DFLY_REST;
	g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_JUSTFLY;
	g_lpfDFlyNavSpeed[dwIndex]=0;
	g_lpdwDFlyNavPos[dwIndex]=0;
	g_lpdwDFlyNavMax[dwIndex]=0;
	g_lpvDFlyDartVec[dwIndex]=D3DXVECTOR3(0,0,0);
	g_lpdwDFlyMateIdx[dwIndex]=NO_OBJECT;
	g_lpdwDFlyAPathID[dwIndex]=NO_OBJECT;
	g_lpdwDFlyBuzzIdx[dwIndex]=NO_OBJECT;
	g_lpbDFlyPlayback[dwIndex]=TRUE;

	// return index
	return(dwIndex);
}

// SetDragonflyMatrix()

void SetDragonflyMatrix(DWORD dwIndex,
						LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDFly)
	{
		DisplayFatalErrorMessage("Invalid dragonfly index.",
								 "SetDragonflyMatrix()",
								 FALSE);
		return;
	}
#endif

	// set matrix
	SetMatrix(g_lpdwDFlyMtx[dwIndex],
			  lpMtx);
}

// PurgeDragonflies()

void PurgeDragonflies(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpdwDFlyMtx);
	FreeMem((LPVOID*)&g_lpdwDFlyPriState);
	FreeMem((LPVOID*)&g_lpdwDFlyNavState);
	FreeMem((LPVOID*)&g_lpfDFlyNavSpeed);
	FreeMem((LPVOID*)&g_lpdwDFlyNavPos);
	FreeMem((LPVOID*)&g_lpdwDFlyNavMax);
	FreeMem((LPVOID*)&g_lpvDFlyDartVec);
	FreeMem((LPVOID*)&g_lpdwDFlyMateIdx);
	FreeMem((LPVOID*)&g_lpdwDFlyAPathID);
	FreeMem((LPVOID*)&g_lpdwDFlyBuzzIdx);
	FreeMem((LPVOID*)&g_lpbDFlyPlayback);

	// reset counters
	g_dwNumDFly=0;
	g_dwMaxDFly=0;
}

// RenderDragonfliesA()

void RenderDragonfliesA(void)
{
	// dragonfly index
	DWORD dwIndex;

	// render dragonflies
	for(dwIndex=0;dwIndex<g_dwNumDFly;dwIndex++)
	{
		// render dragonfly body
		RenderMesh(g_dwDragonflyMesh,
				   g_lpdwDFlyMtx[dwIndex]);
	}

	// enable frontface cull
	EnableFrontfaceCull();
	
	// render dragonfly reflections
	for(dwIndex=0;dwIndex<g_dwNumDFly;dwIndex++)
	{
		// invert y-axis
		g_lpMtx[g_lpdwDFlyMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwDFlyMtx[dwIndex]].fYScale;

		// invert y-position
		g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42=-g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42;

		// render dragonfly body
		RenderMesh(g_dwDragonflyMesh,
				   g_lpdwDFlyMtx[dwIndex]);

		// set keyframe for upper wings
		UpdateRenderFrames(g_dwDragonflyWingAnim,
						   0,
						   1,
						   fabsf(GetRandomFloat()));

		// render upper wings
		RenderAnimationAlpha(g_dwDragonflyWingAnim,
							 g_lpdwDFlyMtx[dwIndex],
							 DFLY_WINGALPHA);

		// set keyframe for lower wings
		UpdateRenderFrames(g_dwDragonflyWingAnim,
						   0,
						   2,
						   fabsf(GetRandomFloat()));

		// render lower wings
		RenderAnimationAlpha(g_dwDragonflyWingAnim,
							 g_lpdwDFlyMtx[dwIndex],
							 DFLY_WINGALPHA);
		
		// reset y-position
		g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42=-g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42;

		// reset y-axis
		g_lpMtx[g_lpdwDFlyMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwDFlyMtx[dwIndex]].fYScale;
	}
	
	// enable backface cull
	EnableBackfaceCull();
}

// RenderDragonfliesB()

void RenderDragonfliesB(void)
{
	// dragonfly index
	DWORD dwIndex;

	// render dragonfly wings
	for(dwIndex=0;dwIndex<g_dwNumDFly;dwIndex++)
	{
		// set keyframe for upper wings
		UpdateRenderFrames(g_dwDragonflyWingAnim,
						   0,
						   1,
						   fabsf(GetRandomFloat()));

		// render upper wings
		RenderAnimationAlpha(g_dwDragonflyWingAnim,
							 g_lpdwDFlyMtx[dwIndex],
							 DFLY_WINGALPHA);

		// set keyframe for lower wings
		UpdateRenderFrames(g_dwDragonflyWingAnim,
						   0,
						   2,
						   fabsf(GetRandomFloat()));

		// render lower wings
		RenderAnimationAlpha(g_dwDragonflyWingAnim,
							 g_lpdwDFlyMtx[dwIndex],
							 DFLY_WINGALPHA);
	}
}

// AddDragonflyLoner()

DWORD AddDragonflyLoner(void)
{
	LoadingOutput("AddDragonflyLoner");

	// dragonfly index
	DWORD dwIndex;

	// polar coordinate values
	float fAngle;
	float fDist;
	
	// position values
	float fX;
	float fZ;

	// matrix
	D3DXMATRIX mtx;

	// counter
	DWORD dwCount;

	// done flag
	BOOL bDone;
	
	// add dragonfly
	dwIndex=AddDragonfly();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// reset done flag
	bDone=FALSE;

	// loop until suitable position is found
	while(!bDone)
	{
		// create random position
		switch(abs(GetRandomInt())%3)
		{
		// upper round portion of boundary
		case(0):
			// set polar coordinates
			fAngle=fabsf(GetRandomFloat()*PI);
			fDist=fabsf(GetRandomFloat()*TERRAIN_XRANGE);
			
			// set position
			fX=TERRAIN_CENTERX-(cosf(fAngle)*fDist);
			fZ=(TERRAIN_CENTERZ-TERRAIN_ZRANGE)-(sinf(fAngle)*fDist);

			// ok
			break;

		// center rectangular portion of boundary
		case(1):
			// set position
			fX=TERRAIN_CENTERX+(GetRandomFloat()*TERRAIN_XRANGE);
			fZ=TERRAIN_CENTERZ+(GetRandomFloat()*TERRAIN_ZRANGE);

			// ok
			break;

		// lower round portion of boundary
		case(2):
			// set polar coordinates
			fAngle=fabsf(GetRandomFloat()*PI);
			fDist=fabsf(GetRandomFloat()*TERRAIN_XRANGE);
			
			// set position
			fX=TERRAIN_CENTERX+(cosf(fAngle)*fDist);
			fZ=(TERRAIN_CENTERZ+TERRAIN_ZRANGE)+(sinf(fAngle)*fDist);

			// ok
			break;
		}

		// assume that we will be done
		bDone=TRUE;
		
		// compare position with other dragonflies
		for(dwCount=0;dwCount<g_dwNumDFly;dwCount++)
		{
			// skip this dragonfly
			if(dwCount!=dwIndex)
			{
				// get dragonfly position
				float fDFlyX=g_lpMtx[g_lpdwDFlyMtx[dwCount]].mtx._41;
				float fDFlyZ=g_lpMtx[g_lpdwDFlyMtx[dwCount]].mtx._43;

				// compare distance with separation distance
				if(SquareMagnitudeXZ(fDFlyX-fX,fDFlyZ-fZ)<Squared(DFLY_SEPARATIONDISTANCE))
				{
					// reset done fkag
					bDone=FALSE;

					// end loop
					dwCount=g_dwNumDFly;
				}
			}
		}
	}

	// set dragonfly position
	g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._41=fX;
	g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._43=fZ;

	// randomize yaw
	for(dwCount=0;dwCount<8;dwCount++)
		MatrixYaw(g_lpdwDFlyMtx[dwIndex],
				  fabsf(GetRandomFloat()));

	// set primary state
	g_lpdwDFlyPriState[dwIndex]=(DWORD)(abs(GetRandomInt())%DFLY_NUMPRIMARYSTATES);

	// return index
	return(dwIndex);
}

// AddDragonflyCouple()

DWORD AddDragonflyCouple(void)
{
	LoadingOutput("AddDragonflyCouple");

	// dragonfly indices
	DWORD dwIndex1;
	DWORD dwIndex2;

	// counter
	DWORD dwCount;

	// add dragonfly #1
	dwIndex1=AddDragonflyLoner();

	// check dragonfly #1
	if(dwIndex1==NO_OBJECT)
		return(NO_OBJECT);

	// add dragonfly #2
	dwIndex2=AddDragonfly();

	// check dragonfly #2
	if(dwIndex2==NO_OBJECT)
		return(NO_OBJECT);

	// set mate values
	g_lpdwDFlyMateIdx[dwIndex1]=dwIndex2;
	g_lpdwDFlyMateIdx[dwIndex2]=dwIndex1;

	// set primary state & position of dragonfly #2
	switch(g_lpdwDFlyPriState[dwIndex1])
	{
	// dragonfly #1 leading
	case(PRST_DFLY_LEAD):
		// set dragonfly #2 to follow
		g_lpdwDFlyPriState[dwIndex2]=PRST_DFLY_FOLLOW;

		// copy dragonfly #1's position to dragonfly #2
		g_lpMtx[g_lpdwDFlyMtx[dwIndex2]].mtx._41=g_lpMtx[g_lpdwDFlyMtx[dwIndex1]].mtx._41;
		g_lpMtx[g_lpdwDFlyMtx[dwIndex2]].mtx._42=g_lpMtx[g_lpdwDFlyMtx[dwIndex1]].mtx._42;
		g_lpMtx[g_lpdwDFlyMtx[dwIndex2]].mtx._43=g_lpMtx[g_lpdwDFlyMtx[dwIndex1]].mtx._43;

		// randomize yaw of dragonfly #2
		for(dwCount=0;dwCount<8;dwCount++)
			MatrixYaw(g_lpdwDFlyMtx[dwIndex2],
					  fabsf(GetRandomFloat()));

		// move dragonfly #2 back by follow distance
		MatrixAhead(g_lpdwDFlyMtx[dwIndex2],
					DFLY_MAXFOLLOWDISTANCE);

		// ok
		break;

	// dragonfly #1 following
	case(PRST_DFLY_FOLLOW):
		// set dragonfly #2 to lead
		g_lpdwDFlyPriState[dwIndex2]=PRST_DFLY_LEAD;

		// copy dragonfly #1's position to dragonfly #2
		g_lpMtx[g_lpdwDFlyMtx[dwIndex2]].mtx._41=g_lpMtx[g_lpdwDFlyMtx[dwIndex1]].mtx._41;
		g_lpMtx[g_lpdwDFlyMtx[dwIndex2]].mtx._42=g_lpMtx[g_lpdwDFlyMtx[dwIndex1]].mtx._42;
		g_lpMtx[g_lpdwDFlyMtx[dwIndex2]].mtx._43=g_lpMtx[g_lpdwDFlyMtx[dwIndex1]].mtx._43;

		// randomize yaw of dragonfly #2
		for(dwCount=0;dwCount<8;dwCount++)
			MatrixYaw(g_lpdwDFlyMtx[dwIndex2],
					  fabsf(GetRandomFloat()));

		// move dragonfly #1 back by follow distance
		MatrixAhead(g_lpdwDFlyMtx[dwIndex1],
					DFLY_MAXFOLLOWDISTANCE);

		// ok
		break;

	// dragonfly #1 resting
	case(PRST_DFLY_REST):
		// set dragonfly #2 to rest
		g_lpdwDFlyPriState[dwIndex2]=PRST_DFLY_REST;

		// copy dragonfly #1's position to dragonfly #2
		g_lpMtx[g_lpdwDFlyMtx[dwIndex2]].mtx._41=g_lpMtx[g_lpdwDFlyMtx[dwIndex1]].mtx._41;
		g_lpMtx[g_lpdwDFlyMtx[dwIndex2]].mtx._42=g_lpMtx[g_lpdwDFlyMtx[dwIndex1]].mtx._42;
		g_lpMtx[g_lpdwDFlyMtx[dwIndex2]].mtx._43=g_lpMtx[g_lpdwDFlyMtx[dwIndex1]].mtx._43;

		// randomize yaw of dragonfly #2
		for(dwCount=0;dwCount<8;dwCount++)
			MatrixYaw(g_lpdwDFlyMtx[dwIndex2],
					  fabsf(GetRandomFloat()));

		// move dragonfly #2 back by follow distance
		MatrixAhead(g_lpdwDFlyMtx[dwIndex2],
					DFLY_MAXFOLLOWDISTANCE);

		// ok
		break;
	}

	// set height of dragonfly #2
	g_lpMtx[g_lpdwDFlyMtx[dwIndex2]].mtx._42=DFLY_MINHEIGHT+((DFLY_MAXHEIGHT-DFLY_MINHEIGHT)*fabsf(GetRandomFloat()));

	// return index #1
	return(dwIndex1);
}

// RandomizeDragonflyPrimaryState()

void RandomizeDragonflyPrimaryState(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDFly)
	{
		DisplayFatalErrorMessage("Invalid dragonfly index.",
								 "RandomizeDragonflyPrimaryState()",
								 FALSE);
		return;
	}
#endif

	// set new state
	g_lpdwDFlyPriState[dwIndex]=(DWORD)(abs(GetRandomInt())%DFLY_NUMPRIMARYSTATES);

	// check for mate
	if(g_lpdwDFlyMateIdx[dwIndex]!=NO_OBJECT)
	{
		// set mate's state accordingly
		switch(g_lpdwDFlyPriState[dwIndex])
		{
		// this dragonfly leading
		case(PRST_DFLY_LEAD):
			// set mate to follow
			g_lpdwDFlyPriState[g_lpdwDFlyMateIdx[dwIndex]]=PRST_DFLY_FOLLOW;

			// ok
			break;

		// this dragonfly following
		case(PRST_DFLY_FOLLOW):
			// set mate to lead
			g_lpdwDFlyPriState[g_lpdwDFlyMateIdx[dwIndex]]=PRST_DFLY_LEAD;

			// ok
			break;

		// this dragonfly resting
		case(PRST_DFLY_REST):
			// set mate to rest
			g_lpdwDFlyPriState[g_lpdwDFlyMateIdx[dwIndex]]=PRST_DFLY_REST;

			// ok
			break;
		}
	}
}

// UpdateDragonflies()

void UpdateDragonflies(void)
{
	// alternating counter
	static DWORD dwAltCount=0;

	// dragonfly index
	DWORD dwIndex;

	// mate index
	DWORD dwMateIndex;

	// navigation update flag
	BOOL bNavFlag;

	// vectors for comparisons
	D3DXVECTOR3 vV1;
	D3DXVECTOR3 vV2;
	D3DXVECTOR3 vV3;

	// distance value
	float fDist;

	// tracking value
	float fTrackingVal;

	// coordinate values
	float fX;
	float fY;
	float fZ;

	// boundary flags
	BOOL bUpper;
	BOOL bLower;
	BOOL bCenter;

	// counter
	DWORD dwCount;

	// audiopath index
	DWORD dwPathIndex;

	// update alternating counter
	dwAltCount=(dwAltCount+1)&1;

	// update dragonflies
	for(dwIndex=0;dwIndex<g_dwNumDFly;dwIndex++)
	{
		// get mate index
		dwMateIndex=g_lpdwDFlyMateIdx[dwIndex];

		// set coordinate values
		fX=g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._41;
		fY=g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42;
		fZ=g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._43;

		// compare dragonfly position with other dragonflies
		for(dwCount=0;dwCount<g_dwNumDFly;dwCount++)
		{
			// skip this dragonfly
			if(dwCount!=dwIndex)
			{
				// skip this dragonfly's mate
				if(dwCount!=dwMateIndex)
				{
					// set up vector
					vV1.x=fX-g_lpMtx[g_lpdwDFlyMtx[dwCount]].mtx._41;
					vV1.z=fZ-g_lpMtx[g_lpdwDFlyMtx[dwCount]].mtx._43;

					// save distance
					fDist=SquareMagnitudeXZ(&vV1);

					// check distance
					if(fDist<Squared(DFLY_SEPARATIONDISTANCE))
					{
						// compute inverse distance
						float fInvDist=1.0F/sqrtf(fDist);
						
						// normalize vector
						vV2.x=vV1.x*fInvDist;
						vV2.y=0;
						vV2.z=vV1.z*fInvDist;

						// set new position
						g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._41+=vV2.x*g_lpfDFlyNavSpeed[dwCount];
						g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._43+=vV2.z*g_lpfDFlyNavSpeed[dwCount];

						// stop this dragonfly if darting
						if(g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_DART)
							g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_JUSTFLY;

						// stop other dragonfly if darting
						if(g_lpdwDFlyNavState[dwCount]=NVST_DFLY_DART)
							g_lpdwDFlyNavState[dwCount]=NVST_DFLY_JUSTFLY;
					}
				}
			}
		}

		// compare dragonfly position with swans
		for(dwCount=0;dwCount<g_dwNumSwan;dwCount++)
		{
			// set up vector
			vV1.x=fX-g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._41;
			vV1.z=fZ-g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._43;

			// check distance
			if(SquareMagnitudeXZ(&vV1)<Squared(DFLY_MINSWANDISTANCE))
			{
				// check height
				if(fY<DFLY_MINSWANHEIGHT)
				{
					// set state to climb
					g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_CLIMB;

					// set navigation values
					g_lpdwDFlyNavPos[dwIndex]=0;
					g_lpdwDFlyNavMax[dwIndex]=2;
				}
			}
		}

		// compare dragonfly position with baby swans
		for(dwCount=0;dwCount<g_dwNumBBSwan;dwCount++)
		{
			// set up vector
			vV1.x=fX-g_lpMtx[g_lpdwBBSwanMtx[dwCount]].mtx._41;
			vV1.z=fZ-g_lpMtx[g_lpdwBBSwanMtx[dwCount]].mtx._43;

			// check distance
			if(SquareMagnitudeXZ(&vV1)<Squared(DFLY_MINBBSWANDISTANCE))
			{
				// check height
				if(fY<DFLY_MINBBSWANHEIGHT)
				{
					// set state to climb
					g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_CLIMB;

					// set navigation values
					g_lpdwDFlyNavPos[dwIndex]=0;
					g_lpdwDFlyNavMax[dwIndex]=2;
				}
			}
		}

		// compare dragonfly position with turtles
		for(dwCount=0;dwCount<g_dwNumTurtle;dwCount++)
		{
			// set up vector
			vV1.x=fX-g_lpMtx[g_lpdwTurtleMtx[dwCount]].mtx._41;
			vV1.z=fZ-g_lpMtx[g_lpdwTurtleMtx[dwCount]].mtx._43;

			// check distance
			if(SquareMagnitudeXZ(&vV1)<Squared(DFLY_MINTURTLEDISTANCE))
			{
				// check height
				if(fY<DFLY_MINTURTLEHEIGHT)
				{
					// set state to climb
					g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_CLIMB;

					// set navigation values
					g_lpdwDFlyNavPos[dwIndex]=0;
					g_lpdwDFlyNavMax[dwIndex]=2;
				}
			}
		}

		// check for audio segment
		if(g_bUseSFX)
		{
			if(g_lpdwDFlyBuzzIdx[dwIndex]!=NO_OBJECT)
			{
				// set up vector from dragonfly to camera
				vV1.x=fX-g_lpMtx[g_dwCamMtx].mtx._41;
				vV1.z=fZ-g_lpMtx[g_dwCamMtx].mtx._43;

				// check distance
				if(SquareMagnitudeXZ(&vV1)<Squared(MAXDIST_BUZZ))
				{
					// check sound effect
					if(IsAudioSegmentPlaying(g_lpdwDFlyBuzzIdx[dwIndex]))
					{
						// set playback flag
						g_lpbDFlyPlayback[dwIndex]=TRUE;

						// get audiopath index
						if(GetManagedAudioPathIndex(g_lpdwDFlyAPathID[dwIndex],
													&dwPathIndex))
						{
							// check 3D sound flag
							if(g_bUse3DSound)
							{
								// update position
								SetAudioPathPosition(dwPathIndex,
													 g_lpdwDFlyMtx[dwIndex]);
							}
							else
							{
								// set simulated 3D settings
								SimulateAudioPath3D(dwPathIndex,
													g_dwCamMtx,
													&g_vCamAhead,
													g_lpdwDFlyMtx[dwIndex],
													MINDIST_BUZZ,
													MAXDIST_BUZZ,
													MAXVOL_BUZZ);
							}
						}
						else
						{
							// audiopath lost
							StopAudioSegment(g_lpdwDFlyBuzzIdx[dwIndex]);

							// reset audiopath id
							g_lpdwDFlyAPathID[dwIndex]=NO_OBJECT;
						}
					}
					else
					{
						// check playback flag
						if(g_lpbDFlyPlayback[dwIndex]!=FALSE)
						{
							// check audiopath id
							if(g_lpdwDFlyAPathID[dwIndex]!=NO_OBJECT)
							{
								// release audiopath
								ReleaseManagedAudioPath(g_lpdwDFlyAPathID[dwIndex]);

								// reset audiopath id
								g_lpdwDFlyAPathID[dwIndex]=NO_OBJECT;
							}

							// check initial updates flag
							if(!g_bInitUpdates)
							{
								// capture audiopath
								if(CaptureManagedAudioPath(&g_lpdwDFlyAPathID[dwIndex],
														   SFXPRI_BUZZ))
								{
									// get audiopath index
									if(GetManagedAudioPathIndex(g_lpdwDFlyAPathID[dwIndex],
																&dwPathIndex))
									{
										// check 3D sound flag
										if(g_bUse3DSound)
										{
											// set audiopath minimum distance
											SetAudioPathMinDistance(dwPathIndex,
																	MINDIST_BUZZ);

											// set audiopath maximum distance
											SetAudioPathMaxDistance(dwPathIndex,
																	MAXDIST_BUZZ);

											// set audiopath max volume
											SetAudioPathVolume(dwPathIndex,
															   MAXVOL_BUZZ);

											// set audiopath position
											SetAudioPathPosition(dwPathIndex,
																 g_lpdwDFlyMtx[dwIndex]);

											// render audio changes
											RenderAudioChanges();
										}
										else
										{
											// set simulated 3D settings
											SimulateAudioPath3D(dwPathIndex,
																g_dwCamMtx,
																&g_vCamAhead,
																g_lpdwDFlyMtx[dwIndex],
																MINDIST_BUZZ,
																MAXDIST_BUZZ,
																MAXVOL_BUZZ);
										}

										// reset playback flag
										g_lpbDFlyPlayback[dwIndex]=FALSE;

										// play segment
										PlayAudioSegment(g_lpdwDFlyBuzzIdx[dwIndex],
														 dwPathIndex);
									}
								}
							}
						}
					}
				}
				else
				{
					// check sound effect
					if(IsAudioSegmentPlaying(g_lpdwDFlyBuzzIdx[dwIndex]))
					{
						// stop segment
						StopAudioSegment(g_lpdwDFlyBuzzIdx[dwIndex]);

						// release audiopath
						ReleaseManagedAudioPath(g_lpdwDFlyAPathID[dwIndex]);

						// reset audiopath id
						g_lpdwDFlyAPathID[dwIndex]=NO_OBJECT;
					}
				}
			}
		}

		// check primary state
		switch(g_lpdwDFlyPriState[dwIndex])
		{
		// lead
		case(PRST_DFLY_LEAD):
			// get boundary flags
			bUpper=IsPointOutOfUpperBounds(fX,fZ);
			bLower=IsPointOutOfLowerBounds(fX,fZ);
			bCenter=IsPointOutOfCenterBounds(fX,fZ);

			// check boundaries
			if(bUpper||bLower||bCenter)
			{
				// get vector from center of boundary to dragonfly
				if(bUpper)
				{
					// center of upper boundary
					vV2=D3DXVECTOR3(fX-TERRAIN_CENTERX,
									0,
									fZ-(TERRAIN_CENTERZ-TERRAIN_ZRANGE));
				}
				else if(bLower)
				{
					// center of lower boundary
					vV2=D3DXVECTOR3(fX-TERRAIN_CENTERX,
									0,
									fZ-(TERRAIN_CENTERZ+TERRAIN_ZRANGE));
				}
				else
				{
					// center of center boundary
					vV2=D3DXVECTOR3(fX-TERRAIN_CENTERX,
									0,
									fZ-TERRAIN_CENTERZ);
				}

				// normalize vector
				Normalize(&vV1,&vV2);

				// get perpendicular vector
				vV2.x=vV1.z;
				vV2.y=0;
				vV2.z=-vV1.x;

				// get dragonfly ahead vector
				vV3=D3DXVECTOR3(g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._31,
								0,
								g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._33);

				// check cosine of angle
				if(DotProduct(&vV2,&vV3)>0)
				{
					// set state to hard right
					g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_HARDRIGHT;
				}
				else
				{
					// set state to hard left
					g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_HARDLEFT;
				}

				// check cosine of angle
				if(DotProduct(&vV1,&vV3)<0)
				{
					// check speed
					if(g_lpfDFlyNavSpeed[dwIndex]>0)
					{
						// check alternating counter
						if(dwAltCount)
						{
							// set state to slow down
							g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SLOWDOWN;
						}
					}
				}
				else
				{
					// check speed
					if(g_lpfDFlyNavSpeed[dwIndex]<(DFLY_MAXSPEED*0.25F))
					{
						// check alternating counter
						if(dwAltCount)
						{
							// set state to speed up
							g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SPEEDUP;
						}
					}
				}

				// set navigation values
				g_lpdwDFlyNavPos[dwIndex]=0;
				g_lpdwDFlyNavMax[dwIndex]=2;
			}
			else
			{
				// check speed
				if(g_lpfDFlyNavSpeed[dwIndex]<(DFLY_MAXSPEED*0.25F))
				{
					// set state to speed up
					g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SPEEDUP;

					// set navigation values
					g_lpdwDFlyNavPos[dwIndex]=0;
					g_lpdwDFlyNavMax[dwIndex]=2;
				}
				else
				{
					// check navigation state
					if(g_lpdwDFlyNavState[dwIndex]==NVST_DFLY_JUSTFLY)
					{
						// change navigation state if necessary
						if((abs(GetRandomInt())%DFLY_NAVIGATIONCHANGERATIO)==1)
						{
							// set new state
							g_lpdwDFlyNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%DFLY_NUMNAVIGATIONSTATES);

							// set navigation values
							g_lpdwDFlyNavPos[dwIndex]=0;
							g_lpdwDFlyNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%DFLY_NAVIGATIONMAXINC)+1);
						}
					}

					// check for mate
					if(dwMateIndex!=NO_OBJECT)
					{
						// get dragonfly position
						vV1=D3DXVECTOR3(g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._41,
										0,
										g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._43);

						// get mate position
						vV2=D3DXVECTOR3(g_lpMtx[g_lpdwDFlyMtx[dwMateIndex]].mtx._41,
										0,
										g_lpMtx[g_lpdwDFlyMtx[dwMateIndex]].mtx._43);

						// get distance to mate
						fDist=SquareMagnitudeXZ(vV1.x-vV2.x,
											    vV1.z-vV2.z);

						// check distance
						if(fDist>Squared(DFLY_MAXFOLLOWDISTANCE))
						{
							// check speed
							if(g_lpfDFlyNavSpeed[dwIndex]>(DFLY_MAXSPEED*0.5F))
							{
								// set state to slow down
								g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SLOWDOWN;

								// set navigation values
								g_lpdwDFlyNavPos[dwIndex]=0;
								g_lpdwDFlyNavMax[dwIndex]=2;
							}
						}
					}
				}

				// change primary state if necessary
				if((abs(GetRandomInt())%DFLY_PRIMARYCHANGERATIO)==1)
					RandomizeDragonflyPrimaryState(dwIndex);
			}

			// ok
			break;

		// follow
		case(PRST_DFLY_FOLLOW):
			// single dragonflies must be leaders
			if(dwMateIndex==NO_OBJECT)
				g_lpdwDFlyPriState[dwIndex]=PRST_DFLY_LEAD;
			else
			{
				// get dragonfly position
				vV1=D3DXVECTOR3(g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._41,
								0,
								g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._43);

				// get mate position
				vV2=D3DXVECTOR3(g_lpMtx[g_lpdwDFlyMtx[dwMateIndex]].mtx._41,
								0,
								g_lpMtx[g_lpdwDFlyMtx[dwMateIndex]].mtx._43);

				// get distance to mate
				fDist=SquareMagnitudeXZ(vV1.x-vV2.x,
									    vV1.z-vV2.z);

				// check distance to mate
				if(fDist<Squared(DFLY_MAXFOLLOWDISTANCE))
				{
					// check distance to mate
					if(fDist<Squared(DFLY_MINDFLYDISTANCE))
					{
						// get vector from mate to dragonfly
						vV3=vV1-vV2;

						// normalize vector
						Normalize(&vV1,&vV3);

						// set new position
						g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._41+=vV1.x*g_lpfDFlyNavSpeed[dwMateIndex];
						g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._43+=vV1.z*g_lpfDFlyNavSpeed[dwMateIndex];
					}
					else
					{
						// check speed
						if(g_lpfDFlyNavSpeed[dwIndex]>(g_lpfDFlyNavSpeed[dwMateIndex]*DFLY_TRACKINGSPEEDVAL)+DFLY_TRACKINGSPEEDINC)
						{
							// set state to slow down
							g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SLOWDOWN;

							// set navigation values
							g_lpdwDFlyNavPos[dwIndex]=0;
							g_lpdwDFlyNavMax[dwIndex]=2;
						}
						else if(g_lpfDFlyNavSpeed[dwIndex]<(g_lpfDFlyNavSpeed[dwMateIndex]*DFLY_TRACKINGSPEEDVAL1)-DFLY_TRACKINGSPEEDINC1)
						{
							// set state to speed up
							g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SPEEDUP;

							// set navigation values
							g_lpdwDFlyNavPos[dwIndex]=0;
							g_lpdwDFlyNavMax[dwIndex]=2;
						}
						else
						{
							// check navigation state
							if(g_lpdwDFlyNavState[dwIndex]==NVST_DFLY_JUSTFLY)
							{
								// change navigation state if necessary
								if((abs(GetRandomInt())%DFLY_NAVIGATIONCHANGERATIO)==1)
								{
									// set new state
									g_lpdwDFlyNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%DFLY_NUMNAVIGATIONSTATES);

									// set navigation values
									g_lpdwDFlyNavPos[dwIndex]=0;
									g_lpdwDFlyNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%DFLY_NAVIGATIONMAXINC)+1);
								}
							}
						}
  					}
				}
				else
				{
					// get vector from dragonfly to mate
					vV3=vV2-vV1;

					// normalize vector
					Normalize(&vV2,&vV3);
					
					// get dragonfly ahead vector
					vV1=D3DXVECTOR3(g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._31,
									0,
									g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._33);

					// check cosine of angle
					if(DotProduct(&vV1,&vV2)>0)
					{
						// get vector perpendicular to ahead vector
						vV3.x=vV1.z;
						vV3.y=0;
						vV3.z=-vV1.x;
 
						// check cosine of angle
						if(DotProduct(&vV3,&vV2)>0)
						{
							// set state to hard right
							g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_HARDRIGHT;
						}
						else
						{
							// set state to hard left
							g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_HARDLEFT;
						}

						// check speed
						if(g_lpfDFlyNavSpeed[dwIndex]>0)
						{
							// check alternating counter
							if(dwAltCount)
							{
								// set state to slow down
								g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SLOWDOWN;
							}
						}

						// set navigation values
						g_lpdwDFlyNavPos[dwIndex]=0;
						g_lpdwDFlyNavMax[dwIndex]=2;
					}
					else
					{
						// get vector perpendicular to ahead vector
						vV3.x=vV1.z;
						vV3.y=0;
						vV3.z=-vV1.x;

						// save tracking value
						fTrackingVal=DotProduct(&vV3,&vV2);

						// check tracking value
						if(fabsf(fTrackingVal)>DFLY_FOLLOWTRACKINGVAL)
						{
							// check tracking value
							if(fTrackingVal>0)
							{
								// set state to hard right
								g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_HARDRIGHT;
							}
							else
							{
								// set state to hard left
								g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_HARDLEFT;
							}

							// check speed
							if(g_lpfDFlyNavSpeed[dwIndex]<((g_lpfDFlyNavSpeed[dwMateIndex]*DFLY_TRACKINGSPEEDVAL)+DFLY_TRACKINGSPEEDINC)-DFLY_SPEEDTOLERANCE)
							{
								// check alternating counter
								if(dwAltCount)
								{
									// set state to speed up
									g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SPEEDUP;
								}
							}
							else if(g_lpfDFlyNavSpeed[dwIndex]>((g_lpfDFlyNavSpeed[dwMateIndex]*DFLY_TRACKINGSPEEDVAL)+DFLY_TRACKINGSPEEDINC)+DFLY_SPEEDTOLERANCE)
							{
								// check alternating counter
								if(dwAltCount)
								{
									// set state to slow down
									g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SLOWDOWN;
								}
							}

							// set navigation values
							g_lpdwDFlyNavPos[dwIndex]=0;
							g_lpdwDFlyNavMax[dwIndex]=2;
						}
						else
						{
							// check distance to mate
							if(fDist>Squared(DFLY_MAXFOLLOWDISTANCE))
							{
								// check speed
								if(g_lpfDFlyNavSpeed[dwIndex]<((g_lpfDFlyNavSpeed[dwMateIndex]*DFLY_TRACKINGSPEEDVAL2)+DFLY_TRACKINGSPEEDINC2)-DFLY_SPEEDTOLERANCE)
								{
									// set state to speed up
									g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SPEEDUP;

									// set navigation values
									g_lpdwDFlyNavPos[dwIndex]=0;
									g_lpdwDFlyNavMax[dwIndex]=2;
								}
								else if(g_lpfDFlyNavSpeed[dwIndex]>((g_lpfDFlyNavSpeed[dwMateIndex]*DFLY_TRACKINGSPEEDVAL2)+DFLY_TRACKINGSPEEDINC2)+DFLY_SPEEDTOLERANCE)
								{
									// set state to slow down
									g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SLOWDOWN;

									// set navigation values
									g_lpdwDFlyNavPos[dwIndex]=0;
									g_lpdwDFlyNavMax[dwIndex]=2;
								}
							}
						}
					}
				}
			}

			// ok
			break;

		// rest
		case(PRST_DFLY_REST):
			// check for mate
			if(dwMateIndex!=NO_OBJECT)
			{
				// get dragonfly position
				vV1=D3DXVECTOR3(g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._41,
								0,
								g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._43);

				// get mate position
				vV2=D3DXVECTOR3(g_lpMtx[g_lpdwDFlyMtx[dwMateIndex]].mtx._41,
								0,
								g_lpMtx[g_lpdwDFlyMtx[dwMateIndex]].mtx._43);

				// get distance to mate
				fDist=SquareMagnitudeXZ(vV1.x-vV2.x,
									    vV1.z-vV2.z);

				// check distance to mate
				if(fDist<Squared(DFLY_MINDFLYDISTANCE))
				{
					// get vector from mate to dragonfly
					vV3=vV1-vV2;

					// normalize vector
					Normalize(&vV1,&vV3);

					// set new position
					g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._41+=vV1.x*g_lpfDFlyNavSpeed[dwMateIndex];
					g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._43+=vV1.z*g_lpfDFlyNavSpeed[dwMateIndex];
				}
				else if(fDist>Squared(DFLY_MAXRESTDISTANCE))
				{
					// set dragonfly to lead
					g_lpdwDFlyPriState[dwIndex]=PRST_DFLY_LEAD;

					// set mate to follow if necessary
					if(dwMateIndex!=NO_OBJECT)
						g_lpdwDFlyPriState[dwMateIndex]=PRST_DFLY_FOLLOW;
				}
			}

			// check boundaries
			if(IsPointOutOfBounds(fX,fZ))
			{
				// set dragonfly to lead
				g_lpdwDFlyPriState[dwIndex]=PRST_DFLY_LEAD;

				// set mate to follow if necessary
				if(dwMateIndex!=NO_OBJECT)
					g_lpdwDFlyPriState[dwMateIndex]=PRST_DFLY_FOLLOW;
			}
			else
			{
				// check speed
				if(g_lpfDFlyNavSpeed[dwIndex]>DFLY_MAXRESTSPEED)
				{
					// set state to slow down
					g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_SLOWDOWN;

					// set navigation values
					g_lpdwDFlyNavPos[dwIndex]=0;
					g_lpdwDFlyNavMax[dwIndex]=2;
				}
				else
				{
					// check navigation state
					if(g_lpdwDFlyNavState[dwIndex]==NVST_DFLY_JUSTFLY)
					{
						// change navigation state if necessary
						if((abs(GetRandomInt())%DFLY_NAVIGATIONCHANGERATIO)==1)
						{
							// set new state
							g_lpdwDFlyNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%DFLY_NUMNAVIGATIONSTATES);

							// set navigation values
							g_lpdwDFlyNavPos[dwIndex]=0;
							g_lpdwDFlyNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%DFLY_NAVIGATIONMAXINC)+1);
						}
					}
				}

				// change primary state if necessary
				if((abs(GetRandomInt())%DFLY_PRIMARYCHANGERATIO)==1)
					RandomizeDragonflyPrimaryState(dwIndex);
			}

			// ok
			break;
		}

		// check navigation state
		if(g_lpdwDFlyNavState[dwIndex]!=NVST_DFLY_JUSTFLY)
		{
			// check navigation position
			if(g_lpdwDFlyNavPos[dwIndex]<g_lpdwDFlyNavMax[dwIndex])
			{
				// set flag
				bNavFlag=TRUE;

				// increment navigation position
				g_lpdwDFlyNavPos[dwIndex]++;
			}
			else
			{
				// reset flag
				bNavFlag=FALSE;
			}
		}

		// check navigation state
		switch(g_lpdwDFlyNavState[dwIndex])
		{
		// speed up
		case(NVST_DFLY_SPEEDUP):
			// increase speed
			g_lpfDFlyNavSpeed[dwIndex]+=DFLY_SPEEDINC;

			// check for overflow
			if(g_lpfDFlyNavSpeed[dwIndex]>DFLY_MAXSPEED)
				g_lpfDFlyNavSpeed[dwIndex]=DFLY_MAXSPEED;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_JUSTFLY;
			}

			// ok
			break;

		// slow down
		case(NVST_DFLY_SLOWDOWN):
			// decrease speed
			g_lpfDFlyNavSpeed[dwIndex]-=DFLY_SPEEDINC;

			// check for overflow
			if(g_lpfDFlyNavSpeed[dwIndex]<0)
				g_lpfDFlyNavSpeed[dwIndex]=0;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_JUSTFLY;
			}

			// ok
			break;

		// turn left
		case(NVST_DFLY_TURNLEFT):
			// turn left
			MatrixYaw(g_lpdwDFlyMtx[dwIndex],
					  DFLY_TURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_JUSTFLY;
			}

			// ok
			break;

		// turn right
		case(NVST_DFLY_TURNRIGHT):
			// turn right
			MatrixYaw(g_lpdwDFlyMtx[dwIndex],
					  -DFLY_TURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_JUSTFLY;
			}

			// ok
			break;

		// hard left
		case(NVST_DFLY_HARDLEFT):
			// turn left
			MatrixYaw(g_lpdwDFlyMtx[dwIndex],
					  DFLY_HARDTURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_JUSTFLY;
			}

			// ok
			break;

		// hard right
		case(NVST_DFLY_HARDRIGHT):
			// turn right
			MatrixYaw(g_lpdwDFlyMtx[dwIndex],
					  -DFLY_HARDTURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_JUSTFLY;
			}

			// ok
			break;

		// climb
		case(NVST_DFLY_CLIMB):
			// increase height
			g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42+=(g_lpfDFlyNavSpeed[dwIndex]*DFLY_HEIGHTINCFACTOR);

			// check for overflow
			if(g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42>DFLY_MAXHEIGHT)
				g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42=DFLY_MAXHEIGHT;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_JUSTFLY;
			}

			// ok
			break;

		// dive
		case(NVST_DFLY_DIVE):
			// decrease height
			g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42-=(g_lpfDFlyNavSpeed[dwIndex]*DFLY_HEIGHTINCFACTOR);

			// check for overflow
			if(g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42<DFLY_MINHEIGHT)
				g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42=DFLY_MINHEIGHT;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_JUSTFLY;
			}

			// ok
			break;

		// dart
		case(NVST_DFLY_DART):
			// adjust position by dart vector
			g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._41+=(g_lpvDFlyDartVec[dwIndex].x*DFLY_DARTINC);
			g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42+=(g_lpvDFlyDartVec[dwIndex].y*DFLY_DARTINC);
			g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._43+=(g_lpvDFlyDartVec[dwIndex].z*DFLY_DARTINC);

			// check height for overflow
			if(g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42>DFLY_MAXHEIGHT)
				g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42=DFLY_MAXHEIGHT;
			else if(g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42<DFLY_MINHEIGHT)
				g_lpMtx[g_lpdwDFlyMtx[dwIndex]].mtx._42=DFLY_MINHEIGHT;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwDFlyNavState[dwIndex]=NVST_DFLY_JUSTFLY;

				// set new dart vector
				vV1=D3DXVECTOR3(GetRandomFloat(),
							    GetRandomFloat(),
							    GetRandomFloat());

				// normalize & store vector
				Normalize(&g_lpvDFlyDartVec[dwIndex],
						  &vV1);
			}

			// ok
			break;
		}

		// move dragonfly
		MatrixBack(g_lpdwDFlyMtx[dwIndex],
				   g_lpfDFlyNavSpeed[dwIndex]);
	}
}

#ifdef DEBUG_MODE
// CreateFishTextures()

BOOL CreateFishTextures(DWORD dwNumTex,
						LPCSTR lpFileNameIn,
						LPCSTR lpFileNameOut)
{
	LoadingOutput("CreateFishTextures");

	// counter
	DWORD dwCount;
	
	// create dds file for each texture
	for(dwCount=0;dwCount<dwNumTex;dwCount++)
	{
		// generate input filename
		char lpSrcFile[16];
		sprintf(lpSrcFile,
				lpFileNameIn,
				dwCount+1);

		// generate output filename
		char lpDestFile[16];
		sprintf(lpDestFile,
				lpFileNameOut,
				dwCount+1);

		// transfer image
		CreateDDSFromImage(lpDestFile,
						   lpSrcFile,
						   TRUE,
						   FISH_TRANSCOLOR);
	}

	// ok
	return(TRUE);
}
#endif

// AddFishTextures()

DWORD AddFishTextures(DWORD dwNumTex,
					  LPCSTR lpFileName)
{
	LoadingOutput("AddFishTextures");

	// counter
	DWORD dwCount;
	
	// texture index
	DWORD dwTexIndex=NO_OBJECT;
	
	// add fish textures
	for(dwCount=0;dwCount<dwNumTex;dwCount++)
	{
		// generate filename
		char lpDestFile[16];
		sprintf(lpDestFile,
				lpFileName,
				dwCount+1);

		// load image
		DWORD dwIndex=AddTexture(lpDestFile);

		// check for error
		if(dwIndex==NO_OBJECT)
			return(NO_OBJECT);

		// save first texture index
		if(dwCount==0)
			dwTexIndex=dwIndex;
	}

	// return index
	return(dwTexIndex);
}

// GetNextFish()

DWORD GetNextFish(void)
{
	LoadingOutput("GetNextFish");

	// check counter
	if(g_dwNumFish==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpvFishPos,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfFishScale,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwFishPriState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwFishAnimFrame,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwFishRipple1Idx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwFishRipple2Idx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwFishAPathID,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwFishSplashIdx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbFishPlayback,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxFish=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumFish>=g_dwMaxFish)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpvFishPos,
					   sizeof(D3DXVECTOR3)*g_dwMaxFish,
					   sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfFishScale,
					   sizeof(float)*g_dwMaxFish,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwFishPriState,
					   sizeof(DWORD)*g_dwMaxFish,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwFishAnimFrame,
					   sizeof(DWORD)*g_dwMaxFish,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwFishRipple1Idx,
					   sizeof(DWORD)*g_dwMaxFish,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwFishRipple2Idx,
					   sizeof(DWORD)*g_dwMaxFish,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwFishAPathID,
					   sizeof(DWORD)*g_dwMaxFish,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwFishSplashIdx,
					   sizeof(DWORD)*g_dwMaxFish,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbFishPlayback,
					   sizeof(BOOL)*g_dwMaxFish,
					   sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxFish+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumFish;

	// increment counter
	g_dwNumFish++;

	// return index
	return(dwIndex);
}

// AddFish()

DWORD AddFish(void)
{
	LoadingOutput("AddFish");

	// get return index
	DWORD dwIndex=GetNextFish();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// set scale factor
	g_lpfFishScale[dwIndex]=FISH_SCALE+(GetRandomFloat()*FISH_SCALEVAR);

	// set default values
	g_lpvFishPos[dwIndex]=D3DXVECTOR3(0,0,0);
	g_lpdwFishPriState[dwIndex]=PRST_FISH_LURK;
	g_lpdwFishAnimFrame[dwIndex]=0;
	g_lpdwFishAPathID[dwIndex]=NO_OBJECT;
	g_lpdwFishSplashIdx[dwIndex]=NO_OBJECT;
	g_lpbFishPlayback[dwIndex]=TRUE;

	// add ripple #1
	g_lpdwFishRipple1Idx[dwIndex]=AddRipple();

	// check ripple #1
	if(g_lpdwFishRipple1Idx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// add ripple #2
	g_lpdwFishRipple2Idx[dwIndex]=AddRipple();

	// check ripple #2
	if(g_lpdwFishRipple2Idx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// return index
	return(dwIndex);
}

// UpdateFish()

void UpdateFish(void)
{
	// fish index
	DWORD dwIndex;

	// coordinate values
	float fX;
	float fZ;

	// comparison vector
	D3DXVECTOR3 vV1;

	// counter
	DWORD dwCount;

	// camera distance
	float fCamDist;

	// allow state change flag
	BOOL bStateChangeOK;

	// audiopath index
	DWORD dwPathIndex;

	// update fish
	for(dwIndex=0;dwIndex<g_dwNumFish;dwIndex++)
	{
		// check primary state
		switch(g_lpdwFishPriState[dwIndex])
		{
		// lurk
		case(PRST_FISH_LURK):
			// change state if necessary
			if((abs(GetRandomInt())%FISH_PRIMARYCHANGERATIO)==1)
			{
				// create random position
				fX=TERRAIN_CENTERX+(GetRandomFloat()*TERRAIN_XRANGE);
				fZ=TERRAIN_CENTERZ+(GetRandomFloat()*(TERRAIN_ZRANGE+TERRAIN_XRANGE));

				// reset state chanage flag
				bStateChangeOK=TRUE;

				// compare fish position with camera
				{
					// set up vector
					vV1.x=fX-g_lpMtx[g_dwCamMtx].mtx._41;
					vV1.z=fZ-g_lpMtx[g_dwCamMtx].mtx._43;

					// save camera distance
					fCamDist=SquareMagnitudeXZ(&vV1);

					// check distance & set flag if necessary
					if(fCamDist<Squared(FISH_MINCAMERADISTANCE))
						bStateChangeOK=FALSE;
				}

				// compare fish position with swans
				if(bStateChangeOK)
					for(dwCount=0;dwCount<g_dwNumSwan;dwCount++)
					{
						// set up vector
						vV1.x=fX-g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._41;
						vV1.z=fZ-g_lpMtx[g_lpdwSwanMtx[dwCount]].mtx._43;

						// check distance & set flag if necessary
						if(SquareMagnitudeXZ(&vV1)<Squared(FISH_MINSWANDISTANCE))
							bStateChangeOK=FALSE;
					}

				// compare fish position with baby swans
				if(bStateChangeOK)
					for(dwCount=0;dwCount<g_dwNumBBSwan;dwCount++)
					{
						// set up vector
						vV1.x=fX-g_lpMtx[g_lpdwBBSwanMtx[dwCount]].mtx._41;
						vV1.z=fZ-g_lpMtx[g_lpdwBBSwanMtx[dwCount]].mtx._43;

						// check distance & set flag if necessary
						if(SquareMagnitudeXZ(&vV1)<Squared(FISH_MINBBSWANDISTANCE))
							bStateChangeOK=FALSE;
					}

				// compare fish position with turtles
				if(bStateChangeOK)
				for(dwCount=0;dwCount<g_dwNumTurtle;dwCount++)
					{
						// set up vector
						vV1.x=fX-g_lpMtx[g_lpdwTurtleMtx[dwCount]].mtx._41;
						vV1.z=fZ-g_lpMtx[g_lpdwTurtleMtx[dwCount]].mtx._43;

						// check distance & set flag if necessary
						if(SquareMagnitudeXZ(&vV1)<Squared(FISH_MINTURTLEDISTANCE))
							bStateChangeOK=FALSE;
					}

				// compare fish position with dragonflies
				if(bStateChangeOK)
				for(dwCount=0;dwCount<g_dwNumDFly;dwCount++)
					{
						// set up vector
						vV1.x=fX-g_lpMtx[g_lpdwDFlyMtx[dwCount]].mtx._41;
						vV1.z=fZ-g_lpMtx[g_lpdwDFlyMtx[dwCount]].mtx._43;

						// check distance & set flag if necessary
						if(SquareMagnitudeXZ(&vV1)<Squared(FISH_MINDFLYDISTANCE))
							bStateChangeOK=FALSE;
					}

				// compare fish position with other fish
				if(bStateChangeOK)
					for(dwCount=0;dwCount<g_dwNumFish;dwCount++)
					{
						// skip this fish
						if(dwCount!=dwIndex)
						{
							// skip lurking fish
							if(g_lpdwFishPriState[dwCount]!=PRST_FISH_LURK)
							{
								// set up vector
								vV1.x=fX-g_lpvFishPos[dwCount].x;
								vV1.z=fZ-g_lpvFishPos[dwCount].z;

								// check distance & set flag if necessary
								if(SquareMagnitudeXZ(&vV1)<Squared(FISH_MINFISHDISTANCE))
									bStateChangeOK=FALSE;
							}
						}
					}

				// check state of playback
				if(g_bUseSFX)
					if(bStateChangeOK)
						if(g_lpbFishPlayback[dwIndex]==FALSE||
						   IsAudioSegmentPlaying(g_lpdwFishSplashIdx[dwIndex]))
							bStateChangeOK=FALSE;

				// check state change flag
				if(bStateChangeOK)
				{
					// set new position
					g_lpvFishPos[dwIndex].x=fX;
					g_lpvFishPos[dwIndex].z=fZ;
					
					// set state to jump as needed
					if((abs(GetRandomInt())%2)==1)
						g_lpdwFishPriState[dwIndex]=PRST_FISH_JUMPL;
					else
						g_lpdwFishPriState[dwIndex]=PRST_FISH_JUMPR;

					// start ripple
					StartRipple(g_lpdwFishRipple1Idx[dwIndex],
								g_lpvFishPos[dwIndex].x,
								g_lpvFishPos[dwIndex].z,
								GetRandomRippleSpeed());
				}
			}

			// ok
			break;

		// jump
		case(PRST_FISH_JUMPL):
		case(PRST_FISH_JUMPR):
			// update animation frame
			g_lpdwFishAnimFrame[dwIndex]++;

			// check sound flag
			if(g_bUseSFX)
			{
				// check current frame
				if(g_lpdwFishAnimFrame[dwIndex]==(FISH_INITSPLASHTEX<<FISH_SLOWDOWN))
				{
					// check distance to camera
					if(fCamDist<Squared(MAXDIST_SPLASH))
					{
						// check initial updates flag
						if(!g_bInitUpdates)
						{
							// capture audiopath
							if(CaptureManagedAudioPath(&g_lpdwFishAPathID[dwIndex],
													   SFXPRI_SPLASH))
							{
								// get audiopath index
								GetManagedAudioPathIndex(g_lpdwFishAPathID[dwIndex],
														 &dwPathIndex);

								// check 3D sound flag
								if(g_bUse3DSound)
								{
									// set audiopath minimum distance
									SetAudioPathMinDistance(dwPathIndex,
															MINDIST_SPLASH);

									// set audiopath maximum distance
									SetAudioPathMaxDistance(dwPathIndex,
															MAXDIST_SPLASH);

									// set audiopath max volume
									SetAudioPathVolume(dwPathIndex,
													   MAXVOL_SPLASH);

									// set audiopath position
									SetAudioPathPosition(dwPathIndex,
														 g_lpvFishPos[dwIndex].x,
														 g_lpvFishPos[dwIndex].y,
														 g_lpvFishPos[dwIndex].z);

									// render audio changes
									RenderAudioChanges();
								}
								else
								{
									// set simulated 3D settings
									SimulateAudioPath3D(dwPathIndex,
														g_dwCamMtx,
														&g_vCamAhead,
														g_lpvFishPos[dwIndex].x,
														g_lpvFishPos[dwIndex].y,
														g_lpvFishPos[dwIndex].z,
														MINDIST_SPLASH,
														MAXDIST_SPLASH,
														MAXVOL_SPLASH);
								}

								// reset playback flag
								g_lpbFishPlayback[dwIndex]=FALSE;

								// play sound
								PlayAudioSegment(g_lpdwFishSplashIdx[dwIndex],
												 dwPathIndex);

							}
						}
					}
				}
			}

			// check for overflow
			if((g_lpdwFishAnimFrame[dwIndex]>>FISH_SLOWDOWN)>=FISH_NUMTEX)
			{
				// reset animation frame
				g_lpdwFishAnimFrame[dwIndex]=0;
				
				// set state to lurk
				g_lpdwFishPriState[dwIndex]=PRST_FISH_LURK;

				// start ripple
				StartRipple(g_lpdwFishRipple2Idx[dwIndex],
							g_lpvFishPos[dwIndex].x,
							g_lpvFishPos[dwIndex].z,
							GetRandomRippleSpeed());
			}

			// ok
			break;
		}

		// check sound flag
		if(g_bUseSFX)
		{
			// check playback flag
			if(!g_lpbFishPlayback[dwIndex])
			{
				// check audio segment
				if(IsAudioSegmentPlaying(g_lpdwFishSplashIdx[dwIndex]))
				{
					// set playback flag
					g_lpbFishPlayback[dwIndex]=TRUE;
				}
			}
			else
			{
				// check audio segment
				if(!IsAudioSegmentPlaying(g_lpdwFishSplashIdx[dwIndex]))
				{
					// release audiopath
					ReleaseManagedAudioPath(g_lpdwFishAPathID[dwIndex]);
				}
			}
		}
	}
}

// RenderFish()

void RenderFish(void)
{
	// fish index
	DWORD dwIndex;

	// render fish
	for(dwIndex=0;dwIndex<g_dwNumFish;dwIndex++)
	{
		// check primary state
		if(g_lpdwFishPriState[dwIndex]==PRST_FISH_JUMPL)
		{
			// render left-hand fish
			RenderVerticalBillboard(NULL,
									g_dwFishTex+(g_lpdwFishAnimFrame[dwIndex]>>FISH_SLOWDOWN),
									&g_lpvFishPos[dwIndex],
									g_lpfFishScale[dwIndex],
									g_lpfFishScale[dwIndex]);
		}
		else if(g_lpdwFishPriState[dwIndex]==PRST_FISH_JUMPR)
		{
			// render right-hand fish
			RenderReverseVerticalBillboard(NULL,
										   g_dwFishTex+(g_lpdwFishAnimFrame[dwIndex]>>FISH_SLOWDOWN),
										   &g_lpvFishPos[dwIndex],
										   g_lpfFishScale[dwIndex],
										   g_lpfFishScale[dwIndex]);
		}
	}
}

// PurgeFish()

void PurgeFish(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpvFishPos);
	FreeMem((LPVOID*)&g_lpfFishScale);
	FreeMem((LPVOID*)&g_lpdwFishPriState);
	FreeMem((LPVOID*)&g_lpdwFishAnimFrame);
	FreeMem((LPVOID*)&g_lpdwFishRipple1Idx);
	FreeMem((LPVOID*)&g_lpdwFishRipple2Idx);
	FreeMem((LPVOID*)&g_lpdwFishAPathID);
	FreeMem((LPVOID*)&g_lpdwFishSplashIdx);
	FreeMem((LPVOID*)&g_lpbFishPlayback);

	// reset counters
	g_dwNumFish=0;
	g_dwMaxFish=0;
}

// GetNextBird()

DWORD GetNextBird(void)
{
	LoadingOutput("GetNextBird");

	// check counter
	if(g_dwNumBird==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwBirdMtx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfBirdWingPos,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfBirdWingDir,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBirdNavState,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfBirdNavSpeed,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBirdNavPos,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBirdNavMax,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBirdKeyframe1,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBirdKeyframe2,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfBirdTweenVal,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxBird=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumBird>=g_dwMaxBird)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwBirdMtx,
					   sizeof(DWORD)*g_dwMaxBird,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfBirdWingPos,
					   sizeof(float)*g_dwMaxBird,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfBirdWingDir,
					   sizeof(float)*g_dwMaxBird,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBirdNavState,
					   sizeof(DWORD)*g_dwMaxBird,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfBirdNavSpeed,
					   sizeof(float)*g_dwMaxBird,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBirdNavPos,
					   sizeof(DWORD)*g_dwMaxBird,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBirdNavMax,
					   sizeof(DWORD)*g_dwMaxBird,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBirdKeyframe1,
					   sizeof(DWORD)*g_dwMaxBird,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBirdKeyframe2,
					   sizeof(DWORD)*g_dwMaxBird,
					   sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfBirdTweenVal,
					   sizeof(float)*g_dwMaxBird,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxBird+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumBird;

	// increment counter
	g_dwNumBird++;

	// return index
	return(dwIndex);
}

// AddBird()

DWORD AddBird(void)
{
	LoadingOutput("AddBird");

	// get return index
	DWORD dwIndex=GetNextBird();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// add bird matrix
	g_lpdwBirdMtx[dwIndex]=AddMatrix();

	// check matrix
	if(g_lpdwBirdMtx[dwIndex]==NO_OBJECT)
		return(NO_OBJECT);

	// set random position
	g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._41=GetRandomFloat()*WSURF_RAD;
	g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._43=GetRandomFloat()*WSURF_RAD;

	// set altitude as needed
	if(dwIndex==0)
		g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._42=BIRD_BASEALTITUDE;
	else
		g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._42=g_lpMtx[g_lpdwBirdMtx[dwIndex-1]].mtx._42+BIRD_ALTITUDEINC;

	// scale matrix by bird scale
	MatrixScale(g_lpdwBirdMtx[dwIndex],
				BIRD_SCALE+(GetRandomFloat()*BIRD_SCALEVAR));

	// set default values
	g_lpdwBirdNavState[dwIndex]=NVST_BIRD_JUSTFLY;
	g_lpfBirdWingPos[dwIndex]=0.0F;
	g_lpfBirdWingDir[dwIndex]=BIRD_WINGSPEED;
	g_lpfBirdNavSpeed[dwIndex]=BIRD_MINSPEED;
	g_lpdwBirdNavPos[dwIndex]=0;
	g_lpdwBirdNavMax[dwIndex]=0;
	g_lpdwBirdKeyframe1[dwIndex]=KEYF_BIRD_WD;
	g_lpdwBirdKeyframe2[dwIndex]=KEYF_BIRD_WU;
	g_lpfBirdTweenVal[dwIndex]=0.0F;

	// return index
	return(dwIndex);
}

// UpdateBirds()

void UpdateBirds(void)
{
	// bird index
	DWORD dwIndex;

	// navigation update flag
	BOOL bNavFlag;

	// coordinate values
	float fX;
	float fZ;

	// update birds
	for(dwIndex=0;dwIndex<g_dwNumBird;dwIndex++)
	{
		// set coordinate values
		fX=g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._41;
		fZ=g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._43;

		// check boundaries
		if(SquareMagnitudeXZ(fX,fZ)>Squared(WSURF_RAD*1.5F))
		{
			// set state to hard left
			g_lpdwBirdNavState[dwIndex]=NVST_BIRD_HARDLEFT;

			// set navigation values
			g_lpdwBirdNavPos[dwIndex]=0;
			g_lpdwBirdNavMax[dwIndex]=2;
		}

		// check navigation state
		if(g_lpdwBirdNavState[dwIndex]==NVST_BIRD_JUSTFLY)
		{
			// change navigation state if necessary
			if((abs(GetRandomInt())%BIRD_NAVIGATIONCHANGERATIO)==1)
			{
				// set new state
				g_lpdwBirdNavState[dwIndex]=(DWORD)(abs(GetRandomInt())%BIRD_NUMNAVIGATIONSTATES);

				// set navigation values
				g_lpdwBirdNavPos[dwIndex]=0;
				g_lpdwBirdNavMax[dwIndex]=(DWORD)((abs(GetRandomInt())%BIRD_NAVIGATIONMAXINC)+1);
			}
		}

		// check navigation state
		if(g_lpdwBirdNavState[dwIndex]!=NVST_BIRD_JUSTFLY)
		{
			// check navigation position
			if(g_lpdwBirdNavPos[dwIndex]<g_lpdwBirdNavMax[dwIndex])
			{
				// set flag
				bNavFlag=TRUE;

				// increment navigation position
				g_lpdwBirdNavPos[dwIndex]++;
			}
			else
			{
				// reset flag
				bNavFlag=FALSE;
			}
		}

		// check navigation state
		switch(g_lpdwBirdNavState[dwIndex])
		{
		// speed up
		case(NVST_BIRD_SPEEDUP):
			// increase speed
			g_lpfBirdNavSpeed[dwIndex]+=BIRD_SPEEDINC;

			// check for overflow
			if(g_lpfBirdNavSpeed[dwIndex]>BIRD_MAXSPEED)
				g_lpfBirdNavSpeed[dwIndex]=BIRD_MAXSPEED;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwBirdNavState[dwIndex]=NVST_BIRD_JUSTFLY;
			}

			// ok
			break;

		// slow down
		case(NVST_BIRD_SLOWDOWN):
			// decrease speed
			g_lpfBirdNavSpeed[dwIndex]-=BIRD_SPEEDINC;

			// check for overflow
			if(g_lpfBirdNavSpeed[dwIndex]<BIRD_MINSPEED)
				g_lpfBirdNavSpeed[dwIndex]=BIRD_MINSPEED;

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwBirdNavState[dwIndex]=NVST_BIRD_JUSTFLY;
			}

			// ok
			break;

		// turn left
		case(NVST_BIRD_TURNLEFT):
			// turn left
			MatrixYaw(g_lpdwBirdMtx[dwIndex],
					  BIRD_TURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwBirdNavState[dwIndex]=NVST_BIRD_JUSTFLY;
			}

			// ok
			break;

		// turn right
		case(NVST_BIRD_TURNRIGHT):
			// turn right
			MatrixYaw(g_lpdwBirdMtx[dwIndex],
					  -BIRD_TURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwBirdNavState[dwIndex]=NVST_BIRD_JUSTFLY;
			}

			// ok
			break;

		// hard left
		case(NVST_BIRD_HARDLEFT):
			// turn left
			MatrixYaw(g_lpdwBirdMtx[dwIndex],
					  BIRD_HARDTURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwBirdNavState[dwIndex]=NVST_BIRD_JUSTFLY;
			}

			// ok
			break;

		// hard right
		case(NVST_BIRD_HARDRIGHT):
			// turn right
			MatrixYaw(g_lpdwBirdMtx[dwIndex],
					  -BIRD_HARDTURNINC);

			// check navigation flag
			if(!bNavFlag)
			{
				// set state to fly
				g_lpdwBirdNavState[dwIndex]=NVST_BIRD_JUSTFLY;
			}

			// ok
			break;
		}

		// move bird
		MatrixAhead(g_lpdwBirdMtx[dwIndex],
				    g_lpfBirdNavSpeed[dwIndex]);

		// update wings
		g_lpfBirdWingPos[dwIndex]+=g_lpfBirdWingDir[dwIndex]*g_lpfBirdNavSpeed[dwIndex];

		// check for overflow
		if(g_lpfBirdWingPos[dwIndex]>=BIRD_MAXWINGPOS)
		{
			// clip position
			g_lpfBirdWingPos[dwIndex]=BIRD_MAXWINGPOS;

			// reverse direction
			g_lpfBirdWingDir[dwIndex]=-g_lpfBirdWingDir[dwIndex];
		}
		else if(g_lpfBirdWingPos[dwIndex]<=-1.0F)
		{
			// clip position
			g_lpfBirdWingPos[dwIndex]=-1.0F;

			// reverse direction
			g_lpfBirdWingDir[dwIndex]=-g_lpfBirdWingDir[dwIndex];
		}

		// set tween value
		if(g_lpfBirdWingPos[dwIndex]>1.0F)
			g_lpfBirdTweenVal[dwIndex]=1;
		else
			g_lpfBirdTweenVal[dwIndex]=GetNaturalizedTweenValue((g_lpfBirdWingPos[dwIndex]+1.0F)*0.5F);
	}
}

// RenderBirds()

void RenderBirds(void)
{
	// bird index
	DWORD dwIndex;

#ifndef REFLECTION_OPTIMIZATION
	// render birds
	for(dwIndex=0;dwIndex<g_dwNumBird;dwIndex++)
	{
		// set keyframe
		UpdateRenderFrames(g_dwBirdAnim,
						   g_lpdwBirdKeyframe1[dwIndex],
						   g_lpdwBirdKeyframe2[dwIndex],
						   g_lpfBirdTweenVal[dwIndex]);

		// render bird
		RenderAnimation(g_dwBirdAnim,
						g_lpdwBirdMtx[dwIndex]);
	}

	// enable frontface cull
	EnableFrontfaceCull();
	
	// render bird reflections
	for(dwIndex=0;dwIndex<g_dwNumBird;dwIndex++)
	{
		// invert y-axis
		g_lpMtx[g_lpdwBirdMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwBirdMtx[dwIndex]].fYScale;
		g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._42=-g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._42;

		// set keyframe
		UpdateRenderFrames(g_dwBirdAnim,
						   g_lpdwBirdKeyframe1[dwIndex],
						   g_lpdwBirdKeyframe2[dwIndex],
						   g_lpfBirdTweenVal[dwIndex]);
		
		// render bird
		RenderAnimation(g_dwBirdAnim,
						g_lpdwBirdMtx[dwIndex]);
		
		// reset y-axis
		g_lpMtx[g_lpdwBirdMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwBirdMtx[dwIndex]].fYScale;
		g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._42=-g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._42;
	}
	
	// enable backface cull
	EnableBackfaceCull();
#else
	// render birds
	for(dwIndex=0;dwIndex<g_dwNumBird;dwIndex++)
	{
		// set keyframe
		UpdateRenderFrames(g_dwBirdAnim,
						   g_lpdwBirdKeyframe1[dwIndex],
						   g_lpdwBirdKeyframe2[dwIndex],
						   g_lpfBirdTweenVal[dwIndex]);

		// render bird
		RenderAnimation(g_dwBirdAnim,
						g_lpdwBirdMtx[dwIndex]);
		
		// enable frontface cull
		EnableFrontfaceCull();
	
		// invert y-axis
		g_lpMtx[g_lpdwBirdMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwBirdMtx[dwIndex]].fYScale;
		g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._42=-g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._42;

		// render bird
		RenderAnimation(g_dwBirdAnim,
						g_lpdwBirdMtx[dwIndex]);
		
		// reset y-axis
		g_lpMtx[g_lpdwBirdMtx[dwIndex]].fYScale=-g_lpMtx[g_lpdwBirdMtx[dwIndex]].fYScale;
		g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._42=-g_lpMtx[g_lpdwBirdMtx[dwIndex]].mtx._42;
	
		// enable backface cull
		EnableBackfaceCull();
	}
#endif
}

// PurgeBirds()

void PurgeBirds(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpdwBirdMtx);
	FreeMem((LPVOID*)&g_lpfBirdWingPos);
	FreeMem((LPVOID*)&g_lpfBirdWingDir);
	FreeMem((LPVOID*)&g_lpdwBirdNavState);
	FreeMem((LPVOID*)&g_lpfBirdNavSpeed);
	FreeMem((LPVOID*)&g_lpdwBirdNavPos);
	FreeMem((LPVOID*)&g_lpdwBirdNavMax);
	FreeMem((LPVOID*)&g_lpdwBirdKeyframe1);
	FreeMem((LPVOID*)&g_lpdwBirdKeyframe2);
	FreeMem((LPVOID*)&g_lpfBirdTweenVal);

	// reset counters
	g_dwNumBird=0;
	g_dwMaxBird=0;
}

#ifdef DEBUG_MODE
// CreateLensFlareTextures()

BOOL CreateLensFlareTextures(void)
{
	LoadingOutput("CreateLensFlareTextures");

	// flare info
	LF_MAINFLAREINFO mfi;
	LF_FLARERINGINFO fri;

	// counter
	DWORD dwCount;

	// filename
	char lpFilename[16];
	
	// prepare main flare structure
	mfi.dwFlareType=LF_MAINFLARE;
	mfi.dwTexSize=128;
	mfi.fGlowDia=0.8F;
	mfi.cGlowClr1=D3DCOLOR_RGBA(255,255,255,255);
	mfi.cGlowClr2=D3DCOLOR_RGBA(255,192,0,0);
	mfi.fGlowFalloff=1.0F;
	mfi.fRingDia1=0.60F;
	mfi.fRingDia2=0.70F;
	mfi.cRingClr1=D3DCOLOR_RGBA(255,255,255,48);
	mfi.cRingClr2=D3DCOLOR_RGBA(255,192,0,0);
	mfi.fRingFalloff=1.0F;
	mfi.fStreakDia=1.4F;
	mfi.fStreakWidth=0.04F;
	mfi.cStreakClr1=D3DCOLOR_RGBA(255,255,255,96);
	mfi.cStreakClr2=D3DCOLOR_RGBA(255,192,0,0);
	mfi.fStreakFalloff=1.0F;
	mfi.fMaxRayDia=1.2F;
	mfi.fMaxRayWidth=0.03F;
	mfi.dwNumRays=8;
	mfi.cRayClr1=D3DCOLOR_RGBA(255,255,255,64);
	mfi.cRayClr2=D3DCOLOR_RGBA(255,192,0,0);
	mfi.fRayFalloff=1.0F;

	// add main flare
	CreateLensFlareDDS("flare0.dds",
					   (LPVOID)&mfi);

	// add flare rings
	for(dwCount=0;dwCount<NUM_FLARERINGS;dwCount++)
	{
		// create random color
		float fR=fabsf(GetRandomFloat());
		float fG=fabsf(GetRandomFloat());
		float fB=fabsf(GetRandomFloat());
		
		// prepare flare ring structure
		fri.dwFlareType=LF_FLARERING;
		fri.dwTexSize=64;
		fri.fRingDia2=0.9F;
		fri.fRingDia1=fri.fRingDia2*fabsf(GetRandomFloat());
		fri.cRingClr1=D3DCOLOR_COLORVALUE(fR,fG,fB,0.075F);
		fri.cRingClr2=D3DCOLOR_COLORVALUE(fR,fG,fB,0.0F);
		fri.fRingFalloff=1.0F;

		// create filename
		sprintf(lpFilename,
			    "flare%d.dds",
				dwCount+1);

		// add flare ring
		CreateLensFlareDDS(lpFilename,
						   (LPVOID)&fri);
	}
	
	// ok
	return(TRUE);
}

// CreateRippleTextures()

BOOL CreateRippleTextures(void)
{
	LoadingOutput("CreateRippleTextures");

	// flare info (to be used as ripple texture)
	LF_FLARERINGINFO fri;
	
	// prepare flare ring structure
	fri.dwFlareType=LF_FLARERING;
	fri.dwTexSize=128;
	fri.fRingDia1=0.65F;
	fri.fRingDia2=0.95F;
	fri.cRingClr2=D3DCOLOR_COLORVALUE(0.26F,0.26F,0.71F,0);
	fri.fRingFalloff=1.0F;

	// create texture #1
	fri.cRingClr1=D3DCOLOR_COLORVALUE(1,1,1,0.4F);
	CreateLensFlareDDS("ripple0.dds",
					   (LPVOID)&fri);
	
	// create texture #2
	fri.cRingClr1=D3DCOLOR_COLORVALUE(1,1,1,0.3F);
	CreateLensFlareDDS("ripple1.dds",
					   (LPVOID)&fri);
	
	// create texture #3
	fri.cRingClr1=D3DCOLOR_COLORVALUE(1,1,1,0.2F);
	CreateLensFlareDDS("ripple2.dds",
					   (LPVOID)&fri);
	
	// create texture #4
	fri.cRingClr1=D3DCOLOR_COLORVALUE(1,1,1,0.1F);
	CreateLensFlareDDS("ripple3.dds",
					   (LPVOID)&fri);
	
	// ok
	return(TRUE);
}
#endif

// AddRippleVertexBuffer()

DWORD AddRippleVertexBuffer(void)
{
	LoadingOutput("AddRippleVertexBuffer");

	// ripple vertex data
	LPRIPPLEVERTEX lpRippleVtx;
	
	// create ripple vertex buffer
	DWORD dwVtxBuf=AddVertexBuffer(D3DFVF_RIPPLEVERTEX,
								   sizeof(RIPPLEVERTEX),
								   4);

	// check vertex buffer
	if(dwVtxBuf==NO_OBJECT)
		return(NO_OBJECT);

	// lock ripple vertex buffer
	lpRippleVtx=(LPRIPPLEVERTEX)LockVertexBuffer(dwVtxBuf);
	
	// check for error
	if(!lpRippleVtx)
		return(NO_OBJECT);

	// fill in vertices
	SET_RIPPLEVERTEX(lpRippleVtx[0],
					 -0.5F,0,-0.5F,
					 0,0);
	SET_RIPPLEVERTEX(lpRippleVtx[1],
					 0.5F,0,-0.5F,
					 1,0);
	SET_RIPPLEVERTEX(lpRippleVtx[2],
					 -0.5F,0,0.5F,
					 0,1);
	SET_RIPPLEVERTEX(lpRippleVtx[3],
					 0.5F,0,0.5F,
					 1,1);

	// unlock vertex buffer
	UnlockVertexBuffer(dwVtxBuf);

	// return index
	return(dwVtxBuf);
}

// AddRippleIndexBuffer()

DWORD AddRippleIndexBuffer(void)
{
	LoadingOutput("AddRippleIndexBuffer");

	// ripple index data
	LPWORD lpwRippleIdx;
	
	// create ripple index buffer
	DWORD dwIdxBuf=AddIndexBuffer(D3DPT_TRIANGLESTRIP,
								  2);

	// check index buffer
	if(dwIdxBuf==NO_OBJECT)
		return(NO_OBJECT);

	// lock ripple index buffer
	lpwRippleIdx=(LPWORD)LockIndexBuffer(dwIdxBuf);
	
	// check for error
	if(!lpwRippleIdx)
		return(NO_OBJECT);

	// fill in indices
	lpwRippleIdx[0]=0;
	lpwRippleIdx[1]=2;
	lpwRippleIdx[2]=1;
	lpwRippleIdx[3]=3;

	// unlock index buffer
	UnlockIndexBuffer(dwIdxBuf);

	// return index
	return(dwIdxBuf);
}

// InitRipples

BOOL InitRipples(void)
{
	LoadingOutput("InitRipples");

	// add ripple vertex buffer
	g_dwRippleVtxBuf=AddRippleVertexBuffer();

	// check vertex buffer
	if(g_dwRippleVtxBuf==NO_OBJECT)
		return(FALSE);

	// add ripple index buffer
	g_dwRippleIdxBuf=AddRippleIndexBuffer();

	// check index buffer
	if(g_dwRippleIdxBuf==NO_OBJECT)
		return(FALSE);

	// add ripple material
	g_dwRippleMat=AddMaterial(0,0,0,0,
							  0,0,0,0,
							  0,0,0,0,
							  1,1,1,1,
							  0);

	// check material
	if(g_dwRippleMat==NO_OBJECT)
		return(FALSE);

	// add ripple texture #0
	g_lpdwRippleTex[0]=AddTexture("ripple0.dds");

	// check texture #0
	if(g_lpdwRippleTex[0]==NO_OBJECT)
		return(FALSE);
	
	// add ripple texture #1
	g_lpdwRippleTex[1]=AddTexture("ripple1.dds");

	// check texture #1
	if(g_lpdwRippleTex[1]==NO_OBJECT)
		return(FALSE);
	
	// add ripple texture #2
	g_lpdwRippleTex[2]=AddTexture("ripple2.dds");

	// check texture #2
	if(g_lpdwRippleTex[2]==NO_OBJECT)
		return(FALSE);
	
	// add ripple texture #3
	g_lpdwRippleTex[3]=AddTexture("ripple3.dds");

	// check texture #3
	if(g_lpdwRippleTex[3]==NO_OBJECT)
		return(FALSE);
	
	// add ripple matrix
	g_dwRippleMtx=AddMatrix();

	// check matrix
	if(g_dwRippleMtx==NO_OBJECT)
		return(FALSE);

	// ok
	return(TRUE);
}

// GetNextRipple()

DWORD GetNextRipple(void)
{
	LoadingOutput("GetNextRipple");

	// check counter
	if(g_dwNumRipple==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpfRippleX,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfRippleZ,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfRipplePos,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfRippleSpeed,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxRipple=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumRipple>=g_dwMaxRipple)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpfRippleX,
					   sizeof(float)*g_dwMaxRipple,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfRippleZ,
					   sizeof(float)*g_dwMaxRipple,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfRipplePos,
					   sizeof(float)*g_dwMaxRipple,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfRippleSpeed,
					   sizeof(float)*g_dwMaxRipple,
					   sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxRipple+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumRipple;

	// increment counter
	g_dwNumRipple++;

	// return index
	return(dwIndex);
}

// AddRipple()

DWORD AddRipple(void)
{
	LoadingOutput("AddRipple");

	// get return index
	DWORD dwIndex=GetNextRipple();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// init data
	g_lpfRippleX[dwIndex]=0;
	g_lpfRippleZ[dwIndex]=0;
	g_lpfRipplePos[dwIndex]=1;
	g_lpfRippleSpeed[dwIndex]=1;

	// return index
	return(dwIndex);
}

// StartRipple()

void StartRipple(DWORD dwIndex,
				 float fX,
				 float fZ,
				 float fSpeed)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumRipple)
	{
		DisplayFatalErrorMessage("Invalid ripple index.",
								 "StartRipple()",
								 FALSE);
		return;
	}
#endif

	// copy data
	g_lpfRippleX[dwIndex]=fX;
	g_lpfRippleZ[dwIndex]=fZ;
	g_lpfRipplePos[dwIndex]=0;
	g_lpfRippleSpeed[dwIndex]=fSpeed;
}

// IsRippleComplete()

BOOL IsRippleComplete(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumRipple)
	{
		DisplayFatalErrorMessage("Invalid ripple index.",
								 "IsRippleComplete()",
								 FALSE);
		return(TRUE);
	}
#endif

	// check ripple position
	if(g_lpfRipplePos[dwIndex]>=1)
		return(TRUE);
	else
		return(FALSE);
}

// UpdateRipples()

void UpdateRipples(void)
{
	// counter
	DWORD dwCount;

	// update ripples
	for(dwCount=0;dwCount<g_dwNumRipple;dwCount++)
	{
		// update position
		g_lpfRipplePos[dwCount]+=g_lpfRippleSpeed[dwCount];

		// check for overflow
		if(g_lpfRipplePos[dwCount]>1)
			g_lpfRipplePos[dwCount]=1;
	}
}

// RenderRipples()

void RenderRipples(void)
{
	// counter
	DWORD dwCount;

	// disable z-buffer writes
	DisableZBufferWrites();
	
	// set render parameters
	SetVertexBuffer(g_dwRippleVtxBuf);
	SetIndexBuffer(g_dwRippleIdxBuf);
	SetMaterial(g_dwRippleMat);
	
	// render ripples
	for(dwCount=0;dwCount<g_dwNumRipple;dwCount++)
	{
		// check ripple position
		if(g_lpfRipplePos[dwCount]<1.0F)
		{		
			// compute ripple scale
			float fRippleScale=Squared(g_lpfRipplePos[dwCount])*MAX_RIPPLEDIA;

			// update ripple matrix
			g_lpMtx[g_dwRippleMtx].mtx._41=g_lpfRippleX[dwCount];
			g_lpMtx[g_dwRippleMtx].mtx._43=g_lpfRippleZ[dwCount];
			g_lpMtx[g_dwRippleMtx].fXScale=fRippleScale;
			g_lpMtx[g_dwRippleMtx].fZScale=fRippleScale;

			// set world matrix
			SetWorldMatrix(g_dwRippleMtx);

			// set texture
			if(g_lpfRipplePos[dwCount]<=0.25F)
				SetTexture(g_lpdwRippleTex[0]);
			else if(g_lpfRipplePos[dwCount]<=0.5F)
				SetTexture(g_lpdwRippleTex[1]);
			else if(g_lpfRipplePos[dwCount]<=0.75F)
				SetTexture(g_lpdwRippleTex[2]);
			else
				SetTexture(g_lpdwRippleTex[3]);

			// render ripple
			RenderPrimitive();
		}
	}
	
	// enable z-buffer writes
	EnableZBufferWrites();
}

// PurgeRipples()

void PurgeRipples(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpfRippleX);
	FreeMem((LPVOID*)&g_lpfRippleZ);
	FreeMem((LPVOID*)&g_lpfRipplePos);
	FreeMem((LPVOID*)&g_lpfRippleSpeed);

	// reset counters
	g_dwNumRipple=0;
	g_dwMaxRipple=0;
}

// GetRandomRippleSpeed()

float GetRandomRippleSpeed(void)
{
	// compute speed
	float fVal=MIN_RIPPLESPEED+((MAX_RIPPLESPEED-MIN_RIPPLESPEED)*fabsf(GetRandomFloat()));

	// return value
	return(fVal);
}

// InitCameraObject()

BOOL InitCameraObject(void)
{
	LoadingOutput("InitCameraObject");

	// add camera matrix
	g_dwCamMtx=AddMatrix();

	// check matrix
	if(g_dwCamMtx==NO_OBJECT)
		return(FALSE);

	// add camera output matrix
	g_dwCamOutputMtx=AddMatrix();

	// check matrix
	if(g_dwCamOutputMtx==NO_OBJECT)
		return(FALSE);

	// move camera up
	MatrixUp(g_dwCamMtx,(CAMERA_MINHEIGHT+CAMERA_MAXHEIGHT)*0.5F);

	// init camera values
	g_dwCamPriState=PRST_CAMERA_FOLLOW;
	g_dwCamNavState=NVST_CAMERA_JUSTFLY;
	g_fCamNavSpeed=0.0F;
	g_dwCamNavPos=0;
	g_dwCamNavMax=0;
	g_vCamAhead=D3DXVECTOR3(0,0,1);
	g_fCamTurnInc=0;
	g_fCamClimbInc=0;
	g_dwCamTargetMtx=NO_OBJECT;

	// ok
	return(TRUE);
}

// UpdateCameraObject()

void UpdateCameraObject(void)
{
// uncomment to allow mouse manipulation
/*
	// constants for mouse manipulation
	#define MM_MOVEINC 1.0F
	#define MM_TURNINC 0.001F
	#define MM_HIGHINC 3.0F

	// turn based on mouse input
	MatrixYaw(g_dwCamMtx,
			  ((float)g_nMouseDX)*MM_TURNINC);

	// move front-back
	MatrixBack(g_dwCamMtx,
			   ((float)g_nMouseDY)*MM_MOVEINC);

	// move up-down
	if(g_bMouseBtnL)
		MatrixUp(g_dwCamMtx,
				 MM_HIGHINC);
	else if(g_bMouseBtnR)
		MatrixDown(g_dwCamMtx,
				   MM_HIGHINC);

	// set look vector to ahead vector
	g_vCamAhead.x=g_lpMtx[g_dwCamMtx].mtx._31;
	g_vCamAhead.y=g_lpMtx[g_dwCamMtx].mtx._32;
	g_vCamAhead.z=g_lpMtx[g_dwCamMtx].mtx._33;

	// reset mouse position
	ResetMousePosition();

	// output position data
	if(BeginText(g_dwFPSFont))
	{
		LockGlobalMsg()

		// output position
		sprintf(g_lpMsg,
				"P(%f,%f,%f)",
				g_lpMtx[g_dwCamMtx].mtx._41,
				g_lpMtx[g_dwCamMtx].mtx._42,
				g_lpMtx[g_dwCamMtx].mtx._43);
		OutputText(g_lpMsg,0,32);
		
		// output ahead
		sprintf(g_lpMsg,
				"A(%f,%f,%f)",
				g_vCamAhead.x,
				g_vCamAhead.y,
				g_vCamAhead.z);
		OutputText(g_lpMsg,0,48);

		UnlockGlobalMsg()
		
		// end text
		EndText();
	}
	
	// ok
	return;
*/	
	
	// alternating counter
	static DWORD dwAltCount=0;

	// navigation update flag
	BOOL bNavFlag;

	// vectors for comparisons
	D3DXVECTOR3 vV1;
	D3DXVECTOR3 vV2;
	D3DXVECTOR3 vV3;

	// matrix value
	DWORD dwMtx;

	// distance value
	float fDist;

	// coordinate values
	float fX;
	float fY;
	float fZ;

	// boundary flags
	BOOL bUpper;
	BOOL bLower;
	BOOL bCenter;

	// counter
	DWORD dwCount;

	// tracking value
	float fTrackingVal;

	// nearest creature info
	DWORD dwNCMtx;
	float fNCDist;

#ifdef DEBUG_MODE
	// user control flag
	static BOOL bUserControl=FALSE;

	// read user input
	ReadUserInput();

	// constants
	#define TURN_SLOW 0.001F
	#define MOVE_SLOW 0.1F
	
	// check user input
	if(fabsf(GetAxisPos(g_dwDefXAxis))>EPSILON)
	{
		// turn camera
		MatrixYaw(g_dwCamMtx,GetAxisPos(g_dwDefXAxis)*TURN_SLOW);

		// set flag
		bUserControl=TRUE;
	}
	if(IsButtonDown(g_dwTopViewBtn))
	{
		if(fabsf(GetAxisPos(g_dwDefYAxis))>EPSILON)
		{
			// move camera
			MatrixDown(g_dwCamMtx,GetAxisPos(g_dwDefYAxis)*MOVE_SLOW);

			// set flag
			bUserControl=TRUE;
		}
	}
	else
	{
		if(fabsf(GetAxisPos(g_dwDefYAxis))>EPSILON)
		{
			// move camera
			MatrixBack(g_dwCamMtx,GetAxisPos(g_dwDefYAxis)*MOVE_SLOW);

			// set flag
			bUserControl=TRUE;
		}
	}

	// check user control flag
	if(bUserControl)
	{
		// set look vector to ahead vector
		g_vCamAhead.x=g_lpMtx[g_dwCamMtx].mtx._31;
		g_vCamAhead.y=g_lpMtx[g_dwCamMtx].mtx._32;
		g_vCamAhead.z=g_lpMtx[g_dwCamMtx].mtx._33;
	}
	else
	{
#endif

	// update alternating counter
	dwAltCount=(dwAltCount+1)&1;

	// set coordinate values
	fX=g_lpMtx[g_dwCamMtx].mtx._41;
	fY=g_lpMtx[g_dwCamMtx].mtx._42;
	fZ=g_lpMtx[g_dwCamMtx].mtx._43;
	
	// check primary state
	switch(g_dwCamPriState)
	{
	// follow
	case(PRST_CAMERA_FOLLOW):
		// reset nearest creature distance
		fNCDist=Squared(SKYBOX_RAD);

		// compare camera position with swans
		for(dwCount=0;dwCount<g_dwNumSwan;dwCount++)
		{
			// save matrix
			dwMtx=g_lpdwSwanMtx[dwCount];
			
			// set up vector
			vV1.x=fX-g_lpMtx[dwMtx].mtx._41;
			vV1.z=fZ-g_lpMtx[dwMtx].mtx._43;

			// compute distance
			fDist=SquareMagnitudeXZ(&vV1);

			// check nearest creature distance
			if(fDist<(fNCDist-Squared(CAMERA_TARGETDISTTOL)))
			{
				// save distance
				fNCDist=fDist;

				// save matrix
				dwNCMtx=dwMtx;
			}

			// check distance
			if(fDist<Squared(CAMERA_MINSWANDISTANCE))
			{
				// decrease speed
				g_fCamNavSpeed-=CAMERA_SPEEDINC;

				// check for overflow
				if(g_fCamNavSpeed<CAMERA_MINSPEED)
					g_fCamNavSpeed=CAMERA_MINSPEED;

				// compute inverse distance
				float fInvDist=1.0F/sqrtf(fDist);

				// normalize vector
				vV2.x=vV1.x*fInvDist;
				vV2.y=0;
				vV2.z=vV1.z*fInvDist;

				// set new position
				g_lpMtx[g_dwCamMtx].mtx._41+=vV2.x*g_fCamNavSpeed;
				g_lpMtx[g_dwCamMtx].mtx._43+=vV2.z*g_fCamNavSpeed;
			}
		}

		// get boundary flags
		bUpper=IsPointOutOfUpperBounds(fX,fZ);
		bLower=IsPointOutOfLowerBounds(fX,fZ);
		bCenter=IsPointOutOfCenterBounds(fX,fZ);

		// check boundaries
		if(bUpper||bLower||bCenter)
		{
			// get vector from center of boundary to camera
			if(bUpper)
			{
				// center of upper boundary
				vV2=D3DXVECTOR3(fX-TERRAIN_CENTERX,
								0,
								fZ-(TERRAIN_CENTERZ-TERRAIN_ZRANGE));
			}
			else if(bLower)
			{
				// center of lower boundary
				vV2=D3DXVECTOR3(fX-TERRAIN_CENTERX,
								0,
								fZ-(TERRAIN_CENTERZ+TERRAIN_ZRANGE));
			}
			else
			{
				// center of center boundary
				vV2=D3DXVECTOR3(fX-TERRAIN_CENTERX,
								0,
								fZ-TERRAIN_CENTERZ);
			}

			// normalize vector
			Normalize(&vV1,&vV2);

			// get perpendicular vector
			vV2.x=vV1.z;
			vV2.y=0;
			vV2.z=-vV1.x;

			// get camera ahead vector
			vV3=D3DXVECTOR3(g_lpMtx[g_dwCamMtx].mtx._31,
							0,
							g_lpMtx[g_dwCamMtx].mtx._33);

			// check cosine of angle
			if(DotProduct(&vV2,&vV3)>0)
			{
				// set state to hard right
				g_dwCamNavState=NVST_CAMERA_HARDRIGHT;
			}
			else
			{
				// set state to hard left
				g_dwCamNavState=NVST_CAMERA_HARDLEFT;
			}

			// check speed
			if(g_fCamNavSpeed>CAMERA_MINSPEED*CAMERA_OOBSPEEDFACTOR)
			{
				// check alternating counter
				if(dwAltCount)
				{
					// set state to slow down
					g_dwCamNavState=NVST_CAMERA_SLOWDOWN;
				}
			}

			// set navigation values
			g_dwCamNavPos=0;
			g_dwCamNavMax=2;
		}
		else
		{
			// check for target
			if(g_dwCamTargetMtx==NO_OBJECT)
				g_dwCamTargetMtx=GetNewCameraObjectTarget();

			// get camera position
			vV1=D3DXVECTOR3(g_lpMtx[g_dwCamMtx].mtx._41,
							0,
							g_lpMtx[g_dwCamMtx].mtx._43);

			// get target position
			vV2=D3DXVECTOR3(g_lpMtx[g_dwCamTargetMtx].mtx._41,
							0,
							g_lpMtx[g_dwCamTargetMtx].mtx._43);

			// get distance to target
			fDist=SquareMagnitudeXZ(vV1.x-vV2.x,
								    vV1.z-vV2.z);

			// check distance to target
			if(fDist<Squared(CAMERA_OBSERVATIONDISTANCE))
			{
				// check speed
				if(g_fCamNavSpeed>CAMERA_MINSPEED*2.0F)
				{
					// set state to slow down
					g_dwCamNavState=NVST_CAMERA_SLOWDOWN;

					// set navigation values
					g_dwCamNavPos=0;
					g_dwCamNavMax=2;
				}
				else if(g_fCamNavSpeed<CAMERA_MINSPEED)
				{
					// set state to speed up
					g_dwCamNavState=NVST_CAMERA_SPEEDUP;

					// set navigation values
					g_dwCamNavPos=0;
					g_dwCamNavMax=2;
				}
				else
				{
					// check navigation state
					if(g_dwCamNavState==NVST_CAMERA_JUSTFLY)
					{
						// change navigation state if necessary
						if((abs(GetRandomInt())%CAMERA_NAVIGATIONCHANGERATIO)==1)
						{
							// set new state
							g_dwCamNavState=(DWORD)(abs(GetRandomInt())%CAMERA_NUMNAVIGATIONSTATES);

							// set navigation values
							g_dwCamNavPos=0;
							g_dwCamNavMax=(DWORD)((abs(GetRandomInt())%CAMERA_NAVIGATIONMAXINC)+1);
						}
					}
				}

				// change target if necessary
				if((abs(GetRandomInt())%CAMERA_PRIMARYCHANGERATIO)==1)
				{
					// reset target matrix
					g_dwCamTargetMtx=NO_OBJECT;
				}
			}
			else
			{
				// get vector from camera to target
				vV3=vV2-vV1;

				// normalize vector
				Normalize(&vV2,&vV3);
				
				// get camera ahead vector
				vV1=D3DXVECTOR3(g_lpMtx[g_dwCamMtx].mtx._31,
								0,
								g_lpMtx[g_dwCamMtx].mtx._33);

				// check cosine of angle
				if(DotProduct(&vV1,&vV2)>0)
				{
					// get vector perpendicular to ahead vector
					vV3.x=vV1.z;
					vV3.y=0;
					vV3.z=-vV1.x;

					// check cosine of angle
					if(DotProduct(&vV3,&vV2)>0)
					{
						// set state to hard right
						g_dwCamNavState=NVST_CAMERA_HARDRIGHT;
					}
					else
					{
						// set state to hard left
						g_dwCamNavState=NVST_CAMERA_HARDLEFT;
					}

					// check speed
					if(g_fCamNavSpeed>CAMERA_MINSPEED)
					{
						// check alternating counter
						if(dwAltCount)
						{
							// set state to slow down
							g_dwCamNavState=NVST_CAMERA_SLOWDOWN;
						}
					}

					// set navigation values
					g_dwCamNavPos=0;
					g_dwCamNavMax=2;
				}
				else
				{
					// get vector perpendicular to ahead vector
					vV3.x=vV1.z;
					vV3.y=0;
					vV3.z=-vV1.x;

					// save tracking value
					fTrackingVal=DotProduct(&vV3,&vV2);

					// check tracking value
					if(fabsf(fTrackingVal)>CAMERA_FOLLOWTRACKINGVAL)
					{
						// check tracking value
						if(fTrackingVal>0)
						{
							// set state to hard right
							g_dwCamNavState=NVST_CAMERA_HARDRIGHT;
						}
						else
						{
							// set state to hard left
							g_dwCamNavState=NVST_CAMERA_HARDLEFT;
						}

						// check speed
						if(g_fCamNavSpeed<CAMERA_MINSPEED)
						{
							// check alternating counter
							if(dwAltCount)
							{
								// set state to speed up
								g_dwCamNavState=NVST_CAMERA_SPEEDUP;
							}
						}
						else if(g_fCamNavSpeed>CAMERA_MINSPEED*2.0F)
						{
							// check alternating counter
							if(dwAltCount)
							{
								// set state to slow down
								g_dwCamNavState=NVST_CAMERA_SLOWDOWN;
							}
						}

						// set navigation values
						g_dwCamNavPos=0;
						g_dwCamNavMax=2;
					}
					else
					{
						// check speed
						if(g_fCamNavSpeed<CAMERA_MAXSPEED)
						{
							// set state to speed up
							g_dwCamNavState=NVST_CAMERA_SPEEDUP;

							// set navigation values
							g_dwCamNavPos=0;
							g_dwCamNavMax=2;
						}
					}
				}
			}
		}

		// ok
		break;
	}

	// check navigation state
	if(g_dwCamNavState!=NVST_CAMERA_JUSTFLY)
	{
		// check navigation position
		if(g_dwCamNavPos<g_dwCamNavMax)
		{
			// set flag
			bNavFlag=TRUE;

			// increment navigation position
			g_dwCamNavPos++;
		}
		else
		{
			// reset flag
			bNavFlag=FALSE;
		}
	}

	// check navigation state
	switch(g_dwCamNavState)
	{
	// speed up
	case(NVST_CAMERA_SPEEDUP):
		// increase speed
		g_fCamNavSpeed+=CAMERA_SPEEDINC;

		// check for overflow
		if(g_fCamNavSpeed>CAMERA_MAXSPEED)
			g_fCamNavSpeed=CAMERA_MAXSPEED;

		// check navigation flag
		if(!bNavFlag)
		{
			// set state to fly
			g_dwCamNavState=NVST_CAMERA_JUSTFLY;
		}

		// ok
		break;

	// slow down
	case(NVST_CAMERA_SLOWDOWN):
		// decrease speed
		g_fCamNavSpeed-=CAMERA_SPEEDINC;

		// check for overflow
		if(g_fCamNavSpeed<0)
			g_fCamNavSpeed=0;

		// check navigation flag
		if(!bNavFlag)
		{
			// set state to fly
			g_dwCamNavState=NVST_CAMERA_JUSTFLY;
		}

		// ok
		break;

	// turn left
	case(NVST_CAMERA_TURNLEFT):
		// increase turn increment
		g_fCamTurnInc+=CAMERA_TURNINC;

		// check for overflow
		if(g_fCamTurnInc>CAMERA_MAXTURNINC)
			g_fCamTurnInc=CAMERA_MAXTURNINC;

		// check navigation flag
		if(!bNavFlag)
		{
			// set state to fly
			g_dwCamNavState=NVST_CAMERA_JUSTFLY;
		}

		// ok
		break;

	// turn right
	case(NVST_CAMERA_TURNRIGHT):
		// decrease turn increment
		g_fCamTurnInc-=CAMERA_TURNINC;

		// check for overflow
		if(g_fCamTurnInc<-CAMERA_MAXTURNINC)
			g_fCamTurnInc=-CAMERA_MAXTURNINC;

		// check navigation flag
		if(!bNavFlag)
		{
			// set state to fly
			g_dwCamNavState=NVST_CAMERA_JUSTFLY;
		}

		// ok
		break;

	// hard left
	case(NVST_CAMERA_HARDLEFT):
		// increase turn increment
		g_fCamTurnInc+=CAMERA_HARDTURNINC;

		// check for overflow
		if(g_fCamTurnInc>CAMERA_MAXTURNINC)
			g_fCamTurnInc=CAMERA_MAXTURNINC;

		// check navigation flag
		if(!bNavFlag)
		{
			// set state to fly
			g_dwCamNavState=NVST_CAMERA_JUSTFLY;
		}

		// ok
		break;

	// hard right
	case(NVST_CAMERA_HARDRIGHT):
		// decrease turn increment
		g_fCamTurnInc-=CAMERA_HARDTURNINC;

		// check for overflow
		if(g_fCamTurnInc<-CAMERA_MAXTURNINC)
			g_fCamTurnInc=-CAMERA_MAXTURNINC;

		// check navigation flag
		if(!bNavFlag)
		{
			// set state to fly
			g_dwCamNavState=NVST_CAMERA_JUSTFLY;
		}

		// ok
		break;

	// climb
	case(NVST_CAMERA_CLIMB):
		// increase climb increment
		g_fCamClimbInc+=CAMERA_CLIMBINC;

		// check for overflow
		if(g_fCamClimbInc>CAMERA_MAXCLIMBINC)
			g_fCamClimbInc=CAMERA_MAXCLIMBINC;

		// check navigation flag
		if(!bNavFlag)
		{
			// set state to fly
			g_dwCamNavState=NVST_CAMERA_JUSTFLY;
		}

		// ok
		break;

	// dive
	case(NVST_CAMERA_DIVE):
		// decrease climb increment
		g_fCamClimbInc-=CAMERA_CLIMBINC;

		// check for overflow
		if(g_fCamClimbInc<-CAMERA_MAXCLIMBINC)
			g_fCamClimbInc=-CAMERA_MAXCLIMBINC;

		// check navigation flag
		if(!bNavFlag)
		{
			// set state to fly
			g_dwCamNavState=NVST_CAMERA_JUSTFLY;
		}

		// ok
		break;
	}

	// turn camera
	MatrixYaw(g_dwCamMtx,
			  g_fCamTurnInc);

	// move camera
	MatrixBack(g_dwCamMtx,
			   g_fCamNavSpeed);
	MatrixUp(g_dwCamMtx,
			 g_fCamClimbInc);

	// clip altitude
	if(g_lpMtx[g_dwCamMtx].mtx._42<CAMERA_MINHEIGHT)
		g_lpMtx[g_dwCamMtx].mtx._42=CAMERA_MINHEIGHT;
	else if(g_lpMtx[g_dwCamMtx].mtx._42>CAMERA_MAXHEIGHT)
		g_lpMtx[g_dwCamMtx].mtx._42=CAMERA_MAXHEIGHT;

	// straighten turn if not turning
	if(g_dwCamNavState!=NVST_CAMERA_TURNLEFT&&
	   g_dwCamNavState!=NVST_CAMERA_HARDLEFT&&
	   g_dwCamNavState!=NVST_CAMERA_TURNRIGHT&&
	   g_dwCamNavState!=NVST_CAMERA_HARDRIGHT)
	{
		// check turn value
		if(g_fCamTurnInc<-EPSILON)
			g_fCamTurnInc+=CAMERA_TURNINC;
		else if(g_fCamTurnInc>EPSILON)
			g_fCamTurnInc-=CAMERA_TURNINC;
	}

	// level path if not climbing or diving
	if(g_dwCamNavState!=NVST_CAMERA_CLIMB&&
	   g_dwCamNavState!=NVST_CAMERA_DIVE)
	{
		// check climb value
		if(g_fCamClimbInc<-CAMERA_CLIMBINC)
			g_fCamClimbInc+=CAMERA_CLIMBINC;
		else if(g_fCamClimbInc>CAMERA_CLIMBINC)
			g_fCamClimbInc-=CAMERA_CLIMBINC;
	}

	// get target vector
	if(dwNCMtx==NO_OBJECT)
	{
		// use camera ahead vector
		vV1.x=-g_lpMtx[g_dwCamMtx].mtx._31;
		vV1.y=-g_lpMtx[g_dwCamMtx].mtx._32;
		vV1.z=-g_lpMtx[g_dwCamMtx].mtx._33;

		// track ahead vector to target
		vV2.x=GetInterpolatedValue(g_vCamAhead.x,vV1.x,CAMERA_TRACKINGINC2);
		vV2.y=GetInterpolatedValue(g_vCamAhead.y,vV1.y,CAMERA_TRACKINGINC2);
		vV2.z=GetInterpolatedValue(g_vCamAhead.z,vV1.z,CAMERA_TRACKINGINC2);
	}
	else
	{
		// use vector to nearest creature
		vV2.x=g_lpMtx[dwNCMtx].mtx._41-g_lpMtx[g_dwCamMtx].mtx._41;
		vV2.y=CAMERA_TARGETYOFFSET    -g_lpMtx[g_dwCamMtx].mtx._42;
		vV2.z=g_lpMtx[dwNCMtx].mtx._43-g_lpMtx[g_dwCamMtx].mtx._43;

		// normalize vector
		Normalize(&vV1,&vV2);

		// track ahead vector to target
		vV2.x=GetInterpolatedValue(g_vCamAhead.x,vV1.x,CAMERA_TRACKINGINC);
		vV2.y=GetInterpolatedValue(g_vCamAhead.y,vV1.y,CAMERA_TRACKINGINC);
		vV2.z=GetInterpolatedValue(g_vCamAhead.z,vV1.z,CAMERA_TRACKINGINC);
	}

	// check ahead y-value
	if(vV2.y>CAMERA_MAXAHEADYVALUE)
		vV2.y=CAMERA_MAXAHEADYVALUE;
	else if(vV2.y<-CAMERA_MAXAHEADYVALUE)
		vV2.y=-CAMERA_MAXAHEADYVALUE;

	// normalize vector
	Normalize(&g_vCamAhead,&vV2);

#ifdef DEBUG_MODE
	}
#endif
}

// SetCameraObjectToViewMatrix()

void SetCameraObjectToViewMatrix(void)
{
	// coordinate values
	float fX;
	float fY;
	float fZ;
	
	// set pointer to output matrix
	LPD3DXMATRIX lpMtx=&g_lpMtx[g_dwCamOutputMtx].mtx;

	// set position
	lpMtx->_41=g_lpMtx[g_dwCamMtx].mtx._41;
	lpMtx->_42=g_lpMtx[g_dwCamMtx].mtx._42;
	lpMtx->_43=g_lpMtx[g_dwCamMtx].mtx._43;

	// set ahead vector
	lpMtx->_31=g_vCamAhead.x;
	lpMtx->_32=g_vCamAhead.y;
	lpMtx->_33=g_vCamAhead.z;
	
	// compute right vector
	CrossProduct(&fX,&fY,&fZ,
				 0,1,0,
				 lpMtx->_31,lpMtx->_32,lpMtx->_33);

	// set right vector
	lpMtx->_11=fX;
	lpMtx->_12=fY;
	lpMtx->_13=fZ;

	// set as camera matrix
	SetCameraMatrix(g_dwCamOutputMtx);
}

// GetNewCameraObjectTarget()

DWORD GetNewCameraObjectTarget(void)
{
	// return value
	DWORD dwRetVal;
	
	// select random swan
	DWORD dwSwanNum=(DWORD)(abs(GetRandomInt())%g_dwNumSwan);

	// set return matrix index
	dwRetVal=g_lpdwSwanMtx[dwSwanNum];

#ifdef DEBUG_MODE
	// verify index
	if(dwRetVal>g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index",
								 "GetNewCameraObjectTarget()",
								 FALSE);
	}
#endif

	// return index
	return(dwRetVal);
}

// InitSoundFX()

BOOL InitSoundFX(void)
{
	LoadingOutput("InitSoundFX");

	// prepare 3D sound as needed
	if(g_bUseSFX)
	{
		if(g_bUse3DSound)
		{
			// enable 3D audio
			Enable3DAudio();

			// init audio listener
			if(!InitAudioListener())
			{
				DisplayErrorMessage("Unable to initialize 3-D audio.",
									"Try disabling 3-D audio in the application settings.",
									FALSE);
				return(FALSE);
			}

			// set listener distance factor
			SetListenerDistanceFactor(DIST_INCH*6.0F);
		}
		else
		{
			// disable 3D audio
			Disable3DAudio();
		}
	}

	// initialize audiopath manager
	if(g_bUseSFX)
		if(!InitAudioPathManager(NUM_MANAGEDAUDIOPATHS,g_bUse3DSound))
			return(FALSE);

	// load music
	if(g_bUseMusic)
	{
		// add segment
		g_dwLeCygneMus=AddAudioSegment("lecygne2.mid");

		// check segment
		if(g_dwLeCygneMus==NO_OBJECT)
			return(FALSE);
	}

	// load sound effects
	if(g_bUseSFX)
	{
		// add & check segment
		g_dwEagleSFX=AddAudioSegment("eagle1.wav");
		if(g_dwEagleSFX==NO_OBJECT)
			return(FALSE);

		// add & check segment
		g_dwBirds1SFX=AddAudioSegment("birds1.wav");
		if(g_dwBirds1SFX==NO_OBJECT)
			return(FALSE);

		// add & check segment
		g_dwBirds2SFX=AddAudioSegment("birds2.wav");
		if(g_dwBirds2SFX==NO_OBJECT)
			return(FALSE);

		// add & check segment
		g_dwSwan1SFX=AddAudioSegment("swan1.wav");
		if(g_dwSwan1SFX==NO_OBJECT)
			return(FALSE);

		// add & check segment
		g_dwSwan2SFX=AddAudioSegment("swan2.wav");
		if(g_dwSwan2SFX==NO_OBJECT)
			return(FALSE);

		// add & check segment
		g_dwSwan3SFX=AddAudioSegment("swan3.wav");
		if(g_dwSwan3SFX==NO_OBJECT)
			return(FALSE);

		// add & check segment
		g_dwSplashSFX=AddAudioSegment("splash1.wav");
		if(g_dwSplashSFX==NO_OBJECT)
			return(FALSE);

		// add & check segment
		g_dwBBSwan1SFX=AddAudioSegment("bbswan1.wav");
		if(g_dwBBSwan1SFX==NO_OBJECT)
			return(FALSE);

		// add & check segment
		g_dwBBSwan2SFX=AddAudioSegment("bbswan2.wav");
		if(g_dwBBSwan2SFX==NO_OBJECT)
			return(FALSE);

		// add & check segment
		g_dwBuzzSFX=AddAudioSegment("buzz1.wav");
		if(g_dwBuzzSFX==NO_OBJECT)
			return(FALSE);
	}

	// set music repeats
	if(g_bUseMusic)
		SetAudioSegmentRepeats(g_dwLeCygneMus);

	// check sound flag
	if(g_bUseSFX)
	{
		// init dragonfly sound effects
		if(!InitDragonflySFX())
			return(FALSE);
		
		// init fish sound effects
		if(!InitFishSFX())
			return(FALSE);
		
		// init sound sources
		if(!InitSoundSources())
			return(FALSE);

		// init sound position data
		if(!InitSoundPositionData())
			return(FALSE);
	}

	// ok
	return(TRUE);
}

// UpdateSoundPosition()

void UpdateSoundPosition(LPSOUNDPOSDATA lpSPD)
{
	// audiopath index
	DWORD dwPathIndex;

	// vectors for comparison
	D3DXVECTOR3 vV1;
	D3DXVECTOR3 vV2;
	D3DXVECTOR3 vV3;

	// distance value
	float fDist;

	// check sound effect
	if(IsAudioSegmentPlaying(lpSPD->dwSFXIndex))
	{
		// set playback flag
		lpSPD->bPlayback=TRUE;

		// get audiopath index
		if(GetManagedAudioPathIndex(lpSPD->dwAudioPathID,
									&dwPathIndex))
		{
			// check 3D sound flag
			if(g_bUse3DSound)
			{
				// update position
				SetAudioPathPosition(dwPathIndex,
									 lpSPD->dwTargetMtxIndex);
			}
		}
		else
		{
			// audiopath lost
			StopAudioSegment(lpSPD->dwSFXIndex);

			// reset audiopath id
			lpSPD->dwAudioPathID=NO_OBJECT;
		}
	}
	else
	{
		// check playback flag
		if(lpSPD->bPlayback==FALSE)
		{
			// chill until playback starts
			return;
		}

		// check audiopath id
		if(lpSPD->dwAudioPathID!=NO_OBJECT)
		{
			// release audiopath
			ReleaseManagedAudioPath(lpSPD->dwAudioPathID);

			// reset audiopath id
			lpSPD->dwAudioPathID=NO_OBJECT;
		}

		// play sound if necessary
		if((abs(GetRandomInt()%lpSPD->dwRepeatVal))==1)
		{
			// check for targets
			if(lpSPD->dwNumTarget)
			{
				// capture audiopath
				if(CaptureManagedAudioPath(&lpSPD->dwAudioPathID,
										   lpSPD->dwSFXPriVal))
				{
					// get audiopath index
					if(GetManagedAudioPathIndex(lpSPD->dwAudioPathID,
												&dwPathIndex))
					{
						// select random target
						lpSPD->dwTargetMtxIndex=lpSPD->lpdwTargetMtxArray[abs(GetRandomInt()%lpSPD->dwNumTarget)];

						// get camera position
						vV1.x=g_lpMtx[g_dwCamMtx].mtx._41;
						vV1.z=g_lpMtx[g_dwCamMtx].mtx._43;

						// get target position
						vV2.x=g_lpMtx[lpSPD->dwTargetMtxIndex].mtx._41;
						vV2.z=g_lpMtx[lpSPD->dwTargetMtxIndex].mtx._43;
						
						// get vector from camera to target
						vV3.x=vV2.x-vV1.x;
						vV3.z=vV2.z-vV1.z;

						// get distance to target
						fDist=SquareMagnitudeXZ(&vV3);

						// check distance
						if(fDist>Squared(CAMERA_MINSOUNDDISTANCE))
						{
							// check 3D sound flag
							if(g_bUse3DSound)
							{
								// set audiopath minimum distance
								SetAudioPathMinDistance(dwPathIndex,
														lpSPD->fMinDist);

								// set audiopath maximum distance
								SetAudioPathMaxDistance(dwPathIndex,
														lpSPD->fMaxDist);

								// set audiopath max volume
								SetAudioPathVolume(dwPathIndex,
												   lpSPD->fMaxVolVal);

								// set audiopath position
								SetAudioPathPosition(dwPathIndex,
													 lpSPD->dwTargetMtxIndex);

								// render audio changes
								RenderAudioChanges();
							}
							else
							{
								// set simulated 3D settings
								SimulateAudioPath3D(dwPathIndex,
													g_dwCamMtx,
													&g_vCamAhead,
													lpSPD->dwTargetMtxIndex,
													lpSPD->fMinDist,
													lpSPD->fMaxDist,
													lpSPD->fMaxVolVal);
							}

							// reset playback flag
							lpSPD->bPlayback=FALSE;

							// play segment
							PlayAudioSegment(lpSPD->dwSFXIndex,
											 dwPathIndex);
						}
					}
				}
			}
		}
	}
}

// RenderSoundFX()

void RenderSoundFX(void)
{
	// check 3D sound flag
	if(g_bUseSFX)
		if(g_bUse3DSound)
			RenderAudioChanges();
}

// ReleaseSoundFX()

void ReleaseSoundFX(void)
{
	// check sound flag
	if(g_bUseSFX)
	{
		// release audiopath manager
		ReleaseAudioPathManager();

		// release audio listener
		if(g_bUse3DSound)
			ReleaseAudioListener();
	}
}

// SetSoundPositionData()

void SetSoundPositionData(LPSOUNDPOSDATA lpSPD,
						  DWORD dwSFXIndex,
						  DWORD dwRepeatVal,
						  DWORD dwSFXPriVal,
						  float fMaxVolVal,
						  float fMinDist,
						  float fMaxDist,
						  DWORD dwNumTarget,
						  LPDWORD lpdwTargetMtxArray)
{
	// set values
	lpSPD->dwSFXIndex=dwSFXIndex;
	lpSPD->dwAudioPathID=NO_OBJECT;
	lpSPD->dwRepeatVal=dwRepeatVal;
	lpSPD->dwSFXPriVal=dwSFXPriVal;
	lpSPD->fMaxVolVal=fMaxVolVal;
	lpSPD->fMinDist=fMinDist;
	lpSPD->fMaxDist=fMaxDist;
	lpSPD->dwNumTarget=dwNumTarget;
	lpSPD->lpdwTargetMtxArray=lpdwTargetMtxArray;
	lpSPD->dwTargetMtxIndex=NO_OBJECT;
	lpSPD->bPlayback=TRUE;
}

// InitSoundPositionData()

BOOL InitSoundPositionData(void)
{
	LoadingOutput("InitSoundPositionData");

	// init eagle sfx
	SetSoundPositionData(&g_spdEagle,
						 g_dwEagleSFX,
						 REPEAT_EAGLE,
						 SFXPRI_EAGLE,
						 MAXVOL_EAGLE,
						 MINDIST_EAGLE,
						 MAXDIST_EAGLE,
						 g_dwNumBird,
						 g_lpdwBirdMtx);
	
	// init swan sfx #1
	SetSoundPositionData(&g_spdSwan1,
						 g_dwSwan1SFX,
						 REPEAT_SWAN1,
						 SFXPRI_SWAN1,
						 MAXVOL_SWAN1,
						 MINDIST_SWAN1,
						 MAXDIST_SWAN1,
						 g_dwNumSwan,
						 g_lpdwSwanMtx);
	
	// init swan sfx #2
	SetSoundPositionData(&g_spdSwan2,
						 g_dwSwan2SFX,
						 REPEAT_SWAN2,
						 SFXPRI_SWAN2,
						 MAXVOL_SWAN2,
						 MINDIST_SWAN2,
						 MAXDIST_SWAN2,
						 g_dwNumSwan,
						 g_lpdwSwanMtx);
	
	// init swan sfx #3
	SetSoundPositionData(&g_spdSwan3,
						 g_dwSwan3SFX,
						 REPEAT_SWAN3,
						 SFXPRI_SWAN3,
						 MAXVOL_SWAN3,
						 MINDIST_SWAN3,
						 MAXDIST_SWAN3,
						 g_dwNumSwan,
						 g_lpdwSwanMtx);
	
	// init baby swan sfx #1
	SetSoundPositionData(&g_spdBBSwan1,
						 g_dwBBSwan1SFX,
						 REPEAT_BBSWAN1,
						 SFXPRI_BBSWAN1,
						 MAXVOL_BBSWAN1,
						 MINDIST_BBSWAN1,
						 MAXDIST_BBSWAN1,
						 g_dwNumBBSwan,
						 g_lpdwBBSwanMtx);
	
	// init baby swan sfx #2
	SetSoundPositionData(&g_spdBBSwan2,
						 g_dwBBSwan2SFX,
						 REPEAT_BBSWAN2,
						 SFXPRI_BBSWAN2,
						 MAXVOL_BBSWAN2,
						 MINDIST_BBSWAN2,
						 MAXDIST_BBSWAN2,
						 g_dwNumBBSwan,
						 g_lpdwBBSwanMtx);
	
	// init birds sfx #1
	SetSoundPositionData(&g_spdBirds1,
						 g_dwBirds1SFX,
						 REPEAT_BIRDS1,
						 SFXPRI_BIRDS1,
						 MAXVOL_BIRDS1,
						 MINDIST_BIRDS1,
						 MAXDIST_BIRDS1,
						 SNDSRC_NUMSOURCES,
						 g_lpdwSndSrcMtx);
	
	// init birds sfx #2
	SetSoundPositionData(&g_spdBirds2,
						 g_dwBirds2SFX,
						 REPEAT_BIRDS2,
						 SFXPRI_BIRDS2,
						 MAXVOL_BIRDS2,
						 MINDIST_BIRDS2,
						 MAXDIST_BIRDS2,
						 SNDSRC_NUMSOURCES,
						 g_lpdwSndSrcMtx);

	// ok
	return(TRUE);
}

// UpdateSoundFX()

void UpdateSoundFX(void)
{
	// check sound flag
	if(g_bUseSFX)
	{
		// check 3D sound flag
		if(g_bUse3DSound)
		{
			// update listener position
			SetListenerPosition(g_dwCamMtx);

			// update listener orientation
			SetListenerOrientation(&g_vCamAhead);
		}

		// update sound positions
		UpdateSoundPosition(&g_spdEagle);
		if(!g_bPhantomSwan)
		{
			UpdateSoundPosition(&g_spdSwan1);
			UpdateSoundPosition(&g_spdSwan2);
			UpdateSoundPosition(&g_spdSwan3);
		}
		UpdateSoundPosition(&g_spdBBSwan1);
		UpdateSoundPosition(&g_spdBBSwan2);
		UpdateSoundPosition(&g_spdBirds1);
		UpdateSoundPosition(&g_spdBirds2);
	}
}

// InitSoundSources()

BOOL InitSoundSources(void)
{
	LoadingOutput("InitSoundSources");

	// counter
	DWORD dwCount;

	// angle value
	float fAngle;
	
	// position values
	float fX;
	float fZ;

	// set random sound sources
	for(dwCount=0;dwCount<SNDSRC_NUMSOURCES;dwCount++)
	{
		// create random position
		switch(abs(GetRandomInt())%3)
		{
		// upper round portion of boundary
		case(0):
			// set angle value
			fAngle=fabsf(GetRandomFloat()*PI);
			
			// set position
			fX=TERRAIN_CENTERX-(cosf(fAngle)*SNDSRC_OOBFACTOR*TERRAIN_XRANGE);
			fZ=(TERRAIN_CENTERZ-TERRAIN_ZRANGE)-(sinf(fAngle)*SNDSRC_OOBFACTOR*TERRAIN_XRANGE);

			// ok
			break;

		// center rectangular portion of boundary
		case(1):
			// set x-position
			if(GetRandomInt()<0)
				fX=TERRAIN_CENTERX+(SNDSRC_OOBFACTOR*TERRAIN_XRANGE);
			else
				fX=TERRAIN_CENTERX-(SNDSRC_OOBFACTOR*TERRAIN_XRANGE);

			// set z-position
			fZ=TERRAIN_CENTERZ+(GetRandomFloat()*TERRAIN_ZRANGE);

			// ok
			break;

		// lower round portion of boundary
		case(2):
			// set angle value
			fAngle=fabsf(GetRandomFloat()*PI);
			
			// set position
			fX=TERRAIN_CENTERX+(cosf(fAngle)*SNDSRC_OOBFACTOR*TERRAIN_XRANGE);
			fZ=(TERRAIN_CENTERZ+TERRAIN_ZRANGE)+(sinf(fAngle)*SNDSRC_OOBFACTOR*TERRAIN_XRANGE);

			// ok
			break;
		}

		// add matrix with new position
		g_lpdwSndSrcMtx[dwCount]=AddMatrix(1,0,0,0,
										   0,1,0,0,
										   0,0,1,0,
										   fX,0,fZ,1);

		// check matrix
		if(g_lpdwSndSrcMtx[dwCount]==NO_OBJECT)
			return(FALSE);
	}

	// ok
	return(TRUE);
}

// InitDragonflySFX()

BOOL InitDragonflySFX(void)
{
	LoadingOutput("InitDragonflySFX");

	// counter
	DWORD dwIndex;

	// mate index
	DWORD dwMateIndex;

	// assign audio segment to each dragonfly or couple
	for(dwIndex=0;dwIndex<g_dwNumDFly;dwIndex++)
	{
		// check counter
		if(dwIndex==0)
		{
			// assign original segment index
			g_lpdwDFlyBuzzIdx[dwIndex]=g_dwBuzzSFX;

			// set infinite repeats
			SetAudioSegmentRepeats(g_lpdwDFlyBuzzIdx[dwIndex]);
		}
		else
		{
			// get mate index
			dwMateIndex=g_lpdwDFlyMateIdx[dwIndex];

			// check mate index
			if(dwMateIndex==NO_OBJECT||
			   (dwMateIndex!=NO_OBJECT&&
			   g_lpdwDFlyBuzzIdx[dwMateIndex]==NO_OBJECT))
			{
				// assign cloned segment index
				g_lpdwDFlyBuzzIdx[dwIndex]=CloneAudioSegment(g_dwBuzzSFX);

				// check for error
				if(g_lpdwDFlyBuzzIdx[dwIndex]==NO_OBJECT)
					return(FALSE);

				// set infinite repeats
				SetAudioSegmentRepeats(g_lpdwDFlyBuzzIdx[dwIndex]);
			}
		}
	}

	// ok
	return(TRUE);
}

// InitFishSFX()

BOOL InitFishSFX(void)
{
	LoadingOutput("InitFishSFX");

	// counter
	DWORD dwIndex;

	// assign audio segment to each fish
	for(dwIndex=0;dwIndex<g_dwNumFish;dwIndex++)
	{
		// check counter
		if(dwIndex==0)
		{
			// assign original segment index
			g_lpdwFishSplashIdx[dwIndex]=g_dwSplashSFX;
		}
		else
		{
			// assign cloned segment index
			g_lpdwFishSplashIdx[dwIndex]=CloneAudioSegment(g_dwSplashSFX);

			// check for error
			if(g_lpdwFishSplashIdx[dwIndex]==NO_OBJECT)
				return(FALSE);
		}
	}

	// ok
	return(TRUE);
}

// AddCreatures()

BOOL AddCreatures(void)
{
	LoadingOutput("AddCreatures");

	// counter
	DWORD dwCount;

	// number of baby swans
	DWORD dwNumBBSwans;

	// set counter to number of swans
	dwCount=g_dwSceneNumSwans;

	// check number of swans
	if(dwCount)
	{	
		// check baby swans flag
		if(g_bSceneBBSwans)
		{
			// add swans & baby swans
			while(dwCount)
			{
				// check counter
				if(dwCount==1)
				{
					// add single swan
					if(AddSwanLoner()==NO_OBJECT)
						return(FALSE);

					// update counter
					dwCount--;
				}
				else if(dwCount==2)
				{
					// check random value
					switch(abs(GetRandomInt())%3)
					{
					case(0):
						// add single swan
						if(AddSwanLoner()==NO_OBJECT)
							return(FALSE);

						// update counter
						dwCount--;

						// ok
						break;
					case(1):
						// add swan couple
						if(AddSwanCouple()==NO_OBJECT)
							return(FALSE);

						// update counter
						dwCount-=2;

						// ok
						break;
					default:
						// add single swan with baby
						if(AddSwanSPFamily(1)==NO_OBJECT)
							return(FALSE);

						// update counter
						dwCount-=2;

						// ok
						break;
					}
				}
				else
				{
					// check random value
					switch(abs(GetRandomInt())%4)
					{
					case(0):
						// add single swan
						if(AddSwanLoner()==NO_OBJECT)
							return(FALSE);

						// update counter
						dwCount--;

						// ok
						break;
					case(1):
						// add swan couple
						if(AddSwanCouple()==NO_OBJECT)
							return(FALSE);

						// update counter
						dwCount-=2;

						// ok
						break;
					case(2):
						// get random number of baby swans
						dwNumBBSwans=(abs(GetRandomInt())%3)+1;

						// clip value as needed
						if((dwCount-1)<dwNumBBSwans)
							dwNumBBSwans=dwCount-1;

						// add single swan with babies
						if(AddSwanSPFamily(dwNumBBSwans)==NO_OBJECT)
							return(FALSE);

						// update counter
						dwCount-=dwNumBBSwans+1;

						// ok
						break;
					default:
						// get random number of baby swans
						dwNumBBSwans=(abs(GetRandomInt())%4)+2;

						// clip value as needed
						if((dwCount-2)<dwNumBBSwans)
							dwNumBBSwans=dwCount-2;

						// add swan couple with babies
						if(AddSwanDPFamily(dwNumBBSwans)==NO_OBJECT)
							return(FALSE);

						// update counter
						dwCount-=dwNumBBSwans+2;

						// ok
						break;
					}
				}
			}
		}
		else
		{
			// add swans
			while(dwCount)
			{
				// check counter
				if(dwCount==1)
				{
					// add single swan
					if(AddSwanLoner()==NO_OBJECT)
						return(FALSE);

					// update counter
					dwCount--;
				}
				else
				{
					// check random value
					switch(abs(GetRandomInt())%2)
					{
					case(0):
						// add single swan
						if(AddSwanLoner()==NO_OBJECT)
							return(FALSE);

						// update counter
						dwCount--;

						// ok
						break;
					default:
						// add swan couple
						if(AddSwanCouple()==NO_OBJECT)
							return(FALSE);

						// update counter
						dwCount-=2;

						// ok
						break;
					}
				}
			}
		}
	}
	else
	{
		// set phantom swan flag
		g_bPhantomSwan=TRUE;

		// add swan loner
		AddSwanLoner();
	}

	// add turtles
	for(dwCount=0;dwCount<g_dwSceneNumTurtles;dwCount++)
		if(AddTurtle()==NO_OBJECT)
			return(FALSE);

	// set counter to number of dragonflies
	dwCount=g_dwSceneNumDFlies;

	// add dragonflies
	while(dwCount)
	{
		// check counter
		if(dwCount==1)
		{
			// add single dragonfly
			if(AddDragonflyLoner()==NO_OBJECT)
				return(FALSE);

			// update counter
			dwCount--;
		}
		else
		{
			// check random value
			switch(abs(GetRandomInt())%2)
			{
			case(0):
				// add single dragonfly
				if(AddDragonflyLoner()==NO_OBJECT)
					return(FALSE);

				// update counter
				dwCount--;

				// ok
				break;
			default:
				// add dragonfly couple
				if(AddDragonflyCouple()==NO_OBJECT)
					return(FALSE);

				// update counter
				dwCount-=2;

				// ok
				break;
			}
		}
	}

	// add fish
	for(dwCount=0;dwCount<g_dwSceneNumFish;dwCount++)
		if(AddFish()==NO_OBJECT)
			return(FALSE);

	// add birds
	for(dwCount=0;dwCount<g_dwSceneNumBirds;dwCount++)
		if(AddBird()==NO_OBJECT)
			return(FALSE);

	// ok
	return(TRUE);
}

// SwanLakeProcessMessage()

BOOL SwanLakeProcessMessage(DWORD dwMsg,
							LPVOID lpData)
{
	// check message
	switch(dwMsg)
	{
	// toggle frame rate display
	case(MSG_TOGGLEFRAMERATE):

		// toggle flag
		if(g_bFrameRate)
			g_bFrameRate=FALSE;
		else
			g_bFrameRate=TRUE;

		// ok
		return(TRUE);

	// toggle high-speed mode
	case(MSG_TOGGLESPEEDMODE):

		// toggle flag
		if(g_bSpeedMode)
			g_bSpeedMode=FALSE;
		else
			g_bSpeedMode=TRUE;

		// ok
		return(TRUE);
	}

	// message not found
	return(FALSE);
}

// SwanLakeSetDataPath()

BOOL SwanLakeSetDataPath(void)
{
	LoadingOutput("SwanLakeSetDataPath");

	LockGlobalMsg()

	// check registry for path
	LPSTR lpPath=GetSystemWideRegistryKey("InstallPath");
	if(!lpPath)
	{
		// display message box
		DisplayInfoMessage("Installation path could not be found in the system registry.\n\n"
						   "Please run the setup program again.");

		// no good
		return(FALSE);
	}
	else
	{
		// copy value
		strcpy(g_lpMsg,
			   lpPath);

		// free key value
		FreeRegistryKey(lpPath);
	}


	// set data path
	if(!SetDataPathAbs((LPCSTR)g_lpMsg,
					   TRUE))
	{
		// display message box
		DisplayInfoMessage("The installation path is invalid.\n\n"
						   "Please run the setup program again.");

		UnlockGlobalMsg()

	// no good
		return(FALSE);
	}

	UnlockGlobalMsg()

	// ok
	return(TRUE);
}

// InitClockDigits()

BOOL InitClockDigits(void)
{
	LoadingOutput("InitClockDigits");

	// load & check texture
	g_dwClockDigit[0]=AddTexture("clock0.jpg");
	if(g_dwClockDigit[0]==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockDigit[1]=AddTexture("clock1.jpg");
	if(g_dwClockDigit[1]==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockDigit[2]=AddTexture("clock2.jpg");
	if(g_dwClockDigit[2]==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockDigit[3]=AddTexture("clock3.jpg");
	if(g_dwClockDigit[3]==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockDigit[4]=AddTexture("clock4.jpg");
	if(g_dwClockDigit[4]==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockDigit[5]=AddTexture("clock5.jpg");
	if(g_dwClockDigit[5]==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockDigit[6]=AddTexture("clock6.jpg");
	if(g_dwClockDigit[6]==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockDigit[7]=AddTexture("clock7.jpg");
	if(g_dwClockDigit[7]==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockDigit[8]=AddTexture("clock8.jpg");
	if(g_dwClockDigit[8]==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockDigit[9]=AddTexture("clock9.jpg");
	if(g_dwClockDigit[9]==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockAM=AddTexture("clockam.jpg");
	if(g_dwClockAM==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockPM=AddTexture("clockpm.jpg");
	if(g_dwClockPM==NO_OBJECT)
		return(FALSE);

	// load & check texture
	g_dwClockColon=AddTexture("clockcln.jpg");
	if(g_dwClockColon==NO_OBJECT)
		return(FALSE);

	// ok
	return(TRUE);
}

// RenderClock()

void RenderClock(void)
{
	// clock sizes
	#define CLOCK_DIGITH 16
	#define CLOCK_DIGITW 12
	#define CLOCK_COLONW 8
	#define CLOCK_AMPMW  16

	// clock color
	#define CLOCK_COLOR D3DCOLOR_COLORVALUE(1,1,0.8F,0.8F)

	// system time structure
	static SYSTEMTIME st;

	// am flag
	BOOL bAM;

	// hour value
	WORD wHour;

	// digit values
	DWORD dwH1;
	DWORD dwH2;
	DWORD dwM1;
	DWORD dwM2;
	DWORD dwS1;
	DWORD dwS2;

#ifdef SHAREWARE_MODE
	// get local time if necessary
	if(!g_bNagScreen)
		GetLocalTime(&st);
#else
	// get local time
	GetLocalTime(&st);
#endif

	// copy hour value
	wHour=st.wHour;
	
	// check am/pm
	if(wHour<12)
	{
		// set am flag
		bAM=TRUE;
	}
	else
	{
		// reset am flag
		bAM=FALSE;

		// adjust hour
		wHour-=12;
	}

	// fix for 12:00
	if(wHour==0)
		wHour=12;

	// extract digits
	dwH1=(DWORD)(((float)(wHour))/10.0F);
	dwM1=(DWORD)(((float)(st.wMinute))/10.0F);
	dwS1=(DWORD)(((float)(st.wSecond))/10.0F);
	dwH2=wHour-(dwH1*10);
	dwM2=st.wMinute-(dwM1*10);
	dwS2=st.wSecond-(dwS1*10);

	// begin sprite output
	if(BeginShadedSprites(g_bTexSmooth))
	{
		// output hour digit #1
		OutputShadedSprite(g_dwClockDigit[dwH1],
						   CLOCK_COLOR,
						   ScrCoordX(636-(CLOCK_AMPMW)-(CLOCK_DIGITW*5)-(CLOCK_COLONW*2)-(CLOCK_DIGITW/2)),
						   ScrCoordY(476-(CLOCK_DIGITH/2)),
						   ScrSizeX(CLOCK_DIGITW),
						   ScrSizeY(CLOCK_DIGITH));

		// output hour digit #2
		OutputShadedSprite(g_dwClockDigit[dwH2],
						   CLOCK_COLOR,
						   ScrCoordX(636-(CLOCK_AMPMW)-(CLOCK_DIGITW*4)-(CLOCK_COLONW*2)-(CLOCK_DIGITW/2)),
						   ScrCoordY(476-(CLOCK_DIGITH/2)),
						   ScrSizeX(CLOCK_DIGITW),
						   ScrSizeY(CLOCK_DIGITH));

		// output colon #1
		OutputShadedSprite(g_dwClockColon,
						   CLOCK_COLOR,
						   ScrCoordX(636-(CLOCK_AMPMW)-(CLOCK_DIGITW*4)-(CLOCK_COLONW/2)-(CLOCK_COLONW)),
						   ScrCoordY(476-(CLOCK_DIGITH/2)),
						   ScrSizeX(CLOCK_COLONW),
						   ScrSizeY(CLOCK_DIGITH));
		
		// output minute digit #1
		OutputShadedSprite(g_dwClockDigit[dwM1],
						   CLOCK_COLOR,
						   ScrCoordX(636-(CLOCK_AMPMW)-(CLOCK_DIGITW*3)-(CLOCK_COLONW)-(CLOCK_DIGITW/2)),
						   ScrCoordY(476-(CLOCK_DIGITH/2)),
						   ScrSizeX(CLOCK_DIGITW),
						   ScrSizeY(CLOCK_DIGITH));

		// output minute digit #2
		OutputShadedSprite(g_dwClockDigit[dwM2],
						   CLOCK_COLOR,
						   ScrCoordX(636-(CLOCK_AMPMW)-(CLOCK_DIGITW*2)-(CLOCK_COLONW)-(CLOCK_DIGITW/2)),
						   ScrCoordY(476-(CLOCK_DIGITH/2)),
						   ScrSizeX(CLOCK_DIGITW),
						   ScrSizeY(CLOCK_DIGITH));

		// output colon #2
		OutputShadedSprite(g_dwClockColon,
						   CLOCK_COLOR,
						   ScrCoordX(636-(CLOCK_AMPMW)-(CLOCK_DIGITW*2)-(CLOCK_COLONW/2)),
						   ScrCoordY(476-(CLOCK_DIGITH/2)),
						   ScrSizeX(CLOCK_COLONW),
						   ScrSizeY(CLOCK_DIGITH));

		// output second digit #1
		OutputShadedSprite(g_dwClockDigit[dwS1],
						   CLOCK_COLOR,
						   ScrCoordX(636-(CLOCK_AMPMW)-(CLOCK_DIGITW/2)-(CLOCK_DIGITW)),
						   ScrCoordY(476-(CLOCK_DIGITH/2)),
						   ScrSizeX(CLOCK_DIGITW),
						   ScrSizeY(CLOCK_DIGITH));

		// output second digit #2
		OutputShadedSprite(g_dwClockDigit[dwS2],
						   CLOCK_COLOR,
						   ScrCoordX(636-(CLOCK_AMPMW)-(CLOCK_DIGITW/2)),
						   ScrCoordY(476-(CLOCK_DIGITH/2)),
						   ScrSizeX(CLOCK_DIGITW),
						   ScrSizeY(CLOCK_DIGITH));

		// output am/pm
		if(bAM)
		{
			// output am
			OutputShadedSprite(g_dwClockAM,
							   CLOCK_COLOR,
							   ScrCoordX(636-(CLOCK_AMPMW/2)),
							   ScrCoordY(476-(CLOCK_DIGITH/2)),
							   ScrSizeX(CLOCK_AMPMW),
							   ScrSizeY(CLOCK_DIGITH));
		}
		else
		{
			// output pm
			OutputShadedSprite(g_dwClockPM,
							   CLOCK_COLOR,
							   ScrCoordX(636-(CLOCK_AMPMW/2)),
							   ScrCoordY(476-(CLOCK_DIGITH/2)),
							   ScrSizeX(CLOCK_AMPMW),
							   ScrSizeY(CLOCK_DIGITH));
		}

		// end sprite output
		EndSprites();
	}
}

// GetResoltionWidth()

DWORD GetResolutionWidth(LPSTR lpText)
{
	// counter
	DWORD dwCount;

	// copy of text
	LPSTR lpCopy;

	// return value
	DWORD dwRet;

	// allocate memory for text
	if(!AllocMem((LPVOID*)&lpCopy,
				 strlen(lpText)+1))
		return(0);

	// copy string
	strcpy(lpCopy,
		   lpText);

	// end string at 'x'
	for(dwCount=0;dwCount<strlen(lpCopy);dwCount++)
		if(lpCopy[dwCount]=='x')
			lpCopy[dwCount]=0;

	// convert value	
	dwRet=(DWORD)atoi(lpCopy);

	// free copy of text
	FreeMem((LPVOID*)&lpCopy);

	// return value
	return(dwRet);
}

// GetResolutionHeight()

DWORD GetResolutionHeight(LPSTR lpText)
{
	// counter
	DWORD dwCount;

	// copy of text
	LPSTR lpCopy;

	// return value
	DWORD dwRet;

	// allocate memory for text
	if(!AllocMem((LPVOID*)&lpCopy,
				 strlen(lpText)+1))
		return(0);

	// copy string
	strcpy(lpCopy,
		   lpText);

	// replace with '0' to 'x'
	for(dwCount=0;dwCount<strlen(lpCopy);dwCount++)
		if(lpCopy[dwCount]=='x')
		{
			// set value
			lpCopy[dwCount]='0';

			// end loop
			dwCount=strlen(lpCopy);
		}
		else
			lpCopy[dwCount]='0';

	// convert value	
	dwRet=(DWORD)atoi(lpCopy);

	// free copy of text
	FreeMem((LPVOID*)&lpCopy);

	// return value
	return(dwRet);
}

// SwanLakeConfigure()

void SwanLakeConfigure(HWND hParentWnd)
{
	LoadingOutput("SwanLakeConfigure");

	// validate window handle
	if(hParentWnd==INVALID_HANDLE_VALUE)
		hParentWnd=GetDesktopWindow();

	// verify current installation
	if(!VerifyDirectXVersion())
		return;

	// set data path
	if(!SwanLakeSetDataPath())
		return;

// this section must be after SetDataPath!
#ifdef SHAREWARE_MODE
	// get days left
	g_dwTrialDays=GetTrialDaysRemaining();
#else
	// reset leftover timestamps
	ResetTimeStamps();
#endif

	// load options
	if(!SwanLakeLoadOptions())
		DisplayErrorMessage("Unable to load application options.",
							NULL,
							FALSE);

	// add dialog boxes
	g_dwConfigDlg=AddDialogFromResource(IDD_CONFIGDIALOG);
	g_dwAboutDlg=AddDialogFromResource(IDD_ABOUTDIALOG);
#ifdef SHAREWARE_MODE
	g_dwPurchaseDlg=AddDialogFromResource(IDD_PURCHDIALOG);
#endif

	// check for errors
	if(g_dwConfigDlg==NO_OBJECT)
		return;
	if(g_dwAboutDlg==NO_OBJECT)
		return;
#ifdef SHAREWARE_MODE
	if(g_dwPurchaseDlg==NO_OBJECT)
		return;
#endif

#ifdef SHAREWARE_MODE
	// check days remaining
	if(g_dwTrialDays)
	{
		// execute config dialog box
		ExecuteDialog(g_dwConfigDlg,
					  SwanLakeConfigDlgProc,
					  hParentWnd);

		// save options
		if(!SwanLakeSaveOptions())
			DisplayErrorMessage("Unable to save application options.",
								NULL,
								FALSE);
	}
	else
	{
		// play sound
		MessageBeep(MB_ICONHAND);
		
		// execute purchase dialog box
		ExecuteDialog(g_dwPurchaseDlg,
					  SwanLakePurchaseDlgProc,
					  hParentWnd);
	}
#else
	// execute config dialog box
	ExecuteDialog(g_dwConfigDlg,
				  SwanLakeConfigDlgProc,
				  hParentWnd);

	// save options
	if(!SwanLakeSaveOptions())
		DisplayErrorMessage("Unable to save application options.",
							NULL,
							FALSE);
#endif

	// purge dialog boxes
	PurgeDialogs();
}

// SwanLakeLoadOptions()

BOOL SwanLakeLoadOptions(void)
{
	LoadingOutput("SwanLakeLoadOptions");

	// get graphics options
	g_dwHorzRes=  (DWORD)GetRegistryConfigValue(CFG_XRES_KEY,CFG_XRES_MIN,CFG_XRES_MAX,CFG_XRES_DEF);
	g_dwVertRes=  (DWORD)GetRegistryConfigValue(CFG_YRES_KEY,CFG_YRES_MIN,CFG_YRES_MAX,CFG_YRES_DEF);
	g_bTexSmooth=  (BOOL)GetRegistryConfigValue(CFG_STEX_KEY,CFG_STEX_MIN,CFG_STEX_MAX,CFG_STEX_DEF);
	g_bShadeSmooth=(BOOL)GetRegistryConfigValue(CFG_SSHD_KEY,CFG_SSHD_MIN,CFG_SSHD_MAX,CFG_SSHD_DEF);

	// get audio options
	g_bUseMusic=  (BOOL)GetRegistryConfigValue(CFG_MUSIC_KEY,CFG_MUSIC_MIN,CFG_MUSIC_MAX,CFG_MUSIC_DEF);
	g_bUseSFX=    (BOOL)GetRegistryConfigValue(CFG_SOUND_KEY,CFG_SOUND_MIN,CFG_SOUND_MAX,CFG_SOUND_DEF);
	g_bUse3DSound=(BOOL)GetRegistryConfigValue(CFG_3DSND_KEY,CFG_3DSND_MIN,CFG_3DSND_MAX,CFG_3DSND_DEF);
	g_fMasterVol=   ((float)GetRegistryConfigValue(CFG_MVOL_KEY,CFG_MVOL_MIN,CFG_MVOL_MAX,CFG_MVOL_DEF))/100.0F;
	g_fPrimaryVol=  ((float)GetRegistryConfigValue(CFG_PVOL_KEY,CFG_PVOL_MIN,CFG_PVOL_MAX,CFG_PVOL_DEF))/100.0F;
	g_fSecondaryVol=((float)GetRegistryConfigValue(CFG_SVOL_KEY,CFG_SVOL_MIN,CFG_SVOL_MAX,CFG_SVOL_DEF))/100.0F;

	// get environment options
	g_dwSceneNumSwans=  (DWORD)GetRegistryConfigValue(CFG_NUMSWANS_KEY,  CFG_NUMSWANS_MIN,  CFG_NUMSWANS_MAX,  CFG_NUMSWANS_DEF);
	g_dwSceneNumTurtles=(DWORD)GetRegistryConfigValue(CFG_NUMTURTLES_KEY,CFG_NUMTURTLES_MIN,CFG_NUMTURTLES_MAX,CFG_NUMTURTLES_DEF);
	g_dwSceneNumDFlies= (DWORD)GetRegistryConfigValue(CFG_NUMDFLIES_KEY, CFG_NUMDFLIES_MIN, CFG_NUMDFLIES_MAX, CFG_NUMDFLIES_DEF);
	g_dwSceneNumFish=   (DWORD)GetRegistryConfigValue(CFG_NUMFISH_KEY,   CFG_NUMFISH_MIN,   CFG_NUMFISH_MAX,   CFG_NUMFISH_DEF);
	g_dwSceneNumBirds=  (DWORD)GetRegistryConfigValue(CFG_NUMBIRDS_KEY,  CFG_NUMBIRDS_MIN,  CFG_NUMBIRDS_MAX,  CFG_NUMBIRDS_DEF);
	g_bFogMode=      (BOOL)GetRegistryConfigValue(CFG_FOG_KEY,    CFG_FOG_MIN,    CFG_FOG_MAX,    CFG_FOG_DEF);
	g_bLensFlareMode=(BOOL)GetRegistryConfigValue(CFG_LFLARE_KEY, CFG_LFLARE_MIN, CFG_LFLARE_MAX, CFG_LFLARE_DEF);
	g_bUseRipples=   (BOOL)GetRegistryConfigValue(CFG_RIPPLES_KEY,CFG_RIPPLES_MIN,CFG_RIPPLES_MAX,CFG_RIPPLES_DEF);
	g_bShowClock=    (BOOL)GetRegistryConfigValue(CFG_CLOCK_KEY,  CFG_CLOCK_MIN,  CFG_CLOCK_MAX,  CFG_CLOCK_DEF);

	// get advanced options
	g_bPriorityMode=(BOOL)GetRegistryConfigValue(CFG_PRIORITY_KEY, CFG_PRIORITY_MIN, CFG_PRIORITY_MAX, CFG_PRIORITY_DEF);
	g_dwBufferCnt= (DWORD)GetRegistryConfigValue(CFG_BUFFERCNT_KEY,CFG_BUFFERCNT_MIN,CFG_BUFFERCNT_MAX,CFG_BUFFERCNT_DEF);

	// set hidden options
	g_bSceneBBSwans=TRUE;
	g_bTerrainHiRes=TRUE;
	g_bFrameRate=FALSE;
	g_bSpeedMode=FALSE;

	// ok
	return(TRUE);
}

// SwanLakeSaveOptions()

BOOL SwanLakeSaveOptions(void)
{
	LoadingOutput("SwanLakeSaveOptions");

	// set graphics options
	SetRegistryConfigValue(CFG_XRES_KEY,(int)g_dwHorzRes);
	SetRegistryConfigValue(CFG_YRES_KEY,(int)g_dwVertRes);
	SetRegistryConfigValue(CFG_STEX_KEY,(int)g_bTexSmooth);
	SetRegistryConfigValue(CFG_SSHD_KEY,(int)g_bShadeSmooth);

	// set audio options
	SetRegistryConfigValue(CFG_MUSIC_KEY,(int)g_bUseMusic);
	SetRegistryConfigValue(CFG_SOUND_KEY,(int)g_bUseSFX);
	SetRegistryConfigValue(CFG_3DSND_KEY,(int)g_bUse3DSound);
	SetRegistryConfigValue(CFG_MVOL_KEY,(int)((g_fMasterVol*100.0F)+0.5F));
	SetRegistryConfigValue(CFG_PVOL_KEY,(int)((g_fPrimaryVol*100.0F)+0.5F));
	SetRegistryConfigValue(CFG_SVOL_KEY,(int)((g_fSecondaryVol*100.0F)+0.5F));

	// set environment options
	SetRegistryConfigValue(CFG_NUMSWANS_KEY,  (int)g_dwSceneNumSwans);
	SetRegistryConfigValue(CFG_NUMTURTLES_KEY,(int)g_dwSceneNumTurtles);
	SetRegistryConfigValue(CFG_NUMDFLIES_KEY, (int)g_dwSceneNumDFlies);
	SetRegistryConfigValue(CFG_NUMFISH_KEY,   (int)g_dwSceneNumFish);
	SetRegistryConfigValue(CFG_NUMBIRDS_KEY,  (int)g_dwSceneNumBirds);
	SetRegistryConfigValue(CFG_FOG_KEY,    (int)g_bFogMode);
	SetRegistryConfigValue(CFG_LFLARE_KEY, (int)g_bLensFlareMode);
	SetRegistryConfigValue(CFG_RIPPLES_KEY,(int)g_bUseRipples);
	SetRegistryConfigValue(CFG_CLOCK_KEY,  (int)g_bShowClock);

	// set advanced options
	SetRegistryConfigValue(CFG_PRIORITY_KEY, (int)g_bPriorityMode);
	SetRegistryConfigValue(CFG_BUFFERCNT_KEY,(int)g_dwBufferCnt);

	// ok
	return(TRUE);
}

// SwanLakeConfigDlgProc()

BOOL APIENTRY SwanLakeConfigDlgProc(HWND hDlg,
									UINT Msg,
									WPARAM wParam,
									LPARAM lParam)
{
	// resolutions list
	static RESOLUTIONS res;

	// counter
	DWORD dwCount;

	// logo button image handle
	static HBITMAP hLogo=(HBITMAP)INVALID_HANDLE_VALUE;

	// pointer to draw item structure
	LPDRAWITEMSTRUCT lpdis;

	// device context
	HDC hDC;

	// edit control values
	long lCurVal;
	long lMinEdit;
	long lMaxEdit;
	LPSTR lpEditErrTxt;

	// scroll bar values
	int nScrollCode;
	int nScrollPos;
	HWND hScroll;

	// check message
	switch(Msg)
	{
	// init dialog box
	case(WM_INITDIALOG):

		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// get available resolutions
		if(!GetAvailableResolutions(&res))
		{
			// end dialog box
			EndDialog(hDlg,NULL);

			// ok
			return(TRUE);
		}

		// init creature values
		SetCurrentDialogEditLong(IDC_NUMSWANS,  (long)g_dwSceneNumSwans);
		SetCurrentDialogEditLong(IDC_NUMTURTLES,(long)g_dwSceneNumTurtles);
		SetCurrentDialogEditLong(IDC_NUMDFLIES, (long)g_dwSceneNumDFlies);
		SetCurrentDialogEditLong(IDC_NUMFISH,   (long)g_dwSceneNumFish);
		SetCurrentDialogEditLong(IDC_NUMBIRDS,  (long)g_dwSceneNumBirds);

		// init environment values
		SetCurrentDialogCheckBox(IDC_FOGHAZE,   g_bFogMode);
		SetCurrentDialogCheckBox(IDC_LENSFLARES,g_bLensFlareMode);
		SetCurrentDialogCheckBox(IDC_RIPPLES,   g_bUseRipples);
		SetCurrentDialogCheckBox(IDC_CLOCK,     g_bShowClock);

		// init resolution list
		for(dwCount=0;dwCount<res.dwNumRes;dwCount++)
		{
			LockGlobalMsg()

			// format string
			sprintf(g_lpMsg,
					"%dx%d",
					res.lpRes[dwCount].dwX,
					res.lpRes[dwCount].dwY);

			// add to list
			AddCurrentDialogDropDownSelection(IDC_RESOLUTIONS,
											  g_lpMsg);

			UnlockGlobalMsg()
		}

		LockGlobalMsg()

		// format string for current selection
		sprintf(g_lpMsg,
				"%dx%d",
				g_dwHorzRes,
				g_dwVertRes);

		// highlight current selection
		if(!HighlightCurrentDialogDropDownSelection(IDC_RESOLUTIONS,g_lpMsg))
		{
			// set defult selection on failure
			HighlightCurrentDialogDropDownSelection(IDC_RESOLUTIONS,CFG_RES_DEF);
		}

		UnlockGlobalMsg()

		// init graphics values
		SetCurrentDialogCheckBox(IDC_TEXSMOOTH,  g_bTexSmooth);
		SetCurrentDialogCheckBox(IDC_SHADESMOOTH,g_bShadeSmooth);
		
		// init audio values
		SetCurrentDialogCheckBox(IDC_ENABLEMUSIC,g_bUseMusic);
		SetCurrentDialogCheckBox(IDC_ENABLESOUND,g_bUseSFX);
		SetCurrentDialogCheckBox(IDC_3DSOUND,    g_bUse3DSound);

		// init volume values
		SetCurrentDialogScrollInfo(IDC_MASTERVOL,CFG_MVOL_MIN,CFG_MVOL_MAX,1,(int)((g_fMasterVol*100.0F)+0.5F));
		SetCurrentDialogScrollInfo(IDC_MUSICVOL, CFG_PVOL_MIN,CFG_PVOL_MAX,1,(int)((g_fPrimaryVol*100.0F)+0.5F));
		SetCurrentDialogScrollInfo(IDC_SOUNDVOL, CFG_SVOL_MIN,CFG_SVOL_MAX,1,(int)((g_fSecondaryVol*100.0F)+0.5F));

		// init advanced options
		SetCurrentDialogCheckBox(IDC_PRIORITY,    g_bPriorityMode);
		if(g_dwBufferCnt==2)
			SetCurrentDialogCheckBox(IDC_TRIPLEBUFFER,TRUE);
		else
			SetCurrentDialogCheckBox(IDC_TRIPLEBUFFER,FALSE);

#ifdef SHAREWARE_MODE
		// show purchase button
		ShowWindow(GetDlgItem(g_hCurrDlg,
							  IDC_PURCHASE),
				   SW_SHOW);
#else
		// hide purchase button
		ShowWindow(GetDlgItem(g_hCurrDlg,
							  IDC_PURCHASE),
				   SW_HIDE);
#endif

		// load logo button image
		hLogo=(HBITMAP)LoadImage(g_hInst,
								 MAKEINTRESOURCE(IDB_LOGOBUTTON),
								 IMAGE_BITMAP,
								 LOGOA_BTN_X,
								 LOGOA_BTN_Y,
								 LR_DEFAULTCOLOR);

		// check for error
		if(hLogo==NULL)
			DisplayErrorMessage("Failed to load image.");
		else
			AddWinResource;

#ifdef SHAREWARE_MODE
		LockGlobalMsg()

		// get window caption
		if(GetWindowText(g_hCurrDlg,
						 g_lpMsg,
						 _MAX_PATH))
		{
			// append "trial version" to caption
			strcat(g_lpMsg," (Trial Version)");

			// set new caption
			SetWindowText(g_hCurrDlg,
						  g_lpMsg);
		}

		UnlockGlobalMsg()
#endif

		// ok
		return(TRUE);

	// window command
	case(WM_COMMAND):

		// check notification message
		switch(HIWORD(wParam))
		{
		// button clicked
		case(BN_CLICKED):

			// check button id
			switch(LOWORD(wParam))
			{
			// wavebreak logo
			case(IDC_OPENABOUT):

				// open about dialog
				ExecuteDialog(g_dwAboutDlg,
							  SwanLakeAboutDlgProc,
							  hDlg);

				// reset current dialog box
				SetCurrentDialogBox(hDlg);

				// ok
				return(TRUE);

			// reset defaults
			case(IDC_DEFAULTS):

				LockGlobalMsg()

				// copy application name
				strcpy(g_lpMsg,
					   ApplicationName());

#ifdef SHAREWARE_MODE
				// append "trial version" to name
				strcat(g_lpMsg,
					   " (Trial Version)");
#endif

				// let user verify
				if(MessageBox(g_hCurrDlg,
							  "This will reset all configuration settings back to their original values.\n\nAre you sure this is what you want to do?",
							  g_lpMsg,
							  MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL)==IDYES)
				{
					// reset creature values
					SetCurrentDialogEditLong(IDC_NUMSWANS,  CFG_NUMSWANS_DEF);
					SetCurrentDialogEditLong(IDC_NUMTURTLES,CFG_NUMTURTLES_DEF);
					SetCurrentDialogEditLong(IDC_NUMDFLIES, CFG_NUMDFLIES_DEF);
					SetCurrentDialogEditLong(IDC_NUMFISH,   CFG_NUMFISH_DEF);
					SetCurrentDialogEditLong(IDC_NUMBIRDS,  CFG_NUMBIRDS_DEF);

					// reset environment values
					SetCurrentDialogCheckBox(IDC_FOGHAZE,   (BOOL)CFG_FOG_DEF);
					SetCurrentDialogCheckBox(IDC_LENSFLARES,(BOOL)CFG_LFLARE_DEF);
					SetCurrentDialogCheckBox(IDC_RIPPLES,   (BOOL)CFG_RIPPLES_DEF);
					SetCurrentDialogCheckBox(IDC_CLOCK,     (BOOL)CFG_CLOCK_DEF);

					// reset defult selection on failure
					HighlightCurrentDialogDropDownSelection(IDC_RESOLUTIONS,CFG_RES_DEF);

					// reset graphics values
					SetCurrentDialogCheckBox(IDC_TEXSMOOTH,  (BOOL)CFG_STEX_DEF);
					SetCurrentDialogCheckBox(IDC_SHADESMOOTH,(BOOL)CFG_SSHD_DEF);
					
					// reset audio values
					SetCurrentDialogCheckBox(IDC_ENABLEMUSIC,(BOOL)CFG_MUSIC_DEF);
					SetCurrentDialogCheckBox(IDC_ENABLESOUND,(BOOL)CFG_SOUND_DEF);
					SetCurrentDialogCheckBox(IDC_3DSOUND,    (BOOL)CFG_3DSND_DEF);

					// reset volume values
					SetCurrentDialogScrollInfo(IDC_MASTERVOL,CFG_MVOL_MIN,CFG_MVOL_MAX,1,CFG_MVOL_DEF);
					SetCurrentDialogScrollInfo(IDC_MUSICVOL, CFG_PVOL_MIN,CFG_PVOL_MAX,1,CFG_PVOL_DEF);
					SetCurrentDialogScrollInfo(IDC_SOUNDVOL, CFG_SVOL_MIN,CFG_SVOL_MAX,1,CFG_SVOL_DEF);

					// reset advanced options
					SetCurrentDialogCheckBox(IDC_PRIORITY,    (BOOL)CFG_PRIORITY_DEF);
					SetCurrentDialogCheckBox(IDC_TRIPLEBUFFER,FALSE);
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);

#ifdef SHAREWARE_MODE
			// purchase
			case(IDC_PURCHASE):

				// open purchase dialog
				ExecuteDialog(g_dwPurchaseDlg,
							  SwanLakePurchaseDlgProc,
							  hDlg);

				// reset current dialog box
				SetCurrentDialogBox(hDlg);

				// ok
				return(TRUE);
#endif

			// help
			case(HIDC_HELP):

				// open help file
				HtmlHelp(NULL,
						 "swanlake.chm",
						 HH_DISPLAY_TOPIC,
						 NULL);

				// ok
				return(TRUE);

			// ok
			case(IDOK):

				LockGlobalMsg()

				// copy application name
				strcpy(g_lpMsg,ApplicationName());

#ifdef SHAREWARE_MODE
				// append "trial version" to name
				strcat(g_lpMsg," (Trial Version)");
#endif

				// check for priority mode
				if(GetCurrentDialogCheckBox(IDC_PRIORITY))
					if(MessageBox(g_hCurrDlg,
								  "You have chosen to run this application in priority mode. This setting may cause problems with some background programs (including audio playback) and will slow down any networking operations (such as shared printers and Internet connections) that run through this computer. (See the help file for more information.)\n\nAre you sure you want to keep this setting?",
								  g_lpMsg,
								  MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL)==IDNO)
					{
						// reset box
						SetCurrentDialogCheckBox(IDC_PRIORITY,
												 FALSE);

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}

				// check for triple buffer
				if(GetCurrentDialogCheckBox(IDC_TRIPLEBUFFER))
					if(MessageBox(g_hCurrDlg,
								  "You have chosen to use triple display buffering. Depending on your system configuration, this setting may cause the application to stop working properly. (See the help file for more information.)\n\nAre you sure you want to keep this setting?",
								  g_lpMsg,
								  MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL)==IDNO)
					{
						// reset box
						SetCurrentDialogCheckBox(IDC_TRIPLEBUFFER,
												 FALSE);

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}

				UnlockGlobalMsg()

				// save creature values
				g_dwSceneNumSwans  =GetCurrentDialogEditLong(IDC_NUMSWANS);
				g_dwSceneNumTurtles=GetCurrentDialogEditLong(IDC_NUMTURTLES);
				g_dwSceneNumDFlies =GetCurrentDialogEditLong(IDC_NUMDFLIES);
				g_dwSceneNumFish   =GetCurrentDialogEditLong(IDC_NUMFISH);
				g_dwSceneNumBirds  =GetCurrentDialogEditLong(IDC_NUMBIRDS);

				// save environment values
				g_bFogMode      =GetCurrentDialogCheckBox(IDC_FOGHAZE);
				g_bLensFlareMode=GetCurrentDialogCheckBox(IDC_LENSFLARES);
				g_bUseRipples   =GetCurrentDialogCheckBox(IDC_RIPPLES);
				g_bShowClock    =GetCurrentDialogCheckBox(IDC_CLOCK);

				LockGlobalMsg()

				// save resolution string
				GetCurrentDialogDropDownSelection(IDC_RESOLUTIONS,
												  g_lpMsg);

				// extract resolution values
				g_dwHorzRes=GetResolutionWidth(g_lpMsg);
				g_dwVertRes=GetResolutionHeight(g_lpMsg);

				UnlockGlobalMsg()

				// save graphics values
				g_bTexSmooth  =GetCurrentDialogCheckBox(IDC_TEXSMOOTH);
				g_bShadeSmooth=GetCurrentDialogCheckBox(IDC_SHADESMOOTH);
				
				// save audio values
				g_bUseMusic  =GetCurrentDialogCheckBox(IDC_ENABLEMUSIC);
				g_bUseSFX    =GetCurrentDialogCheckBox(IDC_ENABLESOUND);
				g_bUse3DSound=GetCurrentDialogCheckBox(IDC_3DSOUND);

				// save volume values
				g_fMasterVol=   ((float)GetCurrentDialogScrollPos(IDC_MASTERVOL))/100.0F;
				g_fPrimaryVol=  ((float)GetCurrentDialogScrollPos(IDC_MUSICVOL))/100.0F;
				g_fSecondaryVol=((float)GetCurrentDialogScrollPos(IDC_SOUNDVOL))/100.0F;

				// save advanced options
				g_bPriorityMode=GetCurrentDialogCheckBox(IDC_PRIORITY);
				if(GetCurrentDialogCheckBox(IDC_TRIPLEBUFFER))
					g_dwBufferCnt=2;
				else
					g_dwBufferCnt=1;

				// fall through to "cancel" for cleanup 

			// cancel
			case(IDCANCEL):

				// delete logo button image
				if(DeleteObject((HGDIOBJ)hLogo))
					RemoveWinResource;
				
				// release available resolutions
				ReleaseAvailableResolutions(&res);

				// end dialog box
				EndDialog(hDlg,NULL);

				// ok
				return(TRUE);
			}
			
			// ok
			break;

		// edit control change
		case(EN_CHANGE):
			
			// check edit control id
			switch(LOWORD(wParam))
			{
			// number of swans
			case(IDC_NUMSWANS):

				// set data
				lMinEdit=CFG_NUMSWANS_MIN;
				lMaxEdit=CFG_NUMSWANS_MAX;
				lpEditErrTxt=(LPSTR)"Number of Swans";
				
				// ok
				break;

			// number of turtles
			case(IDC_NUMTURTLES):

				// set data
				lMinEdit=CFG_NUMTURTLES_MIN;
				lMaxEdit=CFG_NUMTURTLES_MAX;
				lpEditErrTxt=(LPSTR)"Number of Turtles";
				
				// ok
				break;

			// number of dragonfiles
			case(IDC_NUMDFLIES):

				// set data
				lMinEdit=CFG_NUMDFLIES_MIN;
				lMaxEdit=CFG_NUMDFLIES_MAX;
				lpEditErrTxt=(LPSTR)"Number of Dragonflies";
				
				// ok
				break;

			// number of fish
			case(IDC_NUMFISH):

				// set data
				lMinEdit=CFG_NUMFISH_MIN;
				lMaxEdit=CFG_NUMFISH_MAX;
				lpEditErrTxt=(LPSTR)"Number of Fish";
				
				// ok
				break;

			// number of birds
			case(IDC_NUMBIRDS):

				// set data
				lMinEdit=CFG_NUMBIRDS_MIN;
				lMaxEdit=CFG_NUMBIRDS_MAX;
				lpEditErrTxt=(LPSTR)"Number of Eagles";
				
				// ok
				break;
			}

			// get current value
			lCurVal=GetCurrentDialogEditLong(LOWORD(wParam));

			// check value
			if(lCurVal<lMinEdit||
			   lCurVal>lMaxEdit)
			{
				// message string
				char lpMsg[128];

				// prepare message
				sprintf(lpMsg,
						"%s: Please enter a value from %d to %d.",
						lpEditErrTxt,
						lMinEdit,
						lMaxEdit);

				LockGlobalMsg()
				
				// copy application name
				strcpy(g_lpMsg,
					   ApplicationName());

#ifdef SHAREWARE_MODE
				// append "trial version" to name
				strcat(g_lpMsg," (Trial Version)");
#endif

				// display message
				MessageBox(g_hCurrDlg,
						   lpMsg,
						   g_lpMsg,
						   MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL|MB_SETFOREGROUND);

				UnlockGlobalMsg()

				// reset value
				if(lCurVal<lMinEdit)
					SetCurrentDialogEditLong(LOWORD(wParam),
											 lMinEdit);
				else
					SetCurrentDialogEditLong(LOWORD(wParam),
											 lMaxEdit);

				// highlight text
				HighlightCurrentDialogEditText(LOWORD(wParam));
			}

			// ok
			return(TRUE);

		// focus lost
		case(EN_KILLFOCUS):

			LockGlobalMsg()

			// if value is empty, set to 0
			if(!GetCurrentDialogEditText(LOWORD(wParam),
										 g_lpMsg,
										 _MAX_PATH))
				SetCurrentDialogEditLong(LOWORD(wParam),
										 0);

			UnlockGlobalMsg()

			// ok
			return(TRUE);
		}

		// ok
		break;

	// scroll bar message
	case(WM_HSCROLL):

		// extract data
		nScrollCode=(int)LOWORD(wParam);
		nScrollPos=(int)HIWORD(wParam);
		hScroll=(HWND)lParam;

		// check scroll code
		switch(nScrollCode)
		{
		case(SB_LINELEFT):      UpdateScrollBarPos(hScroll,-1);      break;
		case(SB_LINERIGHT):     UpdateScrollBarPos(hScroll,1);       break;
		case(SB_PAGELEFT):      UpdateScrollBarPos(hScroll,-10);     break;
		case(SB_PAGERIGHT):     UpdateScrollBarPos(hScroll,10);      break;
		case(SB_THUMBPOSITION): SetScrollBarPos(hScroll,nScrollPos); break;
		}

		// ok
		return(TRUE);

	// draw item
	case(WM_DRAWITEM):

		// cast pointer to draw item info
		lpdis=(LPDRAWITEMSTRUCT)lParam;

		// check for logo button
		if(wParam==IDC_WAVEBREAK)
		{
			// create device context
			hDC=CreateCompatibleDC(lpdis->hDC);

			// verify handle
			if(hDC)
			{
				// add windows resource
				AddWinResource;

				// select bitmap if available
				if(hLogo)
				{
					// select object
					SelectObject(hDC,
								 (HGDIOBJ)hLogo);

					// output image
					StretchBlt(lpdis->hDC,
							   lpdis->rcItem.left,
							   lpdis->rcItem.top,
							   lpdis->rcItem.right-lpdis->rcItem.left,
							   lpdis->rcItem.bottom-lpdis->rcItem.top,
							   hDC,
							   0,0,
							   LOGOA_BTN_X,
							   LOGOA_BTN_Y,
							   SRCCOPY);

					// de-select bitmap
					SelectObject(hDC,
								 NULL);
				}

				// delete device context
				if(DeleteDC(hDC))
					RemoveWinResource;
			}

			// ok
			return(TRUE);
		}

		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}

// SwanLakeAboutDlgProc()

BOOL APIENTRY SwanLakeAboutDlgProc(HWND hDlg,
								   UINT Msg,
								   WPARAM wParam,
								   LPARAM lParam)
{
	// image handles
	static HBITMAP hIcon=   (HBITMAP)INVALID_HANDLE_VALUE;
	static HBITMAP hLogo=   (HBITMAP)INVALID_HANDLE_VALUE;
	//static HBITMAP hASPLogo=(HBITMAP)INVALID_HANDLE_VALUE;

	// pointer to draw item structure
	LPDRAWITEMSTRUCT lpdis;

	// device context
	HDC hDC;

	// check message
	switch(Msg)
	{
	// init dialog box
	case(WM_INITDIALOG):

		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// load icon image
		hIcon=(HBITMAP)LoadImage(g_hInst,
								 MAKEINTRESOURCE(IDB_APPICONIMAGE),
								 IMAGE_BITMAP,
								 32,32,
								 LR_DEFAULTCOLOR);

		// check for error
		if(hIcon==NULL)
			DisplayErrorMessage("Failed to load image.");
		else
			AddWinResource;

		// load logo image
		hLogo=(HBITMAP)LoadImage(g_hInst,
								 MAKEINTRESOURCE(IDB_LOGOBUTTON2),
								 IMAGE_BITMAP,
								 LOGOB_BTN_X,
								 LOGOB_BTN_Y,
								 LR_DEFAULTCOLOR);

		// check for error
		if(hLogo==NULL)
			DisplayErrorMessage("Failed to load image.");
		else
			AddWinResource;

		/* removed ASP!
		// load ASP logo image
		hASPLogo=(HBITMAP)LoadImage(g_hInst,
								    MAKEINTRESOURCE(IDB_ASPLOGO),
								    IMAGE_BITMAP,
								    LOGOC_BTN_X,
								    LOGOC_BTN_Y,
								    LR_DEFAULTCOLOR);

		// check for error
		if(hASPLogo==NULL)
			DisplayErrorMessage("Failed to load image.");
		else
			AddWinResource;
		*/

#ifdef SHAREWARE_MODE
		LockGlobalMsg()

		// get window caption
		if(GetWindowText(g_hCurrDlg,
						 g_lpMsg,
						 _MAX_PATH))
		{
			// append "trial version" to caption
			strcat(g_lpMsg," (Trial Version)");

			// set new caption
			SetWindowText(g_hCurrDlg,
						  g_lpMsg);
		}

		UnlockGlobalMsg()
#endif

		// ok
		return(TRUE);

	// window command
	case(WM_COMMAND):

		// check notification message
		switch(HIWORD(wParam))
		{
		// button clicked
		case(BN_CLICKED):

			// check button id
			switch(LOWORD(wParam))
			{
			// visit homepage
			case(IDC_HOMEPAGE):

				// begin wait cursor
				BeginWaitCursor();
				
				// open homepage
				OpenExternalDoc("http://www.wb-software.com/");

				// end wait cursor
				EndWaitCursor();

				// ok
				return(TRUE);

			/* removed ASP!
			// visit homepage
			case(IDC_ASPBUTTON):

				// begin wait cursor
				BeginWaitCursor();

				// open ASP homepage
				OpenExternalDoc("http://www.asp-shareware.org/");
				
				// end wait cursor
				EndWaitCursor();

				// ok
				return(TRUE);
			*/

			// view license
			case(IDC_LICENSE):

				// open license file
				ExecuteCommandLine("notepad.exe",
								   "license.txt");

				// ok
				return(TRUE);

			// view readme
			case(IDC_README):

				// open readme file
				ExecuteCommandLine("notepad.exe",
								   "readme.txt");

				// ok
				return(TRUE);

			// ok & cancel
			case(IDOK):
			case(IDCANCEL):

				// delete icon image
				if(DeleteObject((HGDIOBJ)hIcon))
					RemoveWinResource;

				// delete logo button image
				if(DeleteObject((HGDIOBJ)hLogo))
					RemoveWinResource;

				/* removed ASP!
				// delete ASP logo image
				if(DeleteObject((HGDIOBJ)hASPLogo))
					RemoveWinResource;
				*/
				
				// end dialog box
				EndDialog(hDlg,NULL);

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}

		// ok
		break;

	// draw item
	case(WM_DRAWITEM):

		// cast pointer to draw item info
		lpdis=(LPDRAWITEMSTRUCT)lParam;

		// check id
		switch(wParam)
		{
		// swan lake icon
		case(IDC_SWANLAKEICON):

			// create device context
			hDC=CreateCompatibleDC(lpdis->hDC);

			// verify handle
			if(hDC)
			{
				// add windows resource
				AddWinResource;

				// select bitmap if available
				if(hIcon)
				{
					// select image
					SelectObject(hDC,(HGDIOBJ)hIcon);

					// output image
					StretchBlt(lpdis->hDC,
							   lpdis->rcItem.left,
							   lpdis->rcItem.top,
							   32,32,
							   hDC,
							   0,0,
							   32,32,
							   SRCCOPY);

					// de-select bitmap
					SelectObject(hDC,
								 NULL);
				}

				// delete device context
				if(DeleteDC(hDC))
					RemoveWinResource;
			}

			// ok
			return(TRUE);

		// wavebreak logo
		case(IDC_HOMEPAGE):

			// create device context
			hDC=CreateCompatibleDC(lpdis->hDC);

			// verify handle
			if(hDC)
			{
				// add windows resource
				AddWinResource;

				// select bitmap if available
				if(hLogo)
				{
					// select image
					SelectObject(hDC,(HGDIOBJ)hLogo);

					// output image
					StretchBlt(lpdis->hDC,
							   lpdis->rcItem.left,
							   lpdis->rcItem.top,
							   lpdis->rcItem.right-lpdis->rcItem.left,
							   lpdis->rcItem.bottom-lpdis->rcItem.top,
							   hDC,
							   0,0,
							   LOGOB_BTN_X,
							   LOGOB_BTN_Y,
							   SRCCOPY);

					// de-select bitmap
					SelectObject(hDC,
								 NULL);
				}

				// delete device context
				if(DeleteDC(hDC))
					RemoveWinResource;
			}

			// ok
			return(TRUE);

		/* removed ASP!
		// ASP logo
		case(IDC_ASPBUTTON):

			// create device context
			hDC=CreateCompatibleDC(lpdis->hDC);

			// verify handle
			if(hDC)
			{
				// add windows resource
				AddWinResource;

				// select bitmap if available
				if(hLogo)
				{
					// select image
					SelectObject(hDC,(HGDIOBJ)hASPLogo);

					// output image
					StretchBlt(lpdis->hDC,
							   lpdis->rcItem.left,
							   lpdis->rcItem.top,
							   lpdis->rcItem.right-lpdis->rcItem.left,
							   lpdis->rcItem.bottom-lpdis->rcItem.top,
							   hDC,
							   0,0,
							   LOGOC_BTN_X,
							   LOGOC_BTN_Y,
							   SRCCOPY);

					// de-select bitmap
					SelectObject(hDC,
								 NULL);
				}

				// delete device context
				if(DeleteDC(hDC))
					RemoveWinResource;
			}

			// ok
			return(TRUE);
		*/
		}

		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}

#ifdef SHAREWARE_MODE
// SwanLakePurchaseDlgProc()

BOOL APIENTRY SwanLakePurchaseDlgProc(HWND hDlg,
									  UINT Msg,
									  WPARAM wParam,
									  LPARAM lParam)
{
	// image handles
	static HBITMAP hCards1=(HBITMAP)INVALID_HANDLE_VALUE;
	static HBITMAP hCards2=(HBITMAP)INVALID_HANDLE_VALUE;
	static HBITMAP hLogo  =(HBITMAP)INVALID_HANDLE_VALUE;

	// pointer to draw item structure
	LPDRAWITEMSTRUCT lpdis;

	// device context
	HDC hDC;

	// message box text
	LPSTR lpMsgBoxTxt=NULL;

	// check message
	switch(Msg)
	{
	// init dialog box
	case(WM_INITDIALOG):

		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// set days-left text
		if(g_dwTrialDays==0)
		{
			// set text
			SetCurrentDialogEditText(IDC_DAYSLEFTTEXT,
									 "Your free trial has expired! Order now!");
		}
		else
		{
			LockGlobalMsg()

			// prepare text
			sprintf(g_lpMsg,
					"You have %d days left in your free trial!",
					g_dwTrialDays);

			// set text
			SetCurrentDialogEditText(IDC_DAYSLEFTTEXT,
									 g_lpMsg);

			UnlockGlobalMsg()
		}

		// load cards image #1
		hCards1=(HBITMAP)LoadImage(g_hInst,
								   MAKEINTRESOURCE(IDB_CCARDS1),
								   IMAGE_BITMAP,
								   CCARDS_BTN_X,
								   CCARDS_BTN_Y,
								   LR_DEFAULTCOLOR);

		// check for error
		if(hCards1==NULL)
			DisplayErrorMessage("Failed to load image.");
		else
			AddWinResource;

		// load cards image #2
		hCards2=(HBITMAP)LoadImage(g_hInst,
								   MAKEINTRESOURCE(IDB_CCARDS2),
								   IMAGE_BITMAP,
								   CCARDS_BTN_X,
								   CCARDS_BTN_Y,
								   LR_DEFAULTCOLOR);

		// check for error
		if(hCards2==NULL)
			DisplayErrorMessage("Failed to load image.");
		else
			AddWinResource;

		// load Swan Lake logo image
		hLogo=(HBITMAP)LoadImage(g_hInst,
								 MAKEINTRESOURCE(IDB_LOGOBUTTON3),
								 IMAGE_BITMAP,
								 SWANLAKE_BTN_X,
								 SWANLAKE_BTN_Y,
								 LR_DEFAULTCOLOR);

		// check for error
		if(hLogo==NULL)
			DisplayErrorMessage("Failed to load image.");
		else
			AddWinResource;

		// ok
		return(TRUE);

	// window command
	case(WM_COMMAND):

		// check notification message
		switch(HIWORD(wParam))
		{
		// button clicked
		case(BN_CLICKED):

			// check button id
			switch(LOWORD(wParam))
			{
			// order forms
			case(IDC_ORDERNOW):
			case(IDC_VIEWFORM):

				// begin wait cursor
				BeginWaitCursor();

				// check form
				if(LOWORD(wParam)==IDC_ORDERNOW)
				{
					// open order page
					OpenExternalDoc("http://www.wb-software.com/swanlake.htm");
				}
				else if(LOWORD(wParam)==IDC_VIEWFORM)
				{
					// open order form
					ExecuteCommandLine("notepad.exe",
									   "order.txt");
				}

				// end wait cursor
				EndWaitCursor();

				// fall through to close
				
			// ok & cancel
			case(IDOK):
			case(IDCANCEL):

				// delete cards image #1
				if(DeleteObject((HGDIOBJ)hCards1))
					RemoveWinResource;

				// delete cards image #2
				if(DeleteObject((HGDIOBJ)hCards2))
					RemoveWinResource;

				// delete Swan Lake logo image
				if(DeleteObject((HGDIOBJ)hLogo))
					RemoveWinResource;

				// end dialog box
				EndDialog(hDlg,NULL);

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}

		// ok
		break;

	// draw item
	case(WM_DRAWITEM):

		// cast pointer to draw item info
		lpdis=(LPDRAWITEMSTRUCT)lParam;

		// check image
		switch(wParam)
		{
		// cards image #1
		case(IDC_CARDS1):

			// create device context
			hDC=CreateCompatibleDC(lpdis->hDC);

			// verify handle
			if(hDC)
			{
				// add windows resource
				AddWinResource;

				// select bitmap if available
				if(hCards1)
				{
					// select object
					SelectObject(hDC,
								 (HGDIOBJ)hCards1);

					// output image
					StretchBlt(lpdis->hDC,
							   lpdis->rcItem.left,
							   lpdis->rcItem.top,
							   lpdis->rcItem.right-lpdis->rcItem.left,
							   lpdis->rcItem.bottom-lpdis->rcItem.top,
							   hDC,
							   0,0,
							   CCARDS_BTN_X,
							   CCARDS_BTN_Y,
							   SRCCOPY);

					// de-select bitmap
					SelectObject(hDC,
								 NULL);
				}

				// delete device context
				if(DeleteDC(hDC))
					RemoveWinResource;
			}

			// ok
			return(TRUE);

		// cards image #2
		case(IDC_CARDS2):

			// create device context
			hDC=CreateCompatibleDC(lpdis->hDC);

			// verify handle
			if(hDC)
			{
				// add windows resource
				AddWinResource;

				// select bitmap if available
				if(hCards2)
				{
					// select object
					SelectObject(hDC,
								 (HGDIOBJ)hCards2);

					// output image
					StretchBlt(lpdis->hDC,
							   lpdis->rcItem.left,
							   lpdis->rcItem.top,
							   lpdis->rcItem.right-lpdis->rcItem.left,
							   lpdis->rcItem.bottom-lpdis->rcItem.top,
							   hDC,
							   0,0,
							   CCARDS_BTN_X,
							   CCARDS_BTN_Y,
							   SRCCOPY);

					// de-select bitmap
					SelectObject(hDC,
								 NULL);
				}

				// delete device context
				if(DeleteDC(hDC))
					RemoveWinResource;
			}

			// ok
			return(TRUE);

		// Swan Lake logo image
		case(IDC_SLLOGO):

			// create device context
			hDC=CreateCompatibleDC(lpdis->hDC);

			// verify handle
			if(hDC)
			{
				// add windows resource
				AddWinResource;

				// select bitmap if available
				if(hLogo)
				{
					// select object
					SelectObject(hDC,
								 (HGDIOBJ)hLogo);

					// output image
					StretchBlt(lpdis->hDC,
							   lpdis->rcItem.left,
							   lpdis->rcItem.top,
							   lpdis->rcItem.right-lpdis->rcItem.left,
							   lpdis->rcItem.bottom-lpdis->rcItem.top,
							   hDC,
							   0,0,
							   SWANLAKE_BTN_X,
							   SWANLAKE_BTN_Y,
							   SRCCOPY);

					// de-select bitmap
					SelectObject(hDC,
								 NULL);
				}

				// delete device context
				if(DeleteDC(hDC))
					RemoveWinResource;
			}

			// ok
			return(TRUE);
		}

		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}

// SwanLakeExecutePurchaseDlg()

BOOL SwanLakeExecutePurchaseDlg(void)
{
	// set data path
	if(!SwanLakeSetDataPath())
		return(FALSE);
	
// this section must be after SetDataPath!
#ifdef SHAREWARE_MODE
	// get days left
	g_dwTrialDays=GetTrialDaysRemaining();
#else
	// reset leftover timestamps
	ResetTimeStamps();
#endif

	// add dialog box
	DWORD dwDlg=AddDialogFromResource(IDD_PURCHDIALOG);

	// check for error
	if(dwDlg==NO_OBJECT)
		return(FALSE);

	// execute dialog box
	ExecuteDialog(dwDlg,
				  SwanLakePurchaseDlgProc,
				  GetDesktopWindow());

	// purge dialog boxes
	PurgeDialogs();

	// ok
	return(TRUE);
}

// InitNagScreen()

BOOL InitNagScreen(void)
{
	// load main texture
	if(g_dwTrialDays>TRIALDAYS_WARN)
		g_dwNagMain=AddTexture("n_thanks.jpg");
	else if(g_dwTrialDays>0)
		g_dwNagMain=AddTexture("n_expsn.jpg");
	else
		g_dwNagMain=AddTexture("n_expd.jpg");

	// load remaining textures
	g_dwNagTFlash   =AddTexture("n_tflash.jpg");
	g_dwNagClose    =AddTexture("n_close.jpg");
	g_dwNagRedX     =AddTexture("n_redx.jpg");
	g_dwNagDigits[0]=AddTexture("n_digit0.jpg");
	g_dwNagDigits[1]=AddTexture("n_digit1.jpg");
	g_dwNagDigits[2]=AddTexture("n_digit2.jpg");
	g_dwNagDigits[3]=AddTexture("n_digit3.jpg");
	g_dwNagDigits[4]=AddTexture("n_digit4.jpg");
	g_dwNagDigits[5]=AddTexture("n_digit5.jpg");
	g_dwNagDigits[6]=AddTexture("n_digit6.jpg");
	g_dwNagDigits[7]=AddTexture("n_digit7.jpg");
	g_dwNagDigits[8]=AddTexture("n_digit8.jpg");
	g_dwNagDigits[9]=AddTexture("n_digit9.jpg");
	g_dwNagCursor   =AddTexture("n_cursor.dds");
	g_dwNagBlank    =AddTexture("n_blank.bmp");

	// check for errors
	if(g_dwNagMain     ==NO_OBJECT||
	   g_dwNagClose    ==NO_OBJECT||
	   g_dwNagRedX     ==NO_OBJECT||
	   g_dwNagDigits[0]==NO_OBJECT||
	   g_dwNagDigits[1]==NO_OBJECT||
	   g_dwNagDigits[2]==NO_OBJECT||
	   g_dwNagDigits[3]==NO_OBJECT||
	   g_dwNagDigits[4]==NO_OBJECT||
	   g_dwNagDigits[5]==NO_OBJECT||
	   g_dwNagDigits[6]==NO_OBJECT||
	   g_dwNagDigits[7]==NO_OBJECT||
	   g_dwNagDigits[8]==NO_OBJECT||
	   g_dwNagDigits[9]==NO_OBJECT||
	   g_dwNagCursor   ==NO_OBJECT||
	   g_dwNagBlank    ==NO_OBJECT)
		return(FALSE);

	// check sound flag
	if(g_bUseSFX)
	{
		// load nag sound
		g_dwNagSnd=AddAudioSegment("n_chord1.wav");

		// check for error
		if(g_dwNagSnd==NO_OBJECT)
			return(FALSE);
	}

	// set maximum values
	if(g_dwTrialDays==TRIALDAYS_MAX)
	{
		g_dwNagFreqMax =NAG_FREQ1;
		g_dwNagDelayMax=NAG_DELAY1;
	}
	else if(g_dwTrialDays>TRIALDAYS_WARN)
	{
		g_dwNagFreqMax =NAG_FREQ2;
		g_dwNagDelayMax=NAG_DELAY2;
	}
	else if(g_dwTrialDays>0)
	{
		g_dwNagFreqMax =NAG_FREQ3;
		g_dwNagDelayMax=NAG_DELAY3;
	}
	else
	{
		g_dwNagFreqMax =NAG_FREQ4;
		g_dwNagDelayMax=NAG_DELAY4;
	}
	
	// reset active flag
	g_bNagScreen=FALSE;

	// reset frequency counter
	if(g_dwTrialDays==0)
		g_dwNagFreqCur=g_dwNagFreqMax;
	else
		g_dwNagFreqCur=0;
	
	// ok
	return(TRUE);
}

// UpdateNagScreen()

void UpdateNagScreen(void)
{
	// check active flag
	if(g_bNagScreen)
	{
		// check delay value
		if(g_dwNagDelayCur!=0)
		{
			// update counter
			g_dwNagDelayCur--;

			// check value
			if(g_dwNagDelayCur==0)
				CloseNagScreen();

			// check pre-close flag
			if(g_bPreClose)
			{
				// allow 1/4 sec for fade-in
				if(g_dwNagAnimPos>15)
				{
					// reset delay value
					g_dwNagDelayCur=0;

					// allow immediate close
					CloseNagScreen();
				}
			}
		}

		// update animation counter
		g_dwNagAnimPos++;

		// check pre-close flag
		if(g_bPreClose)
		{
			// enable mouse if necessary
			if(!g_bMouseCursor)
				EnableMouseCursor(g_dwNagCursor,
								  -4,-11,
								  ScrSize(32,32));
			
			// method below is faulty
			/*
			// cursor flag
			static BOOL bCursor=FALSE;

			// check flag
			if(!bCursor)
			{
				// show system cursor
				ShowWindowsCursor();

				// set flag
				bCursor=TRUE;
			}
			*/
		}
	}
	else
	{
		// update counter
		g_dwNagFreqCur++;

		// check value
		if(g_dwNagFreqCur>=g_dwNagFreqMax)
			OpenNagScreen();

		// check pre-close flag
		if(g_bPreClose)
			OpenNagScreen();
	}
}

// OpenNagScreen()

void OpenNagScreen(void)
{
	// check active flag
	if(!g_bNagScreen)
	{
		// set active flag
		g_bNagScreen=TRUE;

		// reset delay & position
		g_dwNagDelayCur=g_dwNagDelayMax;
		g_dwNagAnimPos =0;

		// reset speed mode
		if(g_bSpeedMode)
			g_bSpeedMode=FALSE;

		// pause audio output
		PauseAudioOutput();
	}
}

// CloseNagScreen()

void CloseNagScreen(void)
{
	// set closing position
	g_dwNagAnimPos=NAG_CLOSEVAL;
}

// RenderNagScreen()

void RenderNagScreen(void)
{
	// alpha value
	float fAlpha;

	// fade-in delay	
	#define FADEIN_DELAY 8

	// sound-played flag
	static BOOL bSoundPlayed=FALSE;

	// title flash flag
	BOOL bTFlash=FALSE;

	// number of seconds
	DWORD dwSec;

	// trial days digits
	DWORD dwDays1;
	DWORD dwDays2;

	// seconds flash flag
	BOOL bSFlash=FALSE;

	// close flag
	BOOL bClose=FALSE;

	// check animation position
	if(g_dwNagAnimPos>=NAG_CLOSEVAL)
	{
		// check pre-close flag
		if(g_bPreClose)
		{
			// reset animation position
			g_dwNagAnimPos=NAG_CLOSEVAL;

			// check for mouse click
			if(g_bMouseClkL)
			{
				// check close area
				if(IsMouseInArea(428,116,444,132))
					g_bCloseOK=TRUE;

				// check purchase area
				if(IsMouseInArea(199,299,440,323))
				{
					// load purchase page
					g_bPurchPage=TRUE;

					// allow close
					g_bCloseOK=TRUE;
				}
			}

			// check for esc key
			if(IsKeyDown(VK_ESCAPE))
				g_bCloseOK=TRUE;
			
			// check for enter key
			if(IsKeyDown(VK_RETURN))
			{
				// set purchase page
				g_bPurchPage=TRUE;

				// allow close
				g_bCloseOK=TRUE;
			}

			// check close-ok flag
			if(g_bCloseOK)
			{
				// resume audio output
				ResumeAudioOutput();

				// stop all segments
				StopAllAudioSegments();

				// program will close after this loop
			}

			// set alpha value
			fAlpha=1.0F;

			// inactivity timer
			static DWORD dwTimer=0;

			// check activity flags
			if(g_bMouseMove||
			   g_bKeyPress)
			{
				// reset timer
				dwTimer=0;
			}
			else
			{
				// increment timer
				dwTimer+=g_dwFrameSkip;

				// check for overflow
				if(dwTimer>NAG_MAXIDLE)
				{
					// reset pre-close flag
					g_bPreClose=FALSE;

					// disable cursor
					DisableMouseCursor();

					// reset timer
					dwTimer=0;
				}
			}
		}
		else
		{
			// offset animation position
			DWORD dwAnimPos=g_dwNagAnimPos-NAG_CLOSEVAL;

			// check offset position
			if(dwAnimPos>=FADEIN_DELAY)
			{
				// set alpha value
				fAlpha=0.0F;

				// reset nag screen flag
				g_bNagScreen=FALSE;

				// reset frequency value
				g_dwNagFreqCur=0;

				// resume audio output
				ResumeAudioOutput();
			}
			else
			{
				// set alpha value
				fAlpha=1.0F-(((float)dwAnimPos)/((float)FADEIN_DELAY));
			}
		}
	}
	else
	{
		// check animation position
		if(g_dwNagAnimPos>=FADEIN_DELAY)
		{
			// set alpha value
			fAlpha=1.0F;

			// check sound-played flag
			if(!bSoundPlayed)
			{
				// play sound if necessary
				if(g_bUseSFX)
					PlayAudioSegment(g_dwNagSnd);

				// set flag
				bSoundPlayed=TRUE;
			}

			// offset animation posisiton
			DWORD dwAnimPos=g_dwNagAnimPos-FADEIN_DELAY;

			// set flash flag if needed
			if((dwAnimPos>=0 &&dwAnimPos<=4)||
			   (dwAnimPos>=10&&dwAnimPos<=14))
				bTFlash=TRUE;
		}
		else
		{
			// set alpha value
			fAlpha=((float)g_dwNagAnimPos)/((float)FADEIN_DELAY);

			// reset sound-played flag
			bSoundPlayed=FALSE;
		}
	}

	// compute number of seconds
	dwSec=(DWORD)((((float)g_dwNagDelayCur)/60.0F))+1;

	// clip number of seconds
	if(dwSec>9) dwSec=9;

	// set seconds flash flag if needed
	if((g_dwNagDelayCur%60)>35)
		bSFlash=TRUE;

	// extract trial days digits
	dwDays1=(DWORD)(((float)(g_dwTrialDays))/10.0F);
	dwDays2=g_dwTrialDays-(dwDays1*10);

	// clip trial days digits
	if(dwDays1>9) dwDays1=9;
	if(dwDays2>9) dwDays2=9;

	// set close flag as needed
	if(g_dwNagDelayCur==0&&
	   g_bPreClose)
		bClose=TRUE;

	// enable alpha blend
	EnableAlphaBlend();
	
	// fade screen
	FadeScreen(D3DCOLOR_COLORVALUE(0,0,0,(fAlpha*0.5F)-1.0F));

	// begin sprite output
	if(BeginShadedSprites(TRUE))
	{
		// output main window
		OutputShadedSprite(g_dwNagMain,
						   D3DCOLOR_COLORVALUE(1.0F,1.0F,1.0F,fAlpha),
						   ScrCoords(320,240),
						   ScrSize(256,256));

		// output title flash if needed
		if(bTFlash)
			OutputShadedSprite(g_dwNagTFlash,
							   D3DCOLOR_COLORVALUE(1.0F,1.0F,1.0F,fAlpha),
							   ScrCoords(320,124),
							   ScrSize(256,16));

		// check seconds flash flag
		if(bSFlash)
		{
			// output blank flash
			OutputShadedSprite(g_dwNagBlank,
							   D3DCOLOR_COLORVALUE(1.0F,1.0F,1.0F,fAlpha),
							   ScrCoords(320,354),
							   ScrSize(160,16));
		}
		else
		{
			// output seconds digit
			OutputShadedSprite(g_dwNagDigits[dwSec],
							   D3DCOLOR_COLORVALUE(1.0F,1.0F,1.0F,fAlpha),
							   ScrCoords(323.8F,352),
							   ScrSize(6.5F,10.5F));
		}

		// check trial days
		if(g_dwTrialDays)
		{
			// output trial days digit #1
			OutputShadedSprite(g_dwNagDigits[dwDays1],
							   D3DCOLOR_COLORVALUE(1.0F,1.0F,1.0F,fAlpha),
							   ScrCoords(375,286),
							   ScrSize(6.5F,10.5F));

			// output trial days digit #2
			OutputShadedSprite(g_dwNagDigits[dwDays2],
							   D3DCOLOR_COLORVALUE(1.0F,1.0F,1.0F,fAlpha),
							   ScrCoords(382,286),
							   ScrSize(6.5F,10.5F));
		}

		// check close flag
		if(bClose)
		{
			// output close box
			OutputShadedSprite(g_dwNagRedX,
							   D3DCOLOR_COLORVALUE(1.0F,1.0F,1.0F,fAlpha),
							   ScrCoords(320,124),
							   ScrSize(256,24));

			// output close message
			OutputShadedSprite(g_dwNagClose,
							   D3DCOLOR_COLORVALUE(1.0F,1.0F,1.0F,fAlpha),
							   ScrCoords(320,352),
							   ScrSize(256,32));
		}

		// end sprite output
		EndShadedSprites();
	}

	// disable alpha blend
	DisableAlphaBlend();
}

// SwanLakeLoadPurchasePage()

BOOL SwanLakeLoadPurchasePage(void)
{
	// begin wait cursor
	BeginWaitCursor();

	// open purchase page
	BOOL bRet=OpenExternalDoc("http://www.wb-software.com/swanlake.htm");

	// end wait cursor
	EndWaitCursor();

	// ok
	return(bRet);
}

// GetTrialDaysRemaining()

DWORD GetTrialDaysRemaining(void)
{
	// timestamp locations
	LPSTR lpTSLoc=NULL;

	// individual timestamps
	TIMESTAMP lpTS[6];

	// final timestamp
	TIMESTAMP ts;

	// unique id
	DWORD dwID=NO_OBJECT;

	// allocate memory for locations
	if(!AllocMem((LPVOID*)&lpTSLoc,
				 132))
		return(0);

	// get timestamp locations
	GetTimeStampLocations(lpTSLoc);

	// get unique id
	dwID=GetHDDSerialNumber();

	// get timestamps
	GetTimeStamp(&lpTS[0],0,dwID,lpTSLoc);
	GetTimeStamp(&lpTS[1],1,dwID,lpTSLoc);
	GetTimeStamp(&lpTS[2],2,dwID,lpTSLoc);
	GetTimeStamp(&lpTS[3],3,dwID,lpTSLoc);
	GetTimeStamp(&lpTS[4],4,dwID,lpTSLoc);
	GetTimeStamp(&lpTS[5],5,dwID,lpTSLoc);

	// reset output timestamp
	ts.dwDaysLeft=TRIALDAYS_MAX;
	ts.dwID=dwID;
	ts.dwInstTimeHigh=NULL;
	ts.dwInstTimeLow=NULL;

	// sort timestamps
	SortTimeStamps(&ts,lpTS,6);

	// save timestamps
	SetTimeStamp(&ts,0,lpTSLoc);
	SetTimeStamp(&ts,1,lpTSLoc);
	SetTimeStamp(&ts,2,lpTSLoc);
	SetTimeStamp(&ts,3,lpTSLoc);
	SetTimeStamp(&ts,4,lpTSLoc);
	SetTimeStamp(&ts,5,lpTSLoc);

	// clear timestamp locations
	ClearTimeStampLocations(lpTSLoc,
							132);

	// free memory for locations
	FreeMem((LPVOID*)&lpTSLoc);

	// ok
	return(ts.dwDaysLeft);
}
#endif

// GetTimeStampLocations()

void GetTimeStampLocations(LPSTR lpStr)
{
	// timestamps are located as follows:
	// (1): (win.ini)  [scr32_snl] sysid=~
	// (2): bwapps.ini [appcommon] inf32=~
	// (3): HK_LM/Software/ SLS/ProfileData
	// (4): HK_LM/Software/ Microsoft/74ABE3D7-BAF5-4ac2-B848-C655118023E5
	// (5): C:/Windows/     slrcs32.sbw (every other byte starting at #53)
	// (6): in swanlake.dat

	// copy values into string
	lpStr[39] ='f';
	lpStr[84] ='a';
	lpStr[107]='l';
	lpStr[101]='2';
	lpStr[90] ='4';
	lpStr[58] =NULL;
	lpStr[89] ='8';
	lpStr[14] ='d';
	lpStr[126]='.';
	lpStr[61] ='c';
	lpStr[36] =NULL;
	lpStr[80] ='F';
	lpStr[26] =NULL;
	lpStr[109]='c';
	lpStr[16] ='b';
	lpStr[112]='2';
	lpStr[85] ='c';
	lpStr[52] ='l';
	lpStr[59] ='M';
	lpStr[34] ='o';
	lpStr[32] ='m';
	lpStr[120]='a';
	lpStr[103]='E';
	lpStr[44] ='L';
	lpStr[60] ='i';
	lpStr[46] ='\\';
	lpStr[127]='d';
	lpStr[117]=NULL;
	lpStr[7]  ='n';
	lpStr[77] ='-';
	lpStr[21] ='s';
	lpStr[25] ='i';
	lpStr[12] ='s';
	lpStr[78] ='B';
	lpStr[45] ='S';
	lpStr[42] =NULL;
	lpStr[20] ='p';
	lpStr[88] ='B';
	lpStr[65] ='o';
	lpStr[76] ='7';
	lpStr[111]='3';
	lpStr[37] ='i';
	lpStr[106]='s';
	lpStr[40] ='3';
	lpStr[69] ='7';
	lpStr[92] ='-';
	lpStr[96] ='5';
	lpStr[43] ='S';
	lpStr[129]='t';
	lpStr[33] ='m';
	lpStr[29] ='p';
	lpStr[67] ='t';
	lpStr[95] ='5';
	lpStr[75] ='D';
	lpStr[70] ='4';
	lpStr[98] ='1';
	lpStr[122]='l';
	lpStr[99] ='8';
	lpStr[116]='w';
	lpStr[41] ='2';
	lpStr[23] ='i';
	lpStr[102]='3';
	lpStr[38] ='n';
	lpStr[128]='a';
	lpStr[50] ='f';
	lpStr[8]  ='l';
	lpStr[55] ='a';
	lpStr[79] ='A';
	lpStr[5]  ='_';
	lpStr[100]='0';
	lpStr[118]='s';
	lpStr[47] ='P';
	lpStr[94] ='6';
	lpStr[18] ='a';
	lpStr[35] ='n';
	lpStr[2]  ='r';
	lpStr[87] ='-';
	lpStr[3]  ='3';
	lpStr[30] ='c';
	lpStr[68] ='\\';
	lpStr[1]  ='c';
	lpStr[51] ='i';
	lpStr[124]='k';
	lpStr[123]='a';
	lpStr[48] ='r';
	lpStr[6]  ='s';
	lpStr[19] ='p';
	lpStr[93] ='C';
	lpStr[56] ='t';
	lpStr[22] ='.';
	lpStr[63] ='o';
	lpStr[66] ='f';
	lpStr[71] ='A';
	lpStr[15] =NULL;
	lpStr[105]=NULL;
	lpStr[10] ='s';
	lpStr[28] ='p';
	lpStr[27] ='a';
	lpStr[113]='.';
	lpStr[130]=NULL;
	lpStr[86] ='2';
	lpStr[82] ='-';
	lpStr[114]='s';
	lpStr[54] ='D';
	lpStr[24] ='n';
	lpStr[83] ='4';
	lpStr[125]='e';
	lpStr[53] ='e';
	lpStr[17] ='w';
	lpStr[64] ='s';
	lpStr[81] ='5';
	lpStr[119]='w';
	lpStr[49] ='o';
	lpStr[11] ='y';
	lpStr[104]='5';
	lpStr[72] ='B';
	lpStr[110]='s';
	lpStr[13] ='i';
	lpStr[4]  ='2';
	lpStr[9]  =NULL;
	lpStr[57] ='a';
	lpStr[0]  ='s';
	lpStr[74] ='3';
	lpStr[91] ='8';
	lpStr[97] ='1';
	lpStr[62] ='r';
	lpStr[31] ='o';
	lpStr[73] ='E';
	lpStr[108]='r';
	lpStr[121]='n';
	lpStr[115]='b';
	lpStr[131]=NULL;
}

// ClearTimeStampLocations()

void ClearTimeStampLocations(LPSTR lpStr,
							 DWORD dwBytes)
{
	// counter
	DWORD dwCount;

	// randomize bytes
	for(dwCount=0;dwCount<dwBytes;dwCount++)
		lpStr[dwCount]=GetRandomInt()&0xFF;
}

#ifdef SHAREWARE_MODE
// SetTimeStamp()

void SetTimeStamp(LPTIMESTAMP lpTS,
				  DWORD dwLocID,
				  LPSTR lpTSLoc)
{
	// timestamps are located as follows:
	// (1): (win.ini)  [scr32_snl] sysid=~
	// (2): bwapps.ini [appcommon] inf32=~
	// (3): HK_LM/Software/ SLS/ProfileData
	// (4): HK_LM/Software/ Microsoft/74ABE3D7-BAF5-4ac2-B848-C655118023E5
	// (5): C:/Windows/     slrcs32.sbw (every other byte starting at #53)
	// (6): in swanlake.dat

	// encoded timestamp
	char lpEnc[33];

	// encode timestamp
	EncodeTimeStamp((*lpTS),
					lpEnc);

	// check location
	switch(dwLocID)
	{
	case(0):

		// save profile string
		WriteProfileString(GetStringDivision(lpTSLoc,0),
						   GetStringDivision(lpTSLoc,1),
						   lpEnc);

		// ok
		break;

	case(1):

		// save private profile string
		WritePrivateProfileString(GetStringDivision(lpTSLoc,3),
								  GetStringDivision(lpTSLoc,4),
								  lpEnc,
								  GetStringDivision(lpTSLoc,2));

		// ok
		break;

	case(2):

		LockGlobalMsg()

		// prepare key string
		sprintf(g_lpMsg,
				"%s\\%s",
				"Software",
				GetStringDivision(lpTSLoc,5));

		// set registry value
		RegSetValue(HKEY_LOCAL_MACHINE,
					g_lpMsg,
					REG_SZ,
					lpEnc,
					32);

		UnlockGlobalMsg()

		// ok
		break;

	case(3):

		LockGlobalMsg()
		
		// prepare key string
		sprintf(g_lpMsg,
				"%s\\%s",
				"Software",
				GetStringDivision(lpTSLoc,6));

		// set registry value
		RegSetValue(HKEY_LOCAL_MACHINE,
					g_lpMsg,
					REG_SZ,
					lpEnc,
					32);

		UnlockGlobalMsg()

		// ok
		break;

	case(4):

		// set string to file
		SetTimeStampStringToFile(lpEnc,
								 GetStringDivision(lpTSLoc,7));

		// ok
		break;

	case(5):

		// set string to data file
		SetTimeStampStringToDataFile(lpEnc,
									 GetStringDivision(lpTSLoc,8));

		// ok
		break;

	}
}

// GetTimeStamp()

void GetTimeStamp(LPTIMESTAMP lpTS,
				  DWORD dwLocID,
				  DWORD dwUniID,
				  LPSTR lpTSLoc)
{
	// timestamps are located as follows:
	// (1): (win.ini)  [scr32_snl] sysid=~
	// (2): bwapps.ini [appcommon] inf32=~
	// (3): HK_LM/Software/ SLS/ProfileData
	// (4): HK_LM/Software/ Microsoft/74ABE3D7-BAF5-4ac2-B848-C655118023E5
	// (5): C:/Windows/     slrcs32.sbw (every other byte starting at #53)
	// (6): in swanlake.dat

	// encoded timestamp
	char lpEnc[33];

	// set null in case of error
	lpEnc[0]=NULL;

	// data buffer size
	LONG cbSize;

	// check location
	switch(dwLocID)
	{
	case(0):

		// read profile string
		GetProfileString(GetStringDivision(lpTSLoc,0),
						 GetStringDivision(lpTSLoc,1),
						 "q",
						 lpEnc,
						 33);

		// ok
		break;

	case(1):

		// read private profile string
		GetPrivateProfileString(GetStringDivision(lpTSLoc,3),
								GetStringDivision(lpTSLoc,4),
								"w",
								lpEnc,
								33,
								GetStringDivision(lpTSLoc,2));

		// ok
		break;

	case(2):

		LockGlobalMsg()

		// prepare key string
		sprintf(g_lpMsg,
				"%s\\%s",
				"Software",
				GetStringDivision(lpTSLoc,5));

		// get size of registry value
		RegQueryValue(HKEY_LOCAL_MACHINE,
					  g_lpMsg,
					  NULL,
					  &cbSize);

		// check size
		if(cbSize>33)
		{
			// set default string
			strcpy(lpEnc,"e");
		}
		else
		{
			// get registry data
			RegQueryValue(HKEY_LOCAL_MACHINE,
						  g_lpMsg,
						  lpEnc,
						  &cbSize);
		}

		UnlockGlobalMsg()

		// ok
		break;

	case(3):

		LockGlobalMsg()

		// prepare key string
		sprintf(g_lpMsg,
				"%s\\%s",
				"Software",
				GetStringDivision(lpTSLoc,6));

		// get size of registry value
		RegQueryValue(HKEY_LOCAL_MACHINE,
					  g_lpMsg,
					  NULL,
					  &cbSize);

		// check size
		if(cbSize>33)
		{
			// set default string
			strcpy(lpEnc,"r");
		}
		else
		{
			// get registry data
			RegQueryValue(HKEY_LOCAL_MACHINE,
						  g_lpMsg,
						  lpEnc,
						  &cbSize);
		}

		UnlockGlobalMsg()

		// ok
		break;

	case(4):

		// get string from file
		GetTimeStampStringFromFile(lpEnc,
								   GetStringDivision(lpTSLoc,7));

		// ok
		break;

	case(5):

		// get string from data file
		GetTimeStampStringFromDataFile(lpEnc,
									   GetStringDivision(lpTSLoc,8));

		// ok
		break;
	}

	// decode timestamp
	(*lpTS)=DecodeTimeStamp(lpEnc);

	// check id
	if(lpTS->dwID!=dwUniID)
	{
		// set default values
		lpTS->dwDaysLeft=TRIALDAYS_MAX;
		lpTS->dwID=dwUniID;
		lpTS->dwInstTimeHigh=NULL;
		lpTS->dwInstTimeLow=NULL;
	}
}

// SortTimeStamps()

void SortTimeStamps(LPTIMESTAMP lpOut,
					LPTIMESTAMP lpTS,
					DWORD dwNumTS)
{
	// counter
	DWORD dwCount;

	// current time
	FILETIME ft;

	// 64-bit time values
	QWORD qwInstTime;
	QWORD qwCurTime;

	// copy first timestamp
	(*lpOut)=lpTS[0];

	// sort through timestamps
	for(dwCount=1;dwCount<dwNumTS;dwCount++)
	{
		// set lowest days
		if(lpTS[dwCount].dwDaysLeft<lpOut->dwDaysLeft)
			lpOut->dwDaysLeft=lpTS[dwCount].dwDaysLeft;

		// set 64-bit date values
		QWORD qwDateLow=(((QWORD)lpOut->dwInstTimeHigh)       <<32)+((QWORD)lpOut->dwInstTimeLow);
		QWORD qwDateCur=(((QWORD)lpTS[dwCount].dwInstTimeHigh)<<32)+((QWORD)lpTS[dwCount].dwInstTimeLow);

		// check for null date
		if(qwDateCur!=NULL)
		{
			// compare values
			if(qwDateCur<qwDateLow)
			{
				// set new value
				lpOut->dwInstTimeHigh=(DWORD)((qwDateCur&0xFFFFFFFF00000000I64)>>32);
				lpOut->dwInstTimeLow =(DWORD)((qwDateCur&0x00000000FFFFFFFFI64));
			}
		}
	}

	// get current time
	GetSystemTimeAsFileTime(&ft);
	
	// check for null date
	if(lpOut->dwInstTimeHigh==NULL&&
	   lpOut->dwInstTimeLow ==NULL)
	{
		// set current date
		lpOut->dwInstTimeHigh=ft.dwHighDateTime;
		lpOut->dwInstTimeLow =ft.dwLowDateTime;
	}

	// set current time
	qwCurTime=(((QWORD)ft.dwHighDateTime)<<32)+((QWORD)ft.dwLowDateTime);

	// get install time
	qwInstTime=(((QWORD)lpOut->dwInstTimeHigh)<<32)+((QWORD)lpOut->dwInstTimeLow);

	// compare current & install times
	if(qwCurTime<qwInstTime)
	{
		// cheater!!
		lpOut->dwDaysLeft=0;

		// ok
		return;
	}

	// compute days left based on install time
	DWORD dwDaysLeft=TRIALDAYS_MAX-((DWORD)((qwCurTime-qwInstTime)/CNSEC_PER_DAY));

	// check for overflow
	if(dwDaysLeft>TRIALDAYS_MAX)
	{
		// trial expired
		lpOut->dwDaysLeft=0;

		// ok
		return;
	}

	// set lowest days left
	if(dwDaysLeft<lpOut->dwDaysLeft)
		lpOut->dwDaysLeft=dwDaysLeft;
}
#else
void ResetTimeStamps(void)
{
	/* no longer needed - software is freeware
	// timestamps are located as follows:
	// (1): (win.ini)  [scr32_snl] sysid=~
	// (2): bwapps.ini [appcommon] inf32=~
	// (3): HK_LM/Software/ SLS/ProfileData
	// (4): HK_LM/Software/ Microsoft/74ABE3D7-BAF5-4ac2-B848-C655118023E5
	// (5): C:/Windows/     slrcs32.sbw (every other byte starting at #53)
	// (6): in swanlake.dat

	// timestamp locations
	LPSTR lpTSLoc=NULL;

	// allocate memory for locations
	if(!AllocMem((LPVOID*)&lpTSLoc,
				 132))
		return;

	// get timestamp locations
	GetTimeStampLocations(lpTSLoc);

	// reset location #1
	WriteProfileString(GetStringDivision(lpTSLoc,0),
					   GetStringDivision(lpTSLoc,1),
					   "q");

	// reset location #2
	WritePrivateProfileString(GetStringDivision(lpTSLoc,3),
							  GetStringDivision(lpTSLoc,4),
							  "w",
							  GetStringDivision(lpTSLoc,2));

	LockGlobalMsg()
	
	// prepare key string
	sprintf(g_lpMsg,
			"%s\\%s",
			"Software",
			GetStringDivision(lpTSLoc,5));

	// reset location #3
	RegSetValue(HKEY_LOCAL_MACHINE,
				g_lpMsg,
				REG_SZ,
				"e",
				1);

	// prepare key string
	sprintf(g_lpMsg,
			"%s\\%s",
			"Software",
			GetStringDivision(lpTSLoc,6));

	// reset location #4
	RegSetValue(HKEY_LOCAL_MACHINE,
				g_lpMsg,
				REG_SZ,
				"r",
				1);

	UnlockGlobalMsg()

	// reset location #5
	SetTimeStampStringToFile("t",
							 GetStringDivision(lpTSLoc,7));

	// reset location #6
	SetTimeStampStringToDataFile("y",
								 GetStringDivision(lpTSLoc,8));

	// clear timestamp locations
	ClearTimeStampLocations(lpTSLoc,
							132);

	// free memory for locations
	FreeMem((LPVOID*)&lpTSLoc);*/
}
#endif

// eof //

// Copyright 2005-2011 WaveBreak Software
