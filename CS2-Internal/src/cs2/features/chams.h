#pragma once
#include <optional>
#include "cs2/classes/CMaterial2.h"

namespace Features {
	namespace Chams {
		bool Initialize();
		void Shutdown();

		bool OnDrawObject(
			void* pDesc,
			void* pDX11,
			CMeshData* pMeshData,
			int nDataCount,
			void* pView,
			void* pLayer,
			void* pUnk,
			void* pUnk2
		);

		bool OnGeneratePrimitives(
			void* pSceneSys,
			CSceneAnimatableObject* pSceneObject,
			void* pUnk,
			CMeshPrimitiveOutputBuffer_t* pBuf
		);
	}
}