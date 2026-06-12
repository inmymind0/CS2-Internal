#pragma once
#include "cs2/signatures.h"


namespace hooks {
	bool Init();
	void Shutdown();

	bool SetupPresentHook();
	bool SetupResizeBuffersHook();
	bool SetupCreateSwapChainHook();
	bool SetupCreateMoveHook();
	bool SetupValidateInputHook();
	bool SetupDrawObjectHook();
	bool SetupOnGeneratePrimitivesHook();
}