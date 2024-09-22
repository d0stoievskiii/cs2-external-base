#pragma once

#include "syscall.h"

class mouseController {
public:
	uint32_t ntUserSendInputIdx;
	LPVOID call;

	mouseController() {
		ntUserSendInputIdx = syscall::getIndex(L"win32u.dll", "NtUserSendInput");
		call = syscall::allocate_call(ntUserSendInputIdx);
	}

	BOOLEAN WINAPI NtUserSendInput(UINT cInputs, LPINPUT pInputs, int cbSize) {
		if (!call) return 0;
		NTSTATUS result = reinterpret_cast<NTSTATUS(NTAPI*)(UINT, LPINPUT, int)>(call)(cInputs, pInputs, cbSize);
		return (result > 0);
	}

	BOOLEAN WINAPI move(int x, int y) {
		if (!x && !y) return 0;
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		return NtUserSendInput(1, &input, sizeof(input));
	}

	BOOLEAN WINAPI click() {
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		BOOLEAN ret = NtUserSendInput(1, &input, sizeof(input));
		Sleep(20);
		INPUT input2 = { 0 };
		input2.type = INPUT_MOUSE;
		input2.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		return (ret & NtUserSendInput(1, &input2, sizeof(input2)));
	}
};