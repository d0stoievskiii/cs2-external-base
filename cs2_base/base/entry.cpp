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
	
	std::vector<Entity> entities(64);
	aimbot::init();
	for (int i = 1; i < 64; i++) {
		entities[i].init(i, entityList);
		entities[i].getController();
		if (entities[i].controller == 0) continue;
		entities[i].getName();
		entities[i].getPawn();
		entities[i].getHealth();
		entities[i].getTeam();
		if (entities[i].team == localPlayer.team) continue;
		if ((entities[i].health > 100 || entities[i].health <= 0)) {
			//if (aimbot::aimTarget == i) aimbot::aimTarget = 0;
			continue;
		}
		if (!(strcmp(entities[i].name.c_str(), "DemoRecorder")) || !(strcmp(entities[i].name.c_str(), "NoName"))) continue;
		if (shared::config.espconfig.gState) {
			esp::draw(entities[i], viewMatrix);		
		}
		aimbot::searchTarget(localPlayer, entities[i]);
	}
	if (shared::config.aimconfig.drawFov) {
		aimbot::drawFOV();
	}
	aimbot::aim(localPlayer, entities[aimbot::aimTarget]);
	aimbot::trigger(localPlayer, entityList);
}