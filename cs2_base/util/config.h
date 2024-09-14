#pragma once

#include <string>
#include <vector>
#include <map>
#include <WinUser.h>

typedef struct _ESP_CONFIG {

    bool gState = false;
    bool isBox = true;
    bool isHealthBar = true;

    float spottedColors[3] = { 0.f, 1.f, 0.75f };
	float notSpottedColors[3] = { 1.f, 0.f, 0.75f };
    float const width = 2.5f;

} ESP_CONFIG, *PESP_CONFIG;

typedef struct _AIMBOT_CONFIG {

    bool gState = false;
    bool isRCS = false;
    bool isLOCK = false;
    bool isTrigger = false;

    bool isTriggerHotkey = false;
    bool isAimHotkey = false;

    float smoothing = 1.0f;
    float fov = 5.0f;

    int bone = 0;
    std::vector<std::string> bones = { "Head", "Neck", "Chest" };
    std::map<std::string, int> bonemap = { {"Head", 6} , {"Neck", 5}, {"Chest", 4} };


    int aim_HOTKEY_select = 0;
    int trigger_HOTKEY_select = 1;
    int aim_HOTKEY = 0;
    int trigger_HOTKEY = 0;

    std::vector<std::string> HOTKEYS = { "Left mouse", "Right mouse", "MOUSE4", "MOUSE5" ,"SHIFT", "ALT", "CTRL" };
	std::map <std::string, int> HOTKEYmap = { {"SHIFT", VK_SHIFT}, {"ALT", VK_MENU}, {"CTRL", VK_CONTROL}, {"Left mouse", VK_LBUTTON}, {"Right mouse", VK_RBUTTON}, {"MOUSE4", VK_XBUTTON2}, {"MOUSE5", VK_XBUTTON1} };
} AIM_CONFIG, *PAIM_CONFIG;

struct CONFIG {
    ESP_CONFIG espconfig;
    AIM_CONFIG aimconfig;
};