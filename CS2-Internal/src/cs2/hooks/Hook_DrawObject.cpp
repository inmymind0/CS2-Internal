#include "Hook_DrawObject.h"
#include "cs2/features/chams.h"
#include "cs2/settings.h"

void __fastcall hkDrawObject(void* pDesc, void* pDX11, CMeshData* pMeshData, int nDataCount, void* pView, void* pLayer, void* pUnk, void* pUnk2) {
	if (Features::Chams::OnDrawObject(pDesc, pDX11, pMeshData, nDataCount, pView, pLayer, pUnk, pUnk2)) {
		return;
	}
	oDrawObject(pDesc, pDX11, pMeshData, nDataCount, pView, pLayer, pUnk, pUnk2);
}
