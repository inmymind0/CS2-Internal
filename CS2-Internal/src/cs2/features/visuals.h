#pragma once
#include <cstdint>
#include "imgui/imgui.h"
#include "core/math/math.h"

namespace Features {
	namespace Visuals {
		void RenderESP(ImDrawList* drawList, uintptr_t entityList, uintptr_t localController, uint8_t localTeam, const ViewMatrix& viewMatrix, int width, int height);
	}
}
