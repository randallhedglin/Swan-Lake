// Copyright 2005-2007 WaveBreak Software

/////////////////////////////////////////////////////////////////////
// perform step-by-step walkthrough with COMPLETE error checking!! //
/////////////////////////////////////////////////////////////////////

// r3dcol.h -- collision detection header

/* world script reference:

end file with "eof"
enclose comments with "<>"

--environment
alphablend		(on|off)
alphatest		(on|off)
specular		(on|off)
fog				(on|off)
fogparams		start,end,color
ambient			r,g,b
zclip			near,far
gravity			x,y,z
segments		minsize,max-x,max-y,max-z
--materials
material		name,diffuse,ambient,specular,emissive,power
texture			name,filename
--skybox
skybox			radius,offset,left,right,front,back,top,bottom
--meshes
mesh			name,filename
meshbox			name,width,height,depth,material
meshcylinder	name,lowerrad,upperrad,height,slices,stacks,material
meshpolygon		name,lengthofside,sides,material
meshsphere		name,radius,slices,stacks,material
meshteapot		name,material
meshtorus		name,innerrad,outerrad,sides,rings,material
--world objects
swmesh			name,mesh,matrix
dwmesh			name,mesh,matrix
swobject		name,pos,rad
dwobject		name,pos,rad
--lights (must be last for proper render during world editing)
pointlight		name,diffuse,specular,position,range
spotlight		name,diffuse,specular,position,direction,range,theta,phi
dirlight		name,diffuse,direction

*/

// enumerated types //

#ifdef DEBUG_MODE
// world editing modes
typedef enum tagWSEDITMODE
{
	WSM_NAVWALK=1,
	WSM_NAVLOOK,
	WSM_LTPOSXZ,
	WSM_LTPOSXY,
	WSM_LTDIR,
	WSM_SWMPOSX,
	WSM_SWMPOSY,
	WSM_SWMPOSZ,
	WSM_SWMPOSXY,
	WSM_SWMPOSXZ,
	WSM_SWMPOSYZ,
	WSM_SWMYAW,
	WSM_SWMPITCH,
	WSM_SWMROLL,
	WSM_SWMSCALE,
	WSM_SWMSCALEX,
	WSM_SWMSCALEY,
	WSM_SWMSCALEZ,
	WSM_DWMPOSX,
	WSM_DWMPOSY,
	WSM_DWMPOSZ,
	WSM_DWMPOSXY,
	WSM_DWMPOSXZ,
	WSM_DWMPOSYZ,
	WSM_DWMYAW,
	WSM_DWMPITCH,
	WSM_DWMROLL,
	WSM_DWMSCALE,
	WSM_DWMSCALEX,
	WSM_DWMSCALEY,
	WSM_DWMSCALEZ,
	WSM_SWOPOSX,
	WSM_SWOPOSY,
	WSM_SWOPOSZ,
	WSM_SWOPOSXY,
	WSM_SWOPOSXZ,
	WSM_SWOPOSYZ,
	WSM_DWOPOSX,
	WSM_DWOPOSY,
	WSM_DWOPOSZ,
	WSM_DWOPOSXY,
	WSM_DWOPOSXZ,
	WSM_DWOPOSYZ,
	WSM_IDLE=0xFFFFFFFF
} WSEDITMODE,*LPWSEDITMODE;
#endif

// world script opcodes
typedef enum tagWSOPCODE
{
	WS_ABLEND=1,
	WS_ATEST,
	WS_SPECULAR,
	WS_FOG,
	WS_FOGPARAMS,
	WS_AMBIENT,
	WS_ZCLIP,
	WS_GRAVITY,
	WS_SEGMENT,
	WS_PLIGHT,
	WS_SLIGHT,
	WS_DLIGHT,
	WS_MATERIAL,
	WS_TEXTURE,
	WS_MFILE,
	WS_MBOX,
	WS_MCYLINDER,
	WS_MPOLYGON,
	WS_MSPHERE,
	WS_MTEAPOT,
	WS_MTORUS,
	WS_SKYBOX,
	WS_SWMESH,
	WS_DWMESH,
	WS_SWOBJ,
	WS_DWOBJ,
	WS_SWTRI,
	WS_EOF=0xFFFFFFFF
} WSOPCODE,*LPWSOPCODE;

// constants //

#define MAX_RECURSIONS 3

#define DWOBJ_MINSPEED 0.001F
#define DWOBJ_OFFSET   0.001F
#define DWOBJ_UNSTICK  0.01F

#define FRICTION_FACTOR 0.9F

// structures //

#ifdef DEBUG_MODE
// world editing option block
typedef struct tagWSOPTIONBLOCK
{
	// title
	char lpText[16];
	// position
	int nX,nY;
	// rectangle
	RECT rRect;
} WSOPTIONBLOCK,*LPWSOPTIONBLOCK;
#endif

typedef struct tagSEGMENTBLOCK
{
	// number of triangles
	DWORD dwNumTri;
	// triangle index list
	LPDWORD lpdwTriIndexList;
} SEGMENTBLOCK,*LPSEGMENTBLOCK;

// globals //

// static world mesh counters
extern DWORD g_dwNumSWMesh;
extern DWORD g_dwMaxSWMesh;
// static world mesh data
extern LPDWORD g_lpdwSWMMeshIndex;
extern LPDWORD g_lpdwSWMMtxIndex;
extern LPSTR* g_lplpSWMeshNames;
// dynamic world mesh counters
extern DWORD g_dwNumDWMesh;
extern DWORD g_dwMaxDWMesh;
// dynamic world mesh data
extern LPDWORD g_lpdwDWMMeshIndex;
extern LPDWORD g_lpdwDWMMtxIndex;
extern LPSTR* g_lplpDWMeshNames;
#ifdef DEBUG_MODE
// mini font rendering values
extern DWORD g_dwMFTexIdx;
// clip planes
extern float g_fZClipNear;
extern float g_fZClipFar;
// fog color
extern float g_fFogR;
extern float g_fFogG;
extern float g_fFogB;
// ambient color
extern float g_fAmbientR;
extern float g_fAmbientG;
extern float g_fAmbientB;
// current editing indices
extern DWORD g_dwCurLight;
extern DWORD g_dwCurSWMesh;
extern DWORD g_dwCurDWMesh;
extern DWORD g_dwCurSWObj;
extern DWORD g_dwCurDWObj;
// previous editing indices
extern DWORD g_dwLastMat;
extern DWORD g_dwLastTex;
extern DWORD g_dwLastLight;
extern DWORD g_dwLastMesh;
extern DWORD g_dwLastSWMesh;
extern DWORD g_dwLastDWMesh;
extern DWORD g_dwLastSWObj;
extern DWORD g_dwLastDWObj;
#endif
// gravity vector
extern D3DXVECTOR3 g_vGravity;
// static triangle counters
extern DWORD g_dwNumSTri;
extern DWORD g_dwMaxSTri;
// static triangle info
extern LPD3DXVECTOR3 g_lpvSTriVtx1;
extern LPD3DXVECTOR3 g_lpvSTriVtx2;
extern LPD3DXVECTOR3 g_lpvSTriVtx3;
extern LPD3DXVECTOR3 g_lpvSTriNormal;
extern LPDWORD g_lpdwSTriIndexed;
extern LPSEGMENTBLOCK g_lpSTriSegBlocks;
extern DWORD g_dwNumSTriSegBlocks;
// static triangle index list
extern LPDWORD g_lpdwSTriIndexList;
// segment values
extern float g_fMinSTriSegSize;
extern DWORD g_dwMaxSTriSegX;
extern DWORD g_dwMaxSTriSegY;
extern DWORD g_dwMaxSTriSegZ;
extern DWORD g_dwNumSTriSegX;
extern DWORD g_dwNumSTriSegY;
extern DWORD g_dwNumSTriSegZ;
extern DWORD g_dwNumSTriSegXY;
extern int g_nNumSTriSegX1;
extern int g_nNumSTriSegY1;
extern int g_nNumSTriSegZ1;
extern D3DXVECTOR3 g_vSTriSegSize;
extern D3DXVECTOR3 g_vSTriSegASize;
// segment division values
extern D3DXVECTOR3 g_vMinSTriExtents;
extern D3DXVECTOR3 g_vMaxSTriExtents;
// static world object counters
extern DWORD g_dwNumSWObj;
extern DWORD g_dwMaxSWObj;
// static world object data
extern LPDWORD g_lpdwSWObjMtxIdx;
extern LPD3DXVECTOR3 g_lpvSWObjRad;
extern LPSTR* g_lplpSWObjNames;
// dynamic world object counters
extern DWORD g_dwNumDWObj;
extern DWORD g_dwMaxDWObj;
// dynamic world object data
extern LPDWORD g_lpdwDWObjMtxIdx;
extern LPD3DXVECTOR3 g_lpvDWObjSafePos;
extern LPD3DXVECTOR3 g_lpvDWObjRad;
extern LPD3DXVECTOR3 g_lpvDWObjARad;
extern LPD3DXVECTOR3 g_lpvDWObjDisp;
extern LPBOOL g_lpbDWObjSTColl;
extern LPD3DXVECTOR3 g_lpvDWObjSTCollPt;
extern LPDWORD g_lpdwDWObjSTCollIdx;
extern LPSTR* g_lplpDWObjNames;
#ifdef DEBUG_MODE
// world object mesh data
extern DWORD g_dwObjectMeshIdx;
extern DWORD g_dwObjectMtxIdx;
#endif
// collision detection values
extern DWORD g_dwColRetry;
extern DWORD g_dwColNumSTri;
extern DWORD g_dwColIndexNum;
// world camera data
extern BOOL g_bWCEnabled;
extern DWORD g_dwWCPositionObjIdx;
extern DWORD g_dwWCPositionMtxIdx;
extern DWORD g_dwWCTargetMtxIdx;
extern D3DXVECTOR3 g_vWCTargetOffset;
extern D3DXVECTOR3 g_vWCCameraOffset;
extern float g_fWCMaxSpeed;
extern D3DXVECTOR3 g_vWCAhead;

// prototypes //

// static world mesh functions

extern DWORD GetNextStaticWorldMesh(void);

extern DWORD AddStaticWorldMesh(DWORD dwMeshIndex,
								DWORD dwWMtxIndex=NO_OBJECT,
								LPSTR lpName=NULL);

extern DWORD AddStaticWorldMesh(DWORD dwMeshIndex,
							    float fX=0,
							    float fY=0,
							    float fZ=0,
								LPSTR lpName=NULL);

extern DWORD AddStaticWorldMesh(DWORD dwMeshIndex,
								LPD3DXMATRIX lpMtx=NULL,
								LPSTR lpName=NULL);

extern void RenderStaticWorldMeshes(void);

extern void PurgeStaticWorldMeshes(void);

extern DWORD FindStaticWorldMesh(LPCSTR lpName);

extern DWORD GetNextStaticTriangle(void);

extern DWORD AddStaticTriangle(LPD3DXVECTOR3 lpv1,
							   LPD3DXVECTOR3 lpv2,
							   LPD3DXVECTOR3 lpv3);

extern void PurgeStaticTriangles(void);

#ifdef DEBUG_MODE
extern void SegmentStaticTriangles(void);
#endif

extern void ReadWorldScriptStaticTriangleInfo(void);

// dynamic world mesh functions

extern DWORD GetNextDynamicWorldMesh(void);

extern DWORD AddDynamicWorldMesh(DWORD dwMeshIndex,
								 DWORD dwWMtxIndex=NO_OBJECT,
								 LPSTR lpName=NULL);

extern DWORD AddDynamicWorldMesh(DWORD dwMeshIndex,
							     float fX=0,
							     float fY=0,
							     float fZ=0,
								 LPSTR lpName=NULL);

extern DWORD AddDynamicWorldMesh(DWORD dwMeshIndex,
								 LPD3DXMATRIX lpMtx=NULL,
								 LPSTR lpName=NULL);

extern void RenderDynamicWorldMeshes(void);

extern void PurgeDynamicWorldMeshes(void);

extern DWORD FindDynamicWorldMesh(LPCSTR lpName);

// misc functions

#ifdef DEBUG_MODE
extern void AddWorld(LPCSTR lpFile,
					 BOOL bEditInfo=FALSE);
#else
extern void AddWorld(LPCSTR lpFile);
#endif

extern void RenderWorld(DWORD dwMtxIndex);

extern void RenderWorld(float fXCamPos,
						float fYCamPos,
						float fZCamPos);

extern void RenderWorld(void);

extern void PurgeWorldObjects(void);

#ifdef DEBUG_MODE
extern void CompileWorldScript(LPCSTR lpDestFile,
							   LPCSTR lpSrcFile,
							   BOOL bEditMode=FALSE);

extern void AppendWorldScriptStaticTriangleInfo(LPCSTR lpFile);
#endif

extern void SetGravityVector(LPD3DXVECTOR3 lpv);

extern void SetGravityVector(float fX,
							 float fY,
							 float fZ);

#ifdef DEBUG_MODE
extern void RenderStaticTriangle(DWORD dwIndex,
								 BOOL bFace=TRUE,
								 BOOL bNormal=FALSE);

extern void RenderStaticTriangleBoundingBox(void);

extern void RenderStaticTriangleSegmentBlocks(void);

extern void RenderStaticTriangleSegmentBlock(DWORD dwX,
											 DWORD dwY,
											 DWORD dwZ);

extern void InitWorldObjectMesh(void);

extern void RenderDynamicWorldObjectCollisionPoint(DWORD dwIndex);

#endif

#ifdef DEBUG_MODE
// world editing functions

extern void EditWorldScript(LPCSTR lpFile);

extern void InitMiniFont(LPDWORD lpdwMiniFont);

extern DWORD GetMiniChar(LPSTR lpBits);

extern RECT RenderMiniFontString(LPDWORD dwMiniFont,
								 LPSTR lpStr,
								 int nXPos,
								 int nYPos,
								 BOOL bHighlight=FALSE);

extern void CreateDefaultWorldScript(LPCSTR lpFile);

extern BOOL UpdateWorld(void);

extern BOOL SaveWorld(LPCSTR lpFile);

extern DWORD InitDialogEnv(void);

extern BOOL APIENTRY DialogEnvProc(HWND hDlg,
								   UINT Msg,
								   WPARAM wParam,
								   LPARAM lParam);

extern DWORD InitDialogMat(void);

extern BOOL APIENTRY DialogMatProc(HWND hDlg,
								   UINT Msg,
								   WPARAM wParam,
								   LPARAM lParam);

extern DWORD InitDialogTex(void);

extern BOOL APIENTRY DialogTexProc(HWND hDlg,
								   UINT Msg,
								   WPARAM wParam,
								   LPARAM lParam);

extern DWORD InitDialogSky(void);

extern BOOL APIENTRY DialogSkyProc(HWND hDlg,
								   UINT Msg,
								   WPARAM wParam,
								   LPARAM lParam);

extern DWORD InitDialogLight(void);

extern BOOL APIENTRY DialogLightProc(HWND hDlg,
								     UINT Msg,
								     WPARAM wParam,
								     LPARAM lParam);

extern DWORD InitDialogMesh(void);

extern BOOL APIENTRY DialogMeshProc(HWND hDlg,
								    UINT Msg,
								    WPARAM wParam,
								    LPARAM lParam);

extern DWORD InitDialogSWMesh(void);

extern BOOL APIENTRY DialogSWMeshProc(HWND hDlg,
									  UINT Msg,
								      WPARAM wParam,
								      LPARAM lParam);

extern DWORD InitDialogDWMesh(void);

extern BOOL APIENTRY DialogDWMeshProc(HWND hDlg,
									  UINT Msg,
								      WPARAM wParam,
								      LPARAM lParam);

extern DWORD InitDialogSWObj(void);

extern BOOL APIENTRY DialogSWObjProc(HWND hDlg,
								     UINT Msg,
								     WPARAM wParam,
								     LPARAM lParam);

extern DWORD InitDialogDWObj(void);

extern BOOL APIENTRY DialogDWObjProc(HWND hDlg,
								     UINT Msg,
								     WPARAM wParam,
								     LPARAM lParam);

#endif

extern void UnloadWorld(void);

// static world object functions

extern DWORD GetNextStaticWorldObject(void);

extern DWORD AddStaticWorldObject(float fPosX,
								  float fPosY,
								  float fPosZ,
								  float fRadX,
								  float fRadY,
								  float fRadZ,
								  LPSTR lpName=NULL);

extern DWORD AddStaticWorldObject(LPD3DXVECTOR3 lpvPos,
								  LPD3DXVECTOR3 lpvRad,
								  LPSTR lpName=NULL);

extern void PurgeStaticWorldObjects();

extern DWORD FindStaticWorldObject(LPCSTR lpName);

#ifdef DEBUG_MODE
extern void RenderStaticWorldObject(DWORD dwIndex);

extern void RenderStaticWorldObjects(void);
#endif

// dynamic world object functions

extern DWORD GetNextDynamicWorldObject(void);

extern DWORD AddDynamicWorldObject(float fPosX,
								   float fPosY,
								   float fPosZ,
								   float fRadX,
								   float fRadY,
								   float fRadZ,
								   float fAheadX=0,
								   float fAheadY=0,
								   float fAheadZ=1,
								   LPSTR lpName=NULL);

extern DWORD AddDynamicWorldObject(LPD3DXVECTOR3 lpvPos,
								   LPD3DXVECTOR3 lpvRad,
								   LPD3DXVECTOR3 lpvAhead=NULL,
								   LPSTR lpName=NULL);

extern void PurgeDynamicWorldObjects();

extern DWORD FindDynamicWorldObject(LPCSTR lpName);

extern void GetDynamicWorldObjectBoundingBox(DWORD dwIndex,
											 LPD3DXVECTOR3 lpvMin,
											 LPD3DXVECTOR3 lpvMax);

extern void UpdateDynamicWorldObjects(void);

extern void UpdateDynamicWorldObject(DWORD dwIndex);

extern void UpdateStaticTriangleIntersectList(LPD3DXVECTOR3 lpvMin,
											  LPD3DXVECTOR3 lpvMax);

#ifdef DEBUG_MODE
extern void RenderDynamicWorldObjectBoundingBox(DWORD dwIndex);

extern void RenderDynamicWorldObject(DWORD dwIndex);

extern void RenderDynamicWorldObjects(void);

extern void RenderStaticTriangleIntersectList(void);
#endif

// world camera functions

extern void EnableWorldCamera(void);

extern void EnableWorldCamera(DWORD dwCamDWObjIdx,
							  DWORD dwTarDWObjIdx,
							  float fXTarOffset,
							  float fYTarOffset,
							  float fZTarOffset,
							  float fXCamOffset,
							  float fYCamOffset,
							  float fZCamOffset,
							  float fMaxSpeed);

extern void DisableWorldCamera(void);

extern void UpdateWorldCamera(void);

extern void SetWorldCameraViewMatrix(void);

extern void SetWorldCameraObject(DWORD dwDWObjIdx);

extern void SetWorldCameraTarget(DWORD dwDWObjIdx);

extern void SetWorldCameraTargetMatrix(DWORD dwMtxIdx);

extern void SetWorldCameraTargetOffsetVector(LPD3DXVECTOR3 lpv);

extern void SetWorldCameraTargetOffsetVector(float fX,
											 float fY,
											 float fZ);

extern void SetWorldCameraOffsetVector(LPD3DXVECTOR3 lpv);

extern void SetWorldCameraOffsetVector(float fX,
									   float fY,
									   float fZ);

extern void SetWorldCameraMaxSpeed(float fSpeed);

extern void SetWorldCameraAheadVector(LPD3DXVECTOR3 lpv);

extern void SetWorldCameraAheadVector(float fX,
									  float fY,
									  float fZ);

extern void GetWorldCameraAheadVector(LPD3DXVECTOR3 lpv);

extern void GetWorldCameraRightVector(LPD3DXVECTOR3 lpv);

extern void GetWorldCameraTargetPoint(LPD3DXVECTOR3 lpv);

extern void GetWorldCameraTargetPosition(LPD3DXVECTOR3 lpv);

// macros //

#ifdef DEBUG_MODE
#define SET_WSOPTIONBLOCK(ob,t,x,y) { strcpy(ob.lpText,t); ob.nX=x; ob.nY=y; ob.rRect=RenderMiniFontString(dwMiniFont,t,x,y); }

#define RenderOptionBlock(ob) if(IsMouseInRect(ob.rRect)) RenderMiniFontString(dwMiniFont,ob.lpText,ob.nX,ob.nY,TRUE); else RenderMiniFontString(dwMiniFont,ob.lpText,ob.nX,ob.nY,FALSE);
#endif

// dialog identifiers //

#ifdef DEBUG_MODE
// world editor identifiers
#define DLG_WEDIT		1000
#define CHK_ABLEND		DLG_WEDIT+1
#define CHK_ATEST		DLG_WEDIT+2
#define CHK_SPECULAR	DLG_WEDIT+3
#define CHK_FOG			DLG_WEDIT+4
#define EDT_FSTART		DLG_WEDIT+5
#define EDT_FEND		DLG_WEDIT+6
#define EDT_FOGR		DLG_WEDIT+8
#define EDT_FOGG		DLG_WEDIT+9
#define EDT_FOGB		DLG_WEDIT+10
#define EDT_AMBIENTR	DLG_WEDIT+11
#define EDT_AMBIENTG	DLG_WEDIT+12
#define EDT_AMBIENTB	DLG_WEDIT+13
#define EDT_ZNEAR		DLG_WEDIT+14
#define EDT_ZFAR		DLG_WEDIT+15
#define LST_MATERIAL	DLG_WEDIT+16
#define EDT_DIFFUSER	DLG_WEDIT+17
#define EDT_DIFFUSEG	DLG_WEDIT+18
#define EDT_DIFFUSEB	DLG_WEDIT+19
#define EDT_DIFFUSEA	DLG_WEDIT+20
#define EDT_AMBIENTA	DLG_WEDIT+21
#define EDT_SPECULARR	DLG_WEDIT+22
#define EDT_SPECULARG	DLG_WEDIT+23
#define EDT_SPECULARB	DLG_WEDIT+24
#define EDT_SPECULARA	DLG_WEDIT+25
#define EDT_EMISSIVER	DLG_WEDIT+26
#define EDT_EMISSIVEG	DLG_WEDIT+27
#define EDT_EMISSIVEB	DLG_WEDIT+28
#define EDT_EMISSIVEA	DLG_WEDIT+29
#define EDT_POWER		DLG_WEDIT+30
#define EDT_NAME		DLG_WEDIT+31
#define BTN_ADD			DLG_WEDIT+32
#define LST_NAME		DLG_WEDIT+33
#define BTN_DELETE		DLG_WEDIT+34
#define BTN_SAVE		DLG_WEDIT+35
#define EDT_FILE		DLG_WEDIT+36
#define CHK_FRONT		DLG_WEDIT+37
#define LST_FRONT		DLG_WEDIT+38
#define CHK_BACK		DLG_WEDIT+39
#define LST_BACK		DLG_WEDIT+40
#define CHK_LEFT		DLG_WEDIT+41
#define LST_LEFT		DLG_WEDIT+42
#define CHK_RIGHT		DLG_WEDIT+43
#define LST_RIGHT		DLG_WEDIT+44
#define CHK_TOP			DLG_WEDIT+45
#define LST_TOP			DLG_WEDIT+46
#define CHK_BOTTOM		DLG_WEDIT+47
#define LST_BOTTOM		DLG_WEDIT+48
#define EDT_X			DLG_WEDIT+49
#define EDT_Y			DLG_WEDIT+50
#define EDT_Z			DLG_WEDIT+51
#define EDT_RAD			DLG_WEDIT+52
#define RAD_POINT		DLG_WEDIT+53
#define RAD_SPOT		DLG_WEDIT+54
#define RAD_DIR			DLG_WEDIT+55
#define EDT_RANGE		DLG_WEDIT+56
#define EDT_INNERCONE	DLG_WEDIT+57
#define EDT_OUTERCONE	DLG_WEDIT+58
#define EDT_POSX		DLG_WEDIT+59
#define EDT_POSY		DLG_WEDIT+60
#define EDT_POSZ		DLG_WEDIT+61
#define EDT_DIRX		DLG_WEDIT+62
#define EDT_DIRY		DLG_WEDIT+63
#define EDT_DIRZ		DLG_WEDIT+64
#define BTN_POSXY		DLG_WEDIT+65
#define BTN_POSXZ		DLG_WEDIT+66
#define BTN_DIR			DLG_WEDIT+67
#define RAD_FILE		DLG_WEDIT+68
#define RAD_BOX			DLG_WEDIT+69
#define RAD_CYLINDER	DLG_WEDIT+70
#define RAD_POLYGON		DLG_WEDIT+71
#define RAD_SPHERE		DLG_WEDIT+72
#define RAD_TEAPOT		DLG_WEDIT+73
#define RAD_TORUS		DLG_WEDIT+74
#define LST_MAT			DLG_WEDIT+75
#define EDT_BWIDTH		DLG_WEDIT+76
#define EDT_BHEIGHT		DLG_WEDIT+77
#define EDT_BDEPTH		DLG_WEDIT+78
#define EDT_CUPPERRAD	DLG_WEDIT+79
#define EDT_CLOWERRAD	DLG_WEDIT+80
#define EDT_CHEIGHT		DLG_WEDIT+81
#define EDT_SLICES		DLG_WEDIT+82
#define EDT_STACKS		DLG_WEDIT+83
#define EDT_PLOS		DLG_WEDIT+84
#define EDT_PSIDES		DLG_WEDIT+85
#define EDT_TINNERRAD	DLG_WEDIT+86
#define EDT_TOUTERRAD	DLG_WEDIT+87
#define EDT_TSIDES		DLG_WEDIT+88
#define EDT_TRINGS		DLG_WEDIT+89
#define EDT_RIGHTX		DLG_WEDIT+90
#define EDT_RIGHTY		DLG_WEDIT+91
#define EDT_RIGHTZ		DLG_WEDIT+92
#define EDT_UPX			DLG_WEDIT+93
#define EDT_UPY			DLG_WEDIT+94
#define EDT_UPZ			DLG_WEDIT+95
#define EDT_AHEADX		DLG_WEDIT+96
#define EDT_AHEADY		DLG_WEDIT+97
#define EDT_AHEADZ		DLG_WEDIT+98
#define BTN_YAW			DLG_WEDIT+99
#define BTN_PITCH		DLG_WEDIT+100
#define BTN_ROLL		DLG_WEDIT+101
#define BTN_POSX		DLG_WEDIT+102
#define BTN_POSY		DLG_WEDIT+103
#define BTN_POSZ		DLG_WEDIT+104
#define BTN_POSYZ		DLG_WEDIT+105
#define LST_MESH		DLG_WEDIT+106
#define EDT_SCALEX		DLG_WEDIT+107
#define EDT_SCALEY		DLG_WEDIT+108
#define EDT_SCALEZ		DLG_WEDIT+109
#define BTN_SCALE		DLG_WEDIT+110
#define BTN_SCALEX		DLG_WEDIT+111
#define BTN_SCALEY		DLG_WEDIT+112
#define BTN_SCALEZ		DLG_WEDIT+113
#define EDT_GRAVITYX	DLG_WEDIT+114
#define EDT_GRAVITYY	DLG_WEDIT+115
#define EDT_GRAVITYZ	DLG_WEDIT+116
#define EDT_MINSIZE		DLG_WEDIT+117
#define EDT_MAXX		DLG_WEDIT+118
#define EDT_MAXY		DLG_WEDIT+119
#define EDT_MAXZ		DLG_WEDIT+120
#define EDT_RADX		DLG_WEDIT+121
#define EDT_RADY		DLG_WEDIT+122
#define EDT_RADZ		DLG_WEDIT+123
#endif

// eof //

// Copyright 2005-2007 WaveBreak Software
