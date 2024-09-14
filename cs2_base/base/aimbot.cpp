#include "aimbot.h"


inline Vec3 aimbot::calculateAngles(Vec3& v1, Vec3& v2) {
	return ((Vec3)(v2 - v1)).toAngles();
}

inline Vec3 aimbot::normalizeAngles(Vec3& angles) {

	//x is yaw and goes is in range [-89, 89]
	if (angles.x > 89.0f && angles.x <= 180.f) {
		angles.x = 89.0f;
	}
	if (angles.x > 180.0f) {
		angles.x -= 360.0f;
	}
	if (angles.x < -89.0f) {
		angles.x - 89.0f;
	}


	//in terms of game memory, y pitch and has image in [-180, 180]
	if (angles.y > 180.0f) {
		angles.y -= 360.0f;
	}
	if (angles.y < -180.0f) {
		angles.y += 360.0f;
	}

	angles.z = 0;

	return angles;
}

void aimbot::aim(LocalPlayer& localPlayer, Entity& entity) {
	Vec3 aimPos, angle;
	Vec2 angToPix;

	aimPos = entity.getBonePos(6);
	localPlayer.getCameraPos();
	localPlayer.getViewAngles();
	localPlayer.getFOVSentitivityAdjust();
	Vec3 diff = aimPos - localPlayer.cameraPos;
	angle = calculateAngles(localPlayer.cameraPos, aimPos);
	angle = angle - localPlayer.viewAngles;
	angle = normalizeAngles(angle);

	angToPix.x = -(angle.y / (0.022f * localPlayer.sens * localPlayer.FOVSAdjust)) * shared::verticalScale*0.43f;
	angToPix.y = (angle.x / (0.022f * localPlayer.sens* localPlayer.FOVSAdjust)) * shared::horizontalScale*0.33f;

	MC.move((int)angToPix.x, (int)angToPix.y);
}