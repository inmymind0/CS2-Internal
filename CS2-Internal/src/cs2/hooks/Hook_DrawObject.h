#pragma once

class CMaterial2;
struct MeshColor_t;
class CSceneAnimatableObject;
class CMeshData;

typedef void(__fastcall* DrawObjectFn)(void*, void*, CMeshData*, int, void*, void*, void*, void*);
inline DrawObjectFn oDrawObject = nullptr;

void __fastcall hkDrawObject(void* pDesc, void* pDX11, CMeshData* pMeshData, int nDataCount, void* pView, void* pLayer, void* pUnk, void* pUnk2);
