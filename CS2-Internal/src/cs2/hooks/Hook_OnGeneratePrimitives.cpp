#include "Hook_OnGeneratePrimitives.h"
#include "cs2/features/chams.h"

void __fastcall hkOnGeneratePrimitives(void* pSceneSys, CSceneAnimatableObject* pSceneObject, void* pUnk, CMeshPrimitiveOutputBuffer_t* pBuf) {
	if (Features::Chams::OnGeneratePrimitives(pSceneSys, pSceneObject, pUnk, pBuf)) {
		return;
	}
	oOnGeneratePrimitives(pSceneSys, pSceneObject, pUnk, pBuf);
}
