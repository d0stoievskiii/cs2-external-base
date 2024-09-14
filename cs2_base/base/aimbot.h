#pragma once

#include "includes.h"
#include "../util/mousecontroller.h"

namespace aimbot {
	inline mouseController MC;
	inline Vec3 calculateAngles(Vec3& v1, Vec3& v2);
	inline Vec3 normalizeAngles(Vec3& angles);
	void aim(LocalPlayer& localPlayer, Entity& entity);
	void rcs(LocalPlayer& localPlayer);
	void trigger(LocalPlayer& localPlayer);
}