#include "antiaim.h"
#include "cs2/settings.h"
#include "cs2/client.h"
#include "offsets/offsets.h"
#include <algorithm>

namespace Features {
	namespace AntiAim {
		QAngle_t g_RealAngles = {};
		QAngle_t g_FakeAngles = {};
		bool g_IsUsing = false;

		void FixButtons(CUserCmd* pUserCmd) {
			if (!pUserCmd || !pUserCmd->csgoUserCmd.pBaseCmd)
				return;

			auto* pBaseCmd = pUserCmd->csgoUserCmd.pBaseCmd;

			if (pBaseCmd->flForwardMove > 0.f)
				pUserCmd->nButtons.nValue |= IN_FORWARD;
			else if (pBaseCmd->flForwardMove < 0.f)
				pUserCmd->nButtons.nValue |= IN_BACK;

			if (pBaseCmd->flSideMove > 0.f)
				pUserCmd->nButtons.nValue |= IN_MOVELEFT;
			else if (pBaseCmd->flSideMove < 0.f)
				pUserCmd->nButtons.nValue |= IN_MOVERIGHT;
		}

		void MovementCorrection(CUserCmd* pUserCmd, const QAngle_t& RealAngles, const QAngle_t& FakeAngles) {
			if (!pUserCmd || !pUserCmd->csgoUserCmd.pBaseCmd)
				return;
		}

		void Run(CUserCmd* pCmd) {
			if (!pCmd || !Settings::AntiAim::Enabled || !CCS2Client::Get().IsInGame())
				return;

			g_IsUsing = (pCmd->nButtons.nValue & IN_USE);

			if (g_IsUsing)
				return;

			if (true) {
				auto* pBaseCmd = pCmd->csgoUserCmd.pBaseCmd;
				if (pBaseCmd && pBaseCmd->pViewAngles) {
					g_RealAngles = pBaseCmd->pViewAngles->angValue;

					QAngle_t pModifiedViewAngles = g_RealAngles;

					if (Settings::AntiAim::PitchMode == 1) {
						pModifiedViewAngles.pitch = 89.0f;
					} else if (Settings::AntiAim::PitchMode == 2) {
						pModifiedViewAngles.pitch = -89.0f;
					} else if (Settings::AntiAim::PitchMode == 3) {
						pModifiedViewAngles.pitch = 0.f;
					}

					float yawMod = 0.f;
					if (Settings::AntiAim::YawMode == 1) {
						yawMod = 180.f;
					} else if (Settings::AntiAim::YawMode == 2) {
						yawMod = -90.f;
					} else if (Settings::AntiAim::YawMode == 3) {
						yawMod = 90.f;
					}

					pModifiedViewAngles.yaw += yawMod + Settings::AntiAim::YawOffset;
					pModifiedViewAngles.roll = Settings::AntiAim::Roll;

					Math::NormalizeAngles(pModifiedViewAngles);
					Math::ClampAngles(pModifiedViewAngles);

					pBaseCmd->pViewAngles->angValue = pModifiedViewAngles;

					MovementCorrection(pCmd, g_RealAngles, pModifiedViewAngles);

					bool is_firing = (pCmd->nButtons.nValue & IN_ATTACK);
					bool is_knifing = (pCmd->nButtons.nValue & IN_SECOND_ATTACK);

					if (is_firing || is_knifing) {
						pCmd->SetSubTickAngle(g_RealAngles);
					} else {
						pCmd->SetSubTickAngle(pModifiedViewAngles);
					}

					g_FakeAngles = pModifiedViewAngles;
				}
			}
		}
	}
}
