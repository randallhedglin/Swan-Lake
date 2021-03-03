// Copyright 2005-2007 WaveBreak Software

// r3ddxg.cpp -- DirectX Graphics wrapper

// includes //

#include"r3dwin.h"

// initialize globals //

// direct3d object
IDirect3D8* g_lpD3D=NULL;
// direct3d device object
IDirect3DDevice8* g_lpD3DDevice=NULL;
// display adapter id info
D3DADAPTER_IDENTIFIER8 g_lpAdapterID;
// display device capabilities
D3DCAPS8 g_lpDevCaps;
// windowed rendering flag
BOOL g_bWindowedOK=FALSE;
// device presentation parameters
D3DPRESENT_PARAMETERS g_pp;
// list of render modes
RENDERMODES g_rm;
// list of resolutions
RESOLUTIONS g_res;
// client area
RESOLUTION g_rClient;
// billboard flags
BOOL g_bBBEnabled=FALSE;
BOOL g_bVBBEnabled=FALSE;
// vertex processing flag
DWORD g_dwVPF=NULL;
// vertex buffer counters
DWORD g_dwNumVtxBuf=0;
DWORD g_dwMaxVtxBuf=0;
// vertex buffer data
IDirect3DVertexBuffer8** g_lplpVtxBuf=NULL;
LPDWORD g_lpdwFVFSize=NULL;
LPDWORD g_lpdwNumVtx=NULL;
LPDWORD g_lpdwVtxType=NULL;
// index buffer counters
DWORD g_dwNumIdxBuf=0;
DWORD g_dwMaxIdxBuf=0;
// index buffer data
IDirect3DIndexBuffer8** g_lplpIdxBuf=NULL;
LPDWORD g_lpdwPType=NULL;
LPDWORD g_lpdwNumPrim=NULL;
// matrix counters
DWORD g_dwNumMtx=0;
DWORD g_dwMaxMtx=0;
// matrix list
LPSCALEDMATRIX g_lpMtx=NULL;
// current rendering data
DWORD g_dwCurVtxShader=NULL;
DWORD g_dwCurNumVtx=0;
DWORD g_dwCurPType=NULL;
DWORD g_dwCurNumPrim=0;
// material counters
DWORD g_dwNumMat=0;
DWORD g_dwMaxMat=0;
// material data
D3DMATERIAL8* g_lpMat=NULL;
LPSTR* g_lplpMatNames=NULL;
// texture counters
DWORD g_dwNumTex=0;
DWORD g_dwMaxTex=0;
// texture data
IDirect3DTexture8** g_lplpTex=NULL;
LPSTR* g_lplpTexNames=NULL;
// light counters
DWORD g_dwNumLight=0;
DWORD g_dwMaxLight=0;
// light data
D3DLIGHT8* g_lpLight=NULL;
LPBOOL g_lpbLightEnabled=NULL;
#ifdef DEBUG_MODE
LPBOOL g_lpbLightValid=NULL;
#endif
LPSTR* g_lplpLightNames=NULL;
// font counters
DWORD g_dwNumFont=0;
DWORD g_dwMaxFont=0;
// font data
HFONT* g_lphFont=NULL;
// font device context
HDC g_hFontDC=NULL;
// sprite rendering values
DWORD g_dwSpriteVtxBuf=NO_OBJECT;
DWORD g_dwSpriteIdxBuf=NO_OBJECT;
DWORD g_dwSpriteMat=NO_OBJECT;
BOOL g_bSpriteWasInScene=FALSE;
BOOL g_bSpriteWasInFog=FALSE;
BOOL g_bSpriteWasZBuffer=FALSE;
BOOL g_bSpriteWasLinear=FALSE;
// sprite matrices
D3DXMATRIX g_mtxSpriteProj;
DWORD g_dwSpriteMtx=NO_OBJECT;
// identity matrix
D3DXMATRIX g_mtxIdentity=D3DXMATRIX(1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1);
// environment rendering interface
ID3DXRenderToSurface* g_lpRTS=NULL;
// fader vertex & index buffer
DWORD g_dwFaderVtxBuf=NO_OBJECT;
DWORD g_dwFaderIdxBuf=NO_OBJECT;
// z-buffer enabled flag
BOOL g_bZBufferEnabled=TRUE;
// software vertex processing enabled flag
BOOL g_bSVPEnabled=FALSE;
// mesh counters
DWORD g_dwNumMesh=0;
DWORD g_dwMaxMesh=0;
// mesh data
ID3DXMesh** g_lplpMesh=NULL;
LPDWORD g_lpdwNumMeshSubsets=NULL;
LPMESHSUBSETINFO* g_lplpMeshSubsetInfo=NULL;
LPSTR* g_lplpMeshNames=NULL;
// alpha mesh material
DWORD g_dwAlphaMeshMat=NO_OBJECT;
// line vertex & index buffer
#ifdef DEBUG_MODE
DWORD g_dwLineVtxBuf=NO_OBJECT;
DWORD g_dwLineIdxBuf=NO_OBJECT;
#endif
// invalid render values flag
BOOL g_bInvalidRenderValues=TRUE;
// skybox data
DWORD g_dwSkyboxIdxBuf=NO_OBJECT;
DWORD g_dwSkyboxMat=NO_OBJECT;
DWORD g_dwSkyboxMtx=NO_OBJECT;
DWORD g_dwSkyboxTex=NO_OBJECT;
SKYBOXINFO g_sbLeft;
SKYBOXINFO g_sbRight;
SKYBOXINFO g_sbFront;
SKYBOXINFO g_sbBack;
SKYBOXINFO g_sbTop;
SKYBOXINFO g_sbBottom;
float g_fSkyboxRadius=0;
D3DXVECTOR3 g_vSkyboxOffset;
// billboard data
DWORD g_dwBBMtxIndex=NO_OBJECT;
DWORD g_dwVBBMtxIndex=NO_OBJECT;
DWORD g_dwBBVtxBufIdx=NO_OBJECT;
DWORD g_dwBBRevVtxBufIdx=NO_OBJECT;
DWORD g_dwBBIdxBufIdx=NO_OBJECT;
// buffer surfaces
IDirect3DSurface8* g_lpBackBuffer=NULL;
// environment settings
BOOL g_bInScene=FALSE;
BOOL g_bLinearFilter=FALSE;
BOOL g_bWireframe=FALSE;
BOOL g_bGouraud=FALSE;
WORD g_wLPRepeat=NULL;
WORD g_wLPPattern=NULL;
BOOL g_bZBufferWrites=TRUE;
BOOL g_bBackfaceCull=1;
BOOL g_bABlend=FALSE; 
BOOL g_bATest=FALSE;
BOOL g_bFog=FALSE;
D3DCOLOR g_clrFog=NULL;
float g_fFogStart=0;
float g_fFogEnd=0;
BOOL g_bSpecular=FALSE;
DWORD g_dwZBias=0;
D3DCOLOR g_clrAmbient=NULL;
D3DXMATRIX g_mtxProj;
D3DXMATRIX g_mtxView;
VIEWPORT g_vp;
// saved matrices
D3DXMATRIX g_mtxRTSProj;
D3DXMATRIX g_mtxSprProj;
D3DXMATRIX g_mtxSprView;
// current render size
float g_fCurRenderX=0;
float g_fCurRenderY=0;
// saved render sizes
float g_fRTSRenderX=0;
float g_fRTSRenderY=0;
#ifdef USE_TRANSITIONS
// transition data
TRANSTYPE g_ttCurTrans=TT_NULL;
DWORD g_dwTransCurPos=0;
DWORD g_dwTransEndPos=0;
DWORD g_lpdwTransData[MAX_TRANSDATA];
DWORD g_dwTransVtxBuf=NO_OBJECT;
DWORD g_dwTransIdxBuf=NO_OBJECT;
BOOL g_bTransRev=FALSE;
#endif
#ifdef DEBUG_MODE
// shaded sprites flag
BOOL g_bShadedSprites=FALSE;
// line material
DWORD g_dwLineMat=NO_OBJECT;
#endif
// alpha texture flag
BOOL g_bAlphaTextures=FALSE;
// mouse cursor data
BOOL g_bMouseCursor=FALSE;
DWORD g_dwMouseCursorTexIdx=NO_OBJECT;
DWORD g_dwDefMouseCursorTexIdx=NO_OBJECT;
int g_nMouseXHotSpot=0;
int g_nMouseYHotSpot=0;
float g_fMouseXSize=0;
float g_fMouseYSize=0;
#ifdef USE_TRANSITIONS
// shatter data
float* g_lpfShatterVtxTU=NULL;
float* g_lpfShatterVtxTV=NULL;
float* g_lpfShatterCenTU=NULL;
float* g_lpfShatterCenTV=NULL;
LPD3DXVECTOR3 g_lpvShatterVelocity=NULL;
LPDWORD g_lpdwShatterVtx1=NULL;
LPDWORD g_lpdwShatterVtx2=NULL;
LPDWORD g_lpdwShatterVtx3=NULL;
float* g_lpfShatterDeltaYaw=NULL;
float* g_lpfShatterDeltaPitch=NULL;
float* g_lpfShatterDeltaRoll=NULL;
#endif
#ifdef SHOW_LOADINGOUTPUT
// font for loading output
DWORD g_dwLOFont=NO_OBJECT;
#endif
#ifdef USE_LOADINGSCREEN
// loading screen data
BOOL g_bShowLoadingScreen=FALSE;
DWORD g_dwLSLogo=NO_OBJECT;
DWORD g_lpdwLSProgress[4];
DWORD g_lpdwLSPState[10];
#endif

// functions //

// DXGraphicsInit()

BOOL DXGraphicsInit(void)
{
	LoadingOutput("DXGraphicsInit");

	// init direct3d
	if(!InitDirect3D())
		return(FALSE);

	// init d3d device
	if(!InitD3DDevice())
		return(FALSE);

	// init loading screen
	if(!InitLoadingScreen())
		return(FALSE);

	// init dxfile
	if(!InitDXFile())
		return(FALSE);

	// hide windows cursor
	HideWindowsCursor();

	// ok
	return(TRUE);
}

// DXGraphicsRelease()

void DXGraphicsRelease(void)
{
	// purge all objects
	PurgeGraphicsObjects();

	// release interfaces
	ReleaseDXFile();
	ReleaseD3DDevice();
	ReleaseDirect3D();
	
	// show windows cursor
	ShowWindowsCursor();
}

// InitRenderModes()

BOOL InitRenderModes(DWORD dwAdapterNum)
{
	LoadingOutput("InitRenderModes");

	// used for error checking
	HRESULT hr=NULL;
	
	// get number of modes
	g_rm.dwNumModes=g_lpD3D->GetAdapterModeCount(dwAdapterNum);

	// check for error
	if(g_rm.dwNumModes==0)
	{
		DisplayDXErrorMessage("Unable to read display adapter mode count.",
							  g_lpAdapterID.Description);
		return(FALSE);
	}

	// allocate memory for modes
	if(!AllocMem((LPVOID*)&g_rm.lpModes,
				 sizeof(D3DDISPLAYMODE)*g_rm.dwNumModes))
		return(FALSE);

	// read modes
	DWORD dwCount;
	for(dwCount=0;dwCount<g_rm.dwNumModes;dwCount++)
	{
		// get mode
		hr=g_lpD3D->EnumAdapterModes(dwAdapterNum,
									 dwCount,
									 &g_rm.lpModes[dwCount]);
		
		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to read adapter display mode.",
								  g_lpAdapterID.Description,
								  hr);
			return(FALSE);
		}
	}

	// purge modes with formats we don't want
	for(dwCount=0;dwCount<g_rm.dwNumModes;dwCount++)
	{
		// check format
		if(!(IsFormatRGB(g_rm.lpModes[dwCount].Format)))
		{
			// invalidate mode
			g_rm.lpModes[dwCount].Width=0;
		}
	}

	// purge modes that differ only by frequency
	DWORD dwCount2;
	for(dwCount=0;dwCount<g_rm.dwNumModes-1;dwCount++)
		for(dwCount2=dwCount+1;dwCount2<g_rm.dwNumModes;dwCount2++)
		{
			// compare modes
			if(g_rm.lpModes[dwCount].Width==g_rm.lpModes[dwCount2].Width&&
			   g_rm.lpModes[dwCount].Height==g_rm.lpModes[dwCount2].Height&&
			   g_rm.lpModes[dwCount].Format==g_rm.lpModes[dwCount2].Format)
			{
				// invalidate mode
				g_rm.lpModes[dwCount].Width=0;
			}
		}

#ifdef FORCE_ASPECTRATIO
	// purge modes with improper aspect ratio
	for(dwCount=0;dwCount<g_rm.dwNumModes;dwCount++)
	{
		// check aspect ratio
		if(!IsNearEqual((((float)g_rm.lpModes[dwCount].Width)/((float)g_rm.lpModes[dwCount].Height)),
					    ASPECT_RATIO))
		{
			// invalidate mode
			g_rm.lpModes[dwCount].Width=0;
		}
	}
#endif
	
	// count and defragment remaining modes
	dwCount=0;
	for(dwCount2=0;dwCount2<g_rm.dwNumModes;dwCount2++)
	{
		// check mode
		if(g_rm.lpModes[dwCount2].Width!=0)
		{
			// copy mode
			CopyMemory(&g_rm.lpModes[dwCount++],
					   &g_rm.lpModes[dwCount2],
					   sizeof(D3DDISPLAYMODE));
		}
	}

	// check for empty list
	if(dwCount==0)
	{
		DisplayErrorMessage("Unable to find suitable render formats.",
							"Your display adapter does not support a required format.",
							FALSE);
		return(FALSE);
	}
	
	// resize list of modes
	g_rm.dwNumModes=dwCount;

	// compress list
	if(!CompressMem((LPVOID*)&g_rm.lpModes,
				    sizeof(D3DDISPLAYMODE)*g_rm.dwNumModes))
		return(FALSE);

	// sort list into ascending order
	while(TRUE)
	{
		// number of swaps
		DWORD dwSwaps=0;

		// swap flag
		BOOL bSwap;

		// scan through list and swap as needed
		for(dwCount=0;dwCount<g_rm.dwNumModes-1;dwCount++)
		{
			// reset swap flag
			bSwap=FALSE;

			// set swap flag if needed
			if(g_rm.lpModes[dwCount].Width>g_rm.lpModes[dwCount+1].Width)
				bSwap=TRUE;
			else if(g_rm.lpModes[dwCount].Width==g_rm.lpModes[dwCount+1].Width)
				if(g_rm.lpModes[dwCount].Height>g_rm.lpModes[dwCount+1].Height)
					bSwap=TRUE;

			// check swap flag
			if(bSwap)
			{
				// increment counter
				dwSwaps++;

				// swap values
				D3DDISPLAYMODE dm=g_rm.lpModes[dwCount];
				g_rm.lpModes[dwCount]=g_rm.lpModes[dwCount+1];
				g_rm.lpModes[dwCount+1]=dm;
			}
		}

		// check swap counter
		if(dwSwaps==0)
			break;
	}

	// ok
	return(TRUE);
}

// ReleaseRenderModes()

void ReleaseRenderModes(void)
{
	// free mode memory
	FreeMem((LPVOID*)&g_rm.lpModes);

	// reset counter
	g_rm.dwNumModes=0;
}

// IsModeRenderable()

BOOL IsModeRenderable(LPDISPLAYMODE lpdm)
{
	// counter
	DWORD dwCount;

	// check modes
	for(dwCount=0;dwCount<g_rm.dwNumModes;dwCount++)
	{
		// compare width
		if(g_rm.lpModes[dwCount].Width==lpdm->dwX)
		{
			// compare height
			if(g_rm.lpModes[dwCount].Height==lpdm->dwY)
			{
				// compare format
				if(g_rm.lpModes[dwCount].Format==lpdm->fmt)
				{
					// ok
					return(TRUE);
				}
			}
		}
	}

	// not found
	return(FALSE);
}

// SetValidRGBFormat()

BOOL SetValidRGBFormat(LPDISPLAYMODE lpdm)
{
	// check each useful format
	lpdm->fmt=D3DFMT_R5G6B5;
	if(IsModeRenderable(lpdm))
		return(TRUE);
	lpdm->fmt=D3DFMT_X1R5G5B5;
	if(IsModeRenderable(lpdm))
		return(TRUE);
	lpdm->fmt=D3DFMT_X4R4G4B4;
	if(IsModeRenderable(lpdm))
		return(TRUE);
	lpdm->fmt=D3DFMT_X8R8G8B8;
	if(IsModeRenderable(lpdm))
		return(TRUE);

	// none found
	return(FALSE);
}

// InitResolutions()

BOOL InitResolutions(void)
{
	LoadingOutput("InitResolutions");

	// counter
	DWORD dwCount;
	
	// allocate resolutions list
	if(!AllocMem((LPVOID*)&g_res.lpRes,
				 sizeof(RESOLUTION)*g_rm.dwNumModes))
		return(FALSE);

	// copy resolutions
	for(dwCount=0;dwCount<g_rm.dwNumModes;dwCount++)
	{
		// save width & height
		g_res.lpRes[dwCount].dwX=g_rm.lpModes[dwCount].Width;
		g_res.lpRes[dwCount].dwY=g_rm.lpModes[dwCount].Height;
	}

	// purge modes that differ only by format
	DWORD dwCount2;
	for(dwCount=0;dwCount<g_rm.dwNumModes-1;dwCount++)
		for(dwCount2=dwCount+1;dwCount2<g_rm.dwNumModes;dwCount2++)
		{
			// compare formats
			if(g_res.lpRes[dwCount].dwX==g_res.lpRes[dwCount2].dwX&&
			   g_res.lpRes[dwCount].dwY==g_res.lpRes[dwCount2].dwY)
			{
				// invalidate mode
				g_res.lpRes[dwCount].dwX=0;
			}
		}

	// count and defragment remaining modes
	dwCount=0;
	for(dwCount2=0;dwCount2<g_rm.dwNumModes;dwCount2++)
	{
		// check mode
		if(g_res.lpRes[dwCount2].dwX!=0)
		{
			// copy mode
			CopyMemory(&g_res.lpRes[dwCount++],
					   &g_res.lpRes[dwCount2],
					   sizeof(RESOLUTION));
		}
	}

	// check for empty list
	if(dwCount==0)
	{
		DisplayErrorMessage("Unable to find suitable resolutions.",
							"Your display adapter does not support a required format.",
							FALSE);
		return(FALSE);
	}
	
	// resize list of resolutions
	g_res.dwNumRes=dwCount;

	// compress memory
	if(!CompressMem((LPVOID*)&g_res.lpRes,
				     sizeof(RESOLUTION)*g_res.dwNumRes))
		return(FALSE);

	// ok
	return(TRUE);
}

// ReleaseResolutions()

void ReleaseResolutions(void)
{
	// free resolutions memory
	FreeMem((LPVOID*)&g_res.lpRes);

	// reset counter
	g_res.dwNumRes=0;
}

// EnableZBuffer()

void EnableZBuffer(void)
{
	// enable z-buffer
	g_lpD3DDevice->SetRenderState(D3DRS_ZENABLE,
								  D3DZB_TRUE);
	
	// set flag
	g_bZBufferEnabled=TRUE;
}

// DisableZBuffer()

void DisableZBuffer(void)
{
	// disable z-buffer
	g_lpD3DDevice->SetRenderState(D3DRS_ZENABLE,
								  D3DZB_FALSE);
	
	// set flag
	g_bZBufferEnabled=FALSE;
}

// EnableWireframe()

void EnableWireframe(void)
{
	// enable wireframe
	g_lpD3DDevice->SetRenderState(D3DRS_FILLMODE,
								  D3DFILL_WIREFRAME);

	// set flag
	g_bWireframe=TRUE;
}

// DisableWireframe()

void DisableWireframe(void)
{
	// disable wireframe
	g_lpD3DDevice->SetRenderState(D3DRS_FILLMODE,
								  D3DFILL_SOLID);

	// set flag
	g_bWireframe=FALSE;
}

// EnableGouraud()

void EnableGouraud(void)
{
	// enable gouraud
	g_lpD3DDevice->SetRenderState(D3DRS_SHADEMODE,
								  D3DSHADE_GOURAUD);

	// set flag
	g_bGouraud=TRUE;
}

// DisableGouraud()

void DisableGouraud(void)
{
	// disable gouraud
	g_lpD3DDevice->SetRenderState(D3DRS_SHADEMODE,
								  D3DSHADE_FLAT);

	// set flag
	g_bGouraud=FALSE;
}

// SetLinePattern()

void SetLinePattern(WORD wRepeatFactor,
					WORD wLinePattern)
{
	// set up structure
	D3DLINEPATTERN lp;
	lp.wRepeatFactor=wRepeatFactor;
	lp.wLinePattern=wLinePattern;

	// set line pattern
	g_lpD3DDevice->SetRenderState(D3DRS_LINEPATTERN,
								  *((DWORD*)(&lp)));

	// save values
	g_wLPRepeat=wRepeatFactor;
	g_wLPPattern=wLinePattern;
}

// EnableZBufferWrites()

void EnableZBufferWrites(void)
{
	// enable writes
	g_lpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,
								  TRUE);

	// set flag
	g_bZBufferWrites=TRUE;
}

// DisableZBufferWrites()

void DisableZBufferWrites(void)
{
	// disable writes
	g_lpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,
								  FALSE);

	// set flag
	g_bZBufferWrites=FALSE;
}

// EnableBackfaceCull()

void EnableBackfaceCull(void)
{
	// enable cull
	g_lpD3DDevice->SetRenderState(D3DRS_CULLMODE,
								  D3DCULL_CCW);

	// set flag
	g_bBackfaceCull=CULL_BF;
}

// EnableFrontfaceCull()

void EnableFrontfaceCull(void)
{
	// enable cull
	g_lpD3DDevice->SetRenderState(D3DRS_CULLMODE,
								  D3DCULL_CW);

	// set flag
	g_bBackfaceCull=CULL_FF;
}

// DisableBackfaceCull()

void DisableBackfaceCull(void)
{
	// disable cull
	g_lpD3DDevice->SetRenderState(D3DRS_CULLMODE,
								  D3DCULL_NONE);

	// set flag
	g_bBackfaceCull=FALSE;
}

// EnableAlphaBlend()

void EnableAlphaBlend(void)
{
	// enable blend
	g_lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,
								  TRUE);

	// set flag
	g_bABlend=TRUE;
}

// DisableAlphaBlend()

void DisableAlphaBlend(void)
{
	// disable blend
	g_lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,
								  FALSE);

	// set flag
	g_bABlend=FALSE;
}

// EnableAlphaTest()

void EnableAlphaTest(void)
{
	// enable test
	g_lpD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,
								  TRUE);

	// set flag
	g_bATest=TRUE;
}

// DisableAlphaTest()

void DisableAlphaTest(void)
{
	// disable test
	g_lpD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,
								  FALSE);

	// set flag
	g_bATest=FALSE;
}

// EnableFog()

void EnableFog(void)
{
	// enable fog
	g_lpD3DDevice->SetRenderState(D3DRS_FOGENABLE,
								  TRUE);

	// set flag
	g_bFog=TRUE;
}

// DisableFog()

void DisableFog(void)
{
	// disable fog
	g_lpD3DDevice->SetRenderState(D3DRS_FOGENABLE,
								  FALSE);

	// set flag
	g_bFog=FALSE;
}

// EnableSpecular()

void EnableSpecular(void)
{
	// enable specular
	g_lpD3DDevice->SetRenderState(D3DRS_SPECULARENABLE,
								  TRUE);

	// set flag
	g_bSpecular=TRUE;
}

// DisableSpecular()

void DisableSpecular(void)
{
	// disable specular
	g_lpD3DDevice->SetRenderState(D3DRS_SPECULARENABLE,
								  FALSE);

	// set flag
	g_bSpecular=FALSE;
}

// SetFogColor()

void SetFogColor(D3DCOLOR clr)
{
	// set fog color
	g_lpD3DDevice->SetRenderState(D3DRS_FOGCOLOR,
								  (DWORD)clr);

	// save color
	g_clrFog=clr;
}

// SetFogStart()

void SetFogStart(float fVal)
{
	// set fog start
	g_lpD3DDevice->SetRenderState(D3DRS_FOGSTART,
								  *((DWORD*)(&fVal)));

	// save value
	g_fFogStart=fVal;
}

// SetFogEnd()

void SetFogEnd(float fVal)
{
	// set fog end
	g_lpD3DDevice->SetRenderState(D3DRS_FOGEND,
								  *((DWORD*)(&fVal)));

	// save value
	g_fFogEnd=fVal;
}

// SetZBias()

void SetZBias(DWORD dwVal)
{
#ifdef DEBUG_MODE
	// check for overflow
	if(dwVal>16)
	{
		DisplayFatalErrorMessage("Invalid z-bias value.",
								 "SetZBias()",
								 FALSE);
		return;
	}
#endif

	// set z-bias
	g_lpD3DDevice->SetRenderState(D3DRS_ZBIAS,
								  dwVal);

	// save value
	g_dwZBias=dwVal;
}

// SetAmbientLight()

void SetAmbientLight(D3DCOLOR clr)
{
	// set ambient light
	g_lpD3DDevice->SetRenderState(D3DRS_AMBIENT,
								  (DWORD)clr);

	// save value
	g_clrAmbient=clr;
}

// SetViewMatrix()

void SetViewMatrix(LPD3DXMATRIX lpMtx)
{
	// check pointer
	if(lpMtx)
	{
		// save matrix
		CopyMemory(&g_mtxView,
				   lpMtx,
				   sizeof(D3DXMATRIX));
	}
	else
	{
		// save identity matrix
		CopyMemory(&g_mtxView,
				   &g_mtxIdentity,
				   sizeof(D3DXMATRIX));
	}

	// set view matrix
	SetViewMatrixA(lpMtx);
}

// SetProjectionMatrix()

void SetProjectionMatrix(LPD3DXMATRIX lpMtx)
{
	// check pointer
	if(lpMtx)
	{
		// save specified matrix
		CopyMemory(&g_mtxProj,
				   lpMtx,
				   sizeof(D3DXMATRIX));
	}
	else
	{
		// save identity matrix
		CopyMemory(&g_mtxProj,
				   &g_mtxIdentity,
				   sizeof(D3DXMATRIX));
	}

	// set matrix
	SetProjectionMatrixA(lpMtx);
}

// SetWorldMatrix()

void SetWorldMatrix(LPD3DXMATRIX lpMtx)
{
	// check pointer
	if(lpMtx)
	{
		// set specified matrix
		g_lpD3DDevice->SetTransform(D3DTS_WORLD,
									lpMtx);
	}
	else
	{
		// set identity matrix
		g_lpD3DDevice->SetTransform(D3DTS_WORLD,
									&g_mtxIdentity);
	}
}

// SetBillboardMatrix()

void SetBillboardMatrix(LPD3DXMATRIX lpMtx)
{
	// check billboard flag
	if(g_bBBEnabled)
	{
		// set matrix
		SetMatrix(g_dwBBMtxIndex,
				  lpMtx);
	}

	// check vertical billboard flag
	if(g_bVBBEnabled)
	{
		// flatten view vector
		D3DXVECTOR3 vView;
		vView.x=lpMtx->_31;
		vView.y=0;
		vView.z=lpMtx->_33;

		// normalize view vector
		D3DXVECTOR3 vAhead;
		Normalize(&vAhead,
				  &vView);

		// up vector
		static D3DXVECTOR3 vUp=D3DXVECTOR3(0,1,0);

		// right vector
		D3DXVECTOR3 vRight;

		// compute right vector
		CrossProduct(&vRight,
					 &vUp,
					 &vAhead);

		// set matrix
		g_lpMtx[g_dwVBBMtxIndex].mtx._11=vRight.x;
		g_lpMtx[g_dwVBBMtxIndex].mtx._12=vRight.y;
		g_lpMtx[g_dwVBBMtxIndex].mtx._13=vRight.z;
		g_lpMtx[g_dwVBBMtxIndex].mtx._21=vUp.x;
		g_lpMtx[g_dwVBBMtxIndex].mtx._22=vUp.y;
		g_lpMtx[g_dwVBBMtxIndex].mtx._23=vUp.z;
		g_lpMtx[g_dwVBBMtxIndex].mtx._31=vAhead.x;
		g_lpMtx[g_dwVBBMtxIndex].mtx._32=vAhead.y;
		g_lpMtx[g_dwVBBMtxIndex].mtx._33=vAhead.z;
	}
}

// SquareMagnitude()

float SquareMagnitude(D3DXVECTOR3 v)
{
	// return value
	return(Squared(v.x)+
		   Squared(v.y)+
		   Squared(v.z));
}

// SquareMagnitude()

float SquareMagnitude(LPD3DXVECTOR3 lpv)
{
	// return value
	return(Squared(lpv->x)+
		   Squared(lpv->y)+
		   Squared(lpv->z));
}

// SquareMagnitude()

float SquareMagnitudeXZ(LPD3DXVECTOR3 lpv)
{
	// return value
	return(Squared(lpv->x)+
		   Squared(lpv->z));
}

// Magnitude()

float Magnitude(D3DXVECTOR3 v)
{
	// return value
	return(sqrtf(Squared(v.x)+
				 Squared(v.y)+
				 Squared(v.z)));
}

// Magnitude()

float Magnitude(LPD3DXVECTOR3 lpv)
{
	// return value
	return(sqrtf(Squared(lpv->x)+
				 Squared(lpv->y)+
				 Squared(lpv->z)));
}

// Normalize()

D3DXVECTOR3 Normalize(D3DXVECTOR3 v1)
{
#ifdef DEBUG_MODE
	// check magnitude
	if(IsNearZero(Magnitude(v1)))
		DisplayErrorMessage("Undefined normal.",
							NULL,
							FALSE);
#endif

	// compute normal
	return(v1/Magnitude(v1));
}

// Normalize()

void Normalize(LPD3DXVECTOR3 lpv,
			   LPD3DXVECTOR3 lpv1)
{
#ifdef DEBUG_MODE
	// check pointers
	if(lpv==lpv1)
		DisplayErrorMessage("Possible undefined normal.",
							NULL,
							FALSE);

	// check magnitude
	if(IsNearZero(Magnitude(lpv1)))
		DisplayErrorMessage("Undefined normal.",
							NULL,
							FALSE);
#endif
	
	// compute normal
	(*lpv)=(*lpv1)/Magnitude(lpv1);
}

// DotProduct()

float DotProduct(D3DXVECTOR3 v1,
				 D3DXVECTOR3 v2)
{
	// return value
	return((v1.x*v2.x)+
		   (v1.y*v2.y)+
		   (v1.z*v2.z));
}

// DotProduct()

float DotProduct(LPD3DXVECTOR3 lpv1,
				 LPD3DXVECTOR3 lpv2)
{
	// return value
	return((lpv1->x*lpv2->x)+
		   (lpv1->y*lpv2->y)+
		   (lpv1->z*lpv2->z));
}

// CrossProduct()

D3DXVECTOR3 CrossProduct(D3DXVECTOR3 v1,
						 D3DXVECTOR3 v2)
{
	// return value
	D3DXVECTOR3 v3;
	
	// compute cross product
	v3.x=(v1.y*v2.z)-(v1.z*v2.y);
	v3.y=(v1.z*v2.x)-(v1.x*v2.z);
	v3.z=(v1.x*v2.y)-(v1.y*v2.x);

	// return value
	return(v3);
}

// CrossProduct()

void CrossProduct(LPD3DXVECTOR3 lpv,
				  LPD3DXVECTOR3 lpv1,
				  LPD3DXVECTOR3 lpv2)
{
#ifdef DEBUG_MODE
	// check pointers
	if(lpv==lpv1||
	   lpv==lpv2)
		DisplayErrorMessage("Possible undefined cross product.",
							NULL,
							FALSE);
#endif
	
	// compute cross product
	lpv->x=(lpv1->y*lpv2->z)-(lpv1->z*lpv2->y);
	lpv->y=(lpv1->z*lpv2->x)-(lpv1->x*lpv2->z);
	lpv->z=(lpv1->x*lpv2->y)-(lpv1->y*lpv2->x);
}

// GetNextLight()

DWORD GetNextLight(void)
{
	LoadingOutput("GetNextLight");

	// check counter
	if(g_dwNumLight==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpLight,
					 sizeof(D3DLIGHT8)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbLightEnabled,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
#ifdef DEBUG_MODE
		if(!AllocMem((LPVOID*)&g_lpbLightValid,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
#endif
		if(!AllocMem((LPVOID*)&g_lplpLightNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxLight=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumLight>=g_dwMaxLight)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpLight,
					   sizeof(D3DLIGHT8)*g_dwMaxLight,
					   sizeof(D3DLIGHT8)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbLightEnabled,
					   sizeof(BOOL)*g_dwMaxLight,
					   sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
#ifdef DEBUG_MODE
		if(!ExpandMem((LPVOID*)&g_lpbLightValid,
					   sizeof(BOOL)*g_dwMaxLight,
					   sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
#endif
		if(!ExpandMem((LPVOID*)&g_lplpLightNames,
					   sizeof(LPSTR)*g_dwMaxLight,
					   sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxLight+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumLight;

	// increment counter
	g_dwNumLight++;

	// return index
	return(dwIndex);
}

// AddLight()

DWORD AddLight(D3DLIGHT8* lplt,
			   LPSTR lpName)
{
	LoadingOutput("AddLight");

	// error-shown flag
	static BOOL bErrorShown=FALSE;

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()
			
		// create name
		sprintf(g_lpMsg,
				"~LT%d",
				g_dwNumLight);

		// add light
		DWORD dwRet=AddLight(lplt,
							 g_lpMsg);

		// ok
		return(dwRet);
	}

	// get return index
	DWORD dwIndex=GetNextLight();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpLightNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpLightNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// copy data
	g_lpLight[dwIndex]=(*lplt);

	// normalize direction if necessary
	if(IsNearZero(g_lpLight[dwIndex].Direction.x)&&
	   IsNearZero(g_lpLight[dwIndex].Direction.y)&&
	   IsNearZero(g_lpLight[dwIndex].Direction.z))
	{}
	else
		g_lpLight[dwIndex].Direction=Normalize(g_lpLight[dwIndex].Direction);
	
	// set light
	HRESULT hr=g_lpD3DDevice->SetLight(dwIndex,
									   lplt);

	// check for error
	if(FAILED(hr))
	{
		if(!bErrorShown)
		{
			DisplayDXErrorMessage("Unable to set light properties.",
								  "Scene may not display properly.",
								  hr);
			bErrorShown=TRUE;
		}
		return(NO_OBJECT);
	}

	// set enabled flag
	g_lpbLightEnabled[dwIndex]=FALSE;

#ifdef DEBUG_MODE
	// validate light
	g_lpbLightValid[dwIndex]=TRUE;
#endif

	// return index
	return(dwIndex);
}

// UpdateLight()

void UpdateLight(DWORD dwIndex,
				 D3DLIGHT8* lplt)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLight()",
								 FALSE);
		return;
	}
#endif

	// copy data if specified
	if(lplt)
		CopyMemory(&g_lpLight[dwIndex],
				   lplt,
				   sizeof(D3DLIGHT8));

	// update light
	g_lpD3DDevice->SetLight(dwIndex,
							&g_lpLight[dwIndex]);

#ifdef DEBUG_MODE
	// validate light
	g_lpbLightValid[dwIndex]=TRUE;
#endif
}

// EnableLight()

void EnableLight(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "EnableLight()",
								 FALSE);
		return;
	}
#endif
	
	// enable light
	g_lpD3DDevice->LightEnable(dwIndex,
							   TRUE);
	
	// set flag
	g_lpbLightEnabled[dwIndex]=TRUE;
}

// DisableLight()

void DisableLight(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "DisableLight()",
								 FALSE);
		return;
	}
#endif
	
	// disable light
	g_lpD3DDevice->LightEnable(dwIndex,
							   FALSE);

	// set flag
	g_lpbLightEnabled[dwIndex]=FALSE;
}

// PurgeLights()

void PurgeLights(void)
{
	// check counter
	if(g_dwNumLight)
	{
		// counter
		DWORD dwCount;
		
		// disable and free lights
		if(g_dwNumLight)
			for(dwCount=0;dwCount<g_dwNumLight;dwCount++)
			{
				DisableLight(dwCount);
				FreeMem((LPVOID*)&g_lplpLightNames[dwCount]);
			}
	}

	// free memory
	FreeMem((LPVOID*)&g_lpLight);
	FreeMem((LPVOID*)&g_lpbLightEnabled);
#ifdef DEBUG_MODE
	FreeMem((LPVOID*)&g_lpbLightValid);
#endif
	FreeMem((LPVOID*)&g_lplpLightNames);

	// reset counters
	g_dwNumLight=0;
	g_dwMaxLight=0;
}

// EnableSVP()

void EnableSVP(void)
{
	// enable software vertex processing
	g_lpD3DDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING,
								  TRUE);

	// set flag
	g_bSVPEnabled=TRUE;
}

// DisableSVP()

void DisableSVP(void)
{
	// disable software vertex processing
	g_lpD3DDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING,
								  FALSE);

	// reset flag
	g_bSVPEnabled=FALSE;
}

// GetNextVertexBuffer()

DWORD GetNextVertexBuffer(void)
{
	LoadingOutput("GetNextVertexBuffer");

	// check counter
	if(g_dwNumVtxBuf==0)
	{
		// allocate memory		
		if(!AllocMem((LPVOID*)&g_lplpVtxBuf,
					 sizeof(IDirect3DVertexBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwFVFSize,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwNumVtx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwVtxType,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxVtxBuf=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumVtxBuf>=g_dwMaxVtxBuf)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpVtxBuf,
					  sizeof(IDirect3DVertexBuffer8*)*g_dwMaxVtxBuf,
					  sizeof(IDirect3DVertexBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwFVFSize,
					  sizeof(DWORD)*g_dwMaxVtxBuf,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwNumVtx,
					  sizeof(DWORD)*g_dwMaxVtxBuf,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwVtxType,
					  sizeof(DWORD)*g_dwMaxVtxBuf,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxVtxBuf+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumVtxBuf;

	// increment counter
	g_dwNumVtxBuf++;
	
	// return index
	return(dwIndex);
}

// AddVertexBuffer()

DWORD AddVertexBuffer(DWORD dwFVF,
					  DWORD dwFVFSize,
					  DWORD dwNumVtx,
					  BOOL bWriteOnly)
{
	LoadingOutput("AddVertexBuffer");

	// get return index
	DWORD dwIndex=GetNextVertexBuffer();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// used for error checking
	HRESULT hr;

	// set usage flag
	DWORD dwUsage=NULL;
	if(g_dwVPF==VPF_SOFTWARE)
		dwUsage=dwUsage|D3DUSAGE_SOFTWAREPROCESSING;
	if(bWriteOnly)
		dwUsage=dwUsage|D3DUSAGE_WRITEONLY;

	// save rendering values
	g_lpdwFVFSize[dwIndex]=dwFVFSize;
	g_lpdwNumVtx[dwIndex]=dwNumVtx;
	g_lpdwVtxType[dwIndex]=dwFVF;
	
	// create vertex buffer
	hr=g_lpD3DDevice->CreateVertexBuffer(dwFVFSize*dwNumVtx,
										 dwUsage,
										 dwFVF,
										 D3DPOOL_MANAGED,
										 &g_lplpVtxBuf[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create vertex buffer.",
							  "Check available memory.",
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;
										 
	// return index
	return(dwIndex);
}

// LockVertexBuffer()

LPVOID LockVertexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumVtxBuf)
	{
		DisplayFatalErrorMessage("Invalid vertex buffer index.",
								 "LockVertexBuffer()",
								 FALSE);
		return(NULL);
	}
#endif

	// return value
	LPVOID lpRet=NULL;

	// lock buffer
	g_lplpVtxBuf[dwIndex]->Lock(NULL,
								NULL,
								(LPBYTE*)&lpRet,
								NULL);

	// return pointer
	return(lpRet);
}

// UnlockVertexBuffer()

void UnlockVertexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumVtxBuf)
	{
		DisplayFatalErrorMessage("Invalid vertex buffer index.",
								 "UnlockVertexBuffer()",
								 FALSE);
		return;
	}
#endif
	
	// unlock buffer
	g_lplpVtxBuf[dwIndex]->Unlock();
}

// SetVertexBuffer()

void SetVertexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumVtxBuf)
	{
		DisplayFatalErrorMessage("Invalid vertex buffer index.",
								 "SetVertexBuffer()",
								 FALSE);
		return;
	}
#endif
	
	// save necessary values
	g_dwCurNumVtx=g_lpdwNumVtx[dwIndex];

	// set vertex shader
	SetVertexShader(g_lpdwVtxType[dwIndex]);

	// set stream source
	g_lpD3DDevice->SetStreamSource(NULL,
								   g_lplpVtxBuf[dwIndex],
								   g_lpdwFVFSize[dwIndex]);
}

// PurgeVertexBuffers()

void PurgeVertexBuffers(void)
{
	// check counter
	if(g_dwNumVtxBuf)
	{
		// reset stream source
		if(g_lpD3DDevice)
			g_lpD3DDevice->SetStreamSource(NULL,NULL,NULL);

		// counter
		DWORD dwCount;

		// release all vertex buffers
		for(dwCount=0;dwCount<g_dwNumVtxBuf;dwCount++)
			ReleaseCOMPtr(g_lplpVtxBuf[dwCount]);
	}

	// free memory
	FreeMem((LPVOID*)&g_lplpVtxBuf);
	FreeMem((LPVOID*)&g_lpdwFVFSize);
	FreeMem((LPVOID*)&g_lpdwNumVtx);
	FreeMem((LPVOID*)&g_lpdwVtxType);
	
	// reset counters
	g_dwNumVtxBuf=0;
	g_dwMaxVtxBuf=0;
}

// GetNextIndexBuffer()

DWORD GetNextIndexBuffer(void)
{
	LoadingOutput("GetNextIndexBuffer");

	// check counter
	if(g_dwNumIdxBuf==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpIdxBuf,
					 sizeof(IDirect3DIndexBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwPType,
					 sizeof(D3DPRIMITIVETYPE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwNumPrim,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxIdxBuf=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumIdxBuf>=g_dwMaxIdxBuf)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpIdxBuf,
					  sizeof(IDirect3DIndexBuffer8*)*g_dwMaxIdxBuf,
					  sizeof(IDirect3DIndexBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwPType,
					  sizeof(D3DPRIMITIVETYPE)*g_dwMaxIdxBuf,
					  sizeof(D3DPRIMITIVETYPE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwNumPrim,
					  sizeof(DWORD)*g_dwMaxIdxBuf,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxIdxBuf+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumIdxBuf;

	// increment counter
	g_dwNumIdxBuf++;

	// return index
	return(dwIndex);
}

// AddIndexBuffer()

DWORD AddIndexBuffer(DWORD dwPType,
					 DWORD dwNumPrim,
					 BOOL bWriteOnly)
{
	LoadingOutput("AddIndexBuffer");

	// check for invalid number of primitives
	if(dwNumPrim>g_lpDevCaps.MaxPrimitiveCount)
	{
		DisplayErrorMessage("Unable to allocate index buffer.",
							"Your display adapter does not support\na sufficient number of primitives.",
							FALSE);
		return(NO_OBJECT);
	}
	
	// compute number of indices
	DWORD dwNumIdx;
	switch(dwPType)
	{
	case(D3DPT_POINTLIST):
		dwNumIdx=dwNumPrim;
		break;
	case(D3DPT_LINELIST):
		dwNumIdx=dwNumPrim*2;
		break;
	case(D3DPT_LINESTRIP):
		dwNumIdx=dwNumPrim+1;
		break;
	case(D3DPT_TRIANGLELIST):
		dwNumIdx=dwNumPrim*3;
		break;
	case(D3DPT_TRIANGLESTRIP):
		dwNumIdx=dwNumPrim+2;
		break;
	case(D3DPT_TRIANGLEFAN):
		dwNumIdx=dwNumPrim+2;
		break;
#ifdef DEBUG_MODE
	default:
		DisplayFatalErrorMessage("Invalid primitive type.",NULL,FALSE);
		return(NO_OBJECT);
#endif
	}
	
	// get return index
	DWORD dwIndex=GetNextIndexBuffer();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// used for error checking
	HRESULT hr;

	// save rendering values
	g_lpdwPType[dwIndex]=dwPType;
	g_lpdwNumPrim[dwIndex]=dwNumPrim;

	// set usage flag
	DWORD dwUsage=NULL;
	if(bWriteOnly)
		dwUsage=dwUsage|D3DUSAGE_WRITEONLY;
	
	// create index buffer
	hr=g_lpD3DDevice->CreateIndexBuffer(sizeof(WORD)*dwNumIdx,
										dwUsage,
										D3DFMT_INDEX16,
										D3DPOOL_MANAGED,
										&g_lplpIdxBuf[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create index buffer.",
							  "Check available memory.",
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;
										 
	// return index
	return(dwIndex);
}

// LockIndexBuffer()

LPWORD LockIndexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumIdxBuf)
	{
		DisplayFatalErrorMessage("Invalid index buffer index.",
								 "LockIndexBuffer()",
								 FALSE);
		return(NULL);
	}
#endif
	
	// return value
	LPWORD lpRet=NULL;

	// lock buffer
	g_lplpIdxBuf[dwIndex]->Lock(NULL,
								NULL,
								(LPBYTE*)&lpRet,
								NULL);

	// return pointer
	return(lpRet);
}

// UnlockIndexBuffer()

void UnlockIndexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumIdxBuf)
	{
		DisplayFatalErrorMessage("Invalid index buffer index.",
								 "UnlockIndexBuffer()",
								 FALSE);
		return;
	}
#endif
	
	// unlock buffer
	g_lplpIdxBuf[dwIndex]->Unlock();
}

// SetIndexBuffer()

void SetIndexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumIdxBuf)
	{
		DisplayFatalErrorMessage("Invalid index buffer index.",
								 "SetIndexBuffer()",
								 FALSE);
		return;
	}
#endif
	
	// set rendering values
	g_dwCurPType=g_lpdwPType[dwIndex];
	g_dwCurNumPrim=g_lpdwNumPrim[dwIndex];

	// set indices
	g_lpD3DDevice->SetIndices(g_lplpIdxBuf[dwIndex],
							  NULL);
}

// PurgeIndexBuffers()

void PurgeIndexBuffers(void)
{
	// check counter
	if(g_dwNumIdxBuf)
	{
		// reset indices
		if(g_lpD3DDevice)
			g_lpD3DDevice->SetIndices(NULL,NULL);

		// counter
		DWORD dwCount;

		// release all index buffers
		for(dwCount=0;dwCount<g_dwNumIdxBuf;dwCount++)
			ReleaseCOMPtr(g_lplpIdxBuf[dwCount]);
	}

	// free memory
	FreeMem((LPVOID*)&g_lplpIdxBuf);
	FreeMem((LPVOID*)&g_lpdwPType);
	FreeMem((LPVOID*)&g_lpdwNumPrim);
	
	// reset counters
	g_dwNumIdxBuf=0;
	g_dwMaxIdxBuf=0;
}

// GetNextMatrix()

DWORD GetNextMatrix(void)
{
	LoadingOutput("GetNextMatrix");

	// check counter
	if(g_dwNumMtx==0)
	{
		// allocate memory		
		if(!AllocMem((LPVOID*)&g_lpMtx,
					 sizeof(SCALEDMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxMtx=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumMtx>=g_dwMaxMtx)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpMtx,
					  sizeof(SCALEDMATRIX)*g_dwMaxMtx,
					  sizeof(SCALEDMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxMtx+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumMtx;

	// increment counter
	g_dwNumMtx++;

	// return index
	return(dwIndex);
}

// AddMatrix()

DWORD AddMatrix(LPD3DXMATRIX lpmat)
{
	LoadingOutput("AddMatrix");

	// get return index
	DWORD dwIndex=GetNextMatrix();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);
	
	// check pointer
	if(lpmat)
		SetMatrix(dwIndex,
				  lpmat);
	else
		ResetMatrix(dwIndex);
										 
	// return index
	return(dwIndex);
}

// MatrixYaw()

void MatrixYaw(DWORD dwIndex,
			   float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixYaw()",
								 FALSE);
		return;
	}
#endif

	// repeat as needed
	while(fVal>1)
	{
		MatrixYaw(dwIndex,1);
		fVal-=1;
	}
	while(fVal<-1)
	{
		MatrixYaw(dwIndex,-1);
		fVal+=1;
	}
	
	// move ahead vector right
	MtxAheadX+=MtxRightX*fVal;
	MtxAheadY+=MtxRightY*fVal;
	MtxAheadZ+=MtxRightZ*fVal;

	// normalize ahead vector
	Normalize(&MtxAheadX,
			  &MtxAheadY,
			  &MtxAheadZ,
			  MtxAheadX,
			  MtxAheadY,
			  MtxAheadZ);
	
	// compute new right vector
	CrossProduct(&MtxRightX,
				 &MtxRightY,
				 &MtxRightZ,
				 MtxUpX,
				 MtxUpY,
				 MtxUpZ,
				 MtxAheadX,
				 MtxAheadY,
				 MtxAheadZ);
}

// MatrixPitch()

void MatrixPitch(DWORD dwIndex,
			     float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixPitch()",
								 FALSE);
		return;
	}
#endif
	
	// repeat as needed
	while(fVal>1)
	{
		MatrixPitch(dwIndex,1);
		fVal-=1;
	}
	while(fVal<-1)
	{
		MatrixPitch(dwIndex,-1);
		fVal+=1;
	}
	
	// move ahead vector up
	MtxAheadX+=MtxUpX*fVal;
	MtxAheadY+=MtxUpY*fVal;
	MtxAheadZ+=MtxUpZ*fVal;

	// normalize ahead vector
	Normalize(&MtxAheadX,
			  &MtxAheadY,
			  &MtxAheadZ,
			  MtxAheadX,
			  MtxAheadY,
			  MtxAheadZ);
	
	// compute new up vector
	CrossProduct(&MtxUpX,
				 &MtxUpY,
				 &MtxUpZ,
				 MtxAheadX,
				 MtxAheadY,
				 MtxAheadZ,
				 MtxRightX,
				 MtxRightY,
				 MtxRightZ);
}

// MatrixRoll()

void MatrixRoll(DWORD dwIndex,
			    float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixRoll()",
								 FALSE);
		return;
	}
#endif
	
	// repeat as needed
	while(fVal>1)
	{
		MatrixRoll(dwIndex,1);
		fVal-=1;
	}
	while(fVal<-1)
	{
		MatrixRoll(dwIndex,-1);
		fVal+=1;
	}
	
	// move right vector down
	MtxRightX-=MtxUpX*fVal;
	MtxRightY-=MtxUpY*fVal;
	MtxRightZ-=MtxUpZ*fVal;

	// normalize right vector
	Normalize(&MtxRightX,
			  &MtxRightY,
			  &MtxRightZ,
			  MtxRightX,
			  MtxRightY,
			  MtxRightZ);
	
	// compute new up vector
	CrossProduct(&MtxUpX,
				 &MtxUpY,
				 &MtxUpZ,
				 MtxAheadX,
				 MtxAheadY,
				 MtxAheadZ,
				 MtxRightX,
				 MtxRightY,
				 MtxRightZ);
}

// MatrixAhead()

void MatrixAhead(DWORD dwIndex,
			     float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixAhead()",
								 FALSE);
		return;
	}
#endif
	
	// move ahead
	MtxPosX+=MtxAheadX*fVal;
	MtxPosY+=MtxAheadY*fVal;
	MtxPosZ+=MtxAheadZ*fVal;
}

// MatrixBack()

void MatrixBack(DWORD dwIndex,
			    float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixBack()",
								 FALSE);
		return;
	}
#endif
	
	// move back
	MtxPosX-=MtxAheadX*fVal;
	MtxPosY-=MtxAheadY*fVal;
	MtxPosZ-=MtxAheadZ*fVal;
}

// MatrixLeft()

void MatrixLeft(DWORD dwIndex,
			    float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixLeft()",
								 FALSE);
		return;
	}
#endif
	
	// move left
	MtxPosX-=MtxRightX*fVal;
	MtxPosY-=MtxRightY*fVal;
	MtxPosZ-=MtxRightZ*fVal;
}

// MatrixRight()

void MatrixRight(DWORD dwIndex,
			     float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixRight()",
								 FALSE);
		return;
	}
#endif
	
	// move right
	MtxPosX+=MtxRightX*fVal;
	MtxPosY+=MtxRightY*fVal;
	MtxPosZ+=MtxRightZ*fVal;
}

// MatrixUp()

void MatrixUp(DWORD dwIndex,
			  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixUp()",
								 FALSE);
		return;
	}
#endif
	
	// move up
	MtxPosX+=MtxUpX*fVal;
	MtxPosY+=MtxUpY*fVal;
	MtxPosZ+=MtxUpZ*fVal;
}

// MatrixDown()

void MatrixDown(DWORD dwIndex,
			    float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixDown()",
								 FALSE);
		return;
	}
#endif
	
	// move down
	MtxPosX-=MtxUpX*fVal;
	MtxPosY-=MtxUpY*fVal;
	MtxPosZ-=MtxUpZ*fVal;
}

// PurgeMatrices()

void PurgeMatrices(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpMtx);
	
	// reset counters
	g_dwNumMtx=0;
	g_dwMaxMtx=0;
}

// MatrixXScale()

void MatrixXScale(DWORD dwIndex,
				  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixXScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale x
	g_lpMtx[dwIndex].fXScale*=fVal;

	// verify value
	if(g_lpMtx[dwIndex].fXScale<EPSILON)
		g_lpMtx[dwIndex].fXScale=EPSILON;
}

// MatrixYScale()

void MatrixYScale(DWORD dwIndex,
				  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixYScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale y
	g_lpMtx[dwIndex].fYScale*=fVal;

	// verify value
	if(g_lpMtx[dwIndex].fYScale<EPSILON)
		g_lpMtx[dwIndex].fYScale=EPSILON;
}

// MatrixZScale()

void MatrixZScale(DWORD dwIndex,
				  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixZScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale z
	g_lpMtx[dwIndex].fZScale*=fVal;

	// verify value
	if(g_lpMtx[dwIndex].fZScale<EPSILON)
		g_lpMtx[dwIndex].fZScale=EPSILON;
}

// MatrixScale()

void MatrixScale(DWORD dwIndex,
				 float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale xyz
	g_lpMtx[dwIndex].fXScale*=fVal;
	g_lpMtx[dwIndex].fYScale*=fVal;
	g_lpMtx[dwIndex].fZScale*=fVal;
	
	// verify values
	if(g_lpMtx[dwIndex].fXScale<EPSILON)
		g_lpMtx[dwIndex].fXScale=EPSILON;
	if(g_lpMtx[dwIndex].fYScale<EPSILON)
		g_lpMtx[dwIndex].fYScale=EPSILON;
	if(g_lpMtx[dwIndex].fZScale<EPSILON)
		g_lpMtx[dwIndex].fZScale=EPSILON;
}

// SetProjectionMatrix()

void SetProjectionMatrix(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetProjectionMatrix()",
								 FALSE);
		return;
	}
#endif
	
	// get matrix
	D3DXMATRIX mProj;
	GetMatrix(dwIndex,
			  &mProj);
	
	// set matrix
	SetProjectionMatrix(&mProj);
}

// SetWorldMatrix()

void SetWorldMatrix(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetWorldMatrix()",
								 FALSE);
		return;
	}
#endif
	
	// get matrix
	D3DXMATRIX mWorld;
	GetMatrix(dwIndex,
			  &mWorld);

	// set matrix
	g_lpD3DDevice->SetTransform(D3DTS_WORLD,
								&mWorld);
}

// SetViewMatrix()

void SetViewMatrix(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetViewMatrix()",
								 FALSE);
		return;
	}
#endif
	
	// get matrix
	D3DXMATRIX mView;
	GetMatrix(dwIndex,
			  &mView);

	// set view matrix
	SetViewMatrix(&mView);
}

// SquareMagnitude()

float SquareMagnitude(float x,
					  float y,
					  float z)
{
	// return value
	return(Squared(x)+
		   Squared(y)+
		   Squared(z));
}

// SquareMagnitudeXZ()

float SquareMagnitudeXZ(float x,
					    float z)
{
	// return value
	return(Squared(x)+
		   Squared(z));
}

// Magnitude()

float Magnitude(float x,
			    float y,
			    float z)
{
	// return value
	return(sqrtf(Squared(x)+
				 Squared(y)+
				 Squared(z)));
}

// Normalize()

void Normalize(float* lpx,
			   float* lpy,
			   float* lpz,
			   float x1,
			   float y1,
			   float z1)
{
#ifdef DEBUG_MODE
	// check magnitude
	if(IsNearZero(Magnitude(x1,y1,z1)))
		DisplayErrorMessage("Undefined normal.",
							NULL,
							FALSE);
#endif

	// compute distance
	float fDist=1/Magnitude(x1,y1,z1);

	// compute normal
	(*lpx)=x1*fDist;
	(*lpy)=y1*fDist;
	(*lpz)=z1*fDist;
}

// DotProduct()

float DotProduct(float x1,
				 float y1,
				 float z1,
				 float x2,
				 float y2,
				 float z2)
{
	// retrun value
	return((x1*x2)+
		   (y1*y2)+
		   (z1*z2));
}

// CrossProduct()

void CrossProduct(float* lpx,
				  float* lpy,
				  float* lpz,
				  float x1,
				  float y1,
				  float z1,
				  float x2,
				  float y2,
				  float z2)
{
	// return value
	(*lpx)=(y1*z2)-(z1*y2);
	(*lpy)=(z1*x2)-(x1*z2);
	(*lpz)=(x1*y2)-(y1*x2);
}

// BeginScene()

BOOL BeginScene(void)
{
	// begin scene
	HRESULT hr=g_lpD3DDevice->BeginScene();

	// check for error
	if(FAILED(hr))
		return(FALSE);
	
	// set flag
	g_bInScene=TRUE;

	// ok
	return(TRUE);
}

// EndScene()

void EndScene(void)
{
	// end scene
	g_lpD3DDevice->EndScene();

	// set flag
	g_bInScene=FALSE;
}

// PresentScene()

void PresentScene(void)
{
	// validate device
	if(!g_lpD3DDevice)
		return;

	// check closing flag
	if(g_bClosing)
		return;

	// application should render mouse cursor where most appropriate
	/*
	// render mouse cursor
	RenderMouseCursor();
	*/
	
	// present back buffer
	HRESULT hr=g_lpD3DDevice->Present(NULL,NULL,NULL,NULL);

	// check result
	if(FAILED(hr))
	{
		// display error (except lost device)
		if(hr!=D3DERR_DEVICELOST)
			DisplayFatalDXErrorMessage("Unable to present scene.",NULL,hr);
	}

#ifdef DEBUG_MODE
	// check lights
	ValidateLights();
#endif
}

// SetVertexShader()

void SetVertexShader(DWORD dwHandle)
{
	// set vertex shader
	g_lpD3DDevice->SetVertexShader(dwHandle);
}

// RenderPrimitive()

void RenderPrimitive(void)
{
	// render primitive
	g_lpD3DDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)g_dwCurPType,
										0,
										g_dwCurNumVtx,
										0,
										g_dwCurNumPrim);

	// validate render values
	g_bInvalidRenderValues=FALSE;
}

// ClearScreen()

void ClearScreen(D3DCOLOR clr,
				 float fZ)
{
	// clear screen to specified color
	g_lpD3DDevice->Clear(0,
						 NULL,
						 D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
						 clr,
						 fZ,
						 NULL);
}

// ClearZBuffer()

void ClearZBuffer(float fZ)
{
#ifdef DEBUG_MODE
	// clear using random color
	ClearScreen(D3DCOLOR_COLORVALUE(fabsf(GetRandomFloat()),
									fabsf(GetRandomFloat()),
									fabsf(GetRandomFloat()),
									fabsf(GetRandomFloat())));
#else
	// clear z-buffer
	g_lpD3DDevice->Clear(0,
						 NULL,
						 D3DCLEAR_ZBUFFER,
						 NULL,
						 fZ,
						 NULL);
#endif
}

// SetLight()

D3DLIGHT8 SetLight(D3DLIGHTTYPE Type,
				   float DiffuseR,
				   float DiffuseG,
				   float DiffuseB,
				   float DiffuseA,
				   float SpecularR,
				   float SpecularG,
				   float SpecularB,
				   float SpecularA,
				   float AmbientR,
				   float AmbientG,
				   float AmbientB,
				   float AmbientA,
				   float PositionX,
				   float PositionY,
				   float PositionZ,
				   float DirectionX,
				   float DirectionY,
				   float DirectionZ,
				   float Range,
				   float Falloff,
				   float Attenuation0,
				   float Attenuation1,
				   float Attenuation2,
				   float Theta,
				   float Phi)
{
	// return value
	D3DLIGHT8 lt;

	// set values
	lt.Type=Type;
	lt.Diffuse.r=DiffuseR;
	lt.Diffuse.g=DiffuseG;
	lt.Diffuse.b=DiffuseB;
	lt.Diffuse.a=DiffuseA;
	lt.Specular.r=SpecularR;
	lt.Specular.g=SpecularG;
	lt.Specular.b=SpecularB;
	lt.Specular.a=SpecularA;
	lt.Ambient.r=AmbientR;
	lt.Ambient.g=AmbientG;
	lt.Ambient.b=AmbientB;
	lt.Ambient.a=AmbientA;
	lt.Position.x=PositionX;
	lt.Position.y=PositionY;
	lt.Position.z=PositionZ;
	lt.Direction.x=DirectionX;
	lt.Direction.y=DirectionY;
	lt.Direction.z=DirectionZ;
	lt.Range=Range;
	lt.Falloff=Falloff;
	lt.Attenuation0=Attenuation0;
	lt.Attenuation1=Attenuation1;
	lt.Attenuation2=Attenuation2;
	lt.Theta=Theta;
	lt.Phi=Phi;

	// return structure
	return(lt);
}

// SetMaterial()

D3DMATERIAL8 SetMaterial(float DiffuseR,
						 float DiffuseG,
						 float DiffuseB,
						 float DiffuseA,
						 float AmbientR,
						 float AmbientG,
						 float AmbientB,
						 float AmbientA,
						 float SpecularR,
						 float SpecularG,
						 float SpecularB,
						 float SpecularA,
						 float EmissiveR,
						 float EmissiveG,
						 float EmissiveB,
						 float EmissiveA,
						 float Power)
{
	// return value
	D3DMATERIAL8 mat;

	// set up structure
	mat.Diffuse.r=DiffuseR;
	mat.Diffuse.g=DiffuseG;
	mat.Diffuse.b=DiffuseB;
	mat.Diffuse.a=DiffuseA;
	mat.Ambient.r=AmbientR;
	mat.Ambient.g=AmbientG;
	mat.Ambient.b=AmbientB;
	mat.Ambient.a=AmbientA;
	mat.Specular.r=SpecularR;
	mat.Specular.g=SpecularG;
	mat.Specular.b=SpecularB;
	mat.Specular.a=SpecularA;
	mat.Emissive.r=EmissiveR;
	mat.Emissive.g=EmissiveG;
	mat.Emissive.b=EmissiveB;
	mat.Emissive.a=EmissiveA;
	mat.Power=Power;

	// return structure
	return(mat);
}

// CopyRetangle()

void CopyRectangle(IDirect3DSurface8* lpDest,
				   IDirect3DSurface8* lpSource,
				   int nX1Src,
				   int nY1Src,
				   int nX2Src,
				   int nY2Src,
				   int nXDest,
				   int nYDest)
{
	// set up rectangle
	RECT r;
	r.left=nX1Src;
	r.right=nX2Src;
	r.top=nY1Src;
	r.bottom=nY2Src;

	// set up point
	POINT p;
	p.x=nXDest;
	p.y=nYDest;

	// copy rect
	g_lpD3DDevice->CopyRects(lpSource,
							 &r,
							 1,
							 lpDest,
							 &p);
}

// CopyFrontBuffer()

void CopyFrontBuffer(IDirect3DSurface8* lpDest)
{
	// copy buffer to surface
	g_lpD3DDevice->GetFrontBuffer(lpDest);
}

// CopyBackBuffer()

void CopyBackBuffer(IDirect3DSurface8** lplpDest)
{
	// copy buffer to surface
	g_lpD3DDevice->GetBackBuffer(0,
								 D3DBACKBUFFER_TYPE_MONO,
								 lplpDest);
}

// CopyZBuffer()

void CopyZBuffer(IDirect3DSurface8** lplpDest)
{
	// copy buffer to surface
	g_lpD3DDevice->GetDepthStencilSurface(lplpDest);
}

// AddLight()

DWORD AddLight(D3DLIGHTTYPE Type,
			   float DiffuseR,
			   float DiffuseG,
			   float DiffuseB,
			   float DiffuseA,
			   float SpecularR,
			   float SpecularG,
			   float SpecularB,
			   float SpecularA,
			   float AmbientR,
			   float AmbientG,
			   float AmbientB,
			   float AmbientA,
			   float PositionX,
			   float PositionY,
			   float PositionZ,
			   float DirectionX,
			   float DirectionY,
			   float DirectionZ,
			   float Range,
			   float Falloff,
			   float Attenuation0,
			   float Attenuation1,
			   float Attenuation2,
			   float Theta,
			   float Phi,
			   LPSTR lpName)
{
	LoadingOutput("AddLight");

	// set up light structure
	D3DLIGHT8 lt=SetLight(Type,
						  DiffuseR,
						  DiffuseG,
						  DiffuseB,
						  DiffuseA,
						  SpecularR,
						  SpecularG,
						  SpecularB,
						  SpecularA,
						  AmbientR,
						  AmbientG,
						  AmbientB,
						  AmbientA,
						  PositionX,
						  PositionY,
						  PositionZ,
						  DirectionX,
						  DirectionY,
						  DirectionZ,
						  Range,
						  Falloff,
						  Attenuation0,
						  Attenuation1,
						  Attenuation2,
						  Theta,
						  Phi);

	// add light
	return(AddLight(&lt,
		   lpName));
}

// AddMaterial()

DWORD AddMaterial(float DiffuseR,
				  float DiffuseG,
				  float DiffuseB,
				  float DiffuseA,
				  float AmbientR,
				  float AmbientG,
				  float AmbientB,
				  float AmbientA,
				  float SpecularR,
				  float SpecularG,
				  float SpecularB,
				  float SpecularA,
				  float EmissiveR,
				  float EmissiveG,
				  float EmissiveB,
				  float EmissiveA,
				  float Power,
				  LPSTR lpName)
{
	LoadingOutput("AddMaterial");

	// set up material structure
	D3DMATERIAL8 mat=SetMaterial(DiffuseR,
								 DiffuseG,
								 DiffuseB,
								 DiffuseA,
								 AmbientR,
								 AmbientG,
								 AmbientB,
								 AmbientA,
								 SpecularR,
								 SpecularG,
								 SpecularB,
								 SpecularA,
								 EmissiveR,
								 EmissiveG,
								 EmissiveB,
								 EmissiveA,
								 Power);
	
	// add material
	return(AddMaterial(&mat,
		   lpName));
}

// GetNextMaterial()

DWORD GetNextMaterial(void)
{
	LoadingOutput("GetNextMaterial");

	// check counter
	if(g_dwNumMat==0)
	{
		// allocate memory		
		if(!AllocMem((LPVOID*)&g_lpMat,
					 sizeof(D3DMATERIAL8)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpMatNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxMat=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumMat>=g_dwMaxMat)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpMat,
					  sizeof(D3DMATERIAL8)*g_dwMaxMat,
					  sizeof(D3DMATERIAL8)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpMatNames,
					  sizeof(LPSTR)*g_dwMaxMat,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxMat+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumMat;

	// increment counter
	g_dwNumMat++;

	// return index
	return(dwIndex);
}

// AddMaterial()

DWORD AddMaterial(D3DMATERIAL8* lpmat,
				  LPSTR lpName)
{
	LoadingOutput("AddMaterial");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~MT%d",
				g_dwNumMat);

		// add material
		DWORD dwRet=AddMaterial(lpmat,g_lpMsg);

		// ok
		return(dwRet);
	}

	// get return index
	DWORD dwIndex=GetNextMaterial();

	// check material
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMatNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMatNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// copy material
	g_lpMat[dwIndex]=(*lpmat);
										 
	// return index
	return(dwIndex);
}

// UpdateMaterial()

void UpdateMaterial(DWORD dwIndex,
					D3DMATERIAL8* lpmat)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterial()",
								 FALSE);
		return;
	}
#endif
	
	// copy material
	g_lpMat[dwIndex]=(*lpmat);
}

// UpdateMaterialDiffuse()

void UpdateMaterialDiffuse(DWORD dwIndex,
						   float DiffuseR,
						   float DiffuseG,
						   float DiffuseB,
						   float DiffuseA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterialDiffuse()",
								 FALSE);
		return;
	}
#endif
	
	// copy diffuse
	g_lpMat[dwIndex].Diffuse.r=DiffuseR;
	g_lpMat[dwIndex].Diffuse.g=DiffuseG;
	g_lpMat[dwIndex].Diffuse.b=DiffuseB;
	g_lpMat[dwIndex].Diffuse.a=DiffuseA;
}

// UpdateMaterialAmbient()

void UpdateMaterialAmbient(DWORD dwIndex,
						   float AmbientR,
						   float AmbientG,
						   float AmbientB,
						   float AmbientA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterialAmbient()",
								 FALSE);
		return;
	}
#endif
	
	// copy ambient
	g_lpMat[dwIndex].Ambient.r=AmbientR;
	g_lpMat[dwIndex].Ambient.g=AmbientG;
	g_lpMat[dwIndex].Ambient.b=AmbientB;
	g_lpMat[dwIndex].Ambient.a=AmbientA;
}

// UpdateMaterialSpecular()

void UpdateMaterialSpecular(DWORD dwIndex,
							float SpecularR,
							float SpecularG,
							float SpecularB,
							float SpecularA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterialSpecular()",
								 FALSE);
		return;
	}
#endif
	
	// copy specular
	g_lpMat[dwIndex].Specular.r=SpecularR;
	g_lpMat[dwIndex].Specular.g=SpecularG;
	g_lpMat[dwIndex].Specular.b=SpecularB;
	g_lpMat[dwIndex].Specular.a=SpecularA;
}

// UpdateMaterialEmissive()

void UpdateMaterialEmissive(DWORD dwIndex,
							float EmissiveR,
							float EmissiveG,
							float EmissiveB,
							float EmissiveA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterialEmissive()",
								 FALSE);
		return;
	}
#endif
	
	// copy emissive
	g_lpMat[dwIndex].Emissive.r=EmissiveR;
	g_lpMat[dwIndex].Emissive.g=EmissiveG;
	g_lpMat[dwIndex].Emissive.b=EmissiveB;
	g_lpMat[dwIndex].Emissive.a=EmissiveA;
}

// UpdateMaterialPower()

void UpdateMaterialPower(DWORD dwIndex,
						 float Power)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterialPower()",
								 FALSE);
		return;
	}
#endif
	
	// copy power
	g_lpMat[dwIndex].Power=Power;
}

// SetMaterial()

void SetMaterial(DWORD dwIndex)
{
	// default material
	static D3DMATERIAL8 matDef={1,1,1,1, // diffuse
								1,1,1,1, // ambient
								0,0,0,0, // specular
								0,0,0,0, // emissive
								0};		 // power

	// check index
	if(dwIndex==NO_OBJECT)
	{
		// set default material
		g_lpD3DDevice->SetMaterial(&matDef);
	}
	else
	{
#ifdef DEBUG_MODE
		// verify index
		if(dwIndex>=g_dwNumMat)
		{
			DisplayFatalErrorMessage("Invalid material index.",
									 "SetMaterial()",
									 FALSE);
			return;
		}
#endif

		// set specified material
		g_lpD3DDevice->SetMaterial(&g_lpMat[dwIndex]);
	}
}

// PurgeMaterials()

void PurgeMaterials(void)
{
	// check counter
	if(g_dwNumMat)
	{
		// free names
		DWORD dwCount;
		for(dwCount=0;dwCount<g_dwNumMat;dwCount++)
			FreeMem((LPVOID*)&g_lplpMatNames[dwCount]);
	}
		
	// free memory
	FreeMem((LPVOID*)&g_lpMat);
	FreeMem((LPVOID*)&g_lplpMatNames);
	
	// reset counters
	g_dwNumMat=0;
	g_dwMaxMat=0;
}

// UpdateLightType()

void UpdateLightType(DWORD dwIndex,
					 D3DLIGHTTYPE Type)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightType()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Type=Type;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightDiffuse()

void UpdateLightDiffuse(DWORD dwIndex,
						float DiffuseR,
						float DiffuseG,
						float DiffuseB,
						float DiffuseA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightDiffuse()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Diffuse.r=DiffuseR;
	g_lpLight[dwIndex].Diffuse.g=DiffuseG;
	g_lpLight[dwIndex].Diffuse.b=DiffuseB;
	g_lpLight[dwIndex].Diffuse.a=DiffuseA;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightSpecular()

void UpdateLightSpecular(DWORD dwIndex,
						 float SpecularR,
						 float SpecularG,
						 float SpecularB,
						 float SpecularA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightSpecular()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Specular.r=SpecularR;
	g_lpLight[dwIndex].Specular.g=SpecularG;
	g_lpLight[dwIndex].Specular.b=SpecularB;
	g_lpLight[dwIndex].Specular.a=SpecularA;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightAmbient()

void UpdateLightAmbient(DWORD dwIndex,
						float AmbientR,
						float AmbientG,
						float AmbientB,
						float AmbientA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightAmbient()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Ambient.r=AmbientR;
	g_lpLight[dwIndex].Ambient.g=AmbientG;
	g_lpLight[dwIndex].Ambient.b=AmbientB;
	g_lpLight[dwIndex].Ambient.a=AmbientA;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightPosition()

void UpdateLightPosition(DWORD dwIndex,
						 float PositionX,
						 float PositionY,
						 float PositionZ)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightPosition()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Position.x=PositionX;
	g_lpLight[dwIndex].Position.y=PositionY;
	g_lpLight[dwIndex].Position.z=PositionZ;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightDirection()

void UpdateLightDirection(DWORD dwIndex,
						  float DirectionX,
						  float DirectionY,
						  float DirectionZ)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightDirection()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Direction.x=DirectionX;
	g_lpLight[dwIndex].Direction.y=DirectionY;
	g_lpLight[dwIndex].Direction.z=DirectionZ;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightRange()

void UpdateLightRange(DWORD dwIndex,
					  float Range)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightRange()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Range=Range;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightFalloff()

void UpdateLightFalloff(DWORD dwIndex,
						float Falloff)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightFalloff()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Falloff=Falloff;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightAttenuation()

void UpdateLightAttenuation(DWORD dwIndex,
							float Attenuation0,
							float Attenuation1,
							float Attenuation2)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightAttenuation()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Attenuation0=Attenuation0;
	g_lpLight[dwIndex].Attenuation1=Attenuation1;
	g_lpLight[dwIndex].Attenuation2=Attenuation2;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightTheta()

void UpdateLightTheta(DWORD dwIndex,
					  float Theta)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightTheta()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Theta=Theta;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightPhi()

void UpdateLightPhi(DWORD dwIndex,
					float Phi)
{
#ifdef DEBUG_MODE
	// verify light
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightPhi()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Phi=Phi;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// GetNextTexture()

DWORD GetNextTexture(void)
{
	LoadingOutput("GetNextTexture");

	// check counter
	if(g_dwNumTex==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpTex,
					 sizeof(IDirect3DTexture8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpTexNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxTex=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumTex>=g_dwMaxTex)
	{
		// expand memory		
		if(!ExpandMem((LPVOID*)&g_lplpTex,
					  sizeof(IDirect3DTexture8*)*g_dwMaxTex,
					  sizeof(IDirect3DTexture8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpTexNames,
					  sizeof(LPSTR)*g_dwMaxTex,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxTex+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumTex;

	// increment counter
	g_dwNumTex++;

	// return index
	return(dwIndex);
}

// AddTexture()

DWORD AddTexture(LPCSTR lpFile,
				 LPSTR lpName,
				 DWORD dwMipMapLevels)
{
	LoadingOutput("AddTexture");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~TX%d",
				g_dwNumTex);

		// add texture
		DWORD dwRet=AddTexture(lpFile,g_lpMsg,dwMipMapLevels);

		// ok
		return(dwRet);
	}

	// get next texture
	DWORD dwIndex=GetNextTexture();

	// check texture
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpTexNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpTexNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);
	
	// used for error checking
	HRESULT hr;

	// check for data file
	DATAFILEREF dfr=FindDataFileRef((LPSTR)lpFile);

	// set default texture size
	DWORD dwWidth=D3DX_DEFAULT;
	DWORD dwHeight=D3DX_DEFAULT;

	// set maximum texture size
	DWORD dwMaxSize=(g_lpDevCaps.MaxTextureWidth<g_lpDevCaps.MaxTextureHeight)?g_lpDevCaps.MaxTextureWidth:g_lpDevCaps.MaxTextureHeight;

	// get texture info
	D3DXIMAGE_INFO ii;
	if(dfr.lpData)
		hr=D3DXGetImageInfoFromFileInMemory(dfr.lpData,
											dfr.dwSize,
											&ii);
	else
		hr=D3DXGetImageInfoFromFile(lpFile,
									&ii);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to read texture information.",
							  (LPSTR)lpFile,
							  hr);
		return(NO_OBJECT);
	}

	// verify size
	if(ii.Width>dwMaxSize||
	   ii.Height>dwMaxSize)
	{
		// set proper size
		dwWidth=dwMaxSize;
		dwHeight=dwMaxSize;
	}

	// load texture
	if(dfr.lpData)
		hr=D3DXCreateTextureFromFileInMemoryEx(g_lpD3DDevice,
											   dfr.lpData,
											   dfr.dwSize,
											   dwWidth,
											   dwHeight,
											   dwMipMapLevels,
											   NULL,
											   D3DFMT_UNKNOWN,
											   D3DPOOL_MANAGED,
											   D3DX_DEFAULT,
											   D3DX_DEFAULT,
											   NULL,
											   NULL,
											   NULL,
											   &g_lplpTex[dwIndex]);
	else
		hr=D3DXCreateTextureFromFileEx(g_lpD3DDevice,
									   lpFile,
									   dwWidth,
									   dwHeight,
									   dwMipMapLevels,
									   NULL,
									   D3DFMT_UNKNOWN,
									   D3DPOOL_MANAGED,
									   D3DX_DEFAULT,
									   D3DX_DEFAULT,
									   NULL,
									   NULL,
									   NULL,
									   &g_lplpTex[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to load texture.",
							  (LPSTR)lpFile,
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;
	
	// return index
	return(dwIndex);
}

// LockTexture()

D3DLOCKED_RECT LockTexture(DWORD dwIndex)
{
	// return value
	D3DLOCKED_RECT lr;
	InitStruct(lr);

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "LockTexture()",
								 FALSE);
		return(lr);
	}
#endif
	
	// lock texture
	if(FAILED(g_lplpTex[dwIndex]->LockRect(0,
										   &lr,
										   NULL,
										   NULL)))
		lr.pBits=NULL;

	// return locked rect
	return(lr);
}
					   
// UnlockTexture()

void UnlockTexture(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "UnlockTexture()",
								 FALSE);
		return;
	}
#endif
	
	// unlock texture
	g_lplpTex[dwIndex]->UnlockRect(0);
}

// SetTexture()

void SetTexture(DWORD dwIndex)
{
	// previous texture
	static DWORD dwPrevTex=NO_OBJECT-1;
	
	// check index
	if(dwIndex==NO_OBJECT)
	{
		// check previous texture
		if(dwPrevTex!=dwIndex)
		{
			// reset texture
			g_lpD3DDevice->SetTexture(0,
									  NULL);

			// save previous texture
			dwPrevTex=dwIndex;
		}
	}
	else
	{
#ifdef DEBUG_MODE
		// verify index
		if(dwIndex>=g_dwNumTex)
		{
			DisplayFatalErrorMessage("Invalid texture index.",
									 "SetTexture()",
									 FALSE);
			return;
		}
#endif

		// check previous texture
		if(dwPrevTex!=dwIndex)
		{
			// set texture
			g_lpD3DDevice->SetTexture(0,
									  g_lplpTex[dwIndex]);

			// save previous texture
			dwPrevTex=dwIndex;
		}
	}
}

// PurgeTextures()

void PurgeTextures(void)
{
	// check counter
	if(g_dwNumTex)
	{
		// reset texture
		if(g_lpD3DDevice)
			g_lpD3DDevice->SetTexture(0,
									  NULL);

		// counter
		DWORD dwCount;

		// release all textures and names
		for(dwCount=0;dwCount<g_dwNumTex;dwCount++)
		{
			ReleaseCOMPtr(g_lplpTex[dwCount]);
			FreeMem((LPVOID*)&g_lplpTexNames[dwCount]);
		}
	}

	// free memory
	FreeMem((LPVOID*)&g_lplpTex);
	FreeMem((LPVOID*)&g_lplpTexNames);
	
	// reset counters
	g_dwNumTex=0;
	g_dwMaxTex=0;
	
#ifdef USE_LOADINGSCREEN
	// reset loading screen texture
	g_dwLSLogo=NO_OBJECT;
#endif
}

// EnableLinearFilter()

void EnableLinearFilter(void)
{
	// set magnification
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MAGFILTER,
										D3DTEXF_LINEAR);

	// set minification
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MINFILTER,
										D3DTEXF_LINEAR);

	// set mipmap
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MIPFILTER,
										D3DTEXF_LINEAR);

	// set flag
	g_bLinearFilter=TRUE;
}

// DisableLinearFilter()

void DisableLinearFilter(void)
{
	// set magnification
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MAGFILTER,
										D3DTEXF_POINT);
	
	// set minification
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MINFILTER,
										D3DTEXF_POINT);

	// set mipmap
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MIPFILTER,
										D3DTEXF_POINT);

	// set flag
	g_bLinearFilter=FALSE;
}

#ifdef DEBUG_MODE
// CreateDDSFromImage()

void CreateDDSFromImage(LPCSTR lpDestFile,
						LPCSTR lpSrcFile,
						BOOL bTransparent,
						D3DCOLOR clrTransparent,
						float fAlpha)
{
	LoadingOutput("CreateDDSFromImage");

	// pointer to texture
	IDirect3DTexture8* lpTex=NULL;
	
	// used for error checking
	HRESULT hr;

	// set format
	D3DFORMAT fmt;
	if(fAlpha<1)
		fmt=D3DFMT_A4R4G4B4;
	else
	{
		if(bTransparent)
			fmt=D3DFMT_A1R5G5B5;
		else
			fmt=D3DFMT_R5G6B5;
	}

	// load texture
	hr=D3DXCreateTextureFromFileEx(g_lpD3DDevice,
								   lpSrcFile,
								   D3DX_DEFAULT,
								   D3DX_DEFAULT,
								   1,
								   NULL,
								   fmt,
								   D3DPOOL_MANAGED,
								   D3DX_DEFAULT,
								   D3DX_DEFAULT,
								   NULL,
								   NULL,
								   NULL,
								   &lpTex);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to load texture.",
								   (LPSTR)lpSrcFile,
								   hr);
		return;
	}
	else
		AddCOMResource;

	// get surface description
	D3DSURFACE_DESC sd;
	hr=lpTex->GetLevelDesc(0,&sd);
	
	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to read texture description.",
							  (LPSTR)lpSrcFile,
							  hr);
		return;
	}

	// check format
	if(sd.Format!=fmt)
	{
		DisplayErrorMessage("Unable to load texture into proper format.",
							(LPSTR)lpSrcFile,
							FALSE);
		return;
	}

	// add alpha
	AddAlphaToTexture(lpTex,
					  bTransparent,
					  clrTransparent,
					  fAlpha);

	// output to DDS file
	hr=D3DXSaveTextureToFile(lpDestFile,
							 D3DXIFF_DDS,
							 lpTex,
							 NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to output texture.",
								   (LPSTR)lpDestFile,
								   hr);
		return;
	}

	// release pointer
	ReleaseCOMPtr(lpTex);
}
#endif

// InitSprites()

BOOL InitSprites(void)
{
	LoadingOutput("InitSprites");

	// add material
	g_dwSpriteMat=AddMaterial(0,0,0,1,
							  0,0,0,1,
							  0,0,0,1,
							  1,1,1,1,
							  0);

	// check material
	if(g_dwSpriteMat==NO_OBJECT)
		return(FALSE);
	
	// add vertex buffer
	g_dwSpriteVtxBuf=AddVertexBuffer(D3DFVF_SPRITEVERTEX,
									 sizeof(SPRITEVERTEX),
									 4);

	// check vertex buffer
	if(g_dwSpriteVtxBuf==NO_OBJECT)
		return(FALSE);

	// lock vertex buffer	
	LPSPRITEVERTEX lpVtx=(LPSPRITEVERTEX)LockVertexBuffer(g_dwSpriteVtxBuf);

	// check for error
	if(!lpVtx)
		return(FALSE);

	// fill vertex buffer
	SET_SPRITEVERTEX(lpVtx[0],
					 -0.5F,-0.5F,0,
					 0,1);
	SET_SPRITEVERTEX(lpVtx[1],
					 0.5F,-0.5F,0,
					 1,1);
	SET_SPRITEVERTEX(lpVtx[2],
					 -0.5F,0.5F,0,
					 0,0);
	SET_SPRITEVERTEX(lpVtx[3],
					 0.5F,0.5F,0,
					 1,0);
	
	// unlock vertex buffer
	UnlockVertexBuffer(g_dwSpriteVtxBuf);

	// add index buffer
	g_dwSpriteIdxBuf=AddIndexBuffer(D3DPT_TRIANGLEFAN,
									2);

	// check index buffer
	if(g_dwSpriteIdxBuf==NO_OBJECT)
		return(FALSE);

	// lock index buffer
	LPWORD lpIdx=LockIndexBuffer(g_dwSpriteIdxBuf);

	// check for error
	if(!lpIdx)
		return(FALSE);

	// fill index buffer
	lpIdx[0]=1;
	lpIdx[1]=0;
	lpIdx[2]=2;
	lpIdx[3]=3;
	
	// unlock index buffer
	UnlockIndexBuffer(g_dwSpriteIdxBuf);

	// create sprite matrix
	g_dwSpriteMtx=AddMatrix(NULL);

	// check matrix
	if(g_dwSpriteMtx==NO_OBJECT)
		return(FALSE);

	// ok
	return(TRUE);
}

// BeginSprites()

BOOL BeginSprites(BOOL bLinear)
{
	// save projection matrix
	CopyMemory(&g_mtxSprProj,
			   &g_mtxProj,
			   sizeof(D3DXMATRIX));
	
	// save view matrix
	CopyMemory(&g_mtxSprView,
			   &g_mtxView,
			   sizeof(D3DXMATRIX));
	
	// reset sprite projection matrix
	ResetSpriteProjectionMatrix();
	
	// set sprite projection matrix
	SetProjectionMatrixA(&g_mtxSpriteProj);

	// set view matrix to identity
	SetViewMatrixA();

	// set vertex & index buffers
	SetVertexBuffer(g_dwSpriteVtxBuf);
	SetIndexBuffer(g_dwSpriteIdxBuf);

	// set sprite material
	SetMaterial(g_dwSpriteMat);

	// check fog
	if(g_bFog)
	{
		// disable fog		
		DisableFog();

		// set flag
		g_bSpriteWasInFog=TRUE;
	}
	else
	{
		// set flag
		g_bSpriteWasInFog=FALSE;
	}
	
	// check scene
	if(g_bInScene)
	{
		// set flag
		g_bSpriteWasInScene=TRUE;
	}
	else
	{
		// set flag
		g_bSpriteWasInScene=FALSE;

		// begin scene
		if(!BeginScene())
			return(FALSE);
	}
	
	// check z-buffer
	if(g_bZBufferEnabled)
	{
		// disable z-buffer
		DisableZBuffer();

		// set flag
		g_bSpriteWasZBuffer=TRUE;
	}
	else
	{
		// set flag
		g_bSpriteWasZBuffer=FALSE;
	}

	// check linear
	if(g_bLinearFilter)
	{
		// disable linear textures
		DisableLinearFilter();

		// set flag
		g_bSpriteWasLinear=TRUE;
	}
	else
	{
		// set flag
		g_bSpriteWasLinear=FALSE;
	}
	
	// ok
	return(TRUE);
}

// OutputSprite()

void OutputSprite(DWORD dwTexIndex,
				  float fX,
				  float fY,
				  float fXSize,
				  float fYSize,
				  float fRot)
{
#ifdef DEBUG_MODE
	// check flag
	/*if(g_bShadedSprites)
		DisplayInfoMessage("Unshaded sprite in shaded mode!");*/
#endif

	// update values based on render size
	fX*=g_fCurRenderX;
	fY*=g_fCurRenderY;
	fXSize*=g_fCurRenderX;
	fYSize*=g_fCurRenderY;
	
	// reset sprite matrix
	ResetMatrix(g_dwSpriteMtx);

	// translate matrix
	MatrixRight(g_dwSpriteMtx,
				fX);
	MatrixUp(g_dwSpriteMtx,
			 fY);

	// rotate matrix
	if(fRot)
		MatrixRoll(g_dwSpriteMtx,
				   fRot);

	// scale matrix
	MatrixXScale(g_dwSpriteMtx,
				 fXSize);
	MatrixYScale(g_dwSpriteMtx,
				 fYSize);

	// set world matrix
	SetWorldMatrix(g_dwSpriteMtx);

	// set texture
	SetTexture(dwTexIndex);

	// render texture
	RenderPrimitive();
}

// EndSprites()

void EndSprites(void)
{
	// end scene if necessary
	if(!g_bSpriteWasInScene)
		EndScene();

	// enable fog if necessary
	if(g_bSpriteWasInFog)
		EnableFog();
	
	// enable z-buffer if necessary
	if(g_bSpriteWasZBuffer)
		EnableZBuffer();
	
	// enable linear textures if necessary
	if(g_bSpriteWasLinear)
		EnableLinearFilter();
	
	// restore projection & view matrices
	SetProjectionMatrixA(&g_mtxSprProj);
	SetViewMatrixA(&g_mtxSprView);
}

// ResetMatrix()

void ResetMatrix(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "ResetMatrix()",
								 FALSE);
		return;
	}
#endif
	
	// reset matrix to identity
	CopyMemory(&g_lpMtx[dwIndex],
			   &g_mtxIdentity,
			   sizeof(D3DXMATRIX));

	// reset scale values
	g_lpMtx[dwIndex].fXScale=1;
	g_lpMtx[dwIndex].fYScale=1;
	g_lpMtx[dwIndex].fZScale=1;
}

// ResetMatrix()

void ResetMatrix(LPD3DXMATRIX lpMtx)
{
	// reset matrix to identity
	CopyMemory(lpMtx,
			   &g_mtxIdentity,
			   sizeof(D3DXMATRIX));
}

// Get2DLineIntersection()
BOOL Get2DLineIntersection(float* lpfXI,
						   float* lpfYI,
						   float fX1,
						   float fY1,
						   float fX2,
						   float fY2,
						   float fX3,
						   float fY3,
						   float fX4,
						   float fY4)
{
	// compute delta values
	float fDX=fX2-fX1;
	float fDY=fY2-fY1;

	// compute denominator value
	float fD=((fY4-fY3)*fDX)-((fX4-fX3)*fDY);
	
	// check value
	if(fD==0)
	{
		// lines are parallel
		return(FALSE);
	}
	else
	{
		// compute projection value
		float fU=(((fX4-fX3)*(fY1-fY3))-((fY4-fY3)*(fX1-fX3)))/fD;

		// compute intersection
		(*lpfXI)=fX1+(fU*fDX);
		(*lpfYI)=fY1+(fU*fDY);
	}

	// ok
	return(TRUE);
}

// AddTexture()

DWORD AddTexture(DWORD dwWidth,
				 DWORD dwHeight,
				 BOOL bTransparent,
				 BOOL bTranslucent,
				 LPSTR lpName,
				 DWORD dwMipMapLevels)
{
	LoadingOutput("AddTexture");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~TX%d",
				g_dwNumTex);

		// add texture
		DWORD dwRet=AddTexture(dwWidth,
							   dwHeight,
							   bTransparent,
							   bTranslucent,
							   g_lpMsg,
							   dwMipMapLevels);

		// ok
		return(dwRet);
	}

	// get next texture index
	DWORD dwIndex=GetNextTexture();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpTexNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpTexNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// set format
	D3DFORMAT fmt;
	if(bTranslucent)
		fmt=D3DFMT_A4R4G4B4;
	else
	{
		if(bTransparent)
			fmt=D3DFMT_A1R5G5B5;
		else
			fmt=D3DFMT_R5G6B5;
	}

	// clip size if necessary
	if(dwWidth>g_lpDevCaps.MaxTextureWidth)
		dwWidth=g_lpDevCaps.MaxTextureWidth;
	if(dwHeight>g_lpDevCaps.MaxTextureHeight)
		dwHeight=g_lpDevCaps.MaxTextureHeight;

	// used for error checking
	HRESULT hr;

	// create texture
	hr=g_lpD3DDevice->CreateTexture(dwWidth,
									dwHeight,
									dwMipMapLevels,
									NULL,
									fmt,
									D3DPOOL_MANAGED,
									&g_lplpTex[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create texture.",
							  "Check available memory.",
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// return index
	return(dwIndex);
}

// BeginRenderToTexture()

BOOL BeginRenderToTexture(DWORD dwIndex,
						  float fClipNear,
						  float fClipFar,
						  float fAngle)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "BeginRenderToTexture()",
								 FALSE);
		return(FALSE);
	}
#endif
	
	// save projection matrix
	CopyMemory(&g_mtxRTSProj,
			   &g_mtxProj,
			   sizeof(D3DXMATRIX));
	
	// set projection matrix
	SetProjectionMatrixA(fClipNear,
						 fClipFar,
						 fAngle,
						 1);
	
	// get surface description
	D3DSURFACE_DESC sd;
	HRESULT hr=g_lplpTex[dwIndex]->GetLevelDesc(0,&sd);
	
	// check for error
	if(FAILED(hr))
		return(FALSE);

	// save render size
	g_fRTSRenderX=g_fCurRenderX;
	g_fRTSRenderY=g_fCurRenderY;

	// set new render size
	g_fCurRenderX=(float)sd.Width;
	g_fCurRenderY=(float)sd.Height;
	
	// create surface mapper
	hr=D3DXCreateRenderToSurface(g_lpD3DDevice,
								 sd.Width,
								 sd.Height,
								 sd.Format,
								 TRUE,
								 D3DFMT_D16,
								 &g_lpRTS);
	
	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;

	// get texture surface
	IDirect3DSurface8* lpSurf;
	hr=g_lplpTex[dwIndex]->GetSurfaceLevel(0,
										   &lpSurf);
	
	// check for error
	if(FAILED(hr))
	{
		ReleaseCOMPtr(g_lpRTS);
		return(FALSE);
	}
	else
		AddCOMResource;

	// prepare viewport
	D3DVIEWPORT8 vp;
	vp.X=0;
	vp.Y=0;
	vp.Width=sd.Width;
	vp.Height=sd.Height;
	vp.MinZ=0;
	vp.MaxZ=1;
	
	// begin scene
	hr=g_lpRTS->BeginScene(lpSurf,
						   &vp);

	// release surface
	ReleaseCOMPtr(lpSurf);

	// check for error
	if(FAILED(hr))
	{
		ReleaseCOMPtr(g_lpRTS);
		return(FALSE);
	}

	// set scene flag
	g_bInScene=TRUE;
	
	// ok
	return(TRUE);
}

// EndRenderToTexture()

void EndRenderToTexture(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "EndRenderToTexture()",
								 FALSE);
		return;
	}
#endif
	
	// end scene
	if(g_lpRTS)
		g_lpRTS->EndScene();

	// release surface mapper
	ReleaseCOMPtr(g_lpRTS);

	// restore projection matrix
	SetProjectionMatrixA(&g_mtxRTSProj);

	// restore render size
	g_fCurRenderX=g_fRTSRenderX;
	g_fCurRenderY=g_fRTSRenderY;

	// reset scene flag
	g_bInScene=FALSE;
}

#ifdef DEBUG_MODE
// CreateDDSFromTexture()

void CreateDDSFromTexture(LPCSTR lpDestFile,
						  DWORD dwIndex)
{
	LoadingOutput("CreateDDSFromTexture");

	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "CreateDDSFromTexture()",
								 FALSE);
		return;
	}
	
	// used for error checking
	HRESULT hr;

	// output to DDS file
	hr=D3DXSaveTextureToFile(lpDestFile,
							 D3DXIFF_DDS,
							 g_lplpTex[dwIndex],
							 NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to output texture.",
								   (LPSTR)lpDestFile,
								   hr);
		return;
	}
}
#endif

// AddAlphaToTexture()

BOOL AddAlphaToTexture(DWORD dwIndex,
					   BOOL bTransparent,
					   D3DCOLOR clrTransparent,
					   float fAlpha)
{
	LoadingOutput("AddAlphaToTexture");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "AddAlphaToTexture()",
								 FALSE);
		return(FALSE);
	}
#endif
	
	// add alpha
	return(AddAlphaToTexture(g_lplpTex[dwIndex],
							 bTransparent,
							 clrTransparent,
							 fAlpha));
}

// AddAlphaToTexture()

BOOL AddAlphaToTexture(IDirect3DTexture8* lpTex,
					   BOOL bTransparent,
					   D3DCOLOR clrTransparent,
					   float fAlpha)
{
	LoadingOutput("AddAlphaToTexture");
	
	// extract transparency color components
	float fRTrans=GetR888(clrTransparent);
	float fGTrans=GetG888(clrTransparent);
	float fBTrans=GetB888(clrTransparent);

	// set minimum transparency color components
	float fRMin=fRTrans-ALPHA_TOL;
	float fGMin=fGTrans-ALPHA_TOL;
	float fBMin=fBTrans-ALPHA_TOL;

	// set maximum transparency color components
	float fRMax=fRTrans+ALPHA_TOL;
	float fGMax=fGTrans+ALPHA_TOL;
	float fBMax=fBTrans+ALPHA_TOL;

	// get surface description
	D3DSURFACE_DESC sd;
	HRESULT hr=lpTex->GetLevelDesc(0,&sd);
	
	// check for error
	if(FAILED(hr))
		return(FALSE);

	// counters
	DWORD dwX,dwY;

	// lock texture surface
	D3DLOCKED_RECT lr;
	hr=lpTex->LockRect(0,
					   &lr,
					   NULL,
					   NULL);

	// check for error
	if(FAILED(hr))
		return(FALSE);

	// get locked rect info
	DWORD dwPitch=lr.Pitch>>1;
	LPWORD lpData=(LPWORD)lr.pBits;

	// get texture size
	DWORD dwWidth=sd.Width;
	DWORD dwHeight=sd.Height;

	// check format
	switch(sd.Format)
	{
	case(D3DFMT_A4R4G4B4):
		// check transparent flag
		if(bTransparent)
		{
			// set alpha values
			for(dwY=0;dwY<dwHeight;dwY++)
				for(dwX=0;dwX<dwWidth;dwX++)
				{
					// time-savers
					DWORD p=dwX+(dwY*dwPitch);
					WORD c=lpData[p];

					// extract color components
					float fR=GetR444(c);
					float fG=GetG444(c);
					float fB=GetB444(c);

					// set alpha value as needed
					if(fR>=fRMin&&
					   fR<=fRMax&&
					   fG>=fGMin&&
					   fG<=fGMax&&
					   fB>=fBMin&&
					   fB<=fBMax)
						SetA444(lpData[p],0)
					else
						SetA444(lpData[p],fAlpha)
				}
		}
		else
		{
			// set alpha values
			for(dwY=0;dwY<dwHeight;dwY++)
				for(dwX=0;dwX<dwWidth;dwX++)
				{
					// time-savers
					DWORD p=dwX+(dwY*dwPitch);

					// set alpha value
					SetA444(lpData[p],fAlpha)
				}
		}
	case(D3DFMT_A1R5G5B5):
		// set alpha values
		for(dwY=0;dwY<dwHeight;dwY++)
			for(dwX=0;dwX<dwWidth;dwX++)
			{
				// time-savers
				DWORD p=dwX+(dwY*dwPitch);
				WORD c=lpData[p];

				// extract color components
				float fR=GetR555(c);
				float fG=GetG555(c);
				float fB=GetB555(c);

				// reset alpha bit as needed
				if(fR>=fRMin&&
				   fR<=fRMax&&
				   fG>=fGMin&&
				   fG<=fGMax&&
				   fB>=fBMin&&
				   fB<=fBMax)
					SetA555(lpData[p],0)
				else
					SetA555(lpData[p],1)
			}
		break;
	}

	// unlock texture surface
	hr=lpTex->UnlockRect(0);

	// check for error
	if(FAILED(hr))
		return(FALSE);

	// ok
	return(TRUE);
}

// ClearTexture()

void ClearTexture(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "ClearTexture()",
								 FALSE);
		return;
	}
#endif
	
	// clear texture
	ClearTexture(g_lplpTex[dwIndex]);
}

// ClearTexture()

void ClearTexture(IDirect3DTexture8* lpTex)
{
	// get surface description
	D3DSURFACE_DESC sd;
	HRESULT hr=lpTex->GetLevelDesc(0,&sd);
	
	// check for error
	if(FAILED(hr))
		return;

	// counters
	DWORD dwX,dwY;

	// lock texture surface
	D3DLOCKED_RECT lr;
	hr=lpTex->LockRect(0,
					   &lr,
					   NULL,
					   NULL);

	// check for error
	if(FAILED(hr))
		return;

	// get locked rect info
	DWORD dwPitch=lr.Pitch>>1;
	LPWORD lpData=(LPWORD)lr.pBits;

	// get texture size
	DWORD dwWidth=sd.Width;
	DWORD dwHeight=sd.Height;

	// clear all pixels	
	for(dwY=0;dwY<dwHeight;dwY++)
		for(dwX=0;dwX<dwWidth;dwX++)
			lpData[dwX+(dwY*dwPitch)]=NULL;

	// unlock texture surface
	hr=lpTex->UnlockRect(0);

	// check for error
	if(FAILED(hr))
		return;
}

// InitFader()

BOOL InitFader(void)
{
	LoadingOutput("InitFader");

	// add vertex buffer
	g_dwFaderVtxBuf=AddVertexBuffer(D3DFVF_FADERVERTEX,
									sizeof(FADERVERTEX),
									4);

	// check vertex buffer
	if(g_dwFaderVtxBuf==NO_OBJECT)
		return(FALSE);

	// add index buffer
	g_dwFaderIdxBuf=AddIndexBuffer(D3DPT_TRIANGLEFAN,
								   2);

	// check index buffer
	if(g_dwFaderIdxBuf==NO_OBJECT)
		return(FALSE);

	// lock index buffer
	LPWORD lpIdx=LockIndexBuffer(g_dwFaderIdxBuf);

	// check for error
	if(!lpIdx)
		return(FALSE);

	// fill index buffer
	lpIdx[0]=0;
	lpIdx[1]=1;
	lpIdx[2]=3;
	lpIdx[3]=2;

	// unlock index buffer
	UnlockIndexBuffer(g_dwFaderIdxBuf);

	// ok
	return(TRUE);
}

// ResetFaderVertexBuffer()

void ResetFaderVertexBuffer(D3DCOLOR clr)
{
	// check fader buffer index (will be invalid upon exit)
	if(g_dwFaderVtxBuf>g_dwNumVtxBuf)
		return;
	
	// lock buffer
	LPFADERVERTEX lpVtx=(LPFADERVERTEX)LockVertexBuffer(g_dwFaderVtxBuf);

	// check for error
	if(!lpVtx)
		return;
	
	// set vertices
	SET_FADERVERTEX(lpVtx[0],
					0,0,1,1,
					clr);
	SET_FADERVERTEX(lpVtx[1],
					g_fCurRenderX,0,1,1,
					clr);
	SET_FADERVERTEX(lpVtx[2],
					0,g_fCurRenderY,1,1,
					clr);
	SET_FADERVERTEX(lpVtx[3],
					g_fCurRenderX,g_fCurRenderY,1,1,
					clr);

	// unlock buffer
	UnlockVertexBuffer(g_dwFaderVtxBuf);
}

// FadeScreen()

void FadeScreen(D3DCOLOR clr)
{
	// reset fader vertices
	ResetFaderVertexBuffer(clr);
	
	// save z-buffer state
	BOOL bZBufferEnabled=g_bZBufferEnabled;

	// disable z-buffer if necesary
	if(bZBufferEnabled)
		DisableZBuffer();
	
	// render overlay
	SetVertexBuffer(g_dwFaderVtxBuf);
	SetIndexBuffer(g_dwFaderIdxBuf);
	SetMaterial();
	SetTexture();
	RenderPrimitive();
	
	// enable z-buffer if necessary
	if(bZBufferEnabled)
		EnableZBuffer();
}

// GetNextMesh()

DWORD GetNextMesh(void)
{
	LoadingOutput("GetNextMesh");

	// check counter
	if(g_dwNumMesh==0)
	{
		// allocate memory		
		if(!AllocMem((LPVOID*)&g_lplpMesh,
					 sizeof(ID3DXMesh*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwNumMeshSubsets,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo,
					 sizeof(LPMESHSUBSETINFO)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpMeshNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxMesh=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumMesh>=g_dwMaxMesh)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpMesh,
					  sizeof(ID3DXMesh*)*g_dwMaxMesh,
					  sizeof(ID3DXMesh*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwNumMeshSubsets,
					  sizeof(DWORD)*g_dwMaxMesh,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpMeshSubsetInfo,
					  sizeof(LPMESHSUBSETINFO)*g_dwMaxMesh,
					  sizeof(LPMESHSUBSETINFO)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpMeshNames,
					  sizeof(LPSTR)*g_dwMaxMesh,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxMesh+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumMesh;

	// increment counter
	g_dwNumMesh++;

	// return index
	return(dwIndex);
}

// AddMeshBox()

DWORD AddMeshBox(float fWidth,
				 float fHeight,
				 float fDepth,
				 DWORD dwMatIndex,
				 LPSTR lpName,
				 BOOL bOptimize)
{
	LoadingOutput("AddMeshBox");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,"~ME%d",g_dwNumMesh);

		// add mesh
		DWORD dwRet=AddMeshBox(fWidth,
							   fHeight,
							   fDepth,
							   dwMatIndex,
							   g_lpMsg,
							   bOptimize);

		// ok
		return(dwRet);
	}

	// get return index
	DWORD dwIndex=GetNextMesh();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create box mesh
	HRESULT hr=D3DXCreateBox(g_lpD3DDevice,
							 fWidth,
							 fHeight,
							 fDepth,
							 &g_lplpMesh[dwIndex],
							 NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create box mesh.",
							  "Check available memory.",
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// OptimizeMesh()

void OptimizeMesh(DWORD dwIndex)
{
	LoadingOutput("OptimizeMesh");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "OptimizeMesh()",
								 FALSE);
		return;
	}
#endif

	// error-shown flag
	static BOOL bErrorShown=FALSE;
	
	// used for error checking
	HRESULT hr;
	
	// get number of faces
	DWORD dwNumFaces=g_lplpMesh[dwIndex]->GetNumFaces();

	// allocate memory for adjacency
	LPDWORD lpdwAdjInfo;
	if(!AllocMem((LPVOID*)&lpdwAdjInfo,
				 sizeof(DWORD)*(dwNumFaces*3)))
		return;

	// get adjacency info
	hr=g_lplpMesh[dwIndex]->GenerateAdjacency(EPSILON,
											  lpdwAdjInfo);

	// check for error
	if(FAILED(hr))
	{
		if(!bErrorShown)
		{
			DisplayDXErrorMessage("Unable to generate mesh adjacency info.",
								  "Scene may not display correctly.",
								  hr);
			bErrorShown=TRUE;
		}
		FreeMem((LPVOID*)&lpdwAdjInfo);
		return;
	}
	
	// optimize mesh
	hr=g_lplpMesh[dwIndex]->OptimizeInplace(D3DXMESHOPT_COMPACT|
											D3DXMESHOPT_ATTRSORT|
											D3DXMESHOPT_VERTEXCACHE,
											lpdwAdjInfo,
											NULL,
											NULL,
											NULL);
	
	// check for attribute sort error and retry if necessary
	if(hr==D3DXERR_CANNOTATTRSORT)
		hr=g_lplpMesh[dwIndex]->OptimizeInplace(D3DXMESHOPT_COMPACT|
												D3DXMESHOPT_VERTEXCACHE,
												lpdwAdjInfo,
												NULL,
												NULL,
												NULL);

	// check for error
	if(FAILED(hr))
	{
		if(!bErrorShown)
		{
			DisplayDXErrorMessage("Unable to optimize mesh.",
  								  "Scene may not display correctly.",
								  hr);
			bErrorShown=TRUE;
		}
		FreeMem((LPVOID*)&lpdwAdjInfo);
		return;
	}

	// release adjacency info
	FreeMem((LPVOID*)&lpdwAdjInfo);
}

// SetMeshSubsetInfo()

void SetMeshSubsetInfo(DWORD dwIndex,
					   DWORD dwSubset,
					   DWORD dwMatIndex,
					   DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "SetMeshSubsetInfo()",
								 FALSE);
		return;
	}
#endif
	
	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset info
	lpMeshSubsetInfo[dwSubset].dwMat=dwMatIndex;
	lpMeshSubsetInfo[dwSubset].dwTex=dwTexIndex;
}

// AddMeshCylinder()

DWORD AddMeshCylinder(float fLowerRad,
					  float fUpperRad,
					  float fHeight,
					  DWORD dwNumSlices,
					  DWORD dwNumStacks,
					  DWORD dwMatIndex,
					  LPSTR lpName,
					  BOOL bOptimize)
{
	LoadingOutput("AddMeshCylinder");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~ME%d",
				g_dwNumMesh);

		// add mesh
		DWORD dwRet=AddMeshCylinder(fLowerRad,
									fUpperRad,
									fHeight,
									dwNumSlices,
									dwNumStacks,
									dwMatIndex,
									g_lpMsg,
									bOptimize);

		// ok
		return(dwRet);
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create cylinder mesh
	HRESULT hr=D3DXCreateCylinder(g_lpD3DDevice,
								  fLowerRad,
								  fUpperRad,
								  fHeight,
								  dwNumSlices,
								  dwNumStacks,
								  &g_lplpMesh[dwIndex],
								  NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create cylinder mesh.",
							  "Check available memory.",
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMeshPolygon()

DWORD AddMeshPolygon(float fLengthOfSide,
					 DWORD dwNumSides,
					 DWORD dwMatIndex,
					 LPSTR lpName,
					 BOOL bOptimize)
{
	LoadingOutput("AddMeshPolygon");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~ME%d",
				g_dwNumMesh);

		// add mesh
		DWORD dwRet=AddMeshPolygon(fLengthOfSide,
								   dwNumSides,
								   dwMatIndex,
								   g_lpMsg,
								   bOptimize);

		// ok
		return(dwRet);
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create polygon mesh
	HRESULT hr=D3DXCreatePolygon(g_lpD3DDevice,
								 fLengthOfSide,
								 dwNumSides,
								 &g_lplpMesh[dwIndex],
								 NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create polygon mesh.",
							  "Check available memory.",
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMeshSphere()

DWORD AddMeshSphere(float fRadius,
					DWORD dwNumSlices,
					DWORD dwNumStacks,
					DWORD dwMatIndex,
					LPSTR lpName,
					BOOL bOptimize)
{
	LoadingOutput("AddMeshSphere");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~ME%d",
				g_dwNumMesh);

		// create mesh
		DWORD dwRet=AddMeshSphere(fRadius,
								  dwNumSlices,
								  dwNumStacks,
								  dwMatIndex,
								  g_lpMsg,
								  bOptimize);

		// ok
		return(dwRet);
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create sphere mesh
	HRESULT hr=D3DXCreateSphere(g_lpD3DDevice,
								fRadius,
								dwNumSlices,
								dwNumStacks,
								&g_lplpMesh[dwIndex],
								NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create sphere mesh.",
							  "Check available memory.",
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMeshTeapot()

DWORD AddMeshTeapot(DWORD dwMatIndex,
					LPSTR lpName,
					BOOL bOptimize)
{
	LoadingOutput("AddMeshTeapot");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~ME%d",
				g_dwNumMesh);

		// add mesh
		DWORD dwRet=AddMeshTeapot(dwMatIndex,
								  g_lpMsg,
								  bOptimize);

		// ok
		return(dwRet);
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create teapot mesh
	HRESULT hr=D3DXCreateTeapot(g_lpD3DDevice,
								&g_lplpMesh[dwIndex],
								NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create teapot mesh.",
							  "Check available memory.",
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMeshText()

DWORD AddMeshText(DWORD dwFontIndex,
				  LPSTR lpText,
				  float fMaxDeviation,
				  float fExtrusion,
				  DWORD dwMatIndex,
				  LPSTR lpName,
				  BOOL bOptimize)
{
	LoadingOutput("AddMeshText");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~ME%d",
				g_dwNumMesh);

		// add mesh
		DWORD dwRet=AddMeshText(dwFontIndex,
								lpText,
								fMaxDeviation,
								fExtrusion,
								dwMatIndex,
								g_lpMsg,
								bOptimize);

		// ok
		return(dwRet);
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// begin text
	BeginText(dwFontIndex);
	
	// create text mesh
	HRESULT hr=D3DXCreateText(g_lpD3DDevice,
							  g_hFontDC,
							  lpText,
							  fMaxDeviation,
							  fExtrusion,
							  &g_lplpMesh[dwIndex],
							  NULL,
							  NULL);

	// end text
	EndText();

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create text mesh.",
							  "Check available memory.",
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMeshTorus()

DWORD AddMeshTorus(float fInnerRad,
				   float fOuterRad,
				   DWORD dwNumSides,
				   DWORD dwNumRings,
				   DWORD dwMatIndex,
				   LPSTR lpName,
				   BOOL bOptimize)
{
	LoadingOutput("AddMeshTorus");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~ME%d",
				g_dwNumMesh);

		// add mesh
		DWORD dwRet=AddMeshTorus(fInnerRad,
								 fOuterRad,
								 dwNumSides,
								 dwNumRings,
								 dwMatIndex,
								 g_lpMsg,
								 bOptimize);

		// ok
		return(dwRet);
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create torus mesh
	HRESULT hr=D3DXCreateTorus(g_lpD3DDevice,
							   fInnerRad,
							   fOuterRad,
							   dwNumSides,
							   dwNumRings,
							   &g_lplpMesh[dwIndex],
							   NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create torus mesh.",
							  "Check available memory.",
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMesh()

DWORD AddMesh(LPCSTR lpFile,
			  LPSTR lpName,
			  BOOL bOptimize,
			  BOOL bNormals)
{
	LoadingOutput("AddMesh");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~ME%d",
				g_dwNumMesh);

		// add mesh
		DWORD dwRet=AddMesh(lpFile,
							g_lpMsg,
							bOptimize);

		// ok
		return(dwRet);
	}

	// get return index
	DWORD dwIndex=GetNextMesh();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// set creation flags
	DWORD dwFlags=D3DXMESH_MANAGED;
	if(g_dwVPF==VPF_HARDWARE)
		dwFlags=dwFlags|D3DXMESH_USEHWONLY;
	if(g_dwVPF==VPF_SOFTWARE)
		dwFlags=dwFlags|D3DXMESH_SOFTWAREPROCESSING;

	// material buffer
	ID3DXBuffer* lpMaterials;
	DWORD dwNumMat;

	// used for error checking
	HRESULT hr;

	// counter
	DWORD dwCount;
	
	// check for data file
	DATAFILEREF dfr=FindDataFileRef((LPSTR)lpFile);

	// load mesh
	if(dfr.lpData)
		hr=D3DXLoadMeshFromXInMemory((LPBYTE)dfr.lpData,
									 dfr.dwSize,
									 dwFlags,
									 g_lpD3DDevice,
									 NULL,
									 &lpMaterials,
									 &dwNumMat,
									 &g_lplpMesh[dwIndex]);
	else
		hr=D3DXLoadMeshFromX((LPSTR)lpFile,
							  dwFlags,
							  g_lpD3DDevice,
							  NULL,
							  &lpMaterials,
							  &dwNumMat,
							  &g_lplpMesh[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to load mesh file.",
							  NULL,
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResources(2);

	// check number of materials
	if(dwNumMat)
	{
		// set number of subsets
		g_lpdwNumMeshSubsets[dwIndex]=dwNumMat;
		
		// allocate subset array
		if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
					 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		{
			ReleaseCOMPtr(lpMaterials);
			return(NO_OBJECT);
		}

		// get subset info pointer
		LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
		
		// get materials buffer pointer
		LPD3DXMATERIAL lpMat=(LPD3DXMATERIAL)lpMaterials->GetBufferPointer();
		
		// fill in subset array
		for(dwCount=0;dwCount<dwNumMat;dwCount++)
		{
			// update and copy material
			lpMat[dwCount].MatD3D.Ambient=lpMat[dwCount].MatD3D.Diffuse;
			lpMeshSubsetInfo[dwCount].dwMat=AddMaterial(&lpMat[dwCount].MatD3D);

			// check material
			if(lpMeshSubsetInfo[dwCount].dwMat==NO_OBJECT)
			{
				ReleaseCOMPtr(lpMaterials);
				return(NO_OBJECT);
			}

			// load texture if needed
			if(lpMat[dwCount].pTextureFilename)
			{
				// add texture
				lpMeshSubsetInfo[dwCount].dwTex=AddTexture(lpMat[dwCount].pTextureFilename);

				// check texture
				if(lpMeshSubsetInfo[dwCount].dwTex==NO_OBJECT)
				{
					ReleaseCOMPtr(lpMaterials);
					return(NO_OBJECT);
				}
			}
			else
				lpMeshSubsetInfo[dwCount].dwTex=NO_OBJECT;
		}
	}
	else
	{
		// set number of subsets
		g_lpdwNumMeshSubsets[dwIndex]=1;
		
		// allocate subset array
		if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
					 sizeof(MESHSUBSETINFO)))
		{
			ReleaseCOMPtr(lpMaterials);
			return(NO_OBJECT);
		}

		// get subset info pointer
		LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
		
		// fill in subset array
		lpMeshSubsetInfo[0].dwMat=AddMaterial(1,1,1,1,
											  1,1,1,1,
											  0,0,0,0,
											  0,0,0,0,
											  0);
		lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

		// check material
		if(lpMeshSubsetInfo[0].dwMat==NO_OBJECT)
		{
			ReleaseCOMPtr(lpMaterials);
			return(NO_OBJECT);
		}
	}

	// release material buffer
	ReleaseCOMPtr(lpMaterials);

	// add normals to mesh
	if(bNormals)
		AddMeshNormals(dwIndex);

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// RenderMesh()

void RenderMesh(DWORD dwMeshIndex,
				DWORD dwWMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMeshIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "RenderMesh()",
								 FALSE);
		return;
	}

	// verify index
	if(dwWMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "RenderMesh()",
								 FALSE);
		return;
	}
#endif
	
	// counter
	DWORD dwCount;
	
	// set world matrix
	SetWorldMatrix(dwWMtxIndex);
	
	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwMeshIndex];
	
	// render mesh
	for(dwCount=0;dwCount<g_lpdwNumMeshSubsets[dwMeshIndex];dwCount++)
	{
		// set subset info
		SetMaterial(lpMeshSubsetInfo[dwCount].dwMat);
		SetTexture(lpMeshSubsetInfo[dwCount].dwTex);
		
		// render subset
		g_lplpMesh[dwMeshIndex]->DrawSubset(dwCount);
	}

	// invalidate render values
	g_bInvalidRenderValues=TRUE;
}

// RenderMesh()

void RenderMesh(DWORD dwMeshIndex,
				LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMeshIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "RenderMesh()",
								 FALSE);
		return;
	}
#endif
	
	// counter
	DWORD dwCount;
	
	// set world matrix
	SetWorldMatrix(lpMtx);
	
	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwMeshIndex];
	
	// render mesh
	for(dwCount=0;dwCount<g_lpdwNumMeshSubsets[dwMeshIndex];dwCount++)
	{
		// set subset info
		SetMaterial(lpMeshSubsetInfo[dwCount].dwMat);
		SetTexture(lpMeshSubsetInfo[dwCount].dwTex);
		
		// render subset
		g_lplpMesh[dwMeshIndex]->DrawSubset(dwCount);
	}

	// invalidate render values
	g_bInvalidRenderValues=TRUE;
}

// RenderMeshAlpha()

void RenderMeshAlpha(DWORD dwMeshIndex,
					 DWORD dwWMtxIndex,
					 float fAlpha)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMeshIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "RenderMeshAlpha()",
								 FALSE);
		return;
	}

	// verify index
	if(dwWMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "RenderMeshAlpha()",
								 FALSE);
		return;
	}
#endif
	
	// counter
	DWORD dwCount;
	
	// set world matrix
	SetWorldMatrix(dwWMtxIndex);
	
	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwMeshIndex];
	
	// render mesh
	for(dwCount=0;dwCount<g_lpdwNumMeshSubsets[dwMeshIndex];dwCount++)
	{
		// copy material
		g_lpMat[g_dwAlphaMeshMat]=g_lpMat[lpMeshSubsetInfo[dwCount].dwMat];

		// update alpha values
		g_lpMat[g_dwAlphaMeshMat].Diffuse.a*=fAlpha;
		g_lpMat[g_dwAlphaMeshMat].Ambient.a*=fAlpha;
		g_lpMat[g_dwAlphaMeshMat].Specular.a*=fAlpha;
		g_lpMat[g_dwAlphaMeshMat].Emissive.a*=fAlpha;
		
		// set subset info
		SetMaterial(g_dwAlphaMeshMat);
		SetTexture(lpMeshSubsetInfo[dwCount].dwTex);
		
		// render subset
		g_lplpMesh[dwMeshIndex]->DrawSubset(dwCount);
	}

	// invalidate render values
	g_bInvalidRenderValues=TRUE;
}

// RenderMeshAlpha()

void RenderMeshAlpha(DWORD dwMeshIndex,
					 LPD3DXMATRIX lpMtx,
					 float fAlpha)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMeshIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "RenderMeshAlpha()",
								 FALSE);
		return;
	}
#endif
	
	// counter
	DWORD dwCount;
	
	// set world matrix
	SetWorldMatrix(lpMtx);
	
	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwMeshIndex];
	
	// render mesh
	for(dwCount=0;dwCount<g_lpdwNumMeshSubsets[dwMeshIndex];dwCount++)
	{
		// copy material
		g_lpMat[g_dwAlphaMeshMat]=g_lpMat[lpMeshSubsetInfo[dwCount].dwMat];

		// update alpha values
		g_lpMat[g_dwAlphaMeshMat].Diffuse.a*=fAlpha;
		g_lpMat[g_dwAlphaMeshMat].Ambient.a*=fAlpha;
		g_lpMat[g_dwAlphaMeshMat].Specular.a*=fAlpha;
		g_lpMat[g_dwAlphaMeshMat].Emissive.a*=fAlpha;
		
		// set subset info
		SetMaterial(g_dwAlphaMeshMat);
		SetTexture(lpMeshSubsetInfo[dwCount].dwTex);
		
		// render subset
		g_lplpMesh[dwMeshIndex]->DrawSubset(dwCount);
	}

	// invalidate render values
	g_bInvalidRenderValues=TRUE;
}

// PurgeMeshes()

void PurgeMeshes(void)
{
	// check counter
	if(g_dwNumMesh)
	{
		// counter
		DWORD dwCount;

		// release all meshes data
		for(dwCount=0;dwCount<g_dwNumMesh;dwCount++)
		{
			ReleaseCOMPtr(g_lplpMesh[dwCount]);
			FreeMem((LPVOID*)&g_lplpMeshSubsetInfo[dwCount]);
			FreeMem((LPVOID*)&g_lplpMeshNames[dwCount]);
		}
	}

	// free memory
	FreeMem((LPVOID*)&g_lplpMesh);
	FreeMem((LPVOID*)&g_lpdwNumMeshSubsets);
	FreeMem((LPVOID*)&g_lplpMeshSubsetInfo);
	FreeMem((LPVOID*)&g_lplpMeshNames);
	
	// reset counters
	g_dwNumMesh=0;
	g_dwMaxMesh=0;
}

// GetMatrix()

void GetMatrix(DWORD dwIndex,
			   LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "GetMatrix()",
								 FALSE);
		return;
	}
#endif

	// copy matrix
	CopyMemory(lpMtx,
			   &g_lpMtx[dwIndex],		   
			   sizeof(D3DXMATRIX));

	// scale matrix if needed
	if(!IsNearEqual(g_lpMtx[dwIndex].fXScale,1))
	{
		lpMtx->_11*=g_lpMtx[dwIndex].fXScale;
		lpMtx->_21*=g_lpMtx[dwIndex].fXScale;
		lpMtx->_31*=g_lpMtx[dwIndex].fXScale;
	}
	if(!IsNearEqual(g_lpMtx[dwIndex].fYScale,1))
	{
		lpMtx->_12*=g_lpMtx[dwIndex].fYScale;
		lpMtx->_22*=g_lpMtx[dwIndex].fYScale;
		lpMtx->_32*=g_lpMtx[dwIndex].fYScale;
	}
	if(!IsNearEqual(g_lpMtx[dwIndex].fZScale,1))
	{
		lpMtx->_13*=g_lpMtx[dwIndex].fZScale;
		lpMtx->_23*=g_lpMtx[dwIndex].fZScale;
		lpMtx->_33*=g_lpMtx[dwIndex].fZScale;
	}
}

// SetMatrix()

void SetMatrix(DWORD dwIndex,
			   LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetMatrix()",
								 FALSE);
		return;
	}
#endif

	// copy matrix
	CopyMemory(&g_lpMtx[dwIndex],
			   lpMtx,
			   sizeof(D3DXMATRIX));

	// extract scale values
	g_lpMtx[dwIndex].fXScale=Magnitude(MtxRightX,MtxRightY,MtxRightZ);
	g_lpMtx[dwIndex].fYScale=Magnitude(MtxUpX,   MtxUpY,   MtxUpZ);
	g_lpMtx[dwIndex].fZScale=Magnitude(MtxAheadX,MtxAheadY,MtxAheadZ);

	// scale matrix accordingly
	MtxRightX/=g_lpMtx[dwIndex].fXScale;
	MtxRightY/=g_lpMtx[dwIndex].fYScale;
	MtxRightZ/=g_lpMtx[dwIndex].fZScale;
	MtxUpX/=   g_lpMtx[dwIndex].fXScale;
	MtxUpY/=   g_lpMtx[dwIndex].fYScale;
	MtxUpZ/=   g_lpMtx[dwIndex].fZScale;
	MtxAheadX/=g_lpMtx[dwIndex].fXScale;
	MtxAheadY/=g_lpMtx[dwIndex].fYScale;
	MtxAheadZ/=g_lpMtx[dwIndex].fZScale;
}

#ifdef DEBUG_MODE
// InitLineRender()

BOOL InitLineRender(void)
{
	LoadingOutput("InitLineRender");

	// add line material
	g_dwLineMat=AddMaterial(0,0,0,1,
							0,0,0,1,
							0,0,0,1,
							1,1,1,1,
							0);

	// check material
	if(g_dwLineMat==NO_OBJECT)
		return(FALSE);

	// create line vertex buffer
	g_dwLineVtxBuf=AddVertexBuffer(D3DFVF_LINEVERTEX,
								   sizeof(LINEVERTEX),
								   2);

	// check vertex buffer
	if(g_dwLineVtxBuf==NO_OBJECT)
		return(FALSE);

	// create line index buffer
	g_dwLineIdxBuf=AddIndexBuffer(D3DPT_LINELIST,
								  1);

	// check index buffer
	if(g_dwLineIdxBuf==NO_OBJECT)
		return(FALSE);

	// lock index buffer
	LPWORD lpIdx=LockIndexBuffer(g_dwLineIdxBuf);
	
	// check for error
	if(!lpIdx)
		return(FALSE);

	// fill in index buffer
	lpIdx[0]=0;
	lpIdx[1]=1;

	// unlock index buffer
	UnlockIndexBuffer(g_dwLineIdxBuf);

	// ok
	return(TRUE);
}

// RenderLine()

void RenderLine(float fX1,
				float fY1,
				float fZ1,
				float fX2,
				float fY2,
				float fZ2,
				D3DCOLOR clr)
{
	// reset world matrix
	SetWorldMatrix();
	
	// render line
	RenderTransformedLine(fX1,fY1,fZ1,fX2,fY2,fZ2,clr);
}

// RenderLine()

void RenderLine(LPD3DXVECTOR3 lpV1,
				LPD3DXVECTOR3 lpV2,
				D3DCOLOR clr)
{
	// reset world matrix
	SetWorldMatrix();
	
	// render line
	RenderTransformedLine(lpV1->x,
						  lpV1->y,
						  lpV1->z,
						  lpV2->x,
						  lpV2->y,
						  lpV2->z,
						  clr);
}

// RenderTransformedLine()

void RenderTransformedLine(float fX1,
						   float fY1,
						   float fZ1,
						   float fX2,
						   float fY2,
						   float fZ2,
						   D3DCOLOR clr)
{
	// fill in vertex buffer
	LPLINEVERTEX lpVtx=(LPLINEVERTEX)LockVertexBuffer(g_dwLineVtxBuf);
	SET_LINEVERTEX(lpVtx[0],fX1,fY1,fZ1);
	SET_LINEVERTEX(lpVtx[1],fX2,fY2,fZ2);
	UnlockVertexBuffer(g_dwLineVtxBuf);

	// update color
	UpdateMaterialEmissive(g_dwLineMat,
						   GetR888(clr),
						   GetG888(clr),
						   GetB888(clr),
						   GetA888(clr));
	
	// render line
	SetMaterial(g_dwLineMat);
	SetTexture();
	SetVertexBuffer(g_dwLineVtxBuf);
	SetIndexBuffer(g_dwLineIdxBuf);
	RenderPrimitive();
}

// RenderTransformedLine()

void RenderTransformedLine(LPD3DXVECTOR3 lpV1,
						   LPD3DXVECTOR3 lpV2,
						   D3DCOLOR clr)
{
	// render line
	RenderTransformedLine(lpV1->x,
						  lpV1->y,
						  lpV1->z,
						  lpV2->x,
						  lpV2->y,
						  lpV2->z,
						  clr);
}
#endif

// SetViewMatrix()

void SetViewMatrix(float fPosX,
				   float fPosY,
				   float fPosZ,
				   float fTargetX,
				   float fTargetY,
				   float fTargetZ,
				   float fUpX,
				   float fUpY,
				   float fUpZ)
{
	// prepare vectors
	D3DXVECTOR3 vPos=D3DXVECTOR3(fPosX,fPosY,fPosZ);
	D3DXVECTOR3 vTarget=D3DXVECTOR3(fTargetX,fTargetY,fTargetZ);
	D3DXVECTOR3 vUp=D3DXVECTOR3(fUpX,fUpY,fUpZ);
	
	// prepare view matrix
	D3DXMATRIX mView;
	D3DXMatrixLookAtLH(&mView,
					   &vPos,
					   &vTarget,
					   &vUp);

	// set view matrix
	SetViewMatrix(&mView);
}

// SetProjectionMatrix()

void SetProjectionMatrix(float fClipNear,
						 float fClipFar,
						 float fAngle,
						 float fAspect)
{
	// prepare projection matrix
	D3DXMATRIX mProj;
	D3DXMatrixPerspectiveFovLH(&mProj,
							   DegToRad(fAngle),
							   fAspect,
							   fClipNear,
							   fClipFar);
	
	// set projection matrix
	SetProjectionMatrix(AddMatrix(&mProj));
}

// SetCameraMatrix()

void SetCameraMatrix(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetCameraMatrix()",
								 FALSE);
		return;
	}
#endif

	// get view matrix
	D3DXMATRIX mView;
	GetMatrix(dwIndex,
			  &mView);

	// set billboard matrix
	SetBillboardMatrix(&mView);

	// invert view matrix
	D3DXMATRIX mCam;
	D3DXMatrixInverse(&mCam,
					  NULL,
					  &mView);
	
	// save matrix
	CopyMemory(&g_mtxView,
			   &mCam,
			   sizeof(D3DXMATRIX));

	// set matrix
	g_lpD3DDevice->SetTransform(D3DTS_VIEW,
								&mCam);
}

// PurgeGraphicsObjects()

void PurgeGraphicsObjects(void)
{
	// purge each object
	PurgeVertexBuffers();
	PurgeIndexBuffers();
	PurgeMatrices();
	PurgeMaterials();
	PurgeTextures();
	PurgeLights();
	PurgeMeshes();
	PurgeSkinMeshes();
	PurgeAnimations();
}

#ifdef DEBUG_MODE
// DisplayMatrixMessage()

void DisplayMatrixMessage(LPD3DXMATRIX lpMtx)
{
	LockGlobalMsg()

	// prepare message
	sprintf(g_lpMsg,
			"%f, %f, %f, %f\n"
			"%f, %f, %f, %f\n"
			"%f, %f, %f, %f\n"
			"%f, %f, %f, %f",
			lpMtx->_11,
			lpMtx->_12,
			lpMtx->_13,
			lpMtx->_14,
			lpMtx->_21,
			lpMtx->_22,
			lpMtx->_23,
			lpMtx->_24,
			lpMtx->_31,
			lpMtx->_32,
			lpMtx->_33,
			lpMtx->_34,
			lpMtx->_41,
			lpMtx->_42,
			lpMtx->_43,
			lpMtx->_44);

	// display message
	DisplayInfoMessage(g_lpMsg);

	UnlockGlobalMsg()
}
#endif

// InitSkybox()

BOOL InitSkybox(void)
{
	LoadingOutput("InitSkybox");

	// add skybox matrix
	g_dwSkyboxMtx=AddMatrix(NULL);

	// check matrix
	if(g_dwSkyboxMtx==NO_OBJECT)
		return(FALSE);

	// add skybox material
	g_dwSkyboxMat=AddMaterial(0,0,0,1,
							  0,0,0,1,
							  0,0,0,1,
							  1,1,1,1,
							  0);

	// check material
	if(g_dwSkyboxMat==NO_OBJECT)
		return(FALSE);

	// add skybox texture
	g_dwSkyboxTex=AddTexture(2,2);

	// check texture
	if(g_dwSkyboxTex==NO_OBJECT)
		return(FALSE);

	// clear texture
	ClearTexture(g_dwSkyboxTex);

	// add skybox vertex buffer #1
	g_sbLeft.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										   sizeof(SKYBOXVERTEX),
										   5);

	// check skybox vertex buffer #1
	if(g_sbLeft.dwVtxBufIndex==NO_OBJECT)
		return(FALSE);

	// add skybox vertex buffer #2
	g_sbRight.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										    sizeof(SKYBOXVERTEX),
										    5);
	// check skybox vertex buffer #2
	if(g_sbRight.dwVtxBufIndex==NO_OBJECT)
		return(FALSE);

	// add skybox vertex buffer #3
	g_sbFront.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										    sizeof(SKYBOXVERTEX),
										    5);
	// check skybox vertex buffer #3
	if(g_sbFront.dwVtxBufIndex==NO_OBJECT)
		return(FALSE);

	// add skybox vertex buffer #4
	g_sbBack.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										   sizeof(SKYBOXVERTEX),
										   5);
	// check skybox vertex buffer #4
	if(g_sbBack.dwVtxBufIndex==NO_OBJECT)
		return(FALSE);

	// add skybox vertex buffer #5
	g_sbTop.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										  sizeof(SKYBOXVERTEX),
										  5);
	// check skybox vertex buffer #5
	if(g_sbTop.dwVtxBufIndex==NO_OBJECT)
		return(FALSE);

	// add skybox vertex buffer #6
	g_sbBottom.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										     sizeof(SKYBOXVERTEX),
										     5);
	// check skybox vertex buffer #6
	if(g_sbBottom.dwVtxBufIndex==NO_OBJECT)
		return(FALSE);

	// skybox vertices
	LPSKYBOXVERTEX lpVtx;
	
	// lock vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbLeft.dwVtxBufIndex);

	// check for error
	if(!lpVtx)
		return(FALSE);

	// set left skybox vertex buffer
	SET_SKYBOXVERTEX(lpVtx[0],-0.707106F,0.707106F,-0.707106F,SKYBOX_TEXMIN,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[1],-0.707106F,0.707106F,0.707106F,0.996093F,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[2],-1,0,0,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],-0.707106F,-0.707106F,-0.707106F,SKYBOX_TEXMIN,0.996093F);
	SET_SKYBOXVERTEX(lpVtx[4],-0.707106F,-0.707106F,0.707106F,0.996093F,0.996093F);

	// unlock vertex buffer
	UnlockVertexBuffer(g_sbLeft.dwVtxBufIndex);

	// lock vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbRight.dwVtxBufIndex);

	// check for error
	if(!lpVtx)
		return(FALSE);

	// set right skybox vertex buffer
	SET_SKYBOXVERTEX(lpVtx[0],0.707106F,0.707106F,0.707106F,SKYBOX_TEXMIN,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[1],0.707106F,0.707106F,-0.707106F,SKYBOX_TEXMAX,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[2],1,0,0,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],0.707106F,-0.707106F,0.707106F,SKYBOX_TEXMIN,SKYBOX_TEXMAX);
	SET_SKYBOXVERTEX(lpVtx[4],0.707106F,-0.707106F,-0.707106F,SKYBOX_TEXMAX,SKYBOX_TEXMAX);
	
	// unlock vertex buffer
	UnlockVertexBuffer(g_sbRight.dwVtxBufIndex);

	// lock vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbFront.dwVtxBufIndex);
	
	// check for error
	if(!lpVtx)
		return(FALSE);

	// set front skybox vertex buffer
	SET_SKYBOXVERTEX(lpVtx[0],-0.707106F,0.707106F,0.707106F,SKYBOX_TEXMIN,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[1],0.707106F,0.707106F,0.707106F,SKYBOX_TEXMAX,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[2],0,0,1,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],-0.707106F,-0.707106F,0.707106F,SKYBOX_TEXMIN,SKYBOX_TEXMAX);
	SET_SKYBOXVERTEX(lpVtx[4],0.707106F,-0.707106F,0.707106F,SKYBOX_TEXMAX,SKYBOX_TEXMAX);
	
	// unlock vertex buffer
	UnlockVertexBuffer(g_sbFront.dwVtxBufIndex);

	// lock vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbBack.dwVtxBufIndex);
	
	// check for error
	if(!lpVtx)
		return(FALSE);

	// set back skybox vertex buffer
	SET_SKYBOXVERTEX(lpVtx[0],0.707106F,0.707106F,-0.707106F,SKYBOX_TEXMIN,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[1],-0.707106F,0.707106F,-0.707106F,SKYBOX_TEXMAX,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[2],0,0,-1,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],0.707106F,-0.707106F,-0.707106F,SKYBOX_TEXMIN,SKYBOX_TEXMAX);
	SET_SKYBOXVERTEX(lpVtx[4],-0.707106F,-0.707106F,-0.707106F,SKYBOX_TEXMAX,SKYBOX_TEXMAX);
	
	// unlock vertex buffer
	UnlockVertexBuffer(g_sbBack.dwVtxBufIndex);

	// lock vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbTop.dwVtxBufIndex);
	
	// check for error
	if(!lpVtx)
		return(FALSE);

	// set top skybox vertex buffer
	SET_SKYBOXVERTEX(lpVtx[0],-0.707106F,0.707106F,-0.707106F,SKYBOX_TEXMIN,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[1],0.707106F,0.707106F,-0.707106F,SKYBOX_TEXMAX,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[2],0,1,0,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],-0.707106F,0.707106F,0.707106F,SKYBOX_TEXMIN,SKYBOX_TEXMAX);
	SET_SKYBOXVERTEX(lpVtx[4],0.707106F,0.707106F,0.707106F,SKYBOX_TEXMAX,SKYBOX_TEXMAX);
	
	// unlock vertex buffer
	UnlockVertexBuffer(g_sbTop.dwVtxBufIndex);

	// lock vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbBottom.dwVtxBufIndex);
	
	// check for error
	if(!lpVtx)
		return(FALSE);

	// set bottom skybox vertex buffer
	SET_SKYBOXVERTEX(lpVtx[0],-0.707106F,-0.707106F,0.707106F,SKYBOX_TEXMIN,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[1],0.707106F,-0.707106F,0.707106F,SKYBOX_TEXMAX,SKYBOX_TEXMIN);
	SET_SKYBOXVERTEX(lpVtx[2],0,-1,0,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],-0.707106F,-0.707106F,-0.707106F,SKYBOX_TEXMIN,SKYBOX_TEXMAX);
	SET_SKYBOXVERTEX(lpVtx[4],0.707106F,-0.707106F,-0.707106F,SKYBOX_TEXMAX,SKYBOX_TEXMAX);
	
	// unlock vertex buffer
	UnlockVertexBuffer(g_sbBottom.dwVtxBufIndex);

	// add skybox index buffer
	g_dwSkyboxIdxBuf=AddIndexBuffer(D3DPT_TRIANGLEFAN,
									4);

	// check index buffer
	if(g_dwSkyboxIdxBuf==NO_OBJECT)
		return(FALSE);

	// lock index buffer
	LPWORD lpIdx=LockIndexBuffer(g_dwSkyboxIdxBuf);

	// check for error
	if(!lpIdx)
		return(FALSE);

	// set skybox index buffer
	lpIdx[0]=2;
	lpIdx[1]=0;
	lpIdx[2]=1;
	lpIdx[3]=4;
	lpIdx[4]=3;
	lpIdx[5]=0;
	
	// unlock index buffer
	UnlockIndexBuffer(g_dwSkyboxIdxBuf);

	// reset skybox textures
	g_sbLeft.dwTexIndex=g_dwSkyboxTex;
	g_sbRight.dwTexIndex=g_dwSkyboxTex;
	g_sbFront.dwTexIndex=g_dwSkyboxTex;
	g_sbBack.dwTexIndex=g_dwSkyboxTex;
	g_sbTop.dwTexIndex=g_dwSkyboxTex;
	g_sbBottom.dwTexIndex=g_dwSkyboxTex;

	// reset offset
	g_vSkyboxOffset=D3DXVECTOR3(0,0,0);

	// set default radius
	g_lpMtx[g_dwSkyboxMtx].fXScale=75;
	g_lpMtx[g_dwSkyboxMtx].fYScale=75;
	g_lpMtx[g_dwSkyboxMtx].fZScale=75;

	// ok
	return(TRUE);
}

// SetSkybox()

void SetSkybox(float fRadius,
			   float fOffsetX,
			   float fOffsetY,
			   float fOffsetZ,
			   DWORD dwLtTexIdx,
			   DWORD dwRtTexIdx,
			   DWORD dwFtTexIdx,
			   DWORD dwBkTexIdx,
			   DWORD dwTpTexIdx,
			   DWORD dwBtTexIdx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwLtTexIdx>=g_dwNumTex&&
	   dwLtTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}

	// verify index
	if(dwRtTexIdx>=g_dwNumTex&&
	   dwRtTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}

	// verify index
	if(dwFtTexIdx>=g_dwNumTex&&
	   dwFtTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}

	// verify index
	if(dwBkTexIdx>=g_dwNumTex&&
	   dwBkTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTpTexIdx>=g_dwNumTex&&
	   dwTpTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}

	// verify index
	if(dwBtTexIdx>=g_dwNumTex&&
	   dwBtTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}
#endif

	// set textures
	if(dwLtTexIdx==NO_OBJECT)
		g_sbLeft.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbLeft.dwTexIndex=dwLtTexIdx;
	if(dwRtTexIdx==NO_OBJECT)
		g_sbRight.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbRight.dwTexIndex=dwRtTexIdx;
	if(dwFtTexIdx==NO_OBJECT)
		g_sbFront.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbFront.dwTexIndex=dwFtTexIdx;
	if(dwBkTexIdx==NO_OBJECT)
		g_sbBack.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbBack.dwTexIndex=dwBkTexIdx;
	if(dwTpTexIdx==NO_OBJECT)
		g_sbTop.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbTop.dwTexIndex=dwTpTexIdx;
	if(dwBtTexIdx==NO_OBJECT)
		g_sbBottom.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbBottom.dwTexIndex=dwBtTexIdx;
	
	// set radius
	g_fSkyboxRadius=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fXScale=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fYScale=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fZScale=fRadius;

	// set offset
	g_vSkyboxOffset=D3DXVECTOR3(fOffsetX,fOffsetY,fOffsetZ);
}

// SetSkyboxLeft()

void SetSkyboxLeft(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxLeft()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbLeft.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbLeft.dwTexIndex=dwTexIndex;
}

// SetSkyboxRight()

void SetSkyboxRight(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxRight()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbRight.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbRight.dwTexIndex=dwTexIndex;
}

// SetSkyboxFront()

void SetSkyboxFront(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxFront()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbFront.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbFront.dwTexIndex=dwTexIndex;
}

// SetSkyboxBack()

void SetSkyboxBack(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxBack()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbBack.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbBack.dwTexIndex=dwTexIndex;
}

// SetSkyboxTop()

void SetSkyboxTop(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxTop()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbTop.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbTop.dwTexIndex=dwTexIndex;
}

// SetSkyboxBottom()

void SetSkyboxBottom(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxBottom()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbBottom.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbBottom.dwTexIndex=dwTexIndex;
}

// SetSkyboxRadius()

void SetSkyboxRadius(float fRadius)
{
	// copy radius
	g_fSkyboxRadius=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fXScale=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fYScale=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fZScale=fRadius;
}

// RenderSkybox()

void RenderSkybox(LPD3DXVECTOR3 lpV)
{
	// render skybox
	RenderSkybox(lpV->x,
				 lpV->y,
				 lpV->z);
}

// RenderSkybox()

void RenderSkybox(DWORD dwWMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwWMtxIndex>=g_dwNumMtx)

	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "RenderSkybox()",
								 FALSE);
		return;
	}
#endif
	
	// render skybox
	RenderSkybox(g_lpMtx[dwWMtxIndex].mtx._41,
				 g_lpMtx[dwWMtxIndex].mtx._42,
				 g_lpMtx[dwWMtxIndex].mtx._43);
}

// RenderSkybox()

void RenderSkybox(float fX,
				  float fY,
				  float fZ)
{
	// update skybox matrix
	g_lpMtx[g_dwSkyboxMtx].mtx._41=fX-(g_vSkyboxOffset.x*fX);
	g_lpMtx[g_dwSkyboxMtx].mtx._42=fY-(g_vSkyboxOffset.y*fY);
	g_lpMtx[g_dwSkyboxMtx].mtx._43=fZ-(g_vSkyboxOffset.z*fZ);

	// disable z-buffer writes
	BOOL bFlag=g_bZBufferWrites;
	if(bFlag)
		DisableZBufferWrites();

	// set render values
	SetWorldMatrix(g_dwSkyboxMtx);
	SetMaterial(g_dwSkyboxMat);
	SetIndexBuffer(g_dwSkyboxIdxBuf);

	// render left side
	SetVertexBuffer(g_sbLeft.dwVtxBufIndex);
	SetTexture(g_sbLeft.dwTexIndex);
	RenderPrimitive();

	// render right side
	SetVertexBuffer(g_sbRight.dwVtxBufIndex);
	SetTexture(g_sbRight.dwTexIndex);
	RenderPrimitive();

	// render front
	SetVertexBuffer(g_sbFront.dwVtxBufIndex);
	SetTexture(g_sbFront.dwTexIndex);
	RenderPrimitive();

	// render back
	SetVertexBuffer(g_sbBack.dwVtxBufIndex);
	SetTexture(g_sbBack.dwTexIndex);
	RenderPrimitive();

	// render top
	SetVertexBuffer(g_sbTop.dwVtxBufIndex);
	SetTexture(g_sbTop.dwTexIndex);
	RenderPrimitive();

	// render bottom
	SetVertexBuffer(g_sbBottom.dwVtxBufIndex);
	SetTexture(g_sbBottom.dwTexIndex);
	RenderPrimitive();

	// restore z-buffer writes
	if(bFlag)
		EnableZBufferWrites();
}

// InitBillboards()

BOOL InitBillboards(void)
{
	LoadingOutput("InitBillboards");

	// add billboard matrix
	g_dwBBMtxIndex=AddMatrix(NULL);

	// check matrix
	if(g_dwBBMtxIndex==NO_OBJECT)
		return(FALSE);

	// add vertical billboard matrix
	g_dwVBBMtxIndex=AddMatrix(NULL);

	// check matrix
	if(g_dwVBBMtxIndex==NO_OBJECT)
		return(FALSE);

	// add billboard vertex buffers
	g_dwBBVtxBufIdx=AddVertexBuffer(D3DFVF_BILLBOARDVERTEX,
									sizeof(BILLBOARDVERTEX),
									4);

	// check vertex buffer
	if(g_dwBBVtxBufIdx==NO_OBJECT)
		return(FALSE);

	// lock vertex buffer
	LPBILLBOARDVERTEX lpVtx=(LPBILLBOARDVERTEX)LockVertexBuffer(g_dwBBVtxBufIdx);

	// check for error
	if(!lpVtx)
		return(FALSE);

	// set billboard vertex buffer
	SET_BILLBOARDVERTEX(lpVtx[0],
						-0.5F,-0.5F,0,
						0,0,1,
						0,1);
	SET_BILLBOARDVERTEX(lpVtx[1],
						0.5F,-0.5F,0,
						0,0,1,
						1,1);
	SET_BILLBOARDVERTEX(lpVtx[2],
						-0.5F,0.5F,0,
						0,0,1,
						0,0);
	SET_BILLBOARDVERTEX(lpVtx[3],
						0.5F,0.5F,0,
						0,0,1,
						1,0);
	
	// unlock vertex buffer
	UnlockVertexBuffer(g_dwBBVtxBufIdx);

	// add reverse billboard vertex buffers
	g_dwBBRevVtxBufIdx=AddVertexBuffer(D3DFVF_BILLBOARDVERTEX,
									   sizeof(BILLBOARDVERTEX),
									   4);

	// check vertex buffer
	if(g_dwBBRevVtxBufIdx==NO_OBJECT)
		return(FALSE);

	// lock vertex buffer
	lpVtx=(LPBILLBOARDVERTEX)LockVertexBuffer(g_dwBBRevVtxBufIdx);

	// check for error
	if(!lpVtx)
		return(FALSE);

	// set reverse billboard vertex buffer
	SET_BILLBOARDVERTEX(lpVtx[0],
						-0.5F,-0.5F,0,
						0,0,1,
						1,1);
	SET_BILLBOARDVERTEX(lpVtx[1],
						0.5F,-0.5F,0,
						0,0,1,
						0,1);
	SET_BILLBOARDVERTEX(lpVtx[2],
						-0.5F,0.5F,0,
						0,0,1,
						1,0);
	SET_BILLBOARDVERTEX(lpVtx[3],
						0.5F,0.5F,0,
						0,0,1,
						0,0);
	
	// unlock vertex buffer
	UnlockVertexBuffer(g_dwBBRevVtxBufIdx);

	// add billboard index buffer
	g_dwBBIdxBufIdx=AddIndexBuffer(D3DPT_TRIANGLEFAN,
								   2);

	// check index buffer
	if(g_dwBBIdxBufIdx==NO_OBJECT)
		return(FALSE);

	// lock index buffer
	LPWORD lpIdx=LockIndexBuffer(g_dwBBIdxBufIdx);

	// check for error
	if(!lpIdx)
		return(FALSE);

	// set billboard index buffer
	lpIdx[0]=1;
	lpIdx[1]=0;
	lpIdx[2]=2;
	lpIdx[3]=3;

	// unlock index buffer
	UnlockIndexBuffer(g_dwBBIdxBufIdx);

	// ok
	return(TRUE);
}

// RenderBillboard()

void RenderBillboard(DWORD dwMatIndex,
					 DWORD dwTexIndex,
					 LPD3DXVECTOR3 lpvPos,
					 float fDiameter)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMatIndex>=g_dwNumMat&&
	   dwMatIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "RenderBillboard()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderBillboard()",
								 FALSE);
		return;
	}
#endif

	// render list of one billboard
	RenderBillboards(dwMatIndex,
					 dwTexIndex,
					 lpvPos,
					 1,
					 fDiameter);
}

// RenderBillboards()

void RenderBillboards(DWORD dwMatIndex,
					  DWORD dwTexIndex,
					  LPD3DXVECTOR3 lpvPos,
					  DWORD dwNumPos,
					  float fDiameter)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMatIndex>=g_dwNumMat&&
	   dwMatIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "RenderBillboards()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderBillboards()",
								 FALSE);
		return;
	}
#endif
	
	// set render states
	SetMaterial(dwMatIndex);
	SetTexture(dwTexIndex);
	SetVertexBuffer(g_dwBBVtxBufIdx);
	SetIndexBuffer(g_dwBBIdxBufIdx);

	// set size
	g_lpMtx[g_dwBBMtxIndex].fXScale=fDiameter;
	g_lpMtx[g_dwBBMtxIndex].fYScale=fDiameter;
	g_lpMtx[g_dwBBMtxIndex].fZScale=fDiameter;

	// render each billboard
	DWORD dwCount;
	for(dwCount=0;dwCount<dwNumPos;dwCount++)
	{
		// set position
		g_lpMtx[g_dwBBMtxIndex].mtx._41=lpvPos[dwCount].x;
		g_lpMtx[g_dwBBMtxIndex].mtx._42=lpvPos[dwCount].y;
		g_lpMtx[g_dwBBMtxIndex].mtx._43=lpvPos[dwCount].z;

		// set world matrix
		SetWorldMatrix(g_dwBBMtxIndex);

		// render billboard
		RenderPrimitive();
	}
}

// RenderReverseBillboard()

void RenderReverseBillboard(DWORD dwMatIndex,
							DWORD dwTexIndex,
							LPD3DXVECTOR3 lpvPos,
							float fDiameter)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMatIndex>=g_dwNumMat&&
	   dwMatIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "RenderReverseBillboard()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderReverseBillboard()",
								 FALSE);
		return;
	}
#endif

	// render list of one reverse billboard
	RenderReverseBillboards(dwMatIndex,
							dwTexIndex,
							lpvPos,
							1,
							fDiameter);
}

// RenderReverseBillboards()

void RenderReverseBillboards(DWORD dwMatIndex,
							 DWORD dwTexIndex,
							 LPD3DXVECTOR3 lpvPos,
							 DWORD dwNumPos,
							 float fDiameter)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMatIndex>=g_dwNumMat&&
	   dwMatIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "RenderReverseBillboards()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderReverseBillboards()",
								 FALSE);
		return;
	}
#endif
	
	// set render states
	SetMaterial(dwMatIndex);
	SetTexture(dwTexIndex);
	SetVertexBuffer(g_dwBBRevVtxBufIdx);
	SetIndexBuffer(g_dwBBIdxBufIdx);

	// set size
	g_lpMtx[g_dwBBMtxIndex].fXScale=fDiameter;
	g_lpMtx[g_dwBBMtxIndex].fYScale=fDiameter;
	g_lpMtx[g_dwBBMtxIndex].fZScale=fDiameter;

	// render each billboard
	DWORD dwCount;
	for(dwCount=0;dwCount<dwNumPos;dwCount++)
	{
		// set position
		g_lpMtx[g_dwBBMtxIndex].mtx._41=lpvPos[dwCount].x;
		g_lpMtx[g_dwBBMtxIndex].mtx._42=lpvPos[dwCount].y;
		g_lpMtx[g_dwBBMtxIndex].mtx._43=lpvPos[dwCount].z;

		// set world matrix
		SetWorldMatrix(g_dwBBMtxIndex);

		// render billboard
		RenderPrimitive();
	}
}

// AddCameraMatrix()

DWORD AddCameraMatrix(float fPosX,
					  float fPosY,
					  float fPosZ,
					  float fTargetX,
					  float fTargetY,
					  float fTargetZ,
					  float fUpX,
					  float fUpY,
					  float fUpZ)
{
	LoadingOutput("AddCameraMatrix");

	// prepare vectors
	D3DXVECTOR3 vPos=D3DXVECTOR3(fPosX,fPosY,fPosZ);
	D3DXVECTOR3 vTarget=D3DXVECTOR3(fTargetX,fTargetY,fTargetZ);
	D3DXVECTOR3 vUp=D3DXVECTOR3(fUpX,fUpY,fUpZ);
	
	// prepare view matrix
	D3DXMATRIX mView;
	D3DXMatrixLookAtLH(&mView,
					   &vPos,
					   &vTarget,
					   &vUp);

	// invert view matrix
	D3DXMATRIX mCam;
	D3DXMatrixInverse(&mCam,
					  NULL,
					  &mView);

	// add matrix
	return(AddMatrix(&mCam));
}

// AddMatrix()

DWORD AddMatrix(float _11,
				float _12,
				float _13,
				float _14,
				float _21,
				float _22,
				float _23,
				float _24,
				float _31,
				float _32,
				float _33,
				float _34,
				float _41,
				float _42,
				float _43,
				float _44)
{
	LoadingOutput("AddMatrix");

	// prepare matrix
	D3DXMATRIX mMtx=D3DXMATRIX(_11,_12,_13,_14,
							   _21,_22,_23,_24,
							   _31,_32,_33,_34,
							   _41,_42,_43,_44);

	// add matrix
	return(AddMatrix(&mMtx));
}

// FindLight()

DWORD FindLight(LPCSTR lpName)
{
	LoadingOutput("FindLight");

	// counter
	DWORD dwCount;

	// search lights
	if(g_dwNumLight)
		for(dwCount=0;dwCount<g_dwNumLight;dwCount++)
			if(stricmp(lpName,g_lplpLightNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// FindMaterial()

DWORD FindMaterial(LPCSTR lpName)
{
	LoadingOutput("FindMaterial");

	// counter
	DWORD dwCount;

	// search materials
	if(g_dwNumMat)
		for(dwCount=0;dwCount<g_dwNumMat;dwCount++)
			if(stricmp(lpName,g_lplpMatNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// FindTexture()

DWORD FindTexture(LPCSTR lpName)
{
	LoadingOutput("FindTexture");

	// counter
	DWORD dwCount;

	// search textures
	if(g_dwNumTex)
		for(dwCount=0;dwCount<g_dwNumTex;dwCount++)
			if(stricmp(lpName,g_lplpTexNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// FindMesh()

DWORD FindMesh(LPCSTR lpName)
{
	LoadingOutput("FindMesh");

	// counter
	DWORD dwCount;

	// search meshes
	if(g_dwNumMesh)
		for(dwCount=0;dwCount<g_dwNumMesh;dwCount++)
			if(stricmp(lpName,g_lplpMeshNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// SetSkyboxOffset()

void SetSkyboxOffset(float fX,
					 float fY,
					 float fZ)
{
	// set offset
	g_vSkyboxOffset=D3DXVECTOR3(fX,fY,fZ);
}

// AddMeshNormals()

void AddMeshNormals(DWORD dwIndex)
{
	LoadingOutput("AddMeshNormals");

	// error-shown flag
	static BOOL bErrorShown=FALSE;

	// used for error checking
	HRESULT hr;
	
	// get fvf flags
	DWORD dwFVF=g_lplpMesh[dwIndex]->GetFVF();

	// check for existing normals
	if(dwFVF&D3DFVF_NORMAL)
		return;

	// add normals to fvf
	dwFVF=dwFVF|D3DFVF_NORMAL;

	// get mesh creation options
	DWORD dwOptions=g_lplpMesh[dwIndex]->GetOptions();

	// create new mesh
	ID3DXMesh* lpMesh;
	hr=g_lplpMesh[dwIndex]->CloneMeshFVF(dwOptions,
										 dwFVF,
										 g_lpD3DDevice,
										 &lpMesh);

	// check for error
	if(FAILED(hr))
	{
		if(!bErrorShown)
		{
			DisplayDXErrorMessage("Unable to clone mesh for normalization.",
								  "Scene may not display properly",
								  hr);
			bErrorShown=TRUE;
		}
		return;
	}
	else
		AddCOMResource;

	// add normals
	hr=D3DXComputeNormals(lpMesh,
						  NULL);

	// check for error
	if(FAILED(hr))
	{
		if(!bErrorShown)
		{
			DisplayDXErrorMessage("Unable to generate mesh normals.",
								  "Scene may not display properly",
								  hr);
			bErrorShown=TRUE;
		}
		ReleaseCOMPtr(lpMesh);
		return;
	}

	// replace old mesh with new one
	ReleaseCOMPtr(g_lplpMesh[dwIndex]);
	g_lplpMesh[dwIndex]=lpMesh;
}

// LockBackBuffer()

D3DLOCKED_RECT LockBackBuffer(void)
{
	// error-shown flag
	static BOOL bErrorShown=FALSE;

	// return value
	D3DLOCKED_RECT lr;
	InitStruct(lr);

	// get surface
	HRESULT hr=g_lpD3DDevice->GetBackBuffer(0,
											D3DBACKBUFFER_TYPE_MONO,
											&g_lpBackBuffer);

	// check for error
	if(FAILED(hr))
	{
		if(!bErrorShown)
		{
			DisplayDXErrorMessage("Unable to retrieve back buffer surface.",
								  "Scene may not display correctly.",
								  hr);
			bErrorShown=TRUE;
		}
		return(lr);
	}
	else
		AddCOMResource;
	
	// lock buffer
	hr=g_lpBackBuffer->LockRect(&lr,
								NULL,
								D3DLOCK_NO_DIRTY_UPDATE);

	// check for error
	if(FAILED(hr))
	{
		if(!bErrorShown)
		{
			DisplayDXErrorMessage("Unable to lock back buffer.",
								  "Scene may not display correctly.",
								  hr);
			bErrorShown=TRUE;
		}
		return(lr);
	}

	// return locked rect
	return(lr);
}

// UnlockBackBuffer()

void UnlockBackBuffer(void)
{
	// unlock buffer
	g_lpBackBuffer->UnlockRect();

	// release surface
	ReleaseCOMPtr(g_lpBackBuffer);
}

// ResetGraphicsObjects()

BOOL ResetGraphicsObjects(void)
{
	LoadingOutput("ResetGraphicsObjects");

	// purge all previous objects
	PurgeGraphicsObjects();

	// init necessary objects
	if(!InitSprites())
		return(FALSE);
	if(!InitAlphaMeshes())
		return(FALSE);
	if(!InitFader())
		return(FALSE);
	if(!InitSkybox())
		return(FALSE);
	if(!InitBillboards())
		return(FALSE);
#ifdef USE_TRANSITIONS
	if(!InitTransitions())
		return(FALSE);
#endif
	if(!InitMouseCursor())
		return(FALSE);
#ifdef DEBUG_MODE
	if(!InitLineRender())
		return(FALSE);
#endif

	// reset skybox
	SetSkybox(75);

	// ok
	return(TRUE);
}

// ResetRenderDevice()

void ResetRenderDevice(void)
{
	// verify device
	if(!g_lpD3DDevice)
		return;

	// do not reset if closing
	if(g_bClosing)
		return;
	
	// used for error checking
	HRESULT hr;
	
	// do nothing if device is not operational
	if(g_lpD3DDevice->TestCooperativeLevel()==D3DERR_DEVICELOST)
		return;

	// update window metrics
	UpdateWindowMetrics();

	// set presentation parameters
	if(!SetPresentationParameters())
		return;

	// reset device
	hr=g_lpD3DDevice->Reset(&g_pp);

	// check for error
	if(FAILED(hr))
		return;

	// restore surface mapper if necessary
	if(g_lpRTS)
	{
		// reset mapper
		hr=g_lpRTS->OnResetDevice();
		
		// check for error
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to reset surface mapper.",
								  NULL,
								  hr);
	}

	// update client area
	UpdateClientArea();

	// restore device settings
	ResetRenderEnvironment();

	// clear screen
	ClearScreen();
}

// InitDirect3D()

BOOL InitDirect3D(void)
{
	LoadingOutput("InitDirect3D");

	// create d3d interface
	g_lpD3D=Direct3DCreate8(D3D_SDK_VERSION);
	
	// check for error
	if(!g_lpD3D)
	{
		DisplayDXErrorMessage("Unable to create Direct3D interface.",
						      "Make sure you are running the current version of DirectX.");
		return(FALSE);
	}
	else
		AddCOMResource;

	// get display adapter info
	if(!GetDisplayAdapterInfo())
		return(FALSE);

	// get display mode
	g_dmSys.fmt=GetSystemDisplayFormat();

	// save original display mode
	g_dmOrigSys=g_dmSys;

	// ok
	return(TRUE);
}

// ReleaseDirect3D()

void ReleaseDirect3D(void)
{
	// release display data
	ReleaseResolutions();
	ReleaseRenderModes();
	
	// release direct3d
	ReleaseCOMPtr(g_lpD3D);
}

// GetDisplayAdapterInfo()

BOOL GetDisplayAdapterInfo(void)
{
	LoadingOutput("GetDisplayAdapterInfo");

	// used for error checking
	HRESULT hr;
	
	// get adapter id info
	hr=g_lpD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT,
									 NULL,
									 &g_lpAdapterID);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to retrieve display adapter ID information.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// get device caps
	hr=g_lpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,
							  D3DDEVTYPE_HAL,
							  &g_lpDevCaps);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to retrieve display adapter capabilities.",
							  g_lpAdapterID.Description,
							  hr);
		return(FALSE);
	}

	// get available render modes
	if(!InitRenderModes(D3DADAPTER_DEFAULT))
		return(FALSE);
	
	// get available resolutions
	if(!InitResolutions())
		return(FALSE);

	// check required device caps
	if(!VerifyVideoDeviceCaps())
		return(FALSE);

	// ok
	return(TRUE);
}
	
// VerifyWindowedRendering()

BOOL VerifyWindowedRendering(void)
{
	LoadingOutput("VerifyWindowedRendering");

	// reset windowed rendering flag
	g_bWindowedOK=TRUE;
	
	// check device for windowed rendering
	if(!(g_lpDevCaps.Caps2&D3DCAPS2_CANRENDERWINDOWED))
		g_bWindowedOK=FALSE;

	// get application display mode
	g_dmApp=ApplicationDisplayMode();
	
	// check display mode for windowed rendering
	if(!IsModeRenderable(&g_dmSys))
		g_bWindowedOK=FALSE;

	// check display resolution for windowed rendering
	if(g_dmApp.dwX>=g_dmSys.dwX&&
	   g_dmApp.dwY>=g_dmSys.dwY)
	    g_bWindowedOK=FALSE;

#ifdef USE_AVWARNINGS
	// check windowed flag
	if(g_dmApp.bWindowed&&
	   !g_bWindowedOK)
	{
		// check registry for flag
		LPSTR lpFlag=GetRegistryKey("DXNoWinMsg");
		if(!lpFlag)
		{
			// display message box
			DisplayInfoMessage("Your current display settings do not allow windowed execution.\n\n"
							   "Switching to full-screen mode.\n\nThis message will not be displayed again.");

			// set flag
			SetRegistryKey("DXNoWinMsg",
						   "TRUE");
		}
		else
		{
			// free key value
			FreeRegistryKey(lpFlag);
		}
	}
#endif

	// ok
	return(TRUE);
}

// SetPresentationParameters()

BOOL SetPresentationParameters(void)
{
	LoadingOutput("SetPresentationParameters");

	// verify windowed rendering
	if(!VerifyWindowedRendering())
		return(FALSE);

	// reset structure
	InitStruct(g_pp);

	// set presentation parameters
	if(g_dmApp.bWindowed&&
	   g_bWindowedOK)
	{
		// set display mode values
		g_dmApp.fmt=g_dmSys.fmt;
		g_dmApp.bWindowed=TRUE;
		
		// set windowed presentation parameters
		g_pp.BackBufferFormat=g_dmApp.fmt;
		g_pp.BackBufferCount=g_dmApp.dwBackBufferCount;
		g_pp.MultiSampleType=D3DMULTISAMPLE_NONE;
		g_pp.SwapEffect=D3DSWAPEFFECT_DISCARD;
		g_pp.hDeviceWindow=g_hWnd;
		g_pp.Windowed=TRUE;
		g_pp.EnableAutoDepthStencil=TRUE;
		g_pp.AutoDepthStencilFormat=D3DFMT_D16;
		if(g_dmApp.bLockable)
			g_pp.Flags=D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	}
	else
	{
		// set values of desired display mode
		if(!SetValidRGBFormat(&g_dmApp))
		{
			DisplayErrorMessage("Unable to find suitable render formats.",
								"Your display adapter does not support a required format.",
								FALSE);
			return(FALSE);
		}
		g_dmApp.bWindowed=FALSE;

		// full-screen parameters
		g_pp.BackBufferWidth=g_dmApp.dwX;
		g_pp.BackBufferHeight=g_dmApp.dwY;
		g_pp.BackBufferFormat=g_dmApp.fmt;
		g_pp.BackBufferCount=g_dmApp.dwBackBufferCount;
		g_pp.MultiSampleType=D3DMULTISAMPLE_NONE;
		g_pp.SwapEffect=D3DSWAPEFFECT_DISCARD;
		g_pp.hDeviceWindow=g_hWnd;
		g_pp.Windowed=FALSE;
		g_pp.EnableAutoDepthStencil=TRUE;
		g_pp.AutoDepthStencilFormat=D3DFMT_D16;
		if(g_dmApp.bLockable)
			g_pp.Flags=D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		g_pp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
		g_pp.FullScreen_PresentationInterval=D3DPRESENT_INTERVAL_ONE;
	}

	// ok
	return(TRUE);
}

// InitD3DDevice()

BOOL InitD3DDevice(void)
{
	LoadingOutput("InitD3DDevice");

	// used for error checking
	HRESULT hr;
	
	// set presentation parameters
	if(!SetPresentationParameters())
		return(FALSE);

	// check windowed flag
	if(g_pp.Windowed)
	{
		// update window size & position
		UpdateWindowMetrics();
	}

	// show window before creating device
	ShowWindow(g_hWnd,
			   SW_SHOW);

	// create d3d device
	DWORD dwAttempt=0;
	while(dwAttempt<3)
	{
		// set behavior & vertex processing flags
		DWORD dwBehaviorFlags;
		switch(dwAttempt)
		{
		case(0):
#ifdef ALLOW_HWONLYVP
			// set hardware processing
			dwBehaviorFlags=D3DCREATE_HARDWARE_VERTEXPROCESSING;
			g_dwVPF=VPF_HARDWARE;
#else
			// set mixed processing
			dwBehaviorFlags=D3DCREATE_MIXED_VERTEXPROCESSING;
			g_dwVPF=VPF_MIXED;
#endif
			break;
		case(1):
			// set mixed processing
			dwBehaviorFlags=D3DCREATE_MIXED_VERTEXPROCESSING;
			g_dwVPF=VPF_MIXED;
			break;
		case(2):
			// set software processing
			dwBehaviorFlags=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			g_dwVPF=VPF_SOFTWARE;
			break;
		}

		// attempt to create device
		hr=g_lpD3D->CreateDevice(D3DADAPTER_DEFAULT,
								 D3DDEVTYPE_HAL,
								 g_hWnd,
								 dwBehaviorFlags,
								 &g_pp,
								 &g_lpD3DDevice);

		// increment attempt counter
		if(FAILED(hr))
			dwAttempt++;
		else
			dwAttempt=3;
	}

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create Direct3D device object.",
							  "Your display adapter may not support a required format.",
							  hr);
		return(FALSE);
	}
	else
		AddCOMResource;

	// get client area
	UpdateClientArea();

	// set default viewport
	SetViewport();

	// set render states
	if(!SetRenderStates())
		return(FALSE);

	// reset graphics objects
	if(!ResetGraphicsObjects())
		return(FALSE);

#ifdef USE_TRANSITIONS
	// init shatter transition
	if(!InitShatter())
		return(FALSE);
#endif

	// set default environment settings
	EnableZBuffer();
	DisableWireframe();
#ifdef DEBUG_MODE
	SetLinePattern(1,0xFFFF);
#endif
	EnableZBufferWrites();
	EnableBackfaceCull();
	SetZBias(0);
	DisableAlphaBlend();
	DisableAlphaTest();
	DisableAlphaTextures();
	DisableFog();
	DisableSpecular();
	DisableGouraud();
	DisableLinearFilter();

	// clear display buffers
	ClearDisplayBuffers();

	// set default matrices
	SetDefaultProjectionMatrix();
	SetDefaultViewMatrix();

	// ok
	return(TRUE);
}

// ReleaseD3DDevice()

void ReleaseD3DDevice(void)
{
	// clear display buffers
	if(!g_pp.Windowed)
		ClearDisplayBuffers();

#ifdef USE_TRANSITIONS
	// release shatter transition
	ReleaseShatter();
#endif
	
	// release device
	ReleaseCOMPtr(g_lpD3DDevice);
}

// UpdateClientArea()

void UpdateClientArea(void)
{
	// check device
	if(g_lpD3DDevice)
	{
		// check full-screen flag
		if(!g_pp.Windowed)
		{
			// use current render target settings
			g_rClient.dwX=g_dmApp.dwX;
			g_rClient.dwY=g_dmApp.dwY;
			g_rClient.fX=(float)g_rClient.dwX;
			g_rClient.fY=(float)g_rClient.dwY;

			// set current render size values
			g_fCurRenderX=g_rClient.fX;
			g_fCurRenderY=g_rClient.fY;

			// ok
			return;
		}
	}

	// use system client rect
	RECT rC;
	GetClientRect(g_hWnd,&rC);

	// copy values
	g_rClient.dwX=rC.right-rC.left;
	g_rClient.dwY=rC.bottom-rC.top;
	g_rClient.fX=(float)g_rClient.dwX;
	g_rClient.fY=(float)g_rClient.dwY;

	// set current render size values
	g_fCurRenderX=g_rClient.fX;
	g_fCurRenderY=g_rClient.fY;
}

// SetAlphaMode()

BOOL SetAlphaMode(void)
{
	LoadingOutput("SetAlphaMode");

	// used for error checking
	HRESULT hr;
	
	// set source alpha blending value
	hr=g_lpD3DDevice->SetRenderState(D3DRS_SRCBLEND,
									 D3DBLEND_SRCALPHA);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set alpha blending source value.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// set destination alpha blending value
	hr=g_lpD3DDevice->SetRenderState(D3DRS_DESTBLEND,
									 D3DBLEND_INVSRCALPHA);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set alpha blending destination value.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// set alpha reference value
	hr=g_lpD3DDevice->SetRenderState(D3DRS_ALPHAREF,
									 ALPHA_REF);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set alpha blending reference value.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// set alpha comparison function
	hr=g_lpD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,
									 D3DCMP_GREATER);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set alpha blending comparison value.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SetFogMode()

BOOL SetFogMode(void)
{
	LoadingOutput("SetFogMode");

	// used for error checking
	HRESULT hr;
	
	// set fog mode
	hr=g_lpD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE,
									 D3DFOG_LINEAR);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set fog mode.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SetTextureAddressingMode()

BOOL SetTextureAddressingMode(void)
{
	// used for error checking
	HRESULT hr;
	
	// set texture u-addressing mode
	hr=g_lpD3DDevice->SetTextureStageState(0,
										   D3DTSS_ADDRESSU,
										   D3DTADDRESS_WRAP);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set texture addressing u-value.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// set texture v-addressing mode
	hr=g_lpD3DDevice->SetTextureStageState(0,
										   D3DTSS_ADDRESSV,
										   D3DTADDRESS_WRAP);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set texture addressing v-value.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SetRenderStates()

extern BOOL SetRenderStates(void)
{
	LoadingOutput("SetRenderStates");

	// set alpha mode
	if(ApplicationAlphaRequired())
		if(!SetAlphaMode())
			return(FALSE);

	// set fog mode
	if(ApplicationFogRequired())
		if(!SetFogMode())
			return(FALSE);

	// set texture addressing mode
	if(!SetTextureAddressingMode())
		return(FALSE);

	// set vertex processing mode
	if(!SetVertexProcessingMode())
		return(FALSE);

	// ok
	return(TRUE);
}

// SetVertexProcessingMode()

BOOL SetVertexProcessingMode(void)
{
	LoadingOutput("SetVertexProcessingMode");

	// set vertex processing as needed
	switch(g_dwVPF)
	{
	case(VPF_HARDWARE):
		DisableSVP();
		break;
	case(VPF_SOFTWARE):
		EnableSVP();
		break;
	case(VPF_MIXED):
		DisableSVP();
		break;
	}

	// ok
	return(TRUE);
}

// ResetRenderEnvironment()

void ResetRenderEnvironment(void)
{
	LoadingOutput("ResetRenderEnvironment");

	// counter
	DWORD dwCount;
	
	// set render states
	SetRenderStates();

	// reset alpha blend
	if(g_bABlend)
		EnableAlphaBlend();
	else
		DisableAlphaBlend();

	// reset alpha test
	if(g_bATest)
		EnableAlphaTest();
	else
		DisableAlphaTest();

	// reset specular
	if(g_bSpecular)
		EnableSpecular();
	else
		DisableSpecular();

	// reset fog
	if(g_bFog)
		EnableFog();
	else
		DisableFog();

	// reset fog parameters
	SetFogColor(g_clrFog);
	SetFogStart(g_fFogStart);
	SetFogEnd(g_fFogEnd);

	// reset gouraud
	if(g_bGouraud)
		EnableGouraud();
	else
		DisableGouraud();

	// reset filtering mode
	if(g_bLinearFilter)
		EnableLinearFilter();
	else
		DisableLinearFilter();

	// reset line pattern
	SetLinePattern(g_wLPRepeat,
				   g_wLPPattern);

	// reset wireframe
	if(g_bWireframe)
		EnableWireframe();
	else
		DisableWireframe();

	// reset z-buffer
	if(g_bZBufferEnabled)
		EnableZBuffer();
	else
		DisableZBuffer();

	// reset z-buffer writes
	if(g_bZBufferWrites)
		EnableZBufferWrites();
	else
		DisableZBufferWrites();

	// reset z-bias
	SetZBias(g_dwZBias);

	// reset cull mode
	if(g_bBackfaceCull==CULL_BF)
		EnableBackfaceCull();
	else if(g_bBackfaceCull==CULL_FF)
		EnableFrontfaceCull();
	else
		DisableBackfaceCull();

	// reset software vertex processing
	if(g_bSVPEnabled)
		EnableSVP();
	else
		DisableSVP();

	// reset view & projection matrices
	SetViewMatrix(&g_mtxView);
	SetProjectionMatrix(&g_mtxProj);

	// reset ambient light
	SetAmbientLight(g_clrAmbient);

	// check lights
	if(g_dwNumLight)
	{
		// restore all lights
		for(dwCount=0;dwCount<g_dwNumLight;dwCount++)
		{
			// update light
			UpdateLight(dwCount,
						&g_lpLight[dwCount]);

			// enable light as needed
			if(g_lpbLightEnabled[dwCount])
				EnableLight(dwCount);
			else
				DisableLight(dwCount);
		}
	}

	// reset viewport
	ResetViewport();

	// reset fader vertex buffer
	ResetFaderVertexBuffer();

	// reset alpha textures
	if(g_bAlphaTextures)
		EnableAlphaTextures();
	else
		DisableAlphaTextures();
}

#ifdef DEBUG_MODE
// ValidateLights()

void ValidateLights(void)
{
	// counter
	DWORD dwCount;

	// check light validations
	if(g_dwNumLight)
		for(dwCount=0;dwCount<g_dwNumLight;dwCount++)
			if(g_lpbLightEnabled[dwCount]&&
			   !g_lpbLightValid[dwCount])
				DisplayErrorMessage("Warning: Light has not been validated.",
									g_lplpLightNames[dwCount],
									FALSE);
}
#endif

// ResetSpriteProjectionMatrix()

void ResetSpriteProjectionMatrix(void)
{
	// create & store matrix
	D3DXMatrixOrthoLH(&g_mtxSpriteProj,
					  g_fCurRenderX,
					  g_fCurRenderY,
					  0,1);
}
	
// ScaleRectToClient()

void ScaleRectToClient(LPRECT lpR)
{
	// scale each component
	lpR->left=  ClientCoordX(lpR->left);
	lpR->right= ClientCoordX(lpR->right);
	lpR->top=   ClientCoordY(lpR->top);
	lpR->bottom=ClientCoordY(lpR->bottom);
}

// SetViewport()

void SetViewport(float fX,
				 float fY,
				 float fWidth,
				 float fHeight,
				 float fZNear,
				 float fZFar)
{
	// save viewport values
	g_vp.x=fX;
	g_vp.y=fY;
	g_vp.w=fWidth;
	g_vp.h=fHeight;
	g_vp.n=fZNear;
	g_vp.f=fZFar;

	// set viewport structure
	D3DVIEWPORT8 vp;
	vp.X=     (DWORD)(fX*     g_fCurRenderX);
	vp.Y=     (DWORD)(fY*     g_fCurRenderY);
	vp.Width= (DWORD)(fWidth* g_fCurRenderX);
	vp.Height=(DWORD)(fHeight*g_fCurRenderY);
	vp.MinZ=fZNear;
	vp.MaxZ=fZFar;
	
	// set viewport
	g_lpD3DDevice->SetViewport(&vp);
}

// ResetViewport()

void ResetViewport(void)
{
	// set stored viewport
	SetViewport(g_vp.x,
				g_vp.y,
				g_vp.w,
				g_vp.h,
				g_vp.n,
				g_vp.f);
}

// GetSystemDisplayFormat()

D3DFORMAT GetSystemDisplayFormat(void)
{
	LoadingOutput("GetSystemDisplayFormat");

	// used for error checking
	HRESULT hr;
	
	// check direct3d
	if(!g_lpD3D)
		return(D3DFMT_UNKNOWN);
	
	// get adapter display mode
	D3DDISPLAYMODE dm;
	hr=g_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,
									  &dm);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to retrieve display adapter mode.",
							  g_lpAdapterID.Description,
							  hr);
		return(D3DFMT_UNKNOWN);
	}
	
	// return format
	return(dm.Format);
}

// LockMesh()

LPLOCKEDMESH LockMesh(DWORD dwIndex)
{
	LoadingOutput("LockMesh");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "LockMesh()",
								 FALSE);
		return(NULL);
	}
#endif
	
	// counter
	DWORD dwCount;

	// used for error checking
	HRESULT hr;
	
	// pointer to return
	LPLOCKEDMESH lpm;

	// allocate locked mesh structure
	if(!AllocMem((LPVOID*)&lpm,
				 sizeof(LOCKEDMESH)))
		return(NULL);

	// get number of vertices in mesh
	lpm->dwNumVtx=g_lplpMesh[dwIndex]->GetNumVertices();

	// allocate vertex array
	if(!AllocMem((LPVOID*)&lpm->lpVtx,
				 sizeof(D3DXVECTOR3)*lpm->dwNumVtx))
		return(NULL);

	// compute size of vertex
	lpm->dwVtxSize=ComputeVertexSize(g_lplpMesh[dwIndex]->GetFVF())/sizeof(float);

	// lock vertex buffer
	hr=g_lplpMesh[dwIndex]->LockVertexBuffer(NULL,
											 (LPBYTE*)&lpm->lpfVtxBuf);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to lock mesh vertex buffer.",
							  "Check available memory.",
							  hr);
		return(NULL);
	}
	
	// read vertices
	for(dwCount=0;dwCount<lpm->dwNumVtx;dwCount++)
	{
		lpm->lpVtx[dwCount].x=lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)];
		lpm->lpVtx[dwCount].y=lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)+1];
		lpm->lpVtx[dwCount].z=lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)+2];
	}

	// get number of faces in mesh
	lpm->dwNumFaces=g_lplpMesh[dwIndex]->GetNumFaces();

	// get index buffer
	IDirect3DIndexBuffer8* lpidx;
	hr=g_lplpMesh[dwIndex]->GetIndexBuffer(&lpidx);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to retrieve mesh index buffer.",
							  NULL,
							  hr);
		return(NULL);
	}

	// get buffer description
	D3DINDEXBUFFER_DESC ibd;
	hr=lpidx->GetDesc(&ibd);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to retrieve mesh index buffer description.",
							  NULL,
							  hr);
		return(NULL);
	}

	// set index size
	if(ibd.Format==D3DFMT_INDEX16)
		lpm->dwIdxSize=sizeof(WORD);
	else if(ibd.Format==D3DFMT_INDEX32)
		lpm->dwIdxSize=sizeof(DWORD);
	else
	{
		DisplayDXErrorMessage("Invalid mesh index buffer format.",
							  NULL,
							  hr);
		return(NULL);
	}

	// lock index buffer
	hr=lpidx->Lock(0,
				   0,
				   &lpm->lpcIdxBuf,
				   NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to lock mesh index buffer.",
							  NULL,
							  hr);
		return(NULL);
	}

	// check index size
	if(lpm->dwIdxSize==2)
	{
		// allocate memory for faces
		if(!AllocMem((LPVOID*)&lpm->lpFaces,
					 sizeof(MESHFACE)*lpm->dwNumFaces))
			return(NULL);

		// cast pointer to faces
		LPWORD lpwFaces=(LPWORD)lpm->lpcIdxBuf;

		// reset index counter
		DWORD dwIdxCount=0;
		
		// copy faces
		for(dwCount=0;dwCount<lpm->dwNumFaces;dwCount++)
		{
			lpm->lpFaces[dwCount].dwV1=(DWORD)lpwFaces[dwIdxCount++];
			lpm->lpFaces[dwCount].dwV2=(DWORD)lpwFaces[dwIdxCount++];
			lpm->lpFaces[dwCount].dwV3=(DWORD)lpwFaces[dwIdxCount++];
		}
	}
	else
		// cast pointer to faces
		lpm->lpFaces=(LPMESHFACE)lpm->lpcIdxBuf;

	// return pointer
	return(lpm);
}

// UnlockMesh()

void UnlockMesh(DWORD dwIndex,
				LPLOCKEDMESH lpm)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "UnlockMesh()",
								 FALSE);
		return;
	}
#endif
	
	// counter
	DWORD dwCount;

	// used for error checking
	HRESULT hr;
	
	// replace vertices
	for(dwCount=0;dwCount<lpm->dwNumVtx;dwCount++)
	{
		lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)]=lpm->lpVtx[dwCount].x;
		lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)+1]=lpm->lpVtx[dwCount].y;
		lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)+2]=lpm->lpVtx[dwCount].z;
	}

	// unlock vertex buffer
	hr=g_lplpMesh[dwIndex]->UnlockVertexBuffer();

	// check for error
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to unlock mesh vertex buffer.",
							  NULL,
							  hr);

	// check index size
	if(lpm->dwIdxSize==2)
	{
		// cast pointer to faces
		LPWORD lpwFaces=(LPWORD)lpm->lpcIdxBuf;

		// reset index counter
		DWORD dwIdxCount=0;
		
		// replace faces
		for(dwCount=0;dwCount<lpm->dwNumFaces;dwCount++)
		{
			lpwFaces[dwIdxCount++]=(WORD)lpm->lpFaces[dwCount].dwV1;
			lpwFaces[dwIdxCount++]=(WORD)lpm->lpFaces[dwCount].dwV2;
			lpwFaces[dwIdxCount++]=(WORD)lpm->lpFaces[dwCount].dwV3;
		}

		// free faces memory
		FreeMem((LPVOID*)&lpm->lpFaces);
	}

	// unlock index buffer
	hr=g_lplpMesh[dwIndex]->UnlockIndexBuffer();

	// check for error
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to unlock index buffer.",
							  NULL,
							  hr);

	// free memory objects
	FreeMem((LPVOID*)&lpm->lpVtx);
	FreeMem((LPVOID*)&lpm);
}

// OffsetMesh()

BOOL OffsetMesh(DWORD dwIndex,
				float fOffsetX,
				float fOffsetY,
				float fOffsetZ)
{
	LoadingOutput("OffsetMesh");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "OffsetMesh()",
								 FALSE);
		return(FALSE);
	}
#endif

	// counter
	DWORD dwCount;

	// lock mesh
	LPLOCKEDMESH lpm=LockMesh(dwIndex);

	// check pointer
	if(!lpm)
		return(FALSE);

	// offset vertices
	for(dwCount=0;dwCount<lpm->dwNumVtx;dwCount++)
	{
		lpm->lpVtx[dwCount].x+=fOffsetX;
		lpm->lpVtx[dwCount].y+=fOffsetY;
		lpm->lpVtx[dwCount].z+=fOffsetZ;
	}

	// unlock mesh
	UnlockMesh(dwIndex,
			   lpm);

	// ok
	return(TRUE);
}

// TransformVector()

void TransformVector(LPD3DXVECTOR3 lpv,
					 LPD3DXVECTOR3 lpv1,
					 LPD3DXMATRIX lpmtx)
{
	// output vector
	D3DXVECTOR4 v;
	
	// transform vector
	D3DXVec3Transform(&v,
					  lpv1,
					  lpmtx);

	// copy values
	lpv->x=v.x;
	lpv->y=v.y;
	lpv->z=v.z;
}

// InvertMesh()

BOOL InvertMesh(DWORD dwIndex)
{
	LoadingOutput("InvertMesh");

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "InvertMesh()",
								 FALSE);
		return(FALSE);
	}
#endif

	// counter
	DWORD dwCount;
	
	// lock mesh
	LPLOCKEDMESH lm=LockMesh(dwIndex);

	// check pointer
	if(!lm)
		return(FALSE);

	// invert each face
	for(dwCount=0;dwCount<lm->dwNumFaces;dwCount++)
	{
		// swap first two indices
		DWORD dwTemp=lm->lpFaces[dwCount].dwV1;
		lm->lpFaces[dwCount].dwV1=lm->lpFaces[dwCount].dwV2;
		lm->lpFaces[dwCount].dwV2=dwTemp;
	}

	// unlock mesh
	UnlockMesh(dwIndex,
			   lm);

	// ok
	return(TRUE);
}

// GetPlaneNormal()

BOOL GetPlaneNormal(LPD3DXVECTOR3 lpvN,
					LPD3DXVECTOR3 lpv1,
					LPD3DXVECTOR3 lpv2,
					LPD3DXVECTOR3 lpv3)
{
	// compute vectors from point 2
	D3DXVECTOR3 vVec1=(*lpv3)-(*lpv2);
	D3DXVECTOR3 vVec2=(*lpv1)-(*lpv2);

	// compute cross product
	D3DXVECTOR3 vVec3;
	CrossProduct(&vVec3,&vVec1,&vVec2);

	// check for invalid normal
	if(IsNearZero(vVec3.x)&&
	   IsNearZero(vVec3.y)&&
	   IsNearZero(vVec3.z))
		return(FALSE);

	// normalize vector
	Normalize(lpvN,
			  &vVec3);

	// ok
	return(TRUE);
}

// IsVectorToLeft()

BOOL IsVectorToLeft(LPD3DXVECTOR3 lpV,
				    LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->x<lpV1->x)
		return(TRUE);
	else
		return(FALSE);
}

// IsVectorToRight()

BOOL IsVectorToRight(LPD3DXVECTOR3 lpV,
				     LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->x>lpV1->x)
		return(TRUE);
	else
		return(FALSE);
}

// IsVectorInFront()

BOOL IsVectorInFront(LPD3DXVECTOR3 lpV,
				     LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->z>lpV1->z)
		return(TRUE);
	else
		return(FALSE);
}

// IsVectorBehind()

BOOL IsVectorBehind(LPD3DXVECTOR3 lpV,
				    LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->z<lpV1->z)
		return(TRUE);
	else
		return(FALSE);
}

// IsVectorAbove()

BOOL IsVectorAbove(LPD3DXVECTOR3 lpV,
				   LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->y>lpV1->y)
		return(TRUE);
	else
		return(FALSE);
}

// IsVectorBelow()

BOOL IsVectorBelow(LPD3DXVECTOR3 lpV,
				   LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->y<lpV1->y)
		return(TRUE);
	else
		return(FALSE);
}

// ClipPolygonToPlane()

void ClipPolygonToPlane(LPPOLYGON lpPoly,
					    LPD3DXVECTOR3 lpv1,
					    LPD3DXVECTOR3 lpv2,
					    LPD3DXVECTOR3 lpv3)
{
	LoadingOutput("ClipPolygonToPlane");

	// do not process empty polygons
	if(lpPoly->dwNumVtx==0)
		return;
	
	// get plane normal
	D3DXVECTOR3 vN;
	if(!GetPlaneNormal(&vN,
					   lpv1,
					   lpv2,
				       lpv3))
#ifdef DEBUG_MODE
	{
		DisplayErrorMessage("Undefined plane.",
							NULL,
							FALSE);
		return;
	}
#else
		return;
#endif

	// compute d-value
	float fD=-DotProduct(&vN,
						 lpv1);

	// counters
	DWORD dwCount1;
	DWORD dwCount2;
	
	// reset output polygon
	POLYGON pgOut;
	pgOut.dwNumVtx=0;

	// set counter #1 to last vertex
	dwCount1=lpPoly->dwNumVtx-1;

	// check each edge
	for(dwCount2=0;dwCount2<lpPoly->dwNumVtx;dwCount2++)
	{		
		// get edge endpoints
		D3DXVECTOR3 v1=lpPoly->lpvVtx[dwCount1];
		D3DXVECTOR3 v2=lpPoly->lpvVtx[dwCount2];

		// compute flag of point #1
		BOOL bInside1;
		if((DotProduct(&vN,&v1)+fD)<=0)
			bInside1=TRUE;
		else
			bInside1=FALSE;

		// compute flag of point #2
		BOOL bInside2;
		if((DotProduct(&vN,&v2)+fD)<=0)
			bInside2=TRUE;
		else
			bInside2=FALSE;

		// check edge type
		if(bInside1)
		{
			if(bInside2)
			{
				// entirely inside
				pgOut.lpvVtx[pgOut.dwNumVtx++]=v2;
			}
			else
			{
				// inside to outside
				D3DXVECTOR3 vI;
				Get3DLinePlaneIntersection(&vI,
										   &v1,
										   &v2,
										   lpv1,
										   lpv2,
										   lpv3);

				// copy value
				pgOut.lpvVtx[pgOut.dwNumVtx++]=vI;
			}
		}
		else
		{
			if(bInside2)
			{
				// outside to inside
				D3DXVECTOR3 vI;
				Get3DLinePlaneIntersection(&vI,
										   &v1,
										   &v2,
										   lpv1,
										   lpv2,
										   lpv3);

				// copy values
				pgOut.lpvVtx[pgOut.dwNumVtx++]=vI;
				pgOut.lpvVtx[pgOut.dwNumVtx++]=v2;
			}
			else
			{
				// entirely outside, do nothing
			}
		}
		
		// update counter #1
		dwCount1=dwCount2;
	}

	// copy clipped polygon to original
	(*lpPoly)=pgOut;
}

// DoesTriangleIntersectBox()

BOOL DoesTriangleIntersectBox(LPD3DXVECTOR3 lpv1,
							  LPD3DXVECTOR3 lpv2,
							  LPD3DXVECTOR3 lpv3,
							  LPD3DXVECTOR3 lpvMin,
							  LPD3DXVECTOR3 lpvMax)
{
	// set up polygon
	POLYGON pgTri;
	pgTri.dwNumVtx=3;
	pgTri.lpvVtx[0]=(*lpv1);
	pgTri.lpvVtx[1]=(*lpv2);
	pgTri.lpvVtx[2]=(*lpv3);

	// set up box vertices
	D3DXVECTOR3 v1;
	v1.x=lpvMin->x;
	v1.y=lpvMin->y;
	v1.z=lpvMin->z;
	D3DXVECTOR3 v2;
	v2.x=lpvMax->x;
	v2.y=lpvMin->y;
	v2.z=lpvMin->z;
	D3DXVECTOR3 v3;
	v3.x=lpvMax->x;
	v3.y=lpvMin->y;
	v3.z=lpvMax->z;
	D3DXVECTOR3 v4;
	v4.x=lpvMin->x;
	v4.y=lpvMin->y;
	v4.z=lpvMax->z;
	D3DXVECTOR3 v5;
	v5.x=lpvMin->x;
	v5.y=lpvMax->y;
	v5.z=lpvMin->z;
	D3DXVECTOR3 v6;
	v6.x=lpvMax->x;
	v6.y=lpvMax->y;
	v6.z=lpvMin->z;
	D3DXVECTOR3 v7;
	v7.x=lpvMax->x;
	v7.y=lpvMax->y;
	v7.z=lpvMax->z;
	D3DXVECTOR3 v8;
	v8.x=lpvMin->x;
	v8.y=lpvMax->y;
	v8.z=lpvMax->z;

	// clip polygon to each plane
	ClipPolygonToPlane(&pgTri,&v1,&v4,&v8);
	ClipPolygonToPlane(&pgTri,&v2,&v7,&v3);
	ClipPolygonToPlane(&pgTri,&v1,&v5,&v6);
	ClipPolygonToPlane(&pgTri,&v8,&v4,&v3);
	ClipPolygonToPlane(&pgTri,&v1,&v2,&v3);
	ClipPolygonToPlane(&pgTri,&v5,&v8,&v7);

	// get centroid of clipped polygon
	D3DXVECTOR3 vC;
	if(GetPolygonCentroid(&vC,&pgTri))
	{
		// check for enclosure of centroid
		if(IsPointInBox(&vC,
						lpvMin,
						lpvMax))
			return(TRUE);
	}

	// no intersection found
	return(FALSE);
}

// Get3DLinePlaneIntersection()

BOOL Get3DLinePlaneIntersection(LPD3DXVECTOR3 lpvI,
								LPD3DXVECTOR3 lpvL1,
								LPD3DXVECTOR3 lpvL2,
								LPD3DXVECTOR3 lpvP1,
								LPD3DXVECTOR3 lpvP2,
								LPD3DXVECTOR3 lpvP3)
{
	// compute plane equation
	float fA=(lpvP1->y*(lpvP2->z-lpvP3->z))+
			 (lpvP2->y*(lpvP3->z-lpvP1->z))+
			 (lpvP3->y*(lpvP1->z-lpvP2->z));
	float fB=(lpvP1->z*(lpvP2->x-lpvP3->x))+
			 (lpvP2->z*(lpvP3->x-lpvP1->x))+
			 (lpvP3->z*(lpvP1->x-lpvP2->x));
	float fC=(lpvP1->x*(lpvP2->y-lpvP3->y))+
			 (lpvP2->x*(lpvP3->y-lpvP1->y))+
			 (lpvP3->x*(lpvP1->y-lpvP2->y));
	float fD=-(lpvP1->x*((lpvP2->y*lpvP3->z)-(lpvP3->y*lpvP2->z)))
			 -(lpvP2->x*((lpvP3->y*lpvP1->z)-(lpvP1->y*lpvP3->z)))
			 -(lpvP3->x*((lpvP1->y*lpvP2->z)-(lpvP2->y*lpvP1->z)));

	// compute line deltas
	float fI=lpvL2->x-lpvL1->x;
	float fJ=lpvL2->y-lpvL1->y;
	float fK=lpvL2->z-lpvL1->z;

	// compute intersect denominator
	float fU=(fA*fI)+(fB*fJ)+(fC*fK);

	// check denominator
	if(IsNearZero(fU))
	{
		// line is parallel
		return(FALSE);
	}

	// compute t-value
	float fT=-((fA*lpvL1->x)+
			   (fB*lpvL1->y)+
			   (fC*lpvL1->z)+fD)/fU;
	
	// compute intersection
	lpvI->x=lpvL1->x+(fI*fT);
	lpvI->y=lpvL1->y+(fJ*fT);
	lpvI->z=lpvL1->z+(fK*fT);

	// ok
	return(TRUE);
}

// GetPolygonCentroid()

BOOL GetPolygonCentroid(LPD3DXVECTOR3 lpvC,
						LPPOLYGON lpPoly)
{
	// counter
	DWORD dwCount;

	// check number of vertices
	if(lpPoly->dwNumVtx==0)
		return(FALSE);

	// reset centroid
	(*lpvC)=D3DXVECTOR3(0,0,0);

	// add all vertices
	for(dwCount=0;dwCount<lpPoly->dwNumVtx;dwCount++)
		(*lpvC)+=lpPoly->lpvVtx[dwCount];

	// average vertices
	(*lpvC)/=((float)lpPoly->dwNumVtx);

	// ok
	return(TRUE);
}

// IsPointInBox()

BOOL IsPointInBox(LPD3DXVECTOR3 lpvP,
				  LPD3DXVECTOR3 lpvMin,
				  LPD3DXVECTOR3 lpvMax)
{
	// check point
	return(lpvMin->x<=lpvP->x&&
		   lpvMax->x>=lpvP->x&&
		   lpvMin->y<=lpvP->y&&
		   lpvMax->y>=lpvP->y&&
	       lpvMin->z<=lpvP->z&&
	       lpvMax->z>=lpvP->z);
}

// ComputeVertexSize()

DWORD ComputeVertexSize(DWORD dwFVF)
{
	// counter
	DWORD dwCount;
	
	// reset size
	DWORD dwSize=0;

	// mask xyz bits
	DWORD dwXYZ=dwFVF&((DWORD)0x0E);

	// add xyz values
	switch(dwXYZ)
	{
	case(D3DFVF_XYZ):
		// xyz
		dwSize+=sizeof(float)*3;
		break;
	case(D3DFVF_XYZRHW):
		// xyz+rhw
		dwSize+=sizeof(float)*4;
		break;
	case(D3DFVF_XYZB1):
		// xyz+(1) weight
		dwSize+=sizeof(float)*4;
		break;
	case(D3DFVF_XYZB2):
		// xyz+(2) weights
		dwSize+=sizeof(float)*5;
		break;
	case(D3DFVF_XYZB3):
		// xyz+(3) weights
		dwSize+=sizeof(float)*6;
		break;
	case(D3DFVF_XYZB4):
		// xyz+(4) weights
		dwSize+=sizeof(float)*7;
		break;
	case(D3DFVF_XYZB5):
		// xyz+(5) weights
		dwSize+=sizeof(float)*8;
		break;
	}

	// add normal
	if(dwFVF&D3DFVF_NORMAL)
		dwSize+=sizeof(float)*3;
	
	// add point size
	if(dwFVF&D3DFVF_PSIZE)
		dwSize+=sizeof(float);
	
	// add diffuse
	if(dwFVF&D3DFVF_DIFFUSE)
		dwSize+=sizeof(DWORD);
	
	// add specular
	if(dwFVF&D3DFVF_SPECULAR)
		dwSize+=sizeof(DWORD);

	// get number of texture coordinates
	DWORD dwNumTex=(dwFVF&((DWORD)0x0F00))>>8;

	// add texture coordinates
	if(dwNumTex)
	{
		// add each coordinate
		for(dwCount=0;dwCount<dwNumTex;dwCount++)
		{
			// mask size of texture coordinate
			DWORD dwTexSize=(dwFVF>>(16+(dwCount*2)))&((DWORD)0x03);

			// add texture coordinates
			switch(dwTexSize)
			{
			case(0x00):
				// (2) coordinates
				dwSize+=sizeof(float)*2;
				break;
			case(0x01):
				// (3) coordinates
				dwSize+=sizeof(float)*3;
				break;
			case(0x02):
				// (4) coordinates
				dwSize+=sizeof(float)*4;
				break;
			case(0x03):
				// (1) coordinate
				dwSize+=sizeof(float);
				break;
			}
		}
	}

	// return size
	return(dwSize);
}

#ifdef DEBUG_MODE
// DisplayRenderSettings()

void DisplayRenderSettings(void)
{
	LockGlobalMsg()

	// prepare message
	sprintf(g_lpMsg,
			"ABlend=%d\n"
			"ATest=%d\n"
			"Spec=%d\n"
			"Fog=%d\n"
			"FogClr=%d\n"
			"FogS=%f\n"
			"FogE=%f\n"
			"Amb=%d\n"
			"Lin=%d\n"
			"Gou=%d\n"
			"Wire=%d\n"
			"Pat=%d\n"
			"Rep=%d\n"
			"ZBuf=%d\n"
			"ZWri=%d\n"
			"Cull=%d\n"
			"ZBias=%d\n"
			"VPF=%d\n",
			g_bABlend,
			g_bATest,
			g_bSpecular,
			g_bFog,
			g_clrFog,
			g_fFogStart,
			g_fFogEnd,
			g_clrAmbient,
			g_bLinearFilter,
			g_bGouraud,
			g_bWireframe,
			g_wLPPattern,
			g_wLPRepeat,
			g_bZBufferEnabled,
			g_bZBufferWrites,
			g_bBackfaceCull,
			g_dwZBias,
			g_dwVPF);

	// display message
	DisplayInfoMessage(g_lpMsg);

	UnlockGlobalMsg()
}

// DisplayVideoDeviceSettings()

void DisplayVideoDeviceSettings(void)
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
	sprintf(g_lpMsg,"DeviceType="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DeviceType==D3DDEVTYPE_HAL) strcat(lpOutput,"HAL\n");
		if(g_lpDevCaps.DeviceType==D3DDEVTYPE_REF) strcat(lpOutput,"REF\n");
		if(g_lpDevCaps.DeviceType==D3DDEVTYPE_SW)  strcat(lpOutput,"SW\n");
	sprintf(g_lpMsg,"AdapterOrdinal=%d\n",g_lpDevCaps.AdapterOrdinal); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"ReadScanline="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.Caps&D3DCAPS_READ_SCANLINE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CanCalibrateGamma="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.Caps2&D3DCAPS2_CANCALIBRATEGAMMA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CanRenderWindowed="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.Caps2&D3DCAPS2_CANRENDERWINDOWED) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CanManageResource="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.Caps2&D3DCAPS2_CANMANAGERESOURCE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DynamicTextures="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.Caps2&D3DCAPS2_DYNAMICTEXTURES) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"FullScreenGamma="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.Caps2&D3DCAPS2_FULLSCREENGAMMA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"No2DDuring3DScene="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.Caps2&D3DCAPS2_NO2DDURING3DSCENE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"AlphaFullscreenFlipOrDiscard="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.Caps3&D3DCAPS3_ALPHA_FULLSCREEN_FLIP_OR_DISCARD) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"PresentationIntervalImmediate="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PresentationIntervals&D3DPRESENT_INTERVAL_IMMEDIATE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"PresentationIntervalOne="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PresentationIntervals&D3DPRESENT_INTERVAL_ONE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"PresentationIntervalTwo="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PresentationIntervals&D3DPRESENT_INTERVAL_TWO) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"PresentationIntervalThree="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PresentationIntervals&D3DPRESENT_INTERVAL_THREE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"PresentationIntervalFour="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PresentationIntervals&D3DPRESENT_INTERVAL_FOUR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ColorCursor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CursorCaps&D3DCURSORCAPS_COLOR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"LowResCursor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CursorCaps&D3DCURSORCAPS_LOWRES) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CanBltSysToNonLocal="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_CANBLTSYSTONONLOCAL) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CanRenderAfterFlip="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_CANRENDERAFTERFLIP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DrawPrimTLVertex="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_DRAWPRIMTLVERTEX) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ExecuteSystemMemory="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_EXECUTESYSTEMMEMORY) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ExecuteVideoMemory="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_EXECUTEVIDEOMEMORY) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"HWRasterization="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_HWRASTERIZATION) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"HWTransformAndLight="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"NPatches="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_NPATCHES) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"PureDevice="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_PUREDEVICE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"QuinticRTPatches="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_QUINTICRTPATCHES) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"RTPatches="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_RTPATCHES) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"RTPatchHandleZero="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_RTPATCHHANDLEZERO) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SeparateTextureMemories="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_SEPARATETEXTUREMEMORIES) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureNonLocalVidMem="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_TEXTURENONLOCALVIDMEM) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TLVertexSystemMemory="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_TLVERTEXSYSTEMMEMORY) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TLVertexVideoMemory="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DevCaps&D3DDEVCAPS_TLVERTEXVIDEOMEMORY) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");

	UnlockGlobalMsg()

	// display message
	DisplayInfoMessage(lpOutput);

	LockGlobalMsg()
		
	// output data
	strcpy(lpOutput,"");
	sprintf(g_lpMsg,"BlendOp="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_BLENDOP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ClipPlaneScaledPoints="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_CLIPPLANESCALEDPOINTS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ClipTLVerts="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_CLIPTLVERTS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ColorWriteEnable="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_COLORWRITEENABLE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CullCCW="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_CULLCCW) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CullCW="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_CULLCW) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CullNone="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_CULLNONE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"LinePatterRep="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_LINEPATTERNREP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"MaskZ="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_MASKZ) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TSSArgTemp="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_TSSARGTEMP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"Anisotropy="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_ANISOTROPY) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"AntiAliasEdges="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_ANTIALIASEDGES) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ColorPerspective="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_COLORPERSPECTIVE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"Dither="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_DITHER) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"FogRange="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_FOGRANGE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"FogTable="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_FOGTABLE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"FogVertex="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_FOGVERTEX) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"MipMapLODBias="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_MIPMAPLODBIAS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"LinePat="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_PAT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"StretchBltMultiSample="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_STRETCHBLTMULTISAMPLE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"WBuffer="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_WBUFFER) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"WFog="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_WFOG) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZBias="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_ZBIAS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZBufferlessHSR="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_ZBUFFERLESSHSR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZFog="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_ZFOG) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZTest="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_ZTEST) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZCmpAlways="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ZCmpCaps&D3DPCMPCAPS_ALWAYS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZCmpEqual="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ZCmpCaps&D3DPCMPCAPS_EQUAL) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZCmpGreater="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ZCmpCaps&D3DPCMPCAPS_GREATER) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZCmpGreaterEqual="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ZCmpCaps&D3DPCMPCAPS_GREATEREQUAL) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZCmpLess="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ZCmpCaps&D3DPCMPCAPS_LESS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZCmpLessEqual="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ZCmpCaps&D3DPCMPCAPS_LESSEQUAL) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZCmpNever="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ZCmpCaps&D3DPCMPCAPS_NEVER) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ZCmpNotEqual="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ZCmpCaps&D3DPCMPCAPS_NOTEQUAL) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");

	UnlockGlobalMsg()

	// display message
	DisplayInfoMessage(lpOutput);

	LockGlobalMsg()

	// output data
	strcpy(lpOutput,"");
	sprintf(g_lpMsg,"SrcBlendBothInvScrAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_BOTHINVSRCALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendBothSrcAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_BOTHSRCALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendDestAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_DESTALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendDestColor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_DESTCOLOR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendInvDestAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_INVDESTALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendInvDestColor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_INVDESTCOLOR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendInvSrcAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_INVSRCALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendInvSrcColor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_INVSRCCOLOR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendOne="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_ONE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendSrcAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_SRCALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendSrcAlphaSat="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_SRCALPHASAT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendSrcColor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_SRCCOLOR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SrcBlendZero="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_ZERO) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendBothInvScrAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_BOTHINVSRCALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendBothSrcAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_BOTHSRCALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendDestAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_DESTALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendDestColor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_DESTCOLOR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendInvDestAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_INVDESTALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendInvDestColor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_INVDESTCOLOR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendInvSrcAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_INVSRCALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendInvSrcColor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_INVSRCCOLOR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendOne="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_ONE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendSrcAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_SRCALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendSrcAlphaSat="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_SRCALPHASAT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendSrcColor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_SRCCOLOR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"DestBlendZero="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_ZERO) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"AlphaCmpAlways="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.AlphaCmpCaps&D3DPCMPCAPS_ALWAYS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"AlphaCmpEqual="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.AlphaCmpCaps&D3DPCMPCAPS_EQUAL) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"AlphaCmpGreater="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.AlphaCmpCaps&D3DPCMPCAPS_GREATER) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"AlphaCmpGreaterEqual="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.AlphaCmpCaps&D3DPCMPCAPS_GREATEREQUAL) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"AlphaCmpLess="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.AlphaCmpCaps&D3DPCMPCAPS_LESS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"AlphaCmpLessEqual="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.AlphaCmpCaps&D3DPCMPCAPS_LESSEQUAL) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"AlphaCmpNever="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.AlphaCmpCaps&D3DPCMPCAPS_NEVER) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"AlphaCmpNotEqual="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.AlphaCmpCaps&D3DPCMPCAPS_NOTEQUAL) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");

	UnlockGlobalMsg()

	// display message
	DisplayInfoMessage(lpOutput);

	LockGlobalMsg()

	// output data
	strcpy(lpOutput,"");
	sprintf(g_lpMsg,"AlphaGouraudBlend="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ShadeCaps&D3DPSHADECAPS_ALPHAGOURAUDBLEND) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"ColorGouraudRGB="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ShadeCaps&D3DPSHADECAPS_COLORGOURAUDRGB) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"FogGouraud="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ShadeCaps&D3DPSHADECAPS_FOGGOURAUD) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"SpecularGouraudRGB="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.ShadeCaps&D3DPSHADECAPS_SPECULARGOURAUDRGB) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_ALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureAlphaPalette="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_ALPHAPALETTE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureCubeMap="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_CUBEMAP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureCubeMapPow2="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_CUBEMAP_POW2) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureMipCubeMap="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_MIPCUBEMAP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureMipMap="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_MIPMAP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureMipVolumeMap="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_MIPVOLUMEMAP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureNonPow2Conditional="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_NONPOW2CONDITIONAL) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexturePerspective="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_PERSPECTIVE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexturePow2="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_POW2) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureProjected="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_PROJECTED) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureSquareOnly="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_SQUAREONLY) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureTexRepeatNotScaledBySize="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_TEXREPEATNOTSCALEDBYSIZE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureVolumeMap="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_VOLUMEMAP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TextureVoulmeMapPow2="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_VOLUMEMAP_POW2) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexFilterMagFFlatCubic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFAFLATCUBIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexFilterMagFAnisotropic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFANISOTROPIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexFilterMagFGaussianCubic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFGAUSSIANCUBIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexFilterMagFLinear="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFLINEAR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexFilterMagFPoint="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFPOINT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexFilterMinFAnisotropic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureFilterCaps&D3DPTFILTERCAPS_MINFANISOTROPIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexFilterMinFLinear="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureFilterCaps&D3DPTFILTERCAPS_MINFLINEAR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexFilterMinFPoint="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureFilterCaps&D3DPTFILTERCAPS_MINFPOINT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexFilterMipFLinear="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureFilterCaps&D3DPTFILTERCAPS_MIPFLINEAR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexFilterMipFPoint="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureFilterCaps&D3DPTFILTERCAPS_MIPFPOINT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CubeTexFilterMagFFlatCubic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CubeTextureFilterCaps&D3DPTFILTERCAPS_MAGFAFLATCUBIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CubeTexFilterMagFAnisotropic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CubeTextureFilterCaps&D3DPTFILTERCAPS_MAGFANISOTROPIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CubeTexFilterMagFGaussianCubic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CubeTextureFilterCaps&D3DPTFILTERCAPS_MAGFGAUSSIANCUBIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CubeTexFilterMagFLinear="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CubeTextureFilterCaps&D3DPTFILTERCAPS_MAGFLINEAR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CubeTexFilterMagFPoint="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CubeTextureFilterCaps&D3DPTFILTERCAPS_MAGFPOINT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CubeTexFilterMinFAnisotropic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CubeTextureFilterCaps&D3DPTFILTERCAPS_MINFANISOTROPIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CubeTexFilterMinFLinear="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CubeTextureFilterCaps&D3DPTFILTERCAPS_MINFLINEAR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CubeTexFilterMinFPoint="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CubeTextureFilterCaps&D3DPTFILTERCAPS_MINFPOINT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CubeTexFilterMipFLinear="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CubeTextureFilterCaps&D3DPTFILTERCAPS_MIPFLINEAR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"CubeTexFilterMipFPoint="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.CubeTextureFilterCaps&D3DPTFILTERCAPS_MIPFPOINT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");

	UnlockGlobalMsg()

	// display message
	DisplayInfoMessage(lpOutput);

	LockGlobalMsg()

	// output data
	strcpy(lpOutput,"");
	sprintf(g_lpMsg,"VolTexFilterMagFFlatCubic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureFilterCaps&D3DPTFILTERCAPS_MAGFAFLATCUBIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexFilterMagFAnisotropic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureFilterCaps&D3DPTFILTERCAPS_MAGFANISOTROPIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexFilterMagFGaussianCubic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureFilterCaps&D3DPTFILTERCAPS_MAGFGAUSSIANCUBIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexFilterMagFLinear="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureFilterCaps&D3DPTFILTERCAPS_MAGFLINEAR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexFilterMagFPoint="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureFilterCaps&D3DPTFILTERCAPS_MAGFPOINT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexFilterMinFAnisotropic="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureFilterCaps&D3DPTFILTERCAPS_MINFANISOTROPIC) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexFilterMinFLinear="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureFilterCaps&D3DPTFILTERCAPS_MINFLINEAR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexFilterMinFPoint="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureFilterCaps&D3DPTFILTERCAPS_MINFPOINT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexFilterMipFLinear="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureFilterCaps&D3DPTFILTERCAPS_MIPFLINEAR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexFilterMipFPoint="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureFilterCaps&D3DPTFILTERCAPS_MIPFPOINT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexAddrBorder="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureAddressCaps&D3DPTADDRESSCAPS_BORDER) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexAddrClamp="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureAddressCaps&D3DPTADDRESSCAPS_CLAMP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexAddrIndependentUV="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureAddressCaps&D3DPTADDRESSCAPS_INDEPENDENTUV) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexAddrMirror="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureAddressCaps&D3DPTADDRESSCAPS_MIRROR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexAddrMirrorOnce="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureAddressCaps&D3DPTADDRESSCAPS_MIRRORONCE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexAddrWrap="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureAddressCaps&D3DPTADDRESSCAPS_WRAP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexAddrBorder="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureAddressCaps&D3DPTADDRESSCAPS_BORDER) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexAddrClamp="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureAddressCaps&D3DPTADDRESSCAPS_CLAMP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexAddrIndependentUV="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureAddressCaps&D3DPTADDRESSCAPS_INDEPENDENTUV) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexAddrMirror="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureAddressCaps&D3DPTADDRESSCAPS_MIRROR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexAddrMirrorOnce="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureAddressCaps&D3DPTADDRESSCAPS_MIRRORONCE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VolTexAddrWrap="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VolumeTextureAddressCaps&D3DPTADDRESSCAPS_WRAP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"LineAlphaCmp="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.LineCaps&D3DLINECAPS_ALPHACMP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"LineBlend="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.LineCaps&D3DLINECAPS_BLEND) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"LineFog="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.LineCaps&D3DLINECAPS_FOG) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"LineTexture="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.LineCaps&D3DLINECAPS_TEXTURE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"LineZTest="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.LineCaps&D3DLINECAPS_ZTEST) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"MaxTextureWidth=%d\n",g_lpDevCaps.MaxTextureWidth); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxTextureHeight=%d\n",g_lpDevCaps.MaxTextureHeight); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxVolumeExtent=%d\n",g_lpDevCaps.MaxVolumeExtent); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxTextureRepeat=%d\n",g_lpDevCaps.MaxTextureRepeat); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxTextureAspectRatio=%d\n",g_lpDevCaps.MaxTextureAspectRatio); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxAnisotropy=%d\n",g_lpDevCaps.MaxAnisotropy); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxVertexW=%f\n",g_lpDevCaps.MaxVertexW); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"GuardBandLeft=%f\n",g_lpDevCaps.GuardBandLeft); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"GuardBandTop=%f\n",g_lpDevCaps.GuardBandTop); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"GuardBandRight=%f\n",g_lpDevCaps.GuardBandRight); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"GuardBandBottom=%f\n",g_lpDevCaps.GuardBandBottom); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"ExtentsAdjust=%f\n",g_lpDevCaps.ExtentsAdjust); strcat(lpOutput,g_lpMsg);

	UnlockGlobalMsg()

	// display message
	DisplayInfoMessage(lpOutput);

	LockGlobalMsg()

	// output data
	strcpy(lpOutput,"");
	sprintf(g_lpMsg,"StencilDecr="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.StencilCaps&D3DSTENCILCAPS_DECR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"StencilDecrSat="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.StencilCaps&D3DSTENCILCAPS_DECRSAT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"StencilIncr="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.StencilCaps&D3DSTENCILCAPS_INCR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"StencilIncrSat="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.StencilCaps&D3DSTENCILCAPS_INCRSAT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"StencilInvert="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.StencilCaps&D3DSTENCILCAPS_INVERT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"StencilKeep="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.StencilCaps&D3DSTENCILCAPS_KEEP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"StencilReplace="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.StencilCaps&D3DSTENCILCAPS_REPLACE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"StencilZero="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.StencilCaps&D3DSTENCILCAPS_ZERO) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"FVFDoNotStripElements="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.FVFCaps&D3DFVFCAPS_DONOTSTRIPELEMENTS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"FVFPSize="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.FVFCaps&D3DFVFCAPS_PSIZE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"FVFTexCoordCountMask="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.FVFCaps&D3DFVFCAPS_TEXCOORDCOUNTMASK) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpAdd="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_ADD) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpAddSigned="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_ADDSIGNED) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpAddSigned2X="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_ADDSIGNED2X) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpAddSmooth="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_ADDSMOOTH) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpBlendCurrentAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_BLENDCURRENTALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpBlendDiffuseAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_BLENDDIFFUSEALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpBlendFactorAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_BLENDFACTORALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpBlendTextureAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_BLENDTEXTUREALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpBlendTextureAlphaPM="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_BLENDTEXTUREALPHAPM) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpBumpEnvMap="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_BUMPENVMAP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpBumpEnvMapLuminance="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_BUMPENVMAPLUMINANCE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpDisable="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_DISABLE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpDotProduct3="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_DOTPRODUCT3) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpLerp="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_LERP) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpModulate="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_MODULATE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpModulate2X="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_MODULATE2X) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpModulate4X="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_MODULATE4X) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpModulateAlphaAddColor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_MODULATEALPHA_ADDCOLOR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpModulateColorAddAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_MODULATECOLOR_ADDALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpModulateInvAlphaAddColor="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_MODULATEINVALPHA_ADDCOLOR) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpModulateInvColorAddAlpha="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_MODULATEINVCOLOR_ADDALPHA) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpMultiplyAdd="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_MULTIPLYADD) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpPreModulate="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_PREMODULATE) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpSelectArg1="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_SELECTARG1) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpSelectArg2="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_SELECTARG2) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"TexOpSubtract="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.TextureOpCaps&D3DTEXOPCAPS_SUBTRACT) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");

	UnlockGlobalMsg()

	// display message
	DisplayInfoMessage(lpOutput);

	LockGlobalMsg()

	// output data
	strcpy(lpOutput,"");
	sprintf(g_lpMsg,"MaxTextureBlendStages=%d\n",g_lpDevCaps.MaxTextureBlendStages); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxSimultaneousTextures=%d\n",g_lpDevCaps.MaxSimultaneousTextures); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"VtxProcDirLights="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VertexProcessingCaps&D3DVTXPCAPS_DIRECTIONALLIGHTS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VtxProcLocalViewer="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VertexProcessingCaps&D3DVTXPCAPS_LOCALVIEWER) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VtxProcMatSrc7="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VertexProcessingCaps&D3DVTXPCAPS_MATERIALSOURCE7) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VtxProcPosLights="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VertexProcessingCaps&D3DVTXPCAPS_POSITIONALLIGHTS) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VtxProcTexGen="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VertexProcessingCaps&D3DVTXPCAPS_TEXGEN) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VtxProcTweening="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VertexProcessingCaps&D3DVTXPCAPS_TWEENING) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"VtxProcNoVSDTUByte4="); strcat(lpOutput,g_lpMsg);
		if(g_lpDevCaps.VertexProcessingCaps&D3DVTXPCAPS_NO_VSDT_UBYTE4) strcat(lpOutput,"Yes\n"); else strcat(lpOutput,"No\n");
	sprintf(g_lpMsg,"MaxActiveLights=%d\n",g_lpDevCaps.MaxActiveLights); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxUserClipPlanes=%d\n",g_lpDevCaps.MaxUserClipPlanes); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxVertexBlendMatrices=%d\n",g_lpDevCaps.MaxVertexBlendMatrices); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxVertexBlendMatrixIndex=%d\n",g_lpDevCaps.MaxVertexBlendMatrixIndex); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxPointSize=%f\n",g_lpDevCaps.MaxPointSize); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxPrimitiveCount=%d\n",g_lpDevCaps.MaxPrimitiveCount); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxVertexIndex=%d\n",g_lpDevCaps.MaxVertexIndex); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxStreams=%d\n",g_lpDevCaps.MaxStreams); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxStreamStride=%d\n",g_lpDevCaps.MaxStreamStride); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"VertexShaderVersion=%d\n",g_lpDevCaps.VertexShaderVersion); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxVertexShaderConst=%d\n",g_lpDevCaps.MaxVertexShaderConst); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"PixelShaderVersion=%d\n",g_lpDevCaps.PixelShaderVersion); strcat(lpOutput,g_lpMsg);
	sprintf(g_lpMsg,"MaxPixelShaderValue=%f\n",g_lpDevCaps.MaxPixelShaderValue); strcat(lpOutput,g_lpMsg);

	UnlockGlobalMsg()
	
	// display message
	DisplayInfoMessage(lpOutput);
	
	// free output buffer
	FreeMem((LPVOID*)&lpOutput);
}
#endif

// SetViewMatrixA()

void SetViewMatrixA(LPD3DXMATRIX lpMtx)
{
	// check pointer
	if(lpMtx)
	{
		// set specified matrix
		g_lpD3DDevice->SetTransform(D3DTS_VIEW,
									lpMtx);

		// invert view matrix
		D3DXMATRIX mView;
		D3DXMatrixInverse(&mView,
						  NULL,
						  lpMtx);
		
		// set billboard matrix
		SetBillboardMatrix(&mView);
	}
	else
	{
		// set identity matrix
		g_lpD3DDevice->SetTransform(D3DTS_VIEW,
									&g_mtxIdentity);

		// invert view matrix
		D3DXMATRIX mView;
		D3DXMatrixInverse(&mView,
						  NULL,
						  &g_mtxIdentity);
		
		// set billboard matrix
		SetBillboardMatrix(&mView);
	}
}

// SetProjectionMatrixA()

void SetProjectionMatrixA(LPD3DXMATRIX lpMtx)
{
	// check pointer
	if(lpMtx)
	{
		// set specified matrix
		g_lpD3DDevice->SetTransform(D3DTS_PROJECTION,
									lpMtx);
	}
	else
	{
		// set identity matrix
		g_lpD3DDevice->SetTransform(D3DTS_PROJECTION,
									&g_mtxIdentity);
	}
}

// SetProjectionMatrixA()

void SetProjectionMatrixA(float fClipNear,
						  float fClipFar,
						  float fAngle,
						  float fAspect)
{
	// prepare projection matrix
	D3DXMATRIX mProj;
	D3DXMatrixPerspectiveFovLH(&mProj,
							   DegToRad(fAngle),
							   fAspect,
							   fClipNear,
							   fClipFar);
	
	// set projection matrix
	SetProjectionMatrixA(&mProj);
}

// SetMatrixPosition()

void SetMatrixPosition(DWORD dwIndex,
					   LPD3DXVECTOR3 lpvPos)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetMatrixPosition()",
								 FALSE);
		return;
	}
#endif

	// copy values to matrix
	g_lpMtx[dwIndex].mtx._41=lpvPos->x;
	g_lpMtx[dwIndex].mtx._42=lpvPos->y;
	g_lpMtx[dwIndex].mtx._43=lpvPos->z;
}

// SetMatrixPosition()

void SetMatrixPosition(DWORD dwIndex,
					   float fX,
					   float fY,
					   float fZ)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetMatrixPosition()",
								 FALSE);
		return;
	}
#endif

	// copy values to matrix
	g_lpMtx[dwIndex].mtx._41=fX;
	g_lpMtx[dwIndex].mtx._42=fY;
	g_lpMtx[dwIndex].mtx._43=fZ;
}

// SetDefaultProjectionMatrix()

void SetDefaultProjectionMatrix(float fClipNear,
								float fClipFar,
								float fAngle)
{
	// aspect ratio
	float fAspect;
	
	// compute aspect ratio
	fAspect=((float)g_dmApp.dwX)/((float)g_dmApp.dwY);
	
	// set projection matrix
	SetProjectionMatrix(fClipNear,fClipFar,fAngle,fAspect);
}

// SetDefaultViewMatrix()

void SetDefaultViewMatrix(void)
{
	// set view matrix
	SetViewMatrix(0,0,0,
				  0,0,1);
}

// BeginShadedSprites()

BOOL BeginShadedSprites(BOOL bLinear)
{
#ifdef DEBUG_MODE
	// set flag
	g_bShadedSprites=TRUE;
#endif

	// begin sprites
	return(BeginSprites(bLinear));
}

// OutputShadedSprite()

void OutputShadedSprite(DWORD dwTexIndex,
						D3DCOLOR clr,
						float fX,
						float fY,
						float fXSize,
						float fYSize,
						float fRot)
{
#ifdef DEBUG_MODE
	/*// check flag
	if(!g_bShadedSprites)
		DisplayInfoMessage("Shaded sprite in unshaded mode!");*/
#endif

	// prepare material
	D3DMATERIAL8 mat={0,0,0,GetA888(clr),
					  0,0,0,GetA888(clr),
					  0,0,0,GetA888(clr),
					  GetR888(clr),
					  GetG888(clr),
					  GetB888(clr),
					  GetA888(clr),
					  1};
	
	// update sprite material
	UpdateMaterial(g_dwSpriteMat,
				   &mat);

	// set material
	SetMaterial(g_dwSpriteMat);
	
	// output sprite
	OutputSprite(dwTexIndex,
				 fX,fY,
				 fXSize,fYSize,
				 fRot);

#ifdef DEBUG_MODE
	// reset flag
	g_bShadedSprites=TRUE;
#endif
}

void EndShadedSprites(void)
{
#ifdef DEBUG_MODE
	// set flag
	g_bShadedSprites=FALSE;
#endif

	// prepare material
	D3DMATERIAL8 mat={0,0,0,1,
					  0,0,0,1,
					  0,0,0,1,
					  1,1,1,1,
					  1};
	
	// update sprite material
	UpdateMaterial(g_dwSpriteMat,
				   &mat);

	// end sprites
	EndSprites();
}

// EnableAlphaTextures()

void EnableAlphaTextures(void)
{
	// enable alpha textures
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_ALPHAARG1,
										D3DTA_TEXTURE);
	
	// set flag
	g_bAlphaTextures=TRUE;
}

// DisableAlphaTextures()

void DisableAlphaTextures(void)
{
	// disable alpha textures
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_ALPHAARG1,
										D3DTA_DIFFUSE);
	
	// set flag
	g_bAlphaTextures=FALSE;
}

// RotFromAngle()

float RotFromAngle(float fAngle)
{
	// reset return value
	float fRet=0;
	
	// clip angle if necessary
	while(fAngle<0)
		fAngle+=360;
	while(fAngle>360)
		fAngle-=360;

	// subtract 45 degree angles
	while(fAngle>45)
	{
		fRet+=1;
		fAngle-=45;
	}

	// convert remaining angle to radians
	float fRads=DegToRad(fAngle);

	// get sin & cos of angle
	float fCos=cosf(fRads);
	float fSin=sinf(fRads);

	// intersection points
	float fX;
	float fY;

	// get intersection
	Get2DLineIntersection(&fX,&fY,
						  0,0,
						  fCos,fSin,
						  1,0,
						  1,1);

	// add remaining value to return value
	fRet+=fY;

	// ok
	return(fRet);
}

#ifdef USE_TRANSITIONS
// InitTransitions()

BOOL InitTransitions(void)
{
	LoadingOutput("InitTransitions");

	// add vertex buffer
	g_dwTransVtxBuf=AddVertexBuffer(D3DFVF_TRANSVERTEX,
									sizeof(TRANSVERTEX),
									MAX_TRANSVERTEX);

	// check vertex buffer
	if(g_dwTransVtxBuf==NO_OBJECT)
		return(FALSE);

	// add index buffer
	g_dwTransIdxBuf=AddIndexBuffer(D3DPT_TRIANGLELIST,
								   MAX_TRANSTRIANGLE);

	// check index buffer
	if(g_dwTransIdxBuf==NO_OBJECT)
		return(FALSE);

	// ok
	return(TRUE);
}

// SetReverseTransition()

void SetReverseTransition(TRANSTYPE tt,
						  DWORD dwData1,
						  DWORD dwData2,
						  DWORD dwData3,
						  DWORD dwData4)
{
	// set transition
	SetTransition(tt,
				  dwData1,
				  dwData2,
				  dwData3,
				  dwData4);

	// set reverse flag
	g_bTransRev=TRUE;
}

// SetTransition()

void SetTransition(TRANSTYPE tt,
				   DWORD dwData1,
				   DWORD dwData2,
				   DWORD dwData3,
				   DWORD dwData4)
{
	// internal variables
	LPWORD lpIdx;
	DWORD dwX;
	DWORD dwY;
	BOOL bRev;

	// save transition type
	g_ttCurTrans=tt;

	// reset reverse flag
	g_bTransRev=FALSE;

	// check transition type for general settings
	switch(tt)
	{
	// tex,color
	case(TT_FADEIN):
	case(TT_FADEOUT):
		// copy texture index
		g_lpdwTransData[TD_FADE_TEX]=dwData1;

		// copy color value
		g_lpdwTransData[TD_FADE_COLOR]=dwData2;

		// copy timing values
		g_dwTransCurPos=0;
		g_dwTransEndPos=dwData3;

		// ok
		break;

	// tex1,tex2,color
	case(TT_FLASH):
		// copy texture indices
		g_lpdwTransData[TD_FLASH_TEX1]=dwData1;
		g_lpdwTransData[TD_FLASH_TEX2]=dwData2;

		// copy color
		g_lpdwTransData[TD_FLASH_COLOR]=dwData3;

		// copy timing values
		g_dwTransCurPos=0;
		g_dwTransEndPos=dwData4;

		// ok
		break;

	// tex1,tex2
	case(TT_TWEEN):
	case(TT_SHATTER):
		// copy texture indices
		g_lpdwTransData[TD_TWEEN_TEX1]=dwData1;
		g_lpdwTransData[TD_TWEEN_TEX2]=dwData2;

		// copy timing values
		g_dwTransCurPos=0;
		g_dwTransEndPos=dwData3;

		// ok
		break;

	// tex1,tex2,direction
	case(TT_PEEL):
	case(TT_SWING):
	case(TT_SCROLL):
	case(TT_SQUISH):
	case(TT_SLIDE):
		// copy texture indices
		g_lpdwTransData[TD_PEEL_TEX1]=dwData1;
		g_lpdwTransData[TD_PEEL_TEX2]=dwData2;

		// copy direction
		g_lpdwTransData[TD_PEEL_DIR]=dwData3;

		// copy timing values
		g_dwTransCurPos=0;
		g_dwTransEndPos=dwData4;

		// ok
		break;

	// random
	case(TT_RANDOM):
		// create random trasition
		switch(abs(GetRandomInt())%5)
		{
		case(0): tt=TT_PEEL;   break;
		case(1): tt=TT_SWING;  break;
		case(2): tt=TT_SCROLL; break;
		case(3): tt=TT_SLIDE;  break;
		case(4): tt=TT_SQUISH; break;
		}

		// set reverse flag
		if(abs(GetRandomInt())%2)
			bRev=TRUE;
		else
			bRev=FALSE;

		// get random direction
		dwData4=(DWORD)((abs(GetRandomInt())%4)+1);
		
		// set new transition
		if(bRev)
			SetTransition(tt,
						  dwData1,
						  dwData2,
						  dwData4,
						  dwData3);
		else
			SetReverseTransition(tt,
								 dwData2,
								 dwData1,
								 dwData4,
								 dwData3);

		// ok
		return;
	}

	// check transition type for index buffer
	switch(tt)
	{
	// hi-res, 2-sided mesh
	case(TT_PEEL):
		// prepare index buffer
		lpIdx=LockIndexBuffer(g_dwTransIdxBuf);
		for(dwY=0;dwY<7;dwY++)
			for(dwX=0;dwX<7;dwX++)
			{
				// compute positions
				WORD wVtxPos=(WORD)(dwX+(dwY*8));
				WORD wIdxPos=(WORD)(dwX+(dwY*7))*12;

				// set indices
				lpIdx[wIdxPos   ]=wVtxPos+1;
				lpIdx[wIdxPos+1 ]=wVtxPos;
				lpIdx[wIdxPos+2 ]=wVtxPos+8;
				lpIdx[wIdxPos+3 ]=wVtxPos+1;
				lpIdx[wIdxPos+4 ]=wVtxPos+8;
				lpIdx[wIdxPos+5 ]=wVtxPos+9;
				lpIdx[wIdxPos+6 ]=wVtxPos;
				lpIdx[wIdxPos+7 ]=wVtxPos+1;
				lpIdx[wIdxPos+8 ]=wVtxPos+8;
				lpIdx[wIdxPos+9 ]=wVtxPos+8;
				lpIdx[wIdxPos+10]=wVtxPos+1;
				lpIdx[wIdxPos+11]=wVtxPos+9;
			}
		UnlockIndexBuffer(g_dwTransIdxBuf);

		// set number of primitives
		g_lpdwNumPrim[g_dwTransIdxBuf]=196;

		// ok
		break;

	// low-res, 1-sided mesh
	case(TT_SWING):
		// prepare index buffer
		lpIdx=LockIndexBuffer(g_dwTransIdxBuf);
		lpIdx[0]=2;
		lpIdx[1]=1;
		lpIdx[2]=0;
		lpIdx[3]=1;
		lpIdx[4]=2;
		lpIdx[5]=3;
		UnlockIndexBuffer(g_dwTransIdxBuf);

		// set number of primitives
		g_lpdwNumPrim[g_dwTransIdxBuf]=2;

		// ok
		break;

	// shatter mesh
	case(TT_SHATTER):
		// prepare index buffer
		lpIdx=LockIndexBuffer(g_dwTransIdxBuf);
		for(dwX=0;dwX<20;dwX++)
		{
			// compute positions
			WORD wVtxPos=(WORD)(dwX*3);
			WORD wIdxPos=(WORD)(dwX*6);

			// set indices
			lpIdx[wIdxPos  ]=wVtxPos;
			lpIdx[wIdxPos+1]=wVtxPos+1;
			lpIdx[wIdxPos+2]=wVtxPos+2;
			lpIdx[wIdxPos+3]=wVtxPos+1;
			lpIdx[wIdxPos+4]=wVtxPos;
			lpIdx[wIdxPos+5]=wVtxPos+2;
		}
		UnlockIndexBuffer(g_dwTransIdxBuf);

		// set number of primitives
		g_lpdwNumPrim[g_dwTransIdxBuf]=40;

		// randomize shatter
		RandomizeShatter();

		// ok
		break;
	}
}

// RenderTransition()

BOOL RenderTransition(void)
{
	// internal variables
	float fAlpha;
	BOOL bAlphaTextures;
	LPTRANSVERTEX lpVtx;
	DWORD dwX;
	DWORD dwY;
	D3DXMATRIX mProj;
	float fTU;
	float fTV;
	float fX;
	float fY;
	float fZ;
	float fAngle;
	float fCurveX[8];
	float fCurveY[8];
	float fXPos1;
	float fYPos1;
	float fXSize1;
	float fYSize1;
	float fXPos2;
	float fYPos2;
	float fXSize2;
	float fYSize2;

	// reset continue flag
	BOOL bContinue=TRUE;
	
	// update timer
	g_dwTransCurPos+=g_dwFrameSkip;

	// check for overflow
	if(g_dwTransCurPos>g_dwTransEndPos)
	{
		// clip current position
		g_dwTransCurPos=g_dwTransEndPos;
		
		// transition complete
		bContinue=FALSE;
	}

	// compute transition position
	float fPos=((float)(g_dwTransCurPos))/((float)(g_dwTransEndPos));

	// reverse position if necessary
	if(g_bTransRev)
		fPos=1-fPos;

	// clear screen
	ClearScreen();

	// begin scene
	if(!BeginScene())
		return(FALSE);

	// check transition type
	switch(g_ttCurTrans)
	{
	// no transition set
	case(TT_NULL):
		// transition complete
		bContinue=FALSE;

		// ok
		break;

	// fade in
	case(TT_FADEIN):
		// begin sprites
		if(BeginSprites(TRUE))
		{
			// render texture
			OutputSprite(g_lpdwTransData[TD_FADE_TEX],
	 					 ScrCoords(320,240),
						 ScrSize(640,480));
			
			// end sprites
			EndSprites();
		}

		// fade screen
		FadeScreen(D3DCOLOR_COLORVALUE(GetR888(g_lpdwTransData[TD_FADE_COLOR]),
									   GetG888(g_lpdwTransData[TD_FADE_COLOR]),
									   GetB888(g_lpdwTransData[TD_FADE_COLOR]),
									   1-fPos));

		// ok
		break;

	// fade out
	case(TT_FADEOUT):
		// begin sprites
		if(BeginSprites(TRUE))
		{
			// render texture
			OutputSprite(g_lpdwTransData[TD_FADE_TEX],
	 					 ScrCoords(320,240),
						 ScrSize(640,480));
			
			// end sprites
			EndSprites();
		}

		// fade screen
		FadeScreen(D3DCOLOR_COLORVALUE(GetR888(g_lpdwTransData[TD_FADE_COLOR]),
									   GetG888(g_lpdwTransData[TD_FADE_COLOR]),
									   GetB888(g_lpdwTransData[TD_FADE_COLOR]),
									   fPos));
		// ok
		break;

	// flash
	case(TT_FLASH):
		// begin sprites
		if(BeginSprites(TRUE))
		{
			// render appropriate texture
			if(fPos<0.5F)
				OutputSprite(g_lpdwTransData[TD_FLASH_TEX1],
	 						 ScrCoords(320,240),
							 ScrSize(640,480));
			else
				OutputSprite(g_lpdwTransData[TD_FLASH_TEX2],
	 						 ScrCoords(320,240),
							 ScrSize(640,480));
			
			// end sprites
			EndSprites();
		}

		// compute alpha
		fAlpha=1-(fabsf(fPos-0.5F)*2);
		
		// fade screen
		FadeScreen(D3DCOLOR_COLORVALUE(GetR888(g_lpdwTransData[TD_FLASH_COLOR]),
									   GetG888(g_lpdwTransData[TD_FLASH_COLOR]),
									   GetB888(g_lpdwTransData[TD_FLASH_COLOR]),
									   fAlpha));
		// ok
		break;

	// tween
	case(TT_TWEEN):
		// copy alpha textures flag
		bAlphaTextures=g_bAlphaTextures;

		// check alpha textures flag
		if(bAlphaTextures)
			DisableAlphaTextures();
		
		// begin sprites
		if(BeginShadedSprites(TRUE))
		{
			// render texture 1
			OutputShadedSprite(g_lpdwTransData[TD_TWEEN_TEX1],
							   D3DCOLOR_COLORVALUE(1,1,1,1),
	 						   ScrCoords(320,240),
							   ScrSize(640,480));
			
			// render texture 2
			OutputShadedSprite(g_lpdwTransData[TD_TWEEN_TEX2],
							   D3DCOLOR_COLORVALUE(1,1,1,fPos),
	 						   ScrCoords(320,240),
							   ScrSize(640,480));

			// end sprites
			EndShadedSprites();
		}

		// check alpha textures flag
		if(bAlphaTextures)
			EnableAlphaTextures();
		
		// ok
		break;

	// peel
	case(TT_PEEL):
		// compute angle
		fAngle=fPos*0.4F;

		// compute curve values
		fCurveX[0]=0;
		fCurveY[0]=0;
		for(dwX=1;dwX<8;dwX++)
		{
			fCurveX[dwX]=fCurveX[dwX-1]+(cosf(fAngle*((float)(dwX)))/7);
			fCurveY[dwX]=fCurveY[dwX-1]+(sinf(fAngle*((float)(dwX)))/7);
		}

		// lock vertex buffer
		lpVtx=(LPTRANSVERTEX)LockVertexBuffer(g_dwTransVtxBuf);

		// check for error
		if(!lpVtx)
			break;

		// update vertex buffer
		for(dwY=0;dwY<8;dwY++)
			for(dwX=0;dwX<8;dwX++)
			{
				// compute texture coordinates
				fTU=((float)(dwX))/7;
				fTV=((float)(dwY))/7;
				
				// compute position based on direction
				switch(g_lpdwTransData[TD_PEEL_DIR])
				{
				case(TDR_LTOR):
					fX=-((fCurveX[7-dwX]-(fPos*0.15F))-0.5F)*640;
					fY=(fTV-0.5F)*480;
					fZ=579.5F-(fCurveY[7-dwX]*640);
					break;
				case(TDR_RTOL):
					fX=((fCurveX[dwX]-(fPos*0.15F))-0.5F)*640;
					fY=(fTV-0.5F)*480;
					fZ=579.5F-(fCurveY[dwX]*640);
					break;
				case(TDR_BTOT):
					fX=(fTU-0.5F)*640;
					fY=-((fCurveX[7-dwY]-(fPos*0.15F))-0.5F)*480;
					fZ=579.5F-(fCurveY[7-dwY]*480);
					break;
				case(TDR_TTOB):
					fX=(fTU-0.5F)*640;
					fY=((fCurveX[dwY]-(fPos*0.15F))-0.5F)*480;
					fZ=579.5F-(fCurveY[dwY]*480);
					break;
				}

				// set vertex
				SET_TRANSVERTEX(lpVtx[dwX+(dwY*8)],
								fX,fY,fZ,
								fTU,1-fTV);
			}

		// unlock vertex buffer
		UnlockVertexBuffer(g_dwTransVtxBuf);

		// point for future reference
		RenderTransition1:;

		// begin sprites
		if(BeginSprites(TRUE))
		{
			// output texture 2
			OutputSprite(g_lpdwTransData[TD_PEEL_TEX2],
						 ScrCoords(320,240),
						 ScrSize(640,480));

			// end sprites
			EndSprites();
		}
		
		// save projection matrix
		CopyMemory(&mProj,
				   &g_mtxProj,
				   sizeof(D3DXMATRIX));
		

		// output texture 1
		SetWorldMatrix();
		SetViewMatrix();
		SetProjectionMatrixA(1,1000,45);
		SetVertexBuffer(g_dwTransVtxBuf);
		SetIndexBuffer(g_dwTransIdxBuf);
		SetMaterial(g_dwSpriteMat);
		SetTexture(g_lpdwTransData[TD_PEEL_TEX1]);
		RenderPrimitive();

		// reset projection matrix
		SetProjectionMatrixA(&mProj);
		
		// ok
		break;

	// swing
	case(TT_SWING):
		// compute angle
		fAngle=fPos*PI*0.5F;

		// lock vertex buffer
		lpVtx=(LPTRANSVERTEX)LockVertexBuffer(g_dwTransVtxBuf);

		// check for error
		if(!lpVtx)
			break;

		// update vertex buffer
		for(dwY=0;dwY<2;dwY++)
			for(dwX=0;dwX<2;dwX++)
			{
				// compute texture coordinates
				fTU=(float)(dwX);
				fTV=(float)(dwY);
				
				// compute position based on direction
				switch(g_lpdwTransData[TD_PEEL_DIR])
				{
				case(TDR_LTOR):
					fX=-(((1-fTU)*cosf(fAngle))-0.5F)*640;
					fY=(fTV-0.5F)*480;
					fZ=579.5F-((1-fTU)*sinf(fAngle)*640);
					break;
				case(TDR_RTOL):
					fX=((fTU*cosf(fAngle))-0.5F)*640;
					fY=(fTV-0.5F)*480;
					fZ=579.5F-(fTU*sinf(fAngle)*640);
					break;
				case(TDR_BTOT):
					fX=(fTU-0.5F)*640;
					fY=-(((1-fTV)*cosf(fAngle))-0.5F)*480;
					fZ=579.5F-((1-fTV)*sinf(fAngle)*480);
					break;
				case(TDR_TTOB):
					fX=(fTU-0.5F)*640;
					fY=((fTV*cosf(fAngle))-0.5F)*480;
					fZ=579.5F-(fTV*sinf(fAngle)*480);
					break;
				}

				// set vertex
				SET_TRANSVERTEX(lpVtx[dwX+(dwY*2)],
								fX,fY,fZ,
								fTU,1-fTV);
			}

		// unlock vertex buffer
		UnlockVertexBuffer(g_dwTransVtxBuf);

		// jump to previous code
		goto RenderTransition1;

	// scroll
	case(TT_SCROLL):
		// set texture sizes
		fXSize1=1;
		fYSize1=1;
		fXSize2=1;
		fYSize2=1;

		// check direction
		switch(g_lpdwTransData[TD_SCROLL_DIR])
		{
		case(TDR_LTOR):
			fXPos1=fPos;
			fYPos1=0;
			fXPos2=fPos-1;
			fYPos2=0;
			break;
		case(TDR_RTOL):
			fXPos1=(1-fPos)-1;
			fYPos1=0;
			fXPos2=1-fPos;
			fYPos2=0;
			break;
		case(TDR_BTOT):
			fXPos1=0;
			fYPos1=fPos;
			fXPos2=0;
			fYPos2=fPos-1;
			break;
		case(TDR_TTOB):
			fXPos1=0;
			fYPos1=(1-fPos)-1;
			fXPos2=0;
			fYPos2=(1-fPos);
			break;
		}

		// point for future reference
		RenderTransition2:;
				
		// begin sprites
		if(BeginSprites(TRUE))
		{
			// render texture 1
			OutputSprite(g_lpdwTransData[TD_SCROLL_TEX1],
	 					 fXPos1,fYPos1,
						 fXSize1,fYSize1);
			
			// render texture 2
			OutputSprite(g_lpdwTransData[TD_SCROLL_TEX2],
	 					 fXPos2,fYPos2,
						 fXSize2,fYSize2);
			
			// end sprites
			EndSprites();
		}

		// ok
		break;

	// squish
	case(TT_SQUISH):
		// set texture sizes
		fXSize2=1;
		fYSize2=1;

		// check direction
		switch(g_lpdwTransData[TD_SQUISH_DIR])
		{
		case(TDR_LTOR):
			fXPos1=fPos*0.5F;
			fYPos1=0;
			fXSize1=1-fPos;
			fYSize1=1;
			fXPos2=fPos-1;
			fYPos2=0;
			break;
		case(TDR_RTOL):
			fXPos1=((1-fPos)-1)*0.5F;
			fYPos1=0;
			fXSize1=1-fPos;
			fYSize1=1;
			fXPos2=1-fPos;
			fYPos2=0;
			break;
		case(TDR_BTOT):
			fXPos1=0;
			fYPos1=fPos*0.5F;
			fXSize1=1;
			fYSize1=1-fPos;
			fXPos2=0;
			fYPos2=fPos-1;
			break;
		case(TDR_TTOB):
			fXPos1=0;
			fYPos1=((1-fPos)-1)*0.5F;
			fXSize1=1;
			fYSize1=1-fPos;
			fXPos2=0;
			fYPos2=(1-fPos);
			break;
		}

		// jump to previous code
		goto RenderTransition2;

	// slide
	case(TT_SLIDE):
		// check direction
		switch(g_lpdwTransData[TD_SLIDE_DIR])
		{
		case(TDR_LTOR):
			fXPos1=fPos*0.5F;
			fYPos1=0;
			fXSize1=1-fPos;
			fYSize1=1;
			fXPos2=(fPos-1)*0.5F;
			fYPos2=0;
			fXSize2=fPos;
			fYSize2=1;
			break;
		case(TDR_RTOL):
			fXPos1=((1-fPos)-1)*0.5F;
			fYPos1=0;
			fXSize1=1-fPos;
			fYSize1=1;
			fXPos2=(1-fPos)*0.5F;
			fYPos2=0;
			fXSize2=fPos;
			fYSize2=1;
			break;
		case(TDR_BTOT):
			fXPos1=0;
			fYPos1=fPos*0.5F;
			fXSize1=1;
			fYSize1=1-fPos;
			fXPos2=0;
			fYPos2=(fPos-1)*0.5F;
			fXSize2=1;
			fYSize2=fPos;
			break;
		case(TDR_TTOB):
			fXPos1=0;
			fYPos1=((1-fPos)-1)*0.5F;
			fXSize1=1;
			fYSize1=1-fPos;
			fXPos2=0;
			fYPos2=(1-fPos)*0.5F;
			fXSize2=1;
			fYSize2=fPos;
			break;
		}

		// jump to previous code
		goto RenderTransition2;

	// shatter
	case(TT_SHATTER):
		// render shatter
		RenderShatter(fPos);

		// ok
		break;
	}

	// end scene
	EndScene();

	// present scene
	PresentScene();

	// ok
	return(bContinue);
}
#endif

// InitMouseCursor()

BOOL InitMouseCursor(void)
{
	LoadingOutput("InitMouseCursor");

	// create texture
	g_dwDefMouseCursorTexIdx=AddTexture(4,4,
										TRUE);

	// check texture
	if(g_dwDefMouseCursorTexIdx==NO_OBJECT)
		return(FALSE);
	
	// lock texture
	D3DLOCKED_RECT lr=LockTexture(g_dwDefMouseCursorTexIdx);

	// check for error
	if(!lr.pBits)
		return(FALSE);

	// get texture surface data
	LPWORD lpData=(LPWORD)lr.pBits;
	DWORD dwPitch=lr.Pitch>>1;

	// set bits
	lpData[0+(0*dwPitch)]=0xFFFF;
	lpData[1+(0*dwPitch)]=0xFFFF;
	lpData[2+(0*dwPitch)]=0xFFFF;
	lpData[3+(0*dwPitch)]=0x0000;
	lpData[0+(1*dwPitch)]=0xFFFF;
	lpData[1+(1*dwPitch)]=0xFFFF;
	lpData[2+(1*dwPitch)]=0x0000;
	lpData[3+(1*dwPitch)]=0x0000;
	lpData[0+(2*dwPitch)]=0xFFFF;
	lpData[1+(2*dwPitch)]=0x0000;
	lpData[2+(2*dwPitch)]=0xFFFF;
	lpData[3+(2*dwPitch)]=0x0000;
	lpData[0+(3*dwPitch)]=0x0000;
	lpData[1+(3*dwPitch)]=0x0000;
	lpData[2+(3*dwPitch)]=0x0000;
	lpData[3+(3*dwPitch)]=0xFFFF;

	// unlock texture
	UnlockTexture(g_dwDefMouseCursorTexIdx);

	// ok
	return(TRUE);
}

// SetMouseCursor()

void SetMouseCursor(DWORD dwTexIndex,
					int nXHotSpot,
					int nYHotSpot,
					float fXSize,
					float fYSize)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetMouseCursor()",
								 FALSE);
		return;
	}
#endif

	// store values
	if(dwTexIndex==NO_OBJECT)
		g_dwMouseCursorTexIdx=g_dwDefMouseCursorTexIdx;
	else
		g_dwMouseCursorTexIdx=dwTexIndex;
	g_nMouseXHotSpot=nXHotSpot;
	g_nMouseYHotSpot=nYHotSpot;
	g_fMouseXSize=fXSize;
	g_fMouseYSize=fYSize;
}

// EnableMouseCursor()

void EnableMouseCursor(DWORD dwTexIndex,
					   int nXHotSpot,
					   int nYHotSpot,
					   float fXSize,
					   float fYSize)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "EnableMouseCursor()",
								 FALSE);
		return;
	}
#endif

	// set cursor
	SetMouseCursor(dwTexIndex,
				   nXHotSpot,
				   nYHotSpot,
				   fXSize,
				   fYSize);

	// set flag
	g_bMouseCursor=TRUE;
}

// DisableMouseCursor()

void DisableMouseCursor(void)
{
	// set flag
	g_bMouseCursor=FALSE;
}

// RenderMouseCursor()

void RenderMouseCursor(void)
{
	// check flags
	if(!g_bWinMouse)
		return;
	if(!g_bMouseCursor)
		return;

#ifdef DEBUG_MODE
	// verify index
	if(g_dwMouseCursorTexIdx>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderMouseCursor()",
								 FALSE);
		return;
	}
#endif

	// stored flags
	BOOL bAlphaTex;
	BOOL bAlphaTest;
	
	// check alpha textures flag
	if(!g_bAlphaTextures)
	{
		// enable alpha textures
		EnableAlphaTextures();

		// set flag
		bAlphaTex=FALSE;
	}
	else
	{
		// set flag
		bAlphaTex=TRUE;
	}
	
	// check alpha testing flag
	if(!g_bATest)
	{
		// enable alpha testing
		EnableAlphaTest();

		// set flag
		bAlphaTest=FALSE;
	}
	else
	{
		// set flag
		bAlphaTest=TRUE;
	}

	// begin sprites
	if(BeginSprites(TRUE))
	{
		// output sprite
		OutputSprite(g_dwMouseCursorTexIdx,
					 ScrCoords(g_nMouseX-g_nMouseXHotSpot,
							   g_nMouseY-g_nMouseYHotSpot),
					 g_fMouseXSize,
					 g_fMouseYSize);
		
		// end sprites
		EndSprites();	
	}

	// restore alpha testing
	if(!bAlphaTest)
		DisableAlphaTest();

	// restore alpha textures
	if(!bAlphaTex)
		DisableAlphaTextures();
}

#ifdef USE_TRANSITIONS
// InitShatter()

BOOL InitShatter(void)
{
	LoadingOutput("InitShatter");

	// allocate memory
	if(!AllocMem((LPVOID*)&g_lpfShatterVtxTU,
				 sizeof(float)*17))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterVtxTV,
				 sizeof(float)*17))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterCenTU,
				 sizeof(float)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterCenTV,
				 sizeof(float)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpvShatterVelocity,
				 sizeof(D3DXVECTOR3)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpdwShatterVtx1,
				 sizeof(DWORD)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpdwShatterVtx2,
				 sizeof(DWORD)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpdwShatterVtx3,
				 sizeof(DWORD)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterDeltaYaw,
				 sizeof(float)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterDeltaPitch,
				 sizeof(float)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterDeltaRoll,
				 sizeof(float)*20))
		return(FALSE);

	// macro for setting vertex indices
	#define SET_SHATTER_INDICES(v,v1,v2,v3) { g_lpdwShatterVtx1[v]=v1; g_lpdwShatterVtx2[v]=v2; g_lpdwShatterVtx3[v]=v3; }
	
	// init vertex indices
	SET_SHATTER_INDICES(0, 0, 1, 5);
	SET_SHATTER_INDICES(1, 5, 1, 2);
	SET_SHATTER_INDICES(2, 5, 2, 6);
	SET_SHATTER_INDICES(3, 2, 3, 6);
	SET_SHATTER_INDICES(4, 6, 3, 4);
	SET_SHATTER_INDICES(5, 7, 5, 0);
	SET_SHATTER_INDICES(6, 6, 9, 4);
	SET_SHATTER_INDICES(7, 7, 5, 10);
	SET_SHATTER_INDICES(8, 6, 9, 11);
	SET_SHATTER_INDICES(9, 5, 8, 6);
	SET_SHATTER_INDICES(10,6, 8, 11);
	SET_SHATTER_INDICES(11,11,8, 10);
	SET_SHATTER_INDICES(12,10,5, 8);
	SET_SHATTER_INDICES(13,7, 12,10);
	SET_SHATTER_INDICES(14,11,9, 16);
	SET_SHATTER_INDICES(15,12,13,10);
	SET_SHATTER_INDICES(16,13,14,10);
	SET_SHATTER_INDICES(17,14,10,11);
	SET_SHATTER_INDICES(18,14,15,11);
	SET_SHATTER_INDICES(19,15,16,11);
	
	// ok
	return(TRUE);
}

// ReleaseShatter()

void ReleaseShatter(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpfShatterVtxTU);
	FreeMem((LPVOID*)&g_lpfShatterVtxTV);
	FreeMem((LPVOID*)&g_lpfShatterCenTU);
	FreeMem((LPVOID*)&g_lpfShatterCenTV);
	FreeMem((LPVOID*)&g_lpvShatterVelocity);
	FreeMem((LPVOID*)&g_lpdwShatterVtx1);
	FreeMem((LPVOID*)&g_lpdwShatterVtx2);
	FreeMem((LPVOID*)&g_lpdwShatterVtx3);
	FreeMem((LPVOID*)&g_lpfShatterDeltaYaw);
	FreeMem((LPVOID*)&g_lpfShatterDeltaPitch);
	FreeMem((LPVOID*)&g_lpfShatterDeltaRoll);
}

// RandomizeShatter()

void RandomizeShatter(void)
{
	// counter
	DWORD dwCount;
	
	// macro for setting vertices
	#define SET_SHATTER_VERTEX(vtx,tu,tv) { g_lpfShatterVtxTU[vtx]=tu; g_lpfShatterVtxTV[vtx]=tv; }
	
	// create vertex array
	SET_SHATTER_VERTEX(0, 0.000F                          ,0.000F);
	SET_SHATTER_VERTEX(1, 0.250F+(GetRandomFloat()*0.112F),0.000F);
	SET_SHATTER_VERTEX(2, 0.500F+(GetRandomFloat()*0.112F),0.000F);
	SET_SHATTER_VERTEX(3, 0.750F+(GetRandomFloat()*0.112F),0.000F);
	SET_SHATTER_VERTEX(4, 1.000F                          ,0.000F);
	SET_SHATTER_VERTEX(5, 0.250F+(GetRandomFloat()*0.150F),0.333F+(GetRandomFloat()*0.150F));
	SET_SHATTER_VERTEX(6, 0.750F+(GetRandomFloat()*0.150F),0.333F+(GetRandomFloat()*0.150F));
	SET_SHATTER_VERTEX(7, 0.000F                          ,0.500F+(GetRandomFloat()*0.450F));
	SET_SHATTER_VERTEX(8, 0.500F+(GetRandomFloat()*0.075F),0.500F+(GetRandomFloat()*0.150F));
	SET_SHATTER_VERTEX(9, 1.000F                          ,0.500F+(GetRandomFloat()*0.450F));
	SET_SHATTER_VERTEX(10,0.250F+(GetRandomFloat()*0.150F),0.666F+(GetRandomFloat()*0.150F));
	SET_SHATTER_VERTEX(11,0.750F+(GetRandomFloat()*0.150F),0.666F+(GetRandomFloat()*0.150F));
	SET_SHATTER_VERTEX(12,0.000F                          ,1.000F);
	SET_SHATTER_VERTEX(13,0.250F+(GetRandomFloat()*0.112F),1.000F);
	SET_SHATTER_VERTEX(14,0.500F+(GetRandomFloat()*0.112F),1.000F);
	SET_SHATTER_VERTEX(15,0.750F+(GetRandomFloat()*0.112F),1.000F);
	SET_SHATTER_VERTEX(16,1.000F                          ,1.000F);

	// process each triangle
	for(dwCount=0;dwCount<20;dwCount++)
	{
		// compute centroid
		g_lpfShatterCenTU[dwCount]=(g_lpfShatterVtxTU[g_lpdwShatterVtx1[dwCount]]+
									g_lpfShatterVtxTU[g_lpdwShatterVtx2[dwCount]]+
									g_lpfShatterVtxTU[g_lpdwShatterVtx3[dwCount]])/3;
		g_lpfShatterCenTV[dwCount]=(g_lpfShatterVtxTV[g_lpdwShatterVtx1[dwCount]]+
									g_lpfShatterVtxTV[g_lpdwShatterVtx2[dwCount]]+
									g_lpfShatterVtxTV[g_lpdwShatterVtx3[dwCount]])/3;

		// create random velocity
		g_lpvShatterVelocity[dwCount]=D3DXVECTOR3(GetRandomFloat()*128,
												  -(640+(GetRandomFloat()*48)),
												  GetRandomFloat()*128);

		// create random orientation values
		g_lpfShatterDeltaYaw[dwCount]=GetRandomFloat()*PI*1.5F;
		g_lpfShatterDeltaPitch[dwCount]=GetRandomFloat()*PI*1.5F;
		g_lpfShatterDeltaRoll[dwCount]=GetRandomFloat()*PI*1.5F;
	}
}

// RenderShatter()

void RenderShatter(float fPos)
{
	// counters
	DWORD dwCount1;
	DWORD dwCount2;

	// save position
	float fOrigPos=fPos;
	
	// lock vertex buffer
	LPTRANSVERTEX lpVtx=(LPTRANSVERTEX)LockVertexBuffer(g_dwTransVtxBuf);

	// check for error
	if(!lpVtx)
		return;

	// update vertex buffer
	for(dwCount1=0;dwCount1<20;dwCount1++)
	{
		// adjust position for this piece
		float fPos=(fOrigPos*2)-((float)(dwCount1)/20);

		// check for overflow
		if(fPos<0)
			fPos=0;
		
		// adjust position to follow curve
		fPos=1-sqrtf(1-Squared(fPos));
		
		// get centroid
		float fCenTU=g_lpfShatterCenTU[dwCount1];
		float fCenTV=g_lpfShatterCenTV[dwCount1];
		
		// create rotation matrix
		D3DXMATRIX mTri;
		D3DXMatrixRotationYawPitchRoll(&mTri,
									   g_lpfShatterDeltaYaw[dwCount1]*fPos,
									   g_lpfShatterDeltaPitch[dwCount1]*fPos,
									   g_lpfShatterDeltaRoll[dwCount1]*fPos);

		// insert position into matrix
		mTri._41=((fCenTU-0.5F)*640)+(g_lpvShatterVelocity[dwCount1].x*fPos);
		mTri._42=((fCenTV-0.5F)*480)+(g_lpvShatterVelocity[dwCount1].y*fPos);
		mTri._43=579.5F             +(g_lpvShatterVelocity[dwCount1].z*fPos);
		
		// output each vertex
		for(dwCount2=0;dwCount2<3;dwCount2++)
		{
			// vertex coordinates
			float fTU;
			float fTV;
			
			// read vertex coordinates
			switch(dwCount2)
			{
			case(0):
				fTU=g_lpfShatterVtxTU[g_lpdwShatterVtx1[dwCount1]];
				fTV=g_lpfShatterVtxTV[g_lpdwShatterVtx1[dwCount1]];
				break;
			case(1):
				fTU=g_lpfShatterVtxTU[g_lpdwShatterVtx2[dwCount1]];
				fTV=g_lpfShatterVtxTV[g_lpdwShatterVtx2[dwCount1]];
				break;
			case(2):
				fTU=g_lpfShatterVtxTU[g_lpdwShatterVtx3[dwCount1]];
				fTV=g_lpfShatterVtxTV[g_lpdwShatterVtx3[dwCount1]];
				break;
			}

			// compute vector
			D3DXVECTOR3 vVec;
			vVec.x=(fTU-fCenTU)*640;
			vVec.y=(fTV-fCenTV)*480;
			vVec.z=0;

			// transform vector into vertex
			D3DXVECTOR3 vVtx;
			TransformVector(&vVtx,
							&vVec,
							&mTri);

			// output vertex
			SET_TRANSVERTEX(lpVtx[(dwCount1*3)+dwCount2],
							vVtx.x,
							vVtx.y,
							vVtx.z,
							fTU,
							1-fTV);
		}
	}

	// unlock vertex buffer
	UnlockVertexBuffer(g_dwTransVtxBuf);

	// begin sprites
	if(BeginSprites(TRUE))
	{
		// output texture 2
		OutputSprite(g_lpdwTransData[TD_SHATTER_TEX2],
					 ScrCoords(320,240),
					 ScrSize(640,480));

		// end sprites
		EndSprites();
	}
	
	// save projection matrix
	D3DXMATRIX mProj;
	CopyMemory(&mProj,
			   &g_mtxProj,
			   sizeof(D3DXMATRIX));

	// output texture 1
	SetWorldMatrix();
	SetViewMatrix();
	SetProjectionMatrixA(1,1000,45);
	SetVertexBuffer(g_dwTransVtxBuf);
	SetIndexBuffer(g_dwTransIdxBuf);
	SetMaterial(g_dwSpriteMat);
	SetTexture(g_lpdwTransData[TD_SHATTER_TEX1]);
	RenderPrimitive();

	// reset projection matrix
	SetProjectionMatrixA(&mProj);
}
#endif

// ClassifyPoint()

DWORD ClassifyPoint(LPD3DXVECTOR3 lpvPO,
					LPD3DXVECTOR3 lpvPN,
					LPD3DXVECTOR3 lpvP)
{
	// compute d-vector
	D3DXVECTOR3 vD=(*lpvPO)-(*lpvP);

	// compute d-value
	float fD=DotProduct(&vD,lpvPN);

	// check d-value
	if(fD<0)
		// in front
		return(PLANE_FRONT);
	else if(fD>0)
		// in back
		return(PLANE_BACK);
	else
		// on plane
		return(PLANE_ON);
}

// DistanceToRayPlaneIntersection()

float DistanceToRayPlaneIntersection(LPD3DXVECTOR3 lpvRO,
									 LPD3DXVECTOR3 lpvRN,
									 LPD3DXVECTOR3 lpvPO,
									 LPD3DXVECTOR3 lpvPN)
{
	// compute v-value
	float fV=DotProduct(lpvPN,
						lpvRN);
	
	// check v-value
	if(fV==0)
		// no intersection possible
		return(-1);

	// compute d-value
	float fD=-DotProduct(lpvPN,
						 lpvPO);

	// compute u-value
	float fU=DotProduct(lpvPN,
						lpvRO)+fD;
	
	// compute distance
	return(-(fU/fV));
}

// Get2DBarycentricCoordinates()

BOOL Get2DBarycentricCoordinates(float fX1,
								 float fY1,
								 float fX2,
								 float fY2,
								 float fX3,
								 float fY3,
								 float fXP,
								 float fYP,
								 float *lpfB1,
								 float *lpfB2,
								 float *lpfB3)
{
	// compute denominator
	float fB0=((fX2-fX1)*(fY3-fY1))-
			  ((fX3-fX1)*(fY2-fY1));

	// check for zero-area polygon
	if(fB0==0)
		// no good
		return(FALSE);
	else
	{
		// compute reciprocal
		fB0=1/fB0;

		// compute coordinates
		(*lpfB1)=(((fX2-fXP)*(fY3-fYP))-
			      ((fX3-fXP)*(fY2-fYP)))*fB0;
		(*lpfB2)=(((fX3-fXP)*(fY1-fYP))-
				  ((fX1-fXP)*(fY3-fYP)))*fB0;
		(*lpfB3)=1.0F-(*lpfB1)-(*lpfB2);
		
		// ok
		return(TRUE);
	}
}
									
// IsPointInTriangle()

BOOL IsPointInTriangle(LPD3DXVECTOR3 lpvP1,
					   LPD3DXVECTOR3 lpvP2,
					   LPD3DXVECTOR3 lpvP3,
					   LPD3DXVECTOR3 lpvP)
{
	// barycentric coordinates
	float fB1;
	float fB2;
	float fB3;

	// check projection on X-Y plane
	if(!Get2DBarycentricCoordinates(lpvP1->x,lpvP1->y,
									lpvP2->x,lpvP2->y,
									lpvP3->x,lpvP3->y,
									lpvP->x, lpvP->y,
									&fB1,&fB2,&fB3))
	{
		// check projection on X-Z plane
		if(!Get2DBarycentricCoordinates(lpvP1->x,lpvP1->z,
										lpvP2->x,lpvP2->z,
										lpvP3->x,lpvP3->z,
										lpvP->x, lpvP->z,
										&fB1,&fB2,&fB3))
		{
			// get projection on Y-Z plane
			if(!Get2DBarycentricCoordinates(lpvP1->y,lpvP1->z,
											lpvP2->y,lpvP2->z,
											lpvP3->y,lpvP3->z,
											lpvP->y, lpvP->z,
											&fB1,&fB2,&fB3))
			{
				DisplayErrorMessage("Undefined barycentric coordinates.",
									NULL,
									FALSE);
				return(FALSE);
			}
		}
	}

	// check coordinates
	if(fB1>=0&&
	   fB2>=0&&
	   fB3>=0)
		// ok
		return(TRUE);
	else
		// no good
		return(FALSE);
}
									 
// GetNearestPointOnLine()

void GetNearestPointOnLine(LPD3DXVECTOR3 lpvL1,
						   LPD3DXVECTOR3 lpvL2,
						   LPD3DXVECTOR3 lpvP,
						   LPD3DXVECTOR3 lpvNP)

{
	// compute vectors
	D3DXVECTOR3 vC=(*lpvP)-(*lpvL1);
	D3DXVECTOR3 vV=(*lpvL2)-(*lpvL1);

	// compute d-value
	float fD=Magnitude(vV);

	// normalize v-vector
	vV/=fD;
	
	// compute t-value
	float fT=DotProduct(&vV,
						&vC);
	
	// check t-value
	if(fT<=0)
		// use point #1
		(*lpvNP)=(*lpvL1);
	else if(fT>=fD)
		// use point #2
		(*lpvNP)=(*lpvL2);
	else
		// use point between 1 and 2
		(*lpvNP)=(*lpvL1)+(vV*fT);
}

// GetNearestPointOnTriangle()

void GetNearestPointOnTriangle(LPD3DXVECTOR3 lpvP1,
							   LPD3DXVECTOR3 lpvP2,
							   LPD3DXVECTOR3 lpvP3,
							   LPD3DXVECTOR3 lpvP,
							   LPD3DXVECTOR3 lpvNP)
{
	// nearest points on lines
	D3DXVECTOR3 vV1;
	D3DXVECTOR3 vV2;
	D3DXVECTOR3 vV3;
	
	// get closest points on lines
	GetNearestPointOnLine(lpvP1,lpvP2,lpvP,&vV1);
	GetNearestPointOnLine(lpvP2,lpvP3,lpvP,&vV2);
	GetNearestPointOnLine(lpvP3,lpvP1,lpvP,&vV3);

	// compute vectors to points
	D3DXVECTOR3 vPV1=(*lpvP)-vV1;
	D3DXVECTOR3 vPV2=(*lpvP)-vV2;
	D3DXVECTOR3 vPV3=(*lpvP)-vV3;

	// compute square distances
	float fD1=SquareMagnitude(&vPV1);
	float fD2=SquareMagnitude(&vPV2);
	float fD3=SquareMagnitude(&vPV3);

	// find lowest distance
	if(fD1<fD2)
	{
		if(fD1<fD3)
			// use point #1
			(*lpvNP)=vV1;
		else
			// use point #3
			(*lpvNP)=vV3;
	}
	else
	{
		if(fD2<fD3)
			// use point #2
			(*lpvNP)=vV2;
		else
			// use point #3
			(*lpvNP)=vV3;
	}
}

// DistanceToRaySphereIntersection()

float DistanceToRaySphereIntersection(LPD3DXVECTOR3 lpvRO,
									  LPD3DXVECTOR3 lpvRN,
									  LPD3DXVECTOR3 lpvSO)
{
	// compute q-vector
	D3DXVECTOR3 vQ=(*lpvSO)-(*lpvRO);

	// compute equation values
	float fC=Magnitude(&vQ);
	float fV=DotProduct(&vQ,
						lpvRN);
	float fD=1-(Squared(fC)-Squared(fV));
	
	// check d-value
	if(fD<0)
		// no intersection
		return(-1);
	else
		// compute distance to nearest point
		return(fV-sqrtf(fD));
}

// IsPointInSphere()

BOOL IsPointInSphere(LPD3DXVECTOR3 lpvP,
					 LPD3DXVECTOR3 lpvSO)
{
	// get vector between points
	D3DXVECTOR3 vVec=(*lpvP)-(*lpvSO);
	
	// check square length
	if(SquareMagnitude(&vVec)<=1)
		return(TRUE);
	else
		return(FALSE);
}

// SwapTextureSurfaces()

void SwapTextureSurfaces(DWORD dwIndex1,
						 DWORD dwIndex2)
{
#ifdef DEBUG_MODE
	// verify indices
	if(dwIndex1>=g_dwNumTex||
	   dwIndex2>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SwapTextureSurfaces()",
								 FALSE);
		return;
	}
#endif

	// swap surfaces
	IDirect3DTexture8* lpTemp=g_lplpTex[dwIndex1];
	g_lplpTex[dwIndex1]=g_lplpTex[dwIndex2];
	g_lplpTex[dwIndex2]=lpTemp;
}

// VerifyVideoDeviceCaps()

BOOL VerifyVideoDeviceCaps(void)
{
	LoadingOutput("VerifyVideoDeviceCaps");

	// problem flag
	BOOL bProblem=FALSE;
	
	// check alpha capabilities
	if(ApplicationAlphaRequired())
		if(!(g_lpDevCaps.SrcBlendCaps&D3DPBLENDCAPS_SRCALPHA)&&
		   !(g_lpDevCaps.DestBlendCaps&D3DPBLENDCAPS_INVSRCALPHA)&&
		   !(g_lpDevCaps.AlphaCmpCaps&D3DPCMPCAPS_GREATER)&&
		   !(g_lpDevCaps.TextureCaps&D3DPTEXTURECAPS_ALPHA))
			bProblem=TRUE;
	
	// check fog capabilities
	if(ApplicationFogRequired())
		if(!(g_lpDevCaps.RasterCaps&D3DPRASTERCAPS_FOGVERTEX))
			bProblem=TRUE;
	
	// check cull modes
	if(!(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_CULLCCW)&&
	   !(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_CULLCW)&&
	   !(g_lpDevCaps.PrimitiveMiscCaps&D3DPMISCCAPS_CULLNONE))
		bProblem=TRUE;
	
	// check texture addressing mode
	if(!(g_lpDevCaps.TextureAddressCaps&D3DPTADDRESSCAPS_WRAP))
		bProblem=TRUE;

#ifdef USE_AVWARNINGS
	// check flag
	if(bProblem)
	{
		// check registry for flag
		LPSTR lpFlag=GetRegistryKey("DXNoVidMsg");
		if(!lpFlag)
		{
			// display message box
			DisplayInfoMessage("Your display adapter does not support a required option.\n\n"
							   "Some video artifacts or other disruptions may occur.\n\nThis message will not be displayed again.");

			// set flag
			SetRegistryKey("DXNoVidMsg",
						   "TRUE");
		}
		else
		{
			// free key value
			FreeRegistryKey(lpFlag);
		}
	}
#endif

	// ok
	return(TRUE);
}

// EnableBillboards()

void EnableBillboards(void)
{
	// set flag
	g_bBBEnabled=TRUE;
}

// DisableBillboards()

void DisableBillboards(void)
{
	// set flag
	g_bBBEnabled=FALSE;
}

// EnableVerticalBillboards()

void EnableVerticalBillboards(void)
{
	// set flag
	g_bVBBEnabled=TRUE;
}

// DisableVerticalBillboards()

void DisableVerticalBillboards(void)
{
	// set flag
	g_bVBBEnabled=FALSE;
}

// RenderVerticalBillboard()

void RenderVerticalBillboard(DWORD dwMatIndex,
							 DWORD dwTexIndex,
							 LPD3DXVECTOR3 lpvPos,
							 float fDiameter,
							 float fHeight)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMatIndex>=g_dwNumMat&&
	   dwMatIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "RenderVerticalBillboard()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderVerticalBillboard()",
								 FALSE);
		return;
	}
#endif

	// render list of one billboard
	RenderVerticalBillboards(dwMatIndex,
							 dwTexIndex,
							 lpvPos,
							 1,
							 fDiameter,
							 fHeight);
}

// RenderVerticalBillboards()

void RenderVerticalBillboards(DWORD dwMatIndex,
							  DWORD dwTexIndex,
							  LPD3DXVECTOR3 lpvPos,
							  DWORD dwNumPos,
							  float fDiameter,
							  float fHeight)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMatIndex>=g_dwNumMat&&
	   dwMatIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "RenderVerticalBillboards()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderVerticalBillboards()",
								 FALSE);
		return;
	}
#endif
	
	// set render states
	SetMaterial(dwMatIndex);
	SetTexture(dwTexIndex);
	SetVertexBuffer(g_dwBBVtxBufIdx);
	SetIndexBuffer(g_dwBBIdxBufIdx);

	// set size
	g_lpMtx[g_dwVBBMtxIndex].fXScale=fDiameter;
	g_lpMtx[g_dwVBBMtxIndex].fYScale=fHeight;
	g_lpMtx[g_dwVBBMtxIndex].fZScale=fDiameter;

	// render each billboard
	DWORD dwCount;
	for(dwCount=0;dwCount<dwNumPos;dwCount++)
	{
		// set position
		g_lpMtx[g_dwVBBMtxIndex].mtx._41=lpvPos[dwCount].x;
		g_lpMtx[g_dwVBBMtxIndex].mtx._42=lpvPos[dwCount].y;
		g_lpMtx[g_dwVBBMtxIndex].mtx._43=lpvPos[dwCount].z;

		// set world matrix
		SetWorldMatrix(g_dwVBBMtxIndex);

		// render billboard
		RenderPrimitive();
	}
}

// RenderReverseVerticalBillboard()

void RenderReverseVerticalBillboard(DWORD dwMatIndex,
								    DWORD dwTexIndex,
									LPD3DXVECTOR3 lpvPos,
									float fDiameter,
									float fHeight)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMatIndex>=g_dwNumMat&&
	   dwMatIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "RenderReverseVerticalBillboard()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderReverseVerticalBillboard()",
								 FALSE);
		return;
	}
#endif

	// render list of one reverse vertical billboard
	RenderReverseVerticalBillboards(dwMatIndex,
									dwTexIndex,
									lpvPos,
									1,
									fDiameter,
									fHeight);
}

// RenderReverseVerticalBillboards()

void RenderReverseVerticalBillboards(DWORD dwMatIndex,
									 DWORD dwTexIndex,
									 LPD3DXVECTOR3 lpvPos,
									 DWORD dwNumPos,
									 float fDiameter,
									 float fHeight)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMatIndex>=g_dwNumMat&&
	   dwMatIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "RenderReverseVerticalBillboards()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderReverseVerticalBillboards()",
								 FALSE);
		return;
	}
#endif
	
	// set render states
	SetMaterial(dwMatIndex);
	SetTexture(dwTexIndex);
	SetVertexBuffer(g_dwBBRevVtxBufIdx);
	SetIndexBuffer(g_dwBBIdxBufIdx);

	// set size
	g_lpMtx[g_dwVBBMtxIndex].fXScale=fDiameter;
	g_lpMtx[g_dwVBBMtxIndex].fYScale=fHeight;
	g_lpMtx[g_dwVBBMtxIndex].fZScale=fDiameter;

	// render each billboard
	DWORD dwCount;
	for(dwCount=0;dwCount<dwNumPos;dwCount++)
	{
		// set position
		g_lpMtx[g_dwVBBMtxIndex].mtx._41=lpvPos[dwCount].x;
		g_lpMtx[g_dwVBBMtxIndex].mtx._42=lpvPos[dwCount].y;
		g_lpMtx[g_dwVBBMtxIndex].mtx._43=lpvPos[dwCount].z;

		// set world matrix
		SetWorldMatrix(g_dwVBBMtxIndex);

		// render billboard
		RenderPrimitive();
	}
}

// IsZeroMatrix()

BOOL IsZeroMatrix(LPD3DXMATRIX lpMtx)
{
	// check values
	if(IsNearZero(lpMtx->_11)&&
	   IsNearZero(lpMtx->_12)&&
	   IsNearZero(lpMtx->_13)&&
	   IsNearZero(lpMtx->_14)&&
	   IsNearZero(lpMtx->_21)&&
	   IsNearZero(lpMtx->_22)&&
	   IsNearZero(lpMtx->_23)&&
	   IsNearZero(lpMtx->_24)&&
	   IsNearZero(lpMtx->_31)&&
	   IsNearZero(lpMtx->_32)&&
	   IsNearZero(lpMtx->_33)&&
	   IsNearZero(lpMtx->_34)&&
	   IsNearZero(lpMtx->_41)&&
	   IsNearZero(lpMtx->_42)&&
	   IsNearZero(lpMtx->_43)&&
	   IsNearZero(lpMtx->_44))
		return(TRUE);
	else
		return(FALSE);
}

// FixZeroMatrix()

void FixZeroMatrix(LPD3DXMATRIX lpMtx)
{
	// convert matrix to identity if necessary
	if(IsZeroMatrix(lpMtx))
		(*lpMtx)=g_mtxIdentity;
}

// InitAlphaMeshes()

BOOL InitAlphaMeshes(void)
{
	LoadingOutput("InitAlphaMeshes");

	// add material
	g_dwAlphaMeshMat=AddMaterial(0,0,0,0,
								 0,0,0,0,
							     0,0,0,0,
							     0,0,0,0,
							     0);

	// check material
	if(g_dwAlphaMeshMat==NO_OBJECT)
		return(FALSE);

	// ok
	return(TRUE);
}

#ifdef DEBUG_MODE
// CreateLensFlareDDS()

void CreateLensFlareDDS(LPCSTR lpFilename,
						LPVOID lpFlareData)
{
	LoadingOutput("CreateLensFlareDDS");

	// flare type
	DWORD dwFlareType;

	// flare info
	LPLF_MAINFLAREINFO lpMFI;
	LPLF_FLARERINGINFO lpFRI;

	// texture size
	DWORD dwTexSize;
	float fTexSize;

	// texture index
	DWORD dwTex;

	// locked rectangle data
	D3DLOCKED_RECT lr;

	// locked texture data
	LPWORD lpData;
	DWORD dwPitch;

	// coordinates
	DWORD dwX;
	DWORD dwY;

	// radius values
	float fRad1;
	float fRad2;

	// color components
	float fR;
	float fG;
	float fB;
	float fA;
	float fR1;
	float fG1;
	float fB1;
	float fA1;
	float fR2;
	float fG2;
	float fB2;
	float fA2;

	// intensity values
	float fInt;
	float fInt1;
	float fInt2;

	// number of rays
	DWORD dwNumRays;
	
	// counter
	DWORD dwCount;

	// angle values
	float fAngle;
	float fDX;
	float fDY;

	// extract flare type from data
	dwFlareType=*((LPDWORD)lpFlareData);

	// process data based on flare type
	switch(dwFlareType)
	{
	// main flare
	case(LF_MAINFLARE):

		// cast pointer to flare info
		lpMFI=(LPLF_MAINFLAREINFO)lpFlareData;

		// save texture size
		dwTexSize=lpMFI->dwTexSize;
		fTexSize=(float)dwTexSize;

		// add texture
		dwTex=AddTexture(dwTexSize,
						 dwTexSize,
						 TRUE,
						 TRUE);

		// lock texture
		lr=LockTexture(dwTex);

		// extract data pointer & pitch
		lpData=(LPWORD)lr.pBits;
		dwPitch=lr.Pitch>>1;

		// extract glow data
		fRad1=lpMFI->fGlowDia*0.5F;
		fR1=GetR888(lpMFI->cGlowClr1);
		fG1=GetG888(lpMFI->cGlowClr1);
		fB1=GetB888(lpMFI->cGlowClr1);
		fA1=GetA888(lpMFI->cGlowClr1);
		fR2=GetR888(lpMFI->cGlowClr2);
		fG2=GetG888(lpMFI->cGlowClr2);
		fB2=GetB888(lpMFI->cGlowClr2);
		fA2=GetA888(lpMFI->cGlowClr2);

		// render glow
		for(dwX=0;dwX<dwTexSize;dwX++)
			for(dwY=0;dwY<dwTexSize;dwY++)
			{
				// save coordinates
				float fX=(float)dwX;
				float fY=(float)dwY;

				// adjust coordinates
				fX=(fX-(fTexSize*0.5F))/fTexSize;
				fY=(fY-(fTexSize*0.5F))/fTexSize;
				
				// compute distance to center
				float fDist=Magnitude(fX,fY,0);

				// compute intensity
				if(fDist<fRad1)
					fInt=cosf((fRad1-fDist)*lpMFI->fGlowFalloff*PI*2.0F);
				else
					fInt=1;

				// clip intensity
				if(fInt<0.0F) fInt=0.0F;
				if(fInt>1.0F) fInt=1.0F;

				// set color
				SetRGBA444(lpData[dwX+(dwY*dwPitch)],
						   GetInterpolatedValue(fR1,fR2,fInt),
						   GetInterpolatedValue(fG1,fG2,fInt),
						   GetInterpolatedValue(fB1,fB2,fInt),
						   GetInterpolatedValue(fA1,fA2,fInt));
			}

		// extract ring data
		fRad1=lpMFI->fRingDia1*0.5F;
		fRad2=lpMFI->fRingDia2*0.5F;
		fR1=GetR888(lpMFI->cRingClr1);
		fG1=GetG888(lpMFI->cRingClr1);
		fB1=GetB888(lpMFI->cRingClr1);
		fA1=GetA888(lpMFI->cRingClr1);
		fR2=GetR888(lpMFI->cRingClr2);
		fG2=GetG888(lpMFI->cRingClr2);
		fB2=GetB888(lpMFI->cRingClr2);
		fA2=GetA888(lpMFI->cRingClr2);

		// render ring
		for(dwX=0;dwX<dwTexSize;dwX++)
			for(dwY=0;dwY<dwTexSize;dwY++)
			{
				// extract color value
				WORD wC=lpData[dwX+(dwY*dwPitch)];
				
				// extract color components
				fR=GetR444(wC);
				fG=GetG444(wC);
				fB=GetB444(wC);
				fA=GetA444(wC);
				
				// save coordinates
				float fX=(float)dwX;
				float fY=(float)dwY;

				// adjust coordinates
				fX=(fX-(fTexSize*0.5F))/fTexSize;
				fY=(fY-(fTexSize*0.5F))/fTexSize;
				
				// compute distance to center
				float fDist=Magnitude(fX,fY,0);

				// compute intensity
				if(fDist>fRad1&&fDist<fRad2)
					fInt=cosf(GetInterpolationValue(fRad1,fRad2,fDist)*lpMFI->fRingFalloff*PI);
				else
					fInt=1;

				// clip intensity
				if(fInt<0.0F) fInt=0.0F;
				if(fInt>1.0F) fInt=1.0F;

				// combine with current color
				fR+=GetInterpolatedValue(fR1,fR2,fInt);
				fG+=GetInterpolatedValue(fG1,fG2,fInt);
				fB+=GetInterpolatedValue(fB1,fB2,fInt);
				fA+=GetInterpolatedValue(fA1,fA2,fInt);

				// clip components
				if(fR>1.0F) fR=1.0F;
				if(fG>1.0F) fG=1.0F;
				if(fB>1.0F) fB=1.0F;
				if(fA>1.0F) fA=1.0F;

				// set new color
				SetRGBA444(lpData[dwX+(dwY*dwPitch)],
						   fR,fG,fB,fA);
			}

		// extract streak data
		fRad1=lpMFI->fStreakDia*0.5F;
		fRad2=lpMFI->fStreakWidth*0.5F;
		fR1=GetR888(lpMFI->cStreakClr1);
		fG1=GetG888(lpMFI->cStreakClr1);
		fB1=GetB888(lpMFI->cStreakClr1);
		fA1=GetA888(lpMFI->cStreakClr1);
		fR2=GetR888(lpMFI->cStreakClr2);
		fG2=GetG888(lpMFI->cStreakClr2);
		fB2=GetB888(lpMFI->cStreakClr2);
		fA2=GetA888(lpMFI->cStreakClr2);

		// render streaks
		for(dwX=0;dwX<dwTexSize;dwX++)
			for(dwY=0;dwY<dwTexSize;dwY++)
			{
				// extract color value
				WORD wC=lpData[dwX+(dwY*dwPitch)];
				
				// extract color components
				fR=GetR444(wC);
				fG=GetG444(wC);
				fB=GetB444(wC);
				fA=GetA444(wC);
				
				// save coordinates
				float fX=(float)dwX;
				float fY=(float)dwY;

				// adjust coordinates
				fX=(fX-(fTexSize*0.5F))/fTexSize;
				fY=(fY-(fTexSize*0.5F))/fTexSize;

				// distance value
				float fDist;
				
				// adjust coordinates for horizontal streak
				float fX1=fX*(fRad1/fRad2);
				
				// compute distance to center
				fDist=Magnitude(fX1,fY,0);

				// compute intensity
				if(fDist<fRad1)
					fInt1=cosf((fRad1-fDist)*lpMFI->fStreakFalloff*PI*2.0F);
				else
					fInt1=1;

				// clip intensity
				if(fInt1<0.0F) fInt1=0.0F;
				if(fInt1>1.0F) fInt1=1.0F;

				// adjust coordinates for vertical streak
				float fY1=fY*(fRad1/fRad2);

				// compute distance to center
				fDist=Magnitude(fX,fY1,0);

				// compute intensity
				if(fDist<fRad1)
					fInt2=cosf((fRad1-fDist)*lpMFI->fStreakFalloff*PI*2.0F);
				else
					fInt2=1;

				// clip intensity
				if(fInt2<0.0F) fInt2=0.0F;
				if(fInt2>1.0F) fInt2=1.0F;

				// compute average intensity
				fInt=(fInt1+fInt2)*0.5F;

				// combine with current color
				fR+=GetInterpolatedValue(fR1,fR2,fInt);
				fG+=GetInterpolatedValue(fG1,fG2,fInt);
				fB+=GetInterpolatedValue(fB1,fB2,fInt);
				fA+=GetInterpolatedValue(fA1,fA2,fInt);

				// clip components
				if(fR>1.0F) fR=1.0F;
				if(fG>1.0F) fG=1.0F;
				if(fB>1.0F) fB=1.0F;
				if(fA>1.0F) fA=1.0F;

				// set new color
				SetRGBA444(lpData[dwX+(dwY*dwPitch)],
						   fR,fG,fB,fA);
			}

		// extract ray data
		dwNumRays=lpMFI->dwNumRays;
		fR1=GetR888(lpMFI->cRayClr1);
		fG1=GetG888(lpMFI->cRayClr1);
		fB1=GetB888(lpMFI->cRayClr1);
		fA1=GetA888(lpMFI->cRayClr1);
		fR2=GetR888(lpMFI->cRayClr2);
		fG2=GetG888(lpMFI->cRayClr2);
		fB2=GetB888(lpMFI->cRayClr2);
		fA2=GetA888(lpMFI->cRayClr2);

		// render rays
		for(dwCount=0;dwCount<dwNumRays;dwCount++)
		{
			// set radius values
			fRad1=(lpMFI->fMaxRayDia*0.5F)*((fabsf(GetRandomFloat())*0.5F)+0.5F);
			fRad2=(lpMFI->fMaxRayWidth*0.5F)*((fabsf(GetRandomFloat())*0.5F)+0.5F);

			// set angle values
			fAngle=fabsf(GetRandomFloat())*2.0F*PI;
			fDX=cosf(fAngle);
			fDY=sinf(fAngle);

			// render ray
			for(dwX=0;dwX<dwTexSize;dwX++)
				for(dwY=0;dwY<dwTexSize;dwY++)
				{
					// extract color value
					WORD wC=lpData[dwX+(dwY*dwPitch)];
					
					// extract color components
					fR=GetR444(wC);
					fG=GetG444(wC);
					fB=GetB444(wC);
					fA=GetA444(wC);
					
					// save coordinates
					float fX=(float)dwX;
					float fY=(float)dwY;

					// adjust coordinates
					fX=(fX-(fTexSize*0.5F))/fTexSize;
					fY=(fY-(fTexSize*0.5F))/fTexSize;

					// compute distance to ray
					float fS1=(fX*fDX)+(fY*fDY);
					float fX1=fS1*fDX;
					float fY1=fS1*fDY;
					float fDist1=Magnitude(fX1-fX,fY1-fY,0);

					// compute distance to ray's normal
					float fS2=(fX*fDY)-(fY*fDX);
					float fX2=fS2*fDY;
					float fY2=-(fS2*fDX);
					float fDist2=Magnitude(fX2-fX,fY2-fY,0);

					// set new coordiates
					fX=fDist1*(fRad1/fRad2);
					fY=fDist2;

					// compute distance to center
					float fDist=Magnitude(fX,fY,0);

					// compute intensity
					if(fDist<fRad1)
						fInt=cosf((fRad1-fDist)*lpMFI->fRayFalloff*PI*2.0F);
					else
						fInt=1;

					// clip intensity
					if(fInt<0.0F) fInt=0.0F;
					if(fInt>1.0F) fInt=1.0F;

					// combine with current color
					fR+=GetInterpolatedValue(fR1,fR2,fInt);
					fG+=GetInterpolatedValue(fG1,fG2,fInt);
					fB+=GetInterpolatedValue(fB1,fB2,fInt);
					fA+=GetInterpolatedValue(fA1,fA2,fInt);

					// clip components
					if(fR>1.0F) fR=1.0F;
					if(fG>1.0F) fG=1.0F;
					if(fB>1.0F) fB=1.0F;
					if(fA>1.0F) fA=1.0F;

					// set new color
					SetRGBA444(lpData[dwX+(dwY*dwPitch)],
							   fR,fG,fB,fA);
				}
		}

		// unlock texture
		UnlockTexture(dwTex);

		// output texture to file
		CreateDDSFromTexture(lpFilename,
							 dwTex);

		// ok
		break;

	// main flare
	case(LF_FLARERING):

		// cast pointer to flare info
		lpFRI=(LPLF_FLARERINGINFO)lpFlareData;

		// save texture size
		dwTexSize=lpFRI->dwTexSize;
		fTexSize=(float)dwTexSize;

		// add texture
		dwTex=AddTexture(dwTexSize,
						 dwTexSize,
						 TRUE,
						 TRUE);

		// lock texture
		lr=LockTexture(dwTex);

		// extract data pointer & pitch
		lpData=(LPWORD)lr.pBits;
		dwPitch=lr.Pitch>>1;

		// extract ring data
		fRad1=lpFRI->fRingDia1*0.5F;
		fRad2=lpFRI->fRingDia2*0.5F;
		fR1=GetR888(lpFRI->cRingClr1);
		fG1=GetG888(lpFRI->cRingClr1);
		fB1=GetB888(lpFRI->cRingClr1);
		fA1=GetA888(lpFRI->cRingClr1);
		fR2=GetR888(lpFRI->cRingClr2);
		fG2=GetG888(lpFRI->cRingClr2);
		fB2=GetB888(lpFRI->cRingClr2);
		fA2=GetA888(lpFRI->cRingClr2);

		// render ring
		for(dwX=0;dwX<dwTexSize;dwX++)
			for(dwY=0;dwY<dwTexSize;dwY++)
			{
				// save coordinates
				float fX=(float)dwX;
				float fY=(float)dwY;

				// adjust coordinates
				fX=(fX-(fTexSize*0.5F))/fTexSize;
				fY=(fY-(fTexSize*0.5F))/fTexSize;
				
				// compute distance to center
				float fDist=Magnitude(fX,fY,0);

				// compute intensity
				if(fDist>fRad1&&fDist<fRad2)
					fInt=cosf(GetInterpolationValue(fRad1,fRad2,fDist)*lpFRI->fRingFalloff*PI);
				else
					fInt=1;

				// clip intensity
				if(fInt<0.0F) fInt=0.0F;
				if(fInt>1.0F) fInt=1.0F;

				// set color
				SetRGBA444(lpData[dwX+(dwY*dwPitch)],
						   GetInterpolatedValue(fR1,fR2,fInt),
						   GetInterpolatedValue(fG1,fG2,fInt),
						   GetInterpolatedValue(fB1,fB2,fInt),
						   GetInterpolatedValue(fA1,fA2,fInt));
			}

		// unlock texture
		UnlockTexture(dwTex);

		// output texture to file
		CreateDDSFromTexture(lpFilename,
							 dwTex);

		// ok
		break;

	// undefined
	default:
		DisplayErrorMessage("Undefined flare type.",
							"CreateLensFlareDDS()",
							FALSE,
							FALSE);

		// ok
		break;
	}
}
#endif

// InitLensFlareData()

void InitLensFlareData(LPLENSFLAREDATA lpLFD,
					   LPCSTR lpFilename,
					   DWORD dwNumRings,
					   float fMaxRingPos,
					   float fFlareDia,
					   float fMinRingDia,
					   float fMaxRingDia,
					   float fWashoutRed,
					   float fWashoutGreen,
					   float fWashoutBlue,
					   float fWashoutFactor,
					   float fMaxWashout,
					   float fFlareRotFactor)
{
	LoadingOutput("InitLensFlareData");

#ifdef DEBUG_MODE
	// verify number of rings
	if(dwNumRings>=MAX_LENSFLARERINGS)
	{
		DisplayFatalErrorMessage("Invalid lens flare ring quantity.",
								 "InitLensFlareData()",
								 FALSE);
		return;
	}
#endif

	// filename
	char lpFile[16];

	// counter
	DWORD dwCount;

	// prepare main flare filename
	sprintf(lpFile,
			lpFilename,
			0);
	
	// add main flare texture
	lpLFD->dwBaseTex=AddTexture(lpFile);

	// set main flare data
	lpLFD->fFlareXSize=ScrSizeX(fFlareDia);
	lpLFD->fFlareYSize=ScrSizeY(fFlareDia);

	// set number of rings
	lpLFD->dwNumRings=dwNumRings;

	// add rings
	for(dwCount=0;dwCount<dwNumRings;dwCount++)
	{
		// prepare filename
		sprintf(lpFile,
				lpFilename,
				dwCount+1);

		// add ring texture
		AddTexture(lpFile);

		// set ring diameter
		float fRingDia=fMinRingDia+(fabsf(GetRandomFloat())*(fMaxRingDia-fMinRingDia));

		// set ring data
		lpLFD->fRingXSize[dwCount]=ScrSizeX(fRingDia);
		lpLFD->fRingYSize[dwCount]=ScrSizeY(fRingDia);
		lpLFD->fRingPos[dwCount]=fabsf(GetRandomFloat()*fMaxRingPos);
	}

	// set washout data
	lpLFD->fWashoutRed=fWashoutRed;
	lpLFD->fWashoutGreen=fWashoutGreen;
	lpLFD->fWashoutBlue=fWashoutBlue;
	lpLFD->fWashoutFactor=fWashoutFactor;
	lpLFD->fMaxWashout=fMaxWashout;

	// set flare rotation factor
	lpLFD->fFlareRotFactor=fFlareRotFactor;
}

// RenderLensFlare()

void RenderLensFlare(LPLENSFLAREDATA lpLFD,
					 float fScreenX,
					 float fScreenY)
{
	// counter
	DWORD dwCount;

	// compute distance to center
	float fDist=sqrtf(Squared(fScreenX)+Squared(fScreenY))*lpLFD->fWashoutFactor;

	// clip distance
	if(fDist>1.0F)
		return;

	// render washout effect
	FadeScreen(D3DCOLOR_COLORVALUE(lpLFD->fWashoutRed,
								   lpLFD->fWashoutGreen,
								   lpLFD->fWashoutBlue,
								   (1.0F-fDist)*lpLFD->fMaxWashout));

	// begin sprites
	if(BeginSprites())
	{
		// render main flare
		OutputSprite(lpLFD->dwBaseTex,
					 fScreenX,fScreenY,
					 lpLFD->fFlareXSize,
					 lpLFD->fFlareYSize,
					 fDist*lpLFD->fFlareRotFactor);

		// render rings
		for(dwCount=0;dwCount<lpLFD->dwNumRings;dwCount++)
			OutputSprite(lpLFD->dwBaseTex+(dwCount+1),
						 fScreenX-(fScreenX*lpLFD->fRingPos[dwCount]*fDist),
						 fScreenY-(fScreenY*lpLFD->fRingPos[dwCount]*fDist),
						 lpLFD->fRingXSize[dwCount],
						 lpLFD->fRingYSize[dwCount]);
		
		// end sprites
		EndSprites();
	}
}

// RenderLensFlare()

void RenderLensFlare(LPLENSFLAREDATA lpLFD,
					 float fWorldX,
					 float fWorldY,
					 float fWorldZ)
{
	// screen coordinates
	float fScreenX;
	float fScreenY;

	// transformation matrices
	D3DXMATRIX mWorld;
	D3DXMATRIX mWView;
	D3DXMATRIX mFinal;
	
	// prepare world matrix
	mWorld=D3DXMATRIX(1,0,0,0,
					  0,1,0,0,
					  0,0,1,0,
					  fWorldX,
					  fWorldY,
					  fWorldZ,
					  1);

	// transform by view matrix
	D3DXMatrixMultiply(&mWView,
					   &mWorld,
					   &g_mtxView);

	// transform by projection matrix
	D3DXMatrixMultiply(&mFinal,
					   &mWView,
					   &g_mtxProj);
	
	// clip if necessary
	if(mFinal._43<0.0F)
		return;
	
	// set screen coorinates
	fScreenX=(mFinal._41*0.5F)/mFinal._44;
	fScreenY=(mFinal._42*0.5F)/mFinal._44;

	// render lens flare
	RenderLensFlare(lpLFD,
					fScreenX,
					fScreenY);
}

// ClearDisplayBuffers()

void ClearDisplayBuffers(void)
{
	// counter
	DWORD dwCount;

	// check for device
	if(g_lpD3DDevice)
	{
		// loop through each buffer
		for(dwCount=0;dwCount<g_dmApp.dwBackBufferCount+1;dwCount++)
		{
			// clear screen
			ClearScreen();

			// present scene
			PresentScene();
		}
	}
}

// AddTextureFromResource()

DWORD AddTextureFromResource(LPCTSTR lpResName,
							 UINT nWidth,
							 UINT nHeight,
							 LPSTR lpName,
							 DWORD dwMipMapLevels)
{
	LoadingOutput("AddTextureFromResource");

	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~TX%d",g_dwNumTex);
		return(AddTextureFromResource(lpResName,nWidth,nHeight,g_lpMsg,dwMipMapLevels));
	}

	// get next texture
	DWORD dwIndex=GetNextTexture();

	// check texture
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpTexNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpTexNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);
	
	// used for error checking
	HRESULT hr;

	// set maximum texture size
	UINT nMaxSize=(UINT)((g_lpDevCaps.MaxTextureWidth<g_lpDevCaps.MaxTextureHeight)?g_lpDevCaps.MaxTextureWidth:g_lpDevCaps.MaxTextureHeight);

	// verify size
	if(nWidth>nMaxSize||
	   nHeight>nMaxSize)
	{
		// set proper size
		nWidth=nMaxSize;
		nHeight=nMaxSize;
	}

	// load texture
	hr=D3DXCreateTextureFromResourceEx(g_lpD3DDevice,
									   GetModuleHandle(NULL),
									   lpResName,
									   nWidth,
									   nHeight,
									   (UINT)dwMipMapLevels,
									   NULL,
									   D3DFMT_UNKNOWN,
									   D3DPOOL_MANAGED,
									   D3DX_DEFAULT,
									   D3DX_DEFAULT,
									   NULL,
									   NULL,
									   NULL,
									   &g_lplpTex[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to load texture resource.",
							  NULL,
							  hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;
	
	// return index
	return(dwIndex);
}

// InitLoadingScreen()

BOOL InitLoadingScreen(void)
{
#ifdef USE_LOADINGSCREEN
	// counter
	DWORD dwCount;

	// check for existing texture
	if(g_dwLSLogo!=NO_OBJECT)
		return(TRUE);

	// load logo texture
	g_dwLSLogo=AddTextureFromResource(MAKEINTRESOURCE(IDB_LOADINGSCREEN),
									  128,128);

	// check for error
	if(g_dwLSLogo==NO_OBJECT)
		return(FALSE);

	// load bar texture #1
	g_lpdwLSProgress[0]=AddTextureFromResource(MAKEINTRESOURCE(IDB_LOADINGBAR1),
											   16,16);

	// check for error
	if(g_lpdwLSProgress[0]==NO_OBJECT)
		return(FALSE);

	// load bar texture #2
	g_lpdwLSProgress[1]=AddTextureFromResource(MAKEINTRESOURCE(IDB_LOADINGBAR2),
											   16,16);

	// check for error
	if(g_lpdwLSProgress[1]==NO_OBJECT)
		return(FALSE);

	// load bar texture #3
	g_lpdwLSProgress[2]=AddTextureFromResource(MAKEINTRESOURCE(IDB_LOADINGBAR3),
											   16,16);

	// check for error
	if(g_lpdwLSProgress[2]==NO_OBJECT)
		return(FALSE);

	// load bar texture #4
	g_lpdwLSProgress[3]=AddTextureFromResource(MAKEINTRESOURCE(IDB_LOADINGBAR4),
											   16,16);

	// check for error
	if(g_lpdwLSProgress[3]==NO_OBJECT)
		return(FALSE);

	// set progress bar states
	for(dwCount=0;dwCount<10;dwCount++)
		g_lpdwLSPState[dwCount]=dwCount;
#endif

	// ok
	return(TRUE);
}

// EnableLoadingScreen()

void EnableLoadingScreen(void)
{
#ifdef USE_LOADINGSCREEN
	// set flag
	g_bShowLoadingScreen=TRUE;
#endif
}

// DisableLoadingScreen()

void DisableLoadingScreen(void)
{
#ifdef USE_LOADINGSCREEN
	// reset flag
	g_bShowLoadingScreen=FALSE;
#endif
}

// UpdateLoadingScreen()

void UpdateLoadingScreen(void)
{
#ifdef USE_LOADINGSCREEN
	// counter
	DWORD dwCount;

	// check closing flag
	if(g_bClosing)
		return;

	// check show flag
	if(!g_bShowLoadingScreen)
	{
		// do not show if initialized
		if(g_bAppInit)
			return;
	}

	// check D3D device
	if(!g_lpD3DDevice)
		return;

	// check logo texture
	if(g_dwLSLogo==NO_OBJECT)
		return;

	// timer
	static DWORD dwTimer=0;

	// check timer
	if(dwTimer==0)
		dwTimer=GetTickCount();

	// check elapsed time
	if(GetTickCount()<(dwTimer+LS_UPDATEINC))
		return;

	// reset timer
	dwTimer=GetTickCount();

	// clear screen
	ClearScreen();

	// begin sprites
	if(BeginSprites())
	{
		// output logo
		OutputSprite(g_dwLSLogo,
					 ScrCoords(320,208),
					 ScrSize(128,64));

		// output progress bars
		for(dwCount=0;dwCount<10;dwCount++)
		{
			// bar index
			DWORD dwIndex;
			
			// set bar index
			switch(g_lpdwLSPState[dwCount])
			{
			case(9):  dwIndex=0; break;
			case(10): dwIndex=1; break;
			case(11): dwIndex=2; break;
			default:  dwIndex=3; break;
			}

			// update bar state
			g_lpdwLSPState[dwCount]++;

			// check for overflow
			if(g_lpdwLSPState[dwCount]>=12)
				g_lpdwLSPState[dwCount]=0;

			// output bar
			OutputSprite(g_lpdwLSProgress[dwIndex],
						 ScrCoords(366-(dwCount*10),248),
						 ScrSize(8,16));
		}
		
		// end sprites
		EndSprites();
	}

	// present scene
	PresentScene();
#endif
}

// GetAvailableResolutions()

BOOL GetAvailableResolutions(LPRESOLUTIONS lpRes)
{
	LoadingOutput("GetAvailableResolutions");

#ifdef DEBUG_MODE
	// check D3D pointer
	if(g_lpD3D)
	{
		DisplayFatalErrorMessage("Do not call GetAvailableResolutions() when Direct3D is active!",
								 NULL,
								 FALSE);
		return(FALSE);
	}
#endif

	// counter
	DWORD dwCount;
	
	// init direct3d
	if(!InitDirect3D())
		return(FALSE);

	// allocate resolutions list
	if(!AllocMem((LPVOID*)&lpRes->lpRes,
				 sizeof(RESOLUTION)*g_res.dwNumRes))
	{
		ReleaseDirect3D();
		return(FALSE);
	}

	// copy resolutions
	for(dwCount=0;dwCount<g_res.dwNumRes;dwCount++)
	{
		// save width & height
		lpRes->lpRes[dwCount].dwX=g_res.lpRes[dwCount].dwX;
		lpRes->lpRes[dwCount].dwY=g_res.lpRes[dwCount].dwY;
	}

	// copy number of resolutions
	lpRes->dwNumRes=g_res.dwNumRes;

	// release direct3d
	ReleaseDirect3D();

	// ok
	return(TRUE);
}

// ReleaseAvailableResolutions()

void ReleaseAvailableResolutions(LPRESOLUTIONS lpRes)
{
#ifdef DEBUG_MODE
	// check pointer
	if(lpRes==NULL)
	{
		DisplayFatalErrorMessage("Invalid pointer.",
								 "ReleaseAvailableResolutions()",
								 FALSE);
		return;
	}
#endif

	// free resolutions memory
	FreeMem((LPVOID*)&lpRes->lpRes);

	// reset counter
	lpRes->dwNumRes=0;
}

// eof

// Copyright 2005-2007 WaveBreak Software
