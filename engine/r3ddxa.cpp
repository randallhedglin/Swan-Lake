// Copyright 2005-2007 WaveBreak Software

// r3ddxa.cpp -- DirectX Audio wrapper

// includes //

#include"r3dwin.h"

// initialize globals //

// audio initialized flag
BOOL g_bAudioInit=FALSE;
// performance interface
IDirectMusicPerformance8* g_lpPerformance=NULL;
// audio loader interface
IDirectMusicLoader8* g_lpAudioLoader=NULL;
// internal interfaces
IDirectMusic8* g_lpDM=NULL;
IDirectSound8* g_lpDS=NULL;
// audio device capabilities
DSCAPS g_lpAudioDevCaps;
// default audiopath interface
IDirectMusicAudioPath8* g_lpDefAudioPath=NULL;
// default audiopath buffer
IDirectSoundBuffer8* g_lpDefAudioBuf=NULL;
// audio segment counters
DWORD g_dwNumAudioSeg=0;
DWORD g_dwMaxAudioSeg=0;
// audio segment data
LPBYTE* g_lplpAudioData=NULL;
IDirectMusicSegment8** g_lplpAudioSeg=NULL;
// audio volume values
float g_fAudioMasterVol=0;
float g_fAudioPrimaryVol=0;
float g_fAudioSecondaryVol=0;
// audio path counters
DWORD g_dwNumAudioPath=0;
DWORD g_dwMaxAudioPath=0;
// audio path data
IDirectMusicAudioPath8** g_lplpAudioPath=NULL;
IDirectSound3DBuffer8** g_lplpAudioBuf3D=NULL;
IDirectSoundBuffer8** g_lplpAudioBuf=NULL;
float* g_lpfAudioPathVol=NULL;
LPBOOL g_lpbAudioPath3DPrepared=NULL;
LPBOOL g_lpbAudioPath3DEnabled=NULL;
// audio listener
IDirectSound3DListener8* g_lpAudioListener=NULL;
// primary audio buffer
IDirectSoundBuffer* g_lpPrimaryBuf=NULL;
// audio master tempo
float g_fAudioMasterTempo=0;
// primary segment data
IDirectMusicSegment8* g_lpPrimarySeg=NULL;
MUSIC_TIME g_mtCurPriSegPos;
// audio distance factor
float g_fAudioDistFactor=0;
// 3d audio enabled flag
BOOL g_b3DAudioEnabled=FALSE;
// audiopath manager data
DWORD g_dwNumMAudioPaths=0;
DWORD g_dwMAudioPathBaseIdx=NO_OBJECT;
DWORD g_dwMAudioPathCurID=0;
LPDWORD g_lpdwMAudioPathID=NULL;
LPDWORD g_lpdwMAudioPathPriority=NULL;
// audio pause flag
BOOL g_bAudioPaused=FALSE;

// functions //

// DXAudioInit()

BOOL DXAudioInit(void)
{
	LoadingOutput("DXAudioInit");

	// reset initialized flag
	g_bAudioInit=FALSE;

	// set initialized flag
	BOOL bInit=TRUE;

	// init com
	if(!InitializeCOM())
		bInit=FALSE;

	// init performance
	if(bInit)
		if(!InitPerformance())
			bInit=FALSE;

	// init audio loader
	if(bInit)
		if(!InitAudioLoader())
			bInit=FALSE;

	// init default audiopath
	if(bInit)
		if(!InitDefAudioPath())
			bInit=FALSE;

	// init primary buffer
	if(bInit)
		if(!InitPrimaryBuffer())
			bInit=FALSE;

	// copy flag
	g_bAudioInit=bInit;

#ifdef USE_AVWARNINGS
	// check flag
	if(!g_bAudioInit)
	{
		// check registry for flag
		LPSTR lpFlag=GetRegistryKey("DXNoSndMsg");
		if(!lpFlag)
		{
			// display message box
			DisplayInfoMessage("Required audio support has not been detected.\n\n"
							   "Application will run without audio.\n\nThis message will not be displayed again.");

			// set flag
			SetRegistryKey("DXNoSndMsg",
						   "TRUE");
		}
		else
		{
			// release flag
			FreeRegistryKey(lpFlag);
		}
	}
#endif
	
	// begin exclusive mode
	BeginAudioExclusiveMode();

	// reset volume levels
	SetAudioMasterVolume(1.0F);
	SetAudioPrimaryVolume(1.0F);
	SetAudioSecondaryVolume(1.0F);

	// reset master tempo
	SetAudioMasterTempo(1.0F);

	// disable 3d audio
	Disable3DAudio();
	
	// ok
	return(TRUE);
}

// DXAudioRelease()

void DXAudioRelease(void)
{
	// purge objects
	PurgeAudioObjects();

	// end exclusive mode
	EndAudioExclusiveMode();

	// reset initialized flag
	g_bAudioInit=FALSE;

	// release interfaces
	ReleasePrimaryBuffer();
	ReleaseDefAudioPath();
	ReleaseAudioLoader();
	ReleasePerformance();

	// release com
	ReleaseCOM();
}

// InitPerformance()

BOOL InitPerformance(void)
{
	LoadingOutput("InitPerformance");

	// create performance
	HRESULT hr=CoCreateInstance(CLSID_DirectMusicPerformance,
								NULL,
								CLSCTX_INPROC,
								IID_IDirectMusicPerformance8,
								(LPVOID*)&g_lpPerformance);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;

	// internal pointers
	IDirectMusic* lpDM=NULL;
	IDirectSound* lpDS=NULL;

	// init performance
	hr=g_lpPerformance->InitAudio(&lpDM,
								  &lpDS,
								  g_hWnd,
								  DMUS_APATH_SHARED_STEREOPLUSREVERB,
								  128,
								  DMUS_AUDIOF_ALL,
								  NULL);

	// check for error
	if(FAILED(hr))
		return(FALSE);

	// update music interface
	hr=lpDM->QueryInterface(IID_IDirectMusic8,
							(LPVOID*)&g_lpDM);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;

	// update sound interface
	hr=lpDS->QueryInterface(IID_IDirectSound8,
							(LPVOID*)&g_lpDS);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;

	// initialize structure
	InitStruct(g_lpAudioDevCaps);
	
	// set size
	g_lpAudioDevCaps.dwSize=sizeof(g_lpAudioDevCaps);
	
	// get audio device caps
	g_lpDS->GetCaps(&g_lpAudioDevCaps);

	// verify audio device settings
	if(!VerifyAudioDeviceCaps())
		return(FALSE);

	// ok
	return(TRUE);
}

// InitAudioLoader()

BOOL InitAudioLoader(LPCSTR lpPath)
{
	LoadingOutput("InitAudioLoader");

	// create loader
	HRESULT hr=CoCreateInstance(CLSID_DirectMusicLoader,
								NULL,
								CLSCTX_INPROC,
								IID_IDirectMusicLoader8,
								(LPVOID*)&g_lpAudioLoader);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;
	
	// set audio search path
	if(!SetAudioSearchPath(lpPath))
		return(FALSE);

	// ok
	return(TRUE);
}

// ReleasePerformance()

void ReleasePerformance(void)
{
	// release internal pointers
	ReleaseCOMPtr(g_lpDM);
	ReleaseCOMPtr(g_lpDS);

	// close down performance
	if(g_lpPerformance)
		g_lpPerformance->CloseDown();
	
	// release performance
	ReleaseCOMPtr(g_lpPerformance);
}

// ReleaseAudioLoader()

void ReleaseAudioLoader(void)
{
	// release pointer
	ReleaseCOMPtr(g_lpAudioLoader);
}

// SetAudioSearchPath()

BOOL SetAudioSearchPath(LPCSTR lpPath)
{
	LoadingOutput("SetAudioSearchPath");

	// check interface
	if(g_lpAudioLoader)
	{
		// check pointer
		if(lpPath)
		{
			// convert to wide
			MakeWideString(g_lpWMsg,
						   lpPath);
		}
		else
		{
			// just to verify
			LockGlobalMsg()
			UnlockGlobalMsg()

			// get current directory
			if(!getcwd(g_lpMsg,
					   _MAX_PATH-1))
				return(FALSE);

			// convert to wide
			MakeWideString(g_lpWMsg,
						   g_lpMsg);
		}

		// set path
		HRESULT hr=g_lpAudioLoader->SetSearchDirectory(GUID_DirectMusicAllTypes,
													   g_lpWMsg,
													   TRUE);

		// check for error
		if(FAILED(hr))
			return(FALSE);
	}

	// ok
	return(TRUE);
}

// StopAllAudioSegments()

void StopAllAudioSegments(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// stop all output
		g_lpPerformance->Stop(NULL,
							  NULL,
							  0,
							  0);
	}
}

// ClearAudioCache()

void ClearAudioCache(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// check interface
		if(g_lpAudioLoader)
		{
			// collect garbage
			g_lpAudioLoader->CollectGarbage();

			// clear cache
			g_lpAudioLoader->ClearCache(GUID_DirectMusicAllTypes);
		}
	}
}

// GetNextAudioSegment()

DWORD GetNextAudioSegment(void)
{
	LoadingOutput("GetNextAudioSegment");

	// check counter
	if(g_dwNumAudioSeg==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpAudioData,
					 sizeof(LPBYTE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpAudioSeg,
					 sizeof(IDirectMusicSegment8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAudioSeg=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumAudioSeg>=g_dwMaxAudioSeg)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpAudioData,
					  sizeof(LPBYTE)*g_dwMaxAudioSeg,
					  sizeof(LPBYTE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpAudioSeg,
					  sizeof(IDirectMusicSegment8*)*g_dwMaxAudioSeg,
					  sizeof(IDirectMusicSegment8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAudioSeg+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumAudioSeg;

	// increment counter
	g_dwNumAudioSeg++;

	// return index
	return(dwIndex);
}

// AddAudioSegment()

DWORD AddAudioSegment(LPCSTR lpFile)
{
	LoadingOutput("AddAudioSegment");

	// used for error checking
	HRESULT hr;

	// get return index
	DWORD dwIndex=GetNextAudioSegment();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// check for data file
	DATAFILEREF dfr=FindDataFileRef((LPSTR)lpFile);

	// check flag
	if(g_bAudioInit)
	{
		// load segment
		if(dfr.lpData)
		{
			// allocate memory for audio data
			if(!AllocMem((LPVOID*)&g_lplpAudioData[dwIndex],
						 dfr.dwSize))
				return(NO_OBJECT);

			// copy audio data
			CopyMemory((LPVOID)g_lplpAudioData[dwIndex],
					   dfr.lpData,
					   dfr.dwSize);

			// prepare object description
			DMUS_OBJECTDESC dmod;
			InitStruct(dmod);
			dmod.dwSize=sizeof(DMUS_OBJECTDESC);
			dmod.dwValidData=DMUS_OBJ_CLASS|
							 DMUS_OBJ_MEMORY;
			dmod.guidClass=CLSID_DirectMusicSegment;
			dmod.pbMemData=g_lplpAudioData[dwIndex];
			dmod.llMemLength=(LONGLONG)dfr.dwSize;

			// get object
			hr=g_lpAudioLoader->GetObject(&dmod,
										  IID_IDirectMusicSegment8,
										  (LPVOID*)&g_lplpAudioSeg[dwIndex]);
		}
		else
		{
			// reset audio data pointer
			g_lplpAudioData[dwIndex]=NULL;

			// convert filename to wide string
			MakeWideString(g_lpWMsg,
						   lpFile);
			
			// load segment
			hr=g_lpAudioLoader->LoadObjectFromFile(CLSID_DirectMusicSegment,
												   IID_IDirectMusicSegment8,
												   g_lpWMsg,
												   (LPVOID*)&g_lplpAudioSeg[dwIndex]);
		}

		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to load audio segment.",
								  (LPSTR)lpFile,
								  hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// set parameters for std midi (fails silently if file is not midi)
		g_lplpAudioSeg[dwIndex]->SetParam(GUID_StandardMIDIFile,
										  0xFFFFFFFFL,
										  0,
										  0,
										  NULL);

		// download any necessary resources
		hr=g_lplpAudioSeg[dwIndex]->Download(g_lpPerformance);
		
		// check for error
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to download audio resources.",
								  (LPSTR)lpFile,
								  hr);
	}

	// return index
	return(dwIndex);
}

// PlayAudioSegment()

void PlayAudioSegment(DWORD dwSegIndex,
					  DWORD dwPathIndex,
					  DWORD dwAlign)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwSegIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "PlayAudioSegment()",
								 FALSE);
		return;
	}

	// verify index
	if(dwPathIndex>=g_dwNumAudioPath&&
	   dwPathIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "PlayAudioSegment()",
								 FALSE);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set flags
		DWORD dwFlags=DMUS_SEGF_SECONDARY|
					  dwAlign;

		// set audiopath pointer
		IDirectMusicAudioPath8* lpPath;
		if(dwPathIndex==NO_OBJECT)
			lpPath=NULL;
		else
			lpPath=g_lplpAudioPath[dwPathIndex];
		
		// play segment
		g_lpPerformance->PlaySegmentEx(g_lplpAudioSeg[dwSegIndex],
									   NULL,
									   NULL,
									   dwFlags,
									   0,
									   NULL,
									   NULL,
									   lpPath);
	}
}

// StopAudioSegment()

void StopAudioSegment(DWORD dwIndex,
					  DWORD dwAlign)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "PlayAudioSegment()",
								 FALSE);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// stop segment
		g_lpPerformance->StopEx(g_lplpAudioSeg[dwIndex],
								0,
								dwAlign);
	}
}

// PurgeAudioSegments()

void PurgeAudioSegments(void)
{
	// counter
	DWORD dwCount;
	
	// stop all output
	StopAllAudioSegments();

	// check counter
	if(g_dwNumAudioSeg)
	{
		// unload objects
		if(g_lpAudioLoader)
			for(dwCount=0;dwCount<g_dwNumAudioSeg;dwCount++)
				if(g_lplpAudioSeg[dwCount])
				{
					// unload instrument data
					g_lplpAudioSeg[dwCount]->Unload(g_lpPerformance);

					// release object from loader
					g_lpAudioLoader->ReleaseObjectByUnknown(g_lplpAudioSeg[dwCount]);
				}

		// clear audio cache
		ClearAudioCache();

		// free segment memory & release
		for(dwCount=0;dwCount<g_dwNumAudioSeg;dwCount++)
		{
			// free memory
			FreeMem((LPVOID*)&g_lplpAudioData[dwCount]);
			
			// release interface
			ReleaseCOMPtr(g_lplpAudioSeg[dwCount]);
		}
	}

	// free memory objects
	FreeMem((LPVOID*)&g_lplpAudioData);
	FreeMem((LPVOID*)&g_lplpAudioSeg);
		
	// reset counters
	g_dwNumAudioSeg=0;
	g_dwMaxAudioSeg=0;
}

// PlayPrimaryAudioSegment()

void PlayPrimaryAudioSegment(DWORD dwIndex,
							 DWORD dwAlign)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "PlayPrimaryAudioSegment()",
								 FALSE);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set start point
		g_lplpAudioSeg[dwIndex]->SetStartPoint(0);
		
		// play segment
		g_lpPerformance->PlaySegmentEx(g_lplpAudioSeg[dwIndex],
									   NULL,
									   NULL,
									   dwAlign,
									   0,
									   NULL,
									   NULL,
									   NULL);
	}
}

// QueuePrimaryAudioSegment()

void QueuePrimaryAudioSegment(DWORD dwIndex,
							  DWORD dwAlign)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "QueuePrimaryAudioSegment()",
								 FALSE);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set flags
		DWORD dwFlags=DMUS_SEGF_QUEUE|
					  dwAlign;
		
		// set start point
		g_lplpAudioSeg[dwIndex]->SetStartPoint(0);
		
		// play segment
		g_lpPerformance->PlaySegmentEx(g_lplpAudioSeg[dwIndex],
									   NULL,
									   NULL,
									   dwFlags,
									   0,
									   NULL,
									   NULL,
									   NULL);
	}
}

// PauseAudioOutput()

void PauseAudioOutput(void)
{
	// check init flag
	if(g_bAudioInit)
	{
		// check pause flag
		if(!g_bAudioPaused)
		{
			// capture primary segment
			CapturePrimaryAudioSegment();
			
			// stop all segments
			StopAllAudioSegments();

			// set flag
			g_bAudioPaused=TRUE;
		}
	}
}

// ResumeAudioOutput()

void ResumeAudioOutput(void)
{
	// check init flag
	if(g_bAudioInit)
	{
		// check pause flag
		if(g_bAudioPaused)
		{
			// activate any lost ports
			g_lpDM->Activate(TRUE);

			// activate any lost audiopaths
			ActivateAllAudioPaths();

			// release all managed audiopaths
			ReleaseAllManagedAudioPaths();

			// restore volume settings
			ResetAudioMasterVolume();
			ResetAudioPrimaryVolume();
			ResetAudioSecondaryVolume();

			// restore master setting
			ResetAudioMasterTempo();

			// restore primary segment
			RestorePrimaryAudioSegment();

			// clear pause flag
			g_bAudioPaused=FALSE;
		}
	}
}

// IsAudioSegmentPlaying()

BOOL IsAudioSegmentPlaying(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "IsAudioSegmentPlaying()",
								 FALSE);
		return(FALSE);
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check segment
		if(g_lpPerformance->IsPlaying(g_lplpAudioSeg[dwIndex],
									  NULL)==S_OK)
			return(TRUE);
	}

	// not playing
	return(FALSE);
}

// SetAudioSegmentLength()

void SetAudioSegmentLength(DWORD dwIndex,
						   MUSIC_TIME mtTime)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "SetAudioSegmentLength()",
								 FALSE);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set length
		g_lplpAudioSeg[dwIndex]->SetLength(mtTime);
	}
}

// SetAudioSegmentRepeats()

void SetAudioSegmentRepeats(DWORD dwIndex,
							DWORD dwRepeats)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "SetAudioSegmentRepeats()",
								 FALSE);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set repeats
		g_lplpAudioSeg[dwIndex]->SetRepeats(dwRepeats);
	}
}

// InitDefAudioPath()

BOOL InitDefAudioPath(void)
{
	LoadingOutput("InitDefAudioPath");

	// check interface
	if(g_lpPerformance)
	{
		// get audiopath
		HRESULT hr=g_lpPerformance->GetDefaultAudioPath(&g_lpDefAudioPath);

		// check for error
		if(FAILED(hr))
			return(FALSE);
		else
			AddCOMResource;

		// get default audiopath buffer
		hr=g_lpDefAudioPath->GetObjectInPath(DMUS_PCHANNEL_ALL,
											 DMUS_PATH_BUFFER,
											 0,
											 GUID_NULL,
											 0,
											 IID_IDirectSoundBuffer8,
											 (LPVOID*)&g_lpDefAudioBuf);

		// check for error
		if(FAILED(hr))
			return(FALSE);
		else
			AddCOMResource;
	}

	// ok
	return(TRUE);
}

// ReleaseDefAudioPath()

void ReleaseDefAudioPath(void)
{
	// release pointers
	ReleaseCOMPtr(g_lpDefAudioPath);
	ReleaseCOMPtr(g_lpDefAudioBuf);
}

// SetAudioMasterTempo()

void SetAudioMasterTempo(float fTempo)
{
	// save value
	g_fAudioMasterTempo=fTempo;

	// set tempo
	SetAudioMasterTempoA(fTempo);
}	

// ResetAudioMasterTempo()

void ResetAudioMasterTempo(void)
{
	// set tempo using stored value
	SetAudioMasterTempoA(g_fAudioMasterTempo);
}

// InvalidateAudioData()

void InvalidateAudioData(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// invalidate performance
		g_lpPerformance->Invalidate(0,
									NULL);
	}
}

// BeginAudioExclusiveMode()

void BeginAudioExclusiveMode(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// set cooperative level
		g_lpDS->SetCooperativeLevel(g_hWnd,
									DSSCL_PRIORITY);
		
		// start primary buffer
		StartPrimaryBuffer();
	}
}

// EndAudioExclusiveMode()

void EndAudioExclusiveMode(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// stop primary buffer
		StopPrimaryBuffer();
		
		// set cooperative level
		g_lpDS->SetCooperativeLevel(g_hWnd,
									DSSCL_NORMAL);
	}
}

// GetNextAudioPath()

DWORD GetNextAudioPath(void)
{
	LoadingOutput("GetNextAudioPath");

	// check counter
	if(g_dwNumAudioPath==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpAudioPath,
					 sizeof(IDirectMusicAudioPath8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpAudioBuf3D,
					 sizeof(IDirectSound3DBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpAudioBuf,
					 sizeof(IDirectSoundBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpfAudioPathVol,
					 sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbAudioPath3DPrepared,
					 sizeof(LPBOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbAudioPath3DEnabled,
					 sizeof(LPBOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAudioPath=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumAudioPath>=g_dwMaxAudioPath)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpAudioPath,
					  sizeof(IDirectMusicAudioPath8*)*g_dwMaxAudioPath,
					  sizeof(IDirectMusicAudioPath8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpAudioBuf3D,
					  sizeof(IDirectSound3DBuffer8*)*g_dwMaxAudioPath,
					  sizeof(IDirectSound3DBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpAudioBuf,
					  sizeof(IDirectSoundBuffer8*)*g_dwMaxAudioPath,
					  sizeof(IDirectSoundBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpfAudioPathVol,
					  sizeof(float)*g_dwMaxAudioPath,
					  sizeof(float)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbAudioPath3DPrepared,
					  sizeof(LPBOOL)*g_dwMaxAudioPath,
					  sizeof(LPBOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbAudioPath3DEnabled,
					  sizeof(LPBOOL)*g_dwMaxAudioPath,
					  sizeof(LPBOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAudioPath+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumAudioPath;

	// increment counter
	g_dwNumAudioPath++;

	// return index
	return(dwIndex);
}

// AddAudioPath3D()

DWORD AddAudioPath3D(float fMinDist,
					 float fMaxDist,
					 float fVol)
{
	LoadingOutput("AddAudioPath3D");

	// used for error checking
	HRESULT hr;

	// get return index
	DWORD dwIndex=GetNextAudioPath();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// check flag
	if(g_bAudioInit)
	{
		// create audiopath
		hr=g_lpPerformance->CreateStandardAudioPath(DMUS_APATH_DYNAMIC_3D,
													64,
													TRUE,
													&g_lplpAudioPath[dwIndex]);

		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to create 3-D audio path.",
								  NULL,
								  hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// get 3d buffer interface
		hr=g_lplpAudioPath[dwIndex]->GetObjectInPath(DMUS_PCHANNEL_ALL,
													 DMUS_PATH_BUFFER,
													 0,
													 GUID_NULL,
													 0,
													 IID_IDirectSound3DBuffer8,
													 (LPVOID*)&g_lplpAudioBuf3D[dwIndex]);


		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to retrieve 3-D audio buffer.",
								  NULL,
								  hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// get buffer interface
		hr=g_lplpAudioPath[dwIndex]->GetObjectInPath(DMUS_PCHANNEL_ALL,
													 DMUS_PATH_BUFFER,
													 0,
													 GUID_NULL,
													 0,
													 IID_IDirectSoundBuffer8,
													 (LPVOID*)&g_lplpAudioBuf[dwIndex]);

		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to retrieve standard audio buffer.",
								  NULL,
								  hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// set flags
		g_lpbAudioPath3DPrepared[dwIndex]=TRUE;
		g_lpbAudioPath3DEnabled [dwIndex]=TRUE;
		
		// set min & max distances
		SetAudioPathMinDistance(dwIndex,fMinDist);
		SetAudioPathMaxDistance(dwIndex,fMaxDist);

		// enable 3d effects as needed
		if(g_b3DAudioEnabled)
			EnableAudioPath3DA(dwIndex);
		else
			DisableAudioPath3DA(dwIndex);
		
		// set volume
		SetAudioPathVolume(dwIndex,
						   fVol);
	}

	// return index
	return(dwIndex);
}

// AddAudioPathStereo()

DWORD AddAudioPathStereo(float fVol)
{
	LoadingOutput("AddAudioPathStereo");

	// used for error checking
	HRESULT hr;

	// get return index
	DWORD dwIndex=GetNextAudioPath();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// check flag
	if(g_bAudioInit)
	{
		// create audiopath
		hr=g_lpPerformance->CreateStandardAudioPath(DMUS_APATH_DYNAMIC_STEREO,
													64,
													TRUE,
													&g_lplpAudioPath[dwIndex]);

		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to create stereo audio path.",
								  NULL,
								  hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// reset 3d buffer interface
		g_lplpAudioBuf3D[dwIndex]=NULL;

		// get buffer interface
		hr=g_lplpAudioPath[dwIndex]->GetObjectInPath(DMUS_PCHANNEL_ALL,
													 DMUS_PATH_BUFFER,
													 0,
													 GUID_NULL,
													 0,
													 IID_IDirectSoundBuffer8,
													 (LPVOID*)&g_lplpAudioBuf[dwIndex]);

		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to retrieve standard audio buffer.",
								  NULL,
								  hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// set flag
		g_lpbAudioPath3DPrepared[dwIndex]=FALSE;

		// set volume
		SetAudioPathVolume(dwIndex,
						   fVol);
	}

	// return index
	return(dwIndex);
}

// PurgeAudioPaths()

void PurgeAudioPaths(void)
{
	// counter
	DWORD dwCount;
	
	// check counter
	if(g_dwNumAudioPath)
	{
		// release each audiopath
		for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
		{
			// inactivate audiopath
			InactivateAudioPath(dwCount);
			
			// release interfaces
			ReleaseCOMPtr(g_lplpAudioPath[dwCount]);
			ReleaseCOMPtr(g_lplpAudioBuf3D[dwCount]);
			ReleaseCOMPtr(g_lplpAudioBuf[dwCount]);
		}
	}

	// free memory objects
	FreeMem((LPVOID*)&g_lplpAudioPath);
	FreeMem((LPVOID*)&g_lplpAudioBuf3D);
	FreeMem((LPVOID*)&g_lplpAudioBuf);
	FreeMem((LPVOID*)&g_lpfAudioPathVol);
	FreeMem((LPVOID*)&g_lpbAudioPath3DPrepared);
	FreeMem((LPVOID*)&g_lpbAudioPath3DEnabled);
	
	// reset counters
	g_dwNumAudioPath=0;
	g_dwMaxAudioPath=0;
}

// InitPrimaryBuffer()

BOOL InitPrimaryBuffer(void)
{
	LoadingOutput("InitPrimaryBuffer");

	// check interface
	if(g_lpDefAudioPath)
	{
		// used for error checking
		HRESULT hr;

		// get primary buffer
		hr=g_lpDefAudioPath->GetObjectInPath(0,
											 DMUS_PATH_PRIMARY_BUFFER,
											 0,
											 GUID_NULL,
											 0,
											 IID_IDirectSoundBuffer,
											 (LPVOID*)&g_lpPrimaryBuf);

		// check for error
		if(FAILED(hr))
			return(FALSE);
		else
			AddCOMResource;
	}

	// ok
	return(TRUE);
}

// ReleasePrimaryBuffer()

void ReleasePrimaryBuffer(void)
{
	// release interface
	ReleaseCOMPtr(g_lpPrimaryBuf);
}

// StartPrimaryBuffer()

void StartPrimaryBuffer(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// play primary buffer
		g_lpPrimaryBuf->Play(NULL,
							 NULL,
							 DSBPLAY_LOOPING);
	}
}

// StopPrimaryBuffer()

void StopPrimaryBuffer(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// stop primary buffer
		g_lpPrimaryBuf->Stop();
	}
}

// InitAudioListener()

BOOL InitAudioListener(void)
{
	LoadingOutput("InitAudioListener");

	// check flag
	if(g_bAudioInit)
	{
		// check pointer
		if(!g_lpDefAudioPath)
			return(FALSE);
		
		// used for error checking
		HRESULT hr;

		// get listener interface
		hr=g_lpDefAudioPath->GetObjectInPath(0,
											 DMUS_PATH_PRIMARY_BUFFER,
											 0,
											 GUID_NULL,
											 0,
											 IID_IDirectSound3DListener8,
											 (LPVOID*)&g_lpAudioListener);

		// check for error
		if(FAILED(hr))
			return(FALSE);
		else
			AddCOMResource;
	}

	// ok
	return(TRUE);
}

// ReleaseAudioListener()

void ReleaseAudioListener(void)
{
	// release interface
	ReleaseCOMPtr(g_lpAudioListener);
}

// RenderAudioChanges()

void RenderAudioChanges(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_b3DAudioEnabled)
		{
			// commit settings
			g_lpAudioListener->CommitDeferredSettings();
		}
	}
}

// PurgeAllAudioObjects()

void PurgeAudioObjects(void)
{
	// purge all objects
	PurgeAudioSegments();
	PurgeAudioPaths();
}

// SetAudioMasterTempoA()

void SetAudioMasterTempoA(float fTempo)
{
	// check flag
	if(g_bAudioInit)
	{
		// clip tempo if necessary
		if(fTempo<DMUS_MASTERTEMPO_MIN)
			fTempo=DMUS_MASTERTEMPO_MIN;
		else if(fTempo>DMUS_MASTERTEMPO_MAX)
			fTempo=DMUS_MASTERTEMPO_MAX;

		// set tempo
		g_lpPerformance->SetGlobalParam(GUID_PerfMasterTempo,
										(void*)&fTempo,
										sizeof(float));

		// invalidate audio data
		InvalidateAudioData();
	}
}	

// ActivateAllAudioPaths()

void ActivateAllAudioPaths(void)
{
	// counter
	DWORD dwCount;
	
	// check flag
	if(g_bAudioInit)
	{
		// activate default audiopath
		ActivateAudioPath();

		// check number of audiopaths
		if(g_dwNumAudioPath)
		{
			// activate each audio path
			for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
				ActivateAudioPath(dwCount);
		}
	}
}

// CapturePrimaryAudioSegment()

void CapturePrimaryAudioSegment(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// reset segment position
		g_mtCurPriSegPos=0;

		// get current time
		MUSIC_TIME mtCurTime;
		if(FAILED(g_lpPerformance->GetTime(NULL,
										   &mtCurTime)))
			return;
		
		// capture state of primary segment
		IDirectMusicSegmentState* lpSegState;
		if(FAILED(g_lpPerformance->GetSegmentState(&lpSegState,
												   mtCurTime)))
			return;
		else
			AddCOMResource;

		// get primary segment interface
		IDirectMusicSegment* lpSeg;
		if(FAILED(lpSegState->GetSegment(&lpSeg)))
		{
			ReleaseCOMPtr(lpSegState);
			return;
		}
		else
			AddCOMResource;

		// get updated interface
		if(FAILED(lpSeg->QueryInterface(IID_IDirectMusicSegment8,
										(LPVOID*)&g_lpPrimarySeg)))
		{
			ReleaseCOMPtr(lpSeg);
			ReleaseCOMPtr(lpSegState);
			return;
		}
		else
			AddCOMResource;

		// release old interface
		ReleaseCOMPtr(lpSeg);

		// get segment length
		MUSIC_TIME mtLength;
		if(FAILED(g_lpPrimarySeg->GetLength(&mtLength)))
		{
			ReleaseCOMPtr(lpSegState);
			return;
		}

		// get segment start time
		MUSIC_TIME mtStartTime;
		if(FAILED(lpSegState->GetStartTime(&mtStartTime)))
		{
			ReleaseCOMPtr(lpSegState);
			return;
		}

		// get segment start point
		MUSIC_TIME mtStartPoint;
		if(FAILED(lpSegState->GetStartPoint(&mtStartPoint)))
		{
			ReleaseCOMPtr(lpSegState);
			return;
		}

		// release segment state
		ReleaseCOMPtr(lpSegState);

		// compute segment position
		g_mtCurPriSegPos=(mtCurTime-mtStartTime)+mtStartPoint;

		// account for loops
		while(g_mtCurPriSegPos>mtLength)
			g_mtCurPriSegPos-=mtLength;
	}
}

// RestorePrimaryAudioSegment()

void RestorePrimaryAudioSegment(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// check interface
		if(g_lpPrimarySeg)
		{
			// set segment start point
			g_lpPrimarySeg->SetStartPoint(g_mtCurPriSegPos);
			
			// play primary segment
			g_lpPerformance->PlaySegmentEx(g_lpPrimarySeg,
										   NULL,
										   NULL,
										   NULL,
										   0,
										   NULL,
										   NULL,
										   NULL);
			
			// release primary segment
			ReleaseCOMPtr(g_lpPrimarySeg);
		}
	}
}

// SetListenerDistanceFactor()

void SetListenerDistanceFactor(float fDistFactor)
{
	// check flag
	if(g_bAudioInit)
	{
		// save distance factor
		g_fAudioDistFactor=fDistFactor;

		// set value
		g_lpAudioListener->SetDistanceFactor(fDistFactor,
											 DS3D_IMMEDIATE);
	}
}

// SetListenerOrientation()

void SetListenerOrientation(LPD3DXVECTOR3 lpvAhead,
							LPD3DXVECTOR3 lpvUp)
{
	// check flag
	if(g_bAudioInit)
	{
		// set value
		if(lpvUp)
			g_lpAudioListener->SetOrientation(lpvAhead->x,
											  lpvAhead->y,
											  lpvAhead->z,
											  lpvUp->x,
											  lpvUp->y,
											  lpvUp->z,
											  DS3D_DEFERRED);
		else
			g_lpAudioListener->SetOrientation(lpvAhead->x,
											  lpvAhead->y,
											  lpvAhead->z,
											  0,1,0,
											  DS3D_DEFERRED);
	}
}

// SetListenerOrientation()

void SetListenerOrientation(float fAheadX,
							float fAheadY,
							float fAheadZ,
							float fUpX,
							float fUpY,
							float fUpZ)
{
	// check flag
	if(g_bAudioInit)
	{
		// set value
		g_lpAudioListener->SetOrientation(fAheadX,
										  fAheadY,
										  fAheadZ,
										  fUpX,
										  fUpY,
										  fUpZ,
										  DS3D_DEFERRED);
	}
}

// SetListenerOrientation()

void SetListenerOrientation(DWORD dwMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetListenerOrientation()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set value
		g_lpAudioListener->SetOrientation(g_lpMtx[dwMtxIndex].mtx._31,
										  g_lpMtx[dwMtxIndex].mtx._32,
										  g_lpMtx[dwMtxIndex].mtx._33,
										  g_lpMtx[dwMtxIndex].mtx._21,
										  g_lpMtx[dwMtxIndex].mtx._22,
										  g_lpMtx[dwMtxIndex].mtx._23,
										  DS3D_DEFERRED);
	}
}

// SetListenerPosition()

void SetListenerPosition(LPD3DXVECTOR3 lpvPos)
{
	// check flag
	if(g_bAudioInit)
	{
		// set value
		g_lpAudioListener->SetPosition(lpvPos->x,
									   lpvPos->y,
									   lpvPos->z,
									   DS3D_DEFERRED);
	}
}

// SetListenerPosition()

void SetListenerPosition(float fX,
						 float fY,
						 float fZ)
{
	// check flag
	if(g_bAudioInit)
	{
		// set value
		g_lpAudioListener->SetPosition(fX,
									   fY,
									   fZ,
									   DS3D_DEFERRED);
	}
}

// SetListenerPosition()

void SetListenerPosition(DWORD dwMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetListenerPosition()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set value
		g_lpAudioListener->SetPosition(g_lpMtx[dwMtxIndex].mtx._41,
									   g_lpMtx[dwMtxIndex].mtx._42,
									   g_lpMtx[dwMtxIndex].mtx._43,
									   DS3D_DEFERRED);
	}
}

// SetListenerVelocity()

void SetListenerVelocity(LPD3DXVECTOR3 lpvDir,
						 float fSpeed)
{
	// check flag
	if(g_bAudioInit)
	{
		// update velocity by speed
		float fX=lpvDir->x*fSpeed;
		float fY=lpvDir->y*fSpeed;
		float fZ=lpvDir->z*fSpeed;

		// set value
		g_lpAudioListener->SetVelocity(fX,
									   fY,
									   fZ,
									   DS3D_DEFERRED);
	}
}

// SetListenerVelocity()

void SetListenerVelocity(float fX,
						 float fY,
						 float fZ,
						 float fSpeed)
{
	// check flag
	if(g_bAudioInit)
	{
		// update velocity by speed
		fX*=fSpeed;
		fY*=fSpeed;
		fZ*=fSpeed;

		// set value
		g_lpAudioListener->SetVelocity(fX,
									   fY,
									   fZ,
									   DS3D_DEFERRED);
	}
}

// SetListenerVelocity()

void SetListenerVelocity(DWORD dwMtxIndex,
						 float fSpeed)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetListenerVelocity()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// update velocity by speed
		float fX=g_lpMtx[dwMtxIndex].mtx._31*fSpeed;
		float fY=g_lpMtx[dwMtxIndex].mtx._32*fSpeed;
		float fZ=g_lpMtx[dwMtxIndex].mtx._33*fSpeed;

		// set value
		g_lpAudioListener->SetVelocity(fX,
									   fY,
									   fZ,
									   DS3D_DEFERRED);
	}
}

// Enable3DAudio()

void Enable3DAudio(void)
{
	// counter
	DWORD dwCount;

	// check flag
	if(g_bAudioInit)
	{
		// set flag
		g_b3DAudioEnabled=TRUE;

		// check audiopath counter
		if(g_dwNumAudioPath)
		{
			// enable each audiopath
			for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
				if(g_lpbAudioPath3DPrepared[dwCount])
					if(g_lpbAudioPath3DEnabled[dwCount])
						EnableAudioPath3DA(dwCount);
		}
	}
}

// Disable3DAudio()

void Disable3DAudio(void)
{
	// counter
	DWORD dwCount;

	// check flag
	if(g_bAudioInit)
	{
		// check audiopath counter
		if(g_dwNumAudioPath)
		{
			// disable each audiopath
			for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
				if(g_lpbAudioPath3DPrepared[dwCount])
					DisableAudioPath3DA(dwCount);
		}

		// set flag
		g_b3DAudioEnabled=FALSE;
	}
}

// EnableAudioPath3D()

void EnableAudioPath3D(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "EnableAudioPath3D()",
								 NULL);
		return;
	}
#endif

	// set flag
	g_lpbAudioPath3DEnabled[dwIndex]=TRUE;

	// enable audiopath
	EnableAudioPath3DA(dwIndex);
}

// DisableAudioPath3D()

void DisableAudioPath3D(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "DisableAudioPath3D()",
								 NULL);
		return;
	}
#endif

	// set flag
	g_lpbAudioPath3DEnabled[dwIndex]=FALSE;

	// disable audiopath
	DisableAudioPath3DA(dwIndex);
}

// SetAudioPathMinDistance()

void SetAudioPathMinDistance(DWORD dwIndex,
							 float fDist)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathMinDistance()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwIndex]->SetMinDistance(fDist,
													  DS3D_DEFERRED);
		}
	}
}

// SetAudioPathMaxDistance()

void SetAudioPathMaxDistance(DWORD dwIndex,
							 float fDist)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathMaxDistance()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwIndex]->SetMaxDistance(fDist,
													  DS3D_DEFERRED);
		}
	}
}

// SetAudioPathPosition()

void SetAudioPathPosition(DWORD dwIndex,
						  LPD3DXVECTOR3 lpvPos)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathPosition()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwIndex]->SetPosition(lpvPos->x,
												   lpvPos->y,
												   lpvPos->z,
												   DS3D_DEFERRED);
		}
	}
}

// SetAudioPathPosition()

void SetAudioPathPosition(DWORD dwIndex,
						  float fX,
						  float fY,
						  float fZ)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathPosition()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwIndex]->SetPosition(fX,
												   fY,
												   fZ,
												   DS3D_DEFERRED);
		}
	}
}

// SetAudioPathPosition()

void SetAudioPathPosition(DWORD dwPathIndex,
						  DWORD dwMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwPathIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathPosition()",
								 NULL);
		return;
	}

	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetAudioPathPosition()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwPathIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwPathIndex]->SetPosition(g_lpMtx[dwMtxIndex].mtx._41,
													   g_lpMtx[dwMtxIndex].mtx._42,
													   g_lpMtx[dwMtxIndex].mtx._43,
													   DS3D_DEFERRED);
		}
	}
}

// SetAudioPathOrientation()

void SetAudioPathOrientation(DWORD dwIndex,
							 LPD3DXVECTOR3 lpvAhead)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathOrientation()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwIndex]->SetConeOrientation(lpvAhead->x,
														  lpvAhead->y,
														  lpvAhead->z,
														  DS3D_DEFERRED);
		}
	}
}

// SetAudioPathOrientation()

void SetAudioPathOrientation(DWORD dwIndex,
							 float fX,
							 float fY,
							 float fZ)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathOrientation()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwIndex]->SetConeOrientation(fX,
														  fY,
														  fZ,
														  DS3D_DEFERRED);
		}
	}
}

// SetAudioPathOrientation()

void SetAudioPathOrientation(DWORD dwPathIndex,
							 DWORD dwMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwPathIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathOrientation()",
								 NULL);
		return;
	}

	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetAudioPathOrientation()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwPathIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwPathIndex]->SetConeOrientation(g_lpMtx[dwMtxIndex].mtx._31,
															  g_lpMtx[dwMtxIndex].mtx._32,
															  g_lpMtx[dwMtxIndex].mtx._33,
															  DS3D_DEFERRED);
		}
	}
}

// SetAudioPathProjectionCone()

void SetAudioPathProjectionCone(DWORD dwIndex,
								DWORD dwInnerAng,
								DWORD dwOuterAng,
								float fOuterVol)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathProjectionCone()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set angles
			g_lplpAudioBuf3D[dwIndex]->SetConeAngles(dwInnerAng,
													 dwOuterAng,
													 DS3D_DEFERRED);

			// clip volume if necessary
			if(fOuterVol<0)
				fOuterVol=0;
			else if(fOuterVol>1)
				fOuterVol=1;

			// adjust volume curve
			fOuterVol=(float)sqrt(fOuterVol);

			// compute attenuation
			long lAtt=(long)(((float)(DSBVOLUME_MAX-DSBVOLUME_MIN))*fOuterVol);

			// set outer volume
			g_lplpAudioBuf3D[dwIndex]->SetConeOutsideVolume(lAtt,
															DS3D_DEFERRED);
		}
	}
}

// SetAudioPathVelocity()

void SetAudioPathVelocity(DWORD dwIndex,
						  LPD3DXVECTOR3 lpvDir,
						  float fSpeed)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathVelocity()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// update velocity by speed
			float fX=lpvDir->x*fSpeed;
			float fY=lpvDir->y*fSpeed;
			float fZ=lpvDir->z*fSpeed;

			// set value
			g_lplpAudioBuf3D[dwIndex]->SetVelocity(fX,
												   fY,
												   fZ,
												   DS3D_DEFERRED);
		}
	}
}

// SetAudioPathVelocity()

void SetAudioPathVelocity(DWORD dwIndex,
						  float fX,
						  float fY,
						  float fZ,
						  float fSpeed)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathVelocity()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// update velocity by speed
			fX*=fSpeed;
			fY*=fSpeed;
			fZ*=fSpeed;

			// set value
			g_lplpAudioBuf3D[dwIndex]->SetVelocity(fX,
												   fY,
												   fZ,
												   DS3D_DEFERRED);
		}
	}
}

// SetAudioPathVelocity()

void SetAudioPathVelocity(DWORD dwPathIndex,
						  DWORD dwMtxIndex,
						  float fSpeed)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwPathIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathVelocity()",
								 NULL);
		return;
	}

	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetAudioPathVelocity()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwPathIndex])
		{
			// update velocity by speed
			float fX=g_lpMtx[dwMtxIndex].mtx._31*fSpeed;
			float fY=g_lpMtx[dwMtxIndex].mtx._32*fSpeed;
			float fZ=g_lpMtx[dwMtxIndex].mtx._33*fSpeed;

			// set value
			g_lplpAudioBuf3D[dwPathIndex]->SetVelocity(fX,
													   fY,
													   fZ,
													   DS3D_DEFERRED);
		}
	}
}

// EnableAudioPath3DA()

void EnableAudioPath3DA(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "EnableAudioPath3DA()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_b3DAudioEnabled)
		{
			// check flag
			if(g_lpbAudioPath3DPrepared[dwIndex])
			{
				// enable 3d processing
				g_lplpAudioBuf3D[dwIndex]->SetMode(DS3DMODE_NORMAL,
												   DS3D_IMMEDIATE);
			}
		}
	}
}

// DisableAudioPath3DA()

void DisableAudioPath3DA(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "DisableAudioPath3DA()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_b3DAudioEnabled)
		{
			// check flag
			if(g_lpbAudioPath3DPrepared[dwIndex])
			{
				// enable 3d processing
				g_lplpAudioBuf3D[dwIndex]->SetMode(DS3DMODE_DISABLE,
												   DS3D_IMMEDIATE);
			}
		}
	}
}

// SetAudioPathFXChorus()

void SetAudioPathFXChorus(DWORD dwIndex,
						  float fDepth,
						  float fFeedback,
						  float fFrequency,
						  long lWaveform,
						  float fDelay,
						  long lPhase,
						  float fWetDryMix)
{
	LoadingOutput("SetAudioPathFXChorus");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXChorus()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXChorus8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_CHORUS,
								   IID_IDirectSoundFXChorus8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXChorus fx;
			fx.fWetDryMix=fWetDryMix;
			fx.fDepth=fDepth;
			fx.fFeedback=fFeedback;
			fx.fFrequency=fFrequency;
			fx.lWaveform=lWaveform;
			fx.fDelay=fDelay;
			fx.lPhase=lPhase;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXCompressor()

void SetAudioPathFXCompressor(DWORD dwIndex,
							  float fGain,
							  float fAttack,
							  float fRelease,
							  float fThreshold,
							  float fRatio,
							  float fPredelay)
{
	LoadingOutput("SetAudioPathFXCompressor");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXCompressor()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXCompressor8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_COMPRESSOR,
								   IID_IDirectSoundFXCompressor8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXCompressor fx;
			fx.fGain=fGain;
			fx.fAttack=fAttack;
			fx.fRelease=fRelease;
			fx.fThreshold=fThreshold;
			fx.fRatio=fRatio;
			fx.fPredelay=fPredelay;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXDistortion()

void SetAudioPathFXDistortion(DWORD dwIndex,
							  float fGain,
							  float fEdge,
							  float fPostEQCenterFrequency,
							  float fPostEQBandwidth,
							  float fPreLowpassCutoff)
{
	LoadingOutput("SetAudioPathFXDistortion");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXDistortion()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXDistortion8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_DISTORTION,
								   IID_IDirectSoundFXDistortion8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXDistortion fx;
			fx.fGain=fGain;
			fx.fEdge=fEdge;
			fx.fPostEQCenterFrequency=fPostEQCenterFrequency;
			fx.fPostEQBandwidth=fPostEQBandwidth;
			fx.fPreLowpassCutoff=fPreLowpassCutoff;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXEcho()

void SetAudioPathFXEcho(DWORD dwIndex,
						float fFeedback,
						float fLeftDelay,
						float fRightDelay,
						BOOL bSwapDelays,
						float fWetDryMix)
{
	LoadingOutput("SetAudioPathFXEcho");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXEcho()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXEcho8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_ECHO,
								   IID_IDirectSoundFXEcho8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXEcho fx;
			fx.fWetDryMix=fWetDryMix;
			fx.fFeedback=fFeedback;
			fx.fLeftDelay=fLeftDelay;
			fx.fRightDelay=fRightDelay;
			fx.lPanDelay=(long)bSwapDelays;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXFlanger()

void SetAudioPathFXFlanger(DWORD dwIndex,
						   float fDepth,
						   float fFeedback,
						   float fFrequency,
						   long lWaveform,
						   float fDelay,
						   long lPhase,
						   float fWetDryMix)
{
	LoadingOutput("SetAudioPathFXFlanger");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXFlanger()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXFlanger8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_FLANGER,
								   IID_IDirectSoundFXFlanger8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXFlanger fx;
			fx.fWetDryMix=fWetDryMix;
			fx.fDepth=fDepth;
			fx.fFeedback=fFeedback;
			fx.fFrequency=fFrequency;
			fx.lWaveform=lWaveform;
			fx.fDelay=fDelay;
			fx.lPhase=lPhase;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXGargle()

void SetAudioPathFXGargle(DWORD dwIndex,
						  DWORD dwRateHz,
						  DWORD dwWaveShape)
{
	LoadingOutput("SetAudioPathFXGargle");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXGargle()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXGargle8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_GARGLE,
								   IID_IDirectSoundFXGargle8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXGargle fx;
			fx.dwRateHz=dwRateHz;
			fx.dwWaveShape=dwWaveShape;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXReverb()

void SetAudioPathFXReverb(DWORD dwIndex,
						  DWORD dwType,
						  long lQuality)
{
	LoadingOutput("SetAudioPathFXReverb");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXReverb()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXI3DL2Reverb8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_I3DL2REVERB,
								   IID_IDirectSoundFXI3DL2Reverb8,
								   (LPVOID*)&lpFX))
		{
			// set type
			lpFX->SetPreset(dwType);

			// set quality
			lpFX->SetQuality(lQuality);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXNone()

void SetAudioPathFXNone(DWORD dwIndex)
{
	LoadingOutput("SetAudioPathFXNone");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXNone()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set pointer to buffer
		IDirectSoundBuffer8* lpBuf;
		if(dwIndex==NO_OBJECT)
			lpBuf=g_lpDefAudioBuf;
		else
			lpBuf=g_lplpAudioBuf[dwIndex];

		// disable path
		InactivateAudioPath(dwIndex);

		// set no effects
		lpBuf->SetFX(0,
					 NULL,
					 NULL);

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// ActivateAudioPath()

void ActivateAudioPath(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "ActivateAudioPath()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// enable path as needed
		if(dwIndex==NO_OBJECT)
			g_lpDefAudioPath->Activate(TRUE);
		else
			g_lplpAudioPath[dwIndex]->Activate(TRUE);

		// restore audiopath volume
		ResetAudioPathVolume(NO_OBJECT);
	}
}

// InactivateAudioPath()

void InactivateAudioPath(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "InactivateAudioPath()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path as needed
		if(dwIndex==NO_OBJECT)
			g_lpDefAudioPath->Activate(FALSE);
		else
			g_lplpAudioPath[dwIndex]->Activate(FALSE);
	}
}

// SetAudioPathFXInterface()

BOOL SetAudioPathFXInterface(DWORD dwIndex,
							 REFGUID rguidObject,
							 REFGUID rguidInterface,
							 LPVOID* lplpObject)
{
	LoadingOutput("SetAudioPathFXInterface");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXInterface()",
								 NULL);
		return(FALSE);
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// get pointers to buffer
		IDirectSoundBuffer8* lpBuf;
		if(dwIndex==NO_OBJECT)
			lpBuf=g_lpDefAudioBuf;
		else
			lpBuf=g_lplpAudioBuf[dwIndex];

		// prepare effect description
		DSEFFECTDESC dsed;
		dsed.dwSize=sizeof(DSEFFECTDESC);
		dsed.dwFlags=NULL;
		dsed.guidDSFXClass=rguidObject;
		dsed.dwReserved1=NULL;
		dsed.dwReserved2=NULL;

		// result code
		DWORD dwResult;

		// used for error checking
		HRESULT hr;

		// set effect on buffer
		hr=lpBuf->SetFX(1,
						&dsed,
						&dwResult);

		// check for error
		if(FAILED(hr))
			return(FALSE);

		// get effect interface
		hr=lpBuf->GetObjectInPath(rguidObject,
								  0,
								  rguidInterface,
								  lplpObject);

		// check for error
		if(FAILED(hr))
			return(FALSE);
		else
			AddCOMResource;

		// ok
		return(TRUE);
	}

	// no good
	return(FALSE);
}

#ifdef DEBUG_MODE
// DisplayAudioDeviceSettings()

void DisplayAudioDeviceSettings(void)
{
	// buffer for output
	LPSTR lpOutput;
	
	// allocate buffer for data
	if(!AllocMem((LPVOID*)&lpOutput,
				 1024))
		return;

	LockGlobalMsg()

	// output data
	strcpy(lpOutput,"");
	sprintf(g_lpMsg,"Certified="); strcat(lpOutput,g_lpMsg);
		if(g_lpAudioDevCaps.dwFlags&DSCAPS_CERTIFIED) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CountinuousRate="); strcat(lpOutput,g_lpMsg);
		if(g_lpAudioDevCaps.dwFlags&DSCAPS_CONTINUOUSRATE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"EmulDriver="); strcat(lpOutput,g_lpMsg);
		if(g_lpAudioDevCaps.dwFlags&DSCAPS_EMULDRIVER) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"Primary16Bit="); strcat(lpOutput,g_lpMsg);
		if(g_lpAudioDevCaps.dwFlags&DSCAPS_PRIMARY16BIT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"Primary8Bit="); strcat(lpOutput,g_lpMsg);
		if(g_lpAudioDevCaps.dwFlags&DSCAPS_PRIMARY8BIT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"PrimaryMono="); strcat(lpOutput,g_lpMsg);
		if(g_lpAudioDevCaps.dwFlags&DSCAPS_PRIMARYMONO) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"PrimaryStereo="); strcat(lpOutput,g_lpMsg);
		if(g_lpAudioDevCaps.dwFlags&DSCAPS_PRIMARYSTEREO) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"Secondary16Bit="); strcat(lpOutput,g_lpMsg);
		if(g_lpAudioDevCaps.dwFlags&DSCAPS_SECONDARY16BIT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"Secondary8Bit="); strcat(lpOutput,g_lpMsg);
		if(g_lpAudioDevCaps.dwFlags&DSCAPS_SECONDARY8BIT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SecondaryMono="); strcat(lpOutput,g_lpMsg);
		if(g_lpAudioDevCaps.dwFlags&DSCAPS_SECONDARYMONO) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SecondaryStereo="); strcat(lpOutput,g_lpMsg);
		if(g_lpAudioDevCaps.dwFlags&DSCAPS_SECONDARYSTEREO) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"MinSecondarySampleRate=%d\n",g_lpAudioDevCaps.dwMinSecondarySampleRate); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxSecondarySampleRate=%d\n",g_lpAudioDevCaps.dwMaxSecondarySampleRate); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"PrimaryBuffers=%d\n",g_lpAudioDevCaps.dwPrimaryBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxHwMixingAllBuffers=%d\n",g_lpAudioDevCaps.dwMaxHwMixingAllBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxHwMixingStaticBuffers=%d\n",g_lpAudioDevCaps.dwMaxHwMixingStaticBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxHwMixingStreamingBuffers=%d\n",g_lpAudioDevCaps.dwMaxHwMixingStreamingBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"FreeHwMixingAllBuffers=%d\n",g_lpAudioDevCaps.dwFreeHwMixingAllBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"FreeHwMixingStaticBuffers=%d\n",g_lpAudioDevCaps.dwFreeHwMixingStaticBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"FreeHwMixingStreamingBuffers=%d\n",g_lpAudioDevCaps.dwFreeHwMixingStreamingBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxHw3DAllBuffers=%d\n",g_lpAudioDevCaps.dwMaxHw3DAllBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxHw3DStaticBuffers=%d\n",g_lpAudioDevCaps.dwMaxHw3DStaticBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxHw3DStreamingBuffers=%d\n",g_lpAudioDevCaps.dwMaxHw3DStreamingBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"FreeHw3DAllBuffers=%d\n",g_lpAudioDevCaps.dwFreeHw3DAllBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"FreeHw3DStaticBuffers=%d\n",g_lpAudioDevCaps.dwFreeHw3DStaticBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"FreeHw3DStreamingBuffers=%d\n",g_lpAudioDevCaps.dwFreeHw3DStreamingBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"TotalHwMemBytes=%d\n",g_lpAudioDevCaps.dwTotalHwMemBytes); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"FreeHwMemBytes=%d\n",g_lpAudioDevCaps.dwFreeHwMemBytes); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxContigFreeHwMemBytes=%d\n",g_lpAudioDevCaps.dwMaxContigFreeHwMemBytes); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"UnlockTransferRateHwBuffers=%d\n",g_lpAudioDevCaps.dwUnlockTransferRateHwBuffers); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"PlayCpuOverheadSwBuffers=%d\n",g_lpAudioDevCaps.dwPlayCpuOverheadSwBuffers); strcat(lpOutput,g_lpMsg);

	UnlockGlobalMsg()
	
	// display message
	DisplayInfoMessage(lpOutput);
	
	// free output buffer
	FreeMem((LPVOID*)&lpOutput);
}
#endif

// VerifyAudioDeviceCaps()

BOOL VerifyAudioDeviceCaps(void)
{
	LoadingOutput("VerifyAudioDeviceCaps");

	// check flag
	if(g_bAudioInit)
	{
		// nothing to check; audio init will
		// fail if problems are encountered.
	}

	// ok
	return(TRUE);
}

// SetAudioMasterVolume()

void SetAudioMasterVolume(float fVol,
						  DWORD dwLength)
{
	// save master volume
	g_fAudioMasterVol=fVol;

	// set master volume
	SetAudioMasterVolumeA(dwLength);
}

// SetAudioPrimaryVolume()

void SetAudioPrimaryVolume(float fVol,
						   DWORD dwLength)
{
	// save primary volume
	g_fAudioPrimaryVol=fVol;

	// set primary volume
	SetAudioPrimaryVolumeA(dwLength);
}

// SetAudioSecondaryVolume()

void SetAudioSecondaryVolume(float fVol,
							 DWORD dwLength)
{
	// save secondary volume
	g_fAudioSecondaryVol=fVol;

	// set secondary volume
	SetAudioSecondaryVolumeA(dwLength);
}

// SetAudioPathVolume()

void SetAudioPathVolume(DWORD dwIndex,
						float fVol,
						DWORD dwLength)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathVolume()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// save volume
		if(dwIndex!=NO_OBJECT)
			g_lpfAudioPathVol[dwIndex]=fVol;
		else
			g_fAudioPrimaryVol=fVol;
		
		// set audiopath volume
		SetAudioPathVolumeA(dwIndex,
							fVol,
							dwLength);
	}
}

// ResetAudioMasterVolume()

void ResetAudioMasterVolume(DWORD dwLength)
{
	// set volume using stored value
	SetAudioMasterVolumeA(dwLength);
}

// ResetAudioPrimaryVolume()

void ResetAudioPrimaryVolume(DWORD dwLength)
{
	// set volume using stored value
	SetAudioPrimaryVolumeA(dwLength);
}

// ResetAudioSecondaryVolume()

void ResetAudioSecondaryVolume(DWORD dwLength)
{
	// set volume using stored value
	SetAudioSecondaryVolumeA(dwLength);
}

// ResetAudioPathVolume()

void ResetAudioPathVolume(DWORD dwIndex,
						  DWORD dwLength)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "ResetAudioPathVolume()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// volume
		float fVol;
		
		// get volume of audiopath
		if(dwIndex!=NO_OBJECT)
			fVol=g_lpfAudioPathVol[dwIndex];
		else
			fVol=g_fAudioPrimaryVol;
		
		// set audiopath volume
		SetAudioPathVolumeA(dwIndex,
							fVol,
							dwLength);
	}
}

// SetAudioMasterVolumeA()

void SetAudioMasterVolumeA(DWORD dwLength)
{
	// set primary volume
	SetAudioPrimaryVolumeA(dwLength);

	// set secondary volume
	SetAudioSecondaryVolumeA(dwLength);
}

// SetAudioPrimaryVolumeA()

void SetAudioPrimaryVolumeA(DWORD dwLength)
{
	// check flag
	if(g_bAudioInit)
		SetAudioPathVolume(NO_OBJECT,
						   g_fAudioPrimaryVol,
						   dwLength);
}

// SetAudioSecondaryVolumeA()

void SetAudioSecondaryVolumeA(DWORD dwLength)
{
	// counter
	DWORD dwCount;

	// check flag
	if(g_bAudioInit)
	{
		// check number of audiopaths
		if(g_dwNumAudioPath)
		{
			// set volume of each audio path
			for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
				SetAudioPathVolumeA(dwCount,
					    			g_lpfAudioPathVol[dwCount],
								    dwLength);
		}
	}
}

// SetAudioPathVolumeA()

void SetAudioPathVolumeA(DWORD dwIndex,
						 float fVol,
						 DWORD dwLength)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathVolume()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// pointer to audiopath
		IDirectMusicAudioPath8* lpPath;

		// check path index
		if(dwIndex==NO_OBJECT)
		{
			// set pointer to primary audiopath
			lpPath=g_lpDefAudioPath;

			// adjust volume
			fVol*=g_fAudioMasterVol*
				  ApplicationMasterVolume()*
				  g_fAudioPrimaryVol*
				  ApplicationPrimaryVolume();
		}
		else
		{
			// set pointer to secondary audiopath
			lpPath=g_lplpAudioPath[dwIndex];

			// adjust volume
			fVol*=g_fAudioMasterVol*
				  ApplicationMasterVolume()*
				  g_fAudioSecondaryVol*
				  ApplicationSecondaryVolume();
		}
		
		// clip volume if necessary
		if(fVol<0)
			fVol=0;
		else if(fVol>1)
			fVol=1;

		// adjust volume curve
		fVol=(float)sqrt(fVol);

		// compute attenuation value
		long lAtt=((long)(fVol*9600))-9600;

		// set volume of audiopath
		lpPath->SetVolume(lAtt,
						  dwLength);
	}
}

// InitAudioPathManager()

BOOL InitAudioPathManager(DWORD dwNumPaths,
						  BOOL b3D)
{
	LoadingOutput("InitAudioPathManager");

	// counter
	DWORD dwCount;
	
	// set number of paths
	g_dwNumMAudioPaths=dwNumPaths;

	// create base audiopath
	if(b3D)
		g_dwMAudioPathBaseIdx=AddAudioPath3D();
	else
		g_dwMAudioPathBaseIdx=AddAudioPathStereo();

	// check audiopath
	if(g_dwMAudioPathBaseIdx==NO_OBJECT)
		return(FALSE);

	// create remaining audiopaths
	for(dwCount=1;dwCount<dwNumPaths;dwCount++)
		if(b3D)
		{
			if(!AddAudioPath3D())
				return(FALSE);
		}
		else
		{
			if(!AddAudioPathStereo())
				return(FALSE);
		}

	// create id array
	if(!AllocMem((LPVOID*)&g_lpdwMAudioPathID,
				 sizeof(DWORD)*dwNumPaths))
		return(FALSE);

	// init id array
	for(dwCount=0;dwCount<dwNumPaths;dwCount++)
		g_lpdwMAudioPathID[dwCount]=NO_OBJECT;

	// create priority array
	if(!AllocMem((LPVOID*)&g_lpdwMAudioPathPriority,
				 sizeof(DWORD)*dwNumPaths))
		return(FALSE);

	// init priority array
	for(dwCount=0;dwCount<dwNumPaths;dwCount++)
		g_lpdwMAudioPathPriority[dwCount]=0;

	// ok
	return(TRUE);
}

// CaptureManagedAudioPath()

BOOL CaptureManagedAudioPath(LPDWORD lpdwPathID,
							 DWORD dwPriority)
{
	// counter
	DWORD dwCount;

	// best path data
	DWORD dwBestPathPriority;
	DWORD dwBestPathNum;
	
	// no zero priorities allowed
	dwPriority++;

	// reset best path data
	dwBestPathPriority=0xFFFFFFFF;
	dwBestPathNum=0xFFFFFFFF;

	// scan for best available audiopath
	for(dwCount=0;dwCount<g_dwNumMAudioPaths;dwCount++)
		if(g_lpdwMAudioPathPriority[dwCount]<dwBestPathPriority)
		{
			// save info
			dwBestPathPriority=g_lpdwMAudioPathPriority[dwCount];
			dwBestPathNum=dwCount;
		}

	// check best priority
	if(dwBestPathPriority>dwPriority)
	{
		// set return value
		(*lpdwPathID)=NO_OBJECT;

		// sorry!
		return(FALSE);
	}

	// set return value
	(*lpdwPathID)=g_dwMAudioPathCurID++;

	// copy info
	g_lpdwMAudioPathID[dwBestPathNum]=(*lpdwPathID);
	g_lpdwMAudioPathPriority[dwBestPathNum]=dwPriority;

	// ok
	return(TRUE);
}

// GetManagedAudioPathIndex()

BOOL GetManagedAudioPathIndex(DWORD dwPathID,
							  LPDWORD lpdwPathIndex)
{
	// counter
	DWORD dwCount;

	// scan for id
	for(dwCount=0;dwCount<g_dwNumMAudioPaths;dwCount++)
		if(g_lpdwMAudioPathID[dwCount]==dwPathID)
		{
			// set return value
			(*lpdwPathIndex)=g_dwMAudioPathBaseIdx+dwCount;

			// ok
			return(TRUE);
		}

	// set return value
	(*lpdwPathIndex)=NO_OBJECT;

	// sorry!
	return(FALSE);
}

// ReleaseManagedAudioPath()

void ReleaseManagedAudioPath(DWORD dwPathID)
{
	// counter
	DWORD dwCount;

	// scan for id
	for(dwCount=0;dwCount<g_dwNumMAudioPaths;dwCount++)
		if(g_lpdwMAudioPathID[dwCount]==dwPathID)
		{
			// reset info
			g_lpdwMAudioPathID[dwCount]=NO_OBJECT;
			g_lpdwMAudioPathPriority[dwCount]=0;

			// ok
			return;
		}
}

// ReleaseAudioPathManager()

void ReleaseAudioPathManager(void)
{
	// reset variables
	g_dwNumMAudioPaths=0;
	g_dwMAudioPathCurID=0;
	g_dwMAudioPathBaseIdx=NO_OBJECT;

	// release memory
	FreeMem((LPVOID*)&g_lpdwMAudioPathID);
	FreeMem((LPVOID*)&g_lpdwMAudioPathPriority);
}

// SetAudioPathPan()

void SetAudioPathPan(DWORD dwIndex,
					 float fPan)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathPan()",
								 FALSE);
		return;
	}
	
	// verify pan value
	if(fPan<-1||
	   fPan>1)
	{
		DisplayFatalErrorMessage("Invalid pan value.",
								 "SetAudioPathPan()",
								 FALSE);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// compute pan value
		long lPan=(long)(fPan*10000.0F);

		// set pan value
		g_lplpAudioBuf[dwIndex]->SetPan(lPan);
	}
}

// SimulateAudioPath3D()

void SimulateAudioPath3D(DWORD dwPathIndex,
						 DWORD dwCameraMtxIndex,
						 LPD3DXVECTOR3 lpvCameraAhead,
						 float fTargetX,
						 float fTargetY,
						 float fTargetZ,
						 float fMinDist,
						 float fMaxDist,
						 float fMaxVol)
{
#ifdef DEBUG_MODE
	// verify path index
	if(dwPathIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SimulateAudioPath3D()",
								 FALSE);
		return;
	}
#endif

	// vectors for computations
	D3DXVECTOR3 vV1;
	D3DXVECTOR3 vV2;
	D3DXVECTOR3 vV3;

	// distance value
	float fDist;

	// dot product value
	float fD;

	// volume value
	float fVol;

	// get vector from camera to target
	vV1.x=fTargetX-g_lpMtx[dwCameraMtxIndex].mtx._41;
	vV1.y=fTargetY-g_lpMtx[dwCameraMtxIndex].mtx._42;
	vV1.z=fTargetZ-g_lpMtx[dwCameraMtxIndex].mtx._43;

	// get distance to target
	fDist=Magnitude(&vV1);

	// check distance
	if(fDist>fMaxDist)
	{
		// set volume to 0
		SetAudioPathVolume(dwPathIndex,0);

		// no further processing
		return;
	}

	// reset vector y-value
	vV1.y=0;

	// normalize vector
	Normalize(&vV2,&vV1);

	// get vector perpenduclar to ahead vector
	vV1.x=lpvCameraAhead->z;
	vV1.y=0;
	vV1.z=-lpvCameraAhead->x;
	
	// normalize vector
	Normalize(&vV3,&vV1);

	// compute dot product
	fD=DotProduct(&vV3,&vV2);

	// clip value
	if(fD<-1)
		fD=-1;
	else if(fD>1)
		fD=1;
	
	// set pan value
	SetAudioPathPan(dwPathIndex,fD);
	
	// compute volume value
	fVol=1.0F-((fDist-fMinDist)/(fMinDist*40.5F));

	// clip volume value
	if(fVol<0)
		fVol=0;
	else if(fVol>1)
		fVol=1;
	
	// set volume value
	SetAudioPathVolume(dwPathIndex,fVol*fMaxVol);
}

// SimulateAudioPath3D()

void SimulateAudioPath3D(DWORD dwPathIndex,
						 DWORD dwCameraMtxIndex,
						 LPD3DXVECTOR3 lpvCameraAhead,
						 DWORD dwTargetMtxIndex,
						 float fMinDist,
						 float fMaxDist,
						 float fMaxVol)
{
#ifdef DEBUG_MODE
	// verify path index
	if(dwPathIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SimulateAudioPath3D()",
								 FALSE);
		return;
	}
	
	// verify matrix indices
	if(dwCameraMtxIndex>=g_dwNumMtx||
	   dwTargetMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SimulateAudioPath3D()",
								 FALSE);
		return;
	}
#endif

	// use other function
	SimulateAudioPath3D(dwPathIndex,
						dwCameraMtxIndex,
						lpvCameraAhead,
						g_lpMtx[dwTargetMtxIndex].mtx._41,
						g_lpMtx[dwTargetMtxIndex].mtx._42,
						g_lpMtx[dwTargetMtxIndex].mtx._43,
						fMinDist,
						fMaxDist,
						fMaxVol);
}

// CloneAudioSegment()

DWORD CloneAudioSegment(DWORD dwSrcIndex)
{
	LoadingOutput("CloneAudioSegment");

#ifdef DEBUG_MODE
	// verify index
	if(dwSrcIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "CloneAudioSegment()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// used for error checking
	HRESULT hr;

	// temporary segment
	IDirectMusicSegment* lpDMS=NULL;

	// get return index
	DWORD dwDestIndex=GetNextAudioSegment();

	// check index
	if(dwDestIndex==NO_OBJECT)
		return(NO_OBJECT);

	// check flag
	if(g_bAudioInit)
	{
		// clone segment
		hr=g_lplpAudioSeg[dwSrcIndex]->Clone(NULL,
											 NULL,
											 &lpDMS);

		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to clone audio segment.",
								  NULL,
								  hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// get interface to updated segment
		hr=lpDMS->QueryInterface(IID_IDirectMusicSegment8,
								 (LPVOID*)&g_lplpAudioSeg[dwDestIndex]);

		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to get pointer to updated audio segment.",
								  NULL,
								  hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// release temporary interface
		ReleaseCOMPtr(lpDMS);

		// download any necessary resources
		hr=g_lplpAudioSeg[dwDestIndex]->Download(g_lpPerformance);

		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to download cloned audio resources.",
								  NULL,
								  hr);
			return(NO_OBJECT);
		}
	}

	// return index
	return(dwDestIndex);
}

#ifdef DEBUG_MODE
// OutputAudioPathManagerState()

void OutputAudioPathManagerState(DWORD dwFontIndex,
								 DWORD dwXPos,
								 DWORD dwYPos)
{
	// counter
	DWORD dwCount;

	// output state of each path
	for(dwCount=0;dwCount<g_dwNumMAudioPaths;dwCount++)
	{
		LockGlobalMsg()

		// prepare text
		sprintf(g_lpMsg,
				"path=%d id=%d pri=%d",
				dwCount,
				g_lpdwMAudioPathID[dwCount],
				g_lpdwMAudioPathPriority[dwCount]);

		// output text
		OutputText(g_lpMsg,
				   dwXPos,
				   dwYPos+(dwCount*16));

		UnlockGlobalMsg()
	}
}
#endif

// ReleaseAllManagedAudioPaths()

void ReleaseAllManagedAudioPaths(void)
{
	// counter
	DWORD dwCount;

	// check flag
	if(g_bAudioInit)
	{
		// release all managed audiopaths
		if(g_dwNumMAudioPaths)
			for(dwCount=0;dwCount<g_dwNumMAudioPaths;dwCount++)
			{
				// reset values
				g_lpdwMAudioPathID[dwCount]=NO_OBJECT;
				g_lpdwMAudioPathPriority[dwCount]=0;
			}
	}
}

// AudioPreShutdown()

void AudioPreShutdown(void)
{
	// counter
	DWORD dwCount;

	// check flag
	if(g_bAudioInit)
	{
		// resume if paused
		ResumeAudioOutput();

		// disable all volume
		SetAudioMasterVolume(0);
		SetAudioPrimaryVolume(0);
		SetAudioSecondaryVolume(0);

		// mute primary audiopath
		SetAudioPathVolume(NO_OBJECT,0);

		// must secondary audiopaths
		if(g_dwNumAudioPath)
			for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
				SetAudioPathVolume(dwCount,0);

		// stop all output
		g_lpPerformance->Stop(NULL,
							  NULL,
							  0,
							  0);

		// stop primary buffer
		g_lpPrimaryBuf->Stop();

		// deactivate primary audiopath
		InactivateAudioPath(NO_OBJECT);

		// deactivate secondary audiopaths
		if(g_dwNumAudioPath)
			for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
				InactivateAudioPath(dwCount);

		// deactivate DirectMusic
		g_lpDM->Activate(FALSE);

		// clear loader cache
		g_lpAudioLoader->ClearCache(GUID_DirectMusicAllTypes);
		
		// collect garbage
		g_lpAudioLoader->CollectGarbage();
	}
}

// eof //

// Copyright 2005-2007 WaveBreak Software
