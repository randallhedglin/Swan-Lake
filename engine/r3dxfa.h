// Copyright 2005-2007 WaveBreak Software

// r3dxfa.h -- x-file animation header

// NOTES: Always check x files for XSkinMeshHeader
//        and duplicate keyframe time signatures!

// structures

#ifdef DEBUG_MODE
typedef struct tagFRAMENODE
{
	// current frame transform (relative)
	D3DXMATRIX mTrans;
	// current frame transform (absolute)
	D3DXMATRIX mWorld;
	// original frame transform (relative)
	D3DXMATRIX mOrigTrans;
	// original frame transform (absolute)
	D3DXMATRIX mOrigWorld;
	// mesh node
	CNode* lpMesh;
	// animation node
	CNode* lpAnim;
} FRAMENODE,*LPFRAMENODE;

typedef struct tagMESHNODE
{
	// mesh index
	DWORD dwMeshIndex;
	// skin mesh index
	DWORD dwSkinIndex;
} MESHNODE,*LPMESHNODE;

typedef struct tagANIMNODE
{
	// key type
	DWORD dwKeyType;
	// number of keys
	DWORD dwNumKeys;
	// pointer to keys
	LPVOID lpKeys;
} ANIMNODE,*LPANIMNODE;

typedef struct tagDXOFQUATERION
{
	// values
	float w,x,y,z;
} DXOFQUATERNION,*LPDXOFQUATERNION;

typedef struct tagXROTATIONKEY
{
	// time marker
	DWORD dwTime;
	// number of values (dummy variable)
	DWORD dwNumVals;
	// x-file rotation key
	DXOFQUATERNION q;
} XROTATIONKEY,*LPXROTATIONKEY;

typedef struct tagROTATIONKEY
{
	// time marker
	DWORD dwTime;
	// number of values (dummy variable)
	DWORD dwNumVals;
	// rotation key
	D3DXQUATERNION q;
} ROTATIONKEY,*LPROTATIONKEY;

typedef struct tagSCALEKEY
{
	// time marker
	DWORD dwTime;
	// number of values (dummy variable)
	DWORD dwNumVals;
	// scale key
	D3DXVECTOR3 v;
} SCALEKEY,*LPSCALEKEY;

typedef struct tagPOSITIONKEY
{
	// time marker
	DWORD dwTime;
	// number of values (dummy variable)
	DWORD dwNumVals;
	// position key
	D3DXVECTOR3 v;
} POSITIONKEY,*LPPOSITIONKEY;

typedef struct tagMATRIXKEY
{
	// time marker
	DWORD dwTime;
	// number of values (dummy variable)
	DWORD dwNumVals;
	// matrix key
	D3DXMATRIX m;
} MATRIXKEY,*LPMATRIXKEY;

typedef struct tagKEYFRAMES
{
	// reference frame names
	LPSTR lpRefFrames;
	// keyframe reorder flag
	DWORD dwReorderFlag;
	// number of frames to reorder
	DWORD dwNumFrames;
	// reorder data
	LPDWORD lpdwFrames;
} KEYFRAMES,*LPKEYFRAMES;
#endif

// classes

#ifdef DEBUG_MODE
class CAnimation
{
public:
	CAnimation();
	void LoadFromX(LPCSTR lpFile,
				   BOOL bNormals=TRUE,
				   BOOL bOptimize=TRUE);
	void Render(DWORD dwMtxIndex);
	void SetTime(DWORD dwTime);
	DWORD EnumerateMeshes(DWORD dwNumRefFrames,
						  LPSTR lpRefFrames,
						  LPSTR* lplpMeshNames);
	DWORD EnumerateSkinnedMeshes(DWORD dwNumRefFrames,
								 LPSTR lpRefFrames,
								 LPSTR* lplpSkinNames);
	DWORD EnumerateSkinnedMeshBones(DWORD dwNumRefFrames,
									LPSTR lpRefFrames);
	CNode* FindFrame(LPSTR lpName);
	DWORD EnumerateKeyframes(CNode* lpKeyEnum);
	DWORD FindMinKeyframe(CNode* lpKeyEnum);
	DWORD FindMaxKeyframe(CNode* lpKeyEnum);
	void SortKeyframes(CNode* lpKeyEnum);
	void CaptureKeyframes(DWORD dwNumRefFrames,
						  LPSTR lpRefFrames,
						  DWORD dwNumMeshes,
						  LPD3DXMATRIX lpKeys);
	void Release(void);
private:
	void LoadDataObject(IDirectXFileData* lpData,
						CNode* lpFrame,
						BOOL bNormals,
						BOOL bOptimize);
	BOOL LoadMesh(IDirectXFileData* lpData,
				  CNode* lpFrame,
				  LPSTR lpName,
				  BOOL bNormals,
				  BOOL bOptimize);
	void SetTimeChild(CNode* lpNode,
				      DWORD dwTime,
					  D3DXMATRIX mWorld);
	void SetTimeSibling(CNode* lpNode,
				        DWORD dwTime,
						D3DXMATRIX mWorld);
	void SetTimeNode(CNode* lpNode,
				     DWORD dwTime,
					 D3DXMATRIX mWorld);
	void SetKeyMatrix(LPFRAMENODE lpFN,
					  DWORD dwTime);
	void TransformKey(CNode* lpNode,
					  DWORD dwTime,
					  DWORD dwKeyType,
					  LPD3DXMATRIX lpKey);
	void FreeMeshChild(CNode* lpNode);
	void FreeMeshSibling(CNode* lpNode);
	void FreeMeshNode(CNode* lpNode);
	void FreeAnimChild(CNode* lpNode);
	void FreeAnimSibling(CNode* lpNode);
	void FreeAnimNode(CNode* lpNode);
	void FreeAnimChildMem(CNode* lpNode);
	void FreeAnimSiblingMem(CNode* lpNode);
	void FreeAnimNodeMem(CNode* lpNode);
	void UpdateBoneStructure(DWORD dwSkinIndex);
	void RenderChild(CNode* lpNode,
					 D3DXMATRIX mFrame,
					 D3DXMATRIX mWorld);
	void RenderSibling(CNode* lpNode,
					   D3DXMATRIX mFrame,
					   D3DXMATRIX mWorld);
	void RenderNode(CNode* lpNode,
					D3DXMATRIX mFrame,
					D3DXMATRIX mWorld);
	void RenderMeshNode(CNode* lpNode,
					    D3DXMATRIX mFrame,
					    D3DXMATRIX mWorld);
	void EnumerateMeshChild(CNode* lpNode,
							DWORD dwNumRefFrames,
						    LPSTR lpRefFrames,
							LPDWORD lpdwCount,
							LPSTR* lplpMeshNames,
							LPDWORD lpdwMeshNamesSize,
							LPDWORD lpdwMeshNamesPtr);
	void EnumerateMeshSibling(CNode* lpNode,
							  DWORD dwNumRefFrames,
						      LPSTR lpRefFrames,
							  LPDWORD lpdwCount,
							  LPSTR* lplpMeshNames,
							  LPDWORD lpdwMeshNamesSize,
							  LPDWORD lpdwMeshNamesPtr);
	void EnumerateMeshNode(CNode* lpNode,
						   DWORD dwNumRefFrames,
						   LPSTR lpRefFrames,
						   LPDWORD lpdwCount,
						   LPSTR* lplpMeshNames,
						   LPDWORD lpdwMeshNamesSize,
						   LPDWORD lpdwMeshNamesPtr);
	void EnumerateMeshNodes(CNode* lpNode,
						    LPDWORD lpdwCount,
							LPSTR* lplpMeshNames,
							LPDWORD lpdwMeshNamesSize,
							LPDWORD lpdwMeshNamesPtr);
	void EnumerateSkinnedMeshChild(CNode* lpNode,
								   DWORD dwNumRefFrames,
								   LPSTR lpRefFrames,
								   LPDWORD lpdwCount,
								   LPSTR* lplpSkinNames,
								   LPDWORD lpdwSkinNamesSize,
								   LPDWORD lpdwSkinNamesPtr);
	void EnumerateSkinnedMeshSibling(CNode* lpNode,
									 DWORD dwNumRefFrames,
									 LPSTR lpRefFrames,
									 LPDWORD lpdwCount,
									 LPSTR* lplpSkinNames,
								     LPDWORD lpdwSkinNamesSize,
								     LPDWORD lpdwSkinNamesPtr);
	void EnumerateSkinnedMeshNode(CNode* lpNode,
								  DWORD dwNumRefFrames,
								  LPSTR lpRefFrames,
								  LPDWORD lpdwCount,
								  LPSTR* lplpSkinNames,
								  LPDWORD lpdwSkinNamesSize,
								  LPDWORD lpdwSkinNamesPtr);
	void EnumerateSkinnedMeshNodes(CNode* lpNode,
								   LPDWORD lpdwCount,
								   LPSTR* lplpSkinNames,
								   LPDWORD lpdwSkinNamesSize,
								   LPDWORD lpdwSkinNamesPtr);
	void EnumerateSkinnedMeshBoneChild(CNode* lpNode,
									   DWORD dwNumRefFrames,
									   LPSTR lpRefFrames,
									   LPDWORD lpdwCount);
	void EnumerateSkinnedMeshBoneSibling(CNode* lpNode,
										 DWORD dwNumRefFrames,
										 LPSTR lpRefFrames,
										 LPDWORD lpdwCount);
	void EnumerateSkinnedMeshBoneNode(CNode* lpNode,
									  DWORD dwNumRefFrames,
									  LPSTR lpRefFrames,
									  LPDWORD lpdwCount);
	void EnumerateSkinnedMeshBoneNodes(CNode* lpNode,
									   LPDWORD lpdwCount);
	CNode* FindKeyframe(CNode* lpKeyEnum,
						DWORD dwTimeSig);
	CNode* FindKeyframeNode(CNode* lpKeyEnum,
							DWORD dwTimeSig);
	void EnumerateKeyframeChild(CNode* lpNode,
								CNode* lpKeyEnum,
								LPDWORD lpdwCount);
	void EnumerateKeyframeSibling(CNode* lpNode,
								  CNode* lpKeyEnum,
								  LPDWORD lpdwCount);
	void EnumerateKeyframeNode(CNode* lpNode,
							   CNode* lpKeyEnum,
							   LPDWORD lpdwCount);
	void EnumerateKeyframeAnimNode(CNode* lpNode,
								   CNode* lpKeyEnum,
								   LPDWORD lpdwCount);
	void CaptureKeyframeChild(CNode* lpNode,
							  DWORD dwNumRefFrames,
							  LPSTR lpRefFrames,
							  LPD3DXMATRIX lpRefKeys,
							  LPD3DXMATRIX lpMeshKeys,
							  LPDWORD lpdwMeshIdx,
							  LPD3DXMATRIX lpBoneKeys,
							  LPDWORD lpdwBoneIdx);
	void CaptureKeyframeSibling(CNode* lpNode,
							    DWORD dwNumRefFrames,
							    LPSTR lpRefFrames,
							    LPD3DXMATRIX lpRefKeys,
							    LPD3DXMATRIX lpMeshKeys,
								LPDWORD lpdwMeshIdx,
								LPD3DXMATRIX lpBoneKeys,
								LPDWORD lpdwBoneIdx);
	void CaptureKeyframeNode(CNode* lpNode,
							 DWORD dwNumRefFrames,
							 LPSTR lpRefFrames,
							 LPD3DXMATRIX lpRefKeys,
							 LPD3DXMATRIX lpMeshKeys,
							 LPDWORD lpdwMeshIdx,
							 LPD3DXMATRIX lpBoneKeys,
							 LPDWORD lpdwBoneIdx);
	void CaptureKeyframeMeshNode(LPFRAMENODE lpFN,
								 CNode* lpMeshNode,
								 LPD3DXMATRIX lpMeshKeys,
								 LPDWORD lpdwMeshIdx,
								 LPD3DXMATRIX lpBoneKeys,
								 LPDWORD lpdwBoneIdx);

	// root frame
	CNode* lpFrameRoot;
	// animation loading flag
	BOOL bAnimLoading;
};
#endif

// constants

#ifdef DEBUG_MODE
// x-file animation keys
#define ANIMKEY_ROTATION 0
#define ANIMKEY_SCALE    1
#define ANIMKEY_POSITION 2
#define ANIMKEY_MATRIX   4

// x-file animation key flags
#define KEYFRAMES_KEEPEXISTING  1
#define KEYFRAMES_ADDTOEXISTING 2
#define KEYFRAMES_EVENLYTIMED   3
#define KEYFRAMES_SPECIFYNEW    4
#endif

// globals

// directx file interface
extern IDirectXFile* g_lpDXFile;
// skin mesh counters
extern DWORD g_dwNumSkin;
extern DWORD g_dwMaxSkin;
// skin mesh data
extern ID3DXSkinMesh** g_lplpSkin;
extern LPDWORD g_lpdwRenderMeshIndex;
extern ID3DXBuffer** g_lplpBoneNames;
extern ID3DXBuffer** g_lplpBoneTransforms;
extern LPD3DXMATRIX* g_lplpBoneStructure;
// animation counters
extern DWORD g_dwNumAnim;
extern DWORD g_dwMaxAnim;
// animation data
extern LPDWORD g_lpdwAnimNumRef;
extern LPDWORD g_lpdwAnimNumMesh;
extern LPDWORD g_lpdwAnimNumSkin;
extern LPDWORD g_lpdwAnimNumBones;
extern LPDWORD g_lpdwAnimNumMtx;
extern LPDWORD g_lpdwAnimNumRender;
extern LPDWORD g_lpdwAnimNumKeys;
extern LPDWORD* g_lplpdwAnimSkinNumBones;
extern LPDWORD* g_lplpdwAnimKeyTimeSig;
extern LPD3DXMATRIX* g_lplpmAnimKeyframes;
extern LPD3DXMATRIX* g_lplpmAnimCurrentKey;
extern LPDWORD g_lpdwAnimBaseMeshIdx;
extern LPDWORD g_lpdwAnimBaseSkinIdx;

// prototypes

// DXFile functions

extern BOOL InitDXFile(void);

extern void ReleaseDXFile(void);

// skinned mesh functions

extern DWORD GetNextSkinMesh(void);

extern void PurgeSkinMeshes(void);

extern void UpdateSkinMesh(DWORD dwSkinIndex,
						   LPD3DXMATRIX lpMtx=NULL);

extern void RenderSkinMesh(DWORD dwSkinIndex,
						   LPD3DXMATRIX lpMtx);

extern void RenderSkinMeshAlpha(DWORD dwSkinIndex,
								LPD3DXMATRIX lpMtx,
								float fAlpha);

// xfa-handling functions

#ifdef DEBUG_MODE
extern BOOL CreateXFAFromX(LPCSTR lpDestFile,
						   LPCSTR lpSrcFile,
						   LPKEYFRAMES lpKF=NULL);

extern BOOL CompressX(LPCSTR lpFile);

extern BOOL AppendXToXAF(LPCSTR lpSrcFile,
						 DWORD dwNumMesh,
						 LPSTR lpMeshNames,
						 DWORD dwNumSkin,
						 LPSTR lpSkinNames);

extern BOOL AppendXRefData(LPCSTR lpSrcFile);

extern BOOL AppendXMeshData(LPCSTR lpSrcFile,
							LPSTR lpName);

extern char CopyXToFirstTemplate(void);

extern char CopyXToNextTemplate(char c);

extern char GetNextXMeshName(LPSTR lpName);
#endif

// animation functions

extern DWORD GetNextAnimation(void);

extern DWORD AddAnimation(LPCSTR lpFile,
						  BOOL bNormals=TRUE,
						  BOOL bOptimize=TRUE);

extern void PurgeAnimations(void);

extern DWORD GetNumKeyframes(DWORD dwIndex);

extern DWORD GetKeyframeTimeSignature(DWORD dwAnimIndex,
									  DWORD dwKeyIndex);

extern void UpdateReferenceFrames(DWORD dwAnimIndex,
								  DWORD dwKeyIndex);

extern void UpdateReferenceFrames(DWORD dwAnimIndex,
								  DWORD dwKey1Index,
								  DWORD dwKey2Index,
								  float fTweenVal);

extern void UpdateRenderFrames(DWORD dwAnimIndex,
							   DWORD dwKeyIndex);

extern void UpdateRenderFrames(DWORD dwAnimIndex,
							   DWORD dwKey1Index,
							   DWORD dwKey2Index,
							   float fTweenVal);

extern LPD3DXMATRIX GetReferenceFrameMatrixPtr(DWORD dwAnimIndex,
											   DWORD dwRefIndex);

extern void RenderAnimation(DWORD dwAnimIndex,
							DWORD dwWMtxIndex);

extern void RenderAnimationAlpha(DWORD dwAnimIndex,
								 DWORD dwWMtxIndex,
								 float fAlpha);

extern BOOL AddMeshesFromXFA(DWORD dwAnimIndex,
							 LPDATAFILEREF lpDFR,
							 BOOL bNormals,
							 BOOL bOptimize);

extern BOOL AddMeshFromXof(IDirectXFileData* lpData,
						   BOOL bNormals,
						   BOOL bOptimize);

// misc functions

extern float GetNaturalizedTweenValue(float fVal);

// macros

// interpolations
#define GetInterpolatedValue(a,b,u)  (((float)(a))+(((float)(u))*(((float)(b))-((float)(a)))))
#define GetInterpolationValue(a,b,u) ((((float)(u))-((float)(a)))/(((float)(b))-((float)(a))))

// eof

// Copyright 2005-2007 WaveBreak Software
