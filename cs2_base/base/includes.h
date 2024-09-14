#pragma once

#include "gameobjs.h"
#include "../util/config.h"
#include "../util/discord.h"


namespace shared {
	inline CONFIG config;
	inline discord::DiscordController DC;
	inline discord::draw::Frame frame;
	inline discord::draw::Pixel red = { 0, 0, 255, 200 };
	inline discord::draw::Pixel white = { 255, 255, 255, 200 };
	inline discord::draw::Pixel green = { 0, 255, 0, 200 };
	inline int screenx;
	inline int screeny;
	inline float verticalScale;
	inline float horizontalScale;
}