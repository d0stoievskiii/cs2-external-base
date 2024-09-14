#pragma once

#include "syscall.h"

class mouseController {
public:
	uint32_t ntUserSendInputIdx;

	mouseController() {
		ntUserSendInputIdx = syscall::getIndex(L"win32u.dll", "NtUserSendInput");
	}

	BOOLEAN WINAPI NtUserSendInput(UINT cInputs, LPINPUT pInputs, int cbSize) {
		LPVOID call = syscall::allocate_call(ntUserSendInputIdx);
		if (!call) return 0;
		NTSTATUS result = reinterpret_cast<NTSTATUS(NTAPI*)(UINT, LPINPUT, int)>(call)(cInputs, pInputs, cbSize);
		VirtualFree(call, 0, MEM_RELEASE);

		return (result > 0);
	}

	BOOLEAN WINAPI move(int x, int y) {
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		return NtUserSendInput(1, &input, sizeof(input));
	}
};