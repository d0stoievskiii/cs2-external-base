#include "esp.h"

void esp::drawBox(Entity& entity, VIEW_MATRIX& viewMatrix) {
	entity.getOrigin();
	if (ISZERO(entity.origin)) return;
	Vec3 headPos = entity.getBonePos(HEAD);

	Vec2 originToScreen, headToScreen;
	if (!(entity.origin.world2screen(viewMatrix, &originToScreen)) || !(headPos.world2screen(viewMatrix, &headToScreen))) return;

	int height = abs(int(headToScreen.y - originToScreen.y));
	int halfwidth = int(height / 4);

	discord::draw::Point max = { originToScreen.x + halfwidth, originToScreen.y };
	discord::draw::Point min = { originToScreen.x - halfwidth, headToScreen.y - height/10 };

	discord::draw::Rect(shared::frame, max, min, shared::red);
	//printf("Drawn for %s\n", entity.getName().c_str());
	drawHealthBar(min, entity.health, height + height/10);
}

void esp::drawHealthBar(discord::draw::Point min, int health, int height) {

	min.x += -height / 10;

	float healthfrac = (health / 100.0f);

	discord::draw::Pixel color = { 0, int(250.0f * healthfrac), int(250.0f*(1-healthfrac)), 200 };
	//printf("HEALTH: %d\n", health);
	discord::draw::Point max = { min.x + height / 20, min.y + height};
	discord::draw::Rect(shared::frame, max, min, shared::white);
	min.y = max.y - height*healthfrac;
	//printf("min.y %d max.y %d\n", min.y, max.y);
	discord::draw::fillRect(shared::frame, max, min, color);
}

void esp::draw(Entity& entity, VIEW_MATRIX& viewMatrix) {
	drawBox(entity, viewMatrix);
}