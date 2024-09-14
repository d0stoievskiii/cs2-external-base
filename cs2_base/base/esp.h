#pragma once

#include "includes.h"

#define HEAD 6

namespace esp {
	void drawBox(Entity& entity, VIEW_MATRIX& viewMatrix);
	void drawHealthBar(discord::draw::Point min, int32_t health, int height);
	void draw(Entity& entity, VIEW_MATRIX& viewMatrix);
}