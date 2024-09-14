#pragma once

#include <string>
#include "../util/mat.h"
#include "../util/memmanager.h"
#include "offsets.h"

inline MemoryManager MM;

typedef struct _UTILVEC {
    uintptr_t x = 0;
    uintptr_t y = 0;
} UTILVEC;

class Entity {
public:
    uint32_t idx;

    int32_t id;
    uintptr_t entityList;

    Entity(int32_t index, uintptr_t list) {
        id = index;
        entityList = list;
    }

    uintptr_t getEntry(int32_t index);
    uintptr_t getEntry(uint32_t index);

    uintptr_t controller;
    uintptr_t getController();

    int32_t health;
    int32_t getHealth();

    int32_t getHealth2();

    uintptr_t team;
    uintptr_t getTeam();

    Vec3 origin;
    Vec3 getOrigin();

    uintptr_t GameSceneNode;
    uintptr_t getGameSceneNode();

    uintptr_t boneArray;
    uintptr_t getBoneArray();

    uintptr_t pawn;
    uintptr_t getPawn();

    std::string name;
    std::string getName();

    Vec3 getBonePos(int bone);
};

class LocalPlayer {
public:
    uintptr_t controller;
    uintptr_t client;

    LocalPlayer(uintptr_t _client) {
        client = _client;
        controller = MM.ReadMem<uintptr_t>(client + dwLocalPlayerController);
    }

    uintptr_t team;
    uintptr_t getTeam();

    uintptr_t pawn;
    uintptr_t getPawn();
    
    Vec3 viewAngles;
    Vec3 getViewAngles();
    Vec3 getViewAngles2();

    Vec3 cameraPos;
    Vec3 getCameraPos();

    Vec3 pos;
    Vec3 getPos();

    float sens;
    float getSens();

    float FOVSAdjust;
    float getFOVSentitivityAdjust();

    UTILVEC aimPunchCache;
    UTILVEC getAimPunchCache();

    Vec2 aimPunchAngle;
    Vec2 getAimPunchAngle();

    int getShotsFired();

    bool getIsScoped();


};

class Game {
public:
    uintptr_t rules;

    Game(uintptr_t client) {
        rules = MM.ReadMem<uintptr_t>(client + dwGameRules);
    }

    bool isWarmUp();
    bool isMatch();
    bool isFreezeTime();
};