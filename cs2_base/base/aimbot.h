#pragma once

#include "includes.h"
#include "../util/mousecontroller.h"

typedef void(*targetcmp)(LocalPlayer&, Entity&);

namespace aimbot {
	inline mouseController MC;
	inline int aimTarget;
	inline float m_yaw = 0.022f;
	inline float m_pitch = -0.022f;
	inline float min;
	inline targetcmp target_selection_method;

	FORCEINLINE void init() {
		min = FLT_MAX;
		aimTarget = 0;
	}
	FORCEINLINE void uninit() {
		VirtualFree(MC.call, 0, MEM_RELEASE);
	}

	inline Vec3 normalizeAngles(Vec3& angles);
	inline Vec3 calculateAngles(Vec3& v1, Vec3& v2);
	void aim(LocalPlayer& localPlayer, Entity& entity);
	Vec2 rcs(LocalPlayer& localPlayer);
	void trigger(LocalPlayer& localPlayer, uintptr_t entityList);
	void drawFOV();
	void searchTarget(LocalPlayer& localPlayer, Entity& entity);
	void _locateClosestToPlayer(LocalPlayer& localPlayer, Entity& entity);
	void _locateClosestToCrosshair(LocalPlayer& localPlayer, Entity& entity);
}