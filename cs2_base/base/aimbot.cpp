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
	Vec2 angToPix = {0.0f, 0.0f};
	Vec3 angle = entity.aimToAngle;
	if (aimTarget < 1) goto rcs_start;
	if (!(shared::config.aimconfig.gState)) goto rcs_start;
	if (!(GetAsyncKeyState(shared::config.aimconfig.HOTKEYmap[shared::config.aimconfig.HOTKEYS[shared::config.aimconfig.aim_HOTKEY]]))) goto rcs_start;

	angToPix.x = (angle.y / (m_pitch * localPlayer.sens * localPlayer.FOVSAdjust)) * (1 - shared::config.aimconfig.smoothing);
	angToPix.y = (angle.x / (m_yaw * localPlayer.sens * localPlayer.FOVSAdjust)) * (1 - shared::config.aimconfig.smoothing);

rcs_start:
	if (localPlayer.getShotsFired() > 1) {
		Vec2 recoilComp = rcs(localPlayer);
		if (!(recoilComp.x) && !(recoilComp.y)) {
			angToPix.x = 0;//player is already aiming and shooting at target and theres no recoil compensation
			angToPix.y = 0;//for the current shot(because we already compensated for it) then reaiming would undo the previous compensation
		}// and we would overshoot
		else {
		angToPix.x += recoilComp.x;
		angToPix.y += recoilComp.y;
		}
	}
	else {
		shared::oldPunch = { 0.0f, 0.0f };
	}

	MC.move((int)angToPix.x, (int)angToPix.y);
}

void aimbot::drawFOV() {
	auto center = discord::draw::Point(shared::DC.framebuffer->Width / 2, shared::DC.framebuffer->Height / 2);
	discord::draw::Circle(shared::frame, center, shared::config.aimconfig.fov*10, shared::white);
}

Vec2 aimbot::rcs(LocalPlayer& localPlayer) {
	Vec2 angToPix = {0.0f, 0.0f};
	if (shared::config.aimconfig.isRCS) {
		localPlayer.getAimPunchCache();
		if (localPlayer.aimPunchCache.count <= 0 || localPlayer.aimPunchCache.count > 0xFFFF) return angToPix;
		localPlayer.getAimPunchAngle();
		localPlayer.getFOVSentitivityAdjust();
		static uintptr_t bullet;
		if (localPlayer.aimPunchCache.count == bullet) return angToPix;
		float scale = shared::config.aimconfig.RCScale * 2.0f;

		angToPix.x = ((shared::oldPunch.y - localPlayer.aimPunchAngle.y)*scale / (m_pitch * localPlayer.sens * localPlayer.FOVSAdjust));
		angToPix.y = ((shared::oldPunch.x - localPlayer.aimPunchAngle.x)*scale / (m_yaw * localPlayer.sens * localPlayer.FOVSAdjust));

		shared::oldPunch = localPlayer.aimPunchAngle;
		//printf("<%.2f, %.2f>\n", localPlayer.aimPunchAngle.x, localPlayer.aimPunchAngle.y);
		//printf("[%.2f, %.2f]\n", angToPix.x, angToPix.y);
		
		//MC.move((int)angToPix.x, (int)angToPix.y);
		bullet = localPlayer.aimPunchCache.count;
	}
	return angToPix;
}

void aimbot::_locateClosestToPlayer(LocalPlayer& localPlayer, Entity& entity) {
	Vec3 dist = (localPlayer.pos - entity.origin);
	if (dist.length() < min) {
		min = dist.length();
		aimTarget = entity.id;
	}
}

void aimbot::_locateClosestToCrosshair(LocalPlayer& localPlayer, Entity& entity) {
	if (entity.distToCrosshair < min) {
		min = entity.distToCrosshair;
		aimTarget = entity.id;
	}
}

void aimbot::searchTarget(LocalPlayer& localPlayer, Entity& entity) {
	Vec3 aimPos, angle;
	aimPos = entity.getBonePos(shared::config.aimconfig.bonemap[shared::config.aimconfig.bones[shared::config.aimconfig.bone]]);
	localPlayer.getCameraPos();
	localPlayer.getViewAngles();
	localPlayer.getFOVSentitivityAdjust();
	angle = calculateAngles(localPlayer.cameraPos, aimPos);
	angle = angle - localPlayer.viewAngles;
	angle = normalizeAngles(angle);

	entity.aimToAngle = angle;
	entity.distToCrosshair = angle.length();

	if (shared::config.aimconfig.fov < entity.distToCrosshair) return;

	target_selection_method(localPlayer, entity);
}

void aimbot::trigger(LocalPlayer& localPlayer, uintptr_t entityList) {
	int chEntityIdx = localPlayer.getCrosshairEntity();
	if (chEntityIdx <= 0) return;

	Entity entity = Entity();
	entity.init(0, entityList);
	entity.getPawnByID(chEntityIdx);
	entity.getHealth();

	if (entity.health > 0 && entity.health <= 100) {
		if ((GetAsyncKeyState(shared::config.aimconfig.HOTKEYmap[shared::config.aimconfig.HOTKEYS[shared::config.aimconfig.trigger_HOTKEY]]))) {
			MC.click();
		}
	}
}
