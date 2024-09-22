#include "base/entry.h"

#define W 1280
#define H 720


int main(void) {

	printf("Starting...");
	getchar();

	do {
		printf("Looking for cs2\n");
		MM.getProcess(L"cs2.exe");
	} while (MM.pID == 0);

	printf("cs2 found\n");

	MM.getModule(L"client.dll");

	printf("client dll found\n");

	Game game = Game(MM.mInfo.base);

	shared::DC = discord::DiscordController{};
	shared::DC.MM = MM;

	if (!discord::connect(shared::DC)) {
		printf("Handle to framebuffer not established\n");
		exit(-1);
	}
	printf("Discord overlay hijacked\n");

	shared::frame = discord::draw::createFrame(W, H);

	shared::screenx = GetSystemMetrics(SM_CXSCREEN);
	shared::screeny = GetSystemMetrics(SM_CYSCREEN);
	shared::verticalScale = ((float)W / shared::screenx);
	shared::horizontalScale = ((float)H / shared::screeny);
	LoadLibrary(L"win32u");
	aimbot::MC = mouseController();
	aimbot::target_selection_method = aimbot::_locateClosestToCrosshair;
	printf("press any key...");
	getchar();

	while (true) {
		if (game.isMatch()) {
			Sleep(10);
			discord::draw::clearFrame(shared::frame);
			entry::gameLoop();
			discord::sendFrame(shared::DC, (uint32_t)W, (uint32_t)H, shared::frame.buffer, shared::frame.size);
		}
		if ((GetAsyncKeyState(VK_END))) break;
	}

	aimbot::uninit();

	discord::draw::clearFrame(shared::frame);
	discord::sendFrame(shared::DC, (uint32_t)W, (uint32_t)H, shared::frame.buffer, shared::frame.size);
	discord::disconnect(shared::DC);
	printf("discord disconnected...\n");

	return 0;
}