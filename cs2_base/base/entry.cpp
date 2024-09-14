#include "entry.h"


void entry::gameLoop() {
	uintptr_t entityList = MM.ReadMem<uintptr_t>(MM.mInfo.base + dwEntityList);
	LocalPlayer localPlayer = LocalPlayer(MM.mInfo.base);
	VIEW_MATRIX viewMatrix = MM.ReadMem<VIEW_MATRIX>(MM.mInfo.base + dwViewMatrix);

	localPlayer.getPawn();
	localPlayer.getTeam();
	localPlayer.getSens();
	localPlayer.getCameraPos();
	localPlayer.getViewAngles();
	// printf("camera @ <%.2f, %.2f, %.2f>\n", localPlayer.cameraPos.x, localPlayer.cameraPos.y, localPlayer.cameraPos.z);
	//printf("vAngles @ <%.2f, %.2f, %.2f>\n", localPlayer.viewAngles.x, localPlayer.viewAngles.y, localPlayer.viewAngles.z);

	for (int i = 0; i < 64; i++) {
		Entity entity = Entity(i, entityList);
		entity.getController();
		if (entity.controller == 0) continue;
		entity.getName();
		entity.getPawn();
		entity.getHealth();
		entity.getTeam();
		if (entity.team == localPlayer.team) continue;
		if (!(strcmp(entity.name.c_str(), "DemoRecorder")) || !(strcmp(entity.name.c_str(), "NoName")) || (entity.health > 100 || entity.health <= 0)) continue;
		esp::draw(entity, viewMatrix);
		aimbot::aim(localPlayer, entity);
	}
}