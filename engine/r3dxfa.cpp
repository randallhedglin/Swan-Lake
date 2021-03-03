// Copyright 2005-2007 WaveBreak Software

// r3dxfa.cpp -- x-file animation support

// includes //

#include"r3dwin.h"
#include<rmxftmpl.h>
#include<rmxfguid.h>

// globals

// directx file interface
IDirectXFile* g_lpDXFile=NULL;
// skin mesh counters
DWORD g_dwNumSkin=0;
DWORD g_dwMaxSkin=0;
// skin mesh data
ID3DXSkinMesh** g_lplpSkin=NULL;
LPDWORD g_lpdwRenderMeshIndex=NULL;
ID3DXBuffer** g_lplpBoneNames=NULL;
ID3DXBuffer** g_lplpBoneTransforms=NULL;
LPD3DXMATRIX* g_lplpBoneStructure=NULL;
// animation counters
DWORD g_dwNumAnim=0;
DWORD g_dwMaxAnim=0;
// animation data
LPDWORD g_lpdwAnimNumRef=NULL;
LPDWORD g_lpdwAnimNumMesh=NULL;
LPDWORD g_lpdwAnimNumSkin=NULL;
LPDWORD g_lpdwAnimNumBones=NULL;
LPDWORD g_lpdwAnimNumMtx=NULL;
LPDWORD g_lpdwAnimNumRender=NULL;
LPDWORD g_lpdwAnimNumKeys=NULL;
LPDWORD* g_lplpdwAnimSkinNumBones=NULL;
LPDWORD* g_lplpdwAnimKeyTimeSig=NULL;
LPD3DXMATRIX* g_lplpmAnimKeyframes=NULL;
LPD3DXMATRIX* g_lplpmAnimCurrentKey=NULL;
LPDWORD g_lpdwAnimBaseMeshIdx=NULL;
LPDWORD g_lpdwAnimBaseSkinIdx=NULL;

// functions //

#ifdef DEBUG_MODE
// CAnimation::CAnimation()

CAnimation::CAnimation()
{
	// initialize data
	lpFrameRoot=NULL;
	bAnimLoading=FALSE;
}

// CAnimation::LoadFromX()

void CAnimation::LoadFromX(LPCSTR lpFile,
						   BOOL bNormals,
						   BOOL bOptimize)
{
	// used for error checking
	HRESULT hr;

	// reset animation flag
	BOOL bAnimLoading=FALSE;

	// check data file reference
	DATAFILEREF dfr=FindDataFileRef((LPSTR)lpFile);

	// create enumerator object
	IDirectXFileEnumObject* lpDXFileEnum;
	if(dfr.lpData)
	{
		// prepare structure
		DXFILELOADMEMORY flm;
		flm.lpMemory=dfr.lpData;
		flm.dSize=dfr.dwSize;
		
		// create enumerator
		hr=g_lpDXFile->CreateEnumObject((LPVOID)&flm,
										DXFILELOAD_FROMMEMORY,
										&lpDXFileEnum);
	}
	else
		hr=g_lpDXFile->CreateEnumObject((LPVOID)lpFile,
										DXFILELOAD_FROMFILE,
										&lpDXFileEnum);

	// check for error 
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create DirectX file enumerator object.",
								   NULL,
								   hr);
		return;
	}
	else
		AddCOMResource;

	// load top-level data objects
	IDirectXFileData* lpDXFileData;
	while(SUCCEEDED(lpDXFileEnum->GetNextDataObject(&lpDXFileData)))
	{
		// add resource
		AddCOMResource;

		// load data object
		LoadDataObject(lpDXFileData,
					   NULL,
					   bNormals,
					   bOptimize);
		
		// release data object
		ReleaseCOMPtr(lpDXFileData);
	}

	// release enumerator object
	ReleaseCOMPtr(lpDXFileEnum);

	// call SetTime(-1) to finalize frame transforms
	SetTime(-1);
}

// CAnimation::LoadDataObject()

void CAnimation::LoadDataObject(IDirectXFileData* lpData,
							    CNode* lpFrame,
							    BOOL bNormals,
								BOOL bOptimize)
{
	// used for error checking
	HRESULT hr;

	// counter
	DWORD dwCount;

	// flag to process sub-data
	BOOL bProcessSubs=FALSE;

	// template type
	const GUID* lpType;

	// buffer size
	DWORD dwSize;

	// pointer to name
	LPSTR lpName;

	// pointer to sub-frame
	CNode* lpSubFrame=lpFrame;

	// pointer to animation node
	static CNode* lpAnimNode=(CNode*)NO_OBJECT;

	// pointer to frame node
	static CNode* lpFrameNode=(CNode*)NO_OBJECT;

	// animation working node
	CNode* lpAnimWNode;

	// animation key data
	LPVOID lpKeyData;

	// get template type
	hr=lpData->GetType(&lpType);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to read DirectX file template type.",
								   NULL,
								   hr);
		return;
	}

	// get buffer size for object name
	hr=lpData->GetName(NULL,
					   &dwSize);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to read DirectX file object name length.",
								   NULL,
								   hr);
		return;
	}

	// set name as needed
	if(dwSize)
	{
		// allocate buffer
		if(!AllocMem((LPVOID*)&lpName,
				     dwSize))
			return;

		// get name
		hr=lpData->GetName(lpName,
						   &dwSize);

		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to read DirectX file object name.",
									   NULL,
									   hr);
			return;
		}
	}
	else
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&lpName,
				     strlen("Unknown")+1))
			return;
		
		// set default name
		strcpy(lpName,"Unknown");
	}

	// process template
	if((*lpType)==TID_D3DRMFrame)
	{
		// check animation flag
		if(bAnimLoading)
		{
			// set frame node pointer
			lpFrameNode=lpFrameRoot->FindNode(lpName);
			
			// link animation node to frame
			if(lpAnimNode)
			{
				// cast pointer to frame node data
				LPFRAMENODE lpFN=(LPFRAMENODE)lpFrameNode->GetNodeData();

				// set animation node
				lpFN->lpAnim=lpAnimNode;
			}
		}
		else
		{
			// check frame node
			if(lpFrame)
			{
				// add child to frame
				lpSubFrame=lpFrame->AddChild(lpName);
			}
			else
			{
				// check root frame
				if(!lpFrameRoot)
				{
					// create root frame
					AllocClassObject(lpFrameRoot,
									 CNode);

					// set name
					lpFrameRoot->SetNodeName(lpName);
					
					// save as sub-frame
					lpSubFrame=lpFrameRoot;
				}
				else
				{
					DisplayFatalErrorMessage("Multiple frame hierarchies found in animation.",
											 NULL,
											 FALSE);
					return;
				}
			}

			// check sub-frame for node data
			if(!lpSubFrame->GetNodeData())
			{
				// allocate sub-frame node data
				if(!lpSubFrame->AllocNodeMem(sizeof(FRAMENODE)))
					return;

				// cast pointer to sub-frame node data
				LPFRAMENODE lpFN=(LPFRAMENODE)lpSubFrame->GetNodeData();
				
				// initialize sub-frame node data
				lpFN->mTrans=g_mtxIdentity;
				lpFN->mWorld=g_mtxIdentity;
				lpFN->mOrigTrans=g_mtxIdentity;
				lpFN->mOrigWorld=g_mtxIdentity;
				lpFN->lpMesh=NULL;
				lpFN->lpAnim=NULL;
			}

			// process sub-frames
			bProcessSubs=TRUE;
		}
	}
	else if((*lpType)==TID_D3DRMFrameTransformMatrix)
	{
		// matrix
		float* lpfMtx;

		// load data
		hr=lpData->GetData("frameMatrix",
						   &dwSize,
						   (LPVOID*)&lpfMtx);

		// check for error
		if(FAILED(hr)||
		   dwSize!=sizeof(float)*16)
		{
			DisplayFatalDXErrorMessage("Unable to read DirectX file data.",
									   NULL,
									   hr);
			return;
		}

		// cast pointer to frame node data
		LPFRAMENODE lpFN=(LPFRAMENODE)lpFrame->GetNodeData();

		// set original matrix
		lpFN->mOrigTrans._11=lpfMtx[0];
		lpFN->mOrigTrans._12=lpfMtx[1];
		lpFN->mOrigTrans._13=lpfMtx[2];
		lpFN->mOrigTrans._14=lpfMtx[3];
		lpFN->mOrigTrans._21=lpfMtx[4];
		lpFN->mOrigTrans._22=lpfMtx[5];
		lpFN->mOrigTrans._23=lpfMtx[6];
		lpFN->mOrigTrans._24=lpfMtx[7];
		lpFN->mOrigTrans._31=lpfMtx[8];
		lpFN->mOrigTrans._32=lpfMtx[9];
		lpFN->mOrigTrans._33=lpfMtx[10];
		lpFN->mOrigTrans._34=lpfMtx[11];
		lpFN->mOrigTrans._41=lpfMtx[12];
		lpFN->mOrigTrans._42=lpfMtx[13];
		lpFN->mOrigTrans._43=lpfMtx[14];
		lpFN->mOrigTrans._44=lpfMtx[15];

		// copy to current matrix
		lpFN->mTrans=lpFN->mOrigTrans;
	}
	else if((*lpType)==TID_D3DRMMesh)
	{
		// load mesh
		if(!LoadMesh(lpData,
					 lpFrame,
					 lpName,
					 bNormals,
					 bOptimize))
			return;
	}
	else if((*lpType)==TID_D3DRMAnimationSet)
	{
		// set animation loading flag
		bAnimLoading=TRUE;

		// process sub-frames
		bProcessSubs=TRUE;
	}
	else if((*lpType)==TID_D3DRMAnimation)
	{
		// check animation flag
		if(!bAnimLoading)
		{
			DisplayFatalErrorMessage("Animation found outside of animation set.",
									 NULL,
									 FALSE);
			return;
		}

		// check node pointers
		if(!lpAnimNode||
		   !lpFrameNode)
		{
			DisplayFatalErrorMessage("Frame not found in animation.",
									 NULL,
									 FALSE);
			return;
		}
		
		// reset node pointers
		lpAnimNode=NULL;
		lpFrameNode=NULL;

		// process sub-frames
		bProcessSubs=TRUE;
	}
	else if((*lpType)==TID_D3DRMAnimationKey)
	{
		// check animation flag
		if(!bAnimLoading)
		{
			DisplayFatalErrorMessage("Animation key found outside of animation set.",
									 NULL,
									 FALSE);
			return;
		}

		// allocate animation node as needed
		if(!lpAnimNode)
		{
			// create new node
			AllocClassObject(lpAnimNode,
							 CNode);

			// copy pointer to working node
			lpAnimWNode=lpAnimNode;
		}
		else
		{
			// add sibling to existing node
			lpAnimWNode=lpAnimNode->AddSibling();
		}
		
		// allocate animation node memory
		if(!lpAnimWNode->AllocNodeMem(sizeof(ANIMNODE)))
			return;

		// cast pointer to animation node data
		LPANIMNODE lpAN=(LPANIMNODE)lpAnimWNode->GetNodeData();

		// reset node data
		lpAN->dwKeyType=NO_OBJECT;
		lpAN->dwNumKeys=0;
		lpAN->lpKeys=NULL;

		// load key type
		hr=lpData->GetData("keyType",
						   &dwSize,
						   (LPVOID*)&lpKeyData);

		// check for error
		if(FAILED(hr)||
		   dwSize!=sizeof(DWORD))
		{
			DisplayFatalErrorMessage("Unable to load animation key type.",
									 NULL,
									 FALSE);
			return;
		}

		// set key type
		lpAN->dwKeyType=*((LPDWORD)lpKeyData);

		// load number of keys
		hr=lpData->GetData("nKeys",
						   &dwSize,
						   (LPVOID*)&lpKeyData);

		// check for error
		if(FAILED(hr)||
		   dwSize!=sizeof(DWORD))
		{
			DisplayFatalErrorMessage("Unable to load animation key quantity.",
									 NULL,
									 FALSE);
			return;
		}

		// set number of keys
		lpAN->dwNumKeys=*((LPDWORD)lpKeyData);

		// key size
		DWORD dwKeySize=0;
		
		// set key size
		switch(lpAN->dwKeyType)
		{
		// check rotation key
		if(sizeof(ROTATIONKEY)!=sizeof(XROTATIONKEY))
		{
			DisplayFatalErrorMessage("D3DX Rotation key must match XOF rotation key!!",
									 "(You silly programmer, you!)",
									 FALSE);
			return;
		}

		// valid keys
		case(ANIMKEY_ROTATION): dwKeySize=sizeof(ROTATIONKEY); break;
		case(ANIMKEY_SCALE):    dwKeySize=sizeof(SCALEKEY);    break;
		case(ANIMKEY_POSITION): dwKeySize=sizeof(POSITIONKEY); break;
		case(ANIMKEY_MATRIX):   dwKeySize=sizeof(MATRIXKEY);   break;

		// invalid key
		default:
			DisplayFatalErrorMessage("Invalid animation key.",
									 NULL,
									 FALSE);
			return;
		}
		
		// allocate memory for keys
		if(!AllocMem(&lpAN->lpKeys,
					 dwKeySize*lpAN->dwNumKeys))
			return;

		// load keys
		hr=lpData->GetData("keys",
						   &dwSize,
						   &lpKeyData);

		// check for error
		if(FAILED(hr)||
		   dwSize!=dwKeySize*lpAN->dwNumKeys)
		{
			DisplayFatalErrorMessage("Unable to load animation key.",
									 NULL,
									 FALSE);
			return;
		}

		// check for rotation key
		if(lpAN->dwKeyType==ANIMKEY_ROTATION)
		{
			// cast pointers to data
			LPXROTATIONKEY lpXRot=(LPXROTATIONKEY)lpKeyData;
			LPROTATIONKEY lpRot=(LPROTATIONKEY)lpAN->lpKeys;

			// copy data & correct quaternion value
			for(dwCount=0;dwCount<lpAN->dwNumKeys;dwCount++)
			{
				// copy data
				lpRot[dwCount].dwTime=lpXRot[dwCount].dwTime;
				lpRot[dwCount].dwNumVals=lpXRot[dwCount].dwNumVals;

				// convert x-file quaternion to d3d
				D3DXQUATERNION q;
				q.x=lpXRot[dwCount].q.x;
				q.y=lpXRot[dwCount].q.y;
				q.z=lpXRot[dwCount].q.z;
				q.w=lpXRot[dwCount].q.w;

				// conjugate & save quaternion
				D3DXQuaternionConjugate(&lpRot[dwCount].q,
										&q);
			}
		}
		else
		{
			// copy data directly
			CopyMemory(lpAN->lpKeys,
					   lpKeyData,
					   dwKeySize*lpAN->dwNumKeys);
		}

		// pointers to keys for sorting
		LPROTATIONKEY lpkR;
		LPSCALEKEY    lpkS;
		LPPOSITIONKEY lpkP;
		LPMATRIXKEY   lpkM;

		// temporary sorting keys
		ROTATIONKEY kR;
		SCALEKEY    kS;
		POSITIONKEY kP;
		MATRIXKEY   kM;

		// done flag
		BOOL bDone=FALSE;
		
		// sort keys into ascending order
		switch(lpAN->dwKeyType)
		{
		// rotation keys
		case(ANIMKEY_ROTATION):

			// cast pointer
			lpkR=(LPROTATIONKEY)lpAN->lpKeys;

			// perform bubble sort
			while(!bDone)
			{
				// set flag
				bDone=TRUE;

				// sort keys
				for(dwCount=0;dwCount<lpAN->dwNumKeys-1;dwCount++)
					if(lpkR[dwCount].dwTime>lpkR[dwCount+1].dwTime)
					{
						// swap keys
						kR=lpkR[dwCount];
						lpkR[dwCount]=lpkR[dwCount+1];
						lpkR[dwCount+1]=kR;

						// reset flag
						bDone=FALSE;
					}
			}

			// ok
			break;

		// scale keys
		case(ANIMKEY_SCALE):

			// cast pointer
			lpkS=(LPSCALEKEY)lpAN->lpKeys;

			// perform bubble sort
			while(!bDone)
			{
				// set flag
				bDone=TRUE;

				// sort keys
				for(dwCount=0;dwCount<lpAN->dwNumKeys-1;dwCount++)
					if(lpkS[dwCount].dwTime>lpkS[dwCount+1].dwTime)
					{
						// swap keys
						kS=lpkS[dwCount];
						lpkS[dwCount]=lpkS[dwCount+1];
						lpkS[dwCount+1]=kS;

						// reset flag
						bDone=FALSE;
					}
			}

			// ok
			break;

		// position keys
		case(ANIMKEY_POSITION):

			// cast pointer
			lpkP=(LPPOSITIONKEY)lpAN->lpKeys;

			// perform bubble sort
			while(!bDone)
			{
				// set flag
				bDone=TRUE;

				// sort keys
				for(dwCount=0;dwCount<lpAN->dwNumKeys-1;dwCount++)
					if(lpkP[dwCount].dwTime>lpkP[dwCount+1].dwTime)
					{
						// swap keys
						kP=lpkP[dwCount];
						lpkP[dwCount]=lpkP[dwCount+1];
						lpkP[dwCount+1]=kP;

						// reset flag
						bDone=FALSE;
					}
			}

			// ok
			break;

		// matrix keys
		case(ANIMKEY_MATRIX):

			// cast pointer
			lpkM=(LPMATRIXKEY)lpAN->lpKeys;

			// perform bubble sort
			while(!bDone)
			{
				// set flag
				bDone=TRUE;

				// sort keys
				for(dwCount=0;dwCount<lpAN->dwNumKeys-1;dwCount++)
					if(lpkM[dwCount].dwTime>lpkM[dwCount+1].dwTime)
					{
						// swap keys
						kM=lpkM[dwCount];
						lpkM[dwCount]=lpkM[dwCount+1];
						lpkM[dwCount+1]=kM;

						// reset flag
						bDone=FALSE;
					}
			}

			// ok
			break;
		}

		// link animation node to frame
		if(lpFrameNode)
		{
			// cast pointer to frame node data
			LPFRAMENODE lpFN=(LPFRAMENODE)lpFrameNode->GetNodeData();

			// set animation node
			lpFN->lpAnim=lpAnimNode;
		}
	}

	// process any child objects
	if(bProcessSubs)
	{
		// next object
		IDirectXFileObject* lpSubObj;

		// get next object
		while(SUCCEEDED(lpData->GetNextObject(&lpSubObj)))
		{
			// add resource
			AddCOMResource;

			// reference object
			IDirectXFileDataReference* lpRefObj;

			// next object data
			IDirectXFileData* lpSubData;

			// check object type
			if(SUCCEEDED(lpSubObj->QueryInterface(IID_IDirectXFileDataReference,
												  (LPVOID*)&lpRefObj)))
			{
				// add resource
				AddCOMResource;

				// resolve reference object
				if(SUCCEEDED(lpRefObj->Resolve(&lpSubData)))
				{
					// add resource
					AddCOMResource;
					
					// process object data
					LoadDataObject(lpSubData,
								   lpSubFrame,
								   bNormals,
								   bOptimize);

					// release object data
					ReleaseCOMPtr(lpSubData);
				}
				
				// release reference object
				ReleaseCOMPtr(lpRefObj);
			}
			else if(SUCCEEDED(lpSubObj->QueryInterface(IID_IDirectXFileData,
													   (LPVOID*)&lpSubData)))
			{
				// add resource
				AddCOMResource;

				// process object data
				LoadDataObject(lpSubData,
							   lpSubFrame,
							   bNormals,
							   bOptimize);

				// release object data
				ReleaseCOMPtr(lpSubData);
			}

			// release sub-object
			ReleaseCOMPtr(lpSubObj);
		}
	}

	// free name buffer
	FreeMem((LPVOID*)&lpName);
}

// CAnimation::LoadMesh()

BOOL CAnimation::LoadMesh(IDirectXFileData* lpData,
						  CNode* lpFrame,
						  LPSTR lpName,
						  BOOL bNormals,
						  BOOL bOptimize)
{
	// mesh node
	CNode* lpMeshNode;
	
	// mesh creation flags
	DWORD dwFlags;

	// skin mesh buffers
	ID3DXBuffer* lpMaterials;
	ID3DXBuffer* lpAdjacency;
	ID3DXBuffer* lpBoneNames;
	ID3DXBuffer* lpBoneTransforms;

	// skin mesh
	ID3DXSkinMesh* lpSkinMesh;

	// number of materials
	DWORD dwNumMat;

	// counter
	DWORD dwCount;

	// used for error checking
	HRESULT hr;

	// cast pointer to frame node data
	LPFRAMENODE lpFN=(LPFRAMENODE)lpFrame->GetNodeData();

	// add mesh node as needed
	if(!lpFN->lpMesh)
	{
		// create new node
		AllocClassObject(lpFN->lpMesh,
						 CNode);

		// set mesh node
		lpMeshNode=lpFN->lpMesh;

		// set node name
		lpMeshNode->SetNodeName(lpName);
	}
	else
	{
		// add sibling mesh
		lpMeshNode=lpFN->lpMesh->AddSibling(lpName);
	}

	// allocate node memory
	if(!lpMeshNode->AllocNodeMem(sizeof(MESHNODE)))
		return(FALSE);

	// cast pointer to mesh node data
	LPMESHNODE lpMN=(LPMESHNODE)lpMeshNode->GetNodeData();

	// initialze mesh node data
	lpMN->dwMeshIndex=NO_OBJECT;
	lpMN->dwSkinIndex=NO_OBJECT;

	// set creation flags
	dwFlags=D3DXMESH_MANAGED;
	if(g_dwVPF==VPF_HARDWARE)
		dwFlags=dwFlags|D3DXMESH_USEHWONLY;
	if(g_dwVPF==VPF_SOFTWARE)
		dwFlags=dwFlags|D3DXMESH_SOFTWAREPROCESSING;

	// load skin mesh
	hr=D3DXLoadSkinMeshFromXof(lpData,	
							   dwFlags,
							   g_lpD3DDevice,
							   &lpAdjacency,
							   &lpMaterials,
							   &dwNumMat,
							   &lpBoneNames,
							   &lpBoneTransforms,
							   &lpSkinMesh);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to load skinned mesh.",
								   NULL,
								   hr);
		return(FALSE);
	}
	else
		AddCOMResource;

	// add additional resources as needed
	if(lpAdjacency)      AddCOMResource;
	if(lpMaterials)      AddCOMResource;
	if(lpBoneNames)      AddCOMResource;
	if(lpBoneTransforms) AddCOMResource;

	// check mesh type
	if(lpSkinMesh->GetNumBones())
	{
		// get skinned mesh
		DWORD dwIndex=GetNextSkinMesh();
		
		// get render mesh
		DWORD dwRenderMeshIndex=GetNextMesh();
		
		// allocate memory for name
		if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwRenderMeshIndex],
					 strlen(lpName)+1))
			return(NO_OBJECT);

		// copy name
		CopyMemory((LPVOID)g_lplpMeshNames[dwRenderMeshIndex],
				   (LPVOID)lpName,
				   strlen(lpName)+1);

		// set new creation flags
		dwFlags=D3DXMESH_SYSTEMMEM;
		if(g_dwVPF==VPF_HARDWARE)
			dwFlags=dwFlags|D3DXMESH_USEHWONLY;
		if(g_dwVPF==VPF_SOFTWARE)
			dwFlags=dwFlags|D3DXMESH_SOFTWAREPROCESSING;

		// generate render mesh
		hr=lpSkinMesh->GenerateSkinnedMesh(dwFlags,
										   0,
										   (LPDWORD)lpAdjacency->GetBufferPointer(),
										   NULL,
										   NULL,
										   NULL,
										   &g_lplpMesh[dwRenderMeshIndex]);

		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to generate skinned mesh.",
									   NULL,
									   hr);
			return(FALSE);
		}
		else
			AddCOMResource;

		// check number of materials
		if(dwNumMat)
		{
			// set number of subsets
			g_lpdwNumMeshSubsets[dwRenderMeshIndex]=dwNumMat;
			
			// allocate subset array
			if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwRenderMeshIndex],
						 sizeof(MESHSUBSETINFO)*dwNumMat))
				return(FALSE);

			// get subset info pointer
			LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwRenderMeshIndex];
			
			// get materials buffer pointer
			LPD3DXMATERIAL lpMat=(LPD3DXMATERIAL)lpMaterials->GetBufferPointer();
			
			// fill in subset array
			for(dwCount=0;dwCount<dwNumMat;dwCount++)
			{
				// copy material ambient value
				lpMat[dwCount].MatD3D.Ambient=lpMat[dwCount].MatD3D.Diffuse;

				// copy material
				lpMeshSubsetInfo[dwCount].dwMat=AddMaterial(&lpMat[dwCount].MatD3D);

				// load texture if needed
				if(lpMat[dwCount].pTextureFilename)
					lpMeshSubsetInfo[dwCount].dwTex=AddTexture(lpMat[dwCount].pTextureFilename);
				else
					lpMeshSubsetInfo[dwCount].dwTex=NO_OBJECT;
			}
		}
		else
		{
			// set number of subsets
			g_lpdwNumMeshSubsets[dwRenderMeshIndex]=1;
			
			// allocate subset array
			if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwRenderMeshIndex],
						 sizeof(MESHSUBSETINFO)))
				return(FALSE);

			// get subset info pointer
			LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwRenderMeshIndex];
			
			// fill in subset array
			lpMeshSubsetInfo[0].dwMat=AddMaterial(1,1,1,1,
												  1,1,1,1,
												  0,0,0,0,
												  0,0,0,0,
												  0);
			lpMeshSubsetInfo[0].dwTex=NO_OBJECT;
		}

		// set number of bones
		DWORD dwNumBones=lpSkinMesh->GetNumBones();

		// allocate bone structure
		if(!AllocMem((LPVOID*)&g_lplpBoneStructure[dwIndex],
					 sizeof(D3DXMATRIX)*dwNumBones))
			return(FALSE);

		// cast pointer to bone structure
		LPD3DXMATRIX lpBoneStructure=g_lplpBoneStructure[dwIndex];
		
		// reset bone structure
		for(dwCount=0;dwCount<dwNumBones;dwCount++)
			lpBoneStructure[dwCount]=g_mtxIdentity;

		// save skin mesh interface
		g_lplpSkin[dwIndex]=lpSkinMesh;
		
		// save render mesh index
		g_lpdwRenderMeshIndex[dwIndex]=dwRenderMeshIndex;

		// save bone buffers
		g_lplpBoneNames[dwIndex]=lpBoneNames;
		g_lplpBoneTransforms[dwIndex]=lpBoneTransforms;

		// save skin mesh index
		lpMN->dwSkinIndex=dwIndex;

		// release buffers
		ReleaseCOMPtr(lpMaterials);
		ReleaseCOMPtr(lpAdjacency);
	}
	else
	{
		// get next mesh
		DWORD dwIndex=GetNextMesh();

		// allocate memory for name
		if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
					 strlen(lpName)+1))
			return(NO_OBJECT);

		// copy name
		CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
				   (LPVOID)lpName,
				   strlen(lpName)+1);

		// create mesh
		hr=lpSkinMesh->GetOriginalMesh(&g_lplpMesh[dwIndex]);
		
		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to convert skinned mesh to static mesh.",
									   NULL,
									   hr);
			return(FALSE);;
		}
		else
			AddCOMResource;

		// check number of materials
		if(dwNumMat)
		{
			// set number of subsets
			g_lpdwNumMeshSubsets[dwIndex]=dwNumMat;
			
			// allocate subset array
			if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
						 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
				return(NO_OBJECT);

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

				// load texture if needed
				if(lpMat[dwCount].pTextureFilename)
					lpMeshSubsetInfo[dwCount].dwTex=AddTexture(lpMat[dwCount].pTextureFilename);
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
				return(NO_OBJECT);

			// get subset info pointer
			LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
			
			// fill in subset array
			lpMeshSubsetInfo[0].dwMat=AddMaterial(1,1,1,1,
												  1,1,1,1,
												  0,0,0,0,
												  0,0,0,0,
												  0);
			lpMeshSubsetInfo[0].dwTex=NO_OBJECT;
		}

		// add normals to mesh
		if(bNormals)
			AddMeshNormals(dwIndex);

		// optimize if requested
		if(bOptimize)
			OptimizeMesh(dwIndex);

		// save mesh index
		lpMN->dwMeshIndex=dwIndex;

		// release skin mesh
		ReleaseCOMPtr(lpSkinMesh);

		// release buffers
		ReleaseCOMPtr(lpMaterials);
		ReleaseCOMPtr(lpAdjacency);
	}

	// ok
	return(TRUE);
}

// CAnimation::Render()

void CAnimation::Render(DWORD dwMtxIndex)
{
	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "CAnimation::Render()",
								 FALSE);
		return;
	}

	// matrix
	D3DXMATRIX mWorld;
	
	// get matrix
	GetMatrix(dwMtxIndex,
			  &mWorld);

	// render root node
	RenderNode(lpFrameRoot,
			   mWorld,
			   mWorld);
}

// CAnimation::RenderChild()

void CAnimation::RenderChild(CNode* lpNode,
							 D3DXMATRIX mFrame,
							 D3DXMATRIX mWorld)
{
	// check pointer
	if(lpNode->GetChild())
	{
		// render child node
		RenderNode(lpNode->GetChild(),
				   mFrame,
				   mWorld);
	}
}

// CAnimation::RenderSibling()

void CAnimation::RenderSibling(CNode* lpNode,
							   D3DXMATRIX mFrame,
							   D3DXMATRIX mWorld)
{
	// check pointer
	if(lpNode->GetSibling())
	{
		// render sibling node
		RenderNode(lpNode->GetSibling(),
				   mFrame,
				   mWorld);
	}
}

// CAnimation::RenderNode()

void CAnimation::RenderNode(CNode* lpNode,
							D3DXMATRIX mFrame,
							D3DXMATRIX mWorld)
{
	// render siblings
	RenderSibling(lpNode,
				  mFrame,
				  mWorld);
	
	// transformed matrix
	D3DXMATRIX mTrans;

	// cast pointer to frame node data
	LPFRAMENODE lpFN=(LPFRAMENODE)lpNode->GetNodeData();
	
	// transform matrix by frame transform
	D3DXMatrixMultiply(&mTrans,
					   &lpFN->mTrans,
					   &mFrame);

	// render children
	RenderChild(lpNode,
				mTrans,
				mWorld);

	// render attached meshes
	if(lpFN->lpMesh)
		RenderMeshNode(lpFN->lpMesh,
					   mTrans,
					   mWorld);	
}

// CAnimation::RenderMeshNode()

void CAnimation::RenderMeshNode(CNode* lpNode,
								D3DXMATRIX mFrame,
								D3DXMATRIX mWorld)
{
	// cast pointer to mesh node data
	LPMESHNODE lpMN=(LPMESHNODE)lpNode->GetNodeData();

	// render mesh as needed
	if(lpMN->dwMeshIndex!=NO_OBJECT)
	{
		// render static mesh
		RenderMesh(lpMN->dwMeshIndex,
				   &mFrame);
	}
	else if(lpMN->dwSkinIndex!=NO_OBJECT)
	{
		// update bone structure
		UpdateBoneStructure(lpMN->dwSkinIndex);
							
		// update skinned mesh
		UpdateSkinMesh(lpMN->dwSkinIndex);
		
		// render skinned mesh
		RenderSkinMesh(lpMN->dwSkinIndex,
					   &mWorld);
	}

	// render siblings
	if(lpNode->GetSibling())
		RenderMeshNode(lpNode->GetSibling(),
					   mFrame,
					   mWorld);

}

// CAnimation::UpdateBoneStructure()

void CAnimation::UpdateBoneStructure(DWORD dwSkinIndex)
{		
	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skinned mesh index.",
								 "CAnimation::UpdateBoneStructure()",
								 FALSE);
		return;
	}

	// counter
	DWORD dwCount;

	// get number of bones
	DWORD dwNumBones=g_lplpSkin[dwSkinIndex]->GetNumBones();

	// cast pointer to bone names
	LPSTR* lpBoneNames=(LPSTR*)g_lplpBoneNames[dwSkinIndex]->GetBufferPointer();

	// cast pointer to bone transforms
	LPD3DXMATRIX lpBoneTransforms=(LPD3DXMATRIX)g_lplpBoneTransforms[dwSkinIndex]->GetBufferPointer();

	// cast pointer to bone structure
	LPD3DXMATRIX lpBoneStructure=g_lplpBoneStructure[dwSkinIndex];

	// update each bone
	for(dwCount=0;dwCount<dwNumBones;dwCount++)
	{
		// find frame to match bone
		CNode* lpFrame=lpFrameRoot->FindNode(lpBoneNames[dwCount]);

		// cast pointer to frame node data
		LPFRAMENODE lpFN=(LPFRAMENODE)lpFrame->GetNodeData();

		// set new bone transform
		D3DXMatrixMultiply(&lpBoneStructure[dwCount],
						   &lpBoneTransforms[dwCount],
						   &lpFN->mWorld);
	}
}

// CAnimation::SetTime()

void CAnimation::SetTime(DWORD dwTime)
{
	// set time of root frame
	SetTimeNode(lpFrameRoot,
				dwTime,
				g_mtxIdentity);
}

// CAnimation::SetTimeChild()

void CAnimation::SetTimeChild(CNode* lpNode,
							  DWORD dwTime,
							  D3DXMATRIX mWorld)
{
	// check pointer
	if(lpNode->GetChild())
	{
		// process child
		SetTimeNode(lpNode->GetChild(),
					dwTime,
					mWorld);
	}
}

// CAnimation::SetTimeSibling()

void CAnimation::SetTimeSibling(CNode* lpNode,
							    DWORD dwTime,
								D3DXMATRIX mWorld)
{
	// check pointer
	if(lpNode->GetSibling())
	{
		// process sibling
		SetTimeNode(lpNode->GetSibling(),
					dwTime,
					mWorld);
	}
}

// CAnimation::SetTimeNode()

void CAnimation::SetTimeNode(CNode* lpNode,
							 DWORD dwTime,
							 D3DXMATRIX mWorld)
{
	// process siblings
	SetTimeSibling(lpNode,
				   dwTime,
				   mWorld);

	// cast pointer to frame node data
	LPFRAMENODE lpFN=(LPFRAMENODE)lpNode->GetNodeData();

	// set keyframe matrix
	SetKeyMatrix(lpFN,
				 dwTime);

	// transformed world matrix
	D3DXMATRIX mTrans;
	
	// transform world by keyframe
	D3DXMatrixMultiply(&mTrans,
					   &lpFN->mTrans,
					   &mWorld);

	// copy to frame world matrix
	lpFN->mWorld=mTrans;

	// if finalizing, copy to original frame matrix
	if(dwTime==-1)
		lpFN->mOrigWorld=mTrans;

	// process children
	SetTimeChild(lpNode,
				 dwTime,
				 mTrans);
}

// CAnimation::SetKeyMatrix()

void CAnimation::SetKeyMatrix(LPFRAMENODE lpFN,
							  DWORD dwTime)
{
	// do nothing if finalizing
	if(dwTime==-1)
		return;

	// check for animation keys
	if(lpFN->lpAnim)
	{
		// new frame matrix
		D3DXMATRIX mFrame=g_mtxIdentity;

		// transform matrix in proper order
		TransformKey(lpFN->lpAnim,
					 dwTime,
					 ANIMKEY_SCALE,
					 &mFrame);
		TransformKey(lpFN->lpAnim,
					 dwTime,
					 ANIMKEY_ROTATION,
					 &mFrame);
		TransformKey(lpFN->lpAnim,
					 dwTime,
					 ANIMKEY_POSITION,
					 &mFrame);
		TransformKey(lpFN->lpAnim,
					 dwTime,
					 ANIMKEY_MATRIX,
					 &mFrame);

		// set new frame matrix
		lpFN->mTrans=mFrame;
	}
}

// CAnimation::TransformKey()

void CAnimation::TransformKey(CNode* lpNode,
							  DWORD dwTime,
							  DWORD dwKeyType,
							  LPD3DXMATRIX lpKey)
{
	// check pointer
	if(lpNode->GetSibling())
	{
		// transform sibling node
		TransformKey(lpNode->GetSibling(),
					 dwTime,
					 dwKeyType,
					 lpKey);
	}

	// cast pointer to animation node data
	LPANIMNODE lpAN=(LPANIMNODE)lpNode->GetNodeData();

	// check for matching keys
	if(lpAN->dwKeyType!=dwKeyType)
		return;

	// transformation matrix
	D3DXMATRIX mTrans;

	// counter
	DWORD dwCount;

	// pointers to animation keys
	LPROTATIONKEY lpRKey;
	LPSCALEKEY    lpSKey;
	LPPOSITIONKEY lpPKey;
	LPMATRIXKEY   lpMKey;

	// final key values
	D3DXQUATERNION qRKey;
	D3DXVECTOR3    vSKey;
	D3DXVECTOR3    vPKey;
	D3DXMATRIX     mMKey;

	// key indices
	DWORD dwKey1=lpAN->dwNumKeys-1;
	DWORD dwKey2=lpAN->dwNumKeys-1;
	
	// check key type
	switch(lpAN->dwKeyType)
	{
	// rotation key
	case(ANIMKEY_ROTATION):

		// cast pointer to rotation keys
		lpRKey=(LPROTATIONKEY)lpAN->lpKeys;

		// find first key index
		for(dwCount=0;dwCount<lpAN->dwNumKeys;dwCount++)
			if(dwTime>=lpRKey[dwCount].dwTime)
				dwKey1=dwCount;

		// find second key index
		for(dwCount=lpAN->dwNumKeys;dwCount>0;dwCount--)
			if(dwTime<=lpRKey[dwCount-1].dwTime)
				dwKey2=dwCount-1;

		// interpolate rotation as needed
		if(dwKey1==dwKey2)
			qRKey=lpRKey[dwKey1].q;
		else
		{
			// compute interpolation value
			float t=GetInterpolationValue(lpRKey[dwKey1].dwTime,
										  lpRKey[dwKey2].dwTime,
										  dwTime);

			// interpolate rotation
			D3DXQuaternionSlerp(&qRKey,
								&lpRKey[dwKey1].q,
								&lpRKey[dwKey2].q,
								t);
		}

		// check for zero quaternion
		if(IsNearZero(qRKey.x)&&
		   IsNearZero(qRKey.y)&&
		   IsNearZero(qRKey.z)&&
		   IsNearEqual(qRKey.w,1))
		{
			// set identity matrix
			mTrans=g_mtxIdentity;
			D3DXMatrixRotationQuaternion(&mTrans,
										 &qRKey);
		}
		else
		{
			// create matrix from quaternion
			D3DXMatrixRotationQuaternion(&mTrans,
										 &qRKey);
		}
		
		// ok
		break;

	// scale key
	case(ANIMKEY_SCALE):

		// cast pointer to scale keys
		lpSKey=(LPSCALEKEY)lpAN->lpKeys;

		// find first key index
		for(dwCount=0;dwCount<lpAN->dwNumKeys;dwCount++)
			if(dwTime>=lpSKey[dwCount].dwTime)
				dwKey1=dwCount;

		// find second key index
		for(dwCount=lpAN->dwNumKeys;dwCount>0;dwCount--)
			if(dwTime<=lpSKey[dwCount-1].dwTime)
				dwKey2=dwCount-1;

		// interpolate scale as needed
		if(dwKey1==dwKey2)
			vSKey=lpSKey[dwKey1].v;
		else
		{
			// compute interpolation value
			float t=GetInterpolationValue(lpSKey[dwKey1].dwTime,
										  lpSKey[dwKey2].dwTime,
										  dwTime);

			// interpolate scale
			vSKey.x=GetInterpolatedValue(lpSKey[dwKey1].v.x,
										 lpSKey[dwKey2].v.x,
										 t);
			vSKey.y=GetInterpolatedValue(lpSKey[dwKey1].v.y,
										 lpSKey[dwKey2].v.y,
										 t);
			vSKey.z=GetInterpolatedValue(lpSKey[dwKey1].v.z,
										 lpSKey[dwKey2].v.z,
										 t);
		}

		// create matrix from vector
		D3DXMatrixScaling(&mTrans,
						  vSKey.x,
						  vSKey.y,
						  vSKey.z);

		// ok
		break;

	// position key
	case(ANIMKEY_POSITION):

		// cast pointer to position keys
		lpPKey=(LPPOSITIONKEY)lpAN->lpKeys;

		// find first key index
		for(dwCount=0;dwCount<lpAN->dwNumKeys;dwCount++)
			if(dwTime>=lpPKey[dwCount].dwTime)
				dwKey1=dwCount;

		// find second key index
		for(dwCount=lpAN->dwNumKeys;dwCount>0;dwCount--)
			if(dwTime<=lpPKey[dwCount-1].dwTime)
				dwKey2=dwCount-1;

		// interpolate position as needed
		if(dwKey1==dwKey2)
			vPKey=lpPKey[dwKey1].v;
		else
		{
			// compute interpolation value
			float t=GetInterpolationValue(lpPKey[dwKey1].dwTime,
										  lpPKey[dwKey2].dwTime,
										  dwTime);

			// interpolate position
			vPKey.x=GetInterpolatedValue(lpPKey[dwKey1].v.x,
										 lpPKey[dwKey2].v.x,
										 t);
			vPKey.y=GetInterpolatedValue(lpPKey[dwKey1].v.y,
										 lpPKey[dwKey2].v.y,
										 t);
			vPKey.z=GetInterpolatedValue(lpPKey[dwKey1].v.z,
										 lpPKey[dwKey2].v.z,
										 t);
		}

		// create matrix from vector
		D3DXMatrixTranslation(&mTrans,
							  vPKey.x,
							  vPKey.y,
							  vPKey.z);

		// ok
		break;

	// matrix key
	case(ANIMKEY_MATRIX):

		// cast pointer to matrix keys
		lpMKey=(LPMATRIXKEY)lpAN->lpKeys;

		// find first key index
		for(dwCount=0;dwCount<lpAN->dwNumKeys;dwCount++)
			if(dwTime>=lpMKey[dwCount].dwTime)
				dwKey1=dwCount;

		// find second key index
		for(dwCount=lpAN->dwNumKeys;dwCount>0;dwCount--)
			if(dwTime<=lpMKey[dwCount-1].dwTime)
				dwKey2=dwCount-1;

		// interpolate matrix as needed
		if(dwKey1==dwKey2)
			mMKey=lpMKey[dwKey1].m;
		else
		{
			// compute interpolation value
			float t=GetInterpolationValue(lpMKey[dwKey1].dwTime,
										  lpMKey[dwKey2].dwTime,
										  dwTime);

			// cast pointers to matrix values
			float* lpF1=(float*)lpMKey[dwKey1].m;
			float* lpF2=(float*)lpMKey[dwKey2].m;
			float* lpFT=(float*)&mMKey;

			// interpolate values
			for(dwCount=0;dwCount<16;dwCount++)
				lpFT[dwCount]=GetInterpolatedValue(lpF1[dwCount],
												   lpF2[dwCount],
												   t);
		}

		// copy matrix to key
		mTrans=mMKey;
		
		// ok
		break;
	}

	// final matrix
	D3DXMATRIX mFinal;
	
	// transform matrix
	D3DXMatrixMultiply(&mFinal,
					   lpKey,
					   &mTrans);
	
	// copy final matrix to original
	(*lpKey)=mFinal;
}

// CAnimation::FreeMeshChild()

void CAnimation::FreeMeshChild(CNode* lpNode)
{
	// check pointer
	if(lpNode->GetChild())
	{
		// free child node
		FreeMeshNode(lpNode->GetChild());
	}
}

// CAnimation::FreeAnimChild()

void CAnimation::FreeAnimChild(CNode* lpNode)
{
	// check pointer
	if(lpNode->GetChild())
	{
		// free child node
		FreeAnimNode(lpNode->GetChild());
	}
}

// CAnimation::FreeAnimChildMem()

void CAnimation::FreeAnimChildMem(CNode* lpNode)
{
	// check pointer
	if(lpNode->GetChild())
	{
		// free child node
		FreeAnimNodeMem(lpNode->GetChild());
	}
}

// CAnimation::FreeMeshSibling()

void CAnimation::FreeMeshSibling(CNode* lpNode)
{
	// check pointer
	if(lpNode->GetSibling())
	{
		// free sibling node
		FreeMeshNode(lpNode->GetSibling());
	}
}

// CAnimation::FreeAnimSibling()

void CAnimation::FreeAnimSibling(CNode* lpNode)
{
	// check pointer
	if(lpNode->GetSibling())
	{
		// free sibling node
		FreeAnimNode(lpNode->GetSibling());
	}
}

// CAnimation::FreeAnimSiblingMem()

void CAnimation::FreeAnimSiblingMem(CNode* lpNode)
{
	// check pointer
	if(lpNode->GetSibling())
	{
		// free sibling node
		FreeAnimNodeMem(lpNode->GetSibling());
	}
}

// CAnimation::FreeMeshNode()

void CAnimation::FreeMeshNode(CNode* lpNode)
{
	// free members
	FreeMeshChild(lpNode);
	FreeMeshSibling(lpNode);

	// check node data
	if(lpNode->GetNodeData())
	{
		// cast pointer to node data
		LPFRAMENODE lpFN=(LPFRAMENODE)lpNode->GetNodeData();

		// check mesh pointer
		if(lpFN->lpMesh)
		{
			// free node memory
			lpFN->lpMesh->FreeNodeMem();

			// delete node
			lpFN->lpMesh->DeleteNode();

			// free class object
			FreeClassObject(lpFN->lpMesh);
		}
	}
}

// CAnimation::FreeAnimNode()

void CAnimation::FreeAnimNode(CNode* lpNode)
{
	// free members
	FreeAnimChild(lpNode);
	FreeAnimSibling(lpNode);

	// check node data
	if(lpNode->GetNodeData())
	{
		// cast pointer to node data
		LPFRAMENODE lpFN=(LPFRAMENODE)lpNode->GetNodeData();

		// check animation pointer
		if(lpFN->lpAnim)
		{
			// free animation node memory
			FreeAnimNodeMem(lpFN->lpAnim);
			
			// free node memory
			lpFN->lpAnim->FreeNodeMem();

			// delete node
			lpFN->lpAnim->DeleteNode();

			// free class object
			FreeClassObject(lpFN->lpAnim);
		}
	}
}

// CAnimation::FreeAnimNodeMem()

void CAnimation::FreeAnimNodeMem(CNode* lpNode)
{
	// free members
	FreeAnimChildMem(lpNode);
	FreeAnimSiblingMem(lpNode);

	// check node data
	if(lpNode->GetNodeData())
	{
		// cast pointer to node data
		LPANIMNODE lpAN=(LPANIMNODE)lpNode->GetNodeData();

		// free keys
		if(lpAN->lpKeys)
			FreeMem(&lpAN->lpKeys);
	}
}

// CAnimation::Release()

void CAnimation::Release(void)
{
	// free frame nodes
	FreeMeshNode(lpFrameRoot);
	FreeAnimNode(lpFrameRoot);

	// free root frame memory
	lpFrameRoot->FreeNodeMem();
	
	// delete root frame
	lpFrameRoot->DeleteNode();

	// free root frame
	FreeClassObject(lpFrameRoot);
}
#endif

// InitDXFile()

BOOL InitDXFile(void)
{
	LoadingOutput("InitDXFile");

	// used for error checking
	HRESULT hr;
	
	// create directx file interface
	hr=DirectXFileCreate(&g_lpDXFile);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create DirectX file interface.",
							  NULL,
							  hr);
		return(FALSE);
	}
	else
		AddCOMResource;

	// register standard templates
	hr=g_lpDXFile->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES,
								     D3DRM_XTEMPLATE_BYTES);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to load DirectX file templates.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// ReleaseDXFile()

void ReleaseDXFile(void)
{
	// release pointer
	ReleaseCOMPtr(g_lpDXFile);
}

// GetNextSkinMesh()

DWORD GetNextSkinMesh(void)
{
	LoadingOutput("GetNextSkinMesh");

	// check counter
	if(g_dwNumSkin==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpSkin,
					 sizeof(ID3DXSkinMesh*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwRenderMeshIndex,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpBoneNames,
					 sizeof(ID3DXBuffer*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpBoneTransforms,
					 sizeof(ID3DXBuffer*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpBoneStructure,
					 sizeof(LPD3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxSkin=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumSkin>=g_dwMaxSkin)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpSkin,
					  sizeof(ID3DXSkinMesh*)*g_dwMaxSkin,
					  sizeof(ID3DXSkinMesh*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwRenderMeshIndex,
					  sizeof(DWORD)*g_dwMaxSkin,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpBoneNames,
					  sizeof(ID3DXBuffer*)*g_dwMaxSkin,
					  sizeof(ID3DXBuffer*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpBoneTransforms,
					  sizeof(ID3DXBuffer*)*g_dwMaxSkin,
					  sizeof(ID3DXBuffer*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpBoneStructure,
					  sizeof(LPD3DXMATRIX)*g_dwMaxSkin,
					  sizeof(LPD3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxSkin+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumSkin;

	// increment counter
	g_dwNumSkin++;

	// return index
	return(dwIndex);
}

// UpdateSkinMesh()

void UpdateSkinMesh(DWORD dwSkinIndex,
					LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "UpdateSkinMesh()",
								 FALSE);
		return;
	}
#endif

	// check matrix pointer
	if(lpMtx)
	{
		// update mesh using matrix array
		g_lplpSkin[dwSkinIndex]->UpdateSkinnedMesh(lpMtx,
												   NULL,
												   g_lplpMesh[g_lpdwRenderMeshIndex[dwSkinIndex]]);
	}
	else
	{
		// update mesh using bone structure
		g_lplpSkin[dwSkinIndex]->UpdateSkinnedMesh(g_lplpBoneStructure[dwSkinIndex],
												   NULL,
												   g_lplpMesh[g_lpdwRenderMeshIndex[dwSkinIndex]]);
	}
}

// RenderSkinMesh()

void RenderSkinMesh(DWORD dwSkinIndex,
					LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "RenderSkinMesh()",
								 FALSE);
		return;
	}
#endif
	
	// render mesh
	RenderMesh(g_lpdwRenderMeshIndex[dwSkinIndex],
			   lpMtx);
}

// RenderSkinMeshAlpha()

void RenderSkinMeshAlpha(DWORD dwSkinIndex,
						 LPD3DXMATRIX lpMtx,
						 float fAlpha)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "RenderSkinMeshAlpha()",
								 FALSE);
		return;
	}
#endif
	
	// render mesh
	RenderMeshAlpha(g_lpdwRenderMeshIndex[dwSkinIndex],
					lpMtx,
					fAlpha);
}

// PurgeSkinMeshes()

void PurgeSkinMeshes(void)
{
	// check counter
	if(g_dwNumSkin)
	{
		// counter
		DWORD dwCount;

		// release all skin meshes and buffers
		for(dwCount=0;dwCount<g_dwNumSkin;dwCount++)
		{
			ReleaseCOMPtr(g_lplpSkin[dwCount]);
			ReleaseCOMPtr(g_lplpBoneNames[dwCount]);
			ReleaseCOMPtr(g_lplpBoneTransforms[dwCount]);
			FreeMem((LPVOID*)&g_lplpBoneStructure[dwCount]);
		}
	}

	// free memory
	FreeMem((LPVOID*)&g_lplpSkin);
	FreeMem((LPVOID*)&g_lpdwRenderMeshIndex);
	FreeMem((LPVOID*)&g_lplpBoneNames);
	FreeMem((LPVOID*)&g_lplpBoneTransforms);
	FreeMem((LPVOID*)&g_lplpBoneStructure);
	
	// reset counters
	g_dwNumSkin=0;
	g_dwMaxSkin=0;
}

#ifdef DEBUG_MODE
// CompressX()

BOOL CompressX(LPCSTR lpFile)
{
	LoadingOutput("CompressX");

	// last bytes read
	char cLast1=(char)0xFF;
	char cLast2=(char)0xFF;
	
	// open compiler for copying
	OpenScriptCompiler("~compress.tmp",
					   lpFile);

	// copy bytes excluding spaces
	while(TRUE)
	{
		// get next byte
		char c=ReadScriptChar();

		// check for null byte
		if(c==NULL)
			break;
		
		// check for space
		if(c==' ')
		{
			// check last bytes for alphabetic value
			if(IsAlphabeticChar(cLast1)||
			   IsAlphabeticChar(cLast2))
			{ 
				// add space
				 SaveScriptChar(' ');
			}
		}
		else
		{
			// copy byte
			SaveScriptChar(c);
		}

		// save last bytes
		cLast2=cLast1;
		cLast1=c;
	}

	// close compiler
	CloseScriptCompiler();
	
	// copy file to original
	CopyFile("~compress.tmp",
			 lpFile,
			 FALSE);
	
	// delete temporary file
	DeleteFile("~compress.tmp");

	// ok
	return(TRUE);
}

// CAnimation::EnumerateMeshes()

DWORD CAnimation::EnumerateMeshes(DWORD dwNumRefFrames,
								  LPSTR lpRefFrames,
								  LPSTR* lplpMeshNames)
{
	// counters
	DWORD dwCount=0;
	DWORD dwMeshNamesSize=0;
	DWORD dwMeshNamesPtr=0;

	// enumerate root frame
	EnumerateMeshNode(lpFrameRoot,
					  dwNumRefFrames,
					  lpRefFrames,
					  &dwCount,
					  lplpMeshNames,
					  &dwMeshNamesSize,
					  &dwMeshNamesPtr);

	// return number of meshes
	return(dwCount);
}

// CAnimation::EnumerateMeshChild()

void CAnimation::EnumerateMeshChild(CNode* lpNode,
									DWORD dwNumRefFrames,
									LPSTR lpRefFrames,
									LPDWORD lpdwCount,
									LPSTR* lplpMeshNames,
									LPDWORD lpdwMeshNamesSize,
									LPDWORD lpdwMeshNamesPtr)
{
	// check pointer
	if(lpNode->GetChild())
	{
		// process child
		EnumerateMeshNode(lpNode->GetChild(),
						  dwNumRefFrames,
						  lpRefFrames,
						  lpdwCount,
						  lplpMeshNames,
						  lpdwMeshNamesSize,
						  lpdwMeshNamesPtr);
	}
}

// CAnimation::EnumerateMeshSibling()

void CAnimation::EnumerateMeshSibling(CNode* lpNode,
									  DWORD dwNumRefFrames,
									  LPSTR lpRefFrames,
									  LPDWORD lpdwCount,
									  LPSTR* lplpMeshNames,
									  LPDWORD lpdwMeshNamesSize,
									  LPDWORD lpdwMeshNamesPtr)
{
	// check pointer
	if(lpNode->GetSibling())
	{
		// process sibling
		EnumerateMeshNode(lpNode->GetSibling(),
						  dwNumRefFrames,
						  lpRefFrames,
						  lpdwCount,
						  lplpMeshNames,
						  lpdwMeshNamesSize,
						  lpdwMeshNamesPtr);
	}
}


// CAnimation::EnumerateMeshNode()

void CAnimation::EnumerateMeshNode(CNode* lpNode,
								   DWORD dwNumRefFrames,
								   LPSTR lpRefFrames,
								   LPDWORD lpdwCount,
								   LPSTR* lplpMeshNames,
								   LPDWORD lpdwMeshNamesSize,
								   LPDWORD lpdwMeshNamesPtr)
{
	// counter
	DWORD dwCount;
	
	// process members
	EnumerateMeshSibling(lpNode,
					     dwNumRefFrames,
					     lpRefFrames,
					     lpdwCount,
						 lplpMeshNames,
						 lpdwMeshNamesSize,
						 lpdwMeshNamesPtr);
	EnumerateMeshChild(lpNode,
					   dwNumRefFrames,
					   lpRefFrames,
					   lpdwCount,
					   lplpMeshNames,
					   lpdwMeshNamesSize,
					   lpdwMeshNamesPtr);

	// check for reference frame
	if(dwNumRefFrames)
		for(dwCount=0;dwCount<dwNumRefFrames;dwCount++)
			if(!strcmp(GetStringDivision(lpRefFrames,
										 dwCount),
					   lpNode->GetNodeName()))
				return;

	// cast pointer to frame node data
	LPFRAMENODE lpFN=(LPFRAMENODE)lpNode->GetNodeData();

	// count mesh nodes
	if(lpFN->lpMesh)
		EnumerateMeshNodes(lpFN->lpMesh,
						   lpdwCount,
						   lplpMeshNames,
						   lpdwMeshNamesSize,
						   lpdwMeshNamesPtr);
}

// CAnimation::EnumerateMeshNodes()

void CAnimation::EnumerateMeshNodes(CNode* lpNode,
								    LPDWORD lpdwCount,
									LPSTR* lplpMeshNames,
									LPDWORD lpdwMeshNamesSize,
									LPDWORD lpdwMeshNamesPtr)
{
	// process any siblings
	if(lpNode->GetSibling())
		EnumerateMeshNodes(lpNode->GetSibling(),
						   lpdwCount,
						   lplpMeshNames,
						   lpdwMeshNamesSize,
						   lpdwMeshNamesPtr);

	// cast pointer to mesh node data
	LPMESHNODE lpMN=(LPMESHNODE)lpNode->GetNodeData();

	// check for mesh index
	if(lpMN->dwMeshIndex!=NO_OBJECT)
	{
		// increment counter
		(*lpdwCount)++;

		// set pointer to name
		LPSTR lpName=g_lplpMeshNames[lpMN->dwMeshIndex];
		
		// set size of name
		DWORD dwSize=strlen(lpName)+1;

		// allocate memory for name as needed
		if((*lpdwMeshNamesSize)==0)
		{
			// allocate memory
			if(!AllocMem((LPVOID*)lplpMeshNames,
						 dwSize))
				return;
		}
		else
		{
			// expand memory
			if(!ExpandMem((LPVOID*)lplpMeshNames,
						  (*lpdwMeshNamesSize),
						  dwSize))
				return;
		}

		// increment size
		(*lpdwMeshNamesSize)+=dwSize;

		// cast pointer to mesh names
		LPSTR lpMeshNames=(*lplpMeshNames);

		// copy name
		CopyMemory(&lpMeshNames[(*lpdwMeshNamesPtr)],
				   lpName,
				   dwSize);

		// increment pointer
		(*lpdwMeshNamesPtr)+=dwSize;
	}
}

// CAnimation::EnumerateSkinnedMeshes()

DWORD CAnimation::EnumerateSkinnedMeshes(DWORD dwNumRefFrames,
										 LPSTR lpRefFrames,
										 LPSTR* lplpSkinNames)
{
	// counters
	DWORD dwCount=0;
	DWORD dwSkinNamesSize=0;
	DWORD dwSkinNamesPtr=0;

	// enumerate root frame
	EnumerateSkinnedMeshNode(lpFrameRoot,
							 dwNumRefFrames,
							 lpRefFrames,
							 &dwCount,
							 lplpSkinNames,
							 &dwSkinNamesSize,
							 &dwSkinNamesPtr);

	// return number of meshes
	return(dwCount);
}

// CAnimation::EnumerateSkinnedMeshChild()

void CAnimation::EnumerateSkinnedMeshChild(CNode* lpNode,
										   DWORD dwNumRefFrames,
										   LPSTR lpRefFrames,
										   LPDWORD lpdwCount,
										   LPSTR* lplpSkinNames,
										   LPDWORD lpdwSkinNamesSize,
										   LPDWORD lpdwSkinNamesPtr)
{
	// check pointer
	if(lpNode->GetChild())
	{
		// process child
		EnumerateSkinnedMeshNode(lpNode->GetChild(),
								 dwNumRefFrames,
								 lpRefFrames,
								 lpdwCount,
								 lplpSkinNames,
								 lpdwSkinNamesSize,
								 lpdwSkinNamesPtr);
	}
}

// CAnimation::EnumerateSkinnedMeshSibling()

void CAnimation::EnumerateSkinnedMeshSibling(CNode* lpNode,
											 DWORD dwNumRefFrames,
											 LPSTR lpRefFrames,
											 LPDWORD lpdwCount,
											 LPSTR* lplpSkinNames,
										     LPDWORD lpdwSkinNamesSize,
										     LPDWORD lpdwSkinNamesPtr)
{
	// check pointer
	if(lpNode->GetSibling())
	{
		// process sibling
		EnumerateSkinnedMeshNode(lpNode->GetSibling(),
								 dwNumRefFrames,
								 lpRefFrames,
								 lpdwCount,
								 lplpSkinNames,
								 lpdwSkinNamesSize,
								 lpdwSkinNamesPtr);
	}
}


// CAnimation::EnumerateSkinnedMeshNode()

void CAnimation::EnumerateSkinnedMeshNode(CNode* lpNode,
										  DWORD dwNumRefFrames,
										  LPSTR lpRefFrames,
										  LPDWORD lpdwCount,
										  LPSTR* lplpSkinNames,
										  LPDWORD lpdwSkinNamesSize,
										  LPDWORD lpdwSkinNamesPtr)
{
	// counter
	DWORD dwCount;
	
	// process members
	EnumerateSkinnedMeshSibling(lpNode,
								dwNumRefFrames,
								lpRefFrames,
								lpdwCount,
								lplpSkinNames,
								lpdwSkinNamesSize,
								lpdwSkinNamesPtr);
	EnumerateSkinnedMeshChild(lpNode,
							  dwNumRefFrames,
							  lpRefFrames,
							  lpdwCount,
							  lplpSkinNames,
							  lpdwSkinNamesSize,
							  lpdwSkinNamesPtr);

	// check for reference frame
	if(dwNumRefFrames)
		for(dwCount=0;dwCount<dwNumRefFrames;dwCount++)
			if(!strcmp(GetStringDivision(lpRefFrames,
										 dwCount),
					   lpNode->GetNodeName()))
				return;

	// cast pointer to frame node data
	LPFRAMENODE lpFN=(LPFRAMENODE)lpNode->GetNodeData();

	// count mesh nodes
	if(lpFN->lpMesh)
		EnumerateSkinnedMeshNodes(lpFN->lpMesh,
								  lpdwCount,
								  lplpSkinNames,
								  lpdwSkinNamesSize,
								  lpdwSkinNamesPtr);
}

// CAnimation::EnumerateSkinnedMeshNodes()

void CAnimation::EnumerateSkinnedMeshNodes(CNode* lpNode,
										   LPDWORD lpdwCount,
										   LPSTR* lplpSkinNames,
										   LPDWORD lpdwSkinNamesSize,
										   LPDWORD lpdwSkinNamesPtr)
{
	// process any siblings
	if(lpNode->GetSibling())
		EnumerateSkinnedMeshNodes(lpNode->GetSibling(),
								  lpdwCount,
								  lplpSkinNames,
								  lpdwSkinNamesSize,
								  lpdwSkinNamesPtr);

	// cast pointer to mesh node data
	LPMESHNODE lpMN=(LPMESHNODE)lpNode->GetNodeData();

	// check for mesh index
	if(lpMN->dwSkinIndex!=NO_OBJECT)
	{
		// increment counter
		(*lpdwCount)++;

		// set pointer to name
		LPSTR lpName=g_lplpMeshNames[g_lpdwRenderMeshIndex[lpMN->dwSkinIndex]];
		
		// set size of name
		DWORD dwSize=strlen(lpName)+1;

		// allocate memory for name as needed
		if((*lpdwSkinNamesSize)==0)
		{
			// allocate memory
			if(!AllocMem((LPVOID*)lplpSkinNames,
						 dwSize))
				return;
		}
		else
		{
			// expand memory
			if(!ExpandMem((LPVOID*)lplpSkinNames,
						  (*lpdwSkinNamesSize),
						  dwSize))
				return;
		}

		// increment size
		(*lpdwSkinNamesSize)+=dwSize;

		// cast pointer to skinned mesh names
		LPSTR lpSkinNames=(*lplpSkinNames);

		// copy name
		CopyMemory(&lpSkinNames[(*lpdwSkinNamesPtr)],
				   lpName,
				   dwSize);

		// increment pointer
		(*lpdwSkinNamesPtr)+=dwSize;
	}
}

// CAnimation::EnumerateSkinnedMeshBones()

DWORD CAnimation::EnumerateSkinnedMeshBones(DWORD dwNumRefFrames,
											LPSTR lpRefFrames)
{
	// counter
	DWORD dwCount=0;

	// enumerate root frame
	EnumerateSkinnedMeshBoneNode(lpFrameRoot,
								 dwNumRefFrames,
								 lpRefFrames,
								 &dwCount);

	// return number of bones
	return(dwCount);
}

// CAnimation::EnumerateSkinnedMeshBoneChild()

void CAnimation::EnumerateSkinnedMeshBoneChild(CNode* lpNode,
											   DWORD dwNumRefFrames,
											   LPSTR lpRefFrames,
											   LPDWORD lpdwCount)
{
	// check pointer
	if(lpNode->GetChild())
	{
		// process child
		EnumerateSkinnedMeshBoneNode(lpNode->GetChild(),
									 dwNumRefFrames,
									 lpRefFrames,
									 lpdwCount);
	}
}

// CAnimation::EnumerateSkinnedMeshBoneSibling()

void CAnimation::EnumerateSkinnedMeshBoneSibling(CNode* lpNode,
												 DWORD dwNumRefFrames,
												 LPSTR lpRefFrames,
												 LPDWORD lpdwCount)
{
	// check pointer
	if(lpNode->GetSibling())
	{
		// process sibling
		EnumerateSkinnedMeshBoneNode(lpNode->GetSibling(),
									 dwNumRefFrames,
									 lpRefFrames,
									 lpdwCount);
	}
}


// CAnimation::EnumerateSkinnedMeshBoneNode()

void CAnimation::EnumerateSkinnedMeshBoneNode(CNode* lpNode,
											  DWORD dwNumRefFrames,
											  LPSTR lpRefFrames,
											  LPDWORD lpdwCount)
{
	// counter
	DWORD dwCount;
	
	// process members
	EnumerateSkinnedMeshBoneSibling(lpNode,
									dwNumRefFrames,
									lpRefFrames,
									lpdwCount);
	EnumerateSkinnedMeshBoneChild(lpNode,
								  dwNumRefFrames,
								  lpRefFrames,
								  lpdwCount);

	// check for reference frame
	if(dwNumRefFrames)
		for(dwCount=0;dwCount<dwNumRefFrames;dwCount++)
			if(!strcmp(GetStringDivision(lpRefFrames,
										 dwCount),
					   lpNode->GetNodeName()))
				return;

	// cast pointer to frame node data
	LPFRAMENODE lpFN=(LPFRAMENODE)lpNode->GetNodeData();

	// count mesh nodes
	if(lpFN->lpMesh)
		EnumerateSkinnedMeshBoneNodes(lpFN->lpMesh,
									  lpdwCount);
}

// CAnimation::EnumerateSkinnedMeshBoneNodes()

void CAnimation::EnumerateSkinnedMeshBoneNodes(CNode* lpNode,
											   LPDWORD lpdwCount)
{
	// process any siblings
	if(lpNode->GetSibling())
		EnumerateSkinnedMeshBoneNodes(lpNode->GetSibling(),
									  lpdwCount);

	// cast pointer to mesh node data
	LPMESHNODE lpMN=(LPMESHNODE)lpNode->GetNodeData();

	// check for mesh index
	if(lpMN->dwSkinIndex!=NO_OBJECT)
		(*lpdwCount)+=g_lplpSkin[lpMN->dwSkinIndex]->GetNumBones();
}

// CAnimation::FindFrame()

CNode* CAnimation::FindFrame(LPSTR lpName)
{
	// search root frame
	return(lpFrameRoot->FindNode(lpName));
}

// CAnimation::EnumerateKeyframes()

DWORD CAnimation::EnumerateKeyframes(CNode* lpKeyEnum)
{
	// reset keyframe counter
	DWORD dwCount=0;

	// enumerate root frame
	EnumerateKeyframeNode(lpFrameRoot,
						  lpKeyEnum,
						  &dwCount);

	// return number of keyframes
	return(dwCount);
}

// CAnimation::EnumerateKeyframeChild()

void CAnimation::EnumerateKeyframeChild(CNode* lpNode,
										CNode* lpKeyEnum,
										LPDWORD lpdwCount)
{
	// check pointer
	if(lpNode->GetChild())
	{
		// process child
		EnumerateKeyframeNode(lpNode->GetChild(),
							  lpKeyEnum,
							  lpdwCount);
	}
}

// CAnimation::EnumerateKeyframeSibling()

void CAnimation::EnumerateKeyframeSibling(CNode* lpNode,
										  CNode* lpKeyEnum,
										  LPDWORD lpdwCount)
{
	// check pointer
	if(lpNode->GetSibling())
	{
		// process sibling
		EnumerateKeyframeNode(lpNode->GetSibling(),
							  lpKeyEnum,
							  lpdwCount);
	}
}

// CAnimation::EnumerateKeyframeNode()

void CAnimation::EnumerateKeyframeNode(CNode* lpNode,
									   CNode* lpKeyEnum,
									   LPDWORD lpdwCount)
{
	// process members
	EnumerateKeyframeSibling(lpNode,
						     lpKeyEnum,
						     lpdwCount);
	EnumerateKeyframeChild(lpNode,
						   lpKeyEnum,
						   lpdwCount);

	// cast pointer to frame data
	LPFRAMENODE lpFN=(LPFRAMENODE)lpNode->GetNodeData();

	// check for animation data
	if(lpFN->lpAnim)
		EnumerateKeyframeAnimNode(lpFN->lpAnim,
								  lpKeyEnum,
								  lpdwCount);
}

// CAnimation::EnumerateKeyframeAnimNode()

void CAnimation::EnumerateKeyframeAnimNode(CNode* lpNode,
										   CNode* lpKeyEnum,
										   LPDWORD lpdwCount)
{
	// pointers to keys
	LPROTATIONKEY lpkR;
	LPSCALEKEY    lpkS;
	LPPOSITIONKEY lpkP;
	LPMATRIXKEY   lpkM;

	// counter
	DWORD dwCount;

	// cast pointer to animation data
	LPANIMNODE lpAN=(LPANIMNODE)lpNode->GetNodeData();
	
	// check key type
	switch(lpAN->dwKeyType)
	{
	// rotation keys
	case(ANIMKEY_ROTATION):

		// cast pointer
		lpkR=(LPROTATIONKEY)lpAN->lpKeys;

		// add keyframes as needed
		for(dwCount=0;dwCount<lpAN->dwNumKeys;dwCount++)
			if(!FindKeyframe(lpKeyEnum,
							 lpkR[dwCount].dwTime))
			{
				// add sibling keyframe
				CNode* lpNewKey=lpKeyEnum->AddSibling();

				// increment key counter
				(*lpdwCount)++;

				// set time signature
				lpNewKey->SetNodeData((LPVOID)lpkR[dwCount].dwTime);
			}

		// ok
		break;

	// scale keys
	case(ANIMKEY_SCALE):

		// cast pointer
		lpkS=(LPSCALEKEY)lpAN->lpKeys;

		// add keyframes as needed
		for(dwCount=0;dwCount<lpAN->dwNumKeys;dwCount++)
			if(!FindKeyframe(lpKeyEnum,
							 lpkS[dwCount].dwTime))
			{
				// add sibling keyframe
				CNode* lpNewKey=lpKeyEnum->AddSibling();

				// increment key counter
				(*lpdwCount)++;

				// set time signature
				lpNewKey->SetNodeData((LPVOID)lpkS[dwCount].dwTime);
			}

		// ok
		break;

	// position keys
	case(ANIMKEY_POSITION):

		// cast pointer
		lpkP=(LPPOSITIONKEY)lpAN->lpKeys;

		// add keyframes as needed
		for(dwCount=0;dwCount<lpAN->dwNumKeys;dwCount++)
			if(!FindKeyframe(lpKeyEnum,
							 lpkP[dwCount].dwTime))
			{
				// add sibling keyframe
				CNode* lpNewKey=lpKeyEnum->AddSibling();

				// increment key counter
				(*lpdwCount)++;

				// set time signature
				lpNewKey->SetNodeData((LPVOID)lpkP[dwCount].dwTime);
			}

		// ok
		break;

	// matrix keys
	case(ANIMKEY_MATRIX):

		// cast pointer
		lpkM=(LPMATRIXKEY)lpAN->lpKeys;

		// add keyframes as needed
		for(dwCount=0;dwCount<lpAN->dwNumKeys;dwCount++)
			if(!FindKeyframe(lpKeyEnum,
							 lpkM[dwCount].dwTime))
			{
				// add sibling keyframe
				CNode* lpNewKey=lpKeyEnum->AddSibling();

				// increment key counter
				(*lpdwCount)++;

				// set time signature
				lpNewKey->SetNodeData((LPVOID)lpkM[dwCount].dwTime);
			}

		// ok
		break;
	}
	
	// process siblings
	if(lpNode->GetSibling())
		EnumerateKeyframeAnimNode(lpNode->GetSibling(),
								  lpKeyEnum,
								  lpdwCount);
}

// CAnimation::FindMinKeyframe()

DWORD CAnimation::FindMinKeyframe(CNode* lpKeyEnum)
{
	// set current node
	CNode* lpNode=lpKeyEnum;

	// reset minimum value
	DWORD dwMin=0xFFFFFFFF;

	// find minimum value
	while(lpNode->GetSibling())
	{
		// set node to its sibling
		lpNode=lpNode->GetSibling();

		// compare values
		if((DWORD)lpNode->GetNodeData()<dwMin)
			dwMin=(DWORD)lpNode->GetNodeData();
	}

	// return result
	return(dwMin);
}

// CAnimation::FindMaxKeyframe()

DWORD CAnimation::FindMaxKeyframe(CNode* lpKeyEnum)
{
	// set current node
	CNode* lpNode=lpKeyEnum;

	// reset maximum value
	DWORD dwMax=0;

	// find maximum value
	while(lpNode->GetSibling())
	{
		// set node to its sibling
		lpNode=lpNode->GetSibling();

		// compare values
		if((DWORD)lpNode->GetNodeData()>dwMax)
			dwMax=(DWORD)lpNode->GetNodeData();
	}

	// return result
	return(dwMax);
}

// CAnimation::SortKeyframes()

void CAnimation::SortKeyframes(CNode* lpKeyEnum)
{
	// done flag
	BOOL bDone=FALSE;
	
	// perform bubble sort
	while(!bDone)
	{
		// set done flag
		bDone=TRUE;

		// set current node
		CNode* lpNode=lpKeyEnum;

		// sort time signatures
		while(lpNode->GetSibling())
		{
			// compare values
			if((DWORD)lpNode->GetNodeData()>(DWORD)lpNode->GetSibling()->GetNodeData())
			{
				// swap values
				LPVOID lpTemp=lpNode->GetNodeData();
				lpNode->SetNodeData(lpNode->GetSibling()->GetNodeData());
				lpNode->GetSibling()->SetNodeData(lpTemp);

				// reset flag
				bDone=FALSE;
			}
			
			// set node to its sibling
			lpNode=lpNode->GetSibling();
		}
	}
}

// CAnimation::FindKeyframe()

CNode* CAnimation::FindKeyframe(CNode* lpKeyEnum,
								DWORD dwTimeSig)
{
	// check root for siblings
	if(lpKeyEnum->GetSibling())
		return(FindKeyframeNode(lpKeyEnum->GetSibling(),
								dwTimeSig));
	else
		return(NULL);
}

// CAnimation::FindKeyframeNode()

CNode* CAnimation::FindKeyframeNode(CNode* lpKeyEnum,
									DWORD dwTimeSig)
{
	// compare time signature with node data
	if((DWORD)lpKeyEnum->GetNodeData()==dwTimeSig)
		return(lpKeyEnum);
	else
	{
		// check for siblings
		if(lpKeyEnum->GetSibling())
			return(FindKeyframeNode(lpKeyEnum->GetSibling(),
									dwTimeSig));
		else
			return(NULL);
	}
}

// CAnimation::CaptureKeyframes()

void CAnimation::CaptureKeyframes(DWORD dwNumRefFrames,
								  LPSTR lpRefFrames,
								  DWORD dwNumMeshes,
								  LPD3DXMATRIX lpKeys)
{
	// set pointers to matrix groups
	LPD3DXMATRIX lpRefKeys=&lpKeys[0];
	LPD3DXMATRIX lpMeshKeys=&lpKeys[dwNumRefFrames];
	LPD3DXMATRIX lpBoneKeys=&lpKeys[dwNumRefFrames+dwNumMeshes];

	// set indices
	DWORD dwMeshIdx=0;
	DWORD dwBoneIdx=0;
	
	// capture root frame
	CaptureKeyframeNode(lpFrameRoot,
						dwNumRefFrames,
						lpRefFrames,
						lpRefKeys,
						lpMeshKeys,
						&dwMeshIdx,
						lpBoneKeys,
						&dwBoneIdx);
}

// CAnimation::CaptureKeyframeChild()

void CAnimation::CaptureKeyframeChild(CNode* lpNode,
									  DWORD dwNumRefFrames,
									  LPSTR lpRefFrames,
									  LPD3DXMATRIX lpRefKeys,
									  LPD3DXMATRIX lpMeshKeys,
									  LPDWORD lpdwMeshIdx,
									  LPD3DXMATRIX lpBoneKeys,
									  LPDWORD lpdwBoneIdx)
{
	// check pointer
	if(lpNode->GetChild())
	{
		// process child
		CaptureKeyframeNode(lpNode->GetChild(),
						    dwNumRefFrames,
							lpRefFrames,
							lpRefKeys,
							lpMeshKeys,
							lpdwMeshIdx,
							lpBoneKeys,
							lpdwBoneIdx);
	}
}

// CAnimation::CaptureKeyframeSibling()

void CAnimation::CaptureKeyframeSibling(CNode* lpNode,
										DWORD dwNumRefFrames,
										LPSTR lpRefFrames,
										LPD3DXMATRIX lpRefKeys,
										LPD3DXMATRIX lpMeshKeys,
										LPDWORD lpdwMeshIdx,
										LPD3DXMATRIX lpBoneKeys,
										LPDWORD lpdwBoneIdx)
{
	// check pointer
	if(lpNode->GetSibling())
	{
		// process sibling
		CaptureKeyframeNode(lpNode->GetSibling(),
						    dwNumRefFrames,
							lpRefFrames,
							lpRefKeys,
							lpMeshKeys,
							lpdwMeshIdx,
							lpBoneKeys,
							lpdwBoneIdx);
	}
}

// CAnimation::CaptureKeyframeNode()

void CAnimation::CaptureKeyframeNode(CNode* lpNode,
									 DWORD dwNumRefFrames,
									 LPSTR lpRefFrames,
									 LPD3DXMATRIX lpRefKeys,
									 LPD3DXMATRIX lpMeshKeys,
									 LPDWORD lpdwMeshIdx,
									 LPD3DXMATRIX lpBoneKeys,
									 LPDWORD lpdwBoneIdx)
{
	// counter
	DWORD dwCount;
	
	// process members
	CaptureKeyframeSibling(lpNode,
					       dwNumRefFrames,
						   lpRefFrames,
					       lpRefKeys,
					       lpMeshKeys,
						   lpdwMeshIdx,
					       lpBoneKeys,
						   lpdwBoneIdx);
	CaptureKeyframeChild(lpNode,
					     dwNumRefFrames,
					     lpRefFrames,
					     lpRefKeys,
					     lpMeshKeys,
						 lpdwMeshIdx,
					     lpBoneKeys,
						 lpdwBoneIdx);

	// check for reference frame
	if(dwNumRefFrames)
		for(dwCount=0;dwCount<dwNumRefFrames;dwCount++)
			if(!strcmp(GetStringDivision(lpRefFrames,
										 dwCount),
					   lpNode->GetNodeName()))
			{
				// cast pointer to frame node data
				LPFRAMENODE lpFN=(LPFRAMENODE)lpNode->GetNodeData();

				// copy matrix
				lpRefKeys[dwCount]=lpFN->mWorld;

				// all done
				return;
			}

	// cast pointer to frame node data
	LPFRAMENODE lpFN=(LPFRAMENODE)lpNode->GetNodeData();

	// capture mesh nodes
	if(lpFN->lpMesh)
		CaptureKeyframeMeshNode(lpFN,
								lpFN->lpMesh,
								lpMeshKeys,
								lpdwMeshIdx,
								lpBoneKeys,
								lpdwBoneIdx);
}

// void CAnimation::CaptureKeyframeMeshNode()

void CAnimation::CaptureKeyframeMeshNode(LPFRAMENODE lpFN,
										 CNode* lpMeshNode,
										 LPD3DXMATRIX lpMeshKeys,
										 LPDWORD lpdwMeshIdx,
										 LPD3DXMATRIX lpBoneKeys,
										 LPDWORD lpdwBoneIdx)
{
	// process any siblings
	if(lpMeshNode->GetSibling())
		CaptureKeyframeMeshNode(lpFN,
								lpMeshNode->GetSibling(),
								lpMeshKeys,
								lpdwMeshIdx,
								lpBoneKeys,
								lpdwBoneIdx);

	// cast pointer to mesh node data
	LPMESHNODE lpMN=(LPMESHNODE)lpMeshNode->GetNodeData();

	// check for meshes
	if(lpMN->dwMeshIndex!=NO_OBJECT)
	{
		// copy matrix
		lpMeshKeys[(*lpdwMeshIdx)]=lpFN->mWorld;

		// increment mesh key counter
		(*lpdwMeshIdx)++;
	}
	else if(lpMN->dwSkinIndex!=NO_OBJECT)
	{
		// update bone structure
		UpdateBoneStructure(lpMN->dwSkinIndex);
							
		// copy matrices
		CopyMemory(&lpBoneKeys[(*lpdwBoneIdx)],
				   g_lplpBoneStructure[lpMN->dwSkinIndex],
				   sizeof(D3DXMATRIX)*g_lplpSkin[lpMN->dwSkinIndex]->GetNumBones());

		// increment bone key pointer
		(*lpdwBoneIdx)+=g_lplpSkin[lpMN->dwSkinIndex]->GetNumBones();
	}
}

// CreateXFAFromX()

BOOL CreateXFAFromX(LPCSTR lpDestFile,
					LPCSTR lpSrcFile,
					LPKEYFRAMES lpKF)
{
	LoadingOutput("CreateXFAFromX");

	// keyframes data
	KEYFRAMES kf;

	// number of reference frames
	DWORD dwNumRefFrames;

	// animation object
	CAnimation* lpAnim=NULL;

	// number of meshes
	DWORD dwNumMeshes;

	// number of skinned meshes
	DWORD dwNumSkins;

	// number of skinned mesh bones
	DWORD dwNumBones;

	// counter
	DWORD dwCount;

	// pointer to reference frames
	LPSTR lpRefFrames=NULL;

	// number of keyframes
	DWORD dwNumKeys;

	// number of matrices per key
	DWORD dwNumMtx;
	
	// keyframe enumeration node
	CNode* lpKeyEnum;

	// keyframe values
	DWORD dwMinKey;
	DWORD dwMaxKey;

	// time increment value
	float fTimeInc;

	// keyframe matrices
	LPD3DXMATRIX lpKeys=NULL;

	// current keyframe node
	CNode* lpNode;

	// pointer to mesh names
	LPSTR lpMeshNames=NULL;

	// pointer to skinned mesh names
	LPSTR lpSkinNames=NULL;

	// validate keyframes data
	if(lpKF)
		kf=(*lpKF);
	else
	{
		kf.lpRefFrames=NULL;
		kf.dwReorderFlag=KEYFRAMES_KEEPEXISTING;
	}
	
	// set number of reference frames
	if(kf.lpRefFrames)
	{
		// allocate memory for reference frames
		if(!AllocMem((LPVOID*)&lpRefFrames,
					 strlen(kf.lpRefFrames)+1))
			return(FALSE);
		
		// copy string
		strcpy(lpRefFrames,
			   kf.lpRefFrames);
		
		// divide string
		dwNumRefFrames=DivideString(lpRefFrames);
	}
	else
	{
		// no reference frames
		dwNumRefFrames=0;
	}
	
	// compress source file
	CompressX(lpSrcFile);

	// create new animation object
	AllocClassObject(lpAnim,
					 CAnimation)

	// load animation
	lpAnim->LoadFromX(lpSrcFile);

	// verify reference frames
	if(dwNumRefFrames)
		for(dwCount=0;dwCount<dwNumRefFrames;dwCount++)
			if(!lpAnim->FindFrame(GetStringDivision(lpRefFrames,dwCount)))
				DisplayErrorMessage("Reference frame not found.",
									GetStringDivision(lpRefFrames,dwCount),
									FALSE);

	// count meshes
	dwNumMeshes=lpAnim->EnumerateMeshes(dwNumRefFrames,
									    lpRefFrames,
										&lpMeshNames);

	// count skinned meshes
	dwNumSkins=lpAnim->EnumerateSkinnedMeshes(dwNumRefFrames,
										      lpRefFrames,
											  &lpSkinNames);

	// count skinned mesh bones
	dwNumBones=lpAnim->EnumerateSkinnedMeshBones(dwNumRefFrames,
												 lpRefFrames);

	// compute number of matrices
	dwNumMtx=dwNumRefFrames+dwNumMeshes+dwNumBones;

	// add keyframe node object
	AllocClassObject(lpKeyEnum,
					 CNode);

	// check reorder flag
	switch(kf.dwReorderFlag)
	{
	// keep existing
	case(KEYFRAMES_KEEPEXISTING):
		// enumerate existing keyframes
		dwNumKeys=lpAnim->EnumerateKeyframes(lpKeyEnum);
		
		// ok
		break;

	// add to existing
	case(KEYFRAMES_ADDTOEXISTING):
		// enumerate existing keyframes
		dwNumKeys=lpAnim->EnumerateKeyframes(lpKeyEnum);

		// add specified keyframes
		for(dwCount=0;dwCount<kf.dwNumFrames;dwCount++)
		{
			// add sibling keyframe
			CNode* lpNewKey=lpKeyEnum->AddSibling();

			// increment key counter
			dwNumKeys++;

			// set time signature
			lpNewKey->SetNodeData((LPVOID)kf.lpdwFrames[dwCount]);
		}

		// ok
		break;

	// evenly timed
	case(KEYFRAMES_EVENLYTIMED):
		// enumerate existing keyframes
		lpAnim->EnumerateKeyframes(lpKeyEnum);

		// find min & max values
		dwMinKey=lpAnim->FindMinKeyframe(lpKeyEnum);
		dwMaxKey=lpAnim->FindMaxKeyframe(lpKeyEnum);

		// delete keyframe node
		lpKeyEnum->DeleteNode();

		// compute time increment
		fTimeInc=(((float)dwMaxKey)-((float)dwMinKey))/((float)(kf.dwNumFrames-1));

		// reset key counter
		dwNumKeys=0;

		// add evenly-spaced time signatures
		for(dwCount=0;dwCount<kf.dwNumFrames;dwCount++)
		{
			// add sibling keyframe
			CNode* lpNewKey=lpKeyEnum->AddSibling();

			// increment key counter
			dwNumKeys++;

			// compute time signature
			DWORD dwTime=dwMinKey+((DWORD)(fTimeInc*((float)dwCount)));
			
			// set time signature
			lpNewKey->SetNodeData((LPVOID)dwTime);
		}

		// ok
		break;

	// specify new
	case(KEYFRAMES_SPECIFYNEW):
		// reset key counter
		dwNumKeys=0;

		// add specified time signatures
		for(dwCount=0;dwCount<kf.dwNumFrames;dwCount++)
		{
			// add sibling keyframe
			CNode* lpNewKey=lpKeyEnum->AddSibling();

			// increment key counter
			dwNumKeys++;

			// set time signature
			lpNewKey->SetNodeData((LPVOID)kf.lpdwFrames[dwCount]);
		}

		// ok
		break;
	}

	// sort keyframes
	lpAnim->SortKeyframes(lpKeyEnum);

	// allocate memory for keyframe matrices
	if(!AllocMem((LPVOID*)&lpKeys,
				 sizeof(D3DXMATRIX)*dwNumMtx*dwNumKeys))
		return(FALSE);

	// set current keyframe node
	lpNode=lpKeyEnum;
	
	// capture key matrices
	for(dwCount=0;dwCount<dwNumKeys;dwCount++)
	{
		// get next keyframe
		lpNode=lpNode->GetSibling();
		
		// set time for this keyframe
		lpAnim->SetTime((DWORD)lpNode->GetNodeData());

		// capture keyframes
		lpAnim->CaptureKeyframes(dwNumRefFrames,
								 lpRefFrames,
								 dwNumMeshes,
								 &lpKeys[dwNumMtx*dwCount]);
	}
	
	// open compiler for copying
	OpenScriptCompiler(lpDestFile,
					   lpSrcFile);

	// output number of reference frames
	SaveScriptLong((long)dwNumRefFrames);

	// output number of meshes
	SaveScriptLong((long)dwNumMeshes);

	// output number of skinned meshes
	SaveScriptLong((long)dwNumSkins);

	// output number of skinned mesh bones
	SaveScriptLong((long)dwNumBones);

	// output number of keyframes
	SaveScriptLong((long)dwNumKeys);

	// set current keyframe node
	lpNode=lpKeyEnum;
	
	// output time signatures
	for(dwCount=0;dwCount<dwNumKeys;dwCount++)
	{
		// get next keyframe
		lpNode=lpNode->GetSibling();

		// get time for this keyframe
		DWORD dwTime=(DWORD)lpNode->GetNodeData();
		
		// output time value
		SaveScriptLong((long)dwTime);
	}

	// cast pointer to matrix data
	float* lpMtxData=(float*)lpKeys;
	
	// output matrices
	for(dwCount=0;dwCount<dwNumMtx*dwNumKeys*16;dwCount++)
		SaveScriptFloat(lpMtxData[dwCount]);

	// output x file data
	if(!AppendXToXAF(lpSrcFile,
					 dwNumMeshes,
					 lpMeshNames,
					 dwNumSkins,
					 lpSkinNames))
		return(FALSE);

	// close script compiler
	CloseScriptCompiler();

	// free keyframe matrices
	FreeMem((LPVOID*)&lpKeys);

	// delete keyframe node
	lpKeyEnum->DeleteNode();

	// free keyframe node
	FreeClassObject(lpKeyEnum);

	// release animation
	lpAnim->Release();

	// free animation object
	FreeClassObject(lpAnim);

	// free reference frames
	FreeMem((LPVOID*)&lpRefFrames);

	// free skinned mesh names
	FreeMem((LPVOID*)&lpSkinNames);

	// free mesh names
	FreeMem((LPVOID*)&lpMeshNames);

	// reset graphics objects
	ResetGraphicsObjects();
	
	// ok
	return(TRUE);
}

// AppendXToXAF()

BOOL AppendXToXAF(LPCSTR lpSrcFile,
				  DWORD dwNumMesh,
				  LPSTR lpMeshNames,
				  DWORD dwNumSkin,
				  LPSTR lpSkinNames)
{
	LoadingOutput("AppendXToXAF");

	// counter
	DWORD dwCount;
	
	// output x-file reference data
	if(!AppendXRefData(lpSrcFile))
		return(FALSE);

	// output meshes
	for(dwCount=0;dwCount<dwNumMesh;dwCount++)
		if(!AppendXMeshData(lpSrcFile,
							GetStringDivision(lpMeshNames,
											  dwCount)))
			return(FALSE);
	
	// output skinned meshes
	for(dwCount=0;dwCount<dwNumSkin;dwCount++)
		if(!AppendXMeshData(lpSrcFile,
							GetStringDivision(lpSkinNames,
											  dwCount)))
			return(FALSE);
	
	// ok
	return(TRUE);
}

// AppendXRefData()

BOOL AppendXRefData(LPCSTR lpSrcFile)
{
	LoadingOutput("AppendXRefData");

	// reset script source file
	if(!ResetScriptCompilerSource(lpSrcFile))
		return(FALSE);

	// copy header
	CopyScriptBytes(16);

	// copy data to first template
	char c=CopyXToFirstTemplate();

	// loop until done
	while(TRUE)
	{
		// stop at first frame
		if(c=='F')
			break;
		
		// copy data to next template
		c=CopyXToNextTemplate(c);

		// save byte
		SaveScriptChar(c);

		// copy data to first template
		c=CopyXToFirstTemplate();
	}
	
	// ok
	return(TRUE);
}

// CopyXToFirstTemplate()

char CopyXToFirstTemplate(void)
{
	LoadingOutput("CopyXToFirstTemplate");

	// loop until alphabetic character is found
	while(TRUE)
	{
		// read next byte
		char c=ReadScriptChar();

		// check for alphabetic character
		if(IsAlphabeticChar(c))
			return(c);
		else
			SaveScriptChar(c);
	}
}

// CopyXToNextTemplate()

char CopyXToNextTemplate(char c)
{
	LoadingOutput("CopyXToNextTemplate");

	// loop until { is found
	while(TRUE)
	{
		// save byte
		SaveScriptChar(c);

		// check for {
		if(c=='{')
			break;

		// read next byte
		c=ReadScriptChar();
	}

	// read next byte
	c=ReadScriptChar();

	// set bracket count
	DWORD dwNumBrkt=1;

	// loop until bracket is closed
	while(dwNumBrkt)
	{
		// save byte
		SaveScriptChar(c);

		// check for brackets
		if(c=='{') dwNumBrkt++;
		if(c=='}') dwNumBrkt--;

		// read next byte
		c=ReadScriptChar();
	}

	// return carry-over byte
	return(c);
}

// AppendXMeshData()

BOOL AppendXMeshData(LPCSTR lpSrcFile,
					 LPSTR lpName)
{
	LoadingOutput("AppendXMeshData");

	// counter
	DWORD dwCount;

	// carry-over byte
	char c;
	
	// reset script source file
	if(!ResetScriptCompilerSource(lpSrcFile))
		return(FALSE);

	// find mesh
	while(TRUE)
	{
		LockGlobalMsg()

		// get name of next mesh
		c=GetNextXMeshName(g_lpMsg);
		
		// compare names
		if(!strcmp(g_lpMsg,
				   lpName))
		{
			UnlockGlobalMsg()
			break;
		}

		UnlockGlobalMsg()
	}

	// output mesh identifier
	SaveScriptChar('M');
	SaveScriptChar('e');
	SaveScriptChar('s');
	SaveScriptChar('h');
	SaveScriptChar(' ');

	LockGlobalMsg()

	// output mesh name
	for(dwCount=0;dwCount<strlen(g_lpMsg);dwCount++)
		SaveScriptChar(g_lpMsg[dwCount]);

	UnlockGlobalMsg()

	// copy to next template
	CopyXToNextTemplate(c);

	// ok
	return(TRUE);
}

// GetNextXMeshName()

char GetNextXMeshName(LPSTR lpName)
{
	LoadingOutput("GetNextXMeshName");

	// read bytes until "Mesh " is found
	while(TRUE)
	{
		// check next byte
		if(ReadScriptChar()=='M')
			if(ReadScriptChar()=='e')
				if(ReadScriptChar()=='s')
					if(ReadScriptChar()=='h')
						if(ReadScriptChar()==' ')
						{
							// reset counter
							DWORD dwCount=0;
							
							// read until ' ' or '}' is found
							while(TRUE)
							{
								// read next byte
								char c=ReadScriptChar();

								// check byte
								if(c==' '||
								   c=='{')
								{
									// place null terminator
									lpName[dwCount]=0;

									// return carry-over byte
									return(c);
								}
								
								// save byte
								lpName[dwCount++]=c;
							}
						}
	}
}
#endif

// GetNextAnimation()

DWORD GetNextAnimation(void)
{
	LoadingOutput("GetNextAnimation");

	// check counter
	if(g_dwNumAnim==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwAnimNumRef,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwAnimNumMesh,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwAnimNumSkin,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwAnimNumBones,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwAnimNumMtx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwAnimNumRender,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwAnimNumKeys,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpdwAnimSkinNumBones,
					 sizeof(LPDWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpdwAnimKeyTimeSig,
					 sizeof(LPDWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpmAnimKeyframes,
					 sizeof(LPD3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpmAnimCurrentKey,
					 sizeof(LPD3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwAnimBaseMeshIdx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwAnimBaseSkinIdx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAnim=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumAnim>=g_dwMaxAnim)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwAnimNumRef,
					  sizeof(DWORD)*g_dwMaxAnim,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwAnimNumMesh,
					  sizeof(DWORD)*g_dwMaxAnim,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwAnimNumSkin,
					  sizeof(DWORD)*g_dwMaxAnim,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwAnimNumBones,
					  sizeof(DWORD)*g_dwMaxAnim,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwAnimNumMtx,
					  sizeof(DWORD)*g_dwMaxAnim,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwAnimNumRender,
					  sizeof(DWORD)*g_dwMaxAnim,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwAnimNumKeys,
					  sizeof(DWORD)*g_dwMaxAnim,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpdwAnimSkinNumBones,
					  sizeof(LPDWORD)*g_dwMaxAnim,
					  sizeof(LPDWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpdwAnimKeyTimeSig,
					  sizeof(LPDWORD)*g_dwMaxAnim,
					  sizeof(LPDWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpmAnimKeyframes,
					  sizeof(LPD3DXMATRIX)*g_dwMaxAnim,
					  sizeof(LPD3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpmAnimCurrentKey,
					  sizeof(LPD3DXMATRIX)*g_dwMaxAnim,
					  sizeof(LPD3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwAnimBaseMeshIdx,
					  sizeof(DWORD)*g_dwMaxAnim,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwAnimBaseSkinIdx,
					  sizeof(DWORD)*g_dwMaxAnim,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxAnim+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumAnim;

	// increment counter
	g_dwNumAnim++;

	// return index
	return(dwIndex);
}

// AddAnimation()

DWORD AddAnimation(LPCSTR lpFile,
				   BOOL bNormals,
				   BOOL bOptimize)
{
	LoadingOutput("AddAnimation");

	// no data file ref flag
	BOOL bNoDFR=FALSE;
	
	// number of bytes read
	DWORD dwBytesRead;

	// counter
	DWORD dwCount;

	// get return index
	DWORD dwIndex=GetNextAnimation();

	// check index
	if(dwIndex==NO_OBJECT)
		return(NO_OBJECT);

	// find data file reference
	DATAFILEREF dfr=FindDataFileRef((LPSTR)lpFile);

	// load animation file
	if(!dfr.lpData)
	{
		// set flag
		bNoDFR=TRUE;

		// get file size
		dfr.dwSize=GetFileLength((LPSTR)lpFile);

		// check file size
		if(dfr.dwSize==0)
			return(NO_OBJECT);
		
		// allocate memory
		if(!AllocMem(&dfr.lpData,
					 dfr.dwSize))
			return(NO_OBJECT);

		// open file
		HANDLE hFile=CreateFile(lpFile,
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
			DisplayErrorMessage("Unable to open animation file.",
								(LPSTR)lpFile,
								FALSE);
			FreeMem(&dfr.lpData);
			return(NO_OBJECT);
		}
		else
			AddWinResource;
		
		// read data
		ReadFile(hFile,
				 dfr.lpData,
				 dfr.dwSize,
				 &dwBytesRead,
				 NULL);

		// check for error
		if(dwBytesRead!=dfr.dwSize)
		{
			DisplayErrorMessage("Unable to read animation file data.",
								(LPSTR)lpFile,
								FALSE);
			FreeMem(&dfr.lpData);
			if(CloseHandle(hFile))
				RemoveWinResource;
			return(NO_OBJECT);
		}

		// close file
		if(CloseHandle(hFile))
			RemoveWinResource;
	}

	// cast DWORD pointer to data
	LPDWORD dwData=(LPDWORD)dfr.lpData;

	// read number of reference frames
	g_lpdwAnimNumRef[dwIndex]=dwData[0];

	// read number of meshes
	g_lpdwAnimNumMesh[dwIndex]=dwData[1];

	// read number of skinned meshes
	g_lpdwAnimNumSkin[dwIndex]=dwData[2];

	// read number of skinned mesh bones
	g_lpdwAnimNumBones[dwIndex]=dwData[3];

	// compute number of matrices per keyframe
	g_lpdwAnimNumMtx[dwIndex]=g_lpdwAnimNumRef[dwIndex]+
							  g_lpdwAnimNumMesh[dwIndex]+
							  g_lpdwAnimNumBones[dwIndex];

	// compute number of render matrices
	g_lpdwAnimNumRender[dwIndex]=g_lpdwAnimNumMtx[dwIndex]-
							     g_lpdwAnimNumRef[dwIndex];

	// read number of keyframes
	g_lpdwAnimNumKeys[dwIndex]=dwData[4];

	// allocate memory for time signatures
	if(!AllocMem((LPVOID*)&g_lplpdwAnimKeyTimeSig[dwIndex],
				 sizeof(DWORD)*g_lpdwAnimNumKeys[dwIndex]))
	{
		if(bNoDFR)
			FreeMem(&dfr.lpData);
		return(NO_OBJECT);
	}

	// read time signatures
	CopyMemory(g_lplpdwAnimKeyTimeSig[dwIndex],
			   &dwData[5],
			   sizeof(DWORD)*g_lpdwAnimNumKeys[dwIndex]);

	// allocate memory for keyframes
	if(!AllocMem((LPVOID*)&g_lplpmAnimKeyframes[dwIndex],
				 sizeof(D3DXMATRIX)*g_lpdwAnimNumKeys[dwIndex]*g_lpdwAnimNumMtx[dwIndex]))
	{
		if(bNoDFR)
			FreeMem(&dfr.lpData);
		return(NO_OBJECT);
	}

	// read keyframe matrices
	CopyMemory(g_lplpmAnimKeyframes[dwIndex],
 			   &dwData[g_lpdwAnimNumKeys[dwIndex]+5],
			   sizeof(D3DXMATRIX)*g_lpdwAnimNumKeys[dwIndex]*g_lpdwAnimNumMtx[dwIndex]);

	// set base mesh indices
	g_lpdwAnimBaseMeshIdx[dwIndex]=g_dwNumMesh;
	g_lpdwAnimBaseSkinIdx[dwIndex]=g_dwNumSkin;

	// add meshes from x-file data
	if(!AddMeshesFromXFA(dwIndex,
						 &dfr,
						 bNormals,
						 bOptimize))
	{
		if(bNoDFR)
			FreeMem(&dfr.lpData);
		return(NO_OBJECT);
	}
	
	// check number of skinned meshes
	if(g_lpdwAnimNumSkin[dwIndex])
	{
		// allocate memory for bone quantities
		if(!AllocMem((LPVOID*)&g_lplpdwAnimSkinNumBones[dwIndex],
					 sizeof(DWORD)*g_lpdwAnimNumSkin[dwIndex]))
		{
			if(bNoDFR)
				FreeMem(&dfr.lpData);
			return(NO_OBJECT);
		}

		// cast pointer to bone quantities
		LPDWORD lpNumBones=g_lplpdwAnimSkinNumBones[dwIndex];
		
		// set bone quantities
		for(dwCount=0;dwCount<g_lpdwAnimNumSkin[dwIndex];dwCount++)
			lpNumBones[dwCount]=g_lplpSkin[g_lpdwAnimBaseSkinIdx[dwIndex]+dwCount]->GetNumBones();
	}
	else
	{
		// reset bone offset memory pointer
		g_lplpdwAnimSkinNumBones[dwIndex]=NULL;
	}

	// allocate current keyframe matrices
	if(!AllocMem((LPVOID*)&g_lplpmAnimCurrentKey[dwIndex],
				 sizeof(D3DXMATRIX)*g_lpdwAnimNumMtx[dwIndex]))
	{
		if(bNoDFR)
			FreeMem(&dfr.lpData);
		return(NO_OBJECT);
	}

	// cast pointer to current keyframes
	LPD3DXMATRIX lpKeys=g_lplpmAnimCurrentKey[dwIndex];
	
	// set current keyframes to identity
	for(dwCount=0;dwCount<g_lpdwAnimNumMtx[dwIndex];dwCount++)
		lpKeys[dwCount]=g_mtxIdentity;

	// release data if necessary
	if(bNoDFR)
		FreeMem(&dfr.lpData);

	// return index
	return(dwIndex);
}

// PurgeAnimations()

void PurgeAnimations(void)
{
	// check counter
	if(g_dwNumAnim)
	{
		// counter
		DWORD dwCount;

		// release memory objects
		for(dwCount=0;dwCount<g_dwNumAnim;dwCount++)
		{
			FreeMem((LPVOID*)&g_lplpdwAnimSkinNumBones[dwCount]);
			FreeMem((LPVOID*)&g_lplpdwAnimKeyTimeSig[dwCount]);
			FreeMem((LPVOID*)&g_lplpmAnimKeyframes[dwCount]);
			FreeMem((LPVOID*)&g_lplpmAnimCurrentKey[dwCount]);
		}
	}

	// free memory
	FreeMem((LPVOID*)&g_lpdwAnimNumRef);
	FreeMem((LPVOID*)&g_lpdwAnimNumMesh);
	FreeMem((LPVOID*)&g_lpdwAnimNumSkin);
	FreeMem((LPVOID*)&g_lpdwAnimNumBones);
	FreeMem((LPVOID*)&g_lpdwAnimNumMtx);
	FreeMem((LPVOID*)&g_lpdwAnimNumRender);
	FreeMem((LPVOID*)&g_lpdwAnimNumKeys);
	FreeMem((LPVOID*)&g_lplpdwAnimSkinNumBones);
	FreeMem((LPVOID*)&g_lplpdwAnimKeyTimeSig);
	FreeMem((LPVOID*)&g_lplpmAnimKeyframes);
	FreeMem((LPVOID*)&g_lplpmAnimCurrentKey);
	FreeMem((LPVOID*)&g_lpdwAnimBaseMeshIdx);
	FreeMem((LPVOID*)&g_lpdwAnimBaseSkinIdx);
	
	// reset counters
	g_dwNumAnim=0;
	g_dwMaxAnim=0;
}

// GetNumKeyframes()

DWORD GetNumKeyframes(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAnim)
	{
		DisplayFatalErrorMessage("Invalid animation index.",
								 "GetNumKeyframes()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// return value	
	return(g_lpdwAnimNumKeys[dwIndex]);
}

// GetKeyframeTimeSignature()

DWORD GetKeyframeTimeSignature(DWORD dwAnimIndex,
							   DWORD dwKeyIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwAnimIndex>=g_dwNumAnim)
	{
		DisplayFatalErrorMessage("Invalid animation index.",
								 "GetKeyframeTimeSignature()",
								 FALSE);
		return(NO_OBJECT);
	}

	// verify keyframe index
	if(dwKeyIndex>=g_lpdwAnimNumKeys[dwAnimIndex])
	{
		DisplayFatalErrorMessage("Invalid keyframe index.",
								 "GetKeyframeTimeSignature()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// cast pointer to time signatures
	LPDWORD lpTime=g_lplpdwAnimKeyTimeSig[dwAnimIndex];

	// return value
	return(lpTime[dwKeyIndex]);
}

// UpdateReferenceFrames()

void UpdateReferenceFrames(DWORD dwAnimIndex,
						   DWORD dwKeyIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwAnimIndex>=g_dwNumAnim)
	{
		DisplayFatalErrorMessage("Invalid animation index.",
								 "UpdateReferenceFrames()",
								 FALSE);
		return;
	}

	// verify keyframe index
	if(dwKeyIndex>=g_lpdwAnimNumKeys[dwAnimIndex])
	{
		DisplayFatalErrorMessage("Invalid keyframe index.",
								 "UpdateReferenceFrames()",
								 FALSE);
		return;
	}
#endif

	// cast pointer to key matrices
	LPD3DXMATRIX lpKeys=g_lplpmAnimKeyframes[dwAnimIndex];
	
	// copy values to current key
	CopyMemory(g_lplpmAnimCurrentKey[dwAnimIndex],
			   &lpKeys[g_lpdwAnimNumMtx[dwAnimIndex]*dwKeyIndex],
			   sizeof(D3DXMATRIX)*g_lpdwAnimNumRef[dwAnimIndex]);
}

// UpdateReferenceFrames()

void UpdateReferenceFrames(DWORD dwAnimIndex,
						   DWORD dwKey1Index,
						   DWORD dwKey2Index,
						   float fTweenVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwAnimIndex>=g_dwNumAnim)
	{
		DisplayFatalErrorMessage("Invalid animation index.",
								 "UpdateReferenceFrames()",
								 FALSE);
		return;
	}

	// verify keyframe index
	if(dwKey1Index>=g_lpdwAnimNumKeys[dwAnimIndex])
	{
		DisplayFatalErrorMessage("Invalid keyframe index.",
								 "UpdateReferenceFrames()",
								 FALSE);
		return;
	}

	// verify keyframe index
	if(dwKey2Index>=g_lpdwAnimNumKeys[dwAnimIndex])
	{
		DisplayFatalErrorMessage("Invalid keyframe index.",
								 "UpdateReferenceFrames()",
								 FALSE);
		return;
	}
#endif

	// counter
	DWORD dwCount;

	// cast pointer to key matrices
	LPD3DXMATRIX lpKeys=g_lplpmAnimKeyframes[dwAnimIndex];

	// set matrix pointers
	float* lpF1=(float*)&lpKeys[g_lpdwAnimNumMtx[dwAnimIndex]*dwKey1Index];
	float* lpF2=(float*)&lpKeys[g_lpdwAnimNumMtx[dwAnimIndex]*dwKey2Index];
	float* lpFT=(float*)g_lplpmAnimCurrentKey[dwAnimIndex];

	// compute number of floats
	DWORD dwNumFloats=g_lpdwAnimNumRef[dwAnimIndex]<<4;
	
	// update current keys
	for(dwCount=0;dwCount<dwNumFloats;dwCount++)
	{
		// compute new value
		(*lpFT)=GetInterpolatedValue((*lpF1),(*lpF2),fTweenVal);

		// increment pointers
		lpF1++;
		lpF2++;
		lpFT++;
	}
}

// UpdateRenderFrames()

void UpdateRenderFrames(DWORD dwAnimIndex,
						DWORD dwKeyIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwAnimIndex>=g_dwNumAnim)
	{
		DisplayFatalErrorMessage("Invalid animation index.",
								 "UpdateRenderFrames()",
								 FALSE);
		return;
	}

	// verify keyframe index
	if(dwKeyIndex>=g_lpdwAnimNumKeys[dwAnimIndex])
	{
		DisplayFatalErrorMessage("Invalid keyframe index.",
								 "UpdateRenderFrames()",
								 FALSE);
		return;
	}
#endif

	// counter
	DWORD dwCount;
	
	// cast pointer to current keys
	LPD3DXMATRIX lpCurr=g_lplpmAnimCurrentKey[dwAnimIndex];
	
	// cast pointer to key matrices
	LPD3DXMATRIX lpKeys=g_lplpmAnimKeyframes[dwAnimIndex];
	
	// copy values to current key
	CopyMemory(&lpCurr[g_lpdwAnimNumRef[dwAnimIndex]],
			   &lpKeys[(g_lpdwAnimNumMtx[dwAnimIndex]*dwKeyIndex)+g_lpdwAnimNumRef[dwAnimIndex]],
			   sizeof(D3DXMATRIX)*g_lpdwAnimNumRender[dwAnimIndex]);

	// cast pointer to current matrices
	lpKeys=g_lplpmAnimCurrentKey[dwAnimIndex];
	
	// set pointer to first skin matrix
	DWORD dwMtx=g_lpdwAnimNumRef[dwAnimIndex]+g_lpdwAnimNumMesh[dwAnimIndex];

	// cast pointer to skinned mesh bone quantities
	LPDWORD dwNumBones=g_lplpdwAnimSkinNumBones[dwAnimIndex];

	// set pointer to first skinned mesh
	DWORD dwSkin=g_lpdwAnimBaseSkinIdx[dwAnimIndex];

	// update skinned meshes
	for(dwCount=0;dwCount<g_lpdwAnimNumSkin[dwAnimIndex];dwCount++)
	{
		// update mesh
		UpdateSkinMesh(dwSkin++,
					   &lpKeys[dwMtx]);

		// increment matrix counter
		dwMtx+=dwNumBones[dwCount];
	}
}

// UpdateRenderFrames()

void UpdateRenderFrames(DWORD dwAnimIndex,
						DWORD dwKey1Index,
						DWORD dwKey2Index,
						float fTweenVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwAnimIndex>=g_dwNumAnim)
	{
		DisplayFatalErrorMessage("Invalid animation index.",
								 "UpdateRenderFrames()",
								 FALSE);
		return;
	}

	// verify keyframe index
	if(dwKey1Index>=g_lpdwAnimNumKeys[dwAnimIndex])
	{
		DisplayFatalErrorMessage("Invalid keyframe index.",
								 "UpdateRenderFrames()",
								 FALSE);
		return;
	}

	// verify keyframe index
	if(dwKey2Index>=g_lpdwAnimNumKeys[dwAnimIndex])
	{
		DisplayFatalErrorMessage("Invalid keyframe index.",
								 "UpdateRenderFrames()",
								 FALSE);
		return;
	}
#endif

	// counter
	DWORD dwCount;

	// cast pointer to current keys
	LPD3DXMATRIX lpCurr=g_lplpmAnimCurrentKey[dwAnimIndex];
	
	// cast pointer to key matrices
	LPD3DXMATRIX lpKeys=g_lplpmAnimKeyframes[dwAnimIndex];

	// set matrix pointers
	float* lpF1=(float*)&lpKeys[(g_lpdwAnimNumMtx[dwAnimIndex]*dwKey1Index)+g_lpdwAnimNumRef[dwAnimIndex]];
	float* lpF2=(float*)&lpKeys[(g_lpdwAnimNumMtx[dwAnimIndex]*dwKey2Index)+g_lpdwAnimNumRef[dwAnimIndex]];
	float* lpFT=(float*)&lpCurr[g_lpdwAnimNumRef[dwAnimIndex]];

	// compute number of floats
	DWORD dwNumFloats=g_lpdwAnimNumRender[dwAnimIndex]<<4;

	// update current keys
	for(dwCount=0;dwCount<dwNumFloats;dwCount++)
	{
		// compute new value
		(*lpFT)=GetInterpolatedValue((*lpF1),(*lpF2),fTweenVal);

		// increment pointers
		lpF1++;
		lpF2++;
		lpFT++;
	}

	// cast pointer to current matrices
	lpKeys=g_lplpmAnimCurrentKey[dwAnimIndex];
	
	// set pointer to first skin matrix
	DWORD dwMtx=g_lpdwAnimNumRef[dwAnimIndex]+g_lpdwAnimNumMesh[dwAnimIndex];

	// cast pointer to skinned mesh bone quantities
	LPDWORD dwNumBones=g_lplpdwAnimSkinNumBones[dwAnimIndex];

	// set pointer to first skinned mesh
	DWORD dwSkin=g_lpdwAnimBaseSkinIdx[dwAnimIndex];

	// update skinned meshes
	for(dwCount=0;dwCount<g_lpdwAnimNumSkin[dwAnimIndex];dwCount++)
	{
		// update mesh
		UpdateSkinMesh(dwSkin++,
					   &lpKeys[dwMtx]);

		// increment matrix counter
		dwMtx+=dwNumBones[dwCount];
	}
}

// GetReferenceFrameMatrixPtr()

LPD3DXMATRIX GetReferenceFrameMatrixPtr(DWORD dwAnimIndex,
										DWORD dwRefIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwAnimIndex>=g_dwNumAnim)
	{
		DisplayFatalErrorMessage("Invalid animation index.",
								 "GetReferenceFrameMatrixPtr()",
								 FALSE);
		return(NULL);
	}

	// verify keyframe index
	if(dwRefIndex>=g_lpdwAnimNumRef[dwAnimIndex])
	{
		DisplayFatalErrorMessage("Invalid reference frame index.",
								 "GetReferenceFrameMatrixPtr()",
								 FALSE);
		return(NULL);
	}
#endif

	// cast pointer to current matrices
	LPD3DXMATRIX lpKeys=g_lplpmAnimCurrentKey[dwAnimIndex];
	
	// return pointer to specified key
	return(&lpKeys[dwRefIndex]);
}

// RenderAnimation()

void RenderAnimation(DWORD dwAnimIndex,
					 DWORD dwWMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwAnimIndex>=g_dwNumAnim)
	{
		DisplayFatalErrorMessage("Invalid animation index.",
								 "RenderAnimation()",
								 FALSE);
		return;
	}

	// verify matrix index
	if(dwWMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "RenderAnimation()",
								 FALSE);
		return;
	}
#endif

	// counter
	DWORD dwCount;
	
	// world matrix
	D3DXMATRIX mWorld;
	
	// get world matrix
	GetMatrix(dwWMtxIndex,
			  &mWorld);

	// cast pointer to current matrices
	LPD3DXMATRIX lpKeys=g_lplpmAnimCurrentKey[dwAnimIndex];
	
	// set pointer to first render matrix
	DWORD dwMtx=g_lpdwAnimNumRef[dwAnimIndex];

	// set pointer to first mesh
	DWORD dwMesh=g_lpdwAnimBaseMeshIdx[dwAnimIndex];

	// render meshes
	for(dwCount=0;dwCount<g_lpdwAnimNumMesh[dwAnimIndex];dwCount++)
	{
		// transformed matrix
		D3DXMATRIX mTrans;
		
		// transform key by world
		D3DXMatrixMultiply(&mTrans,
						   &lpKeys[dwMtx++],
						   &mWorld);

		// render mesh
		RenderMesh(dwMesh++,
				   &mTrans);
	}

	// set pointer to first skinned mesh
	DWORD dwSkin=g_lpdwAnimBaseSkinIdx[dwAnimIndex];

	// render skinned meshes
	for(dwCount=0;dwCount<g_lpdwAnimNumSkin[dwAnimIndex];dwCount++)
	{
		// render mesh
		RenderSkinMesh(dwSkin++,
					   &mWorld);
	}
}

// RenderAnimationAlpha()

void RenderAnimationAlpha(DWORD dwAnimIndex,
						  DWORD dwWMtxIndex,
						  float fAlpha)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwAnimIndex>=g_dwNumAnim)
	{
		DisplayFatalErrorMessage("Invalid animation index.",
								 "RenderAnimationAlpha()",
								 FALSE);
		return;
	}

	// verify matrix index
	if(dwWMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "RenderAnimationAlpha()",
								 FALSE);
		return;
	}
#endif

	// counter
	DWORD dwCount;
	
	// world matrix
	D3DXMATRIX mWorld;
	
	// get world matrix
	GetMatrix(dwWMtxIndex,
			  &mWorld);

	// cast pointer to current matrices
	LPD3DXMATRIX lpKeys=g_lplpmAnimCurrentKey[dwAnimIndex];
	
	// set pointer to first render matrix
	DWORD dwMtx=g_lpdwAnimNumRef[dwAnimIndex];

	// set pointer to first mesh
	DWORD dwMesh=g_lpdwAnimBaseMeshIdx[dwAnimIndex];

	// render meshes
	for(dwCount=0;dwCount<g_lpdwAnimNumMesh[dwAnimIndex];dwCount++)
	{
		// transformed matrix
		D3DXMATRIX mTrans;
		
		// transform key by world
		D3DXMatrixMultiply(&mTrans,
						   &lpKeys[dwMtx++],
						   &mWorld);

		// render mesh
		RenderMeshAlpha(dwMesh++,
						&mTrans,
						fAlpha);
	}

	// set pointer to first skinned mesh
	DWORD dwSkin=g_lpdwAnimBaseSkinIdx[dwAnimIndex];

	// render skinned meshes
	for(dwCount=0;dwCount<g_lpdwAnimNumSkin[dwAnimIndex];dwCount++)
	{
		// render mesh
		RenderSkinMeshAlpha(dwSkin++,
							&mWorld,
							fAlpha);
	}
}

// AddMeshesFromXFA()

BOOL AddMeshesFromXFA(DWORD dwAnimIndex,
					  LPDATAFILEREF lpDFR,
					  BOOL bNormals,
					  BOOL bOptimize)
{
	LoadingOutput("AddMeshesFromXFA");

#ifdef DEBUG_MODE
	// verify index
	if(dwAnimIndex>=g_dwNumAnim)
	{
		DisplayFatalErrorMessage("Invalid animation index.",
								 "AddMeshesFromXFA()",
								 FALSE);
		return(FALSE);
	}
#endif

	// used for error checking
	HRESULT hr;

	// cast BYTE pointer to data
	LPBYTE cData=(LPBYTE)lpDFR->lpData;

	// compute x-file offset
	DWORD dwXFileOfs=(sizeof(DWORD)*5)+
					 (sizeof(DWORD)*g_lpdwAnimNumKeys[dwAnimIndex])+
					 (sizeof(D3DXMATRIX)*g_lpdwAnimNumKeys[dwAnimIndex]*g_lpdwAnimNumMtx[dwAnimIndex]);

	// prepare x-file load structure
	DXFILELOADMEMORY flm;
	flm.lpMemory=(LPVOID)&cData[dwXFileOfs];
	flm.dSize=lpDFR->dwSize-dwXFileOfs;
	
	// x-file enumerator object
	IDirectXFileEnumObject* lpDXFileEnum;

	// create enumerator
	hr=g_lpDXFile->CreateEnumObject((LPVOID)&flm,
									DXFILELOAD_FROMMEMORY,
									&lpDXFileEnum);

	// check for error 
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create DirectX file enumerator object.",
							  NULL,
							  hr);
		return(FALSE);
	}
	else
		AddCOMResource;

	// data objects
	IDirectXFileData* lpDXFileData;

	// load top-level data objects
	while(SUCCEEDED(lpDXFileEnum->GetNextDataObject(&lpDXFileData)))
	{
		// add resource
		AddCOMResource;

		// load mesh objects
		BOOL bFlag=AddMeshFromXof(lpDXFileData,
								  bNormals,
								  bOptimize);

		// release data object
		ReleaseCOMPtr(lpDXFileData);

		// check flag
		if(!bFlag)
		{
			ReleaseCOMPtr(lpDXFileEnum);
			return(FALSE);
		}
	}

	// release enumerator object
	ReleaseCOMPtr(lpDXFileEnum);

	// ok
	return(TRUE);
}
	
// AddMeshFromXof()

BOOL AddMeshFromXof(IDirectXFileData* lpData,
					BOOL bNormals,
					BOOL bOptimize)
{
	LoadingOutput("AddMeshFromXof");

	// used for error checking
	HRESULT hr;

	// template type
	const GUID* lpType;

	// buffer size
	DWORD dwSize;

	// pointer to name
	LPSTR lpName;

	// mesh creation flags
	DWORD dwFlags;

	// skin mesh buffers
	ID3DXBuffer* lpMaterials;
	ID3DXBuffer* lpAdjacency;
	ID3DXBuffer* lpBoneNames;
	ID3DXBuffer* lpBoneTransforms;

	// skin mesh
	ID3DXSkinMesh* lpSkinMesh;

	// number of materials
	DWORD dwNumMat;

	// counter
	DWORD dwCount;

	// get template type
	hr=lpData->GetType(&lpType);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to read DirectX file template type.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// get buffer size for object name
	hr=lpData->GetName(NULL,
					   &dwSize);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to read DirectX file object name length.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// set name as needed
	if(dwSize)
	{
		// allocate buffer
		if(!AllocMem((LPVOID*)&lpName,
				     dwSize))
			return(FALSE);

		// get name
		hr=lpData->GetName(lpName,
						   &dwSize);

		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to read DirectX file object name.",
								  NULL,
								  hr);
			FreeMem((LPVOID*)&lpName);
			return(FALSE);
		}
	}
	else
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&lpName,
				     strlen("Unknown")+1))
			return(FALSE);
		
		// set default name
		strcpy(lpName,"Unknown");
	}

	// process template
	if((*lpType)==TID_D3DRMMesh)
	{
		// set creation flags
		dwFlags=D3DXMESH_MANAGED;
		if(g_dwVPF==VPF_HARDWARE)
			dwFlags=dwFlags|D3DXMESH_USEHWONLY;
		if(g_dwVPF==VPF_SOFTWARE)
			dwFlags=dwFlags|D3DXMESH_SOFTWAREPROCESSING;

		// load skin mesh
		hr=D3DXLoadSkinMeshFromXof(lpData,	
								   dwFlags,
								   g_lpD3DDevice,
								   &lpAdjacency,
								   &lpMaterials,
								   &dwNumMat,
								   &lpBoneNames,
								   &lpBoneTransforms,
								   &lpSkinMesh);

		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to load skinned mesh.",
								  NULL,
								  hr);
			FreeMem((LPVOID*)&lpName);
			ReleaseCOMPtr(lpMaterials);
			ReleaseCOMPtr(lpAdjacency);
			ReleaseCOMPtr(lpBoneNames);
			ReleaseCOMPtr(lpBoneTransforms);
			ReleaseCOMPtr(lpSkinMesh);
			return(FALSE);
		}
		else
			AddCOMResource;

		// add additional resources as needed
		if(lpAdjacency)      AddCOMResource;
		if(lpMaterials)      AddCOMResource;
		if(lpBoneNames)      AddCOMResource;
		if(lpBoneTransforms) AddCOMResource;

		// check mesh type
		if(lpSkinMesh->GetNumBones())
		{
			// get skinned mesh
			DWORD dwIndex=GetNextSkinMesh();

			// check index
			if(dwIndex==NO_OBJECT)
			{
				FreeMem((LPVOID*)&lpName);
				ReleaseCOMPtr(lpMaterials);
				ReleaseCOMPtr(lpAdjacency);
				ReleaseCOMPtr(lpBoneNames);
				ReleaseCOMPtr(lpBoneTransforms);
				ReleaseCOMPtr(lpSkinMesh);
				return(FALSE);
			}
			
			// get render mesh
			DWORD dwRenderMeshIndex=GetNextMesh();

			// check index
			if(dwRenderMeshIndex==NO_OBJECT)
			{
				FreeMem((LPVOID*)&lpName);
				ReleaseCOMPtr(lpMaterials);
				ReleaseCOMPtr(lpAdjacency);
				ReleaseCOMPtr(lpBoneNames);
				ReleaseCOMPtr(lpBoneTransforms);
				ReleaseCOMPtr(lpSkinMesh);
				return(FALSE);
			}
			
			// allocate memory for name
			if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwRenderMeshIndex],
						 strlen(lpName)+1))
			{
				FreeMem((LPVOID*)&lpName);
				ReleaseCOMPtr(lpMaterials);
				ReleaseCOMPtr(lpAdjacency);
				ReleaseCOMPtr(lpBoneNames);
				ReleaseCOMPtr(lpBoneTransforms);
				ReleaseCOMPtr(lpSkinMesh);
				return(FALSE);
			}

			// copy name
			CopyMemory((LPVOID)g_lplpMeshNames[dwRenderMeshIndex],
					   (LPVOID)lpName,
					   strlen(lpName)+1);

			// set new creation flags
			dwFlags=D3DXMESH_SYSTEMMEM;
			if(g_dwVPF==VPF_HARDWARE)
				dwFlags=dwFlags|D3DXMESH_USEHWONLY;
			if(g_dwVPF==VPF_SOFTWARE)
				dwFlags=dwFlags|D3DXMESH_SOFTWAREPROCESSING;

			// generate render mesh
			hr=lpSkinMesh->GenerateSkinnedMesh(dwFlags,
											   0,
											   (LPDWORD)lpAdjacency->GetBufferPointer(),
											   NULL,
											   NULL,
											   NULL,
											   &g_lplpMesh[dwRenderMeshIndex]);
			
			// check for error
			if(FAILED(hr))
			{
				DisplayDXErrorMessage("Unable to generate skinned mesh.",
									  NULL,
									  hr);
				FreeMem((LPVOID*)&lpName);
				ReleaseCOMPtr(lpMaterials);
				ReleaseCOMPtr(lpAdjacency);
				ReleaseCOMPtr(lpBoneNames);
				ReleaseCOMPtr(lpBoneTransforms);
				ReleaseCOMPtr(lpSkinMesh);
				return(FALSE);
			}
			else
				AddCOMResource;

			// check number of materials
			if(dwNumMat)
			{
				// set number of subsets
				g_lpdwNumMeshSubsets[dwRenderMeshIndex]=dwNumMat;
				
				// allocate subset array
				if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwRenderMeshIndex],
							 sizeof(MESHSUBSETINFO)*dwNumMat))
				{
					FreeMem((LPVOID*)&lpName);
					ReleaseCOMPtr(lpMaterials);
					ReleaseCOMPtr(lpAdjacency);
					ReleaseCOMPtr(lpBoneNames);
					ReleaseCOMPtr(lpBoneTransforms);
					ReleaseCOMPtr(lpSkinMesh);
					return(FALSE);
				}

				// get subset info pointer
				LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwRenderMeshIndex];
				
				// get materials buffer pointer
				LPD3DXMATERIAL lpMat=(LPD3DXMATERIAL)lpMaterials->GetBufferPointer();
				
				// fill in subset array
				for(dwCount=0;dwCount<dwNumMat;dwCount++)
				{
					// copy material ambient value
					lpMat[dwCount].MatD3D.Ambient=lpMat[dwCount].MatD3D.Diffuse;

					// copy material
					lpMeshSubsetInfo[dwCount].dwMat=AddMaterial(&lpMat[dwCount].MatD3D);

					// check material
					if(lpMeshSubsetInfo[dwCount].dwMat==NO_OBJECT)
					{
						FreeMem((LPVOID*)&lpName);
						ReleaseCOMPtr(lpMaterials);
						ReleaseCOMPtr(lpAdjacency);
						ReleaseCOMPtr(lpBoneNames);
						ReleaseCOMPtr(lpBoneTransforms);
						ReleaseCOMPtr(lpSkinMesh);
						return(FALSE);
					}

					// load texture if needed
					if(lpMat[dwCount].pTextureFilename)
					{
						// add texture
						lpMeshSubsetInfo[dwCount].dwTex=AddTexture(lpMat[dwCount].pTextureFilename);

						// check texture
						if(lpMeshSubsetInfo[dwCount].dwTex==NO_OBJECT)
						{
							FreeMem((LPVOID*)&lpName);
							ReleaseCOMPtr(lpMaterials);
							ReleaseCOMPtr(lpAdjacency);
							ReleaseCOMPtr(lpBoneNames);
							ReleaseCOMPtr(lpBoneTransforms);
							ReleaseCOMPtr(lpSkinMesh);
							return(FALSE);
						}
					}
					else
						lpMeshSubsetInfo[dwCount].dwTex=NO_OBJECT;
				}
			}
			else
			{
				// set number of subsets
				g_lpdwNumMeshSubsets[dwRenderMeshIndex]=1;
				
				// allocate subset array
				if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwRenderMeshIndex],
							 sizeof(MESHSUBSETINFO)))
				{
					FreeMem((LPVOID*)&lpName);
					ReleaseCOMPtr(lpMaterials);
					ReleaseCOMPtr(lpAdjacency);
					ReleaseCOMPtr(lpBoneNames);
					ReleaseCOMPtr(lpBoneTransforms);
					ReleaseCOMPtr(lpSkinMesh);
					return(FALSE);
				}

				// get subset info pointer
				LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwRenderMeshIndex];
				
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
					FreeMem((LPVOID*)&lpName);
					ReleaseCOMPtr(lpMaterials);
					ReleaseCOMPtr(lpAdjacency);
					ReleaseCOMPtr(lpBoneNames);
					ReleaseCOMPtr(lpBoneTransforms);
					ReleaseCOMPtr(lpSkinMesh);
					return(FALSE);
				}
			}

			// set number of bones
			DWORD dwNumBones=lpSkinMesh->GetNumBones();

			// allocate bone structure
			if(!AllocMem((LPVOID*)&g_lplpBoneStructure[dwIndex],
						 sizeof(D3DXMATRIX)*dwNumBones))
			{
				FreeMem((LPVOID*)&lpName);
				ReleaseCOMPtr(lpMaterials);
				ReleaseCOMPtr(lpAdjacency);
				ReleaseCOMPtr(lpBoneNames);
				ReleaseCOMPtr(lpBoneTransforms);
				ReleaseCOMPtr(lpSkinMesh);
				return(FALSE);
			}

			// cast pointer to bone structure
			LPD3DXMATRIX lpBoneStructure=g_lplpBoneStructure[dwIndex];
			
			// reset bone structure
			for(dwCount=0;dwCount<dwNumBones;dwCount++)
				lpBoneStructure[dwCount]=g_mtxIdentity;

			// save skin mesh interface
			g_lplpSkin[dwIndex]=lpSkinMesh;
			
			// save render mesh index
			g_lpdwRenderMeshIndex[dwIndex]=dwRenderMeshIndex;

			// reset bone buffers (not needed)
			g_lplpBoneNames[dwIndex]=NULL;
			g_lplpBoneTransforms[dwIndex]=NULL;

			// release buffers
			ReleaseCOMPtr(lpMaterials);
			ReleaseCOMPtr(lpAdjacency);
			ReleaseCOMPtr(lpBoneNames);
			ReleaseCOMPtr(lpBoneTransforms);
		}
		else
		{
			// get next mesh
			DWORD dwIndex=GetNextMesh();

			// check index
			if(dwIndex==NO_OBJECT)
			{
				FreeMem((LPVOID*)&lpName);
				ReleaseCOMPtr(lpMaterials);
				ReleaseCOMPtr(lpAdjacency);
				ReleaseCOMPtr(lpBoneNames);
				ReleaseCOMPtr(lpBoneTransforms);
				ReleaseCOMPtr(lpSkinMesh);
				return(FALSE);
			}

			// allocate memory for name
			if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
						 strlen(lpName)+1))
			{
				FreeMem((LPVOID*)&lpName);
				ReleaseCOMPtr(lpMaterials);
				ReleaseCOMPtr(lpAdjacency);
				ReleaseCOMPtr(lpBoneNames);
				ReleaseCOMPtr(lpBoneTransforms);
				ReleaseCOMPtr(lpSkinMesh);
				return(FALSE);
			}

			// copy name
			CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
					   (LPVOID)lpName,
					   strlen(lpName)+1);

			// create mesh
			hr=lpSkinMesh->GetOriginalMesh(&g_lplpMesh[dwIndex]);
			
			// check for error
			if(FAILED(hr))
			{
				DisplayDXErrorMessage("Unable to convert skinned mesh to static mesh.",
									  NULL,
									  hr);
				FreeMem((LPVOID*)&lpName);
				ReleaseCOMPtr(lpMaterials);
				ReleaseCOMPtr(lpAdjacency);
				ReleaseCOMPtr(lpBoneNames);
				ReleaseCOMPtr(lpBoneTransforms);
				ReleaseCOMPtr(lpSkinMesh);
				return(FALSE);
			}
			else
				AddCOMResource;

			// check number of materials
			if(dwNumMat)
			{
				// set number of subsets
				g_lpdwNumMeshSubsets[dwIndex]=dwNumMat;
				
				// allocate subset array
				if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
							 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
				{
					FreeMem((LPVOID*)&lpName);
					ReleaseCOMPtr(lpMaterials);
					ReleaseCOMPtr(lpAdjacency);
					ReleaseCOMPtr(lpBoneNames);
					ReleaseCOMPtr(lpBoneTransforms);
					ReleaseCOMPtr(lpSkinMesh);
					return(FALSE);
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
						FreeMem((LPVOID*)&lpName);
						ReleaseCOMPtr(lpMaterials);
						ReleaseCOMPtr(lpAdjacency);
						ReleaseCOMPtr(lpBoneNames);
						ReleaseCOMPtr(lpBoneTransforms);
						ReleaseCOMPtr(lpSkinMesh);
						return(FALSE);
					}

					// load texture if needed
					if(lpMat[dwCount].pTextureFilename)
					{
						// add texture
						lpMeshSubsetInfo[dwCount].dwTex=AddTexture(lpMat[dwCount].pTextureFilename);

						// check texture
						if(lpMeshSubsetInfo[dwCount].dwTex==NO_OBJECT)
						{
							FreeMem((LPVOID*)&lpName);
							ReleaseCOMPtr(lpMaterials);
							ReleaseCOMPtr(lpAdjacency);
							ReleaseCOMPtr(lpBoneNames);
							ReleaseCOMPtr(lpBoneTransforms);
							ReleaseCOMPtr(lpSkinMesh);
							return(FALSE);
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
					FreeMem((LPVOID*)&lpName);
					ReleaseCOMPtr(lpMaterials);
					ReleaseCOMPtr(lpAdjacency);
					ReleaseCOMPtr(lpBoneNames);
					ReleaseCOMPtr(lpBoneTransforms);
					ReleaseCOMPtr(lpSkinMesh);
					return(FALSE);
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
					FreeMem((LPVOID*)&lpName);
					ReleaseCOMPtr(lpMaterials);
					ReleaseCOMPtr(lpAdjacency);
					ReleaseCOMPtr(lpBoneNames);
					ReleaseCOMPtr(lpBoneTransforms);
					ReleaseCOMPtr(lpSkinMesh);
					return(FALSE);
				}
			}

			// add normals to mesh
			if(bNormals)
				AddMeshNormals(dwIndex);

			// optimize if requested
			if(bOptimize)
				OptimizeMesh(dwIndex);

			// release skin mesh
			ReleaseCOMPtr(lpSkinMesh);

			// release buffers
			ReleaseCOMPtr(lpMaterials);
			ReleaseCOMPtr(lpAdjacency);
		}
	}

	// free name buffer
	FreeMem((LPVOID*)&lpName);

	// ok
	return(TRUE);
}

// GetNaturalizedTweenValue()

float GetNaturalizedTweenValue(float fVal)
{
	// expand value to range -1.0 to 1.0
	fVal=(fVal*2)-1;

	// compute new value
	if(fVal<0)
		fVal=Squared(fVal+1)-1;
	else
		fVal=1-Squared(1-fVal);
	
	// restore value to range 0.0 to 1.0
	fVal=(fVal+1)*0.5F;
	
	// return new value
	return(fVal);
}

// eof

// Copyright 2005-2007 WaveBreak Software
