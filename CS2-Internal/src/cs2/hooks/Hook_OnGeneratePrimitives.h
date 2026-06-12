#pragma once

class CSceneAnimatableObject;
class CMeshPrimitiveOutputBuffer_t;

typedef void(__fastcall* OnGeneratePrimitivesFn)(void*, CSceneAnimatableObject*, void*, CMeshPrimitiveOutputBuffer_t*);
inline OnGeneratePrimitivesFn oOnGeneratePrimitives = nullptr;

void __fastcall hkOnGeneratePrimitives(void* pSceneSys, CSceneAnimatableObject* pSceneObject, void* pUnk, CMeshPrimitiveOutputBuffer_t* pBuf);
