// Copyright 2005-2007 WaveBreak Software

///////////////////////////////////////////////////////////////////
// perform step-by-step walkthrough with COMPLETE error checking //
///////////////////////////////////////////////////////////////////

// finish hit detection
// - detect collisions with dynamic meshes
// - detect proximity with static objects
// - detect proximity with dynamic objects
// fix camera following algorithm

// r3dcol.cpp -- collision detection

// includes //

#include"..\\engine\\r3dwin.h"

// globals //

// static world mesh counters
DWORD g_dwNumSWMesh=0;
DWORD g_dwMaxSWMesh=0;
// static world mesh data
LPDWORD g_lpdwSWMMeshIndex=NULL;
LPDWORD g_lpdwSWMMtxIndex=NULL;
LPSTR* g_lplpSWMeshNames=NULL;
// dynamic world mesh counters
DWORD g_dwNumDWMesh=0;
DWORD g_dwMaxDWMesh=0;
// dynamic world mesh data
LPDWORD g_lpdwDWMMeshIndex=NULL;
LPDWORD g_lpdwDWMMtxIndex=NULL;
LPSTR* g_lplpDWMeshNames=NULL;
#ifdef DEBUG_MODE
// mini font texture
DWORD g_dwMFTexIdx=NO_OBJECT;
// clip planes
float g_fZClipNear=1;
float g_fZClipFar=100;
// fog color
float g_fFogR=0;
float g_fFogG=0;
float g_fFogB=0;
// ambient color
float g_fAmbientR=0;
float g_fAmbientG=0;
float g_fAmbientB=0;
// current editing indices
DWORD g_dwCurLight=NO_OBJECT;
DWORD g_dwCurSWMesh=NO_OBJECT;
DWORD g_dwCurDWMesh=NO_OBJECT;
DWORD g_dwCurSWObj=NO_OBJECT;
DWORD g_dwCurDWObj=NO_OBJECT;
// previous editing indices
DWORD g_dwLastMat=NO_OBJECT;
DWORD g_dwLastTex=NO_OBJECT;
DWORD g_dwLastLight=NO_OBJECT;
DWORD g_dwLastMesh=NO_OBJECT;
DWORD g_dwLastSWMesh=NO_OBJECT;
DWORD g_dwLastDWMesh=NO_OBJECT;
DWORD g_dwLastSWObj=NO_OBJECT;
DWORD g_dwLastDWObj=NO_OBJECT;
#endif
// gravity vector
D3DXVECTOR3 g_vGravity;
// static triangle counters
DWORD g_dwNumSTri=0;
DWORD g_dwMaxSTri=0;
// static triangle info
LPD3DXVECTOR3 g_lpvSTriVtx1=NULL;
LPD3DXVECTOR3 g_lpvSTriVtx2=NULL;
LPD3DXVECTOR3 g_lpvSTriVtx3=NULL;
LPD3DXVECTOR3 g_lpvSTriNormal=NULL;
LPDWORD g_lpdwSTriIndexed=NULL;
LPSEGMENTBLOCK g_lpSTriSegBlocks=NULL;
DWORD g_dwNumSTriSegBlocks=0;
// static triangle index list
LPDWORD g_lpdwSTriIndexList=NULL;
// segment values
float g_fMinSTriSegSize=1;
DWORD g_dwMaxSTriSegX=8;
DWORD g_dwMaxSTriSegY=8;
DWORD g_dwMaxSTriSegZ=8;
DWORD g_dwNumSTriSegX=8;
DWORD g_dwNumSTriSegY=8;
DWORD g_dwNumSTriSegZ=8;
DWORD g_dwNumSTriSegXY=64;
int g_nNumSTriSegX1=7;
int g_nNumSTriSegY1=7;
int g_nNumSTriSegZ1=7;
D3DXVECTOR3 g_vSTriSegSize;
D3DXVECTOR3 g_vSTriSegASize;
// segment division values
D3DXVECTOR3 g_vMinSTriExtents;
D3DXVECTOR3 g_vMaxSTriExtents;
// static world object counters
DWORD g_dwNumSWObj=0;
DWORD g_dwMaxSWObj=0;
// static world object data
LPDWORD g_lpdwSWObjMtxIdx=NULL;
LPD3DXVECTOR3 g_lpvSWObjRad=NULL;
LPSTR* g_lplpSWObjNames=NULL;
// dynamic world object counters
DWORD g_dwNumDWObj=0;
DWORD g_dwMaxDWObj=0;
// dynamic world object data
LPDWORD g_lpdwDWObjMtxIdx=NULL;
LPD3DXVECTOR3 g_lpvDWObjSafePos=NULL;
LPD3DXVECTOR3 g_lpvDWObjRad=NULL;
LPD3DXVECTOR3 g_lpvDWObjARad=NULL;
LPD3DXVECTOR3 g_lpvDWObjDisp=NULL;
LPBOOL g_lpbDWObjSTColl=NULL;
LPD3DXVECTOR3 g_lpvDWObjSTCollPt=NULL;
LPDWORD g_lpdwDWObjSTCollIdx=NULL;
LPSTR* g_lplpDWObjNames=NULL;
#ifdef DEBUG_MODE
// world object mesh data
DWORD g_dwObjectMeshIdx=NO_OBJECT;
DWORD g_dwObjectMtxIdx=NO_OBJECT;
#endif
// collision detection values
DWORD g_dwColRetry=0;
DWORD g_dwColNumSTri=0;
DWORD g_dwColIndexNum=NO_OBJECT;
// world camera data
BOOL g_bWCEnabled=FALSE;
DWORD g_dwWCPositionObjIdx=NO_OBJECT;
DWORD g_dwWCPositionMtxIdx=NO_OBJECT;
DWORD g_dwWCTargetMtxIdx=NO_OBJECT;
D3DXVECTOR3 g_vWCTargetOffset;
D3DXVECTOR3 g_vWCCameraOffset;
float g_fWCMaxSpeed=0;
D3DXVECTOR3 g_vWCAhead;

// functions //

// GetNextStaticWorldMesh()

DWORD GetNextStaticWorldMesh(void)
{
	LoadingOutput("GetNextStaticWorldMesh");

	// check counter
	if(g_dwNumSWMesh==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwSWMMeshIndex,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSWMMtxIndex,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpSWMeshNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxSWMesh=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumSWMesh>=g_dwMaxSWMesh)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwSWMMeshIndex,
					  sizeof(DWORD)*g_dwMaxSWMesh,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSWMMtxIndex,
					  sizeof(DWORD)*g_dwMaxSWMesh,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpSWMeshNames,
					  sizeof(LPSTR)*g_dwMaxSWMesh,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxSWMesh+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumSWMesh;

	// increment couter
	g_dwNumSWMesh++;

	// return index
	return(dwIndex);
}

// AddStaticWorldMesh()

DWORD AddStaticWorldMesh(DWORD dwMeshIndex,
						 DWORD dwWMtxIndex,
						 LPSTR lpName)
{
	LoadingOutput("AddStaticWorldMesh");

#ifdef DEBUG_MODE
	// verify index
	if(dwMeshIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "AddStaticWorldMesh()",
								 FALSE);
		return(NO_OBJECT);
	}

	// verify index
	if(dwWMtxIndex>=g_dwNumMtx&&
	   dwWMtxIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "AddStaticWorldMesh()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// get matrix
	D3DXMATRIX mMtx;
	GetMatrix(dwWMtxIndex,
			  &mMtx);

	// add mesh
	return(AddStaticWorldMesh(dwMeshIndex,
							  &mMtx,
							  lpName));
}

// AddStaticWorldMesh()

DWORD AddStaticWorldMesh(DWORD dwMeshIndex,
					     float fX,
					     float fY,
					     float fZ,
						 LPSTR lpName)
{
	LoadingOutput("AddStaticWorldMesh");

#ifdef DEBUG_MODE
	// verify index
	if(dwMeshIndex>=g_dwNumMesh)

	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "AddStaticWorldMesh()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// prepare matrix
	D3DXMATRIX mMtx=D3DXMATRIX(1, 0, 0, 0,
							   0, 1, 0, 0,
							   0, 0, 1, 0,
							   fX,fY,fZ,1);

	// add mesh
	return(AddStaticWorldMesh(dwMeshIndex,
	 						  &mMtx,
							  lpName));
}

// AddStaticWorldMesh()

DWORD AddStaticWorldMesh(DWORD dwMeshIndex,
						 LPD3DXMATRIX lpMtx,
						 LPSTR lpName)
{
	LoadingOutput("AddStaticWorldMesh");

#ifdef DEBUG_MODE
	// verify index
	if(dwMeshIndex>=g_dwNumMesh)

	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "AddStaticWorldMesh()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// counter
	DWORD dwCount;
	
	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,"~SM%d",g_dwNumSWMesh);

		// add mesh
		int nRet=AddStaticWorldMesh(dwMeshIndex,
									lpMtx,
									g_lpMsg);

		// ok
		return(nRet);
	}
	
	// get return index
	DWORD dwIndex=GetNextStaticWorldMesh();

	// copy mesh index
	g_lpdwSWMMeshIndex[dwIndex]=dwMeshIndex;

	// get matrix index
	g_lpdwSWMMtxIndex[dwIndex]=AddMatrix(lpMtx);
	
	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpSWMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpSWMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// lock mesh
	LPLOCKEDMESH lm=LockMesh(dwMeshIndex);

// check LockMesh() function for every possible error!!

	// add mesh faces
	for(dwCount=0;dwCount<lm->dwNumFaces;dwCount++)
	{
		// get face
		MESHFACE mf=lm->lpFaces[dwCount];
		
		// transform face vertices
		D3DXVECTOR3 vVtx1; TransformVector(&vVtx1,&lm->lpVtx[mf.dwV1],lpMtx);
		D3DXVECTOR3 vVtx2; TransformVector(&vVtx2,&lm->lpVtx[mf.dwV2],lpMtx);
		D3DXVECTOR3 vVtx3; TransformVector(&vVtx3,&lm->lpVtx[mf.dwV3],lpMtx);
		
		// add triangle
		AddStaticTriangle(&vVtx1,
						  &vVtx2,
						  &vVtx3);
	}
	
	// unlock mesh
	UnlockMesh(dwMeshIndex,
			   lm);

	// return index
	return(dwIndex);
}

// RenderStaticWorldMeshes()

void RenderStaticWorldMeshes(void)
{
	// counter
	DWORD dwCount;

	// render each mesh
	if(g_dwNumSWMesh)
		for(dwCount=0;dwCount<g_dwNumSWMesh;dwCount++)
			RenderMesh(g_lpdwSWMMeshIndex[dwCount],
					   g_lpdwSWMMtxIndex[dwCount]);
}

// PurgeStaticWorldMeshes()

void PurgeStaticWorldMeshes(void)
{
	// check counter
	if(g_dwNumSWMesh)
	{
		// counter
		DWORD dwCount;
		
		// release name buffers
		for(dwCount=0;dwCount<g_dwNumSWMesh;dwCount++)
			FreeMem((LPVOID*)&g_lplpSWMeshNames[dwCount]);
	}

	// free memory
	FreeMem((LPVOID*)&g_lpdwSWMMeshIndex);
	FreeMem((LPVOID*)&g_lpdwSWMMtxIndex);
	FreeMem((LPVOID*)&g_lplpSWMeshNames);

	// reset counters
	g_dwNumSWMesh=0;
	g_dwMaxSWMesh=0;

	// purge static triangles
	PurgeStaticTriangles();
}

// PurgeWorldObjects()

void PurgeWorldObjects(void)
{
	// purge all objects
	PurgeStaticWorldMeshes();
	PurgeDynamicWorldMeshes();
	PurgeStaticWorldObjects();
	PurgeDynamicWorldObjects();
}

#ifdef DEBUG_MODE
// CompileWorldScript()

void CompileWorldScript(LPCSTR lpDestFile,
						LPCSTR lpSrcFile,
						BOOL bEditMode)
{
	LoadingOutput("CompileWorldScript");

	// open compiler
	if(!OpenScriptCompiler(lpDestFile,
						   lpSrcFile))
		return;

	// read script
	while(TRUE)
	{
		// read string
		char lpCmd[32];
		ReadScriptString(lpCmd);

		// process command
		if(stricmp(lpCmd,"alphablend")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_ABLEND);
			
			// read flag
			ReadScriptString(lpCmd);

			// output flag
			if(stricmp(lpCmd,"on")==0)
				SaveScriptLong(1);
			else if(stricmp(lpCmd,"off")==0)
				SaveScriptLong(0);
			else
			{
				DisplayFatalErrorMessage("Invalid script operation.",
										 lpCmd,
										 FALSE);
				return;
			}
		}
		else if(stricmp(lpCmd,"alphatest")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_ATEST);
			
			// read flag
			ReadScriptString(lpCmd);

			// output flag
			if(stricmp(lpCmd,"on")==0)
				SaveScriptLong(1);
			else if(stricmp(lpCmd,"off")==0)
				SaveScriptLong(0);
			else
			{
				DisplayFatalErrorMessage("Invalid script operation.",
										 lpCmd,
										 FALSE);
				return;
			}
		}
		else if(stricmp(lpCmd,"specular")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_SPECULAR);
			
			// read flag
			ReadScriptString(lpCmd);

			// output flag
			if(stricmp(lpCmd,"on")==0)
				SaveScriptLong(1);
			else if(stricmp(lpCmd,"off")==0)
				SaveScriptLong(0);
			else
			{
				DisplayFatalErrorMessage("Invalid script operation.",
										 lpCmd,
										 FALSE);
				return;
			}
		}
		else if(stricmp(lpCmd,"fog")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_FOG);
			
			// read flag
			ReadScriptString(lpCmd);

			// output flag
			if(stricmp(lpCmd,"on")==0)
				SaveScriptLong(1);
			else if(stricmp(lpCmd,"off")==0)
				SaveScriptLong(0);
			else
			{
				DisplayFatalErrorMessage("Invalid script operation.",
										 lpCmd,
										 FALSE);
				return;
			}
		}
		else if(stricmp(lpCmd,"fogparams")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_FOGPARAMS);
			
			// output data
			CopyScriptFloat(); // start
			CopyScriptFloat(); // end
			CopyScriptFloat(); // r
			CopyScriptFloat(); // g
			CopyScriptFloat(); // b
		}
		else if(stricmp(lpCmd,"ambient")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_AMBIENT);
			// output data
			CopyScriptFloat(); // r
			CopyScriptFloat(); // g
			CopyScriptFloat(); // b
		}
		else if(stricmp(lpCmd,"zclip")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_ZCLIP);
			
			// output data
			CopyScriptFloat(); // near
			CopyScriptFloat(); // far
		}
		else if(stricmp(lpCmd,"gravity")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_GRAVITY);
			
			// output data
			CopyScriptFloat(); // x
			CopyScriptFloat(); // y
			CopyScriptFloat(); // z
		}
		else if(stricmp(lpCmd,"segment")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_SEGMENT);
			
			// output data
			CopyScriptFloat(); // min size
			CopyScriptLong(); // max x
			CopyScriptLong(); // max y
			CopyScriptLong(); // max z
		}
		else if(stricmp(lpCmd,"pointlight")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_PLIGHT);
			
			// output data
			CopyScriptString(); // name
			CopyScriptFloat(); // diffuse r
			CopyScriptFloat(); // diffuse g
			CopyScriptFloat(); // diffuse b
			CopyScriptFloat(); // diffuse a
			CopyScriptFloat(); // specular r
			CopyScriptFloat(); // specular g
			CopyScriptFloat(); // specular b
			CopyScriptFloat(); // specular a
			CopyScriptFloat(); // position x
			CopyScriptFloat(); // position y
			CopyScriptFloat(); // position z
			CopyScriptFloat(); // range
		}
		else if(stricmp(lpCmd,"spotlight")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_SLIGHT);
			
			// output data
			CopyScriptString(); // name
			CopyScriptFloat(); // diffuse r
			CopyScriptFloat(); // diffuse g
			CopyScriptFloat(); // diffuse b
			CopyScriptFloat(); // diffuse a
			CopyScriptFloat(); // specular r
			CopyScriptFloat(); // specular g
			CopyScriptFloat(); // specular b
			CopyScriptFloat(); // specular a
			CopyScriptFloat(); // position x
			CopyScriptFloat(); // position y
			CopyScriptFloat(); // position z
			CopyScriptFloat(); // direction x
			CopyScriptFloat(); // direction y
			CopyScriptFloat(); // direction z
			CopyScriptFloat(); // range
			CopyScriptFloat(); // theta
			CopyScriptFloat(); // phi
		}
		else if(stricmp(lpCmd,"dirlight")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_DLIGHT);
			
			// output data
			CopyScriptString(); // name
			CopyScriptFloat(); // diffuse r
			CopyScriptFloat(); // diffuse g
			CopyScriptFloat(); // diffuse b
			CopyScriptFloat(); // diffuse a
			CopyScriptFloat(); // direction x
			CopyScriptFloat(); // direction y
			CopyScriptFloat(); // direction z
		}
		else if(stricmp(lpCmd,"material")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_MATERIAL);
			
			// output data
			CopyScriptString(); // name
			CopyScriptFloat(); // diffuse r
			CopyScriptFloat(); // diffuse g
			CopyScriptFloat(); // diffuse b
			CopyScriptFloat(); // diffuse a
			CopyScriptFloat(); // ambient r
			CopyScriptFloat(); // ambient g
			CopyScriptFloat(); // ambient b
			CopyScriptFloat(); // ambient a
			CopyScriptFloat(); // specular r
			CopyScriptFloat(); // specular g
			CopyScriptFloat(); // specular b
			CopyScriptFloat(); // specular a
			CopyScriptFloat(); // emissive r
			CopyScriptFloat(); // emissive g
			CopyScriptFloat(); // emissive b
			CopyScriptFloat(); // emissive a
			CopyScriptFloat(); // power
		}
		else if(stricmp(lpCmd,"texture")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_TEXTURE);
			
			// output data
			CopyScriptString(); // name
			CopyScriptString(); // filename
		}
		else if(stricmp(lpCmd,"mesh")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_MFILE);
			
			// output data
			CopyScriptString(); // name
			CopyScriptString(); // filename
		}
		else if(stricmp(lpCmd,"meshbox")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_MBOX);
			
			// output data
			CopyScriptString(); // name
			CopyScriptFloat(); // width
			CopyScriptFloat(); // height
			CopyScriptFloat(); // depth
			CopyScriptString(); // material
		}
		else if(stricmp(lpCmd,"meshcylinder")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_MCYLINDER);
			
			// output data
			CopyScriptString(); // name
			CopyScriptFloat(); // lower radius
			CopyScriptFloat(); // upper radius
			CopyScriptFloat(); // height
			CopyScriptLong(); // slices
			CopyScriptLong(); // stacks
			CopyScriptString(); // material
		}
		else if(stricmp(lpCmd,"meshpolygon")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_MPOLYGON);
			
			// output data
			CopyScriptString(); // name
			CopyScriptFloat(); // length of side
			CopyScriptLong(); // sides
			CopyScriptString(); // material
		}
		else if(stricmp(lpCmd,"meshsphere")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_MSPHERE);
			
			// output data
			CopyScriptString(); // name
			CopyScriptFloat(); // radius
			CopyScriptLong(); // slices
			CopyScriptLong(); // stacks
			CopyScriptString(); // material
		}
		else if(stricmp(lpCmd,"meshteapot")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_MTEAPOT);
			
			// output data
			CopyScriptString(); // name
			CopyScriptString(); // material
		}
		else if(stricmp(lpCmd,"meshtorus")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_MTORUS);
			
			// output data
			CopyScriptString(); // name
			CopyScriptFloat(); // inner radius
			CopyScriptFloat(); // outer radius
			CopyScriptLong(); // sides
			CopyScriptLong(); // rings
			CopyScriptString(); // material
		}
		else if(stricmp(lpCmd,"skybox")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_SKYBOX);
			
			// output data
			CopyScriptFloat(); // radius
			CopyScriptFloat(); // offset x
			CopyScriptFloat(); // offset y
			CopyScriptFloat(); // offset z
			CopyScriptString(); // left texture
			CopyScriptString(); // right texture
			CopyScriptString(); // front texture
			CopyScriptString(); // back texture
			CopyScriptString(); // top texture
			CopyScriptString(); // bottom texture
		}
		else if(stricmp(lpCmd,"swmesh")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_SWMESH);
			
			// output data
			CopyScriptString(); // name
			CopyScriptString(); // mesh
			for(int n=0;n<16;n++)
				CopyScriptFloat(); // matrix
		}
		else if(stricmp(lpCmd,"dwmesh")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_DWMESH);
			
			// output data
			CopyScriptString(); // name
			CopyScriptString(); // mesh
			for(int n=0;n<16;n++)
				CopyScriptFloat(); // matrix
		}
		else if(stricmp(lpCmd,"swobject")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_SWOBJ);
			
			// output data
			CopyScriptString(); // name
			CopyScriptFloat(); // position x
			CopyScriptFloat(); // position y
			CopyScriptFloat(); // position z
			CopyScriptFloat(); // radius x
			CopyScriptFloat(); // radius y
			CopyScriptFloat(); // radius z
		}
		else if(stricmp(lpCmd,"dwobject")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_DWOBJ);
			
			// output data
			CopyScriptString(); // name
			CopyScriptFloat(); // position x
			CopyScriptFloat(); // position y
			CopyScriptFloat(); // position z
			CopyScriptFloat(); // radius x
			CopyScriptFloat(); // radius y
			CopyScriptFloat(); // radius z
		}
		else if(stricmp(lpCmd,"eof")==0)
		{
			// output opcode
			SaveScriptLong((long)WS_EOF);
			break;
		}
		else
		{
			DisplayFatalErrorMessage("Invalid script command.",
									 lpCmd,
									 FALSE);
			return;
		}
	}

	// close compiler
	CloseScriptCompiler();

	// check flag
	if(!bEditMode)
	{
		// add world
		AddWorld(lpDestFile);

		// segment static triangles
		SegmentStaticTriangles();

		// add static triangle info
		AppendWorldScriptStaticTriangleInfo(lpDestFile);

		// unload world
		UnloadWorld();
	}
}
#endif

// AddWorld()

#ifdef DEBUG_MODE
void AddWorld(LPCSTR lpFile,
			  BOOL bEditInfo)
#else
void AddWorld(LPCSTR lpFile)
#endif
{
	LoadingOutput("AddWorld");

	// open compiled script
	if(!OpenCompiledScript(lpFile))
		return;

// check OpenCompiledScript() and related functions for all errors!!

	// string storage
	char lpStr1[32];
	char lpStr2[32];

	// read script
	while(TRUE)
	{
		// read command
		WSOPCODE wsCmd=(WSOPCODE)ReadCScriptLong();

		// process command
		if(wsCmd==WS_ABLEND)
		{
			// read flag
			BOOL bFlag=ReadCScriptLong();
			
			// set alpha blending
			if(bFlag)
				EnableAlphaBlend();
			else
				DisableAlphaBlend();
		}
		else if(wsCmd==WS_ATEST)
		{
			// read flag
			BOOL bFlag=ReadCScriptLong();
			
			// set alpha testing
			if(bFlag)
				EnableAlphaTest();
			else
				DisableAlphaTest();
		}
		else if(wsCmd==WS_SPECULAR)
		{
			// read flag
			BOOL bFlag=ReadCScriptLong();
			
			// set specular
			if(bFlag)
				EnableSpecular();
			else
				DisableSpecular();
		}
		else if(wsCmd==WS_FOG)
		{
			// read flag
			BOOL bFlag=ReadCScriptLong();
			
			// set fog
			if(bFlag)
				EnableFog();
			else
				DisableFog();
		}
		else if(wsCmd==WS_FOGPARAMS)
		{
			// read values
			float fStart=ReadCScriptFloat();
			float fEnd=ReadCScriptFloat();
			float fRed=ReadCScriptFloat();
			float fGreen=ReadCScriptFloat();
			float fBlue=ReadCScriptFloat();

			// set fog parameters
			SetFogStart(fStart);
			SetFogEnd(fEnd);
			SetFogColor(D3DCOLOR_COLORVALUE(fRed,
											fGreen,
											fBlue,
											1));

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				g_fFogR=fRed;
				g_fFogG=fGreen;
				g_fFogB=fBlue;
			}
#endif
		}
		else if(wsCmd==WS_AMBIENT)
		{
			// read values
			float fRed=ReadCScriptFloat();
			float fGreen=ReadCScriptFloat();
			float fBlue=ReadCScriptFloat();
			
			// set ambient light
			SetAmbientLight(D3DCOLOR_COLORVALUE(fRed,
												fGreen,
												fBlue,
												1));

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				g_fAmbientR=fRed;
				g_fAmbientG=fGreen;
				g_fAmbientB=fBlue;
			}
#endif
		}
		else if(wsCmd==WS_ZCLIP)
		{
			// read values
			float fClipNear=ReadCScriptFloat();
			float fClipFar=ReadCScriptFloat();
			
			// set projection matix
			SetProjectionMatrix(fClipNear,
								fClipFar);

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				g_fZClipNear=fClipNear;
				g_fZClipFar=fClipFar;
			}
#endif
		}
		else if(wsCmd==WS_GRAVITY)
		{
			// read values
			float fX=ReadCScriptFloat();
			float fY=ReadCScriptFloat();
			float fZ=ReadCScriptFloat();

			// set vector
			g_vGravity=D3DXVECTOR3(fX,fY,fZ);
		}
		else if(wsCmd==WS_SEGMENT)
		{
			// read values
			float fSize=ReadCScriptFloat();
			DWORD dwX=(DWORD)ReadCScriptLong();
			DWORD dwY=(DWORD)ReadCScriptLong();
			DWORD dwZ=(DWORD)ReadCScriptLong();

			// copy values
			g_fMinSTriSegSize=fSize;
			g_dwMaxSTriSegX=dwX;
			g_dwMaxSTriSegY=dwY;
			g_dwMaxSTriSegZ=dwZ;
		}
		else if(wsCmd==WS_PLIGHT)
		{
			// get light name
			ReadCScriptString(lpStr1);

			// read values
			float fDiffuseR=ReadCScriptFloat();
			float fDiffuseG=ReadCScriptFloat();
			float fDiffuseB=ReadCScriptFloat();
			float fDiffuseA=ReadCScriptFloat();
			float fSpecularR=ReadCScriptFloat();
			float fSpecularG=ReadCScriptFloat();
			float fSpecularB=ReadCScriptFloat();
			float fSpecularA=ReadCScriptFloat();
			float fPositionX=ReadCScriptFloat();
			float fPositionY=ReadCScriptFloat();
			float fPositionZ=ReadCScriptFloat();
			float fRange=ReadCScriptFloat();
			
#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				// add light material
				DWORD dwMat=AddMaterial(0,0,0,0.1F,
										0,0,0,0.1F,
										0,0,0,0.1F,
										fDiffuseR,
										fDiffuseG,
										fDiffuseB,
										0.05F,
										0);
				
				// add light matrix
				DWORD dwMtx=AddMatrix(1,0,0,0,
									  0,1,0,0,
									  0,0,1,0,
									  fPositionX,
									  fPositionY,
									  fPositionZ,
									  1);

				// add light source mesh (uses emissive material from sprites)
				DWORD dwMesh=AddMeshSphere(1,
										   8,8,
										   g_dwSkyboxMat);
				DWORD dwSWMesh1=AddStaticWorldMesh(dwMesh,dwMtx);

				// add light range mesh
				dwMesh=AddMeshSphere(fRange,
									 16,16,
									 dwMat);
				DWORD dwSWMesh2=AddStaticWorldMesh(dwMesh,dwMtx);

				LockGlobalMsg()
				
				// add info to name
				sprintf(g_lpMsg,
						"%s&%d&%d",
						lpStr1,
						dwSWMesh1,
						dwSWMesh2);

				// add point light
				DWORD dwLight=AddLight(D3DLIGHT_POINT,
									   fDiffuseR,
									   fDiffuseG,
									   fDiffuseB,
									   fDiffuseA,
									   fSpecularR,
									   fSpecularG,
									   fSpecularB,
									   fSpecularA,
									   0,0,0,1, // ambient
									   fPositionX,
									   fPositionY,
									   fPositionZ,
									   0,0,0, // direction
									   fRange,
									   1, // falloff
									   0,1,0, // attenuation
									   0, // theta,
									   0, // phi
									   g_lpMsg);

				UnlockGlobalMsg()

				// enable light
				EnableLight(dwLight);

				// separate name & info
				DivideString(g_lplpLightNames[dwLight]);
			}
			else
#endif
			// add & enable point light
			EnableLight(AddLight(D3DLIGHT_POINT,
								 fDiffuseR,
								 fDiffuseG,
								 fDiffuseB,
								 fDiffuseA,
								 fSpecularR,
								 fSpecularG,
								 fSpecularB,
								 fSpecularA,
								 0,0,0,1, // ambient
								 fPositionX,
								 fPositionY,
								 fPositionZ,
								 0,0,0, // direction
								 fRange,
								 1, // falloff
								 0,1,0, // attenuation
								 0, // theta,
								 0, // phi
								 lpStr1));
		}
		else if(wsCmd==WS_SLIGHT)
		{
			// get light name
			ReadCScriptString(lpStr1);
			
			// read values
			float fDiffuseR=ReadCScriptFloat();
			float fDiffuseG=ReadCScriptFloat();
			float fDiffuseB=ReadCScriptFloat();
			float fDiffuseA=ReadCScriptFloat();
			float fSpecularR=ReadCScriptFloat();
			float fSpecularG=ReadCScriptFloat();
			float fSpecularB=ReadCScriptFloat();
			float fSpecularA=ReadCScriptFloat();
			float fPositionX=ReadCScriptFloat();
			float fPositionY=ReadCScriptFloat();
			float fPositionZ=ReadCScriptFloat();
			float fDirectionX=ReadCScriptFloat();
			float fDirectionY=ReadCScriptFloat();
			float fDirectionZ=ReadCScriptFloat();
			float fRange=ReadCScriptFloat();
			float fTheta=DegToRad(ReadCScriptFloat());
			float fPhi=DegToRad(ReadCScriptFloat());

			// normalize direction
			Normalize(&fDirectionX,
					  &fDirectionY,
					  &fDirectionZ,
					  fDirectionX,
					  fDirectionY,
					  fDirectionZ);

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				// add light material
				DWORD dwMat=AddMaterial(0,0,0,0.1F,
										0,0,0,0.1F,
										0,0,0,0.1F,
										fDiffuseR,
										fDiffuseG,
										fDiffuseB,
										0.05F,
										0);
				
				// add light matrix
				DWORD dwMtx=AddCameraMatrix(fPositionX,
											fPositionY,
											fPositionZ,
											fPositionX+fDirectionX,
											fPositionY+fDirectionY,
											fPositionZ+fDirectionZ,
											fPositionX-fDirectionZ,
											fPositionY-fDirectionX,
											fPositionZ-fDirectionY);
				
				// add light source (uses emissive material from sprites)
				DWORD dwMesh=AddMeshSphere(0.5F,
										   8,8,
										   g_dwSkyboxMat);
				DWORD dwSWMesh1=AddStaticWorldMesh(dwMesh,dwMtx);

				// add light inner cone mesh
				dwMesh=AddMeshCylinder(EPSILON,
									   sinf(fTheta)*0.5F*fRange,
									   fRange-EPSILON,
									   16,1,
									   dwMat);
				OffsetMesh(dwMesh,0,0,fRange*0.5F);
				DWORD dwSWMesh2=AddStaticWorldMesh(dwMesh,dwMtx);

				// add light outer cone mesh
				dwMesh=AddMeshCylinder(0,
									   sinf(fPhi)*0.5F*fRange,
									   fRange,
									   16,1,
									   dwMat);
				OffsetMesh(dwMesh,0,0,fRange*0.5F);
				DWORD dwSWMesh3=AddStaticWorldMesh(dwMesh,dwMtx);

				LockGlobalMsg()

				// add info to name
				sprintf(g_lpMsg,
						"%s&%d&%d&%d",
						lpStr1,
						dwSWMesh1,
						dwSWMesh2,
						dwSWMesh3);

				// add spot light
				DWORD dwLight=AddLight(D3DLIGHT_SPOT,
									   fDiffuseR,
									   fDiffuseG,
									   fDiffuseB,
									   fDiffuseA,
									   fSpecularR,
									   fSpecularG,
									   fSpecularB,
									   fSpecularA,
									   0,0,0,1, // ambient
									   fPositionX,
									   fPositionY,
									   fPositionZ,
									   fDirectionX,
									   fDirectionY,
									   fDirectionZ,
									   fRange,
									   1, // falloff
									   0,1,0, // attenuation
									   fTheta,
									   fPhi,
									   g_lpMsg);

				UnlockGlobalMsg()

				// enable light
				EnableLight(dwLight);

				// separate name & info
				DivideString(g_lplpLightNames[dwLight]);
			}
			else
#endif
				// add & enable spot light
				EnableLight(AddLight(D3DLIGHT_SPOT,
									 fDiffuseR,
									 fDiffuseG,
									 fDiffuseB,
									 fDiffuseA,
									 fSpecularR,
									 fSpecularG,
									 fSpecularB,
									 fSpecularA,
									 0,0,0,1, // ambient
									 fPositionX,
									 fPositionY,
									 fPositionZ,
									 fDirectionX,
									 fDirectionY,
									 fDirectionZ,
									 fRange,
									 1, // falloff
									 0,1,0, // attenuation
									 fTheta,
									 fPhi,
									 lpStr1));
		}
		else if(wsCmd==WS_DLIGHT)
		{
			// get light name
			ReadCScriptString(lpStr1);

			// read values
			float fDiffuseR=ReadCScriptFloat();
			float fDiffuseG=ReadCScriptFloat();
			float fDiffuseB=ReadCScriptFloat();
			float fDiffuseA=ReadCScriptFloat();
			float fDirectionX=ReadCScriptFloat();
			float fDirectionY=ReadCScriptFloat();
			float fDirectionZ=ReadCScriptFloat();

			// normalize direction
			Normalize(&fDirectionX,
					  &fDirectionY,
					  &fDirectionZ,
					  fDirectionX,
					  fDirectionY,
					  fDirectionZ);

			// add & enable directional light
			EnableLight(AddLight(D3DLIGHT_DIRECTIONAL,
								 fDiffuseR,
								 fDiffuseG,
								 fDiffuseB,
								 fDiffuseA,
								 0,0,0,1, // specular
								 0,0,0,1, // ambient
								 0,0,0, // position
								 fDirectionX,
								 fDirectionY,
								 fDirectionZ,
								 0, // range
								 1, // falloff
								 0,1,0, // attenuation
								 0, // theta
								 0, // phi
								 lpStr1));
		}
		else if(wsCmd==WS_MATERIAL)
		{
			// get material name
			ReadCScriptString(lpStr1);

			// read values
			float fDiffuseR=ReadCScriptFloat();
			float fDiffuseG=ReadCScriptFloat();
			float fDiffuseB=ReadCScriptFloat();
			float fDiffuseA=ReadCScriptFloat();
			float fAmbientR=ReadCScriptFloat();
			float fAmbientG=ReadCScriptFloat();
			float fAmbientB=ReadCScriptFloat();
			float fAmbientA=ReadCScriptFloat();
			float fSpecularR=ReadCScriptFloat();
			float fSpecularG=ReadCScriptFloat();
			float fSpecularB=ReadCScriptFloat();
			float fSpecularA=ReadCScriptFloat();
			float fEmissiveR=ReadCScriptFloat();
			float fEmissiveG=ReadCScriptFloat();
			float fEmissiveB=ReadCScriptFloat();
			float fEmissiveA=ReadCScriptFloat();
			float fPower=ReadCScriptFloat();

			// add material
			AddMaterial(fDiffuseR,
						fDiffuseG,
						fDiffuseB,
						fDiffuseA,
						fAmbientR,
						fAmbientG,
						fAmbientB,
						fAmbientA,
						fSpecularR,
						fSpecularG,
						fSpecularB,
						fSpecularA,
						fEmissiveR,
						fEmissiveG,
						fEmissiveB,
						fEmissiveA,
						fPower,
						lpStr1);
		}
		else if(wsCmd==WS_TEXTURE)
		{
			// get texture name & filename
			ReadCScriptString(lpStr1);
			ReadCScriptString(lpStr2);

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				LockGlobalMsg()

				// add texture w/ filename
				sprintf(g_lpMsg,
						"%s&%s",
						lpStr1,
						lpStr2);

				// add texture
				DWORD dwIndex=AddTexture(lpStr2,
										 g_lpMsg);

				// separate texture & filename
				DivideString(g_lplpTexNames[dwIndex]);

				UnlockGlobalMsg()
			}
			else
#endif
			// add texture
			AddTexture(lpStr2,lpStr1);
		}
		else if(wsCmd==WS_MFILE)
		{
			// get mesh name & filename
			ReadCScriptString(lpStr1);
			ReadCScriptString(lpStr2);

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				LockGlobalMsg()

				// add mesh w/ info
				sprintf(g_lpMsg,
						"%s&file&%s",
						lpStr1,
						lpStr2);

				// add mesh
				DWORD dwIndex=AddMesh(lpStr2,
									  g_lpMsg);

				// separate mesh & info
				DivideString(g_lplpMeshNames[dwIndex]);

				UnlockGlobalMsg()
			}
			else
#endif
			// add mesh
			AddMesh(lpStr2,lpStr1);
		}
		else if(wsCmd==WS_MBOX)
		{
			// get mesh name
			ReadCScriptString(lpStr1);

			// read values
			float fWidth=ReadCScriptFloat();
			float fHeight=ReadCScriptFloat();
			float fDepth=ReadCScriptFloat();

			// get material
			ReadCScriptString(lpStr2);
			DWORD dwMat=FindMaterial(lpStr2);

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				LockGlobalMsg()

				// add mesh w/ info
				sprintf(g_lpMsg,
						"%s&box&%f&%f&%f&%d",
						lpStr1,
						fWidth,
						fHeight,
						fDepth,
						dwMat);

				// add mesh
				DWORD dwIndex=AddMeshBox(fWidth,
										 fHeight,
										 fDepth,
										 dwMat,
										 g_lpMsg);

				// separate mesh & info
				DivideString(g_lplpMeshNames[dwIndex]);

				UnlockGlobalMsg()
			}
			else
#endif
			// add mesh
			AddMeshBox(fWidth,
					   fHeight,
					   fDepth,
					   dwMat,
					   lpStr1);
		}
		else if(wsCmd==WS_MCYLINDER)
		{
			// get mesh name
			ReadCScriptString(lpStr1);

			// read values
			float fLowerRad=ReadCScriptFloat();
			float fUpperRad=ReadCScriptFloat();
			float fHeight=ReadCScriptFloat();
			DWORD dwNumSlices=(DWORD)ReadCScriptLong();
			DWORD dwNumStacks=(DWORD)ReadCScriptLong();

			// get material
			ReadCScriptString(lpStr2);
			DWORD dwMat=FindMaterial(lpStr2);

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				LockGlobalMsg()

				// add mesh w/ info
				sprintf(g_lpMsg,
						"%s&cylinder&%f&%f&%f&%d&%d&%d",
						lpStr1,
						fLowerRad,
						fUpperRad,
						fHeight,
						dwNumSlices,
						dwNumStacks,
						dwMat);

				// add mesh
				DWORD dwIndex=AddMeshCylinder(fLowerRad,
											  fUpperRad,
											  fHeight,
											  dwNumSlices,
											  dwNumStacks,
											  dwMat,
											  g_lpMsg);

				// separate mesh & info
				DivideString(g_lplpMeshNames[dwIndex]);

				UnlockGlobalMsg()
			}
			else
#endif
			// add mesh
			AddMeshCylinder(fLowerRad,
							fUpperRad,
						    fHeight,
							dwNumSlices,
							dwNumStacks,
							dwMat,
							lpStr1);
		}
		else if(wsCmd==WS_MPOLYGON)
		{
			// get mesh name
			ReadCScriptString(lpStr1);

			// read values
			float fLengthOfSide=ReadCScriptFloat();
			DWORD dwNumSides=(DWORD)ReadCScriptLong();

			// get material
			ReadCScriptString(lpStr2);
			DWORD dwMat=FindMaterial(lpStr2);

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				LockGlobalMsg()

				// add mesh w/ info
				sprintf(g_lpMsg,
						"%s&polygon&%f&%d&%d",
						lpStr1,
						fLengthOfSide,
						dwNumSides,
						dwMat);

				// add mesh
				DWORD dwIndex=AddMeshPolygon(fLengthOfSide,
											 dwNumSides,
											 dwMat,
											 g_lpMsg);

				// separate mesh & info
				DivideString(g_lplpMeshNames[dwIndex]);

				UnlockGlobalMsg()
			}
			else
#endif
			// add mesh
			AddMeshPolygon(fLengthOfSide,
						   dwNumSides,
						   dwMat,
						   lpStr1);
		}
		else if(wsCmd==WS_MSPHERE)
		{
			// get mesh name
			ReadCScriptString(lpStr1);

			// read values
			float fRadius=ReadCScriptFloat();
			DWORD dwNumSlices=(DWORD)ReadCScriptLong();
			DWORD dwNumStacks=(DWORD)ReadCScriptLong();

			// get material
			ReadCScriptString(lpStr2);
			DWORD dwMat=FindMaterial(lpStr2);

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				LockGlobalMsg()

				// add mesh w/ info
				sprintf(g_lpMsg,
						"%s&sphere&%f&%d&%d&%d",
						lpStr1,
						fRadius,
						dwNumSlices,
						dwNumStacks,
						dwMat);

				// add mesh
				DWORD dwIndex=AddMeshSphere(fRadius,
											dwNumSlices,
											dwNumStacks,
											dwMat,
											g_lpMsg);

				// separate mesh & info
				DivideString(g_lplpMeshNames[dwIndex]);

				UnlockGlobalMsg()
			}
			else
#endif
			// add mesh
			AddMeshSphere(fRadius,
						  dwNumSlices,
						  dwNumStacks,
						  dwMat,
						  lpStr1);
		}
		else if(wsCmd==WS_MTEAPOT)
		{
			// get mesh name
			ReadCScriptString(lpStr1);

			// read values
			ReadCScriptString(lpStr2);
			DWORD dwMat=FindMaterial(lpStr2);

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				LockGlobalMsg()

				// add mesh w/ info
				sprintf(g_lpMsg,
						"%s&teapot&%d",
						lpStr1,
						dwMat);

				// add mesh
				DWORD dwIndex=AddMeshTeapot(dwMat,
										    g_lpMsg);

				// separate mesh & info
				DivideString(g_lplpMeshNames[dwIndex]);

				UnlockGlobalMsg()
			}
			else
#endif
			// add mesh
			AddMeshTeapot(dwMat,
						  lpStr1);
		}
		else if(wsCmd==WS_MTORUS)
		{
			// get mesh name
			ReadCScriptString(lpStr1);

			// read values
			float fInnerRadius=ReadCScriptFloat();
			float fOuterRadius=ReadCScriptFloat();
			DWORD dwNumSides=(DWORD)ReadCScriptLong();
			DWORD dwNumRings=(DWORD)ReadCScriptLong();

			// get material
			ReadCScriptString(lpStr2);
			DWORD dwMat=FindMaterial(lpStr2);

#ifdef DEBUG_MODE
			if(bEditInfo)
			{
				LockGlobalMsg()

				// add mesh w/ info
				sprintf(g_lpMsg,
						"%s&torus&%f&%f&%d&%d&%d",
						lpStr1,
						fInnerRadius,
						fOuterRadius,
						dwNumSides,
						dwNumRings,
						dwMat);

				// add mesh
				DWORD dwIndex=AddMeshTorus(fInnerRadius,
										   fOuterRadius,
										   dwNumSides,
										   dwNumRings,
										   dwMat,
										   g_lpMsg);

				// separate mesh & info
				DivideString(g_lplpMeshNames[dwIndex]);

				UnlockGlobalMsg()
			}
			else
#endif
			// add mesh
			AddMeshTorus(fInnerRadius,
						 fOuterRadius,
						 dwNumSides,
						 dwNumRings,
						 dwMat,
						 lpStr1);
		}
		else if(wsCmd==WS_SKYBOX)
		{
			// get radius
			float fRadius=ReadCScriptFloat();
			
			// get offset
			float fOffsetX=ReadCScriptFloat();
			float fOffsetY=ReadCScriptFloat();
			float fOffsetZ=ReadCScriptFloat();

			// get texture indices
			ReadCScriptString(lpStr1);
			DWORD dwTex1=FindTexture(lpStr1);
			ReadCScriptString(lpStr1);
			DWORD dwTex2=FindTexture(lpStr1);
			ReadCScriptString(lpStr1);
			DWORD dwTex3=FindTexture(lpStr1);
			ReadCScriptString(lpStr1);
			DWORD dwTex4=FindTexture(lpStr1);
			ReadCScriptString(lpStr1);
			DWORD dwTex5=FindTexture(lpStr1);
			ReadCScriptString(lpStr1);
			DWORD dwTex6=FindTexture(lpStr1);

			// set skybox
			SetSkybox(fRadius,
					  fOffsetX,
					  fOffsetY,
					  fOffsetZ,
					  dwTex1,
					  dwTex2,
					  dwTex3,
					  dwTex4,
					  dwTex5,
					  dwTex6);
		}
		else if(wsCmd==WS_SWMESH)
		{
			// get mesh name
			ReadCScriptString(lpStr1);
			
			// get mesh index
			ReadCScriptString(lpStr2);
			DWORD dwMeshIndex=FindMesh(lpStr2);

			// get matrix
			D3DXMATRIX mMtx;
			mMtx._11=ReadCScriptFloat();
			mMtx._12=ReadCScriptFloat();
			mMtx._13=ReadCScriptFloat();
			mMtx._14=ReadCScriptFloat();
			mMtx._21=ReadCScriptFloat();
			mMtx._22=ReadCScriptFloat();
			mMtx._23=ReadCScriptFloat();
			mMtx._24=ReadCScriptFloat();
			mMtx._31=ReadCScriptFloat();
			mMtx._32=ReadCScriptFloat();
			mMtx._33=ReadCScriptFloat();
			mMtx._34=ReadCScriptFloat();
			mMtx._41=ReadCScriptFloat();
			mMtx._42=ReadCScriptFloat();
			mMtx._43=ReadCScriptFloat();
			mMtx._44=ReadCScriptFloat();

			// add static world mesh
			AddStaticWorldMesh(dwMeshIndex,
							   &mMtx,
							   lpStr1);
		}
		else if(wsCmd==WS_DWMESH)
		{
			// get mesh name
			ReadCScriptString(lpStr1);
			
			// get mesh index
			ReadCScriptString(lpStr2);
			DWORD dwMeshIndex=FindMesh(lpStr2);

			// get matrix
			D3DXMATRIX mMtx;
			mMtx._11=ReadCScriptFloat();
			mMtx._12=ReadCScriptFloat();
			mMtx._13=ReadCScriptFloat();
			mMtx._14=ReadCScriptFloat();
			mMtx._21=ReadCScriptFloat();
			mMtx._22=ReadCScriptFloat();
			mMtx._23=ReadCScriptFloat();
			mMtx._24=ReadCScriptFloat();
			mMtx._31=ReadCScriptFloat();
			mMtx._32=ReadCScriptFloat();
			mMtx._33=ReadCScriptFloat();
			mMtx._34=ReadCScriptFloat();
			mMtx._41=ReadCScriptFloat();
			mMtx._42=ReadCScriptFloat();
			mMtx._43=ReadCScriptFloat();
			mMtx._44=ReadCScriptFloat();

			// add dynamic world mesh
			AddDynamicWorldMesh(dwMeshIndex,
							    &mMtx,
								lpStr1);
		}
		else if(wsCmd==WS_SWOBJ)
		{
			// get name
			ReadCScriptString(lpStr1);
			
			// get data
			float fPositionX=ReadCScriptFloat();
			float fPositionY=ReadCScriptFloat();
			float fPositionZ=ReadCScriptFloat();
			float fRadiusX=ReadCScriptFloat();
			float fRadiusY=ReadCScriptFloat();
			float fRadiusZ=ReadCScriptFloat();

			// add static world object
			AddStaticWorldObject(fPositionX,
								 fPositionY,
								 fPositionZ,
								 fRadiusX,
								 fRadiusY,
								 fRadiusZ,
								 lpStr1);
		}
		else if(wsCmd==WS_DWOBJ)
		{
			// get name
			ReadCScriptString(lpStr1);
			
			// get data
			float fPositionX=ReadCScriptFloat();
			float fPositionY=ReadCScriptFloat();
			float fPositionZ=ReadCScriptFloat();
			float fRadiusX=ReadCScriptFloat();
			float fRadiusY=ReadCScriptFloat();
			float fRadiusZ=ReadCScriptFloat();

			// add dynamic world object
			AddDynamicWorldObject(fPositionX,
								  fPositionY,
								  fPositionZ,
								  fRadiusX,
								  fRadiusY,
								  fRadiusZ,
								  0,0,1,
								  lpStr1);
		}
		else if(wsCmd==WS_SWTRI)
		{
			// read static triangle info
			ReadWorldScriptStaticTriangleInfo();
		}
		else if(wsCmd==WS_EOF)
		{
			// ok
			break;
		}
		else
		{
			// uh-oh!
			DisplayFatalErrorMessage("Invalid compiled script.",
									 NULL,
									 FALSE);
			return;
		}
	}

	// close compiled script
	CloseCompiledScript();

#ifdef DEBUG_MODE
	// init world object mesh
	InitWorldObjectMesh();
#endif
}

// RenderWorld()

void RenderWorld(DWORD dwMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "RenderWorld()",
								 FALSE);
		return;
	}
#endif

	// use other function
	RenderWorld(g_lpMtx[dwMtxIndex].mtx._41,
				g_lpMtx[dwMtxIndex].mtx._42,
				g_lpMtx[dwMtxIndex].mtx._43);
}

// RenderWorld()

void RenderWorld(float fXCamPos,
				 float fYCamPos,
				 float fZCamPos)
{
	// render individual objects
	RenderSkybox(fXCamPos,
				 fYCamPos,
				 fZCamPos);
	RenderDynamicWorldMeshes();
	RenderStaticWorldMeshes();
}

#ifdef DEBUG_MODE
// InitMiniFont()

void InitMiniFont(LPDWORD dwMiniFont)
{
	LoadingOutput("InitMiniFont");

	// counter
	DWORD dwCount;

	// add mini font texture
	g_dwMFTexIdx=AddTexture(256,256,TRUE,TRUE);

	// init all values
	for(dwCount=0;dwCount<256;dwCount++)
		dwMiniFont[dwCount]=0xFFFFFFFF;

	// init specific characters
	dwMiniFont[32]=GetMiniChar("     "
							   "     "
							   "     "
							   "     "
							   "     ");

	dwMiniFont['A']=GetMiniChar(" xxx "
							    "x   x"
							    "xxxxx"
							    "x   x"
							    "x   x");

	dwMiniFont['B']=GetMiniChar("xxxx "
							    "x   x"
							    "xxxx "
							    "x   x"
							    "xxxx ");

	dwMiniFont['C']=GetMiniChar(" xxxx"
							    "x    "
							    "x    "
							    "x    "
							    " xxxx");

	dwMiniFont['D']=GetMiniChar("xxxx "
							    "x   x"
							    "x   x"
							    "x   x"
							    "xxxx ");

	dwMiniFont['E']=GetMiniChar("xxxxx"
							    "x    "
							    "xxxx "
							    "x    "
							    "xxxxx");

	dwMiniFont['F']=GetMiniChar("xxxxx"
							    "x    "
							    "xxxx "
							    "x    "
							    "x    ");

	dwMiniFont['G']=GetMiniChar(" xxxx"
							    "x    "
							    "x  xx"
							    "x   x"
							    " xxxx");

	dwMiniFont['H']=GetMiniChar("x   x"
							    "x   x"
							    "xxxxx"
							    "x   x"
							    "x   x");

	dwMiniFont['I']=GetMiniChar(" xxx "
							    "  x  "
							    "  x  "
							    "  x  "
							    " xxx ");

	dwMiniFont['J']=GetMiniChar("    x"
							    "    x"
							    "    x"
							    "x   x"
							    " xxx ");

	dwMiniFont['K']=GetMiniChar("x   x"
							    "x  x "
							    "xxx  "
							    "x  x "
							    "x   x");

	dwMiniFont['L']=GetMiniChar("x    "
							    "x    "
							    "x    "
							    "x    "
							    "xxxxx");

	dwMiniFont['M']=GetMiniChar("x   x"
							    "xx xx"
							    "x x x"
							    "x   x"
							    "x   x");

	dwMiniFont['N']=GetMiniChar("x   x"
							    "xx  x"
							    "x x x"
							    "x  xx"
							    "x   x");

	dwMiniFont['O']=GetMiniChar(" xxx "
							    "x   x"
							    "x   x"
							    "x   x"
							    " xxx ");

	dwMiniFont['P']=GetMiniChar("xxxx "
							    "x   x"
							    "xxxx "
							    "x    "
							    "x    ");

	dwMiniFont['Q']=GetMiniChar(" xxx "
							    "x   x"
							    "x   x"
							    "x  x "
							    " xx x");

	dwMiniFont['R']=GetMiniChar("xxxx "
							    "x   x"
							    "xxxx "
							    "x   x"
							    "x   x");

	dwMiniFont['S']=GetMiniChar(" xxxx"
							    "x    "
							    " xxx "
							    "    x"
							    "xxxx ");

	dwMiniFont['T']=GetMiniChar("xxxxx"
							    "  x  "
							    "  x  "
							    "  x  "
							    "  x  ");

	dwMiniFont['U']=GetMiniChar("x   x"
							    "x   x"
							    "x   x"
							    "x   x"
							    " xxx ");

	dwMiniFont['V']=GetMiniChar("x   x"
							    "x   x"
							    "x   x"
							    " x x "
							    "  x  ");

	dwMiniFont['W']=GetMiniChar("x   x"
							    "x   x"
							    "x x x"
							    "x x x"
							    " x x ");

	dwMiniFont['X']=GetMiniChar("x   x"
							    " x x "
							    "  x  "
							    " x x "
							    "x   x");

	dwMiniFont['Y']=GetMiniChar("x   x"
							    " x x "
							    "  x  "
							    "  x  "
							    "  x  ");

	dwMiniFont['Z']=GetMiniChar("xxxxx"
							    "   x "
							    "  x  "
							    " x   "
							    "xxxxx");

	dwMiniFont['0']=GetMiniChar(" xxx "
							    "x  xx"
							    "x x x"
							    "xx  x"
							    " xxx ");

	dwMiniFont['1']=GetMiniChar("  x  "
							    " xx  "
							    "  x  "
							    "  x  "
							    " xxx ");

	dwMiniFont['2']=GetMiniChar("xxxx "
							    "    x"
							    "  xx "
							    " x   "
							    "xxxxx");

	dwMiniFont['3']=GetMiniChar("xxxx "
							    "    x"
							    " xxx "
							    "    x"
							    "xxxx ");

	dwMiniFont['4']=GetMiniChar("   x "
							    "  xx "
							    " x x "
							    "xxxxx"
							    "   x ");

	dwMiniFont['5']=GetMiniChar("xxxxx"
							    "x    "
							    "xxxx "
							    "    x"
							    "xxxx ");

	dwMiniFont['6']=GetMiniChar(" xxx "
							    "x    "
							    "xxxx "
							    "x   x"
							    " xxx ");

	dwMiniFont['7']=GetMiniChar("xxxxx"
							    "    x"
							    "   x "
							    "  x  "
							    "  x  ");

	dwMiniFont['8']=GetMiniChar(" xxx "
							    "x   x"
							    " xxx "
							    "x   x"
							    " xxx ");

	dwMiniFont['9']=GetMiniChar(" xxx "
							    "x   x"
							    " xxxx"
							    "    x"
							    " xxx ");

	dwMiniFont['.']=GetMiniChar("     "
							    "     "
							    "     "
							    "     "
							    "  x  ");

	// copy upper-case to lower-case
	for(dwCount='A';dwCount<='Z';dwCount++)
		dwMiniFont[dwCount+('a'-'A')]=dwMiniFont[dwCount];
}

// GetMiniChar()

DWORD GetMiniChar(LPSTR lpBits)
{
	// counters
	DWORD dwCountX;
	DWORD dwCountY;
	
	// return value
	DWORD dwBits=0;

	// process bits
	for(dwCountY=0;dwCountY<5;dwCountY++)
		for(dwCountX=0;dwCountX<5;dwCountX++)
			if(lpBits[dwCountX+(dwCountY*5)]!=' ')
				dwBits+=1<<(dwCountX+(dwCountY*5));

	// return bits
	return(dwBits);
}

// RenderMiniFontString()

RECT RenderMiniFontString(LPDWORD lpdwMiniFont,
						  LPSTR lpStr,
						  int nXPos,
						  int nYPos,
						  BOOL bHighlight)
{
	// counters
	DWORD dwCount;
	int nX;
	int nY;

	// string color
	WORD wStrColor;

	// compute size 
	int nXSize=(strlen(lpStr)*6)+2;
	int nYSize=8;
	
	// compute bounding rectangle
	RECT rBound;
	rBound.left=nXPos-2;
	rBound.right=rBound.left+nXSize;
	rBound.top=nYPos-2;
	rBound.bottom=rBound.top+nYSize;

	// scale rectangle to client coordinates
	ScaleRectToClient(&rBound);

	// check for out-of-bounds
	if(nXSize>=256||
	   nYSize>=256)
	{
		DisplayErrorMessage("Mini font string out of bounds.",
							lpStr,
							FALSE);
		return(rBound);
	}

	// clear texture
	ClearTexture(g_dwMFTexIdx);
	
	// lock texture
	D3DLOCKED_RECT lr=LockTexture(g_dwMFTexIdx);
	DWORD dwPitch=lr.Pitch>>1;
	LPWORD lpData=(LPWORD)lr.pBits;

	// check highlight flag
	if(bHighlight)
	{
		// render highlighted bounding box
		for(nY=0;nY<=nYSize;nY++)
			for(nX=0;nX<=nXSize;nX++)
				if(nX==0||
				   nX==nXSize||
				   nY==0||
				   nY==nYSize)
					SetRGBA444(lpData[nX+(nY*((int)dwPitch))],1,1,1,1)
				else
					SetRGBA444(lpData[nX+(nY*((int)dwPitch))],1,1,0,0.75F)

		// set string color
		SetRGBA444(wStrColor,0,0,0,1);
	}
	else
	{
		// render darkened bounding box
		for(nY=0;nY<=nYSize;nY++)
			for(nX=0;nX<=nXSize;nX++)
				if(nX==0||
				   nX==nXSize||
				   nY==0||
				   nY==nYSize)
					SetRGBA444(lpData[nX+(nY*((int)dwPitch))],1,1,1,1)
				else
					SetRGBA444(lpData[nX+(nY*((int)dwPitch))],0,0,1,0.5F)

		// set string color
		SetRGBA444(wStrColor,1,1,1,1);
	}

	// check string
	if(strlen(lpStr))
	{
		// set x render position
		int nXRPos=2;
		
		// render string
		for(dwCount=0;dwCount<strlen(lpStr);dwCount++)
		{
			// render character
			for(nY=0;nY<5;nY++)
				for(nX=0;nX<5;nX++)
					if(lpdwMiniFont[lpStr[dwCount]]&(1<<(nX+(nY*5))))
						lpData[(nXRPos+nX)+((nY+2)*((int)dwPitch))]=wStrColor;

			// increment x render position
			nXRPos+=6;
		}
	}

	// unlock texture
	UnlockTexture(g_dwMFTexIdx);

	// render texture as sprite
	if(BeginSprites())
	{
		OutputSprite(g_dwMFTexIdx,
					 ScrCoords(nXPos+128,nYPos+128),
					 ScrSize(256,256));
		EndSprites();
	}

	// return bounding box
	return(rBound);
}

// CreateDefaultWorldScript()

void CreateDefaultWorldScript(LPCSTR lpFile)
{
	LoadingOutput("CreateDefaultWorldScript");

	// open script creator
	if(!OpenScriptCreator(lpFile))
		return;

	// output default script
	CreateScriptString("<environment>");
	CreateScriptString("alphablend on");
	CreateScriptString("alphatest  on");
	CreateScriptString("specular   off");
	CreateScriptString("fog        off");
	CreateScriptString("ambient    (1,1,1)");
	CreateScriptString("zclip      (1,100)");
	CreateScriptString("gravity    (0,0,0)");
	CreateScriptString("segment    1 (8,8,8)");
	CreateScriptString("<end file>");
	CreateScriptString("eof");

	// close script creator
	CloseScriptCreator();
}

// UpdateWorld()

BOOL UpdateWorld(void)
{
	LoadingOutput("UpdateWorld");

	// output script to temporary file
	if(!SaveWorld("~temp.rws"))
		return(FALSE);
	
	// unload world
	UnloadWorld();

	// load world
	CompileWorldScript("~temp.rwc",
					   "~temp.rws",
					   TRUE);
	AddWorld("~temp.rwc",TRUE);

	// delete temporary files
	DeleteFile("~temp.rws");
	DeleteFile("~temp.rwc");

	// ok
	return(TRUE);
}

// SaveWorld()

BOOL SaveWorld(LPCSTR lpFile)
{
	LoadingOutput("SaveWorld");

	// counter
	DWORD dwCount;
	
	// open script creator
	if(!OpenScriptCreator(lpFile))
		return(FALSE);

	LockGlobalMsg()

	// output environment settings
	CreateScriptString("<environment>");
	if(g_bABlend)   CreateScriptString("alphablend on"); else CreateScriptString("alphablend off");
	if(g_bATest)    CreateScriptString("alphatest  on"); else CreateScriptString("alphatest  off");
	if(g_bSpecular) CreateScriptString("specular   on"); else CreateScriptString("specular   off");
	if(g_bFog)		CreateScriptString("fog        on"); else CreateScriptString("fog        off");
	
	// output fog parameters
	sprintf(g_lpMsg,
			"fogparams  %f,%f (%f,%f,%f)",
			g_fFogStart,
			g_fFogEnd,
			g_fFogR,
			g_fFogG,
			g_fFogB);
	CreateScriptString(g_lpMsg);

	// output ambient light
	sprintf(g_lpMsg,
			"ambient    (%f,%f,%f)",
			g_fAmbientR,
			g_fAmbientG,
			g_fAmbientB);
	CreateScriptString(g_lpMsg);

	// output z-clip values
	sprintf(g_lpMsg,
			"zclip      (%f,%f)",
			g_fZClipNear,
			g_fZClipFar);
	CreateScriptString(g_lpMsg);

	// output gravity
	sprintf(g_lpMsg,
			"gravity    (%f,%f,%f)",
			g_vGravity.x,
			g_vGravity.y,
			g_vGravity.z);
	CreateScriptString(g_lpMsg);

	// output segment parameters
	sprintf(g_lpMsg,
			"segment    %f (%d,%d,%d)",
			g_fMinSTriSegSize,
			g_dwMaxSTriSegX,
			g_dwMaxSTriSegY,
			g_dwMaxSTriSegZ);
	CreateScriptString(g_lpMsg);

	// check materials
	if(g_dwNumMat)
	{
		// reset flag
		BOOL bFlag=FALSE;

		// output each material
		for(dwCount=0;dwCount<g_dwNumMat;dwCount++)
		{
			// get material name
			LPSTR lpName=g_lplpMatNames[dwCount];

			// verify name
			if(lpName[0]!='~')
			{
				// check flag
				if(!bFlag)
				{
					// output header
					CreateScriptString("<materials>");

					// set flag
					bFlag=TRUE;
				}
				
				// output data
				sprintf(g_lpMsg,
						"material %s (%f,%f,%f,%f) (%f,%f,%f,%f) (%f,%f,%f,%f) (%f,%f,%f,%f) %f",
						lpName,
						g_lpMat[dwCount].Diffuse.r,
						g_lpMat[dwCount].Diffuse.g,
						g_lpMat[dwCount].Diffuse.b,
						g_lpMat[dwCount].Diffuse.a,
						g_lpMat[dwCount].Ambient.r,
						g_lpMat[dwCount].Ambient.g,
						g_lpMat[dwCount].Ambient.b,
						g_lpMat[dwCount].Ambient.a,
						g_lpMat[dwCount].Specular.r,
						g_lpMat[dwCount].Specular.g,
						g_lpMat[dwCount].Specular.b,
						g_lpMat[dwCount].Specular.a,
						g_lpMat[dwCount].Emissive.r,
						g_lpMat[dwCount].Emissive.g,
						g_lpMat[dwCount].Emissive.b,
						g_lpMat[dwCount].Emissive.a,
						g_lpMat[dwCount].Power);
				CreateScriptString(g_lpMsg);
			}
		}
	}

	// check textures
	if(g_dwNumTex)
	{
		// reset flag
		BOOL bFlag=FALSE;

		// output each texture
		for(dwCount=0;dwCount<g_dwNumTex;dwCount++)
		{
			// get texture name
			LPSTR lpName=g_lplpTexNames[dwCount];

			// verify name
			if(lpName[0]!='~')
			{
				// verify file
				if(GetFileLength(GetStringDivision(lpName,1)))
				{
					// check flag
					if(!bFlag)
					{
						// output header
						CreateScriptString("<textures>");

						// set flag
						bFlag=TRUE;
					}

					// output data
					sprintf(g_lpMsg,
							"texture %s %s",
							GetStringDivision(lpName,0),
							GetStringDivision(lpName,1));
					CreateScriptString(g_lpMsg);
				}
			}
		}
	}

	// macro for skybox output
	#define SKYBOX_OUT(a) { if(a!=g_dwSkyboxTex) strcat(g_lpMsg,g_lplpTexNames[a]); else strcat(g_lpMsg,"null"); }
	
	// output skybox
	CreateScriptString("<skybox>");
	sprintf(g_lpMsg,
			"skybox %f (%f,%f,%f) (",
			g_fSkyboxRadius,
			g_vSkyboxOffset.x,
			g_vSkyboxOffset.y,
			g_vSkyboxOffset.z);
	SKYBOX_OUT(g_sbLeft.dwTexIndex);
	strcat(g_lpMsg,",");
	SKYBOX_OUT(g_sbRight.dwTexIndex);
	strcat(g_lpMsg,",");
	SKYBOX_OUT(g_sbFront.dwTexIndex);
	strcat(g_lpMsg,",");
	SKYBOX_OUT(g_sbBack.dwTexIndex);
	strcat(g_lpMsg,",");
	SKYBOX_OUT(g_sbTop.dwTexIndex);
	strcat(g_lpMsg,",");
	SKYBOX_OUT(g_sbBottom.dwTexIndex);
	strcat(g_lpMsg,")");
	CreateScriptString(g_lpMsg);

	// check meshes
	if(g_dwNumMesh)
	{
		// reset flag
		BOOL bFlag=FALSE;
		
		// output each mesh
		for(dwCount=0;dwCount<g_dwNumMesh;dwCount++)
		{
			// get mesh name
			LPSTR lpName=g_lplpMeshNames[dwCount];

			// verify name
			if(lpName[0]!='~')
			{
				// check flag
				if(!bFlag)
				{
					// output header
					CreateScriptString("<meshes>");

					// set flag
					bFlag=TRUE;
				}

				// determine mesh type
				if(stricmp(GetStringDivision(g_lplpMeshNames[dwCount],1),
						   "file")==0)
				{
					// verify file
					if(GetFileLength(GetStringDivision(g_lplpMeshNames[dwCount],2)))
					{
						// generate string
						sprintf(g_lpMsg,
								"mesh         %s %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								GetStringDivision(g_lplpMeshNames[dwCount],2));

						// output mesh
						CreateScriptString(g_lpMsg);
					}
				}
				else if(stricmp(GetStringDivision(g_lplpMeshNames[dwCount],1),
								"box")==0)
				{
					// get material
					DWORD dwMat=atoi(GetStringDivision(g_lplpMeshNames[dwCount],5));
					
					// generate string
					if(dwMat<g_dwNumMat)
						sprintf(g_lpMsg,
								"meshbox      %s (%s,%s,%s) %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								GetStringDivision(g_lplpMeshNames[dwCount],2),
								GetStringDivision(g_lplpMeshNames[dwCount],3),
								GetStringDivision(g_lplpMeshNames[dwCount],4),
								g_lplpMatNames[dwMat]);
					else
						sprintf(g_lpMsg,
								"meshbox      %s (%s,%s,%s) %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								GetStringDivision(g_lplpMeshNames[dwCount],2),
								GetStringDivision(g_lplpMeshNames[dwCount],3),
								GetStringDivision(g_lplpMeshNames[dwCount],4),
								"null");

					// output mesh
					CreateScriptString(g_lpMsg);
				}
				else if(stricmp(GetStringDivision(g_lplpMeshNames[dwCount],1),
								"cylinder")==0)
				{
					// get material
					DWORD dwMat=atoi(GetStringDivision(g_lplpMeshNames[dwCount],7));
					
					// generate string
					if(dwMat<g_dwNumMat)
						sprintf(g_lpMsg,
								"meshcylinder %s (%s,%s) %s (%s,%s) %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								GetStringDivision(g_lplpMeshNames[dwCount],2),
								GetStringDivision(g_lplpMeshNames[dwCount],3),
								GetStringDivision(g_lplpMeshNames[dwCount],4),
								GetStringDivision(g_lplpMeshNames[dwCount],5),
								GetStringDivision(g_lplpMeshNames[dwCount],6),
								g_lplpMatNames[dwMat]);
					else
						sprintf(g_lpMsg,
								"meshcylinder %s (%s,%s) %s (%s,%s) %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								GetStringDivision(g_lplpMeshNames[dwCount],2),
								GetStringDivision(g_lplpMeshNames[dwCount],3),
								GetStringDivision(g_lplpMeshNames[dwCount],4),
								GetStringDivision(g_lplpMeshNames[dwCount],5),
								GetStringDivision(g_lplpMeshNames[dwCount],6),
								"null");

					// output mesh
					CreateScriptString(g_lpMsg);
				}
				else if(stricmp(GetStringDivision(g_lplpMeshNames[dwCount],1),
								"polygon")==0)
				{
					// get material
					DWORD dwMat=atoi(GetStringDivision(g_lplpMeshNames[dwCount],4));
					
					// generate string
					if(dwMat<g_dwNumMat)
						sprintf(g_lpMsg,
								"meshpolygon  %s (%s,%s) %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								GetStringDivision(g_lplpMeshNames[dwCount],2),
								GetStringDivision(g_lplpMeshNames[dwCount],3),
								g_lplpMatNames[dwMat]);
					else
						sprintf(g_lpMsg,
								"meshpolygon  %s (%s,%s) %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								GetStringDivision(g_lplpMeshNames[dwCount],2),
								GetStringDivision(g_lplpMeshNames[dwCount],3),
								"null");

					// output mesh
					CreateScriptString(g_lpMsg);
				}
				else if(stricmp(GetStringDivision(g_lplpMeshNames[dwCount],1),
								"sphere")==0)
				{
					// get material
					DWORD dwMat=atoi(GetStringDivision(g_lplpMeshNames[dwCount],5));
					
					// generate string
					if(dwMat<g_dwNumMat)
							sprintf(g_lpMsg,
								"meshsphere   %s %s (%s,%s) %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								GetStringDivision(g_lplpMeshNames[dwCount],2),
								GetStringDivision(g_lplpMeshNames[dwCount],3),
								GetStringDivision(g_lplpMeshNames[dwCount],4),
								g_lplpMatNames[dwMat]);
					else
							sprintf(g_lpMsg,
								"meshsphere   %s %s (%s,%s) %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								GetStringDivision(g_lplpMeshNames[dwCount],2),
								GetStringDivision(g_lplpMeshNames[dwCount],3),
								GetStringDivision(g_lplpMeshNames[dwCount],4),
								"null");

					// output mesh
					CreateScriptString(g_lpMsg);
				}
				else if(stricmp(GetStringDivision(g_lplpMeshNames[dwCount],1),
								"teapot")==0)
				{
					// get material
					DWORD dwMat=atoi(GetStringDivision(g_lplpMeshNames[dwCount],2));
					
					// generate string
					if(dwMat<g_dwNumMat)
						sprintf(g_lpMsg,
								"meshteapot   %s %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								g_lplpMatNames[dwMat]);
					else
						sprintf(g_lpMsg,
								"meshteapot   %s %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								"null");

					// output mesh
					CreateScriptString(g_lpMsg);
				}
				else if(stricmp(GetStringDivision(g_lplpMeshNames[dwCount],1),
								"torus")==0)
				{
					// get material
					DWORD dwMat=atoi(GetStringDivision(g_lplpMeshNames[dwCount],6));
					
					// generate string
					if(dwMat<g_dwNumMat)
						sprintf(g_lpMsg,
								"meshtorus    %s (%s,%s) (%s,%s) %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								GetStringDivision(g_lplpMeshNames[dwCount],2),
								GetStringDivision(g_lplpMeshNames[dwCount],3),
								GetStringDivision(g_lplpMeshNames[dwCount],4),
								GetStringDivision(g_lplpMeshNames[dwCount],5),
								g_lplpMatNames[dwMat]);
					else
						sprintf(g_lpMsg,
								"meshtorus    %s (%s,%s) (%s,%s) %s",
								GetStringDivision(g_lplpMeshNames[dwCount],0),
								GetStringDivision(g_lplpMeshNames[dwCount],2),
								GetStringDivision(g_lplpMeshNames[dwCount],3),
								GetStringDivision(g_lplpMeshNames[dwCount],4),
								GetStringDivision(g_lplpMeshNames[dwCount],5),
								"null");

					// output mesh
					CreateScriptString(g_lpMsg);
				}
			}
		}
	}

	// check static world meshes
	if(g_dwNumSWMesh)
	{
		// reset flag
		BOOL bFlag=FALSE;

		// output each static world mesh
		for(dwCount=0;dwCount<g_dwNumSWMesh;dwCount++)
		{
			// get static world mesh name
			LPSTR lpSWMeshName=g_lplpSWMeshNames[dwCount];

			// verify name
			if(lpSWMeshName[0]!='~')
			{
				// verify mesh index
				if(g_lpdwSWMMeshIndex[dwCount]<g_dwNumMesh)
				{
					// get mesh name
					LPSTR lpMeshName=g_lplpMeshNames[g_lpdwSWMMeshIndex[dwCount]];

					// verify name
					if(lpMeshName[0]!='~')
					{
						// get matrix
						D3DXMATRIX mMtx;
						GetMatrix(g_lpdwSWMMtxIndex[dwCount],
								  &mMtx);

						// check flag
						if(!bFlag)
						{					
							// output header
							CreateScriptString("<static world meshes>");

							// set flag
							bFlag=TRUE;
						}

						// output data
						sprintf(g_lpMsg,
								"swmesh %s %s (%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f)",
								lpSWMeshName,
								lpMeshName,
								mMtx._11,
								mMtx._12,
								mMtx._13,
								mMtx._14,
								mMtx._21,
								mMtx._22,
								mMtx._23,
								mMtx._24,
								mMtx._31,
								mMtx._32,
								mMtx._33,
								mMtx._34,
								mMtx._41,
								mMtx._42,
								mMtx._43,
								mMtx._44);
						CreateScriptString(g_lpMsg);
					}
				}
			}
		}
	}

	// check dynamic world meshes
	if(g_dwNumDWMesh)
	{
		// reset flag
		BOOL bFlag=FALSE;
		
		// output each dynamic world mesh
		for(dwCount=0;dwCount<g_dwNumDWMesh;dwCount++)
		{
			// get dynamic world mesh name
			LPSTR lpDWMeshName=g_lplpDWMeshNames[dwCount];

			// verify name
			if(lpDWMeshName[0]!='~')
			{
				// verify mesh index
				if(g_lpdwDWMMeshIndex[dwCount]<g_dwNumMesh)
				{
					// get mesh name
					LPSTR lpMeshName=g_lplpMeshNames[g_lpdwDWMMeshIndex[dwCount]];

					// verify name
					if(lpMeshName[0]!='~')
					{
						// get matrix
						D3DXMATRIX mMtx;
						GetMatrix(g_lpdwDWMMtxIndex[dwCount],
								  &mMtx);

						// check flag
						if(!bFlag)
						{						
							// output header
							CreateScriptString("<dynamic world meshes>");

							// set flag
							bFlag=TRUE;
						}

						// output data
						sprintf(g_lpMsg,
								"dwmesh %s %s (%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f)",
								lpDWMeshName,
								lpMeshName,
								mMtx._11,
								mMtx._12,
								mMtx._13,
								mMtx._14,
								mMtx._21,
								mMtx._22,
								mMtx._23,
								mMtx._24,
								mMtx._31,
								mMtx._32,
								mMtx._33,
								mMtx._34,
								mMtx._41,
								mMtx._42,
								mMtx._43,
								mMtx._44);
						CreateScriptString(g_lpMsg);
					}
				}
			}
		}
	}

	// check static world objects
	if(g_dwNumSWObj)
	{
		// output static world objects
		CreateScriptString("<static world objects>");

		// output each static world object
		for(dwCount=0;dwCount<g_dwNumSWObj;dwCount++)
		{
			// get static world object name
			LPSTR lpSWObjName=g_lplpSWObjNames[dwCount];

			// verify name
			if(lpSWObjName[0]!='~')
			{
				// output data
				sprintf(g_lpMsg,
						"swobject %s (%f,%f,%f) (%f,%f,%f)",
						lpSWObjName,
						g_lpMtx[g_lpdwSWObjMtxIdx[dwCount]].mtx._41,
						g_lpMtx[g_lpdwSWObjMtxIdx[dwCount]].mtx._42,
						g_lpMtx[g_lpdwSWObjMtxIdx[dwCount]].mtx._43,
						g_lpvSWObjRad[dwCount].x,
						g_lpvSWObjRad[dwCount].y,
						g_lpvSWObjRad[dwCount].z);
				CreateScriptString(g_lpMsg);
			}
		}
	}

	// check dynamic world objects
	if(g_dwNumDWObj)
	{
		// output dynamic world objects
		CreateScriptString("<dynamic world objects>");

		// output each dynamic world object
		for(dwCount=0;dwCount<g_dwNumDWObj;dwCount++)
		{
			// get dynamic world object name
			LPSTR lpDWObjName=g_lplpDWObjNames[dwCount];

			// verify name
			if(lpDWObjName[0]!='~')
			{
				// output data
				sprintf(g_lpMsg,
						"dwobject %s (%f,%f,%f) (%f,%f,%f)",
						lpDWObjName,
						g_lpMtx[g_lpdwDWObjMtxIdx[dwCount]].mtx._41,
						g_lpMtx[g_lpdwDWObjMtxIdx[dwCount]].mtx._42,
						g_lpMtx[g_lpdwDWObjMtxIdx[dwCount]].mtx._43,
						g_lpvDWObjRad[dwCount].x,
						g_lpvDWObjRad[dwCount].y,
						g_lpvDWObjRad[dwCount].z);
				CreateScriptString(g_lpMsg);
			}
		}
	}

	// check lights
	if(g_dwNumLight)
	{
		// output lights
		CreateScriptString("<lights>");

		// output each light
		for(dwCount=0;dwCount<g_dwNumLight;dwCount++)
		{
			// get light name
			LPSTR lpName=g_lplpLightNames[dwCount];

			// verify name
			if(lpName[0]!='~')
			{
				// output data based on light type
				switch(g_lpLight[dwCount].Type)
				{
				case(D3DLIGHT_POINT):
					// output point light
					sprintf(g_lpMsg,
							"pointlight %s (%f,%f,%f,%f) (%f,%f,%f,%f) (%f,%f,%f) %f",
							lpName,
							g_lpLight[dwCount].Diffuse.r,
							g_lpLight[dwCount].Diffuse.g,
							g_lpLight[dwCount].Diffuse.b,
							g_lpLight[dwCount].Diffuse.a,
							g_lpLight[dwCount].Specular.r,
							g_lpLight[dwCount].Specular.g,
							g_lpLight[dwCount].Specular.b,
							g_lpLight[dwCount].Specular.a,
							g_lpLight[dwCount].Position.x,
							g_lpLight[dwCount].Position.y,
							g_lpLight[dwCount].Position.z,
							g_lpLight[dwCount].Range);
					CreateScriptString(g_lpMsg);

					// ok
					break;
				case(D3DLIGHT_SPOT):
					// output spot light
					sprintf(g_lpMsg,
							"spotlight  %s (%f,%f,%f,%f) (%f,%f,%f,%f) (%f,%f,%f) (%f,%f,%f) %f (%f,%f)",
							lpName,
							g_lpLight[dwCount].Diffuse.r,
							g_lpLight[dwCount].Diffuse.g,
							g_lpLight[dwCount].Diffuse.b,
							g_lpLight[dwCount].Diffuse.a,
							g_lpLight[dwCount].Specular.r,
							g_lpLight[dwCount].Specular.g,
							g_lpLight[dwCount].Specular.b,
							g_lpLight[dwCount].Specular.a,
							g_lpLight[dwCount].Position.x,
							g_lpLight[dwCount].Position.y,
							g_lpLight[dwCount].Position.z,
							g_lpLight[dwCount].Direction.x,
							g_lpLight[dwCount].Direction.y,
							g_lpLight[dwCount].Direction.z,
							g_lpLight[dwCount].Range,
							RadToDeg(g_lpLight[dwCount].Theta),
							RadToDeg(g_lpLight[dwCount].Phi));
					CreateScriptString(g_lpMsg);

					// ok
					break;
				case(D3DLIGHT_DIRECTIONAL):
					// output directional light
					sprintf(g_lpMsg,
							"dirlight   %s (%f,%f,%f,%f) (%f,%f,%f)",
							lpName,
							g_lpLight[dwCount].Diffuse.r,
							g_lpLight[dwCount].Diffuse.g,
							g_lpLight[dwCount].Diffuse.b,
							g_lpLight[dwCount].Diffuse.a,
							g_lpLight[dwCount].Direction.x,
							g_lpLight[dwCount].Direction.y,
							g_lpLight[dwCount].Direction.z);
					CreateScriptString(g_lpMsg);

					// ok
					break;
				}
			}
		}
	}
	
	// output end-of-file
	CreateScriptString("<end of file>");
	CreateScriptString("eof");

	// close script creator
	CloseScriptCreator();

	UnlockGlobalMsg()
	
	// ok
	return(TRUE);
}
#endif

// UnloadWorld()

void UnloadWorld(void)
{
	LoadingOutput("UnloadWorld");

	// remove all objects
	ResetGraphicsObjects();
	PurgeWorldObjects();
}

#ifdef DEBUG_MODE
// EditWorldScript()

void EditWorldScript(LPCSTR lpFile)
{
	// sw mesh indices
	DWORD dwSWMesh1;
	DWORD dwSWMesh2;
	DWORD dwSWMesh3;

	// offset vectors
	D3DXVECTOR3 vX;
	D3DXVECTOR3 vY;
	D3DXVECTOR3 vZ;

	// rotation values
	float fRotX;
	float fRotY;
	float fRotZ;

	// scaling values
	float fScale;

	// camera matrix
	static DWORD dwCamMtx;

	// current mode
	static WSEDITMODE wsmMode;

	// navigation increments
	static float fNavPos;
	static float fNavRot;
	static float fNavScale;

	// mini-font characters
	static DWORD dwMiniFont[256];

	// dialog box indices
	static DWORD dwDlgEnv;
	static DWORD dwDlgMat;
	static DWORD dwDlgTex;
	static DWORD dwDlgSky;
	static DWORD dwDlgLight;
	static DWORD dwDlgMesh;
	static DWORD dwDlgSWMesh;
	static DWORD dwDlgDWMesh;
	static DWORD dwDlgSWObj;
	static DWORD dwDlgDWObj;

	// check for initialization
	static BOOL bInit=FALSE;
	if(!bInit)
	{
		// enable mouse
		EnableMouseCursor();
		
		// output filename
		LPSTR lpOutFile;
		if(!AllocMem((LPVOID*)&lpOutFile,
					 strlen(lpFile)+1))
			return;					 
		
		// set output filename
		strcpy(lpOutFile,
			   lpFile);
		if(lpOutFile[strlen(lpOutFile)-1]=='s')
			lpOutFile[strlen(lpOutFile)-1]='c';
		else
			lpOutFile[strlen(lpOutFile)-1]=NULL;

		// create script if necessary
		if(GetFileAttributes(lpFile)==0xFFFFFFFF)
			CreateDefaultWorldScript(lpFile);
		
		// process script
		CompileWorldScript(lpOutFile,
						   lpFile,
						   TRUE);
		AddWorld(lpOutFile,TRUE);

		// release output file
		FreeMem((LPVOID*)&lpOutFile);

		// init camera matrix
		dwCamMtx=AddCameraMatrix(0,0,0,
								 0,0,1);

		// set mode
		wsmMode=WSM_IDLE;

		// set navigation increments
		fNavPos=0.05F;
		fNavRot=0.002F;
		fNavScale=0.002F;

		// initialize mini-font
		InitMiniFont(dwMiniFont);

		// initialize dialog boxes
		dwDlgEnv=InitDialogEnv();
		dwDlgMat=InitDialogMat();
		dwDlgTex=InitDialogTex();
		dwDlgSky=InitDialogSky();
		dwDlgLight=InitDialogLight();
		dwDlgMesh=InitDialogMesh();
		dwDlgSWMesh=InitDialogSWMesh();
		dwDlgDWMesh=InitDialogDWMesh();
		dwDlgSWObj=InitDialogSWObj();
		dwDlgDWObj=InitDialogDWObj();

		// set flag
		bInit=TRUE;
	}

	// validate last indices
	if(g_dwLastMat>g_dwNumMat)
		g_dwLastMat=0;
	if(g_dwLastTex>g_dwNumTex)
		g_dwLastTex=0;
	if(g_dwLastLight>g_dwNumLight)
		g_dwLastLight=0;
	if(g_dwLastMesh>g_dwNumMesh)
		g_dwLastMesh=0;
	if(g_dwLastSWMesh>g_dwNumSWMesh)
		g_dwLastSWMesh=0;
	if(g_dwLastDWMesh>g_dwNumDWMesh)
		g_dwLastDWMesh=0;
	if(g_dwLastSWMesh>g_dwNumSWObj)
		g_dwLastSWMesh=0;
	if(g_dwLastDWMesh>g_dwNumDWObj)
		g_dwLastDWMesh=0;

	// clear z-buffer
	ClearZBuffer();

	// set camera matrix
	SetCameraMatrix(dwCamMtx);

	// begin render
	if(BeginScene())
	{
		// render world
		RenderWorld(dwCamMtx);

		// render objects
		RenderStaticWorldObjects();
		RenderDynamicWorldObjects();

		// end scene
		EndScene();
	}

	// option blocks
	static WSOPTIONBLOCK obReload;
	static WSOPTIONBLOCK obSWObj;
	static WSOPTIONBLOCK obDWObj;
	static WSOPTIONBLOCK obSWMesh;
	static WSOPTIONBLOCK obDWMesh;
	static WSOPTIONBLOCK obMesh;
	static WSOPTIONBLOCK obLight;
	static WSOPTIONBLOCK obSky;
	static WSOPTIONBLOCK obTex;
	static WSOPTIONBLOCK obMat;
	static WSOPTIONBLOCK obEnv;
	static WSOPTIONBLOCK obSave;
	
	// init option blocks
	static BOOL bOBInit=FALSE;
	if(!bOBInit)
	{
		// set flag
		bOBInit=TRUE;

		// set blocks
		SET_WSOPTIONBLOCK(obReload,"Reload File", 568,468);
		SET_WSOPTIONBLOCK(obDWObj ,"Dynamic Obj", 3,  312);
		SET_WSOPTIONBLOCK(obSWObj ,"Static Obj",  3,  324);
		SET_WSOPTIONBLOCK(obDWMesh,"Dynamic Mesh",3,  348);
		SET_WSOPTIONBLOCK(obSWMesh,"Static Mesh", 3,  360);
		SET_WSOPTIONBLOCK(obMesh,  "Meshes",      3,  384);
		SET_WSOPTIONBLOCK(obLight, "Lights",      3,  396);
		SET_WSOPTIONBLOCK(obSky,   "Skybox",      3,  408);
		SET_WSOPTIONBLOCK(obTex,   "Textures",    3,  420);
		SET_WSOPTIONBLOCK(obMat,   "Materials",   3,  432);
		SET_WSOPTIONBLOCK(obEnv,   "Environment", 3,  444);
		SET_WSOPTIONBLOCK(obSave,  "Save File",   3,  468);
	}

	// set amplification factor
	float fAmp=1;
	if(g_bMouseBtnS)
		fAmp*=5;
	if(g_bMouseBtnC)
		fAmp*=10;
	
	// process input
	switch(wsmMode)
	{
	case(WSM_NAVWALK):
		// check mouse button
		if(!g_bMouseBtnL)
			wsmMode=WSM_IDLE;
		else
		{
			// update camera
			MatrixYaw(dwCamMtx,((float)g_nMouseDX)*fNavRot);
			MatrixBack(dwCamMtx,((float)g_nMouseDY)*fNavPos*fAmp);
		}
		
		// ok
		break;

	case(WSM_NAVLOOK):
		// check mouse button
		if(!g_bMouseBtnR)
			wsmMode=WSM_IDLE;
		else
		{
			// update camera
			MatrixYaw(dwCamMtx,((float)g_nMouseDX)*fNavRot);
			MatrixPitch(dwCamMtx,((float)g_nMouseDY)*fNavRot);
		}

		// ok
		break;

	case(WSM_IDLE):
		// render option blocks
		RenderOptionBlock(obReload);
		RenderOptionBlock(obDWObj);
		RenderOptionBlock(obSWObj);
		RenderOptionBlock(obDWMesh);
		RenderOptionBlock(obSWMesh);
		RenderOptionBlock(obMesh);
		RenderOptionBlock(obLight);
		RenderOptionBlock(obSky);
		RenderOptionBlock(obTex);
		RenderOptionBlock(obMat);
		RenderOptionBlock(obEnv);
		RenderOptionBlock(obSave);
		
		// macro to execute dialog box safely
		#define EXEC_DIALOG(a,b) { D3DXMATRIX mMtx; GetMatrix(dwCamMtx,&mMtx); wsmMode=(WSEDITMODE)ExecuteDialog(a,b); dwCamMtx=AddMatrix(&mMtx); InitMiniFont(dwMiniFont); }
		
		// check left mouse button
		if(g_bMouseClkL)
		{
			// check individual option blocks
			if(IsMouseInRect(obEnv.rRect))
				EXEC_DIALOG(dwDlgEnv,DialogEnvProc)
			else if(IsMouseInRect(obReload.rRect))
			{
				// verify action
				if(DisplayMessageBox(g_hWnd,
									 "Are you sure you want to reload this world?",
									 (LPSTR)ApplicationName(),
									 MB_YESNO|MB_ICONQUESTION)==IDYES)
				{
					// unload world
					UnloadWorld();

					// reload script
					bInit=FALSE;
				}
			}
			else if(IsMouseInRect(obSave.rRect))
			{
				// verify action
				if(DisplayMessageBox(g_hWnd,
									 "Are you sure you want to save current world?",
									 (LPSTR)ApplicationName(),
									 MB_YESNO|MB_ICONQUESTION)==IDYES)
					SaveWorld(lpFile);
			}
			else if(IsMouseInRect(obMat.rRect))
				EXEC_DIALOG(dwDlgMat,DialogMatProc)
			else if(IsMouseInRect(obTex.rRect))
				EXEC_DIALOG(dwDlgTex,DialogTexProc)
			else if(IsMouseInRect(obSky.rRect))
				EXEC_DIALOG(dwDlgSky,DialogSkyProc)
			else if(IsMouseInRect(obLight.rRect))
				EXEC_DIALOG(dwDlgLight,DialogLightProc)
			else if(IsMouseInRect(obMesh.rRect))
				EXEC_DIALOG(dwDlgMesh,DialogMeshProc)
			else if(IsMouseInRect(obSWMesh.rRect))
				EXEC_DIALOG(dwDlgSWMesh,DialogSWMeshProc)
			else if(IsMouseInRect(obDWMesh.rRect))
				EXEC_DIALOG(dwDlgDWMesh,DialogDWMeshProc)
			else if(IsMouseInRect(obSWObj.rRect))
				EXEC_DIALOG(dwDlgSWObj,DialogSWObjProc)
			else if(IsMouseInRect(obDWObj.rRect))
				EXEC_DIALOG(dwDlgDWObj,DialogDWObjProc)
		}
		else if(g_bMouseBtnL)
			wsmMode=WSM_NAVWALK;
		
		// check right mouse button
		if(g_bMouseBtnR)
			wsmMode=WSM_NAVLOOK;

		// ok
		break;

	// macro for getting light meshes
	#define GET_LIGHT_MESHES if(g_lpLight[g_dwCurLight].Type==D3DLIGHT_POINT) { dwSWMesh1=atoi(GetStringDivision(g_lplpLightNames[g_dwCurLight],1)); dwSWMesh2=atoi(GetStringDivision(g_lplpLightNames[g_dwCurLight],2)); } else if(g_lpLight[g_dwCurLight].Type==D3DLIGHT_SPOT) { dwSWMesh1=atoi(GetStringDivision(g_lplpLightNames[g_dwCurLight],1)); dwSWMesh2=atoi(GetStringDivision(g_lplpLightNames[g_dwCurLight],2)); dwSWMesh3=atoi(GetStringDivision(g_lplpLightNames[g_dwCurLight],3)); }

	// macros for computing offset vectors
	#define COMPUTE_OFFSET_VECTOR_X  { float fDX= (((float)g_nMouseDX)*fNavPos*fAmp); D3DXMATRIX mtx; GetMatrix(dwCamMtx,&mtx); vX.x=mtx._11; vX.y=0; vX.z=mtx._13; vX=Normalize(vX)*fDX; }
	#define COMPUTE_OFFSET_VECTOR_Y  { float fDY=-(((float)g_nMouseDY)*fNavPos*fAmp); D3DXMATRIX mtx; GetMatrix(dwCamMtx,&mtx); vY.x=0;       vY.y=1; vY.z=0;       vY*=fDY;              }
	#define COMPUTE_OFFSET_VECTOR_Z  { float fDZ=-(((float)g_nMouseDY)*fNavPos*fAmp); D3DXMATRIX mtx; GetMatrix(dwCamMtx,&mtx); vZ.x=mtx._31; vZ.y=0; vZ.z=mtx._33; vZ=Normalize(vZ)*fDZ; }
	#define COMPUTE_OFFSET_VECTOR_Zx { float fDZ= (((float)g_nMouseDX)*fNavPos*fAmp); D3DXMATRIX mtx; GetMatrix(dwCamMtx,&mtx); vZ.x=mtx._31; vZ.y=0; vZ.z=mtx._33; vZ=Normalize(vZ)*fDZ; }

	// macros for updating matrices
	#define OFFSET_MATRIX_X(idx) { g_lpMtx[idx].mtx._41+=vX.x; g_lpMtx[idx].mtx._42+=vX.y; g_lpMtx[idx].mtx._43+=vX.z; }
	#define OFFSET_MATRIX_Y(idx) { g_lpMtx[idx].mtx._41+=vY.x; g_lpMtx[idx].mtx._42+=vY.y; g_lpMtx[idx].mtx._43+=vY.z; }
	#define OFFSET_MATRIX_Z(idx) { g_lpMtx[idx].mtx._41+=vZ.x; g_lpMtx[idx].mtx._42+=vZ.y; g_lpMtx[idx].mtx._43+=vZ.z; }

	// macros for updating lights
	#define OFFSET_LIGHT_X(idx) { g_lpLight[idx].Position.x+=vX.x; g_lpLight[idx].Position.y+=vX.y; g_lpLight[idx].Position.z+=vX.z; }
	#define OFFSET_LIGHT_Y(idx) { g_lpLight[idx].Position.x+=vY.x; g_lpLight[idx].Position.y+=vY.y; g_lpLight[idx].Position.z+=vY.z; }
	#define OFFSET_LIGHT_Z(idx) { g_lpLight[idx].Position.x+=vZ.x; g_lpLight[idx].Position.y+=vZ.y; g_lpLight[idx].Position.z+=vZ.z; }		

	// macro to update world safely
	#define UPDATE_WORLD { D3DXMATRIX mMtx; GetMatrix(dwCamMtx,&mMtx); UpdateWorld(); dwCamMtx=AddMatrix(&mMtx); InitMiniFont(dwMiniFont); }

	case(WSM_LTPOSXZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// get light meshes
			GET_LIGHT_MESHES;

			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			COMPUTE_OFFSET_VECTOR_Z;
			
			// update meshes
			if(g_lpLight[g_dwCurLight].Type==D3DLIGHT_POINT)
			{
				// offset x
				OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh1]);
				OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh2]);
				
				// offset z
				OFFSET_MATRIX_Z(g_lpdwSWMMtxIndex[dwSWMesh1]);
				OFFSET_MATRIX_Z(g_lpdwSWMMtxIndex[dwSWMesh2]);

				// update light
				OFFSET_LIGHT_X(g_dwCurLight);
				OFFSET_LIGHT_Z(g_dwCurLight);
				UpdateLight(g_dwCurLight);
			}
			else if(g_lpLight[g_dwCurLight].Type==D3DLIGHT_SPOT)
			{
				// offset x
				OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh1]);
				OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh2]);
				OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh3]);
				
				// offset z
				OFFSET_MATRIX_Z(g_lpdwSWMMtxIndex[dwSWMesh1]);
				OFFSET_MATRIX_Z(g_lpdwSWMMtxIndex[dwSWMesh2]);
				OFFSET_MATRIX_Z(g_lpdwSWMMtxIndex[dwSWMesh3]);

				// update light
				OFFSET_LIGHT_X(g_dwCurLight);
				OFFSET_LIGHT_Z(g_dwCurLight);
				UpdateLight(g_dwCurLight);
			}
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;
	case(WSM_LTPOSXY):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// get light meshes
			GET_LIGHT_MESHES;

			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			COMPUTE_OFFSET_VECTOR_Y;
			
			// update meshes
			if(g_lpLight[g_dwCurLight].Type==D3DLIGHT_POINT)
			{
				// offset x
				OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh1]);
				OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh2]);
				
				// offset y
				OFFSET_MATRIX_Y(g_lpdwSWMMtxIndex[dwSWMesh1]);
				OFFSET_MATRIX_Y(g_lpdwSWMMtxIndex[dwSWMesh2]);

				// update light
				OFFSET_LIGHT_X(g_dwCurLight);
				OFFSET_LIGHT_Y(g_dwCurLight);
				UpdateLight(g_dwCurLight);
			}
			else if(g_lpLight[g_dwCurLight].Type==D3DLIGHT_SPOT)
			{
				// offset x
				OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh1]);
				OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh2]);
				OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh3]);
				
				// offset y
				OFFSET_MATRIX_Y(g_lpdwSWMMtxIndex[dwSWMesh1]);
				OFFSET_MATRIX_Y(g_lpdwSWMMtxIndex[dwSWMesh2]);
				OFFSET_MATRIX_Y(g_lpdwSWMMtxIndex[dwSWMesh3]);

				// update light
				OFFSET_LIGHT_X(g_dwCurLight);
				OFFSET_LIGHT_Y(g_dwCurLight);
				UpdateLight(g_dwCurLight);
			}
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	// macros for coputing offset rotations
	#define COMPUTE_OFFSET_ROTATION_X { fRotX=(((float)g_nMouseDX)*fAmp*fNavRot); }
	#define COMPUTE_OFFSET_ROTATION_Y { fRotY=(((float)g_nMouseDY)*fAmp*fNavRot); }
	#define COMPUTE_OFFSET_ROTATION_Z { fRotZ=(((float)g_nMouseDY)*fAmp*fNavRot); }

	// macros for rotating matrices
	#define ROTATE_MATRIX_X(idx) { MatrixYaw  (idx,fRotX); }
	#define ROTATE_MATRIX_Y(idx) { MatrixPitch(idx,fRotY); }
	#define ROTATE_MATRIX_Z(idx) { MatrixRoll (idx,fRotZ); }

	// macros to copy light orientation
	#define COPY_LIGHT_ROTATION(midx,lidx) { g_lpLight[lidx].Direction.x=g_lpMtx[midx].mtx._31; g_lpLight[lidx].Direction.y=g_lpMtx[midx].mtx._32; g_lpLight[lidx].Direction.z=g_lpMtx[midx].mtx._33; }
	
	case(WSM_LTDIR):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// get light meshes
			GET_LIGHT_MESHES;

			// compute offset rotations
			COMPUTE_OFFSET_ROTATION_X;
			COMPUTE_OFFSET_ROTATION_Y;
			
			// update meshes
			if(g_lpLight[g_dwCurLight].Type==D3DLIGHT_SPOT)
			{
				// offset x
				ROTATE_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh1]);
				ROTATE_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh2]);
				ROTATE_MATRIX_X(g_lpdwSWMMtxIndex[dwSWMesh3]);
				
				// offset z
				ROTATE_MATRIX_Y(g_lpdwSWMMtxIndex[dwSWMesh1]);
				ROTATE_MATRIX_Y(g_lpdwSWMMtxIndex[dwSWMesh2]);
				ROTATE_MATRIX_Y(g_lpdwSWMMtxIndex[dwSWMesh3]);

				// update light
				COPY_LIGHT_ROTATION(g_lpdwSWMMtxIndex[dwSWMesh1],g_dwCurLight);
				UpdateLight(g_dwCurLight);
			}
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMPOSX):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			
			// offset x
			OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMPOSY):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Y;
			
			// offset y
			OFFSET_MATRIX_Y(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMPOSZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Z;
			
			// offset z
			OFFSET_MATRIX_Z(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMPOSXY):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			COMPUTE_OFFSET_VECTOR_Y;
			
			// offset x-y
			OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
			OFFSET_MATRIX_Y(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMPOSXZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			COMPUTE_OFFSET_VECTOR_Z;
			
			// offset x-z
			OFFSET_MATRIX_X(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
			OFFSET_MATRIX_Z(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMPOSYZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Y;
			COMPUTE_OFFSET_VECTOR_Zx;
			
			// offset y-z
			OFFSET_MATRIX_Y(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
			OFFSET_MATRIX_Z(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMYAW):

		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset rotation
			COMPUTE_OFFSET_ROTATION_X;
			
			// rotate x
			ROTATE_MATRIX_X(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMPITCH):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset rotation
			COMPUTE_OFFSET_ROTATION_Y;
			
			// rotate y
			ROTATE_MATRIX_Y(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMROLL):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset rotation
			COMPUTE_OFFSET_ROTATION_Z;
			
			// rotate z
			ROTATE_MATRIX_Z(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	// macro for coputing offset scale
	#define COMPUTE_OFFSET_SCALE { fScale=(((float)(g_nMouseDX+g_nMouseDY))*fAmp*fNavScale); }

	// macros for scaling matrices
	#define SCALE_MATRIX_X(idx) { g_lpMtx[idx].fXScale+=fScale; }
	#define SCALE_MATRIX_Y(idx) { g_lpMtx[idx].fYScale+=fScale; }
	#define SCALE_MATRIX_Z(idx) { g_lpMtx[idx].fZScale+=fScale; }

	case(WSM_SWMSCALE):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute scaling offset value
			COMPUTE_OFFSET_SCALE;

			// scale x-y-z
			SCALE_MATRIX_X(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
			SCALE_MATRIX_Y(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
			SCALE_MATRIX_Z(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMSCALEX):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute scaling offset value
			COMPUTE_OFFSET_SCALE;

			// scale x
			SCALE_MATRIX_X(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMSCALEY):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute scaling offset value
			COMPUTE_OFFSET_SCALE;

			// scale y
			SCALE_MATRIX_Y(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWMSCALEZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute scaling offset value
			COMPUTE_OFFSET_SCALE;

			// scale z
			SCALE_MATRIX_Z(g_lpdwSWMMtxIndex[g_dwCurSWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMPOSX):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			
			// offset x
			OFFSET_MATRIX_X(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMPOSY):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Y;
			
			// offset y
			OFFSET_MATRIX_Y(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMPOSZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Z;
			
			// offset z
			OFFSET_MATRIX_Z(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMPOSXY):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			COMPUTE_OFFSET_VECTOR_Y;
			
			// offset x-y
			OFFSET_MATRIX_X(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
			OFFSET_MATRIX_Y(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMPOSXZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			COMPUTE_OFFSET_VECTOR_Z;
			
			// offset x-z
			OFFSET_MATRIX_X(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
			OFFSET_MATRIX_Z(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMPOSYZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Y;
			COMPUTE_OFFSET_VECTOR_Zx;
			
			// offset y-z
			OFFSET_MATRIX_Y(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
			OFFSET_MATRIX_Z(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMYAW):

		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset rotation
			COMPUTE_OFFSET_ROTATION_X;
			
			// rotate x
			ROTATE_MATRIX_X(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMPITCH):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset rotation
			COMPUTE_OFFSET_ROTATION_Y;
			
			// rotate y
			ROTATE_MATRIX_Y(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMROLL):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset rotation
			COMPUTE_OFFSET_ROTATION_Z;
			
			// rotate z
			ROTATE_MATRIX_Z(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMSCALE):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute scaling offset value
			COMPUTE_OFFSET_SCALE;

			// scale x-y-z
			SCALE_MATRIX_X(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
			SCALE_MATRIX_Y(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
			SCALE_MATRIX_Z(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMSCALEX):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute scaling offset value
			COMPUTE_OFFSET_SCALE;

			// scale x
			SCALE_MATRIX_X(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMSCALEY):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute scaling offset value
			COMPUTE_OFFSET_SCALE;

			// scale y
			SCALE_MATRIX_Y(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWMSCALEZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute scaling offset value
			COMPUTE_OFFSET_SCALE;

			// scale z
			SCALE_MATRIX_Z(g_lpdwDWMMtxIndex[g_dwCurDWMesh]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWOPOSX):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			
			// offset x
			OFFSET_MATRIX_X(g_lpdwSWObjMtxIdx[g_dwCurSWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWOPOSY):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Y;
			
			// offset y
			OFFSET_MATRIX_Y(g_lpdwSWObjMtxIdx[g_dwCurSWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWOPOSZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Z;
			
			// offset z
			OFFSET_MATRIX_Z(g_lpdwSWObjMtxIdx[g_dwCurSWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWOPOSXY):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			COMPUTE_OFFSET_VECTOR_Y;
			
			// offset x-y
			OFFSET_MATRIX_X(g_lpdwSWObjMtxIdx[g_dwCurSWObj]);
			OFFSET_MATRIX_Y(g_lpdwSWObjMtxIdx[g_dwCurSWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWOPOSXZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			COMPUTE_OFFSET_VECTOR_Z;
			
			// offset x-z
			OFFSET_MATRIX_X(g_lpdwSWObjMtxIdx[g_dwCurSWObj]);
			OFFSET_MATRIX_Z(g_lpdwSWObjMtxIdx[g_dwCurSWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_SWOPOSYZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Y;
			COMPUTE_OFFSET_VECTOR_Zx;
			
			// offset y-z
			OFFSET_MATRIX_Y(g_lpdwSWObjMtxIdx[g_dwCurSWObj]);
			OFFSET_MATRIX_Z(g_lpdwSWObjMtxIdx[g_dwCurSWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWOPOSX):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			
			// offset x
			OFFSET_MATRIX_X(g_lpdwDWObjMtxIdx[g_dwCurDWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWOPOSY):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Y;
			
			// offset y
			OFFSET_MATRIX_Y(g_lpdwDWObjMtxIdx[g_dwCurDWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWOPOSZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Z;
			
			// offset z
			OFFSET_MATRIX_Z(g_lpdwDWObjMtxIdx[g_dwCurDWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWOPOSXY):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			COMPUTE_OFFSET_VECTOR_Y;
			
			// offset x-y
			OFFSET_MATRIX_X(g_lpdwDWObjMtxIdx[g_dwCurDWObj]);
			OFFSET_MATRIX_Y(g_lpdwDWObjMtxIdx[g_dwCurDWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWOPOSXZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_X;
			COMPUTE_OFFSET_VECTOR_Z;
			
			// offset x-z
			OFFSET_MATRIX_X(g_lpdwDWObjMtxIdx[g_dwCurDWObj]);
			OFFSET_MATRIX_Z(g_lpdwDWObjMtxIdx[g_dwCurDWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;

	case(WSM_DWOPOSYZ):
		// check mouse button
		if(g_bMouseBtnL)
		{
			// compute offset vectors
			COMPUTE_OFFSET_VECTOR_Y;
			COMPUTE_OFFSET_VECTOR_Zx;
			
			// offset y-z
			OFFSET_MATRIX_Y(g_lpdwDWObjMtxIdx[g_dwCurDWObj]);
			OFFSET_MATRIX_Z(g_lpdwDWObjMtxIdx[g_dwCurDWObj]);
		}
		else if(g_bMouseClkR)
		{
			UPDATE_WORLD;
			wsmMode=WSM_IDLE;
		}

		// ok
		break;
	}

	// present scene
	PresentScene();

	// check for exit
	if(IsKeyDown(VK_ESCAPE))
	{
		// verify action
		int nVal=DisplayMessageBox(g_hWnd,
							       "Do you want to save before exiting?",
							       (LPSTR)ApplicationName(),
							       MB_YESNO|MB_ICONQUESTION);
		
		// check input
		if(nVal==IDYES)
			SaveWorld(lpFile);

		// close application
		SendCloseMessage();
	}
}

// InitDialogEnv()

DWORD InitDialogEnv(void)
{
	LoadingOutput("InitDialogEnv");

	// get dialog index
	DWORD dwIndex=AddDialog(120,292,
							"Environment Settings");

	// add controls
	AddDialogCheckBox(dwIndex,4, 4,112,8,"Alpha Blending",     CHK_ABLEND);
	AddDialogCheckBox(dwIndex,4,14,112,8,"Alpha Testing",      CHK_ATEST);
	AddDialogCheckBox(dwIndex,4,24,112,8,"Specular Highlights",CHK_SPECULAR);
	AddDialogCheckBox(dwIndex,4,34,112,8,"Fog",                CHK_FOG);

	AddDialogGroupBox(dwIndex,4,44,112,56,"Fog Parameters");
	AddDialogStaticControl(dwIndex,8, 54,50,8,"Start");
	AddDialogStaticControl(dwIndex,62,54,50,8,"End");
	AddDialogEditControl(dwIndex,8, 64,50,10,EDT_FSTART);
	AddDialogEditControl(dwIndex,62,64,50,10,EDT_FEND);
	AddDialogStaticControl(dwIndex,8, 76,32,8,"Red");
	AddDialogStaticControl(dwIndex,44,76,32,8,"Green");
	AddDialogStaticControl(dwIndex,80,76,32,8,"Blue");
	AddDialogEditControl(dwIndex,8, 86,32,10,EDT_FOGR);
	AddDialogEditControl(dwIndex,44,86,32,10,EDT_FOGG);
	AddDialogEditControl(dwIndex,80,86,32,10,EDT_FOGB);

	AddDialogGroupBox(dwIndex,4,102,112,34,"Ambient Light");
	AddDialogStaticControl(dwIndex,8, 112,32,8,"Red");
	AddDialogStaticControl(dwIndex,44,112,32,8,"Green");
	AddDialogStaticControl(dwIndex,80,112,32,8,"Blue");
	AddDialogEditControl(dwIndex,8, 122,32,10,EDT_AMBIENTR);
	AddDialogEditControl(dwIndex,44,122,32,10,EDT_AMBIENTG);
	AddDialogEditControl(dwIndex,80,122,32,10,EDT_AMBIENTB);

	AddDialogGroupBox(dwIndex,4,138,112,34,"Z-Clipping Planes");
	AddDialogStaticControl(dwIndex,8, 148,50,8,"Near");
	AddDialogStaticControl(dwIndex,62,148,50,8,"Far");
	AddDialogEditControl(dwIndex,8, 158,50,10,EDT_ZNEAR);
	AddDialogEditControl(dwIndex,62,158,50,10,EDT_ZFAR);

	AddDialogGroupBox(dwIndex,4,174,112,34,"Gravity");
	AddDialogStaticControl(dwIndex,8, 184,32,8,"X");
	AddDialogStaticControl(dwIndex,44,184,32,8,"Y");
	AddDialogStaticControl(dwIndex,80,184,32,8,"Z");
	AddDialogEditControl(dwIndex,8, 194,32,10,EDT_GRAVITYX);
	AddDialogEditControl(dwIndex,44,194,32,10,EDT_GRAVITYY);
	AddDialogEditControl(dwIndex,80,194,32,10,EDT_GRAVITYZ);

	AddDialogGroupBox(dwIndex,4,210,112,56,"Segments");
	AddDialogStaticControl(dwIndex,8, 220,50,8,"Min Size");
	AddDialogStaticControl(dwIndex,62,220,50,8,"Max X");
	AddDialogEditControl(dwIndex,8, 230,50,10,EDT_MINSIZE);
	AddDialogEditControl(dwIndex,62,230,50,10,EDT_MAXX);
	AddDialogStaticControl(dwIndex,8, 242,50,8,"Max Y");
	AddDialogStaticControl(dwIndex,62,242,50,8,"Max Z");
	AddDialogEditControl(dwIndex,8, 252,50,10,EDT_MAXY);
	AddDialogEditControl(dwIndex,62,252,50,10,EDT_MAXZ);

	AddDialogPushButton(dwIndex,4, 270,54,18,"OK",    BTN_OK,TRUE);
	AddDialogPushButton(dwIndex,62,270,54,18,"Cancel",BTN_CANCEL);

	// return index
	return(dwIndex);
}

// DialogEnvProc()

BOOL APIENTRY DialogEnvProc(HWND hDlg,
						    UINT Msg,
						    WPARAM wParam,
						    LPARAM lParam)
{
	// check message
	switch(Msg)
	{
	case(WM_INITDIALOG):
		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// init values
		SetCurrentDialogCheckBox(CHK_ABLEND,  g_bABlend);
		SetCurrentDialogCheckBox(CHK_ATEST,   g_bATest);
		SetCurrentDialogCheckBox(CHK_SPECULAR,g_bSpecular);
		SetCurrentDialogCheckBox(CHK_FOG,     g_bFog);

		SetCurrentDialogEditFloat(EDT_FSTART,g_fFogStart);
		SetCurrentDialogEditFloat(EDT_FEND,  g_fFogEnd);
	
		SetCurrentDialogEditFloat(EDT_FOGR,g_fFogR);
		SetCurrentDialogEditFloat(EDT_FOGG,g_fFogG);
		SetCurrentDialogEditFloat(EDT_FOGB,g_fFogB);
	
		SetCurrentDialogEditFloat(EDT_AMBIENTR,g_fAmbientR);
		SetCurrentDialogEditFloat(EDT_AMBIENTG,g_fAmbientG);
		SetCurrentDialogEditFloat(EDT_AMBIENTB,g_fAmbientB);
	
		SetCurrentDialogEditFloat(EDT_ZNEAR,g_fZClipNear);
		SetCurrentDialogEditFloat(EDT_ZFAR, g_fZClipFar);
		
		SetCurrentDialogEditFloat(EDT_GRAVITYX,g_vGravity.x);
		SetCurrentDialogEditFloat(EDT_GRAVITYY,g_vGravity.y);
		SetCurrentDialogEditFloat(EDT_GRAVITYZ,g_vGravity.z);

		SetCurrentDialogEditFloat(EDT_MINSIZE,g_fMinSTriSegSize);
		SetCurrentDialogEditLong(EDT_MAXX,g_dwMaxSTriSegX);
		SetCurrentDialogEditLong(EDT_MAXY,g_dwMaxSTriSegY);
		SetCurrentDialogEditLong(EDT_MAXZ,g_dwMaxSTriSegZ);

		// ok
		return(TRUE);
	case(WM_COMMAND):
		// check notification message
		switch(HIWORD(wParam))
		{
		case(BN_CLICKED):
			// check for ok button
			if(LOWORD(wParam)==BTN_OK)
			{
				// save values
				if(GetCurrentDialogCheckBox(CHK_ABLEND))
					EnableAlphaBlend();
				else
					DisableAlphaBlend();
				if(GetCurrentDialogCheckBox(CHK_ATEST))
					EnableAlphaTest();
				else
					DisableAlphaTest();
				if(GetCurrentDialogCheckBox(CHK_SPECULAR))
					EnableSpecular();
				else
					DisableSpecular();
				if(GetCurrentDialogCheckBox(CHK_FOG))
					EnableFog();
				else
					DisableFog();

				SetFogStart(GetCurrentDialogEditFloat(EDT_FSTART));
				SetFogEnd  (GetCurrentDialogEditFloat(EDT_FEND));
			
				g_fFogR=GetCurrentDialogEditFloat(EDT_FOGR);
				g_fFogG=GetCurrentDialogEditFloat(EDT_FOGG);
				g_fFogB=GetCurrentDialogEditFloat(EDT_FOGB);
				SetFogColor(D3DCOLOR_COLORVALUE(g_fFogR,
												g_fFogG,
												g_fFogB,
												1));
			
				g_fAmbientR=GetCurrentDialogEditFloat(EDT_AMBIENTR);
				g_fAmbientG=GetCurrentDialogEditFloat(EDT_AMBIENTG);
				g_fAmbientB=GetCurrentDialogEditFloat(EDT_AMBIENTB);
				SetAmbientLight(D3DCOLOR_COLORVALUE(g_fAmbientR,
													g_fAmbientG,
													g_fAmbientB,
													1));
			
				g_fZClipNear=GetCurrentDialogEditFloat(EDT_ZNEAR);
				g_fZClipFar= GetCurrentDialogEditFloat(EDT_ZFAR);
				SetProjectionMatrix(g_fZClipNear,
									g_fZClipFar);

				g_vGravity.x=GetCurrentDialogEditFloat(EDT_GRAVITYX);
				g_vGravity.y=GetCurrentDialogEditFloat(EDT_GRAVITYY);
				g_vGravity.z=GetCurrentDialogEditFloat(EDT_GRAVITYZ);

				g_fMinSTriSegSize=GetCurrentDialogEditFloat(EDT_MINSIZE);
				g_dwMaxSTriSegX=GetCurrentDialogEditLong(EDT_MAXX);
				g_dwMaxSTriSegY=GetCurrentDialogEditLong(EDT_MAXY);
				g_dwMaxSTriSegZ=GetCurrentDialogEditLong(EDT_MAXZ);

				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update environment
				UpdateWorld();
				
				// ok
				return(TRUE);
			}

			// check for cancel button
			if(LOWORD(wParam)==BTN_CANCEL)
			{
				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update world
				UpdateWorld();

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}
		
		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}

// InitDialogMat()

DWORD InitDialogMat(void)
{
	LoadingOutput("InitDialogMat");

	// get dialog index
	DWORD dwIndex=AddDialog(240,144,
							"Materials");

	// add controls
	AddDialogEditControl(dwIndex,4,5,72,10,EDT_NAME);
	AddDialogDropDownListBox(dwIndex,122,4,72,64,LST_NAME);
	AddDialogPushButton(dwIndex,80, 5,38,10,"Add",   BTN_ADD);
	AddDialogPushButton(dwIndex,198,5,38,10,"Delete",BTN_DELETE);

	AddDialogGroupBox(dwIndex,4,18,55,100,"Diffuse");
	AddDialogStaticControl(dwIndex,8,28,47,8,"Red");
	AddDialogStaticControl(dwIndex,8,50,47,8,"Green");
	AddDialogStaticControl(dwIndex,8,72,47,8,"Blue");
	AddDialogStaticControl(dwIndex,8,94,47,8,"Alpha");
	AddDialogEditControl(dwIndex,8,38, 47,10,EDT_DIFFUSER);
	AddDialogEditControl(dwIndex,8,60, 47,10,EDT_DIFFUSEG);
	AddDialogEditControl(dwIndex,8,82, 47,10,EDT_DIFFUSEB);
	AddDialogEditControl(dwIndex,8,104,47,10,EDT_DIFFUSEA);

	AddDialogGroupBox(dwIndex,63,18,55,100,"Ambient");
	AddDialogStaticControl(dwIndex,67,28,47,8,"Red");
	AddDialogStaticControl(dwIndex,67,50,47,8,"Green");
	AddDialogStaticControl(dwIndex,67,72,47,8,"Blue");
	AddDialogStaticControl(dwIndex,67,94,47,8,"Alpha");
	AddDialogEditControl(dwIndex,67,38, 47,10,EDT_AMBIENTR);
	AddDialogEditControl(dwIndex,67,60, 47,10,EDT_AMBIENTG);
	AddDialogEditControl(dwIndex,67,82, 47,10,EDT_AMBIENTB);
	AddDialogEditControl(dwIndex,67,104,47,10,EDT_AMBIENTA);

	AddDialogGroupBox(dwIndex,122,18,55,122,"Specular");
	AddDialogStaticControl(dwIndex,126,28,47,8,"Red");
	AddDialogStaticControl(dwIndex,126,50,47,8,"Green");
	AddDialogStaticControl(dwIndex,126,72,47,8,"Blue");
	AddDialogStaticControl(dwIndex,126,94,47,8,"Alpha");
	AddDialogStaticControl(dwIndex,126,116,47,8,"Power");
	AddDialogEditControl(dwIndex,126,38, 47,10,EDT_SPECULARR);
	AddDialogEditControl(dwIndex,126,60, 47,10,EDT_SPECULARG);
	AddDialogEditControl(dwIndex,126,82, 47,10,EDT_SPECULARB);
	AddDialogEditControl(dwIndex,126,104,47,10,EDT_SPECULARA);
	AddDialogEditControl(dwIndex,126,126,47,10,EDT_POWER);

	AddDialogGroupBox(dwIndex,181,18,55,100,"Emissive");
	AddDialogStaticControl(dwIndex,185,28,47,8,"Red");
	AddDialogStaticControl(dwIndex,185,50,47,8,"Green");
	AddDialogStaticControl(dwIndex,185,72,47,8,"Blue");
	AddDialogStaticControl(dwIndex,185,94,47,8,"Alpha");
	AddDialogEditControl(dwIndex,185,38, 47,10,EDT_EMISSIVER);
	AddDialogEditControl(dwIndex,185,60, 47,10,EDT_EMISSIVEG);
	AddDialogEditControl(dwIndex,185,82, 47,10,EDT_EMISSIVEB);
	AddDialogEditControl(dwIndex,185,104,47,10,EDT_EMISSIVEA);

	AddDialogPushButton(dwIndex,4,  122,55,18,"OK",   BTN_OK,TRUE);
	AddDialogPushButton(dwIndex,63, 122,55,18,"Canel",BTN_CANCEL);
	AddDialogPushButton(dwIndex,181,122,55,18,"Save", BTN_SAVE);

	// return index
	return(dwIndex);
}

// DialogMatProc()

BOOL APIENTRY DialogMatProc(HWND hDlg,
						    UINT Msg,
						    WPARAM wParam,
						    LPARAM lParam)
{
	// counter
	DWORD dwCount;

	// check message
	switch(Msg)
	{
	case(WM_INITDIALOG):
		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// init edit controls
		SetCurrentDialogEditText(EDT_NAME,"");
		SetCurrentDialogEditFloat(EDT_DIFFUSER,0);
		SetCurrentDialogEditFloat(EDT_DIFFUSEG,0);
		SetCurrentDialogEditFloat(EDT_DIFFUSEB,0);
		SetCurrentDialogEditFloat(EDT_DIFFUSEA,1);
		SetCurrentDialogEditFloat(EDT_AMBIENTR,0);
		SetCurrentDialogEditFloat(EDT_AMBIENTG,0);
		SetCurrentDialogEditFloat(EDT_AMBIENTB,0);
		SetCurrentDialogEditFloat(EDT_AMBIENTA,1);
		SetCurrentDialogEditFloat(EDT_SPECULARR,1);
		SetCurrentDialogEditFloat(EDT_SPECULARG,1);
		SetCurrentDialogEditFloat(EDT_SPECULARB,1);
		SetCurrentDialogEditFloat(EDT_SPECULARA,1);
		SetCurrentDialogEditFloat(EDT_EMISSIVER,0);
		SetCurrentDialogEditFloat(EDT_EMISSIVEG,0);
		SetCurrentDialogEditFloat(EDT_EMISSIVEB,0);
		SetCurrentDialogEditFloat(EDT_EMISSIVEA,1);
		SetCurrentDialogEditFloat(EDT_POWER,0);

		// reset list box
		ResetCurrentDialogDropDownSelections(LST_NAME);
		
		// init list box
		for(dwCount=0;dwCount<g_dwNumMat;dwCount++)
		{
			// cast pointer to material name
			LPSTR lpName=g_lplpMatNames[dwCount];
			
			// add material to list if necessary
			if(lpName[0]!='~')
				AddCurrentDialogDropDownSelection(LST_NAME,
												  lpName);
		}
		
		// highlight previous material
		if(g_dwLastMat<g_dwNumMat)
			HighlightCurrentDialogDropDownSelection(LST_NAME,
													g_lplpMatNames[g_dwLastMat]);
		
		// force info to load
		goto MatSelChange;

	case(WM_COMMAND):
		// check notification message
		switch(HIWORD(wParam))
		{
		case(BN_CLICKED):
			// check for ok button
			if(LOWORD(wParam)==BTN_OK)
			{
				// send save message
				SendMessage(hDlg,
							WM_COMMAND,
							(WPARAM)((((DWORD)BN_CLICKED)<<16)+(DWORD)BTN_SAVE),
							NULL);

				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update environment
				UpdateWorld();
				
				// ok
				return(TRUE);
			}

			// check for cancel button
			if(LOWORD(wParam)==BTN_CANCEL)
			{
				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update world
				UpdateWorld();

				// ok
				return(TRUE);
			}
			
			// check for add button
			if(LOWORD(wParam)==BTN_ADD)
			{
				LockGlobalMsg()

				// get name to add
				GetCurrentDialogEditText(EDT_NAME,
										 g_lpMsg,
										 _MAX_PATH);

				// check name
				if(strlen(g_lpMsg)==0)
					DisplayInfoMessage("You must specify a name to add.");
				else if(g_lpMsg[0]=='~')
					DisplayInfoMessage("Specified name is invalid.");
				else if(FindMaterial(g_lpMsg)!=NO_OBJECT)
					DisplayInfoMessage("Specified material already exists.");
				else
				{
					// allocate name buffer
					LPSTR lpName;
					if(!AllocMem((LPVOID*)&lpName,
								 strlen(g_lpMsg)+1))
					{
						UnlockGlobalMsg()
						return(TRUE);
					}

					// copy name
					strcpy(lpName,
						   g_lpMsg);

					// add material
					DWORD dwIndex=AddMaterial(GetCurrentDialogEditFloat(EDT_DIFFUSER),
											  GetCurrentDialogEditFloat(EDT_DIFFUSEG),
											  GetCurrentDialogEditFloat(EDT_DIFFUSEB),
											  GetCurrentDialogEditFloat(EDT_DIFFUSEA),
											  GetCurrentDialogEditFloat(EDT_AMBIENTR),
											  GetCurrentDialogEditFloat(EDT_AMBIENTG),
											  GetCurrentDialogEditFloat(EDT_AMBIENTB),
											  GetCurrentDialogEditFloat(EDT_AMBIENTA),
											  GetCurrentDialogEditFloat(EDT_SPECULARR),
											  GetCurrentDialogEditFloat(EDT_SPECULARG),
											  GetCurrentDialogEditFloat(EDT_SPECULARB),
											  GetCurrentDialogEditFloat(EDT_SPECULARA),
											  GetCurrentDialogEditFloat(EDT_EMISSIVER),
											  GetCurrentDialogEditFloat(EDT_EMISSIVEG),
											  GetCurrentDialogEditFloat(EDT_EMISSIVEB),
											  GetCurrentDialogEditFloat(EDT_EMISSIVEA),
											  GetCurrentDialogEditFloat(EDT_POWER),
											  lpName);

					// check material
					if(dwIndex<g_dwNumMat)
					{
						// add name to list
						AddCurrentDialogDropDownSelection(LST_NAME,
														  lpName);

						// highlight name
						HighlightCurrentDialogDropDownSelection(LST_NAME,
																lpName);
					}
							
					// free name buffer
					FreeMem((LPVOID*)&lpName);

					// remove name
					SetCurrentDialogEditText(EDT_NAME,
											 "");
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for delete button
			if(LOWORD(wParam)==BTN_DELETE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find material
					DWORD dwIndex=FindMaterial(g_lpMsg);

					// check material
					if(dwIndex<g_dwNumMat)
					{
						// eliminate material
						LPSTR lpName=g_lplpMatNames[dwIndex];
						lpName[0]='~';

						// delete name from list
						RemoveCurrentDialogDropDownSelection(LST_NAME);
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for save button
			if(LOWORD(wParam)==BTN_SAVE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find material
					DWORD dwIndex=FindMaterial(g_lpMsg);

					// check material
					if(dwIndex<g_dwNumMat)
					{
						// update material
						g_lpMat[dwIndex].Diffuse.r=GetCurrentDialogEditFloat(EDT_DIFFUSER);
						g_lpMat[dwIndex].Diffuse.g=GetCurrentDialogEditFloat(EDT_DIFFUSEG);
						g_lpMat[dwIndex].Diffuse.b=GetCurrentDialogEditFloat(EDT_DIFFUSEB);
						g_lpMat[dwIndex].Diffuse.a=GetCurrentDialogEditFloat(EDT_DIFFUSEA);
						g_lpMat[dwIndex].Ambient.r=GetCurrentDialogEditFloat(EDT_AMBIENTR);
						g_lpMat[dwIndex].Ambient.g=GetCurrentDialogEditFloat(EDT_AMBIENTG);
						g_lpMat[dwIndex].Ambient.b=GetCurrentDialogEditFloat(EDT_AMBIENTB);
						g_lpMat[dwIndex].Ambient.a=GetCurrentDialogEditFloat(EDT_AMBIENTA);
						g_lpMat[dwIndex].Specular.r=GetCurrentDialogEditFloat(EDT_SPECULARR);
						g_lpMat[dwIndex].Specular.g=GetCurrentDialogEditFloat(EDT_SPECULARG);
						g_lpMat[dwIndex].Specular.b=GetCurrentDialogEditFloat(EDT_SPECULARB);
						g_lpMat[dwIndex].Specular.a=GetCurrentDialogEditFloat(EDT_SPECULARA);
						g_lpMat[dwIndex].Emissive.r=GetCurrentDialogEditFloat(EDT_EMISSIVER);
						g_lpMat[dwIndex].Emissive.g=GetCurrentDialogEditFloat(EDT_EMISSIVEG);
						g_lpMat[dwIndex].Emissive.b=GetCurrentDialogEditFloat(EDT_EMISSIVEB);
						g_lpMat[dwIndex].Emissive.a=GetCurrentDialogEditFloat(EDT_EMISSIVEA);
						g_lpMat[dwIndex].Power=GetCurrentDialogEditFloat(EDT_POWER);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			// ok
			break;

		case(CBN_SELCHANGE):
			// check identifier
			if(LOWORD(wParam)==LST_NAME)
			{
				MatSelChange:;

				LockGlobalMsg()

				// get selection text
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// replace index with material index
					DWORD dwIndex=FindMaterial(g_lpMsg);

					// check material
					if(dwIndex<g_dwNumMat)
					{
						// set last material
						g_dwLastMat=dwIndex;
						
						// fill in values
						SetCurrentDialogEditFloat(EDT_DIFFUSER,g_lpMat[dwIndex].Diffuse.r);
						SetCurrentDialogEditFloat(EDT_DIFFUSEG,g_lpMat[dwIndex].Diffuse.g);
						SetCurrentDialogEditFloat(EDT_DIFFUSEB,g_lpMat[dwIndex].Diffuse.b);
						SetCurrentDialogEditFloat(EDT_DIFFUSEA,g_lpMat[dwIndex].Diffuse.a);
						SetCurrentDialogEditFloat(EDT_AMBIENTR,g_lpMat[dwIndex].Ambient.r);
						SetCurrentDialogEditFloat(EDT_AMBIENTG,g_lpMat[dwIndex].Ambient.g);
						SetCurrentDialogEditFloat(EDT_AMBIENTB,g_lpMat[dwIndex].Ambient.b);
						SetCurrentDialogEditFloat(EDT_AMBIENTA,g_lpMat[dwIndex].Ambient.a);
						SetCurrentDialogEditFloat(EDT_SPECULARR,g_lpMat[dwIndex].Specular.r);
						SetCurrentDialogEditFloat(EDT_SPECULARG,g_lpMat[dwIndex].Specular.g);
						SetCurrentDialogEditFloat(EDT_SPECULARB,g_lpMat[dwIndex].Specular.b);
						SetCurrentDialogEditFloat(EDT_SPECULARA,g_lpMat[dwIndex].Specular.a);
						SetCurrentDialogEditFloat(EDT_EMISSIVER,g_lpMat[dwIndex].Emissive.r);
						SetCurrentDialogEditFloat(EDT_EMISSIVEG,g_lpMat[dwIndex].Emissive.g);
						SetCurrentDialogEditFloat(EDT_EMISSIVEB,g_lpMat[dwIndex].Emissive.b);
						SetCurrentDialogEditFloat(EDT_EMISSIVEA,g_lpMat[dwIndex].Emissive.a);
						SetCurrentDialogEditFloat(EDT_POWER,g_lpMat[dwIndex].Power);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}
		
		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}

// InitDialogTex()

DWORD InitDialogTex(void)
{
	LoadingOutput("InitDialogTex");

	// get dialog index
	DWORD dwIndex=AddDialog(120,82,
							"Textures");

	// add controls
	AddDialogEditControl(dwIndex,4,4,72,10,EDT_NAME);
	AddDialogDropDownListBox(dwIndex,4,18,72,64,LST_NAME);
	AddDialogPushButton(dwIndex,80,4, 38,10,"Add",   BTN_ADD);
	AddDialogPushButton(dwIndex,80,19,38,10,"Delete",BTN_DELETE);

	AddDialogGroupBox(dwIndex,4,32,112,24,"Filename");
	AddDialogEditControl(dwIndex,8,42, 68,10,EDT_FILE);
	AddDialogPushButton(dwIndex,80,42,32,10,"Save",BTN_SAVE);

	AddDialogPushButton(dwIndex,4, 60,54,18,"OK",   BTN_OK,TRUE);
	AddDialogPushButton(dwIndex,62,60,54,18,"Canel",BTN_CANCEL);

	// return index
	return(dwIndex);
}

// DialogTexProc()

BOOL APIENTRY DialogTexProc(HWND hDlg,
						    UINT Msg,
						    WPARAM wParam,
						    LPARAM lParam)
{
	// counter
	DWORD dwCount;

	// check message
	switch(Msg)
	{
	case(WM_INITDIALOG):
		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// init edit controls
		SetCurrentDialogEditText(EDT_NAME,"");
		SetCurrentDialogEditText(EDT_FILE,"");

		// reset list box
		ResetCurrentDialogDropDownSelections(LST_NAME);
		
		// init list box
		for(dwCount=0;dwCount<g_dwNumTex;dwCount++)
		{
			// cast pointer to texture name
			LPSTR lpName=g_lplpTexNames[dwCount];
			
			// add texture to list if necessary
			if(lpName[0]!='~')
				AddCurrentDialogDropDownSelection(LST_NAME,
												  lpName);
		}

		// highlight previous texture
		if(g_dwLastTex<g_dwNumTex)
			HighlightCurrentDialogDropDownSelection(LST_NAME,
													g_lplpTexNames[g_dwLastTex]);
		
		// force info to load
		goto TexSelChange;

	case(WM_COMMAND):
		// check notification message
		switch(HIWORD(wParam))
		{
		case(BN_CLICKED):
			// check for ok button
			if(LOWORD(wParam)==BTN_OK)
			{
				// send save message
				SendMessage(hDlg,
							WM_COMMAND,
							(WPARAM)((((DWORD)BN_CLICKED)<<16)+(DWORD)BTN_SAVE),
							NULL);

				// end dialog box
				EndDialog(hDlg,WSM_IDLE);
				
				// update world
				UpdateWorld();

				// ok
				return(TRUE);
			}

			// check for cancel button
			if(LOWORD(wParam)==BTN_CANCEL)
			{
				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update world
				UpdateWorld();

				// ok
				return(TRUE);
			}
			
			// check for add button
			if(LOWORD(wParam)==BTN_ADD)
			{
				LockGlobalMsg()

				// get name to add
				GetCurrentDialogEditText(EDT_NAME,
										 g_lpMsg,
										 _MAX_PATH);

				// check name
				if(strlen(g_lpMsg)==0)
					DisplayInfoMessage("You must specify a name to add.");
				else if(g_lpMsg[0]=='~')
					DisplayInfoMessage("Specified name is invalid.");
				else if(FindTexture(g_lpMsg)!=NO_OBJECT)
					DisplayInfoMessage("Specified texture already exists.");
				else
				{
					// allocate name buffer
					LPSTR lpName;
					if(!AllocMem((LPVOID*)&lpName,
								 strlen(g_lpMsg)+1))
					{
						UnlockGlobalMsg()
						return(TRUE);
					}

					// copy name
					strcpy(lpName,
						   g_lpMsg);

					// get filename
					GetCurrentDialogEditText(EDT_FILE,
											 g_lpMsg,
											 _MAX_PATH);

					// allocate filename buffer
					LPSTR lpFile;
					if(!AllocMem((LPVOID*)&lpFile,
								 strlen(g_lpMsg)+1))
					{
						UnlockGlobalMsg()
						return(TRUE);
					}

					// copy name
					strcpy(lpFile,
						   g_lpMsg);

					// set texture w/ filename
					sprintf(g_lpMsg,
							"%s&%s",
							lpName,
							lpFile);

					// add texture
					DWORD dwIndex=AddTexture(lpFile,
											 g_lpMsg);

					// check texture
					if(dwIndex<g_dwNumTex)
					{
						// separate texture & filename
						DivideString(g_lplpTexNames[dwIndex]);

						// add name to list
						AddCurrentDialogDropDownSelection(LST_NAME,
														  lpName);

						// highlight name
						HighlightCurrentDialogDropDownSelection(LST_NAME,
																lpName);
					}
							
					// free name buffer
					FreeMem((LPVOID*)&lpName);

					// free filename buffer
					FreeMem((LPVOID*)&lpFile);

					// remove name
					SetCurrentDialogEditText(EDT_NAME,
											 "");
				}
				
				UnlockGlobalMsg()
					
				// ok
				return(TRUE);
			}
			
			// check for delete button
			if(LOWORD(wParam)==BTN_DELETE)
			{
				LockGlobalMsg()
				
				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find texture
					DWORD dwIndex=FindTexture(g_lpMsg);

					// check texture
					if(dwIndex<g_dwNumTex)
					{
						// eliminate texture
						LPSTR lpName=g_lplpTexNames[dwIndex];
						lpName[0]='~';

						// delete name from list
						RemoveCurrentDialogDropDownSelection(LST_NAME);
					}
				}
				
				UnlockGlobalMsg()
					
				// ok
				return(TRUE);
			}
			
			// check for save button
			if(LOWORD(wParam)==BTN_SAVE)
			{
				LockGlobalMsg()
					
				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find texture
					DWORD dwIndex=FindTexture(g_lpMsg);

					// check texture
					if(dwIndex<g_dwNumTex)
					{
						// allocate name buffer
						LPSTR lpName;
						if(!AllocMem((LPVOID*)&lpName,
									 strlen(g_lpMsg)+1))
						{
							UnlockGlobalMsg()
							return(TRUE);
						}

						// copy name
						strcpy(lpName,
							   g_lpMsg);

						// get filename
						GetCurrentDialogEditText(EDT_FILE,
												 g_lpMsg,
												 _MAX_PATH);

						// allocate filename buffer
						LPSTR lpFile;
						if(!AllocMem((LPVOID*)&lpFile,
									 strlen(g_lpMsg)+1))
						{
							UnlockGlobalMsg()
							return(TRUE);
						}

						// copy name
						strcpy(lpFile,
							   g_lpMsg);

						// set texture w/ filename
						sprintf(g_lpMsg,
								"%s&%s",
								lpName,
								lpFile);

						// release old texture name
						FreeMem((LPVOID*)&g_lplpTexNames[dwIndex]);

						// allocate new texture name
						if(!AllocMem((LPVOID*)&g_lplpTexNames[dwIndex],
									 strlen(g_lpMsg)+1))
						{
							UnlockGlobalMsg()
							return(TRUE);
						}

						// copy name
						strcpy(g_lplpTexNames[dwIndex],
							   g_lpMsg);

						// separate texture & filename
						DivideString(g_lplpTexNames[dwIndex]);
						
						// free name buffer
						FreeMem((LPVOID*)&lpName);

						// free filename buffer
						FreeMem((LPVOID*)&lpFile);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			// ok
			break;

		case(CBN_SELCHANGE):
			// check identifier
			if(LOWORD(wParam)==LST_NAME)
			{
				TexSelChange:;

				LockGlobalMsg()

				// get selection text
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// replace index with texture index
					DWORD dwIndex=FindTexture(g_lpMsg);

					// check texture
					if(dwIndex<g_dwNumTex)
					{
						// set last texture
						g_dwLastTex=dwIndex;
						
						// fill in values
						SetCurrentDialogEditText(EDT_FILE,GetStringDivision(g_lplpTexNames[dwIndex],1));
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}
		
		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}

// InitDialogSky()

DWORD InitDialogSky(void)
{
	LoadingOutput("InitDialogSky");

	// get dialog index
	DWORD dwIndex=AddDialog(240,144,
							"Skybox Settings");

	// add controls
	AddDialogGroupBox(dwIndex,4,4,75,36,"Front");
	AddDialogCheckBox(dwIndex,8,14,67,8,"Use Texture",CHK_FRONT);
	AddDialogDropDownListBox(dwIndex,8,24,67,64,LST_FRONT);

	AddDialogGroupBox(dwIndex,4,42,75,36,"Back");
	AddDialogCheckBox(dwIndex,8,52,67,8,"Use Texture",CHK_BACK);
	AddDialogDropDownListBox(dwIndex,8,62,67,64,LST_BACK);

	AddDialogGroupBox(dwIndex,83,4,74,36,"Left");
	AddDialogCheckBox(dwIndex,87,14,67,8,"Use Texture",CHK_LEFT);
	AddDialogDropDownListBox(dwIndex,87,24,67,64,LST_LEFT);

	AddDialogGroupBox(dwIndex,83,42,74,36,"Right");
	AddDialogCheckBox(dwIndex,87,52,67,8,"Use Texture",CHK_RIGHT);
	AddDialogDropDownListBox(dwIndex,87,62,67,64,LST_RIGHT);

	AddDialogGroupBox(dwIndex,161,4,75,36,"Top");
	AddDialogCheckBox(dwIndex,165,14,67,8,"Use Texture",CHK_TOP);
	AddDialogDropDownListBox(dwIndex,165,24,67,64,LST_TOP);

	AddDialogGroupBox(dwIndex,161,42,75,36,"Bottom");
	AddDialogCheckBox(dwIndex,165,52,67,8,"Use Texture",CHK_BOTTOM);
	AddDialogDropDownListBox(dwIndex,165,62,67,64,LST_BOTTOM);

	AddDialogGroupBox(dwIndex,4,80,232,34,"Offset");
	AddDialogStaticControl(dwIndex,8,  90,72,8,"X");
	AddDialogStaticControl(dwIndex,84, 90,72,8,"Y");
	AddDialogStaticControl(dwIndex,160,90,72,8,"Z");
	AddDialogEditControl(dwIndex,8,  100,72,10,EDT_X);
	AddDialogEditControl(dwIndex,84, 100,72,10,EDT_Y);
	AddDialogEditControl(dwIndex,160,100,72,10,EDT_Z);

	AddDialogGroupBox(dwIndex,4,116,75,24,"Radius");
	AddDialogEditControl(dwIndex,8,126,67,10,EDT_RAD);
	
	AddDialogPushButton(dwIndex,83, 120,74,20,"OK",    BTN_OK,TRUE);
	AddDialogPushButton(dwIndex,161,120,75,20,"Cancel",BTN_CANCEL);

	// return index
	return(dwIndex);
}

// DialogSkyProc()

BOOL APIENTRY DialogSkyProc(HWND hDlg,
						    UINT Msg,
						    WPARAM wParam,
						    LPARAM lParam)
{
	// counter
	DWORD dwCount;
	
	// check message
	switch(Msg)
	{
	case(WM_INITDIALOG):
		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// reset list boxes
		ResetCurrentDialogDropDownSelections(LST_FRONT);
		ResetCurrentDialogDropDownSelections(LST_BACK);
		ResetCurrentDialogDropDownSelections(LST_LEFT);
		ResetCurrentDialogDropDownSelections(LST_RIGHT);
		ResetCurrentDialogDropDownSelections(LST_TOP);
		ResetCurrentDialogDropDownSelections(LST_BOTTOM);
		
		// init list boxes
		for(dwCount=0;dwCount<g_dwNumTex;dwCount++)
		{
			// cast pointer to texture name
			LPSTR lpName=g_lplpTexNames[dwCount];
			
			// add texture to list if necessary
			if(lpName[0]!='~')
			{
				AddCurrentDialogDropDownSelection(LST_FRONT,lpName);
				AddCurrentDialogDropDownSelection(LST_BACK,lpName);
				AddCurrentDialogDropDownSelection(LST_LEFT,lpName);
				AddCurrentDialogDropDownSelection(LST_RIGHT,lpName);
				AddCurrentDialogDropDownSelection(LST_TOP,lpName);
				AddCurrentDialogDropDownSelection(LST_BOTTOM,lpName);
			}
		}
		
		// macro for highlighting textures
		#define SKYBOX_HIGHLIGHT(sb,cid,lid) if(sb.dwTexIndex!=g_dwSkyboxTex&&sb.dwTexIndex!=NO_OBJECT) { SetCurrentDialogCheckBox(cid,TRUE); HighlightCurrentDialogDropDownSelection(lid,g_lplpTexNames[sb.dwTexIndex]); } else SetCurrentDialogCheckBox(cid,FALSE);		

		// highlight textures
		SKYBOX_HIGHLIGHT(g_sbFront, CHK_FRONT, LST_FRONT);
		SKYBOX_HIGHLIGHT(g_sbBack,  CHK_BACK,  LST_BACK);
		SKYBOX_HIGHLIGHT(g_sbLeft,  CHK_LEFT,  LST_LEFT);
		SKYBOX_HIGHLIGHT(g_sbRight, CHK_RIGHT, LST_RIGHT);
		SKYBOX_HIGHLIGHT(g_sbTop,   CHK_TOP,   LST_TOP);
		SKYBOX_HIGHLIGHT(g_sbBottom,CHK_BOTTOM,LST_BOTTOM);

		// init edit controls
		SetCurrentDialogEditFloat(EDT_X,  g_vSkyboxOffset.x);
		SetCurrentDialogEditFloat(EDT_Y,  g_vSkyboxOffset.y);
		SetCurrentDialogEditFloat(EDT_Z,  g_vSkyboxOffset.z);
		SetCurrentDialogEditFloat(EDT_RAD,g_fSkyboxRadius);
		
		// ok
		return(TRUE);
	case(WM_COMMAND):
		// check notification message
		switch(HIWORD(wParam))
		{
		case(BN_CLICKED):
			// check for ok button
			if(LOWORD(wParam)==BTN_OK)
			{
				// macro for saving skybox values
				#define SKYBOX_SAVE(sb,cid,lid) if(GetCurrentDialogCheckBox(cid)) { LockGlobalMsg() if(!GetCurrentDialogDropDownSelection(lid,g_lpMsg)) { DisplayInfoMessage("You must select a texture."); UnlockGlobalMsg() return(TRUE); } sb.dwTexIndex=FindTexture(g_lpMsg); UnlockGlobalMsg() } else sb.dwTexIndex=g_dwSkyboxTex;
				
				// save skybox values
				SKYBOX_SAVE(g_sbFront, CHK_FRONT, LST_FRONT);
				SKYBOX_SAVE(g_sbBack,  CHK_BACK,  LST_BACK);
				SKYBOX_SAVE(g_sbLeft,  CHK_LEFT,  LST_LEFT);
				SKYBOX_SAVE(g_sbRight, CHK_RIGHT, LST_RIGHT);
				SKYBOX_SAVE(g_sbTop,   CHK_TOP,   LST_TOP);
				SKYBOX_SAVE(g_sbBottom,CHK_BOTTOM,LST_BOTTOM);

				// save edit controls
				g_vSkyboxOffset.x=GetCurrentDialogEditFloat(EDT_X);
				g_vSkyboxOffset.y=GetCurrentDialogEditFloat(EDT_Y);
				g_vSkyboxOffset.z=GetCurrentDialogEditFloat(EDT_Z);
				g_fSkyboxRadius=  GetCurrentDialogEditFloat(EDT_RAD);

				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update environment
				UpdateWorld();
				
				// ok
				return(TRUE);
			}

			// check for cancel button
			if(LOWORD(wParam)==BTN_CANCEL)
			{
				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update world
				UpdateWorld();

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}
		
		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}

// InitDialogLight()

DWORD InitDialogLight(void)
{
	LoadingOutput("InitDialogLight");

	// get dialog index
	DWORD dwIndex=AddDialog(240,184,
							"Lights");

	// add controls
	AddDialogEditControl(dwIndex,4,5,72,10,EDT_NAME);
	AddDialogDropDownListBox(dwIndex,122,4,72,64,LST_NAME);
	AddDialogPushButton(dwIndex,80, 5,38,10,"Add",   BTN_ADD);
	AddDialogPushButton(dwIndex,198,5,38,10,"Delete",BTN_DELETE);

	AddDialogGroupBox(dwIndex,4,18,55,42,"Type");
	AddDialogRadioButton(dwIndex,8,28,47,8,"Point",      RAD_POINT);
	AddDialogRadioButton(dwIndex,8,38,47,8,"Spot",       RAD_SPOT);
	AddDialogRadioButton(dwIndex,8,48,47,8,"Directional",RAD_DIR);

	AddDialogGroupBox(dwIndex,4,62,55,56,"Cone");
	AddDialogStaticControl(dwIndex,8,72,47,8,"Inner");
	AddDialogStaticControl(dwIndex,8,94,47,8,"Outer");
	AddDialogEditControl(dwIndex,8,82, 47,10,EDT_INNERCONE);
	AddDialogEditControl(dwIndex,8,104,47,10,EDT_OUTERCONE);

	AddDialogGroupBox(dwIndex,63,18,55,100,"Position");
	AddDialogStaticControl(dwIndex,67,28,47,8,"X");
	AddDialogStaticControl(dwIndex,67,50,47,8,"Y");
	AddDialogStaticControl(dwIndex,67,72,47,8,"Z");
	AddDialogEditControl(dwIndex,67,38, 47,10,EDT_POSX);
	AddDialogEditControl(dwIndex,67,60, 47,10,EDT_POSY);
	AddDialogEditControl(dwIndex,67,82, 47,10,EDT_POSZ);
	AddDialogPushButton(dwIndex,67,96,21,18,"X-Y",BTN_POSXY);
	AddDialogPushButton(dwIndex,92,96,22,18,"X-Z",BTN_POSXZ);

	AddDialogGroupBox(dwIndex,122,18,55,100,"Diffuse");
	AddDialogStaticControl(dwIndex,126,28,47,8,"Red");
	AddDialogStaticControl(dwIndex,126,50,47,8,"Green");
	AddDialogStaticControl(dwIndex,126,72,47,8,"Blue");
	AddDialogStaticControl(dwIndex,126,94,47,8,"Alpha");
	AddDialogEditControl(dwIndex,126,38, 47,10,EDT_DIFFUSER);
	AddDialogEditControl(dwIndex,126,60, 47,10,EDT_DIFFUSEG);
	AddDialogEditControl(dwIndex,126,82, 47,10,EDT_DIFFUSEB);
	AddDialogEditControl(dwIndex,126,104,47,10,EDT_DIFFUSEA);

	AddDialogGroupBox(dwIndex,181,18,55,100,"Specular");
	AddDialogStaticControl(dwIndex,185,28,47,8,"Red");
	AddDialogStaticControl(dwIndex,185,50,47,8,"Green");
	AddDialogStaticControl(dwIndex,185,72,47,8,"Blue");
	AddDialogStaticControl(dwIndex,185,94,47,8,"Alpha");
	AddDialogEditControl(dwIndex,185,38, 47,10,EDT_SPECULARR);
	AddDialogEditControl(dwIndex,185,60, 47,10,EDT_SPECULARG);
	AddDialogEditControl(dwIndex,185,82, 47,10,EDT_SPECULARB);
	AddDialogEditControl(dwIndex,185,104,47,10,EDT_SPECULARA);

	AddDialogGroupBox(dwIndex,4,120,232,34,"Direction");
	AddDialogStaticControl(dwIndex,8,  130,53,8,"X");
	AddDialogStaticControl(dwIndex,65, 130,53,8,"Y");
	AddDialogStaticControl(dwIndex,122,130,53,8,"Z");
	AddDialogEditControl(dwIndex,8,  140,53,10,EDT_DIRX);
	AddDialogEditControl(dwIndex,65, 140,53,10,EDT_DIRY);
	AddDialogEditControl(dwIndex,122,140,53,10,EDT_DIRZ);
	AddDialogPushButton(dwIndex,179,128,53,22,"Edit",BTN_DIR);

	AddDialogGroupBox(dwIndex,122,156,55,24,"Range");
	AddDialogEditControl(dwIndex,126,166,47,10,EDT_RANGE);

	AddDialogPushButton(dwIndex,4,  160,55,20,"OK",   BTN_OK,TRUE);
	AddDialogPushButton(dwIndex,63, 160,55,20,"Canel",BTN_CANCEL);
	AddDialogPushButton(dwIndex,181,160,55,20,"Save", BTN_SAVE);

	// return index
	return(dwIndex);
}

// DialogLightProc()

BOOL APIENTRY DialogLightProc(HWND hDlg,
						      UINT Msg,
						      WPARAM wParam,
						      LPARAM lParam)
{
	// counter
	DWORD dwCount;

	// check message
	switch(Msg)
	{
	case(WM_INITDIALOG):
		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// reset radio buttons
		SetCurrentDialogCheckBox(RAD_SPOT,FALSE);
		SetCurrentDialogCheckBox(RAD_POINT,FALSE);
		SetCurrentDialogCheckBox(RAD_DIR,FALSE);

		// init controls
		SetCurrentDialogCheckBox(RAD_POINT,TRUE);
		SetCurrentDialogEditText(EDT_NAME,"");
		SetCurrentDialogEditFloat(EDT_INNERCONE,30);
		SetCurrentDialogEditFloat(EDT_OUTERCONE,60);
		SetCurrentDialogEditFloat(EDT_POSX,0);
		SetCurrentDialogEditFloat(EDT_POSY,0);
		SetCurrentDialogEditFloat(EDT_POSZ,0);
		SetCurrentDialogEditFloat(EDT_DIFFUSER,0);
		SetCurrentDialogEditFloat(EDT_DIFFUSEG,0);
		SetCurrentDialogEditFloat(EDT_DIFFUSEB,0);
		SetCurrentDialogEditFloat(EDT_DIFFUSEA,1);
		SetCurrentDialogEditFloat(EDT_SPECULARR,1);
		SetCurrentDialogEditFloat(EDT_SPECULARG,1);
		SetCurrentDialogEditFloat(EDT_SPECULARB,1);
		SetCurrentDialogEditFloat(EDT_SPECULARA,1);
		SetCurrentDialogEditFloat(EDT_DIRX,0);
		SetCurrentDialogEditFloat(EDT_DIRY,-1);
		SetCurrentDialogEditFloat(EDT_DIRZ,0);
		SetCurrentDialogEditFloat(EDT_RANGE,10);

		// reset list box
		ResetCurrentDialogDropDownSelections(LST_NAME);
		
		// init list box
		for(dwCount=0;dwCount<g_dwNumLight;dwCount++)
		{
			// cast pointer to light name
			LPSTR lpName=g_lplpLightNames[dwCount];
			
			// add light to list if necessary
			if(lpName[0]!='~')
				AddCurrentDialogDropDownSelection(LST_NAME,
												  lpName);
		}
		
		// highlight previous light
		if(g_dwLastLight<g_dwNumLight)
			HighlightCurrentDialogDropDownSelection(LST_NAME,
													g_lplpLightNames[g_dwLastLight]);
		
		// force info to load
		goto LightSelChange;

	case(WM_COMMAND):
		// check notification message
		switch(HIWORD(wParam))
		{
		case(BN_CLICKED):
			// check for ok button
			if(LOWORD(wParam)==BTN_OK)
			{
				// send save message
				SendMessage(hDlg,
							WM_COMMAND,
							(WPARAM)((((DWORD)BN_CLICKED)<<16)+(DWORD)BTN_SAVE),
							NULL);

				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update environment
				UpdateWorld();
				
				// ok
				return(TRUE);
			}

			// check for cancel button
			if(LOWORD(wParam)==BTN_CANCEL)
			{
				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update world
				UpdateWorld();

				// ok
				return(TRUE);
			}
			
			// check for add button
			if(LOWORD(wParam)==BTN_ADD)
			{
				LockGlobalMsg()
					
				// get name to add
				GetCurrentDialogEditText(EDT_NAME,
										 g_lpMsg,
										 _MAX_PATH);

				// check name
				if(strlen(g_lpMsg)==0)
					DisplayInfoMessage("You must specify a name to add.");
				else if(g_lpMsg[0]=='~')
					DisplayInfoMessage("Specified name is invalid.");
				else if(FindLight(g_lpMsg)!=NO_OBJECT)
					DisplayInfoMessage("Specified light already exists.");
				else
				{
					// allocate name buffer
					LPSTR lpName;
					if(!AllocMem((LPVOID*)&lpName,
								 strlen(g_lpMsg)+1))
					{
						UnlockGlobalMsg()
						return(TRUE);
					}

					// copy name
					strcpy(lpName,
						   g_lpMsg);

					// set type
					DWORD dwType;
					if(GetCurrentDialogCheckBox(RAD_POINT))
						dwType=D3DLIGHT_POINT;
					else if(GetCurrentDialogCheckBox(RAD_SPOT))
						dwType=D3DLIGHT_SPOT;
					else
						dwType=D3DLIGHT_DIRECTIONAL;

					// add light
					DWORD dwIndex=AddLight((D3DLIGHTTYPE)dwType,
										   GetCurrentDialogEditFloat(EDT_DIFFUSER),
										   GetCurrentDialogEditFloat(EDT_DIFFUSEG),
										   GetCurrentDialogEditFloat(EDT_DIFFUSEB),
										   GetCurrentDialogEditFloat(EDT_DIFFUSEA),
										   GetCurrentDialogEditFloat(EDT_SPECULARR),
										   GetCurrentDialogEditFloat(EDT_SPECULARG),
										   GetCurrentDialogEditFloat(EDT_SPECULARB),
										   GetCurrentDialogEditFloat(EDT_SPECULARA),
										   0,0,0,1, // ambient
										   GetCurrentDialogEditFloat(EDT_POSX),
										   GetCurrentDialogEditFloat(EDT_POSY),
										   GetCurrentDialogEditFloat(EDT_POSZ),
										   GetCurrentDialogEditFloat(EDT_DIRX),
										   GetCurrentDialogEditFloat(EDT_DIRY),
										   GetCurrentDialogEditFloat(EDT_DIRZ),
										   GetCurrentDialogEditFloat(EDT_RANGE),
										   1,
										   0,1,0,
										   DegToRad(GetCurrentDialogEditFloat(EDT_INNERCONE)),
										   DegToRad(GetCurrentDialogEditFloat(EDT_OUTERCONE)),
										   lpName);

					// check light
					if(dwIndex<g_dwNumLight)
					{
						// enable light
						EnableLight(dwIndex);

						// add name to list
						AddCurrentDialogDropDownSelection(LST_NAME,
														  lpName);

						// highlight name
						HighlightCurrentDialogDropDownSelection(LST_NAME,
																lpName);
					}
							
					// free name buffer
					FreeMem((LPVOID*)&lpName);

					// remove name
					SetCurrentDialogEditText(EDT_NAME,
											 "");
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for delete button
			if(LOWORD(wParam)==BTN_DELETE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find light
					DWORD dwIndex=FindLight(g_lpMsg);

					// check light
					if(dwIndex<g_dwNumLight)
					{
						// eliminate light
						LPSTR lpName=g_lplpLightNames[dwIndex];
						lpName[0]='~';

						// delete name from list
						RemoveCurrentDialogDropDownSelection(LST_NAME);
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for save button
			if(LOWORD(wParam)==BTN_SAVE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find light
					DWORD dwIndex=FindLight(g_lpMsg);

					// check light
					if(dwIndex<g_dwNumLight)
					{
						// set type
						DWORD dwType;
						if(GetCurrentDialogCheckBox(RAD_POINT))
							dwType=D3DLIGHT_POINT;
						else if(GetCurrentDialogCheckBox(RAD_SPOT))
							dwType=D3DLIGHT_SPOT;
						else
							dwType=D3DLIGHT_DIRECTIONAL;
						
						// update light
						g_lpLight[dwIndex].Type=(D3DLIGHTTYPE)dwType;
						g_lpLight[dwIndex].Diffuse.r=GetCurrentDialogEditFloat(EDT_DIFFUSER);
						g_lpLight[dwIndex].Diffuse.g=GetCurrentDialogEditFloat(EDT_DIFFUSEG);
						g_lpLight[dwIndex].Diffuse.b=GetCurrentDialogEditFloat(EDT_DIFFUSEB);
						g_lpLight[dwIndex].Diffuse.a=GetCurrentDialogEditFloat(EDT_DIFFUSEA);
						g_lpLight[dwIndex].Specular.r=GetCurrentDialogEditFloat(EDT_SPECULARR);
						g_lpLight[dwIndex].Specular.g=GetCurrentDialogEditFloat(EDT_SPECULARG);
						g_lpLight[dwIndex].Specular.b=GetCurrentDialogEditFloat(EDT_SPECULARB);
						g_lpLight[dwIndex].Specular.a=GetCurrentDialogEditFloat(EDT_SPECULARA);
						g_lpLight[dwIndex].Position.x=GetCurrentDialogEditFloat(EDT_POSX);
						g_lpLight[dwIndex].Position.y=GetCurrentDialogEditFloat(EDT_POSY);
						g_lpLight[dwIndex].Position.z=GetCurrentDialogEditFloat(EDT_POSZ);
						g_lpLight[dwIndex].Direction.x=GetCurrentDialogEditFloat(EDT_DIRX);
						g_lpLight[dwIndex].Direction.y=GetCurrentDialogEditFloat(EDT_DIRY);
						g_lpLight[dwIndex].Direction.z=GetCurrentDialogEditFloat(EDT_DIRZ);
						g_lpLight[dwIndex].Range=GetCurrentDialogEditFloat(EDT_RANGE);
						g_lpLight[dwIndex].Theta=DegToRad(GetCurrentDialogEditFloat(EDT_INNERCONE));
						g_lpLight[dwIndex].Phi=DegToRad(GetCurrentDialogEditFloat(EDT_OUTERCONE));
						UpdateLight(dwIndex);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			// check for x-y button
			if(LOWORD(wParam)==BTN_POSXY)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find light
					DWORD dwIndex=FindLight(g_lpMsg);

					// check light
					if(dwIndex<g_dwNumLight)
					{
						// send save message
						SendMessage(hDlg,
									WM_COMMAND,
									(WPARAM)((((DWORD)BN_CLICKED)<<16)+(DWORD)BTN_SAVE),
									NULL);

						// set current light
						g_dwCurLight=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_LTPOSXY);

						// update environment
						UpdateWorld();
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for x-z button
			if(LOWORD(wParam)==BTN_POSXZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find light
					DWORD dwIndex=FindLight(g_lpMsg);

					// check light
					if(dwIndex<g_dwNumLight)
					{
						// send save message
						SendMessage(hDlg,
									WM_COMMAND,
									(WPARAM)((((DWORD)BN_CLICKED)<<16)+(DWORD)BTN_SAVE),
									NULL);

						// set current light
						g_dwCurLight=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_LTPOSXZ);

						// update environment
						UpdateWorld();
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for direction button
			if(LOWORD(wParam)==BTN_DIR)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find light
					DWORD dwIndex=FindLight(g_lpMsg);

					// check light
					if(dwIndex<g_dwNumLight)
					{
						// send save message
						SendMessage(hDlg,
									WM_COMMAND,
									(WPARAM)((((DWORD)BN_CLICKED)<<16)+(DWORD)BTN_SAVE),
									NULL);

						// set current light
						g_dwCurLight=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_LTDIR);

						// update environment
						UpdateWorld();
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}

			// ok
			break;

		case(CBN_SELCHANGE):
			// check identifier
			if(LOWORD(wParam)==LST_NAME)
			{
				LightSelChange:;

				LockGlobalMsg()
				
				// get selection text
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// replace index with light index
					DWORD dwIndex=FindLight(g_lpMsg);

					// check light
					if(dwIndex<g_dwNumLight)
					{
						// set last light
						g_dwLastLight=dwIndex;
						
						// reset radio buttons
						SetCurrentDialogCheckBox(RAD_SPOT,FALSE);
						SetCurrentDialogCheckBox(RAD_POINT,FALSE);
						SetCurrentDialogCheckBox(RAD_DIR,FALSE);

						// fill in values
						if(g_lpLight[dwIndex].Type==D3DLIGHT_SPOT)
							SetCurrentDialogCheckBox(RAD_SPOT,TRUE);
						else if(g_lpLight[dwIndex].Type==D3DLIGHT_POINT)
							SetCurrentDialogCheckBox(RAD_POINT,TRUE);
						else
							SetCurrentDialogCheckBox(RAD_DIR,TRUE);
						SetCurrentDialogEditFloat(EDT_INNERCONE,RadToDeg(g_lpLight[dwIndex].Theta));
						SetCurrentDialogEditFloat(EDT_OUTERCONE,RadToDeg(g_lpLight[dwIndex].Phi));
						SetCurrentDialogEditFloat(EDT_POSX,g_lpLight[dwIndex].Position.x);
						SetCurrentDialogEditFloat(EDT_POSY,g_lpLight[dwIndex].Position.y);
						SetCurrentDialogEditFloat(EDT_POSZ,g_lpLight[dwIndex].Position.z);
						SetCurrentDialogEditFloat(EDT_DIFFUSER,g_lpLight[dwIndex].Diffuse.r);
						SetCurrentDialogEditFloat(EDT_DIFFUSEG,g_lpLight[dwIndex].Diffuse.g);
						SetCurrentDialogEditFloat(EDT_DIFFUSEB,g_lpLight[dwIndex].Diffuse.b);
						SetCurrentDialogEditFloat(EDT_DIFFUSEA,g_lpLight[dwIndex].Diffuse.a);
						SetCurrentDialogEditFloat(EDT_SPECULARR,g_lpLight[dwIndex].Specular.r);
						SetCurrentDialogEditFloat(EDT_SPECULARG,g_lpLight[dwIndex].Specular.g);
						SetCurrentDialogEditFloat(EDT_SPECULARB,g_lpLight[dwIndex].Specular.b);
						SetCurrentDialogEditFloat(EDT_SPECULARA,g_lpLight[dwIndex].Specular.a);
						SetCurrentDialogEditFloat(EDT_DIRX,g_lpLight[dwIndex].Direction.x);
						SetCurrentDialogEditFloat(EDT_DIRY,g_lpLight[dwIndex].Direction.y);
						SetCurrentDialogEditFloat(EDT_DIRZ,g_lpLight[dwIndex].Direction.z);
						SetCurrentDialogEditFloat(EDT_RANGE,g_lpLight[dwIndex].Range);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}
		
		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}

// InitDialogMesh()

DWORD InitDialogMesh(void)
{
	LoadingOutput("InitDialogMesh");

	// get dialog index
	DWORD dwIndex=AddDialog(240,184,
							"Meshes");

	// add controls
	AddDialogEditControl(dwIndex,4,5,72,10,EDT_NAME);
	AddDialogDropDownListBox(dwIndex,122,4,72,64,LST_NAME);
	AddDialogPushButton(dwIndex,80, 5,38,10,"Add",   BTN_ADD);
	AddDialogPushButton(dwIndex,198,5,38,10,"Delete",BTN_DELETE);

	AddDialogGroupBox(dwIndex,4,18,55,82,"Type");
	AddDialogRadioButton(dwIndex,8,28,47,8,"File",    RAD_FILE);
	AddDialogRadioButton(dwIndex,8,38,47,8,"Box",     RAD_BOX);
	AddDialogRadioButton(dwIndex,8,48,47,8,"Cylinder",RAD_CYLINDER);
	AddDialogRadioButton(dwIndex,8,58,47,8,"Polygon", RAD_POLYGON);
	AddDialogRadioButton(dwIndex,8,68,47,8,"Sphere",  RAD_SPHERE);
	AddDialogRadioButton(dwIndex,8,78,47,8,"Teapot",  RAD_TEAPOT);
	AddDialogRadioButton(dwIndex,8,88,47,8,"Torus",   RAD_TORUS);

	AddDialogGroupBox(dwIndex,4,102,55,24,"Filename");
	AddDialogEditControl(dwIndex,8,112,47,10,EDT_FILE);

	AddDialogGroupBox(dwIndex,4,128,55,26,"Material");
	AddDialogDropDownListBox(dwIndex,8,138,47,64,LST_MAT);

	AddDialogGroupBox(dwIndex,63,18,55,78,"Box");
	AddDialogStaticControl(dwIndex,67,28,47,8,"Width");
	AddDialogStaticControl(dwIndex,67,50,47,8,"Height");
	AddDialogStaticControl(dwIndex,67,72,47,8,"Depth");
	AddDialogEditControl(dwIndex,67,38,47,10,EDT_BWIDTH);
	AddDialogEditControl(dwIndex,67,60,47,10,EDT_BHEIGHT);
	AddDialogEditControl(dwIndex,67,82,47,10,EDT_BDEPTH);

	AddDialogGroupBox(dwIndex,63,98,55,56,"Polygon");
	AddDialogStaticControl(dwIndex,67,108,47,8,"Length of Side");
	AddDialogStaticControl(dwIndex,67,130,47,8,"Sides");
	AddDialogEditControl(dwIndex,67,118,47,10,EDT_PLOS);
	AddDialogEditControl(dwIndex,67,140,47,10,EDT_PSIDES);

	AddDialogGroupBox(dwIndex,122,18,55,78,"Cylinder");
	AddDialogStaticControl(dwIndex,126,28,47,8,"Upper Radius");
	AddDialogStaticControl(dwIndex,126,50,47,8,"Lower Radius");
	AddDialogStaticControl(dwIndex,126,72,47,8,"Height");
	AddDialogEditControl(dwIndex,126,38,47,10,EDT_CUPPERRAD);
	AddDialogEditControl(dwIndex,126,60,47,10,EDT_CLOWERRAD);
	AddDialogEditControl(dwIndex,126,82,47,10,EDT_CHEIGHT);

	AddDialogGroupBox(dwIndex,122,98,55,56,"Detail");
	AddDialogStaticControl(dwIndex,126,108,47,8,"Slices");
	AddDialogStaticControl(dwIndex,126,130,47,8,"Stacks");
	AddDialogEditControl(dwIndex,126,118,47,10,EDT_SLICES);
	AddDialogEditControl(dwIndex,126,140,47,10,EDT_STACKS);

	AddDialogGroupBox(dwIndex,181,18,55,34,"Sphere");
	AddDialogStaticControl(dwIndex,185,28,47,8,"Radius");
	AddDialogEditControl(dwIndex,185,38,47,10,EDT_RAD);

	AddDialogGroupBox(dwIndex,181,54,55,100,"Torus");
	AddDialogStaticControl(dwIndex,185,64, 47,8,"Inner Radius");
	AddDialogStaticControl(dwIndex,185,86, 47,8,"Outer Radius");
	AddDialogStaticControl(dwIndex,185,108,47,8,"Sides");
	AddDialogStaticControl(dwIndex,185,130,47,8,"Rings");
	AddDialogEditControl(dwIndex,185,74, 47,10,EDT_TINNERRAD);
	AddDialogEditControl(dwIndex,185,96, 47,10,EDT_TOUTERRAD);
	AddDialogEditControl(dwIndex,185,118,47,10,EDT_TSIDES);
	AddDialogEditControl(dwIndex,185,140,47,10,EDT_TRINGS);

	AddDialogPushButton(dwIndex,4,  160,75,20,"OK",   BTN_OK,TRUE);
	AddDialogPushButton(dwIndex,83, 160,74,20,"Canel",BTN_CANCEL);
	AddDialogPushButton(dwIndex,161,160,75,20,"Save", BTN_SAVE);

	// return index
	return(dwIndex);
}

// DialogMeshProc()

BOOL APIENTRY DialogMeshProc(HWND hDlg,
						     UINT Msg,
						     WPARAM wParam,
						     LPARAM lParam)
{
	// counter
	DWORD dwCount;

	// check message
	switch(Msg)
	{
	case(WM_INITDIALOG):
		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// reset radio buttons
		SetCurrentDialogCheckBox(RAD_FILE,FALSE);
		SetCurrentDialogCheckBox(RAD_BOX,FALSE);
		SetCurrentDialogCheckBox(RAD_CYLINDER,FALSE);
		SetCurrentDialogCheckBox(RAD_POLYGON,FALSE);
		SetCurrentDialogCheckBox(RAD_SPHERE,FALSE);
		SetCurrentDialogCheckBox(RAD_TEAPOT,FALSE);
		SetCurrentDialogCheckBox(RAD_TORUS,FALSE);

		// init controls
		SetCurrentDialogCheckBox(RAD_FILE,TRUE);
		SetCurrentDialogEditText(EDT_NAME,"");
		SetCurrentDialogEditText(EDT_FILE,"");
		SetCurrentDialogEditFloat(EDT_BWIDTH,1);
		SetCurrentDialogEditFloat(EDT_BHEIGHT,1);
		SetCurrentDialogEditFloat(EDT_BDEPTH,1);
		SetCurrentDialogEditFloat(EDT_PLOS,1);
		SetCurrentDialogEditLong(EDT_PSIDES,3);
		SetCurrentDialogEditFloat(EDT_CUPPERRAD,1);
		SetCurrentDialogEditFloat(EDT_CLOWERRAD,1);
		SetCurrentDialogEditFloat(EDT_CHEIGHT,1);
		SetCurrentDialogEditLong(EDT_SLICES,8);
		SetCurrentDialogEditLong(EDT_STACKS,8);
		SetCurrentDialogEditFloat(EDT_RAD,1);
		SetCurrentDialogEditFloat(EDT_TINNERRAD,0.25F);
		SetCurrentDialogEditFloat(EDT_TOUTERRAD,1);
		SetCurrentDialogEditLong(EDT_TSIDES,8);
		SetCurrentDialogEditLong(EDT_TRINGS,8);

		// reset list boxes
		ResetCurrentDialogDropDownSelections(LST_NAME);
		ResetCurrentDialogDropDownSelections(LST_MAT);
		
		// init mesh list box
		for(dwCount=0;dwCount<g_dwNumMesh;dwCount++)
		{
			// cast pointer to mesh name
			LPSTR lpName=g_lplpMeshNames[dwCount];
			
			// add mesh to list if necessary
			if(lpName[0]!='~')
				AddCurrentDialogDropDownSelection(LST_NAME,
												  lpName);
		}
		
		// init material list box
		for(dwCount=0;dwCount<g_dwNumMat;dwCount++)
		{
			// cast pointer to material name
			LPSTR lpName=g_lplpMatNames[dwCount];
			
			// add material to list if necessary
			if(lpName[0]!='~')
				AddCurrentDialogDropDownSelection(LST_MAT,
												  lpName);
		}
		
		// highlight previous mesh
		if(g_dwLastMesh<g_dwNumMesh)
			HighlightCurrentDialogDropDownSelection(LST_NAME,
													g_lplpMeshNames[g_dwLastMesh]);

		// force info to load
		goto MeshSelChange;

	case(WM_COMMAND):
		// check notification message
		switch(HIWORD(wParam))
		{
		case(BN_CLICKED):
			// check for ok button
			if(LOWORD(wParam)==BTN_OK)
			{
				// send save message
				SendMessage(hDlg,
							WM_COMMAND,
							(WPARAM)((((DWORD)BN_CLICKED)<<16)+(DWORD)BTN_SAVE),
							NULL);

				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update environment
				UpdateWorld();
				
				// ok
				return(TRUE);
			}

			// check for cancel button
			if(LOWORD(wParam)==BTN_CANCEL)
			{
				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update world
				UpdateWorld();

				// ok
				return(TRUE);
			}
			
			// check for add button
			if(LOWORD(wParam)==BTN_ADD)
			{
				LockGlobalMsg()

				// get name to add
				GetCurrentDialogEditText(EDT_NAME,
										 g_lpMsg,
										 _MAX_PATH);

				// check name
				if(strlen(g_lpMsg)==0)
					DisplayInfoMessage("You must specify a name to add.");
				else if(g_lpMsg[0]=='~')
					DisplayInfoMessage("Specified name is invalid.");
				else if(FindMesh(g_lpMsg)!=NO_OBJECT)
					DisplayInfoMessage("Specified mesh already exists.");
				else
				{
					// allocate name buffer
					LPSTR lpName;
					if(!AllocMem((LPVOID*)&lpName,
								 strlen(g_lpMsg)+1))
					{
						UnlockGlobalMsg()
						return(TRUE);
					}

					// copy name
					strcpy(lpName,
						   g_lpMsg);

					// add info to name
					if(GetCurrentDialogCheckBox(RAD_FILE))
					{
						// get filename
						GetCurrentDialogEditText(EDT_FILE,
												 g_lpMsg,
												 _MAX_PATH);
						
						// allocate filename buffer
						LPSTR lpFile;
						if(!AllocMem((LPVOID*)&lpFile,
									 strlen(g_lpMsg)+1))
						{
							UnlockGlobalMsg()
							return(TRUE);
						}

						// copy filename
						strcpy(lpFile,
							   g_lpMsg);

						// create name w/ info
						sprintf(g_lpMsg,
								"%s&file&%s",
								lpName,
								lpFile);

						// free filename
						FreeMem((LPVOID*)&lpFile);
					}
					else if(GetCurrentDialogCheckBox(RAD_BOX))
					{
						// get info
						float fWidth =GetCurrentDialogEditFloat(EDT_BWIDTH);
						float fHeight=GetCurrentDialogEditFloat(EDT_BHEIGHT);
						float fDepth =GetCurrentDialogEditFloat(EDT_BDEPTH);
						
						// get material
						if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
						{
							DisplayInfoMessage("You must select a mesh.");
							FreeMem((LPVOID*)&lpName);
							UnlockGlobalMsg()
							return(TRUE);
						}
						DWORD dwMat=FindMaterial(g_lpMsg);

						// create name w/ info
						sprintf(g_lpMsg,
								"%s&box&%f&%f&%f&%d",
								lpName,
								fWidth,
								fHeight,
								fDepth,
								dwMat);
					}
					else if(GetCurrentDialogCheckBox(RAD_CYLINDER))
					{
						// get info
						float fLowerRad  =GetCurrentDialogEditFloat(EDT_CLOWERRAD);
						float fUpperRad  =GetCurrentDialogEditFloat(EDT_CUPPERRAD);
						float fHeight    =GetCurrentDialogEditFloat(EDT_CHEIGHT);
						DWORD dwNumSlices=(DWORD)GetCurrentDialogEditLong(EDT_SLICES);
						DWORD dwNumStacks=(DWORD)GetCurrentDialogEditLong(EDT_STACKS);
						
						// get material
						if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
						{
							DisplayInfoMessage("You must select a mesh.");
							FreeMem((LPVOID*)&lpName);
							UnlockGloblaMsg()
							return(TRUE);
						}
						DWORD dwMat=FindMaterial(g_lpMsg);

						// create name w/ info
						sprintf(g_lpMsg,
								"%s&cylinder&%f&%f&%f&%d&%d&%d",
								lpName,
								fLowerRad,
								fUpperRad,
								fHeight,
								dwNumSlices,
								dwNumStacks,
								dwMat);
					}
					else if(GetCurrentDialogCheckBox(RAD_POLYGON))
					{
						// get info
						float fLengthOfSide=GetCurrentDialogEditFloat(EDT_PLOS);
						DWORD dwNumSides   =(DWORD)GetCurrentDialogEditLong(EDT_PSIDES);
						
						// get material
						if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
						{
							DisplayInfoMessage("You must select a mesh.");
							FreeMem((LPVOID*)&lpName);
							UnlockGlobalMsg()
							return(TRUE);
						}
						DWORD dwMat=FindMaterial(g_lpMsg);

						// create name w/ info
						sprintf(g_lpMsg,
								"%s&polygon&%f&%d&%d",
								lpName,
								fLengthOfSide,
								dwNumSides,
								dwMat);
					}
					else if(GetCurrentDialogCheckBox(RAD_SPHERE))
					{
						// get info
						float fRadius=GetCurrentDialogEditFloat(EDT_RAD);
						DWORD dwNumSlices=(DWORD)GetCurrentDialogEditLong(EDT_SLICES);
						DWORD dwNumStacks=(DWORD)GetCurrentDialogEditLong(EDT_STACKS);
						
						// get material
						if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
						{
							DisplayInfoMessage("You must select a mesh.");
							FreeMem((LPVOID*)&lpName);
							UnlockGlobalMsg()
							return(TRUE);
						}
						DWORD dwMat=FindMaterial(g_lpMsg);

						// create name w/ info
						sprintf(g_lpMsg,
								"%s&sphere&%f&%d&%d&%d",
								lpName,
								fRadius,
								dwNumSlices,
								dwNumStacks,
								dwMat);
					}
					else if(GetCurrentDialogCheckBox(RAD_TEAPOT))
					{
						// get material
						if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
						{
							DisplayInfoMessage("You must select a mesh.");
							FreeMem((LPVOID*)&lpName);
							UnlockGlobalMsg()
							return(TRUE);
						}
						DWORD dwMat=FindMaterial(g_lpMsg);

						// create name w/ info
						sprintf(g_lpMsg,
								"%s&teapot&%d",
								lpName,
								dwMat);
					}
					else if(GetCurrentDialogCheckBox(RAD_TORUS))
					{
						// get info
						float fInnerRad =GetCurrentDialogEditFloat(EDT_TINNERRAD);
						float fOuterRad =GetCurrentDialogEditFloat(EDT_TOUTERRAD);
						DWORD dwNumSides=(DWORD)GetCurrentDialogEditLong(EDT_TSIDES);
						DWORD dwNumRings=(DWORD)GetCurrentDialogEditLong(EDT_TRINGS);
						
						// get material
						if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
						{
							DisplayInfoMessage("You must select a mesh.");
							FreeMem((LPVOID*)&lpName);
							UnlockGlobalMsg()
							return(TRUE);
						}
						DWORD dwMat=FindMaterial(g_lpMsg);

						// create name w/ info
						sprintf(g_lpMsg,
								"%s&torus&%f&%f&%d&%d&%d",
								lpName,
								fInnerRad,
								fOuterRad,
								dwNumSides,
								dwNumRings,
								dwMat);
					}
					else
					{
						DisplayInfoMessage("You must specify a type of mesh to add.");
						UnlockGlobalMsg()
						return(TRUE);
					}

					// free name buffer
					FreeMem((LPVOID*)&lpName);

					// allocate name buffer
					if(!AllocMem((LPVOID*)&lpName,
								 strlen(g_lpMsg)+1))
					{
						UnlockGlobalMsg()
						return(TRUE);
					}

					// copy name
					strcpy(lpName,
						   g_lpMsg);

					// mesh index
					DWORD dwMeshIndex;

					// add mesh
					if(GetCurrentDialogCheckBox(RAD_FILE))
					{
						// get filename
						GetCurrentDialogEditText(EDT_FILE,
												 g_lpMsg,
												 _MAX_PATH);
						
						// add mesh
						dwMeshIndex=AddMesh(g_lpMsg,
											lpName);
					}
					else if(GetCurrentDialogCheckBox(RAD_BOX))
					{
						// get info
						float fWidth =GetCurrentDialogEditFloat(EDT_BWIDTH);
						float fHeight=GetCurrentDialogEditFloat(EDT_BHEIGHT);
						float fDepth =GetCurrentDialogEditFloat(EDT_BDEPTH);
						
						// get material
						GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg);
						DWORD dwMat=FindMaterial(g_lpMsg);

						// add mesh
						dwMeshIndex=AddMeshBox(fWidth,
											   fHeight,
											   fDepth,
											   dwMat,
											   lpName);
					}
					else if(GetCurrentDialogCheckBox(RAD_CYLINDER))
					{
						// get info
						float fLowerRad  =GetCurrentDialogEditFloat(EDT_CLOWERRAD);
						float fUpperRad  =GetCurrentDialogEditFloat(EDT_CUPPERRAD);
						float fHeight    =GetCurrentDialogEditFloat(EDT_CHEIGHT);
						DWORD dwNumSlices=(DWORD)GetCurrentDialogEditLong(EDT_SLICES);
						DWORD dwNumStacks=(DWORD)GetCurrentDialogEditLong(EDT_STACKS);
						
						// get material
						GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg);
						DWORD dwMat=FindMaterial(g_lpMsg);

						// add mesh
						dwMeshIndex=AddMeshCylinder(fLowerRad,
													fUpperRad,
													fHeight,
													dwNumSlices,
													dwNumStacks,
													dwMat,
													lpName);
					}
					else if(GetCurrentDialogCheckBox(RAD_POLYGON))
					{
						// get info
						float fLengthOfSide=GetCurrentDialogEditFloat(EDT_PLOS);
						DWORD dwNumSides   =(DWORD)GetCurrentDialogEditLong(EDT_PSIDES);
						
						// get material
						GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg);
						DWORD dwMat=FindMaterial(g_lpMsg);

						// add mesh
						dwMeshIndex=AddMeshPolygon(fLengthOfSide,
												   dwNumSides,
												   dwMat,
												   lpName);
					}
					else if(GetCurrentDialogCheckBox(RAD_SPHERE))
					{
						// get info
						float fRadius=GetCurrentDialogEditFloat(EDT_RAD);
						DWORD dwNumSlices=(DWORD)GetCurrentDialogEditLong(EDT_SLICES);
						DWORD dwNumStacks=(DWORD)GetCurrentDialogEditLong(EDT_STACKS);
						
						// get material
						GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg);
						DWORD dwMat=FindMaterial(g_lpMsg);

						// add mesh
						dwMeshIndex=AddMeshSphere(fRadius,
												  dwNumSlices,
												  dwNumStacks,
												  dwMat,
												  lpName);
					}
					else if(GetCurrentDialogCheckBox(RAD_TEAPOT))
					{
						// get material
						GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg);
						DWORD dwMat=FindMaterial(g_lpMsg);

						// add mesh
						dwMeshIndex=AddMeshTeapot(dwMat,
												  lpName);
					}
					else if(GetCurrentDialogCheckBox(RAD_TORUS))
					{
						// get info
						float fInnerRad =GetCurrentDialogEditFloat(EDT_TINNERRAD);
						float fOuterRad =GetCurrentDialogEditFloat(EDT_TOUTERRAD);
						DWORD dwNumSides=(DWORD)GetCurrentDialogEditLong(EDT_TSIDES);
						DWORD dwNumRings=(DWORD)GetCurrentDialogEditLong(EDT_TRINGS);
						
						// get material
						GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg);
						DWORD dwMat=FindMaterial(g_lpMsg);

						// add mesh
						dwMeshIndex=AddMeshTorus(fInnerRad,
												 fOuterRad,
												 dwNumSides,
												 dwNumRings,
												 dwMat,
												 lpName);
					}

					// check mesh
					if(dwMeshIndex<g_dwNumMesh)
					{
						// divide name
						DivideString(g_lplpMeshNames[dwMeshIndex]);

						// add name to list
						AddCurrentDialogDropDownSelection(LST_NAME,
														  g_lplpMeshNames[dwMeshIndex]);

						// highlight name
						HighlightCurrentDialogDropDownSelection(LST_NAME,
																g_lplpMeshNames[dwMeshIndex]);
					}

					// free name buffer
					FreeMem((LPVOID*)&lpName);

					// remove name
					SetCurrentDialogEditText(EDT_NAME,
											 "");
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for delete button
			if(LOWORD(wParam)==BTN_DELETE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find mesh
					DWORD dwIndex=FindMesh(g_lpMsg);

					// check mesh
					if(dwIndex<g_dwNumMesh)
					{
						// eliminate mesh
						LPSTR lpName=g_lplpMeshNames[dwIndex];
						lpName[0]='~';

						// delete name from list
						RemoveCurrentDialogDropDownSelection(LST_NAME);
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for save button
			if(LOWORD(wParam)==BTN_SAVE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find mesh
					DWORD dwIndex=FindMesh(g_lpMsg);

					// check mesh
					if(dwIndex<g_dwNumMesh)
					{
						// allocate name buffer
						LPSTR lpName;
						if(!AllocMem((LPVOID*)&lpName,
									 strlen(g_lpMsg)+1))
						{
							UnlockGlobalMsg()
							return(TRUE);
						}

						// copy name
						strcpy(lpName,
							   g_lpMsg);

						// add info to name
						if(GetCurrentDialogCheckBox(RAD_FILE))
						{
							// get filename
							GetCurrentDialogEditText(EDT_FILE,
													 g_lpMsg,
													 _MAX_PATH);
							
							// allocate filename buffer
							LPSTR lpFile;
							if(!AllocMem((LPVOID*)&lpFile,
										 strlen(g_lpMsg)+1))
							{
								UnlockGlobalMsg()
								return(TRUE);
							}

							// copy filename
							strcpy(lpFile,
								   g_lpMsg);

							// create name w/ info
							sprintf(g_lpMsg,
									"%s&file&%s",
									lpName,
									lpFile);

							// free filename
							FreeMem((LPVOID*)&lpFile);
						}
						else if(GetCurrentDialogCheckBox(RAD_BOX))
						{
							// get info
							float fWidth =GetCurrentDialogEditFloat(EDT_BWIDTH);
							float fHeight=GetCurrentDialogEditFloat(EDT_BHEIGHT);
							float fDepth =GetCurrentDialogEditFloat(EDT_BDEPTH);
							
							// get material
							if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
							{
								DisplayInfoMessage("You must select a mesh.");
								FreeMem((LPVOID*)&lpName);
								UnlockGlobalMsg()
								return(TRUE);
							}
							DWORD dwMat=FindMaterial(g_lpMsg);

							// create name w/ info
							sprintf(g_lpMsg,
									"%s&box&%f&%f&%f&%d",
									lpName,
									fWidth,
									fHeight,
									fDepth,
									dwMat);
						}
						else if(GetCurrentDialogCheckBox(RAD_CYLINDER))
						{
							// get info
							float fLowerRad  =GetCurrentDialogEditFloat(EDT_CLOWERRAD);
							float fUpperRad  =GetCurrentDialogEditFloat(EDT_CUPPERRAD);
							float fHeight    =GetCurrentDialogEditFloat(EDT_CHEIGHT);
							DWORD dwNumSlices=(DWORD)GetCurrentDialogEditLong(EDT_SLICES);
							DWORD dwNumStacks=(DWORD)GetCurrentDialogEditLong(EDT_STACKS);
							
							// get material
							if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
							{
								DisplayInfoMessage("You must select a mesh.");
								FreeMem((LPVOID*)&lpName);
								UnlockGlobalMsg()
								return(TRUE);
							}
							DWORD dwMat=FindMaterial(g_lpMsg);

							// create name w/ info
							sprintf(g_lpMsg,
									"%s&cylinder&%f&%f&%f&%d&%d&%d",
									lpName,
									fLowerRad,
									fUpperRad,
									fHeight,
									dwNumSlices,
									dwNumStacks,
									dwMat);
						}
						else if(GetCurrentDialogCheckBox(RAD_POLYGON))
						{
							// get info
							float fLengthOfSide=GetCurrentDialogEditFloat(EDT_PLOS);
							DWORD dwNumSides   =(DWORD)GetCurrentDialogEditLong(EDT_PSIDES);
							
							// get material
							if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
							{
								DisplayInfoMessage("You must select a mesh.");
								FreeMem((LPVOID*)&lpName);
								UnlockGlobalMsg()
								return(TRUE);
							}
							DWORD dwMat=FindMaterial(g_lpMsg);

							// create name w/ info
							sprintf(g_lpMsg,
									"%s&polygon&%f&%d&%d",
									lpName,
									fLengthOfSide,
									dwNumSides,
									dwMat);
						}
						else if(GetCurrentDialogCheckBox(RAD_SPHERE))
						{
							// get info
							float fRadius=GetCurrentDialogEditFloat(EDT_RAD);
							DWORD dwNumSlices=(DWORD)GetCurrentDialogEditLong(EDT_SLICES);
							DWORD dwNumStacks=(DWORD)GetCurrentDialogEditLong(EDT_STACKS);
							
							// get material
							if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
							{
								DisplayInfoMessage("You must select a mesh.");
								FreeMem((LPVOID*)&lpName);
								UnlockGlobalMsg()
								return(TRUE);
							}
							DWORD dwMat=FindMaterial(g_lpMsg);

							// create name w/ info
							sprintf(g_lpMsg,
									"%s&sphere&%f&%d&%d&%d",
									lpName,
									fRadius,
									dwNumSlices,
									dwNumStacks,
									dwMat);
						}
						else if(GetCurrentDialogCheckBox(RAD_TEAPOT))
						{
							// get material
							if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
							{
								DisplayInfoMessage("You must select a mesh.");
								FreeMem((LPVOID*)&lpName);
								UnlockGlobalMsg()
								return(TRUE);
							}
							DWORD dwMat=FindMaterial(g_lpMsg);

							// create name w/ info
							sprintf(g_lpMsg,
									"%s&teapot&%d",
									lpName,
									dwMat);
						}
						else if(GetCurrentDialogCheckBox(RAD_TORUS))
						{
							// get info
							float fInnerRad =GetCurrentDialogEditFloat(EDT_TINNERRAD);
							float fOuterRad =GetCurrentDialogEditFloat(EDT_TOUTERRAD);
							DWORD dwNumSides=(DWORD)GetCurrentDialogEditLong(EDT_TSIDES);
							DWORD dwNumRings=(DWORD)GetCurrentDialogEditLong(EDT_TRINGS);
							
							// get material
							if(!GetCurrentDialogDropDownSelection(LST_MAT,g_lpMsg))
							{
								DisplayInfoMessage("You must select a mesh.");
								FreeMem((LPVOID*)&lpName);
								UnlockGlobalMsg()
								return(TRUE);
							}
							DWORD dwMat=FindMaterial(g_lpMsg);

							// create name w/ info
							sprintf(g_lpMsg,
									"%s&torus&%f&%f&%d&%d&%d",
									lpName,
									fInnerRad,
									fOuterRad,
									dwNumSides,
									dwNumRings,
									dwMat);
						}
						else
						{
							DisplayInfoMessage("You must specify a type of mesh to save.");
							UnlockGlobalMsg()
							return(TRUE);
						}

						// free name buffer
						FreeMem((LPVOID*)&lpName);

						// free mesh name
						FreeMem((LPVOID*)&g_lplpMeshNames[dwIndex]);

						// allocate new mesh name
						if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
									 strlen(g_lpMsg)+1))
						{
							UnlockGlobalMsg()
							return(TRUE);
						}

						// copy name
						strcpy(g_lplpMeshNames[dwIndex],
							   g_lpMsg);

						// divide name
						DivideString(g_lplpMeshNames[dwIndex]);
					}

					UnlockGlobalMsg()
					
					// ok
					return(TRUE);

				}

				// ok
				return(TRUE);
			}

			// ok
			break;

		case(CBN_SELCHANGE):
			// check identifier
			if(LOWORD(wParam)==LST_NAME)
			{
				MeshSelChange:;

				LockGlobalMsg()

				// get selection text
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// replace index with mesh index
					DWORD dwIndex=FindMesh(g_lpMsg);

					// check mesh
					if(dwIndex<g_dwNumMesh)
					{
						// set last mesh
						g_dwLastMesh=dwIndex;
						
						// reset radio buttons
						SetCurrentDialogCheckBox(RAD_FILE,FALSE);
						SetCurrentDialogCheckBox(RAD_BOX,FALSE);
						SetCurrentDialogCheckBox(RAD_CYLINDER,FALSE);
						SetCurrentDialogCheckBox(RAD_POLYGON,FALSE);
						SetCurrentDialogCheckBox(RAD_SPHERE,FALSE);
						SetCurrentDialogCheckBox(RAD_TEAPOT,FALSE);
						SetCurrentDialogCheckBox(RAD_TORUS,FALSE);

						// init values
						SetCurrentDialogEditText(EDT_NAME,"");
						SetCurrentDialogEditText(EDT_FILE,"");
						SetCurrentDialogEditFloat(EDT_BWIDTH,1);
						SetCurrentDialogEditFloat(EDT_BHEIGHT,1);
						SetCurrentDialogEditFloat(EDT_BDEPTH,1);
						SetCurrentDialogEditFloat(EDT_PLOS,1);
						SetCurrentDialogEditLong(EDT_PSIDES,3);
						SetCurrentDialogEditFloat(EDT_CUPPERRAD,1);
						SetCurrentDialogEditFloat(EDT_CLOWERRAD,1);
						SetCurrentDialogEditFloat(EDT_CHEIGHT,1);
						SetCurrentDialogEditLong(EDT_SLICES,8);
						SetCurrentDialogEditLong(EDT_STACKS,8);
						SetCurrentDialogEditFloat(EDT_RAD,1);
						SetCurrentDialogEditFloat(EDT_TINNERRAD,0.25F);
						SetCurrentDialogEditFloat(EDT_TOUTERRAD,1);
						SetCurrentDialogEditLong(EDT_TSIDES,8);
						SetCurrentDialogEditLong(EDT_TRINGS,8);
						HighlightCurrentDialogDropDownSelection(LST_MAT,"");

						// fill in necessary values
						if(stricmp(GetStringDivision(g_lplpMeshNames[dwIndex],1),
								   "file")==0)
						{
							SetCurrentDialogCheckBox(RAD_FILE,TRUE);
							SetCurrentDialogEditText(EDT_FILE,GetStringDivision(g_lplpMeshNames[dwIndex],2));
						}
						else if(stricmp(GetStringDivision(g_lplpMeshNames[dwIndex],1),
										"box")==0)
						{
							SetCurrentDialogCheckBox(RAD_BOX,TRUE);
							SetCurrentDialogEditText(EDT_BWIDTH,GetStringDivision(g_lplpMeshNames[dwIndex],2));
							SetCurrentDialogEditText(EDT_BHEIGHT,GetStringDivision(g_lplpMeshNames[dwIndex],3));
							SetCurrentDialogEditText(EDT_BDEPTH,GetStringDivision(g_lplpMeshNames[dwIndex],4));
							DWORD dwMat=(DWORD)atol(GetStringDivision(g_lplpMeshNames[dwIndex],5));
							if(dwMat<g_dwNumMat)
								HighlightCurrentDialogDropDownSelection(LST_MAT,g_lplpMatNames[dwMat]);
						}
						else if(stricmp(GetStringDivision(g_lplpMeshNames[dwIndex],1),
										"cylinder")==0)
						{
							SetCurrentDialogCheckBox(RAD_CYLINDER,TRUE);
							SetCurrentDialogEditText(EDT_CLOWERRAD,GetStringDivision(g_lplpMeshNames[dwIndex],2));
							SetCurrentDialogEditText(EDT_CUPPERRAD,GetStringDivision(g_lplpMeshNames[dwIndex],3));
							SetCurrentDialogEditText(EDT_CHEIGHT,GetStringDivision(g_lplpMeshNames[dwIndex],4));
							SetCurrentDialogEditText(EDT_SLICES,GetStringDivision(g_lplpMeshNames[dwIndex],5));
							SetCurrentDialogEditText(EDT_STACKS,GetStringDivision(g_lplpMeshNames[dwIndex],6));
							DWORD dwMat=(DWORD)atol(GetStringDivision(g_lplpMeshNames[dwIndex],7));
							if(dwMat<g_dwNumMat)
								HighlightCurrentDialogDropDownSelection(LST_MAT,g_lplpMatNames[dwMat]);
						}
						else if(stricmp(GetStringDivision(g_lplpMeshNames[dwIndex],1),
										"polygon")==0)
						{
							SetCurrentDialogCheckBox(RAD_POLYGON,TRUE);
							SetCurrentDialogEditText(EDT_PLOS,GetStringDivision(g_lplpMeshNames[dwIndex],2));
							SetCurrentDialogEditText(EDT_PSIDES,GetStringDivision(g_lplpMeshNames[dwIndex],3));
							DWORD dwMat=(DWORD)atol(GetStringDivision(g_lplpMeshNames[dwIndex],4));
							if(dwMat<g_dwNumMat)
								HighlightCurrentDialogDropDownSelection(LST_MAT,g_lplpMatNames[dwMat]);
						}
						else if(stricmp(GetStringDivision(g_lplpMeshNames[dwIndex],1),
										"sphere")==0)
						{
							SetCurrentDialogCheckBox(RAD_SPHERE,TRUE);
							SetCurrentDialogEditText(EDT_RAD,GetStringDivision(g_lplpMeshNames[dwIndex],2));
							SetCurrentDialogEditText(EDT_SLICES,GetStringDivision(g_lplpMeshNames[dwIndex],3));
							SetCurrentDialogEditText(EDT_STACKS,GetStringDivision(g_lplpMeshNames[dwIndex],4));
							DWORD dwMat=(DWORD)atol(GetStringDivision(g_lplpMeshNames[dwIndex],5));
							if(dwMat<g_dwNumMat)
								HighlightCurrentDialogDropDownSelection(LST_MAT,g_lplpMatNames[dwMat]);
						}
						else if(stricmp(GetStringDivision(g_lplpMeshNames[dwIndex],1),
										"teapot")==0)
						{
							SetCurrentDialogCheckBox(RAD_TEAPOT,TRUE);
							DWORD dwMat=(DWORD)atol(GetStringDivision(g_lplpMeshNames[dwIndex],2));
							if(dwMat<g_dwNumMat)
								HighlightCurrentDialogDropDownSelection(LST_MAT,g_lplpMatNames[dwMat]);
						}
						else if(stricmp(GetStringDivision(g_lplpMeshNames[dwIndex],1),
										"torus")==0)
						{
							SetCurrentDialogCheckBox(RAD_TORUS,TRUE);
							SetCurrentDialogEditText(EDT_TINNERRAD,GetStringDivision(g_lplpMeshNames[dwIndex],2));
							SetCurrentDialogEditText(EDT_TOUTERRAD,GetStringDivision(g_lplpMeshNames[dwIndex],3));
							SetCurrentDialogEditText(EDT_TSIDES,GetStringDivision(g_lplpMeshNames[dwIndex],4));
							SetCurrentDialogEditText(EDT_TRINGS,GetStringDivision(g_lplpMeshNames[dwIndex],5));
							DWORD dwMat=(DWORD)atol(GetStringDivision(g_lplpMeshNames[dwIndex],6));
							if(dwMat<g_dwNumMat)
								HighlightCurrentDialogDropDownSelection(LST_MAT,g_lplpMatNames[dwMat]);
						}
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}
		
		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}
#endif

// GetNextDynamicWorldMesh()

DWORD GetNextDynamicWorldMesh(void)
{
	LoadingOutput("GetNextDynamicWorldMesh");

	// check counter
	if(g_dwNumDWMesh==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwDWMMeshIndex,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDWMMtxIndex,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpDWMeshNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDWMesh=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumDWMesh>=g_dwMaxDWMesh)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwDWMMeshIndex,
					  sizeof(DWORD)*g_dwMaxDWMesh,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDWMMtxIndex,
					  sizeof(DWORD)*g_dwMaxDWMesh,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpDWMeshNames,
					  sizeof(LPSTR)*g_dwMaxDWMesh,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDWMesh+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumDWMesh;

	// increment counter
	g_dwNumDWMesh++;

	// return index
	return(dwIndex);
}

// AddDynamicWorldMesh()

DWORD AddDynamicWorldMesh(DWORD dwMeshIndex,
						  DWORD dwWMtxIndex,
						  LPSTR lpName)
{
	LoadingOutput("AddDynamicWorldMesh");

#ifdef DEBUG_MODE
	// verify index
	if(dwMeshIndex>=g_dwNumMesh)

	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "AddDynamicWorldMesh()",
								 FALSE);
		return(NO_OBJECT);
	}

	// verify indices
	if(dwWMtxIndex>=g_dwNumMtx&&
	   dwWMtxIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "AddDynamicWorldMesh()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// get matrix
	D3DXMATRIX mMtx;
	GetMatrix(dwWMtxIndex,
			  &mMtx);

	// add mesh
	return(AddDynamicWorldMesh(dwMeshIndex,
							   &mMtx,
							   lpName));
}

// AddDynamicWorldMesh()

DWORD AddDynamicWorldMesh(DWORD dwMeshIndex,
					      float fX,
					      float fY,
					      float fZ,
						  LPSTR lpName)
{
	LoadingOutput("AddDynamicWorldMesh");

#ifdef DEBUG_MODE
	// verify index
	if(dwMeshIndex>=g_dwNumMesh)

	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "AddDynamicWorldMesh()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// prepare matrix
	D3DXMATRIX mMtx=D3DXMATRIX(1, 0, 0, 0,
							   0, 1, 0, 0,
							   0, 0, 1, 0,
							   fX,fY,fZ,1);

	// add mesh
	return(AddDynamicWorldMesh(dwMeshIndex,
	 						   &mMtx,
							   lpName));
}

// AddDynamicWorldMesh()

DWORD AddDynamicWorldMesh(DWORD dwMeshIndex,
						  LPD3DXMATRIX lpMtx,
						  LPSTR lpName)
{
	LoadingOutput("AddDynamicWorldMesh");

#ifdef DEBUG_MODE
	// verify index
	if(dwMeshIndex>=g_dwNumMesh)

	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "AddDynamicWorldMesh()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()
		
		// create name
		sprintf(g_lpMsg,"~DM%d",g_dwNumDWMesh);

		// add mesh
		DWORD dwRet=AddDynamicWorldMesh(dwMeshIndex,lpMtx,g_lpMsg);

		// ok
		return(dwRet);
	}
	
	// get return index
	DWORD dwIndex=GetNextDynamicWorldMesh();

	// copy mesh index
	g_lpdwDWMMeshIndex[dwIndex]=dwMeshIndex;

	// get matrix index
	g_lpdwDWMMtxIndex[dwIndex]=AddMatrix(lpMtx);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpDWMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpDWMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);
	
	// return index
	return(dwIndex);
}

// RenderDynamicWorldMeshes()

void RenderDynamicWorldMeshes(void)
{
	// counter
	DWORD dwCount;

	// render each mesh
	if(g_dwNumDWMesh)
		for(dwCount=0;dwCount<g_dwNumDWMesh;dwCount++)
			RenderMesh(g_lpdwDWMMeshIndex[dwCount],
					   g_lpdwDWMMtxIndex[dwCount]);
}

// PurgeDynamicWorldMeshes()

void PurgeDynamicWorldMeshes(void)
{
	// check counter
	if(g_dwNumDWMesh)
	{
		// counter
		DWORD dwCount;
		
		// release name buffers
		for(dwCount=0;dwCount<g_dwNumDWMesh;dwCount++)
			FreeMem((LPVOID*)&g_lplpDWMeshNames[dwCount]);
	}

	// free memory
	FreeMem((LPVOID*)&g_lpdwDWMMeshIndex);
	FreeMem((LPVOID*)&g_lpdwDWMMtxIndex);
	FreeMem((LPVOID*)&g_lplpDWMeshNames);

	// reset counters
	g_dwNumDWMesh=0;
	g_dwMaxDWMesh=0;
}

// FindStaticWorldMesh()

DWORD FindStaticWorldMesh(LPCSTR lpName)
{
	LoadingOutput("FindStaticWorldMesh");

	// counter
	DWORD dwCount;

	// search meshes
	if(g_dwNumSWMesh)
		for(dwCount=0;dwCount<g_dwNumSWMesh;dwCount++)
			if(stricmp(lpName,g_lplpSWMeshNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// FindDynamicWorldMesh()

DWORD FindDynamicWorldMesh(LPCSTR lpName)
{
	LoadingOutput("FindDynamicWorldMesh");

	// counter
	DWORD dwCount;

	// search meshes
	if(g_dwNumDWMesh)
		for(dwCount=0;dwCount<g_dwNumDWMesh;dwCount++)
			if(stricmp(lpName,g_lplpDWMeshNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

#ifdef DEBUG_MODE
// InitDialogSWMesh()

DWORD InitDialogSWMesh(void)
{
	LoadingOutput("InitDialogSWMesh");

	// get dialog index
	DWORD dwIndex=AddDialog(240,170,
							"Static Meshes");

	// add controls
	AddDialogEditControl(dwIndex,4,5,72,10,EDT_NAME);
	AddDialogDropDownListBox(dwIndex,122,4,72,64,LST_NAME);
	AddDialogPushButton(dwIndex,80, 5,38,10,"Add",   BTN_ADD);
	AddDialogPushButton(dwIndex,198,5,38,10,"Delete",BTN_DELETE);

	AddDialogGroupBox(dwIndex,4,18,55,120,"Position");
	AddDialogStaticControl(dwIndex,8,28,47,8,"X");
	AddDialogStaticControl(dwIndex,8,50,47,8,"Y");
	AddDialogStaticControl(dwIndex,8,72,47,8,"Z");
	AddDialogEditControl(dwIndex,8,38, 47,10,EDT_POSX);
	AddDialogEditControl(dwIndex,8,60, 47,10,EDT_POSY);
	AddDialogEditControl(dwIndex,8,82, 47,10,EDT_POSZ);
	AddDialogPushButton(dwIndex,8, 96,13,10,"X",BTN_POSX);
	AddDialogPushButton(dwIndex,25,96,13,10,"Y",BTN_POSY);
	AddDialogPushButton(dwIndex,42,96,13,10,"Z",BTN_POSZ);
	AddDialogPushButton(dwIndex,8, 110,13,10,"XY",BTN_POSXY);
	AddDialogPushButton(dwIndex,25,110,13,10,"XZ",BTN_POSXZ);
	AddDialogPushButton(dwIndex,42,110,13,10,"YZ",BTN_POSYZ);
	AddDialogPushButton(dwIndex,8,124,47,10,"Scale",BTN_SCALE);

	AddDialogGroupBox(dwIndex,4,140,55,26,"Mesh");
	AddDialogDropDownListBox(dwIndex,8,150,47,64,LST_MESH);

	AddDialogGroupBox(dwIndex,63,18,55,120,"Right");
	AddDialogStaticControl(dwIndex,67,28,47,8,"X");
	AddDialogStaticControl(dwIndex,67,50,47,8,"Y");
	AddDialogStaticControl(dwIndex,67,72,47,8,"Z");
	AddDialogStaticControl(dwIndex,67,94,47,8,"Scale");
	AddDialogEditControl(dwIndex,67,38, 47,10,EDT_RIGHTX);
	AddDialogEditControl(dwIndex,67,60, 47,10,EDT_RIGHTY);
	AddDialogEditControl(dwIndex,67,82, 47,10,EDT_RIGHTZ);
	AddDialogEditControl(dwIndex,67,104,47,10,EDT_SCALEX);
	AddDialogPushButton(dwIndex,67,118,22,16,"Pitch",BTN_PITCH);
	AddDialogPushButton(dwIndex,93,118,21,16,"Scale",BTN_SCALEX);

	AddDialogGroupBox(dwIndex,122,18,55,120,"Up");
	AddDialogStaticControl(dwIndex,126,28,47,8,"X");
	AddDialogStaticControl(dwIndex,126,50,47,8,"Y");
	AddDialogStaticControl(dwIndex,126,72,47,8,"Z");
	AddDialogStaticControl(dwIndex,126,94,47,8,"Scale");
	AddDialogEditControl(dwIndex,126,38, 47,10,EDT_UPX);
	AddDialogEditControl(dwIndex,126,60, 47,10,EDT_UPY);
	AddDialogEditControl(dwIndex,126,82, 47,10,EDT_UPZ);
	AddDialogEditControl(dwIndex,126,104,47,10,EDT_SCALEY);
	AddDialogPushButton(dwIndex,126,118,22,16,"Yaw",BTN_YAW);
	AddDialogPushButton(dwIndex,152,118,21,16,"Scale",BTN_SCALEY);

	AddDialogGroupBox(dwIndex,181,18,55,120,"Ahead");
	AddDialogStaticControl(dwIndex,185,28,47,8,"X");
	AddDialogStaticControl(dwIndex,185,50,47,8,"Y");
	AddDialogStaticControl(dwIndex,185,72,47,8,"Z");
	AddDialogStaticControl(dwIndex,185,94,47,8,"Scale");
	AddDialogEditControl(dwIndex,185,38, 47,10,EDT_AHEADX);
	AddDialogEditControl(dwIndex,185,60, 47,10,EDT_AHEADY);
	AddDialogEditControl(dwIndex,185,82, 47,10,EDT_AHEADZ);
	AddDialogEditControl(dwIndex,185,104,47,10,EDT_SCALEZ);
	AddDialogPushButton(dwIndex,185,118,22,16,"Roll",BTN_ROLL);
	AddDialogPushButton(dwIndex,211,118,21,16,"Scale",BTN_SCALEZ);

	AddDialogPushButton(dwIndex,63, 144,55,22,"OK",   BTN_OK,TRUE);
	AddDialogPushButton(dwIndex,122,144,55,22,"Canel",BTN_CANCEL);
	AddDialogPushButton(dwIndex,181,144,55,22,"Save", BTN_SAVE);

	// return index
	return(dwIndex);
}

// DialogSWMeshProc()

BOOL APIENTRY DialogSWMeshProc(HWND hDlg,
						       UINT Msg,
						       WPARAM wParam,
						       LPARAM lParam)
{
	// counter
	DWORD dwCount;

	// check message
	switch(Msg)
	{
	case(WM_INITDIALOG):
		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// init controls
		SetCurrentDialogEditText(EDT_NAME,"");
		SetCurrentDialogEditFloat(EDT_POSX,0);
		SetCurrentDialogEditFloat(EDT_POSY,0);
		SetCurrentDialogEditFloat(EDT_POSZ,0);
		SetCurrentDialogEditFloat(EDT_RIGHTX,1);
		SetCurrentDialogEditFloat(EDT_RIGHTY,0);
		SetCurrentDialogEditFloat(EDT_RIGHTZ,0);
		SetCurrentDialogEditFloat(EDT_UPX,0);
		SetCurrentDialogEditFloat(EDT_UPY,1);
		SetCurrentDialogEditFloat(EDT_UPZ,0);
		SetCurrentDialogEditFloat(EDT_AHEADX,0);
		SetCurrentDialogEditFloat(EDT_AHEADY,0);
		SetCurrentDialogEditFloat(EDT_AHEADZ,1);
		SetCurrentDialogEditFloat(EDT_SCALEX,1);
		SetCurrentDialogEditFloat(EDT_SCALEY,1);
		SetCurrentDialogEditFloat(EDT_SCALEZ,1);

		// reset list boxes
		ResetCurrentDialogDropDownSelections(LST_NAME);
		ResetCurrentDialogDropDownSelections(LST_MESH);
		
		// init static world mesh list box
		for(dwCount=0;dwCount<g_dwNumSWMesh;dwCount++)
		{
			// cast pointer to static world mesh name
			LPSTR lpName=g_lplpSWMeshNames[dwCount];
			
			// add static world mesh to list if necessary
			if(lpName[0]!='~')
				AddCurrentDialogDropDownSelection(LST_NAME,
												  lpName);
		}
		
		// init mesh list box
		for(dwCount=0;dwCount<g_dwNumMesh;dwCount++)
		{
			// cast pointer to mesh name
			LPSTR lpName=g_lplpMeshNames[dwCount];
			
			// add mesh to list if necessary
			if(lpName[0]!='~')
				AddCurrentDialogDropDownSelection(LST_MESH,
												  lpName);
		}
		
		// highlight previous static world mesh
		if(g_dwLastSWMesh<g_dwNumSWMesh)
			HighlightCurrentDialogDropDownSelection(LST_NAME,
													g_lplpSWMeshNames[g_dwLastSWMesh]);
		
		// force info to load
		goto SWMeshSelChange;

	case(WM_COMMAND):
		// check notification message
		switch(HIWORD(wParam))
		{
		case(BN_CLICKED):
			// check for ok button
			if(LOWORD(wParam)==BTN_OK)
			{
				// send save message
				SendMessage(hDlg,
							WM_COMMAND,
							(WPARAM)((((DWORD)BN_CLICKED)<<16)+(DWORD)BTN_SAVE),
							NULL);

				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update environment
				UpdateWorld();
				
				// ok
				return(TRUE);
			}

			// check for cancel button
			if(LOWORD(wParam)==BTN_CANCEL)
			{
				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update world
				UpdateWorld();

				// ok
				return(TRUE);
			}
			
			// check for add button
			if(LOWORD(wParam)==BTN_ADD)
			{
				LockGlobalMsg()

				// get name to add
				GetCurrentDialogEditText(EDT_NAME,
										 g_lpMsg,
										 _MAX_PATH);

				// check name
				if(strlen(g_lpMsg)==0)
					DisplayInfoMessage("You must specify a name to add.");
				else if(g_lpMsg[0]=='~')
					DisplayInfoMessage("Specified name is invalid.");
				else if(FindStaticWorldMesh(g_lpMsg)!=NO_OBJECT)
					DisplayInfoMessage("Specified static world mesh already exists.");
				else
				{
					// allocate name buffer
					LPSTR lpName;
					if(!AllocMem((LPVOID*)&lpName,
								 strlen(g_lpMsg)+1))
					{
						UnlockGlobalMsg()
						return(TRUE);
					}

					// copy name
					strcpy(lpName,
						   g_lpMsg);

					// get mesh
					if(!GetCurrentDialogDropDownSelection(LST_MESH,g_lpMsg))
					{
						DisplayInfoMessage("You must select a mesh.");
						FreeMem((LPVOID*)&lpName);
						UnlockGlobalMsg()
						return(TRUE);
					}
					DWORD dwMeshIndex=FindMesh(g_lpMsg);

					// check mesh
					if(dwMeshIndex<g_dwNumMesh)
					{
						// prepare matrix
						D3DXMATRIX mtx;
						mtx._11=GetCurrentDialogEditFloat(EDT_RIGHTX)*GetCurrentDialogEditFloat(EDT_SCALEX);
						mtx._12=GetCurrentDialogEditFloat(EDT_RIGHTY)*GetCurrentDialogEditFloat(EDT_SCALEX);
						mtx._13=GetCurrentDialogEditFloat(EDT_RIGHTZ)*GetCurrentDialogEditFloat(EDT_SCALEX);
						mtx._14=0;
						mtx._21=GetCurrentDialogEditFloat(EDT_UPX)*   GetCurrentDialogEditFloat(EDT_SCALEY);
						mtx._22=GetCurrentDialogEditFloat(EDT_UPY)*   GetCurrentDialogEditFloat(EDT_SCALEY);
						mtx._23=GetCurrentDialogEditFloat(EDT_UPZ)*   GetCurrentDialogEditFloat(EDT_SCALEY);
						mtx._24=0;
						mtx._31=GetCurrentDialogEditFloat(EDT_AHEADX)*GetCurrentDialogEditFloat(EDT_SCALEZ);
						mtx._32=GetCurrentDialogEditFloat(EDT_AHEADY)*GetCurrentDialogEditFloat(EDT_SCALEZ);
						mtx._33=GetCurrentDialogEditFloat(EDT_AHEADZ)*GetCurrentDialogEditFloat(EDT_SCALEZ);
						mtx._34=0;
						mtx._41=GetCurrentDialogEditFloat(EDT_POSX);
						mtx._42=GetCurrentDialogEditFloat(EDT_POSY);
						mtx._43=GetCurrentDialogEditFloat(EDT_POSZ);
						mtx._44=1;

						// add static world mesh
						DWORD dwIndex=AddStaticWorldMesh(dwMeshIndex,
													     &mtx,
														 lpName);

						// check static world mesh
						if(dwIndex<g_dwNumSWMesh)
						{
							// add name to list
							AddCurrentDialogDropDownSelection(LST_NAME,
															  lpName);

							// highlight name
							HighlightCurrentDialogDropDownSelection(LST_NAME,
																	lpName);
						}

						// free name buffer
						FreeMem((LPVOID*)&lpName);

						// remove name
						SetCurrentDialogEditText(EDT_NAME,
												 "");
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for delete button
			if(LOWORD(wParam)==BTN_DELETE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// eliminate static world mesh
						LPSTR lpName=g_lplpSWMeshNames[dwIndex];
						lpName[0]='~';

						// delete name from list
						RemoveCurrentDialogDropDownSelection(LST_NAME);
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for save button
			if(LOWORD(wParam)==BTN_SAVE)
			{
				LockGlobalMsg()
					
				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// get mesh
						if(!GetCurrentDialogDropDownSelection(LST_MESH,g_lpMsg))
						{
							DisplayInfoMessage("You must select a mesh.");
							UnlockGlobalMsg()
							return(TRUE);
						}
						DWORD dwMeshIndex=FindMesh(g_lpMsg);

						// check mesh
						if(dwMeshIndex<g_dwNumMesh)
						{
							// prepare matrix
							D3DXMATRIX mtx;
							mtx._11=GetCurrentDialogEditFloat(EDT_RIGHTX)*GetCurrentDialogEditFloat(EDT_SCALEX);
							mtx._12=GetCurrentDialogEditFloat(EDT_RIGHTY)*GetCurrentDialogEditFloat(EDT_SCALEX);
							mtx._13=GetCurrentDialogEditFloat(EDT_RIGHTZ)*GetCurrentDialogEditFloat(EDT_SCALEX);
							mtx._14=0;
							mtx._21=GetCurrentDialogEditFloat(EDT_UPX)*   GetCurrentDialogEditFloat(EDT_SCALEY);
							mtx._22=GetCurrentDialogEditFloat(EDT_UPY)*   GetCurrentDialogEditFloat(EDT_SCALEY);
							mtx._23=GetCurrentDialogEditFloat(EDT_UPZ)*   GetCurrentDialogEditFloat(EDT_SCALEY);
							mtx._24=0;
							mtx._31=GetCurrentDialogEditFloat(EDT_AHEADX)*GetCurrentDialogEditFloat(EDT_SCALEZ);
							mtx._32=GetCurrentDialogEditFloat(EDT_AHEADY)*GetCurrentDialogEditFloat(EDT_SCALEZ);
							mtx._33=GetCurrentDialogEditFloat(EDT_AHEADZ)*GetCurrentDialogEditFloat(EDT_SCALEZ);
							mtx._34=0;
							mtx._41=GetCurrentDialogEditFloat(EDT_POSX);
							mtx._42=GetCurrentDialogEditFloat(EDT_POSY);
							mtx._43=GetCurrentDialogEditFloat(EDT_POSZ);
							mtx._44=1;

							// update matrix
							SetMatrix(g_lpdwSWMMtxIndex[dwIndex],
									  &mtx);

							// update mesh index
							g_lpdwSWMMeshIndex[dwIndex]=dwMeshIndex;
						}
					}

					UnlockGlobalMsg()
					
					// ok
					return(TRUE);

				}

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSX)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMPOSX);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSY)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMPOSY);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMPOSZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSXY)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMPOSXY);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSXZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMPOSXZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSYZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMPOSYZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_SCALE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMSCALE);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_YAW)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMYAW);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_PITCH)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMPITCH);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_ROLL)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMROLL);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_SCALEX)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMSCALEX);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_SCALEY)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMSCALEY);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_SCALEZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world mesh
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set current mesh
						g_dwCurSWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWMSCALEZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			// ok
			break;

		case(CBN_SELCHANGE):
			// check identifier
			if(LOWORD(wParam)==LST_NAME)
			{
				SWMeshSelChange:;

				LockGlobalMsg()

				// get selection text
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// replace index with static world mesh index
					DWORD dwIndex=FindStaticWorldMesh(g_lpMsg);

					// check static world mesh
					if(dwIndex<g_dwNumSWMesh)
					{
						// set last static world mesh
						g_dwLastSWMesh=dwIndex;
						
						// highlight mesh
						HighlightCurrentDialogDropDownSelection(LST_MESH,
																g_lplpMeshNames[g_lpdwSWMMeshIndex[dwIndex]]);

						// fill in values
						SetCurrentDialogEditFloat(EDT_RIGHTX,g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._11);
						SetCurrentDialogEditFloat(EDT_RIGHTY,g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._12);
						SetCurrentDialogEditFloat(EDT_RIGHTZ,g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._13);
						SetCurrentDialogEditFloat(EDT_UPX,   g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._21);
						SetCurrentDialogEditFloat(EDT_UPY,   g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._22);
						SetCurrentDialogEditFloat(EDT_UPZ,   g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._23);
						SetCurrentDialogEditFloat(EDT_AHEADX,g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._31);
						SetCurrentDialogEditFloat(EDT_AHEADY,g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._32);
						SetCurrentDialogEditFloat(EDT_AHEADZ,g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._33);
						SetCurrentDialogEditFloat(EDT_POSX,  g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._41);
						SetCurrentDialogEditFloat(EDT_POSY,  g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._42);
						SetCurrentDialogEditFloat(EDT_POSZ,  g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].mtx._43);
						SetCurrentDialogEditFloat(EDT_SCALEX,g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].fXScale);
						SetCurrentDialogEditFloat(EDT_SCALEY,g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].fYScale);
						SetCurrentDialogEditFloat(EDT_SCALEZ,g_lpMtx[g_lpdwSWMMtxIndex[dwIndex]].fZScale);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}
		
		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}

// InitDialogDWMesh()

DWORD InitDialogDWMesh(void)
{
	LoadingOutput("InitDialogDWMesh");

	// get dialog index
	DWORD dwIndex=AddDialog(240,170,
							"Dynamic Meshes");

	// add controls
	AddDialogEditControl(dwIndex,4,5,72,10,EDT_NAME);
	AddDialogDropDownListBox(dwIndex,122,4,72,64,LST_NAME);
	AddDialogPushButton(dwIndex,80, 5,38,10,"Add",   BTN_ADD);
	AddDialogPushButton(dwIndex,198,5,38,10,"Delete",BTN_DELETE);

	AddDialogGroupBox(dwIndex,4,18,55,120,"Position");
	AddDialogStaticControl(dwIndex,8,28,47,8,"X");
	AddDialogStaticControl(dwIndex,8,50,47,8,"Y");
	AddDialogStaticControl(dwIndex,8,72,47,8,"Z");
	AddDialogEditControl(dwIndex,8,38, 47,10,EDT_POSX);
	AddDialogEditControl(dwIndex,8,60, 47,10,EDT_POSY);
	AddDialogEditControl(dwIndex,8,82, 47,10,EDT_POSZ);
	AddDialogPushButton(dwIndex,8, 96,13,10,"X",BTN_POSX);
	AddDialogPushButton(dwIndex,25,96,13,10,"Y",BTN_POSY);
	AddDialogPushButton(dwIndex,42,96,13,10,"Z",BTN_POSZ);
	AddDialogPushButton(dwIndex,8, 110,13,10,"XY",BTN_POSXY);
	AddDialogPushButton(dwIndex,25,110,13,10,"XZ",BTN_POSXZ);
	AddDialogPushButton(dwIndex,42,110,13,10,"YZ",BTN_POSYZ);
	AddDialogPushButton(dwIndex,8,124,47,10,"Scale",BTN_SCALE);

	AddDialogGroupBox(dwIndex,4,140,55,26,"Mesh");
	AddDialogDropDownListBox(dwIndex,8,150,47,64,LST_MESH);

	AddDialogGroupBox(dwIndex,63,18,55,120,"Right");
	AddDialogStaticControl(dwIndex,67,28,47,8,"X");
	AddDialogStaticControl(dwIndex,67,50,47,8,"Y");
	AddDialogStaticControl(dwIndex,67,72,47,8,"Z");
	AddDialogStaticControl(dwIndex,67,94,47,8,"Scale");
	AddDialogEditControl(dwIndex,67,38, 47,10,EDT_RIGHTX);
	AddDialogEditControl(dwIndex,67,60, 47,10,EDT_RIGHTY);
	AddDialogEditControl(dwIndex,67,82, 47,10,EDT_RIGHTZ);
	AddDialogEditControl(dwIndex,67,104,47,10,EDT_SCALEX);
	AddDialogPushButton(dwIndex,67,118,22,16,"Pitch",BTN_PITCH);
	AddDialogPushButton(dwIndex,93,118,21,16,"Scale",BTN_SCALEX);

	AddDialogGroupBox(dwIndex,122,18,55,120,"Up");
	AddDialogStaticControl(dwIndex,126,28,47,8,"X");
	AddDialogStaticControl(dwIndex,126,50,47,8,"Y");
	AddDialogStaticControl(dwIndex,126,72,47,8,"Z");
	AddDialogStaticControl(dwIndex,126,94,47,8,"Scale");
	AddDialogEditControl(dwIndex,126,38, 47,10,EDT_UPX);
	AddDialogEditControl(dwIndex,126,60, 47,10,EDT_UPY);
	AddDialogEditControl(dwIndex,126,82, 47,10,EDT_UPZ);
	AddDialogEditControl(dwIndex,126,104,47,10,EDT_SCALEY);
	AddDialogPushButton(dwIndex,126,118,22,16,"Yaw",BTN_YAW);
	AddDialogPushButton(dwIndex,152,118,21,16,"Scale",BTN_SCALEY);

	AddDialogGroupBox(dwIndex,181,18,55,120,"Ahead");
	AddDialogStaticControl(dwIndex,185,28,47,8,"X");
	AddDialogStaticControl(dwIndex,185,50,47,8,"Y");
	AddDialogStaticControl(dwIndex,185,72,47,8,"Z");
	AddDialogStaticControl(dwIndex,185,94,47,8,"Scale");
	AddDialogEditControl(dwIndex,185,38, 47,10,EDT_AHEADX);
	AddDialogEditControl(dwIndex,185,60, 47,10,EDT_AHEADY);
	AddDialogEditControl(dwIndex,185,82, 47,10,EDT_AHEADZ);
	AddDialogEditControl(dwIndex,185,104,47,10,EDT_SCALEZ);
	AddDialogPushButton(dwIndex,185,118,22,16,"Roll",BTN_ROLL);
	AddDialogPushButton(dwIndex,211,118,21,16,"Scale",BTN_SCALEZ);

	AddDialogPushButton(dwIndex,63, 144,55,22,"OK",   BTN_OK,TRUE);
	AddDialogPushButton(dwIndex,122,144,55,22,"Canel",BTN_CANCEL);
	AddDialogPushButton(dwIndex,181,144,55,22,"Save", BTN_SAVE);

	// return index
	return(dwIndex);
}

// DialogDWMeshProc()

BOOL APIENTRY DialogDWMeshProc(HWND hDlg,
						       UINT Msg,
						       WPARAM wParam,
						       LPARAM lParam)
{
	// counter
	DWORD dwCount;

	// check message
	switch(Msg)
	{
	case(WM_INITDIALOG):
		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// init controls
		SetCurrentDialogEditText(EDT_NAME,"");
		SetCurrentDialogEditFloat(EDT_POSX,0);
		SetCurrentDialogEditFloat(EDT_POSY,0);
		SetCurrentDialogEditFloat(EDT_POSZ,0);
		SetCurrentDialogEditFloat(EDT_RIGHTX,1);
		SetCurrentDialogEditFloat(EDT_RIGHTY,0);
		SetCurrentDialogEditFloat(EDT_RIGHTZ,0);
		SetCurrentDialogEditFloat(EDT_UPX,0);
		SetCurrentDialogEditFloat(EDT_UPY,1);
		SetCurrentDialogEditFloat(EDT_UPZ,0);
		SetCurrentDialogEditFloat(EDT_AHEADX,0);
		SetCurrentDialogEditFloat(EDT_AHEADY,0);
		SetCurrentDialogEditFloat(EDT_AHEADZ,1);
		SetCurrentDialogEditFloat(EDT_SCALEX,1);
		SetCurrentDialogEditFloat(EDT_SCALEY,1);
		SetCurrentDialogEditFloat(EDT_SCALEZ,1);

		// reset list boxes
		ResetCurrentDialogDropDownSelections(LST_NAME);
		ResetCurrentDialogDropDownSelections(LST_MESH);
		
		// init dynamic world mesh list box
		for(dwCount=0;dwCount<g_dwNumDWMesh;dwCount++)
		{
			// cast pointer to dynamic world mesh name
			LPSTR lpName=g_lplpDWMeshNames[dwCount];
			
			// add dynamic world mesh to list if necessary
			if(lpName[0]!='~')
				AddCurrentDialogDropDownSelection(LST_NAME,
												  lpName);
		}
		
		// init mesh list box
		for(dwCount=0;dwCount<g_dwNumMesh;dwCount++)
		{
			// cast pointer to mesh name
			LPSTR lpName=g_lplpMeshNames[dwCount];
			
			// add mesh to list if necessary
			if(lpName[0]!='~')
				AddCurrentDialogDropDownSelection(LST_MESH,
												  lpName);
		}
		
		// highlight previous dynamic world mesh
		if(g_dwLastDWMesh<g_dwNumDWMesh)
			HighlightCurrentDialogDropDownSelection(LST_NAME,
													g_lplpDWMeshNames[g_dwLastDWMesh]);
		
		// force info to load
		goto DWMeshSelChange;

	case(WM_COMMAND):
		// check notification message
		switch(HIWORD(wParam))
		{
		case(BN_CLICKED):
			// check for ok button
			if(LOWORD(wParam)==BTN_OK)
			{
				// send save message
				SendMessage(hDlg,
							WM_COMMAND,
							(WPARAM)((((DWORD)BN_CLICKED)<<16)+(DWORD)BTN_SAVE),
							NULL);

				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update environment
				UpdateWorld();
				
				// ok
				return(TRUE);
			}

			// check for cancel button
			if(LOWORD(wParam)==BTN_CANCEL)
			{
				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update world
				UpdateWorld();

				// ok
				return(TRUE);
			}
			
			// check for add button
			if(LOWORD(wParam)==BTN_ADD)
			{
				LockGlobalMsg()

				// get name to add
				GetCurrentDialogEditText(EDT_NAME,
										 g_lpMsg,
										 _MAX_PATH);

				// check name
				if(strlen(g_lpMsg)==0)
					DisplayInfoMessage("You must specify a name to add.");
				else if(g_lpMsg[0]=='~')
					DisplayInfoMessage("Specified name is invalid.");
				else if(FindDynamicWorldMesh(g_lpMsg)!=NO_OBJECT)
					DisplayInfoMessage("Specified dynamic world mesh already exists.");
				else
				{
					// allocate name buffer
					LPSTR lpName;
					if(!AllocMem((LPVOID*)&lpName,
								 strlen(g_lpMsg)+1))
					{
						UnlockGlobalMsg()
						return(TRUE);
					}

					// copy name
					strcpy(lpName,
						   g_lpMsg);

					// get mesh
					if(!GetCurrentDialogDropDownSelection(LST_MESH,g_lpMsg))
					{
						DisplayInfoMessage("You must select a mesh.");
						FreeMem((LPVOID*)&lpName);
						UnlockGlobalMsg()
						return(TRUE);
					}
					DWORD dwMeshIndex=FindMesh(g_lpMsg);

					// check mesh
					if(dwMeshIndex<g_dwNumMesh)
					{
						// prepare matrix
						D3DXMATRIX mtx;
						mtx._11=GetCurrentDialogEditFloat(EDT_RIGHTX)*GetCurrentDialogEditFloat(EDT_SCALEX);
						mtx._12=GetCurrentDialogEditFloat(EDT_RIGHTY)*GetCurrentDialogEditFloat(EDT_SCALEX);
						mtx._13=GetCurrentDialogEditFloat(EDT_RIGHTZ)*GetCurrentDialogEditFloat(EDT_SCALEX);
						mtx._14=0;
						mtx._21=GetCurrentDialogEditFloat(EDT_UPX)*   GetCurrentDialogEditFloat(EDT_SCALEY);
						mtx._22=GetCurrentDialogEditFloat(EDT_UPY)*   GetCurrentDialogEditFloat(EDT_SCALEY);
						mtx._23=GetCurrentDialogEditFloat(EDT_UPZ)*   GetCurrentDialogEditFloat(EDT_SCALEY);
						mtx._24=0;
						mtx._31=GetCurrentDialogEditFloat(EDT_AHEADX)*GetCurrentDialogEditFloat(EDT_SCALEZ);
						mtx._32=GetCurrentDialogEditFloat(EDT_AHEADY)*GetCurrentDialogEditFloat(EDT_SCALEZ);
						mtx._33=GetCurrentDialogEditFloat(EDT_AHEADZ)*GetCurrentDialogEditFloat(EDT_SCALEZ);
						mtx._34=0;
						mtx._41=GetCurrentDialogEditFloat(EDT_POSX);
						mtx._42=GetCurrentDialogEditFloat(EDT_POSY);
						mtx._43=GetCurrentDialogEditFloat(EDT_POSZ);
						mtx._44=1;

						// add dynamic world mesh
						DWORD dwIndex=AddDynamicWorldMesh(dwMeshIndex,
													      &mtx,
														  lpName);

						// check dynamic world mesh
						if(dwIndex<g_dwNumDWMesh)
						{
							// add name to list
							AddCurrentDialogDropDownSelection(LST_NAME,
															  lpName);

							// highlight name
							HighlightCurrentDialogDropDownSelection(LST_NAME,
																	lpName);
						}

						// free name buffer
						FreeMem((LPVOID*)&lpName);

						// remove name
						SetCurrentDialogEditText(EDT_NAME,
												 "");
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for delete button
			if(LOWORD(wParam)==BTN_DELETE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// eliminate dynamic world mesh
						LPSTR lpName=g_lplpDWMeshNames[dwIndex];
						lpName[0]='~';

						// delete name from list
						RemoveCurrentDialogDropDownSelection(LST_NAME);
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for save button
			if(LOWORD(wParam)==BTN_SAVE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// get mesh
						if(!GetCurrentDialogDropDownSelection(LST_MESH,g_lpMsg))
						{
							DisplayInfoMessage("You must select a mesh.");
							UnlockGlobalMsg()
							return(TRUE);
						}
						DWORD dwMeshIndex=FindMesh(g_lpMsg);

						// check mesh
						if(dwMeshIndex<g_dwNumMesh)
						{
							// prepare matrix
							D3DXMATRIX mtx;
							mtx._11=GetCurrentDialogEditFloat(EDT_RIGHTX)*GetCurrentDialogEditFloat(EDT_SCALEX);
							mtx._12=GetCurrentDialogEditFloat(EDT_RIGHTY)*GetCurrentDialogEditFloat(EDT_SCALEX);
							mtx._13=GetCurrentDialogEditFloat(EDT_RIGHTZ)*GetCurrentDialogEditFloat(EDT_SCALEX);
							mtx._14=0;
							mtx._21=GetCurrentDialogEditFloat(EDT_UPX)*   GetCurrentDialogEditFloat(EDT_SCALEY);
							mtx._22=GetCurrentDialogEditFloat(EDT_UPY)*   GetCurrentDialogEditFloat(EDT_SCALEY);
							mtx._23=GetCurrentDialogEditFloat(EDT_UPZ)*   GetCurrentDialogEditFloat(EDT_SCALEY);
							mtx._24=0;
							mtx._31=GetCurrentDialogEditFloat(EDT_AHEADX)*GetCurrentDialogEditFloat(EDT_SCALEZ);
							mtx._32=GetCurrentDialogEditFloat(EDT_AHEADY)*GetCurrentDialogEditFloat(EDT_SCALEZ);
							mtx._33=GetCurrentDialogEditFloat(EDT_AHEADZ)*GetCurrentDialogEditFloat(EDT_SCALEZ);
							mtx._34=0;
							mtx._41=GetCurrentDialogEditFloat(EDT_POSX);
							mtx._42=GetCurrentDialogEditFloat(EDT_POSY);
							mtx._43=GetCurrentDialogEditFloat(EDT_POSZ);
							mtx._44=1;

							// update matrix
							SetMatrix(g_lpdwDWMMtxIndex[dwIndex],
									  &mtx);

							// update mesh index
							g_lpdwDWMMeshIndex[dwIndex]=dwMeshIndex;
						}
					}

					UnlockGlobalMsg()
					
					// ok
					return(TRUE);

				}

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSX)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMPOSX);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSY)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMPOSY);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMPOSZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSXY)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMPOSXY);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSXZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMPOSXZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSYZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMPOSYZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_SCALE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMSCALE);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_YAW)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMYAW);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_PITCH)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMPITCH);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_ROLL)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMROLL);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_SCALEX)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMSCALEX);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_SCALEY)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMSCALEY);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_SCALEZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world mesh
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set current mesh
						g_dwCurDWMesh=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWMSCALEZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			// ok
			break;

		case(CBN_SELCHANGE):
			// check identifier
			if(LOWORD(wParam)==LST_NAME)
			{
				DWMeshSelChange:;

				LockGlobalMsg()

				// get selection text
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// replace index with dynamic world mesh index
					DWORD dwIndex=FindDynamicWorldMesh(g_lpMsg);

					// check dynamic world mesh
					if(dwIndex<g_dwNumDWMesh)
					{
						// set last dynamic world mesh
						g_dwLastDWMesh=dwIndex;
						
						// highlight mesh
						HighlightCurrentDialogDropDownSelection(LST_MESH,
																g_lplpMeshNames[g_lpdwDWMMeshIndex[dwIndex]]);

						// fill in values
						SetCurrentDialogEditFloat(EDT_RIGHTX,g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._11);
						SetCurrentDialogEditFloat(EDT_RIGHTY,g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._12);
						SetCurrentDialogEditFloat(EDT_RIGHTZ,g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._13);
						SetCurrentDialogEditFloat(EDT_UPX,   g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._21);
						SetCurrentDialogEditFloat(EDT_UPY,   g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._22);
						SetCurrentDialogEditFloat(EDT_UPZ,   g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._23);
						SetCurrentDialogEditFloat(EDT_AHEADX,g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._31);
						SetCurrentDialogEditFloat(EDT_AHEADY,g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._32);
						SetCurrentDialogEditFloat(EDT_AHEADZ,g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._33);
						SetCurrentDialogEditFloat(EDT_POSX,  g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._41);
						SetCurrentDialogEditFloat(EDT_POSY,  g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._42);
						SetCurrentDialogEditFloat(EDT_POSZ,  g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].mtx._43);
						SetCurrentDialogEditFloat(EDT_SCALEX,g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].fXScale);
						SetCurrentDialogEditFloat(EDT_SCALEY,g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].fYScale);
						SetCurrentDialogEditFloat(EDT_SCALEZ,g_lpMtx[g_lpdwDWMMtxIndex[dwIndex]].fZScale);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}
		
		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}
#endif

// SetGravityVector()

void SetGravityVector(LPD3DXVECTOR3 lpv)
{
	// copy vector
	g_vGravity=(*lpv);
}

// SetGravityVector()

void SetGravityVector(float fX,
					  float fY,
					  float fZ)
{
	// copy values
	g_vGravity.x=fX;
	g_vGravity.y=fY;
	g_vGravity.z=fZ;
}

// GetNextStaticTriangle()

DWORD GetNextStaticTriangle(void)
{
	LoadingOutput("GetNextStaticTriangle");

	// check counter
	if(g_dwNumSTri==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpvSTriVtx1,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpvSTriVtx2,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpvSTriVtx3,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpvSTriNormal,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwSTriIndexed,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxSTri=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumSTri>=g_dwMaxSTri)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpvSTriVtx1,
					  sizeof(D3DXVECTOR3)*g_dwMaxSTri,
					  sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpvSTriVtx2,
					  sizeof(D3DXVECTOR3)*g_dwMaxSTri,
					  sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpvSTriVtx3,
					  sizeof(D3DXVECTOR3)*g_dwMaxSTri,
					  sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpvSTriNormal,
					  sizeof(D3DXVECTOR3)*g_dwMaxSTri,
					  sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwSTriIndexed,
					  sizeof(DWORD)*g_dwMaxSTri,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxSTri+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumSTri;

	// increment counter
	g_dwNumSTri++;

	// return index
	return(dwIndex);
}

// AddStaticTriangle()

DWORD AddStaticTriangle(LPD3DXVECTOR3 lpv1,
						LPD3DXVECTOR3 lpv2,
						LPD3DXVECTOR3 lpv3)
{
	LoadingOutput("AddStaticTriangle");

	// eliminate degenerate triangles
	if(IsNearEqual(lpv1->x,lpv2->x)&&
	   IsNearEqual(lpv1->y,lpv2->y)&&
	   IsNearEqual(lpv1->z,lpv2->z))
		return(NO_OBJECT);
	if(IsNearEqual(lpv2->x,lpv3->x)&&
	   IsNearEqual(lpv2->y,lpv3->y)&&
	   IsNearEqual(lpv2->z,lpv3->z))
		return(NO_OBJECT);
	if(IsNearEqual(lpv3->x,lpv1->x)&&
	   IsNearEqual(lpv3->y,lpv1->y)&&
	   IsNearEqual(lpv3->z,lpv1->z))
		return(NO_OBJECT);
	
	// get return index
	DWORD dwIndex=GetNextStaticTriangle();

	// copy vertices
	g_lpvSTriVtx1[dwIndex]=(*lpv1);
	g_lpvSTriVtx2[dwIndex]=(*lpv2);
	g_lpvSTriVtx3[dwIndex]=(*lpv3);

	// compute normal
	GetPlaneNormal(&g_lpvSTriNormal[dwIndex],
				   lpv1,
				   lpv2,
				   lpv3);

	// reset indexed flag
	g_lpdwSTriIndexed[dwIndex]=g_dwColIndexNum;

	// return index
	return(dwIndex);
}

// PurgeStaticTriangles()

void PurgeStaticTriangles(void)
{
	// check counter
	if(g_dwNumSTri)
	{
		// counter
		DWORD dwCount;

		// free triangle index lists
		for(dwCount=0;dwCount<g_dwNumSTriSegBlocks;dwCount++)
			FreeMem((LPVOID*)&g_lpSTriSegBlocks[dwCount].lpdwTriIndexList);
	}

	// free memory
	FreeMem((LPVOID*)&g_lpvSTriVtx1);
	FreeMem((LPVOID*)&g_lpvSTriVtx2);
	FreeMem((LPVOID*)&g_lpvSTriVtx3);
	FreeMem((LPVOID*)&g_lpvSTriNormal);
	FreeMem((LPVOID*)&g_lpdwSTriIndexed);
	FreeMem((LPVOID*)&g_lpSTriSegBlocks);
	FreeMem((LPVOID*)&g_lpdwSTriIndexList);

	// reset counters
	g_dwNumSTri=0;
	g_dwMaxSTri=0;

	// reset index counter
	g_dwColIndexNum=NO_OBJECT;
}

#ifdef DEBUG_MODE
// RenderStaticTriangle()

void RenderStaticTriangle(DWORD dwIndex,
						  BOOL bFace,
						  BOOL bNormal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumSTri)
	{
		DisplayFatalErrorMessage("Invalid static triangle index.",
								 "RenderStaticTriangle()",
								 FALSE);
		return;
	}
#endif

	// reset world matrix
	SetWorldMatrix();

	// render face
	if(bFace)
	{
		// render edges
		RenderTransformedLine(&g_lpvSTriVtx1[dwIndex],
							  &g_lpvSTriVtx2[dwIndex]);
		RenderTransformedLine(&g_lpvSTriVtx2[dwIndex],
							  &g_lpvSTriVtx3[dwIndex]);
		RenderTransformedLine(&g_lpvSTriVtx3[dwIndex],
							  &g_lpvSTriVtx1[dwIndex]);
	}
	
	// render normal
	if(bNormal)
	{
		// compute centroid
		D3DXVECTOR3 vVtx1=(g_lpvSTriVtx1[dwIndex]+
						   g_lpvSTriVtx2[dwIndex]+
						   g_lpvSTriVtx3[dwIndex])/3;

		// compute normal endpoint
		D3DXVECTOR3 vVtx2=vVtx1+g_lpvSTriNormal[dwIndex];

		// render normal
		RenderTransformedLine(&vVtx1,
							  &vVtx2);
	}
}

// RenderStaticTriangleBoundingBox()

void RenderStaticTriangleBoundingBox(void)
{
	// get vertices
	D3DXVECTOR3 v11=D3DXVECTOR3(g_vMinSTriExtents.x,
							    g_vMinSTriExtents.y,
							    g_vMinSTriExtents.z);
	D3DXVECTOR3 v12=D3DXVECTOR3(g_vMaxSTriExtents.x,
							    g_vMinSTriExtents.y,
							    g_vMinSTriExtents.z);
	D3DXVECTOR3 v13=D3DXVECTOR3(g_vMaxSTriExtents.x,
							    g_vMaxSTriExtents.y,
							    g_vMinSTriExtents.z);
	D3DXVECTOR3 v14=D3DXVECTOR3(g_vMinSTriExtents.x,
							    g_vMaxSTriExtents.y,
							    g_vMinSTriExtents.z);
	D3DXVECTOR3 v21=D3DXVECTOR3(g_vMinSTriExtents.x,
							    g_vMinSTriExtents.y,
							    g_vMaxSTriExtents.z);
	D3DXVECTOR3 v22=D3DXVECTOR3(g_vMaxSTriExtents.x,
							    g_vMinSTriExtents.y,
							    g_vMaxSTriExtents.z);
	D3DXVECTOR3 v23=D3DXVECTOR3(g_vMaxSTriExtents.x,
							    g_vMaxSTriExtents.y,
							    g_vMaxSTriExtents.z);
	D3DXVECTOR3 v24=D3DXVECTOR3(g_vMinSTriExtents.x,
							    g_vMaxSTriExtents.y,
							    g_vMaxSTriExtents.z);

	// reset world matrix
	SetWorldMatrix();

	// render bounding box
	RenderTransformedLine(&v11,&v12);
	RenderTransformedLine(&v12,&v13);
	RenderTransformedLine(&v13,&v14);
	RenderTransformedLine(&v14,&v11);
	RenderTransformedLine(&v21,&v22);
	RenderTransformedLine(&v22,&v23);
	RenderTransformedLine(&v23,&v24);
	RenderTransformedLine(&v24,&v21);
	RenderTransformedLine(&v11,&v21);
	RenderTransformedLine(&v12,&v22);
	RenderTransformedLine(&v13,&v23);
	RenderTransformedLine(&v14,&v24);
}

// RenderStaticTriangleSegmentBlocks()

void RenderStaticTriangleSegmentBlocks(void)
{
	// counters
	DWORD dwX;
	DWORD dwY;
	DWORD dwZ;

	// reset world matrix
	SetWorldMatrix();
	
	// render x-y segment grid
	for(dwX=0;dwX<=g_dwNumSTriSegX;dwX++)
		for(dwY=0;dwY<=g_dwNumSTriSegY;dwY++)
		{
			// compute endpoint 1
			D3DXVECTOR3 v1;
			v1.x=g_vMinSTriExtents.x+(g_vSTriSegSize.x*((float)dwX));
			v1.y=g_vMinSTriExtents.y+(g_vSTriSegSize.y*((float)dwY));
			v1.z=g_vMinSTriExtents.z;

			// compute endpoint 2
			D3DXVECTOR3 v2;
			v2.x=v1.x;
			v2.y=v1.y;
			v2.z=g_vMaxSTriExtents.z;

			// render line
			RenderTransformedLine(&v1,&v2);
		}

	// render x-z segment grid
	for(dwX=0;dwX<=g_dwNumSTriSegX;dwX++)
		for(dwZ=0;dwZ<=g_dwNumSTriSegZ;dwZ++)
		{
			// compute endpoint 1
			D3DXVECTOR3 v1;
			v1.x=g_vMinSTriExtents.x+(g_vSTriSegSize.x*((float)dwX));
			v1.y=g_vMinSTriExtents.y;
			v1.z=g_vMinSTriExtents.z+(g_vSTriSegSize.z*((float)dwZ));

			// compute endpoint 2
			D3DXVECTOR3 v2;
			v2.x=v1.x;
			v2.y=g_vMaxSTriExtents.y;
			v2.z=v1.z;

			// render line
			RenderTransformedLine(&v1,&v2);
		}

	// render y-z segment grid
	for(dwY=0;dwY<=g_dwNumSTriSegY;dwY++)
		for(dwZ=0;dwZ<=g_dwNumSTriSegZ;dwZ++)
		{
			// compute endpoint 1
			D3DXVECTOR3 v1;
			v1.x=g_vMinSTriExtents.x;
			v1.y=g_vMinSTriExtents.y+(g_vSTriSegSize.y*((float)dwY));
			v1.z=g_vMinSTriExtents.z+(g_vSTriSegSize.z*((float)dwZ));

			// compute endpoint 2
			D3DXVECTOR3 v2;
			v2.x=g_vMaxSTriExtents.x;
			v2.y=v1.y;
			v2.z=v1.z;

			// render line
			RenderTransformedLine(&v1,&v2);
		}
}

// SegmentStaticTriangles()

void SegmentStaticTriangles(void)
{
	LoadingOutput("SegmentStaticTriangles");

	// counters
	DWORD dwCount;
	DWORD dwX;
	DWORD dwY;
	DWORD dwZ;
	
	// reset extents to arbitrary point
	g_vMinSTriExtents=g_lpvSTriVtx1[0];
	g_vMaxSTriExtents=g_lpvSTriVtx1[0];

	// find extents of static triangles
	for(dwCount=0;dwCount<g_dwNumSTri;dwCount++)
	{
		// compare vertices with min-x extent
		if(IsVectorToLeft(&g_lpvSTriVtx1[dwCount],&g_vMinSTriExtents))
			g_vMinSTriExtents.x=g_lpvSTriVtx1[dwCount].x;
		if(IsVectorToLeft(&g_lpvSTriVtx2[dwCount],&g_vMinSTriExtents))
			g_vMinSTriExtents.x=g_lpvSTriVtx2[dwCount].x;
		if(IsVectorToLeft(&g_lpvSTriVtx3[dwCount],&g_vMinSTriExtents))
			g_vMinSTriExtents.x=g_lpvSTriVtx3[dwCount].x;

		// compare vertices with max-x extent
		if(IsVectorToRight(&g_lpvSTriVtx1[dwCount],&g_vMaxSTriExtents))
			g_vMaxSTriExtents.x=g_lpvSTriVtx1[dwCount].x;
		if(IsVectorToRight(&g_lpvSTriVtx2[dwCount],&g_vMaxSTriExtents))
			g_vMaxSTriExtents.x=g_lpvSTriVtx2[dwCount].x;
		if(IsVectorToRight(&g_lpvSTriVtx3[dwCount],&g_vMaxSTriExtents))
			g_vMaxSTriExtents.x=g_lpvSTriVtx3[dwCount].x;

		// compare vertices with min-y extent
		if(IsVectorBelow(&g_lpvSTriVtx1[dwCount],&g_vMinSTriExtents))
			g_vMinSTriExtents.y=g_lpvSTriVtx1[dwCount].y;
		if(IsVectorBelow(&g_lpvSTriVtx2[dwCount],&g_vMinSTriExtents))
			g_vMinSTriExtents.y=g_lpvSTriVtx2[dwCount].y;
		if(IsVectorBelow(&g_lpvSTriVtx3[dwCount],&g_vMinSTriExtents))
			g_vMinSTriExtents.y=g_lpvSTriVtx3[dwCount].y;

		// compare vertices with max-y extent
		if(IsVectorAbove(&g_lpvSTriVtx1[dwCount],&g_vMaxSTriExtents))
			g_vMaxSTriExtents.y=g_lpvSTriVtx1[dwCount].y;
		if(IsVectorAbove(&g_lpvSTriVtx2[dwCount],&g_vMaxSTriExtents))
			g_vMaxSTriExtents.y=g_lpvSTriVtx2[dwCount].y;
		if(IsVectorAbove(&g_lpvSTriVtx3[dwCount],&g_vMaxSTriExtents))
			g_vMaxSTriExtents.y=g_lpvSTriVtx3[dwCount].y;

		// compare vertices with min-z extent
		if(IsVectorBehind(&g_lpvSTriVtx1[dwCount],&g_vMinSTriExtents))
			g_vMinSTriExtents.z=g_lpvSTriVtx1[dwCount].z;
		if(IsVectorBehind(&g_lpvSTriVtx2[dwCount],&g_vMinSTriExtents))
			g_vMinSTriExtents.z=g_lpvSTriVtx2[dwCount].z;
		if(IsVectorBehind(&g_lpvSTriVtx3[dwCount],&g_vMinSTriExtents))
			g_vMinSTriExtents.z=g_lpvSTriVtx3[dwCount].z;

		// compare vertices with max-z extent
		if(IsVectorInFront(&g_lpvSTriVtx1[dwCount],&g_vMaxSTriExtents))
			g_vMaxSTriExtents.z=g_lpvSTriVtx1[dwCount].z;
		if(IsVectorInFront(&g_lpvSTriVtx2[dwCount],&g_vMaxSTriExtents))
			g_vMaxSTriExtents.z=g_lpvSTriVtx2[dwCount].z;
		if(IsVectorInFront(&g_lpvSTriVtx3[dwCount],&g_vMaxSTriExtents))
			g_vMaxSTriExtents.z=g_lpvSTriVtx3[dwCount].z;
	}

	// compute number of segment blocks
	g_dwNumSTriSegX=(DWORD)((g_vMaxSTriExtents.x-g_vMinSTriExtents.x)/g_fMinSTriSegSize);
	g_dwNumSTriSegY=(DWORD)((g_vMaxSTriExtents.y-g_vMinSTriExtents.y)/g_fMinSTriSegSize);
	g_dwNumSTriSegZ=(DWORD)((g_vMaxSTriExtents.z-g_vMinSTriExtents.z)/g_fMinSTriSegSize);

	// compute segment block multiplier
	g_dwNumSTriSegXY=g_dwNumSTriSegX*g_dwNumSTriSegY;

	// compute number of segment blocks -1
	g_nNumSTriSegX1=(int)(g_dwNumSTriSegX-1);
	g_nNumSTriSegY1=(int)(g_dwNumSTriSegY-1);
	g_nNumSTriSegZ1=(int)(g_dwNumSTriSegZ-1);

	// check for overflow
	if(g_dwNumSTriSegX>g_dwMaxSTriSegX)
		g_dwNumSTriSegX=g_dwMaxSTriSegX;
	if(g_dwNumSTriSegY>g_dwMaxSTriSegY)
		g_dwNumSTriSegY=g_dwMaxSTriSegY;
	if(g_dwNumSTriSegZ>g_dwMaxSTriSegZ)
		g_dwNumSTriSegZ=g_dwMaxSTriSegZ;

	// check for 0
	if(g_dwNumSTriSegX==0)
		g_dwNumSTriSegX=1;
	if(g_dwNumSTriSegY==0)
		g_dwNumSTriSegY=1;
	if(g_dwNumSTriSegZ==0)
		g_dwNumSTriSegZ=1;

	// compute total segment blocks
	g_dwNumSTriSegBlocks=g_dwNumSTriSegX*
						 g_dwNumSTriSegY*
						 g_dwNumSTriSegZ;

	// compute segment size vector
	g_vSTriSegSize.x=((g_vMaxSTriExtents.x-g_vMinSTriExtents.x)/((float)g_dwNumSTriSegX))+EPSILON;
	g_vSTriSegSize.y=((g_vMaxSTriExtents.y-g_vMinSTriExtents.y)/((float)g_dwNumSTriSegY))+EPSILON;
	g_vSTriSegSize.z=((g_vMaxSTriExtents.z-g_vMinSTriExtents.z)/((float)g_dwNumSTriSegZ))+EPSILON;

	// compute reciprocal of segment size
	g_vSTriSegASize.x=1/g_vSTriSegSize.x;
	g_vSTriSegASize.y=1/g_vSTriSegSize.y;
	g_vSTriSegASize.z=1/g_vSTriSegSize.z;

	// allocate segment block list
	if(!AllocMem((LPVOID*)&g_lpSTriSegBlocks,
				 sizeof(SEGMENTBLOCK)*g_dwNumSTriSegBlocks))
		return;

	// allocate triangle index list
	if(!AllocMem((LPVOID*)&g_lpdwSTriIndexList,
				 sizeof(DWORD)*g_dwNumSTri))
		return;

	// sort triangles into segments
	for(dwZ=0;dwZ<g_dwNumSTriSegZ;dwZ++)
		for(dwY=0;dwY<g_dwNumSTriSegY;dwY++)
			for(dwX=0;dwX<g_dwNumSTriSegX;dwX++)
			{
				// reset triangle counter
				DWORD dwNumTri=0;
				
				// compute segment bounding box
				D3DXVECTOR3 vMin;
				vMin.x=g_vMinSTriExtents.x+(g_vSTriSegSize.x*((float)dwX));
				vMin.y=g_vMinSTriExtents.y+(g_vSTriSegSize.y*((float)dwY));
				vMin.z=g_vMinSTriExtents.z+(g_vSTriSegSize.z*((float)dwZ));
				D3DXVECTOR3 vMax;
				vMax=vMin+g_vSTriSegSize;
				
				// check each triangle for intersection
				for(dwCount=0;dwCount<g_dwNumSTri;dwCount++)
					if(DoesTriangleIntersectBox(&g_lpvSTriVtx1[dwCount],
											    &g_lpvSTriVtx2[dwCount],
											    &g_lpvSTriVtx3[dwCount],
											    &vMin,
											    &vMax))
						g_lpdwSTriIndexList[dwNumTri++]=dwCount;

				// set pointer to block
				DWORD dwPos=dwX+
							(dwY*g_dwNumSTriSegX)+
							(dwZ*g_dwNumSTriSegX*g_dwNumSTriSegY);

				// copy number of intersections
				g_lpSTriSegBlocks[dwPos].dwNumTri=dwNumTri;

				// copy list if necessary
				if(dwNumTri)
				{
					// allocate memory for list
					if(!AllocMem((LPVOID*)&g_lpSTriSegBlocks[dwPos].lpdwTriIndexList,
								 sizeof(DWORD)*dwNumTri))
						return;

					// copy list
					CopyMemory(g_lpSTriSegBlocks[dwPos].lpdwTriIndexList,
							   g_lpdwSTriIndexList,
							   sizeof(DWORD)*dwNumTri);
				}
				else
				{
					// set list to null
					g_lpSTriSegBlocks[dwPos].lpdwTriIndexList=NULL;
				}
			}
}

// RenderStaticTriangleSegmentBlock()

void RenderStaticTriangleSegmentBlock(DWORD dwX,
									  DWORD dwY,
									  DWORD dwZ)
{
	// verify values
	if(dwX>=g_dwMaxSTriSegX||
	   dwY>=g_dwMaxSTriSegY||
	   dwZ>=g_dwMaxSTriSegZ)
	{
		DisplayFatalErrorMessage("Invalid segment value.",
								 "RenderStaticTriangleSegmentBlock()",
								 FALSE);
		return;
	}
	
	// counter
	DWORD dwCount;
	
	// compute segment index
	DWORD dwPos=dwX+
				(dwY*g_dwNumSTriSegX)+
				(dwZ*g_dwNumSTriSegX*g_dwNumSTriSegY);

	// render each face in block
	if(g_lpSTriSegBlocks[dwPos].dwNumTri)
		for(dwCount=0;dwCount<g_lpSTriSegBlocks[dwPos].dwNumTri;dwCount++)
			RenderStaticTriangle(g_lpSTriSegBlocks[dwPos].lpdwTriIndexList[dwCount]);
}
#endif

// GetNextStaticWorldObject()

DWORD GetNextStaticWorldObject(void)
{
	LoadingOutput("GetNextStaticWorldObject");

	// check counter
	if(g_dwNumSWObj==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwSWObjMtxIdx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpvSWObjRad,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpSWObjNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxSWObj=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumSWObj>=g_dwMaxSWObj)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwSWObjMtxIdx,
					  sizeof(DWORD)*g_dwMaxSWObj,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpvSWObjRad,
					  sizeof(D3DXVECTOR3)*g_dwMaxSWObj,
					  sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpSWObjNames,
					  sizeof(LPSTR)*g_dwMaxSWObj,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxSWObj+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumSWObj;

	// increment counter
	g_dwNumSWObj++;

	// return index
	return(dwIndex);
}

// AddStaticWorldObject()

DWORD AddStaticWorldObject(float fPosX,
						   float fPosY,
						   float fPosZ,
						   float fRadX,
						   float fRadY,
						   float fRadZ,
						   LPSTR lpName)
{
	LoadingOutput("AddStaticWorldObject");

	// prepare vectors
	D3DXVECTOR3 vPos=D3DXVECTOR3(fPosX,fPosY,fPosZ);
	D3DXVECTOR3 vRad=D3DXVECTOR3(fRadX,fRadY,fRadZ);

	// add object
	return(AddStaticWorldObject(&vPos,
								&vRad,
								lpName));
}

// AddStaticWorldObject()

DWORD AddStaticWorldObject(LPD3DXVECTOR3 lpvPos,
						   LPD3DXVECTOR3 lpvRad,
						   LPSTR lpName)
{
	LoadingOutput("AddStaticWorldObject");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~SO%d",
				g_dwNumSWObj);

		// add object
		DWORD dwRet=AddStaticWorldObject(lpvPos,
										 lpvRad,
										 g_lpMsg);

		// ok
		return(dwRet);
	}

	// get return index
	DWORD dwIndex=GetNextStaticWorldObject();

	// add matrix
	g_lpdwSWObjMtxIdx[dwIndex]=AddMatrix(1,0,0,0,
										 0,1,0,0,
										 0,0,1,0,
										 lpvPos->x,
										 lpvPos->y,
										 lpvPos->z,
										 1);
	
	// copy radius
	g_lpvSWObjRad[dwIndex]=(*lpvRad);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpSWObjNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpSWObjNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);
	
	// return index
	return(dwIndex);
}

// PurgeStaticWorldObjects()

void PurgeStaticWorldObjects()
{
	// check counter
	if(g_dwNumSWObj)
	{
		// counter
		DWORD dwCount;
		
		// release name buffers
		for(dwCount=0;dwCount<g_dwNumSWObj;dwCount++)
			FreeMem((LPVOID*)&g_lplpSWObjNames[dwCount]);
	}

	// free memory
	FreeMem((LPVOID*)&g_lpdwSWObjMtxIdx);
	FreeMem((LPVOID*)&g_lpvSWObjRad);
	FreeMem((LPVOID*)&g_lplpSWObjNames);

	// reset counters
	g_dwNumSWObj=0;
	g_dwMaxSWObj=0;
}

// FindStaticWorldObject()

DWORD FindStaticWorldObject(LPCSTR lpName)
{
	LoadingOutput("FindStaticWorldObject");

	// counter
	DWORD dwCount;

	// search static objects
	if(g_dwNumSWObj)
		for(dwCount=0;dwCount<g_dwNumSWObj;dwCount++)
			if(stricmp(lpName,g_lplpSWObjNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// GetNextDynamicWorldObject()

DWORD GetNextDynamicWorldObject(void)
{
	LoadingOutput("GetNextDynamicWorldObject");

	// check counter
	if(g_dwNumDWObj==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwDWObjMtxIdx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpvDWObjSafePos,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpvDWObjRad,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpvDWObjARad,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpvDWObjDisp,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbDWObjSTColl,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpvDWObjSTCollPt,
					 sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDWObjSTCollIdx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpDWObjNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDWObj=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumDWObj>=g_dwMaxDWObj)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwDWObjMtxIdx,
					  sizeof(DWORD)*g_dwMaxDWObj,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpvDWObjSafePos,
					  sizeof(D3DXVECTOR3)*g_dwMaxDWObj,
					  sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpvDWObjRad,
					  sizeof(D3DXVECTOR3)*g_dwMaxDWObj,
					  sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpvDWObjARad,
					  sizeof(D3DXVECTOR3)*g_dwMaxDWObj,
					  sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpvDWObjDisp,
					  sizeof(D3DXVECTOR3)*g_dwMaxDWObj,
					  sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbDWObjSTColl,
					  sizeof(BOOL)*g_dwMaxDWObj,
					  sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpvDWObjSTCollPt,
					  sizeof(D3DXVECTOR3)*g_dwMaxDWObj,
					  sizeof(D3DXVECTOR3)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDWObjSTCollIdx,
					  sizeof(DWORD)*g_dwMaxDWObj,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpDWObjNames,
					  sizeof(LPSTR)*g_dwMaxDWObj,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDWObj+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumDWObj;

	// increment counter
	g_dwNumDWObj++;

	// return index
	return(dwIndex);
}

// AddDynamicWorldObject()

DWORD AddDynamicWorldObject(float fPosX,
							float fPosY,
							float fPosZ,
							float fRadX,
							float fRadY,
							float fRadZ,
							float fAheadX,
							float fAheadY,
							float fAheadZ,
							LPSTR lpName)
{
	LoadingOutput("AddDynamicWorldObject");

	// prepare vectors
	D3DXVECTOR3 vPos=  D3DXVECTOR3(fPosX,  fPosY,  fPosZ);
	D3DXVECTOR3 vRad=  D3DXVECTOR3(fRadX,  fRadY,  fRadZ);
	D3DXVECTOR3 vAhead=D3DXVECTOR3(fAheadX,fAheadY,fAheadZ);

	// add object
	return(AddDynamicWorldObject(&vPos,
								 &vRad,
								 &vAhead,
								 lpName));
}

// AddDynamicWorldObject()

DWORD AddDynamicWorldObject(LPD3DXVECTOR3 lpvPos,
							LPD3DXVECTOR3 lpvRad,
							LPD3DXVECTOR3 lpvAhead,
							LPSTR lpName)
{
	LoadingOutput("AddDynamicWorldObject");

	// check name
	if(!lpName)
	{
		// just to verify
		LockGlobalMsg()
		UnlockGlobalMsg()

		// create name
		sprintf(g_lpMsg,
				"~DO%d",
				g_dwNumDWObj);

		// add object
		DWORD dwRet=AddDynamicWorldObject(lpvPos,
										  lpvRad,
										  lpvAhead,
										  g_lpMsg);

		// ok
		return(dwRet);
	}

	// get return index
	DWORD dwIndex=GetNextDynamicWorldObject();

	// temporary up vector
	D3DXVECTOR3 vUp=D3DXVECTOR3(0,1,0);
	
	// set ahead vector
	D3DXVECTOR3 vAhead;
	if(lpvAhead)
		vAhead=(*lpvAhead);
	else
		vAhead=D3DXVECTOR3(0,0,1);

	// compute right vector
	D3DXVECTOR3 vRight;
	CrossProduct(&vRight,
				 &vUp,
				 &vAhead);

	// create position matrix
	D3DXMATRIX mMtx=D3DXMATRIX(vRight.x, vRight.y, vRight.z, 0,
							   vUp.x,    vUp.y,    vUp.z,    0,
							   vAhead.x, vAhead.y, vAhead.z, 0,
							   lpvPos->x,lpvPos->y,lpvPos->z,1);

	// add matrix
	g_lpdwDWObjMtxIdx[dwIndex]=AddMatrix(&mMtx);

	// set radius
	g_lpvDWObjRad[dwIndex]=(*lpvRad);
	
	// reset values
	g_lpvDWObjSafePos[dwIndex]=(*lpvPos);
	g_lpvDWObjARad[dwIndex].x=1/g_lpvDWObjRad[dwIndex].x;
	g_lpvDWObjARad[dwIndex].y=1/g_lpvDWObjRad[dwIndex].y;
	g_lpvDWObjARad[dwIndex].z=1/g_lpvDWObjRad[dwIndex].z;
	g_lpvDWObjDisp[dwIndex]=D3DXVECTOR3(0,0,0);
	g_lpbDWObjSTColl[dwIndex]=FALSE;

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpDWObjNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpDWObjNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);
	
	// return index
	return(dwIndex);
}

// PurgeDynamicWorldObjects()

void PurgeDynamicWorldObjects()
{
	// check counter
	if(g_dwNumDWObj)
	{
		// counter
		DWORD dwCount;
		
		// release name buffers
		for(dwCount=0;dwCount<g_dwNumDWObj;dwCount++)
			FreeMem((LPVOID*)&g_lplpDWObjNames[dwCount]);
	}

	// free memory
	FreeMem((LPVOID*)&g_lpdwDWObjMtxIdx);
	FreeMem((LPVOID*)&g_lpvDWObjSafePos);
	FreeMem((LPVOID*)&g_lpvDWObjRad);
	FreeMem((LPVOID*)&g_lpvDWObjARad);
	FreeMem((LPVOID*)&g_lplpDWObjNames);
	FreeMem((LPVOID*)&g_lpvDWObjDisp);
	FreeMem((LPVOID*)&g_lpbDWObjSTColl);
	FreeMem((LPVOID*)&g_lpvDWObjSTCollPt);
	FreeMem((LPVOID*)&g_lpdwDWObjSTCollIdx);

	// reset counters
	g_dwNumDWObj=0;
	g_dwMaxDWObj=0;
}

// FindDynamicWorldObject()

DWORD FindDynamicWorldObject(LPCSTR lpName)
{
	LoadingOutput("FindDynamicWorldObject");

	// counter
	DWORD dwCount;

	// search dynamic objects
	if(g_dwNumDWObj)
		for(dwCount=0;dwCount<g_dwNumDWObj;dwCount++)
			if(stricmp(lpName,g_lplpDWObjNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// GetDynamicWorldObjectBoundingBox()

void GetDynamicWorldObjectBoundingBox(DWORD dwIndex,
									  LPD3DXVECTOR3 lpvMin,
									  LPD3DXVECTOR3 lpvMax)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDWObj)
	{
		DisplayFatalErrorMessage("Invalid dynamic world object index.",
								 "GetDynamicWorldObjectBoundingBox()",
								 FALSE);
		return;
	}
#endif

	// get object position
	float fXPos=g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._41;
	float fYPos=g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._42;
	float fZPos=g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._43;
	
	// compute x-values
	if(g_lpvDWObjDisp[dwIndex].x<0)
	{
		lpvMin->x=fXPos+
				  g_lpvDWObjDisp[dwIndex].x-
				  g_lpvDWObjRad[dwIndex].x;
		lpvMax->x=fXPos+
				  g_lpvDWObjRad[dwIndex].x;
	}
	else
	{
		lpvMin->x=fXPos-
				  g_lpvDWObjRad[dwIndex].x;
		lpvMax->x=fXPos+
				  g_lpvDWObjDisp[dwIndex].x+
				  g_lpvDWObjRad[dwIndex].x;
	}

	// compute y-values
	if(g_lpvDWObjDisp[dwIndex].y<0)
	{
		lpvMin->y=fYPos+
				  g_lpvDWObjDisp[dwIndex].y-
				  g_lpvDWObjRad[dwIndex].y;
		lpvMax->y=fYPos+
				  g_lpvDWObjRad[dwIndex].y;
	}
	else
	{
		lpvMin->y=fYPos-
				  g_lpvDWObjRad[dwIndex].y;
		lpvMax->y=fYPos+
				  g_lpvDWObjDisp[dwIndex].y+
				  g_lpvDWObjRad[dwIndex].y;
	}

	// compute z-values
	if(g_lpvDWObjDisp[dwIndex].z<0)
	{
		lpvMin->z=fZPos+
				  g_lpvDWObjDisp[dwIndex].z-
				  g_lpvDWObjRad[dwIndex].z;
		lpvMax->z=fZPos+
				  g_lpvDWObjRad[dwIndex].z;
	}
	else
	{
		lpvMin->z=fZPos-
				  g_lpvDWObjRad[dwIndex].z;
		lpvMax->z=fZPos+
				  g_lpvDWObjDisp[dwIndex].z+
				  g_lpvDWObjRad[dwIndex].z;
	}
}

#ifdef DEBUG_MODE
// RenderStaticWorldObject()

void RenderStaticWorldObject(DWORD dwIndex)
{
	// verify index
	if(dwIndex>=g_dwNumSWObj)
	{
		DisplayFatalErrorMessage("Invalid static world object index.",
								 "RenderStaticWorldObject()",
								 FALSE);
		return;
	}

	// get matrix index
	DWORD dwMtxIdx=g_lpdwSWObjMtxIdx[dwIndex];
	
	// save matrix scale factors
	float fXScale=g_lpMtx[dwMtxIdx].fXScale;
	float fYScale=g_lpMtx[dwMtxIdx].fYScale;
	float fZScale=g_lpMtx[dwMtxIdx].fZScale;

	// set scale factors
	g_lpMtx[dwMtxIdx].fXScale=g_lpvSWObjRad[dwIndex].x;
	g_lpMtx[dwMtxIdx].fYScale=g_lpvSWObjRad[dwIndex].y;
	g_lpMtx[dwMtxIdx].fZScale=g_lpvSWObjRad[dwIndex].z;

	// render mesh
	RenderMesh(g_dwObjectMeshIdx,
			   dwMtxIdx);
	
	// restore scale factors
	g_lpMtx[dwMtxIdx].fXScale=fXScale;
	g_lpMtx[dwMtxIdx].fYScale=fYScale;
	g_lpMtx[dwMtxIdx].fZScale=fZScale;
}

// RenderStaticWorldObjects()

void RenderStaticWorldObjects(void)
{
	// counter
	DWORD dwCount;

	// check counter
	if(g_dwNumSWObj)
		for(dwCount=0;dwCount<g_dwNumSWObj;dwCount++)
			RenderStaticWorldObject(dwCount);
}

// RenderDynamicWorldObject()

void RenderDynamicWorldObject(DWORD dwIndex)
{
	// verify index
	if(dwIndex>=g_dwNumDWObj)
	{
		DisplayFatalErrorMessage("Invalid dynamic world object index.",
								 "RenderStaticWorldObject()",
								 FALSE);
		return;
	}
	
	// get matrix index
	DWORD dwMtxIdx=g_lpdwDWObjMtxIdx[dwIndex];
	
	// save matrix scale factors
	float fXScale=g_lpMtx[dwMtxIdx].fXScale;
	float fYScale=g_lpMtx[dwMtxIdx].fYScale;
	float fZScale=g_lpMtx[dwMtxIdx].fZScale;

	// set scale factors
	g_lpMtx[dwMtxIdx].fXScale=g_lpvDWObjRad[dwIndex].x;
	g_lpMtx[dwMtxIdx].fYScale=g_lpvDWObjRad[dwIndex].y;
	g_lpMtx[dwMtxIdx].fZScale=g_lpvDWObjRad[dwIndex].z;

	// render mesh
	RenderMesh(g_dwObjectMeshIdx,
			   dwMtxIdx);
	
	// restore scale factors
	g_lpMtx[dwMtxIdx].fXScale=fXScale;
	g_lpMtx[dwMtxIdx].fYScale=fYScale;
	g_lpMtx[dwMtxIdx].fZScale=fZScale;

	// render ahead vector
	D3DXVECTOR3 vV1=D3DXVECTOR3(g_lpMtx[dwMtxIdx].mtx._41,
								g_lpMtx[dwMtxIdx].mtx._42,
								g_lpMtx[dwMtxIdx].mtx._43);
	D3DXVECTOR3 vV2=vV1+(D3DXVECTOR3(g_lpMtx[dwMtxIdx].mtx._31,
									 g_lpMtx[dwMtxIdx].mtx._32,
									 g_lpMtx[dwMtxIdx].mtx._33)*5);
	RenderLine(&vV1,&vV2);
}

// RenderDynamicWorldObjects()

void RenderDynamicWorldObjects(void)
{
	// counter
	DWORD dwCount;

	// check counter
	if(g_dwNumDWObj)
		for(dwCount=0;dwCount<g_dwNumDWObj;dwCount++)
			RenderDynamicWorldObject(dwCount);
}

// InitWorldObjectMesh()

void InitWorldObjectMesh(void)
{
	LoadingOutput("InitWorldObjectMesh");

	// add material
	DWORD dwMat=AddMaterial(0,0,0,0.7F,
							0,0,0,0.7F,
							0,0,0,0.7F,
							1,1,1,0.7F,
							0);

	// add mesh
	g_dwObjectMeshIdx=AddMeshSphere(1,
									8,8,
									dwMat);
	
	// add matrix
	g_dwObjectMtxIdx=AddMatrix();
}

// RenderDynamicWorldObjectBoundingBox()

void RenderDynamicWorldObjectBoundingBox(DWORD dwIndex)
{
	// verify index
	if(dwIndex>=g_dwNumDWObj)
	{
		DisplayFatalErrorMessage("Invalid dynamic world object index.",
								 "GetDynamicWorldObjectBoundingBox()",
								 FALSE);
		return;
	}

	// bounding box vectors
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;

	// get bounding box
	GetDynamicWorldObjectBoundingBox(dwIndex,
									 &vMin,
									 &vMax);

	// get vertices
	D3DXVECTOR3 v11=D3DXVECTOR3(vMin.x,
							    vMin.y,
							    vMin.z);
	D3DXVECTOR3 v12=D3DXVECTOR3(vMax.x,
							    vMin.y,
							    vMin.z);
	D3DXVECTOR3 v13=D3DXVECTOR3(vMax.x,
							    vMax.y,
							    vMin.z);
	D3DXVECTOR3 v14=D3DXVECTOR3(vMin.x,
							    vMax.y,
							    vMin.z);
	D3DXVECTOR3 v21=D3DXVECTOR3(vMin.x,
							    vMin.y,
							    vMax.z);
	D3DXVECTOR3 v22=D3DXVECTOR3(vMax.x,
							    vMin.y,
							    vMax.z);
	D3DXVECTOR3 v23=D3DXVECTOR3(vMax.x,
							    vMax.y,
							    vMax.z);
	D3DXVECTOR3 v24=D3DXVECTOR3(vMin.x,
							    vMax.y,
							    vMax.z);

	// reset world matrix
	SetWorldMatrix();

	// render bounding box
	RenderTransformedLine(&v11,&v12);
	RenderTransformedLine(&v12,&v13);
	RenderTransformedLine(&v13,&v14);
	RenderTransformedLine(&v14,&v11);
	RenderTransformedLine(&v21,&v22);
	RenderTransformedLine(&v22,&v23);
	RenderTransformedLine(&v23,&v24);
	RenderTransformedLine(&v24,&v21);
	RenderTransformedLine(&v11,&v21);
	RenderTransformedLine(&v12,&v22);
	RenderTransformedLine(&v13,&v23);
	RenderTransformedLine(&v14,&v24);
}

// AppendWorldScriptStaticTriangleInfo()

void AppendWorldScriptStaticTriangleInfo(LPCSTR lpFile)
{
	LoadingOutput("AppendWorldScriptStaticTriangleInfo");

	// counter
	DWORD dwCount;

	// number of bytes written
	DWORD dwBytesWritten;
	
	// check handle
	if(g_hCScriptFile!=INVALID_HANDLE_VALUE)
	{
		DisplayFatalErrorMessage("Unable to open multiple script files.",
								 NULL,
								 FALSE);
		return;
	}

	// open file
	g_hCScriptFile=CreateFile(lpFile,
							  GENERIC_READ|
							  GENERIC_WRITE,
							  NULL,
							  NULL,
							  OPEN_EXISTING,
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
		return;
	}
	else
		AddWinResource;

	// set file pointer
	if(SetFilePointer(g_hCScriptFile,
					  -((long)(sizeof(long))),
					  NULL,
					  FILE_END)==0xFFFFFFFF)
	{
		DisplayFatalErrorMessage("Unable to set script file pointer.",
								 NULL,
								 FALSE);
		goto AppendWorldScriptStaticTriangleInfo1;
	}

	// output new opcode
	SaveScriptLong(WS_SWTRI);

	// output extents
	SaveScriptFloat(g_vMinSTriExtents.x);
	SaveScriptFloat(g_vMinSTriExtents.y);
	SaveScriptFloat(g_vMinSTriExtents.z);
	SaveScriptFloat(g_vMaxSTriExtents.x);
	SaveScriptFloat(g_vMaxSTriExtents.y);
	SaveScriptFloat(g_vMaxSTriExtents.z);

	// output segment numbers
	SaveScriptLong((long)g_dwNumSTriSegX);
	SaveScriptLong((long)g_dwNumSTriSegY);
	SaveScriptLong((long)g_dwNumSTriSegZ);
	SaveScriptLong((long)g_dwNumSTriSegBlocks);

	// output segment size
	SaveScriptFloat(g_vSTriSegSize.x);
	SaveScriptFloat(g_vSTriSegSize.y);
	SaveScriptFloat(g_vSTriSegSize.z);

	// output segment blocks
	if(g_dwNumSTriSegBlocks)
		for(dwCount=0;dwCount<g_dwNumSTriSegBlocks;dwCount++)
		{
			// get number of triangles
			DWORD dwNumTri=g_lpSTriSegBlocks[dwCount].dwNumTri;
			
			// output number of triangles
			SaveScriptLong((long)dwNumTri);

			// output index list
			if(dwNumTri)
			{
				// write data
				WriteFile(g_hCScriptFile,
						  g_lpSTriSegBlocks[dwCount].lpdwTriIndexList,
						  sizeof(DWORD)*dwNumTri,
						  &dwBytesWritten,
						  NULL);

				// check for error
				if(dwBytesWritten!=(sizeof(DWORD)*dwNumTri))
				{
					DisplayFatalErrorMessage("Unable to output data to script file.");
					goto AppendWorldScriptStaticTriangleInfo1;
				}
			}
		}

	// reference point
	AppendWorldScriptStaticTriangleInfo1:;

	// output eof opcode
	SaveScriptLong(WS_EOF);

	// close file
	if(!CloseHandle(g_hCScriptFile))
	{
		DisplayFatalErrorMessage("Unable to close script file.");
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
#endif

// ReadWorldScriptStaticTriangleInfo()

void ReadWorldScriptStaticTriangleInfo(void)
{
	LoadingOutput("ReadWorldScriptStaticTriangleInfo");

	// counter
	DWORD dwCount;
	
	// read extents
	g_vMinSTriExtents.x=ReadCScriptFloat();
	g_vMinSTriExtents.y=ReadCScriptFloat();
	g_vMinSTriExtents.z=ReadCScriptFloat();
	g_vMaxSTriExtents.x=ReadCScriptFloat();
	g_vMaxSTriExtents.y=ReadCScriptFloat();
	g_vMaxSTriExtents.z=ReadCScriptFloat();

	// read segment numbers
	g_dwNumSTriSegX=(DWORD)ReadCScriptLong();
	g_dwNumSTriSegY=(DWORD)ReadCScriptLong();
	g_dwNumSTriSegZ=(DWORD)ReadCScriptLong();
	g_dwNumSTriSegBlocks=(DWORD)ReadCScriptLong();

	// compute segment block multiplier
	g_dwNumSTriSegXY=g_dwNumSTriSegX*g_dwNumSTriSegY;

	// compute numbers of segment blocks -1
	g_nNumSTriSegX1=(int)(g_dwNumSTriSegX-1);
	g_nNumSTriSegY1=(int)(g_dwNumSTriSegY-1);
	g_nNumSTriSegZ1=(int)(g_dwNumSTriSegZ-1);

	// read segment size
	g_vSTriSegSize.x=ReadCScriptFloat();
	g_vSTriSegSize.y=ReadCScriptFloat();
	g_vSTriSegSize.z=ReadCScriptFloat();

	// compute reciprocal of segment size
	g_vSTriSegASize.x=1/g_vSTriSegSize.x;
	g_vSTriSegASize.y=1/g_vSTriSegSize.y;
	g_vSTriSegASize.z=1/g_vSTriSegSize.z;

	// allocate segment block list
	if(!AllocMem((LPVOID*)&g_lpSTriSegBlocks,
				 sizeof(SEGMENTBLOCK)*g_dwNumSTriSegBlocks))
		return;

	// read segment blocks
	if(g_dwNumSTriSegBlocks)
		for(dwCount=0;dwCount<g_dwNumSTriSegBlocks;dwCount++)
		{
			// get number of triangles
			DWORD dwNumTri=(DWORD)ReadCScriptLong();
			g_lpSTriSegBlocks[dwCount].dwNumTri=dwNumTri;
			
			// read index list
			if(dwNumTri)
			{
				// allocate memory for list
				if(!AllocMem((LPVOID*)&g_lpSTriSegBlocks[dwCount].lpdwTriIndexList,
							 sizeof(DWORD)*dwNumTri))
					return;
				
				// read data
				ReadCScriptBytes((LPVOID)g_lpSTriSegBlocks[dwCount].lpdwTriIndexList,
								 sizeof(DWORD)*dwNumTri);
			}
			else
			{
				// reset list to null
				g_lpSTriSegBlocks[dwCount].lpdwTriIndexList=NULL;
			}
		}

	// allocate triangle index list
	if(!AllocMem((LPVOID*)&g_lpdwSTriIndexList,
				 sizeof(DWORD)*g_dwNumSTri))
		return;
}

#ifdef DEBUG_MODE
// InitDialogSWObj()

DWORD InitDialogSWObj(void)
{
	LoadingOutput("InitDialogSWObj");

	// get dialog index
	DWORD dwIndex=AddDialog(120,168,
							"Static Objects");

	// add controls
	AddDialogEditControl(dwIndex,4,5,70,10,EDT_NAME);
	AddDialogDropDownListBox(dwIndex,4,18,70,64,LST_NAME);
	AddDialogPushButton(dwIndex,80,4, 38,10,"Add",   BTN_ADD);
	AddDialogPushButton(dwIndex,80,19,38,10,"Delete",BTN_DELETE);

	AddDialogGroupBox(dwIndex,4,32,54,106,"Position");
	AddDialogStaticControl(dwIndex,8,42,46,8,"X");
	AddDialogStaticControl(dwIndex,8,64,46,8,"Y");
	AddDialogStaticControl(dwIndex,8,86,46,8,"Z");
	AddDialogEditControl(dwIndex,8,52,46,10,EDT_POSX);
	AddDialogEditControl(dwIndex,8,74,46,10,EDT_POSY);
	AddDialogEditControl(dwIndex,8,96,46,10,EDT_POSZ);
	AddDialogPushButton(dwIndex,8, 110,12,10,"X",BTN_POSX);
	AddDialogPushButton(dwIndex,24,110,13,10,"Y",BTN_POSY);
	AddDialogPushButton(dwIndex,41,110,12,10,"Z",BTN_POSZ);
	AddDialogPushButton(dwIndex,8, 124,12,10,"XY",BTN_POSXY);
	AddDialogPushButton(dwIndex,24,124,13,10,"XZ",BTN_POSXZ);
	AddDialogPushButton(dwIndex,41,124,12,10,"YZ",BTN_POSYZ);

	AddDialogGroupBox(dwIndex,62,32,54,78,"Radius");
	AddDialogStaticControl(dwIndex,66,42,46,8,"X");
	AddDialogStaticControl(dwIndex,66,64,46,8,"Y");
	AddDialogStaticControl(dwIndex,66,86,46,8,"Z");
	AddDialogEditControl(dwIndex,66,52,46,10,EDT_RADX);
	AddDialogEditControl(dwIndex,66,74,46,10,EDT_RADY);
	AddDialogEditControl(dwIndex,66,96,46,10,EDT_RADZ);

	AddDialogPushButton(dwIndex,4, 142,54,22,"OK",   BTN_OK,TRUE);
	AddDialogPushButton(dwIndex,62,142,54,22,"Canel",BTN_CANCEL);
	AddDialogPushButton(dwIndex,62,114,54,24,"Save", BTN_SAVE);

	// return index
	return(dwIndex);
}

// DialogSWObjProc()

BOOL APIENTRY DialogSWObjProc(HWND hDlg,
						      UINT Msg,
						      WPARAM wParam,
						      LPARAM lParam)
{
	// counter
	DWORD dwCount;

	// check message
	switch(Msg)
	{
	case(WM_INITDIALOG):
		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// init controls
		SetCurrentDialogEditText(EDT_NAME,"");
		SetCurrentDialogEditFloat(EDT_POSX,0);
		SetCurrentDialogEditFloat(EDT_POSY,0);
		SetCurrentDialogEditFloat(EDT_POSZ,0);
		SetCurrentDialogEditFloat(EDT_RADX,1);
		SetCurrentDialogEditFloat(EDT_RADY,1);
		SetCurrentDialogEditFloat(EDT_RADZ,1);

		// reset list box
		ResetCurrentDialogDropDownSelections(LST_NAME);
		
		// init static world object list box
		for(dwCount=0;dwCount<g_dwNumSWObj;dwCount++)
		{
			// cast pointer to static world object name
			LPSTR lpName=g_lplpSWObjNames[dwCount];
			
			// add static world object to list if necessary
			if(lpName[0]!='~')
				AddCurrentDialogDropDownSelection(LST_NAME,
												  lpName);
		}
		
		// highlight previous static world object
		if(g_dwLastSWObj<g_dwNumSWObj)
			HighlightCurrentDialogDropDownSelection(LST_NAME,
													g_lplpSWObjNames[g_dwLastSWObj]);
		
		// force info to load
		goto SWObjSelChange;

	case(WM_COMMAND):
		// check notification message
		switch(HIWORD(wParam))
		{
		case(BN_CLICKED):
			// check for ok button
			if(LOWORD(wParam)==BTN_OK)
			{
				// send save message
				SendMessage(hDlg,
							WM_COMMAND,
							(WPARAM)((((DWORD)BN_CLICKED)<<16)+(DWORD)BTN_SAVE),
							NULL);

				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update environment
				UpdateWorld();
				
				// ok
				return(TRUE);
			}

			// check for cancel button
			if(LOWORD(wParam)==BTN_CANCEL)
			{
				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update world
				UpdateWorld();

				// ok
				return(TRUE);
			}
			
			// check for add button
			if(LOWORD(wParam)==BTN_ADD)
			{
				LockGlobalMsg()

				// get name to add
				GetCurrentDialogEditText(EDT_NAME,
										 g_lpMsg,
										 _MAX_PATH);

				// check name
				if(strlen(g_lpMsg)==0)
					DisplayInfoMessage("You must specify a name to add.");
				else if(g_lpMsg[0]=='~')
					DisplayInfoMessage("Specified name is invalid.");
				else if(FindStaticWorldObject(g_lpMsg)!=NO_OBJECT)
					DisplayInfoMessage("Specified static world object already exists.");
				else
				{
					// allocate name buffer
					LPSTR lpName;
					if(!AllocMem((LPVOID*)&lpName,
								 strlen(g_lpMsg)+1))
					{
						UnlockGlobalMsg()
						return(TRUE);
					}

					// copy name
					strcpy(lpName,
						   g_lpMsg);

					// get position
					float fPosX=GetCurrentDialogEditFloat(EDT_POSX);
					float fPosY=GetCurrentDialogEditFloat(EDT_POSY);
					float fPosZ=GetCurrentDialogEditFloat(EDT_POSZ);

					//get radius
					float fRadX=GetCurrentDialogEditFloat(EDT_RADX);
					float fRadY=GetCurrentDialogEditFloat(EDT_RADY);
					float fRadZ=GetCurrentDialogEditFloat(EDT_RADZ);

					// add static world object
					DWORD dwIndex=AddStaticWorldObject(fPosX,
													   fPosY,
													   fPosZ,
													   fRadX,
													   fRadY,
													   fRadZ,
													   lpName);
													 
					// check static world object
					if(dwIndex<g_dwNumSWObj)
					{
						// add name to list
						AddCurrentDialogDropDownSelection(LST_NAME,
														  lpName);

						// highlight name
						HighlightCurrentDialogDropDownSelection(LST_NAME,
																lpName);
					}

					// free name buffer
					FreeMem((LPVOID*)&lpName);

					// remove name
					SetCurrentDialogEditText(EDT_NAME,
											 "");
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for delete button
			if(LOWORD(wParam)==BTN_DELETE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world object
					DWORD dwIndex=FindStaticWorldObject(g_lpMsg);

					// check static world object
					if(dwIndex<g_dwNumSWObj)
					{
						// eliminate static world object
						LPSTR lpName=g_lplpSWObjNames[dwIndex];
						lpName[0]='~';

						// delete name from list
						RemoveCurrentDialogDropDownSelection(LST_NAME);
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for save button
			if(LOWORD(wParam)==BTN_SAVE)
			{
				LockGlobalMsg()
					
				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world object
					DWORD dwIndex=FindStaticWorldObject(g_lpMsg);

					// check static world object
					if(dwIndex<g_dwNumSWObj)
					{
						// update position
						g_lpMtx[g_lpdwSWObjMtxIdx[dwIndex]].mtx._41=GetCurrentDialogEditFloat(EDT_POSX);
						g_lpMtx[g_lpdwSWObjMtxIdx[dwIndex]].mtx._42=GetCurrentDialogEditFloat(EDT_POSY);
						g_lpMtx[g_lpdwSWObjMtxIdx[dwIndex]].mtx._43=GetCurrentDialogEditFloat(EDT_POSZ);

						// update radius
						g_lpvSWObjRad[dwIndex].x=GetCurrentDialogEditFloat(EDT_RADX);
						g_lpvSWObjRad[dwIndex].y=GetCurrentDialogEditFloat(EDT_RADY);
						g_lpvSWObjRad[dwIndex].z=GetCurrentDialogEditFloat(EDT_RADZ);
					}

					UnlockGlobalMsg()
					
					// ok
					return(TRUE);

				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSX)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world object
					DWORD dwIndex=FindStaticWorldObject(g_lpMsg);

					// check static world object
					if(dwIndex<g_dwNumSWObj)
					{
						// set current static world object
						g_dwCurSWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWOPOSX);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSY)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world object
					DWORD dwIndex=FindStaticWorldObject(g_lpMsg);

					// check static world object
					if(dwIndex<g_dwNumSWObj)
					{
						// set current mesh
						g_dwCurSWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWOPOSY);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world object
					DWORD dwIndex=FindStaticWorldObject(g_lpMsg);

					// check static world object
					if(dwIndex<g_dwNumSWObj)
					{
						// set current static world object
						g_dwCurSWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWOPOSZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSXY)
			{
				UnlockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world object
					DWORD dwIndex=FindStaticWorldObject(g_lpMsg);

					// check static world object
					if(dwIndex<g_dwNumSWObj)
					{
						// set current static world object
						g_dwCurSWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWOPOSXY);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSXZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world object
					DWORD dwIndex=FindStaticWorldObject(g_lpMsg);

					// check static world object
					if(dwIndex<g_dwNumSWObj)
					{
						// set current static world object
						g_dwCurSWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWOPOSXZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSYZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find static world object
					DWORD dwIndex=FindStaticWorldObject(g_lpMsg);

					// check static world object
					if(dwIndex<g_dwNumSWObj)
					{
						// set current static world object
						g_dwCurSWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_SWOPOSYZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			// ok
			break;

		case(CBN_SELCHANGE):
			// check identifier
			if(LOWORD(wParam)==LST_NAME)
			{
				SWObjSelChange:;
				
				LockGlobalMsg()

				// get selection text
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// replace index with static world object index
					DWORD dwIndex=FindStaticWorldObject(g_lpMsg);

					// check static world object
					if(dwIndex<g_dwNumSWObj)
					{
						// set last static world object
						g_dwLastSWObj=dwIndex;
						
						// fill in values
						SetCurrentDialogEditFloat(EDT_POSX,g_lpMtx[g_lpdwSWObjMtxIdx[dwIndex]].mtx._41);
						SetCurrentDialogEditFloat(EDT_POSY,g_lpMtx[g_lpdwSWObjMtxIdx[dwIndex]].mtx._42);
						SetCurrentDialogEditFloat(EDT_POSZ,g_lpMtx[g_lpdwSWObjMtxIdx[dwIndex]].mtx._43);
						SetCurrentDialogEditFloat(EDT_RADX,g_lpvSWObjRad[dwIndex].x);
						SetCurrentDialogEditFloat(EDT_RADY,g_lpvSWObjRad[dwIndex].y);
						SetCurrentDialogEditFloat(EDT_RADZ,g_lpvSWObjRad[dwIndex].z);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}
		
		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}

// InitDialogDWObj()

DWORD InitDialogDWObj(void)
{
	LoadingOutput("InitDialogDWObj");

	// get dialog index
	DWORD dwIndex=AddDialog(120,168,
							"Dynamic Objects");

	// add controls
	AddDialogEditControl(dwIndex,4,5,70,10,EDT_NAME);
	AddDialogDropDownListBox(dwIndex,4,18,70,64,LST_NAME);
	AddDialogPushButton(dwIndex,80,4, 38,10,"Add",   BTN_ADD);
	AddDialogPushButton(dwIndex,80,19,38,10,"Delete",BTN_DELETE);

	AddDialogGroupBox(dwIndex,4,32,54,106,"Position");
	AddDialogStaticControl(dwIndex,8,42,46,8,"X");
	AddDialogStaticControl(dwIndex,8,64,46,8,"Y");
	AddDialogStaticControl(dwIndex,8,86,46,8,"Z");
	AddDialogEditControl(dwIndex,8,52,46,10,EDT_POSX);
	AddDialogEditControl(dwIndex,8,74,46,10,EDT_POSY);
	AddDialogEditControl(dwIndex,8,96,46,10,EDT_POSZ);
	AddDialogPushButton(dwIndex,8, 110,12,10,"X",BTN_POSX);
	AddDialogPushButton(dwIndex,24,110,13,10,"Y",BTN_POSY);
	AddDialogPushButton(dwIndex,41,110,12,10,"Z",BTN_POSZ);
	AddDialogPushButton(dwIndex,8, 124,12,10,"XY",BTN_POSXY);
	AddDialogPushButton(dwIndex,24,124,13,10,"XZ",BTN_POSXZ);
	AddDialogPushButton(dwIndex,41,124,12,10,"YZ",BTN_POSYZ);

	AddDialogGroupBox(dwIndex,62,32,54,78,"Radius");
	AddDialogStaticControl(dwIndex,66,42,46,8,"X");
	AddDialogStaticControl(dwIndex,66,64,46,8,"Y");
	AddDialogStaticControl(dwIndex,66,86,46,8,"Z");
	AddDialogEditControl(dwIndex,66,52,46,10,EDT_RADX);
	AddDialogEditControl(dwIndex,66,74,46,10,EDT_RADY);
	AddDialogEditControl(dwIndex,66,96,46,10,EDT_RADZ);

	AddDialogPushButton(dwIndex,4, 142,54,22,"OK",   BTN_OK,TRUE);
	AddDialogPushButton(dwIndex,62,142,54,22,"Canel",BTN_CANCEL);
	AddDialogPushButton(dwIndex,62,114,54,24,"Save", BTN_SAVE);

	// return index
	return(dwIndex);
}

// DialogDWObjProc()

BOOL APIENTRY DialogDWObjProc(HWND hDlg,
						      UINT Msg,
						      WPARAM wParam,
						      LPARAM lParam)
{
	// counter
	DWORD dwCount;

	// check message
	switch(Msg)
	{
	case(WM_INITDIALOG):
		// set dialog box as current
		SetCurrentDialogBox(hDlg);

		// init controls
		SetCurrentDialogEditText(EDT_NAME,"");
		SetCurrentDialogEditFloat(EDT_POSX,0);
		SetCurrentDialogEditFloat(EDT_POSY,0);
		SetCurrentDialogEditFloat(EDT_POSZ,0);
		SetCurrentDialogEditFloat(EDT_RADX,1);
		SetCurrentDialogEditFloat(EDT_RADY,1);
		SetCurrentDialogEditFloat(EDT_RADZ,1);

		// reset list box
		ResetCurrentDialogDropDownSelections(LST_NAME);
		
		// init dynamic world object list box
		for(dwCount=0;dwCount<g_dwNumDWObj;dwCount++)
		{
			// cast pointer to dynamic world object name
			LPSTR lpName=g_lplpDWObjNames[dwCount];
			
			// add dynamic world object to list if necessary
			if(lpName[0]!='~')
				AddCurrentDialogDropDownSelection(LST_NAME,
												  lpName);
		}
		
		// highlight previous dynamic world object
		if(g_dwLastDWObj<g_dwNumDWObj)
			HighlightCurrentDialogDropDownSelection(LST_NAME,
													g_lplpDWObjNames[g_dwLastDWObj]);
		
		// force info to load
		goto DWObjSelChange;

	case(WM_COMMAND):
		// check notification message
		switch(HIWORD(wParam))
		{
		case(BN_CLICKED):
			// check for ok button
			if(LOWORD(wParam)==BTN_OK)
			{
				// send save message
				SendMessage(hDlg,
							WM_COMMAND,
							(WPARAM)((((DWORD)BN_CLICKED)<<16)+(DWORD)BTN_SAVE),
							NULL);

				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update environment
				UpdateWorld();
				
				// ok
				return(TRUE);
			}

			// check for cancel button
			if(LOWORD(wParam)==BTN_CANCEL)
			{
				// end dialog box
				EndDialog(hDlg,WSM_IDLE);

				// update world
				UpdateWorld();

				// ok
				return(TRUE);
			}
			
			// check for add button
			if(LOWORD(wParam)==BTN_ADD)
			{
				LockGlobalMsg()

				// get name to add
				GetCurrentDialogEditText(EDT_NAME,
										 g_lpMsg,
										 _MAX_PATH);

				// check name
				if(strlen(g_lpMsg)==0)
					DisplayInfoMessage("You must specify a name to add.");
				else if(g_lpMsg[0]=='~')
					DisplayInfoMessage("Specified name is invalid.");
				else if(FindDynamicWorldObject(g_lpMsg)!=NO_OBJECT)
					DisplayInfoMessage("Specified dynamic world object already exists.");
				else
				{
					// allocate name buffer
					LPSTR lpName;
					if(!AllocMem((LPVOID*)&lpName,
								 strlen(g_lpMsg)+1))
					{
						UnlockGlobalMsg()
						return(TRUE);
					}

					// copy name
					strcpy(lpName,
						   g_lpMsg);

					// get position
					float fPosX=GetCurrentDialogEditFloat(EDT_POSX);
					float fPosY=GetCurrentDialogEditFloat(EDT_POSY);
					float fPosZ=GetCurrentDialogEditFloat(EDT_POSZ);

					//get radius
					float fRadX=GetCurrentDialogEditFloat(EDT_RADX);
					float fRadY=GetCurrentDialogEditFloat(EDT_RADY);
					float fRadZ=GetCurrentDialogEditFloat(EDT_RADZ);

					// add dynamic world object
					DWORD dwIndex=AddDynamicWorldObject(fPosX,
													    fPosY,
													    fPosZ,
													    fRadX,
													    fRadY,
													    fRadZ,
														0,0,1,
													    lpName);
													 
					// check dynamic world object
					if(dwIndex<g_dwNumDWObj)
					{
						// add name to list
						AddCurrentDialogDropDownSelection(LST_NAME,
														  lpName);

						// highlight name
						HighlightCurrentDialogDropDownSelection(LST_NAME,
																lpName);
					}

					// free name buffer
					FreeMem((LPVOID*)&lpName);

					// remove name
					SetCurrentDialogEditText(EDT_NAME,
											 "");
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for delete button
			if(LOWORD(wParam)==BTN_DELETE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world object
					DWORD dwIndex=FindDynamicWorldObject(g_lpMsg);

					// check dynamic world object
					if(dwIndex<g_dwNumDWObj)
					{
						// eliminate dynamic world object
						LPSTR lpName=g_lplpDWObjNames[dwIndex];
						lpName[0]='~';

						// delete name from list
						RemoveCurrentDialogDropDownSelection(LST_NAME);
					}
				}

				UnlockGlobalMsg()
				
				// ok
				return(TRUE);
			}
			
			// check for save button
			if(LOWORD(wParam)==BTN_SAVE)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world object
					DWORD dwIndex=FindDynamicWorldObject(g_lpMsg);

					// check dynamic world object
					if(dwIndex<g_dwNumDWObj)
					{
						// update position
						g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._41=GetCurrentDialogEditFloat(EDT_POSX);
						g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._42=GetCurrentDialogEditFloat(EDT_POSY);
						g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._43=GetCurrentDialogEditFloat(EDT_POSZ);

						// update radius
						g_lpvDWObjRad[dwIndex].x=GetCurrentDialogEditFloat(EDT_RADX);
						g_lpvDWObjRad[dwIndex].y=GetCurrentDialogEditFloat(EDT_RADY);
						g_lpvDWObjRad[dwIndex].z=GetCurrentDialogEditFloat(EDT_RADZ);
					}

					UnlockGlobalMsg()
					
					// ok
					return(TRUE);

				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSX)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world object
					DWORD dwIndex=FindDynamicWorldObject(g_lpMsg);

					// check dynamic world object
					if(dwIndex<g_dwNumDWObj)
					{
						// set current dynamic world object
						g_dwCurDWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWOPOSX);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSY)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world object
					DWORD dwIndex=FindDynamicWorldObject(g_lpMsg);

					// check dynamic world object
					if(dwIndex<g_dwNumDWObj)
					{
						// set current dynamic world object
						g_dwCurDWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWOPOSY);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world object
					DWORD dwIndex=FindDynamicWorldObject(g_lpMsg);

					// check dynamic world object
					if(dwIndex<g_dwNumDWObj)
					{
						// set current dynamic world object
						g_dwCurDWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWOPOSZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSXY)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world object
					DWORD dwIndex=FindDynamicWorldObject(g_lpMsg);

					// check dynamic world object
					if(dwIndex<g_dwNumDWObj)
					{
						// set current dynamic world object
						g_dwCurDWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWOPOSXY);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSXZ)
			{
				UnlockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world object
					DWORD dwIndex=FindDynamicWorldObject(g_lpMsg);

					// check dynamic world object
					if(dwIndex<g_dwNumDWObj)
					{
						// set current dynamic world object
						g_dwCurDWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWOPOSXZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			if(LOWORD(wParam)==BTN_POSYZ)
			{
				LockGlobalMsg()

				// get current selection
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// find dynamic world object
					DWORD dwIndex=FindDynamicWorldObject(g_lpMsg);

					// check dynamic world object
					if(dwIndex<g_dwNumDWObj)
					{
						// set current dynamic world object
						g_dwCurDWObj=dwIndex;
						
						// end dialog box
						EndDialog(hDlg,WSM_DWOPOSYZ);

						// update world
						UpdateWorld();

						UnlockGlobalMsg()

						// ok
						return(TRUE);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}

			// ok
			break;

		case(CBN_SELCHANGE):
			// check identifier
			if(LOWORD(wParam)==LST_NAME)
			{
				DWObjSelChange:;

				LockGlobalMsg()

				// get selection text
				if(GetCurrentDialogDropDownSelection(LST_NAME,
													 g_lpMsg))
				{
					// replace index with dynamic world object index
					DWORD dwIndex=FindDynamicWorldObject(g_lpMsg);

					// check dynamic world object
					if(dwIndex<g_dwNumDWObj)
					{
						// set last dynamic world object
						g_dwLastDWObj=dwIndex;
						
						// fill in values
						SetCurrentDialogEditFloat(EDT_POSX,g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._41);
						SetCurrentDialogEditFloat(EDT_POSY,g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._42);
						SetCurrentDialogEditFloat(EDT_POSZ,g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._43);
						SetCurrentDialogEditFloat(EDT_RADX,g_lpvDWObjRad[dwIndex].x);
						SetCurrentDialogEditFloat(EDT_RADY,g_lpvDWObjRad[dwIndex].y);
						SetCurrentDialogEditFloat(EDT_RADZ,g_lpvDWObjRad[dwIndex].z);
					}
				}

				UnlockGlobalMsg()

				// ok
				return(TRUE);
			}
			
			// ok
			break;
		}
		
		// ok
		break;
	}

	// let windows handle it
	return(FALSE);
}
#endif

// UpdateDynamicWorldObjects()

void UpdateDynamicWorldObjects(void)
{
	// counter
	DWORD dwCount;

	// update each object
	if(g_dwNumDWObj)
		for(dwCount=0;dwCount<g_dwNumDWObj;dwCount++)
		{
			// check for camera object
			if(dwCount==g_dwWCPositionObjIdx)
			{
				// update world camera if necessary
				if(g_bWCEnabled)
					UpdateWorldCamera();
			}
			else
			{
				// apply gravity
				g_lpvDWObjDisp[dwCount]+=g_vGravity;
			}

			// reset collision flag
			g_lpbDWObjSTColl[dwCount]=FALSE;

			// reset recursion counter
			g_dwColRetry=0;

			// update object
			UpdateDynamicWorldObject(dwCount);
		}
}

// UpdateStaticTriangleIntersectList()

void UpdateStaticTriangleIntersectList(LPD3DXVECTOR3 lpvMin,
									   LPD3DXVECTOR3 lpvMax)
{
	// counter
	DWORD dwCount;

	// reset number of triangles
	g_dwColNumSTri=0;

	// adjusted bounding box
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;
	
	// compute adjusted bounding box
	vMin.x=((lpvMin->x)-g_vMinSTriExtents.x)*g_vSTriSegASize.x;
	vMin.y=((lpvMin->y)-g_vMinSTriExtents.y)*g_vSTriSegASize.y;
	vMin.z=((lpvMin->z)-g_vMinSTriExtents.z)*g_vSTriSegASize.z;
	vMax.x=((lpvMax->x)-g_vMinSTriExtents.x)*g_vSTriSegASize.x;
	vMax.y=((lpvMax->y)-g_vMinSTriExtents.y)*g_vSTriSegASize.y;
	vMax.z=((lpvMax->z)-g_vMinSTriExtents.z)*g_vSTriSegASize.z;

	// compute block extents
	int nMinX=(int)vMin.x;
	int nMinY=(int)vMin.y;
	int nMinZ=(int)vMin.z;
	int nMaxX=(int)vMax.x;
	int nMaxY=(int)vMax.y;
	int nMaxZ=(int)vMax.z;

	// check for enclosure in world
	if(nMinX>g_nNumSTriSegX1||
	   nMinY>g_nNumSTriSegY1||
	   nMinZ>g_nNumSTriSegZ1||
	   nMaxX<0||
	   nMaxY<0||
	   nMaxZ<0)
	{
		// no possible intersections
		return;
	}

	// clip x-extents
	if(nMinX<0)
		nMinX=0;
	if(nMaxX>g_nNumSTriSegX1)
		nMaxX=g_nNumSTriSegX1;

	// clip y-extents
	if(nMinY<0)
		nMinY=0;
	if(nMaxY>g_nNumSTriSegY1)
		nMaxY=g_nNumSTriSegY1;

	// clip z-extents
	if(nMinZ<0)
		nMinZ=0;
	if(nMaxZ>g_nNumSTriSegZ1)
		nMaxZ=g_nNumSTriSegZ1;

	// set loop values
	int nX=nMinX;
	int nY=nMinY;
	int nZ=nMinZ;

	// update index counter
	g_dwColIndexNum++;

	// check index counter
	if(g_dwColIndexNum==NO_OBJECT)
	{
		// reset indexing flags
		for(dwCount=0;dwCount<g_dwNumSTri;dwCount++)
			g_lpdwSTriIndexed[dwCount]=NO_OBJECT;

		// reset index counter
		g_dwColIndexNum=0;
	}

	// get list of polygons
	while(nZ<=nMaxZ)
	{
		while(nY<=nMaxY)
		{
			while(nX<=nMaxX)
			{
				// compute block index
				DWORD dwBlock= ((DWORD)(nX))+
							  (((DWORD)(nY))*g_dwNumSTriSegX)+
							  (((DWORD)(nZ))*g_dwNumSTriSegXY);

				// get number of triangles in block
				DWORD dwNumTri=g_lpSTriSegBlocks[dwBlock].dwNumTri;

				// add triangles to list if necessary
				if(dwNumTri)
					for(dwCount=0;dwCount<dwNumTri;dwCount++)
					{
						// get triangle index
						DWORD dwTriIndex=g_lpSTriSegBlocks[dwBlock].lpdwTriIndexList[dwCount];
						
						// check indexed flag
						if(g_lpdwSTriIndexed[dwTriIndex]!=g_dwColIndexNum)
						{
							// add triangle to list
							g_lpdwSTriIndexList[g_dwColNumSTri++]=dwTriIndex;
							
							// set indexed flag
							g_lpdwSTriIndexed[dwTriIndex]=g_dwColIndexNum;
						}
					}
				
				// increment x
				nX++;
			}
			
			// increment y
			nY++;

			// reset x
			nX=nMinX;
		}
		
		// increment z
		nZ++;
		
		// reset y
		nY=nMinY;
	}
}

#ifdef DEBUG_MODE
// RenderStaticTriangleIntersectList()

void RenderStaticTriangleIntersectList(void)
{
	// counter
	DWORD dwCount;

	// render static triangles
	if(g_dwColNumSTri)
		for(dwCount=0;dwCount<g_dwColNumSTri;dwCount++)
			RenderStaticTriangle(g_lpdwSTriIndexList[dwCount]);
}
#endif

// UpdateDynamicWorldObject()

void UpdateDynamicWorldObject(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDWObj)
	{
		DisplayFatalErrorMessage("Invalid dynamic world object index.",
								 "UpdateDynamicWorldObject()",
								 FALSE);
		return;
	}
#endif

	// counter
	DWORD dwCount;
	
	// increment recursion counter
	g_dwColRetry++;
	
	// check recursion counter
	if(g_dwColRetry>MAX_RECURSIONS)
	{
		// revert to last safe position
		g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._41=g_lpvDWObjSafePos[dwIndex].x;
		g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._42=g_lpvDWObjSafePos[dwIndex].y;
		g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._43=g_lpvDWObjSafePos[dwIndex].z;

#ifdef DEBUG_MODE
		// flash screen black to indicate max recursions
		//FadeScreen(D3DCOLOR_COLORVALUE(0,0,0,0.5F));
#endif
		
		// ok
		return;
	}

	// get scaled position
	D3DXVECTOR3 vPos;
	vPos.x=g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._41*g_lpvDWObjARad[dwIndex].x;
	vPos.y=g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._42*g_lpvDWObjARad[dwIndex].y;
	vPos.z=g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._43*g_lpvDWObjARad[dwIndex].z;

	// get scaled displacement
	D3DXVECTOR3 vDisp;
	vDisp.x=g_lpvDWObjDisp[dwIndex].x*g_lpvDWObjARad[dwIndex].x;
	vDisp.y=g_lpvDWObjDisp[dwIndex].y*g_lpvDWObjARad[dwIndex].y;
	vDisp.z=g_lpvDWObjDisp[dwIndex].z*g_lpvDWObjARad[dwIndex].z;

	// compute minimum distance
	float fMinDist=Magnitude(&vDisp);

	// stop if displacement is too small
	if(fabsf(fMinDist)<DWOBJ_MINSPEED)
		return;

	// compute normalized displacement
	D3DXVECTOR3 vNDisp=vDisp/fMinDist;

	// object bounding box
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;
	
	// get object bounding box
	GetDynamicWorldObjectBoundingBox(dwIndex,
									 &vMin,
									 &vMax);

	// update triangle list
	UpdateStaticTriangleIntersectList(&vMin,
									  &vMax);

	// reset internal collision flag
	BOOL bCollision=FALSE;

	// minimum sphere intersection point
	D3DXVECTOR3 vSIMin;

	// check static triangle list
	if(g_dwColNumSTri)
		for(dwCount=0;dwCount<g_dwColNumSTri;dwCount++)
		{
			// get triangle index
			DWORD dwTriIndex=g_lpdwSTriIndexList[dwCount];

			// get scaled normal of triangle
			D3DXVECTOR3 vVec;
			vVec.x=g_lpvSTriNormal[dwTriIndex].x*g_lpvDWObjRad[dwIndex].x;
			vVec.y=g_lpvSTriNormal[dwTriIndex].y*g_lpvDWObjRad[dwIndex].y;
			vVec.z=g_lpvSTriNormal[dwTriIndex].z*g_lpvDWObjRad[dwIndex].z;

			// normalize vector
			D3DXVECTOR3 vNVec;
			Normalize(&vNVec,
					  &vVec);

			// use only front faces
			if(DotProduct(&vNVec,
						  &vNDisp)<0)
			{
				// scaled vertices
				D3DXVECTOR3 vVtx1;
				D3DXVECTOR3 vVtx2;
				D3DXVECTOR3 vVtx3;
				
				// get scaled vertices
				vVtx1.x=g_lpvSTriVtx1[dwTriIndex].x*g_lpvDWObjARad[dwIndex].x;
				vVtx1.y=g_lpvSTriVtx1[dwTriIndex].y*g_lpvDWObjARad[dwIndex].y;
				vVtx1.z=g_lpvSTriVtx1[dwTriIndex].z*g_lpvDWObjARad[dwIndex].z;
				vVtx2.x=g_lpvSTriVtx2[dwTriIndex].x*g_lpvDWObjARad[dwIndex].x;
				vVtx2.y=g_lpvSTriVtx2[dwTriIndex].y*g_lpvDWObjARad[dwIndex].y;
				vVtx2.z=g_lpvSTriVtx2[dwTriIndex].z*g_lpvDWObjARad[dwIndex].z;
				vVtx3.x=g_lpvSTriVtx3[dwTriIndex].x*g_lpvDWObjARad[dwIndex].x;
				vVtx3.y=g_lpvSTriVtx3[dwTriIndex].y*g_lpvDWObjARad[dwIndex].y;
				vVtx3.z=g_lpvSTriVtx3[dwTriIndex].z*g_lpvDWObjARad[dwIndex].z;

				// distance to plane intersection
				float fPIDist;
				
				// plane intersection point
				D3DXVECTOR3 vPI;

				// compute sphere intersection point
				D3DXVECTOR3 vSI=vPos-vNVec;

				// classify sphere intersection point
				DWORD dwClass=ClassifyPoint(&vVtx1,
											&vNVec,
											&vSI);

				// check for embedded plane
				if(dwClass==PLANE_BACK)
				{
					// get distance to plane intersection
					fPIDist=DistanceToRayPlaneIntersection(&vSI,
														   &vNVec,
														   &vVtx1,
														   &vNVec);
					
					// compute plane intersection point
					vPI=vSI+(fPIDist*vNVec);
				}
				else
				{
					// get distance to plane intersection
					fPIDist=DistanceToRayPlaneIntersection(&vSI,
														   &vNDisp,
														   &vVtx1,
														   &vNVec);

					// compute plane intersection point
					vPI=vSI+(fPIDist*vNDisp);
				}

				// distance to sphere intersection
				float fSIDist;

				// triangle intersection point
				D3DXVECTOR3 vTI;
				
				// check point against triangle
				if(IsPointInTriangle(&vVtx1,
									 &vVtx2,
									 &vVtx3,
									 &vPI))
				{
					// we are inside, so copy values
					fSIDist=fPIDist;
					vTI=vPI;
				}
				else			
				{
					// find nearest point on triangle
					GetNearestPointOnTriangle(&vVtx1,
											  &vVtx2,
											  &vVtx3,
											  &vPI,
											  &vTI);

					// compute reverse displacement
					D3DXVECTOR3 vNDispR=-vNDisp;

					// compute distance to sphere intersection
					fSIDist=DistanceToRaySphereIntersection(&vTI,
															&vNDispR,
															&vPos);

					// compute new sphere intersection point
					vSI=vTI+(fSIDist*vNDispR);
				}

				// see if sphere is stuck in polygon
				if(IsPointInSphere(&vTI,
								   &vPos))
				{
					// revert to last safe position
					g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._41=g_lpvDWObjSafePos[dwIndex].x;
					g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._42=g_lpvDWObjSafePos[dwIndex].y;
					g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._43=g_lpvDWObjSafePos[dwIndex].z;

#ifdef DEBUG_MODE
					// flash screen white to indicate stuck sphere
					//FadeScreen(D3DCOLOR_COLORVALUE(1,1,1,0.5F));
#endif

					// compute displacement away from triangle
					vDisp=vNVec;
					
					// scale displacement
					vDisp.x*=g_lpvDWObjRad[dwIndex].x;
					vDisp.y*=g_lpvDWObjRad[dwIndex].y;
					vDisp.z*=g_lpvDWObjRad[dwIndex].z;

					// normalize displacement
					Normalize(&vNDisp,
							  &vDisp);

					// set new displacement
					g_lpvDWObjDisp[dwIndex]=(vNDisp*DWOBJ_UNSTICK)-g_vGravity;
					
					// set collision flag
					g_lpbDWObjSTColl[dwIndex]=TRUE;

					// compute point of collision
					D3DXVECTOR3 vColl=vTI-vPos;

					// normalize point of collision vector
					D3DXVECTOR3 vNColl;
					Normalize(&vNColl,
							  &vColl);

					// set point of collision
					g_lpvDWObjSTCollPt[dwIndex]=vNColl;

					// no need to continue
					return;
				}

				// check distance
				if(fSIDist>=0&&
				   fSIDist<=fMinDist)
				{
					// save necessary values
					fMinDist=fSIDist;
					vSIMin=vSI;

					// set collision flag
					bCollision=TRUE;
				}
			}
		}

	// save this position as safe
	g_lpvDWObjSafePos[dwIndex].x=g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._41;
	g_lpvDWObjSafePos[dwIndex].y=g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._42;
	g_lpvDWObjSafePos[dwIndex].z=g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._43;

	// check collision flag
	if(bCollision)
	{
		// check object collision flag
		if(!g_lpbDWObjSTColl[dwIndex])
		{
			// set flag
			g_lpbDWObjSTColl[dwIndex]=TRUE;

			// set collision point
			g_lpvDWObjSTCollPt[dwIndex]=vSIMin-vPos;
		}
		else
		{
			// compute average collision point
			D3DXVECTOR3 vCollPt=((vSIMin-vPos)+g_lpvDWObjSTCollPt[dwIndex])*0.5F;

			// normalize collision point vector
			D3DXVECTOR3 vNCollPt;
			Normalize(&vNCollPt,
					  &vCollPt);

			// set collision point
			g_lpvDWObjSTCollPt[dwIndex]=vNCollPt;

			// compute new sphere intersection point
			vSIMin=vPos+vNCollPt;
		}

		// compute destination point
		D3DXVECTOR3 vDest=vSIMin+vDisp;

		// check minimum distance
		if(fMinDist>DWOBJ_OFFSET)
		{
			// set displacement close to distance needed to travel
			vDisp=vNDisp*(fMinDist-DWOBJ_OFFSET);

			// update position
			vPos+=vDisp;

			// update sphere intersection point
			vSIMin+=vDisp;

			// set new scaled position
			g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._41=vPos.x*g_lpvDWObjRad[dwIndex].x;
			g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._42=vPos.y*g_lpvDWObjRad[dwIndex].y;
			g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._43=vPos.z*g_lpvDWObjRad[dwIndex].z;
		}

		// compute slide plane normal
		D3DXVECTOR3 vSPN=vPos-vSIMin;

		// get distance to sliding plane
		float fSPDist=DistanceToRayPlaneIntersection(&vDest,
													 &vSPN,
													 &vSIMin,
													 &vSPN);

		// compute new destination point
		vDest+=(fSPDist*vSPN);

		// compute new displacement vector
		vDisp=(vDest-vSIMin)*FRICTION_FACTOR;

		// set new scaled displacement
		g_lpvDWObjDisp[dwIndex].x=vDisp.x*g_lpvDWObjRad[dwIndex].x;
		g_lpvDWObjDisp[dwIndex].y=vDisp.y*g_lpvDWObjRad[dwIndex].y;
		g_lpvDWObjDisp[dwIndex].z=vDisp.z*g_lpvDWObjRad[dwIndex].z;
		
		// repeat this function
		UpdateDynamicWorldObject(dwIndex);

		// ok
		return;
	}
	else
	{
		// check minimum distance
		if(fMinDist>DWOBJ_OFFSET)
		{
			// set displacement close to distance needed to travel
			vDisp=vNDisp*(fMinDist-DWOBJ_OFFSET);

			// update position
			vPos+=vDisp;

			// set new scaled position
			g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._41=vPos.x*g_lpvDWObjRad[dwIndex].x;
			g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._42=vPos.y*g_lpvDWObjRad[dwIndex].y;
			g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._43=vPos.z*g_lpvDWObjRad[dwIndex].z;
		}
	}
}

#ifdef DEBUG_MODE
// RenderDynamicWorldObjectCollisionPoint()

void RenderDynamicWorldObjectCollisionPoint(DWORD dwIndex)
{
	// verify index
	if(dwIndex>=g_dwNumDWObj)
	{
		DisplayFatalErrorMessage("Invalid dynamic world object index.",
								 "RedernDynamicWorldObjectCollisionPoint()",
								 FALSE);
		return;
	}

	// check collision flag
	if(!g_lpbDWObjSTColl[dwIndex])
		return;

	// prepare matrix
	D3DXMATRIX mMtx=D3DXMATRIX(0.25F,0,0,0,
							   0,0.25F,0,0,
							   0,0,0.25F,0,
							   g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._41+(g_lpvDWObjSTCollPt[dwIndex].x*g_lpvDWObjRad[dwIndex].x),
							   g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._42+(g_lpvDWObjSTCollPt[dwIndex].y*g_lpvDWObjRad[dwIndex].y),
							   g_lpMtx[g_lpdwDWObjMtxIdx[dwIndex]].mtx._43+(g_lpvDWObjSTCollPt[dwIndex].z*g_lpvDWObjRad[dwIndex].z),
							   1);

	// set matrix
	SetMatrix(g_dwObjectMtxIdx,
			  &mMtx);

	// render mesh
	RenderMesh(g_dwObjectMeshIdx,
			   g_dwObjectMtxIdx);
}
#endif

// EnableWorldCamera()

void EnableWorldCamera(void)
{
	// set flag
	g_bWCEnabled=TRUE;
}

// EnableWorldCamera()

void EnableWorldCamera(DWORD dwCamDWObjIdx,
					   DWORD dwTarDWObjIdx,
					   float fXTarOffset,
					   float fYTarOffset,
					   float fZTarOffset,
					   float fXCamOffset,
					   float fYCamOffset,
					   float fZCamOffset,
					   float fMaxSpeed)
{
#ifdef DEBUG_MODE
	// verify indices
	if(dwCamDWObjIdx>=g_dwNumDWObj||
	   dwTarDWObjIdx>=g_dwNumDWObj)
	{
		DisplayFatalErrorMessage("Invalid dynamic world object index.",
								 "EnableWorldCamera()",
								 FALSE);
		return;
	}
#endif
	
	// set camera & target objects
	SetWorldCameraObject(dwCamDWObjIdx);
	SetWorldCameraTarget(dwTarDWObjIdx);

	// set target offset vector
	SetWorldCameraTargetOffsetVector(fXTarOffset,
									 fYTarOffset,
									 fZTarOffset);

	// set camera offset vector
	SetWorldCameraOffsetVector(fXCamOffset,
							   fYCamOffset,
							   fZCamOffset);

	// set max speed
	SetWorldCameraMaxSpeed(fMaxSpeed);

	// get target point
	D3DXVECTOR3 vTar;
	GetWorldCameraTargetPoint(&vTar);

	// get position matrix
	D3DXMATRIX mPos;
	GetMatrix(g_dwWCPositionMtxIdx,
			  &mPos);

	// set ahead vector
	SetWorldCameraAheadVector(vTar.x-mPos._41,
							  vTar.y-mPos._42,
							  vTar.z-mPos._43);

	// enable world camera
	EnableWorldCamera();
}

// DisableWorldCamera()

void DisableWorldCamera(void)
{
	// set flag
	g_bWCEnabled=FALSE;
}

// UpdateWorldCamera()

void UpdateWorldCamera(void)
{
	// get position matrix
	D3DXMATRIX mPos;
	GetMatrix(g_dwWCPositionMtxIdx,
			  &mPos);

	// set position vector
	D3DXVECTOR3 vPos;
	vPos.x=mPos._41;
	vPos.y=mPos._42;
	vPos.z=mPos._43;

	// get target point
	D3DXVECTOR3 vTar;
	GetWorldCameraTargetPoint(&vTar);

	// compute displacement vector
	D3DXVECTOR3 vDisp=vTar-vPos;

	// set camera ahead vector
	SetWorldCameraAheadVector(&vDisp);

	// get target position
	GetWorldCameraTargetPosition(&vTar);

	// set displacement vector
	vDisp=vTar-vPos;

	// get length of displacement
	float fLen=Magnitude(&vDisp);

	// clip displacement if necessary
	if(fLen>g_fWCMaxSpeed)
		vDisp=(vDisp/fLen)*g_fWCMaxSpeed;

	// set camera displacement
	g_lpvDWObjDisp[g_dwWCPositionObjIdx]=vDisp;
}

// SetWorldCameraViewMatrix()

void SetWorldCameraViewMatrix(void)
{
	// get position matrix
	D3DXMATRIX mPos;
	GetMatrix(g_dwWCPositionMtxIdx,
			  &mPos);

	// get target point
	D3DXVECTOR3 vTar;
	GetWorldCameraTargetPoint(&vTar);

	// set view matrix
	SetViewMatrix(mPos._41,
				  mPos._42,
				  mPos._43,
				  vTar.x,
				  vTar.y,
				  vTar.z);
}

// SetWorldCameraObject()

void SetWorldCameraObject(DWORD dwDWObjIdx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwDWObjIdx>=g_dwNumDWObj)
	{
		DisplayFatalErrorMessage("Invalid dynamic world object index.",
								 "SetWorldCameraObject()",
								 FALSE);
		return;
	}
#endif

	// save values
	g_dwWCPositionObjIdx=dwDWObjIdx;
	g_dwWCPositionMtxIdx=g_lpdwDWObjMtxIdx[dwDWObjIdx];
}

// SetWorldCameraTarget()

void SetWorldCameraTarget(DWORD dwDWObjIdx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwDWObjIdx>=g_dwNumDWObj)
	{
		DisplayFatalErrorMessage("Invalid dynamic world object index.",
								 "SetWorldCameraTarget()",
								 FALSE);
		return;
	}
#endif

	// set target matrix
	SetWorldCameraTargetMatrix(g_lpdwDWObjMtxIdx[dwDWObjIdx]);
}

// SetWorldCameraTargetMatrix()

void SetWorldCameraTargetMatrix(DWORD dwMtxIdx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMtxIdx>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetWorldCameraTargetMatrix()",
								 FALSE);
		return;
	}
#endif

	// save value
	g_dwWCTargetMtxIdx=dwMtxIdx;
}

// SetWorldCameraTargetOffsetVector()

void SetWorldCameraTargetOffsetVector(LPD3DXVECTOR3 lpv)
{
	// use other function
	SetWorldCameraTargetOffsetVector(lpv->x,
									 lpv->y,
									 lpv->z);
}

// SetWorldCameraTargetOffsetVector()

void SetWorldCameraTargetOffsetVector(float fX,
									  float fY,
									  float fZ)
{
	// save values
	g_vWCTargetOffset.x=fX;
	g_vWCTargetOffset.y=fY;
	g_vWCTargetOffset.z=fZ;
}

// SetWorldCameraOffsetVector()

void SetWorldCameraOffsetVector(LPD3DXVECTOR3 lpv)
{
	// use other function
	SetWorldCameraOffsetVector(lpv->x,
							   lpv->y,
							   lpv->z);
}

// SetWorldCameraOffsetVector()

void SetWorldCameraOffsetVector(float fX,
								float fY,
								float fZ)
{
	// save values
	g_vWCCameraOffset.x=fX;
	g_vWCCameraOffset.y=fY;
	g_vWCCameraOffset.z=fZ;
}

// SetWorldCameraMaxSpeed()

void SetWorldCameraMaxSpeed(float fSpeed)
{
	// save value
	g_fWCMaxSpeed=fSpeed;
}

// RenderWorld()

void RenderWorld(void)
{
	// set view matrix
	SetWorldCameraViewMatrix();
	
	// render world
	RenderWorld(g_dwWCPositionMtxIdx);
}

// SetWorldCameraAheadVector()

void SetWorldCameraAheadVector(LPD3DXVECTOR3 lpv)
{
	// use other function
	SetWorldCameraAheadVector(lpv->x,
							  lpv->y,
							  lpv->z);
}

// SetWorldCameraAheadVector()

void SetWorldCameraAheadVector(float fX,
							   float fY,
							   float fZ)
{
	// prepare vector, ignoring y
	D3DXVECTOR3 vAhead;
	vAhead.x=fX;
	vAhead.y=0;
	vAhead.z=fZ;

	// normalize vector
	D3DXVECTOR3 vNAhead;
	Normalize(&vNAhead,
			  &vAhead);

	// set vector
	g_vWCAhead=vNAhead;
}

// GetWorldCameraAheadVector()

void GetWorldCameraAheadVector(LPD3DXVECTOR3 lpv)
{
	// copy value
	(*lpv)=g_vWCAhead;
}

// GetWorldCameraRightVector()

void GetWorldCameraRightVector(LPD3DXVECTOR3 lpv)
{
	// set value
	lpv->x=g_vWCAhead.z;
	lpv->y=0;
	lpv->z=-g_vWCAhead.x;
}

// GetWorldCameraTargetPoint()

void GetWorldCameraTargetPoint(LPD3DXVECTOR3 lpv)
{
	// get position matrix
	D3DXMATRIX mPos;
	GetMatrix(g_dwWCPositionMtxIdx,
			  &mPos);

	// get target matrix
	D3DXMATRIX mTar;
	GetMatrix(g_dwWCTargetMtxIdx,
			  &mTar);

	// set target position
	D3DXVECTOR3 vTar;
	lpv->x=(mTar._11*g_vWCTargetOffset.x)+
		   (mTar._21*g_vWCTargetOffset.y)+
		   (mTar._31*g_vWCTargetOffset.z)+
		    mTar._41;
	lpv->y=(mTar._12*g_vWCTargetOffset.x)+
		   (mTar._22*g_vWCTargetOffset.y)+
		   (mTar._32*g_vWCTargetOffset.z)+
		    mTar._42;
	lpv->z=(mTar._13*g_vWCTargetOffset.x)+
		   (mTar._23*g_vWCTargetOffset.y)+
		   (mTar._33*g_vWCTargetOffset.z)+
		    mTar._43;
}

// GetWorldCameraTargetPosition()

void GetWorldCameraTargetPosition(LPD3DXVECTOR3 lpv)
{
	// get position matrix
	D3DXMATRIX mPos;
	GetMatrix(g_dwWCPositionMtxIdx,
			  &mPos);

	// get target matrix
	D3DXMATRIX mTar;
	GetMatrix(g_dwWCTargetMtxIdx,
			  &mTar);

	// set target position
	D3DXVECTOR3 vTar;
	lpv->x=(mTar._11*g_vWCCameraOffset.x)+
		   (mTar._21*g_vWCCameraOffset.y)+
		   (mTar._31*g_vWCCameraOffset.z)+
		    mTar._41;
	lpv->y=(mTar._12*g_vWCCameraOffset.x)+
		   (mTar._22*g_vWCCameraOffset.y)+
		   (mTar._32*g_vWCCameraOffset.z)+
		    mTar._42;
	lpv->z=(mTar._13*g_vWCCameraOffset.x)+
		   (mTar._23*g_vWCCameraOffset.y)+
		   (mTar._33*g_vWCCameraOffset.z)+
		    mTar._43;
}

// eof //

// Copyright 2005-2007 WaveBreak Software
