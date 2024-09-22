#include "gameobjs.h"

uintptr_t LocalPlayer::getPawn() {
    pawn = MM.ReadMem<uintptr_t>(client + dwLocalPlayerPawn);
    return pawn;
}

uintptr_t LocalPlayer::getTeam() {
    team = MM.ReadMem<uintptr_t>(controller + m_iTeamNum);
    return team;
}

Vec3 LocalPlayer::getViewAngles() {
    viewAngles = MM.ReadMem<Vec3>(client + dwViewAngles);
    return viewAngles;
}

Vec3 LocalPlayer::getViewAngles2() {
    return MM.ReadMem<Vec3>(pawn + v_angle);
}

Vec3 LocalPlayer::getCameraPos() {
    cameraPos = MM.ReadMem<Vec3>(pawn + m_vecLastClipCameraPos);
    return cameraPos;
}

Vec3 LocalPlayer::getPos() {
    pos = MM.ReadMem<Vec3>(pawn + m_vLastSlopeCheckPos);
    return pos;
}

float LocalPlayer::getSens() {
    uintptr_t dwSens = MM.ReadMem<uintptr_t>(client + dwSensitivity);
    sens = MM.ReadMem<float>(dwSens + dwSensitivity_sensitivity);
    return sens;
}

float LocalPlayer::getFOVSentitivityAdjust() {
    FOVSAdjust = MM.ReadMem<float>(pawn + m_flFOVSensitivityAdjust);
    return FOVSAdjust;
}

bool LocalPlayer::getIsScoped() {
    return MM.ReadMem<bool>(pawn + m_bIsScoped);
}

UTILVEC LocalPlayer::getAimPunchCache() {
    aimPunchCache = MM.ReadMem<UTILVEC>(pawn + m_aimPunchCache);
    return aimPunchCache;
}

Vec2 LocalPlayer::getAimPunchAngle() {
    aimPunchAngle = MM.ReadMem<Vec2>(aimPunchCache.data + (aimPunchCache.count -1)*sizeof(Vec3));
    return aimPunchAngle;
}

int LocalPlayer::getShotsFired() {
    int shotsFired = MM.ReadMem<int>(pawn + m_iShotsFired);
    return shotsFired;
}

int LocalPlayer::getCrosshairEntity() {
    return MM.ReadMem<int>(pawn + m_iIDEntIndex);
}

void Entity::init(int32_t index, uintptr_t list) {
    id = index;
    entityList = list;
}

uintptr_t Entity::getEntry(int32_t index) {
    uintptr_t entry = MM.ReadMem<uintptr_t>(entityList + 0x10);
    return entry;
}
//0x8 * ((index & 0x7FFF) >> 9)
uintptr_t Entity::getEntry(uint32_t index) {
    uintptr_t entry = MM.ReadMem<uintptr_t>(entityList + 0x10);
    return entry;
}

uintptr_t Entity::getController() {
    uintptr_t entry = getEntry(id);
    controller = MM.ReadMem<uintptr_t>(entry + 0x78 * id);
    return controller;
}

uintptr_t Entity::getPawn() {
    idx = MM.ReadMem<uint32_t>(controller + m_hPlayerPawn);
    uintptr_t entry = getEntry(idx);
    pawn = MM.ReadMem<uintptr_t>(entry + 0x78 * (idx & 0x1FF));
    return pawn;
}

uintptr_t Entity::getPawnByID(int id) {
    idx = id;
    uintptr_t entry = getEntry(idx);
    pawn = MM.ReadMem<uintptr_t>(entry + 0x78 * (idx & 0x1FF));
    return pawn;
}

int32_t Entity::getHealth() {
    health = MM.ReadMem<int>(pawn + m_iHealth);
    return health;
}

int32_t Entity::getHealth2() {
    return MM.ReadMem<int>(controller + m_iPawnHealth);
}

uintptr_t Entity::getTeam() {
    team = MM.ReadMem<uintptr_t>(controller + m_iTeamNum);
    return team;
}

uintptr_t Entity::getGameSceneNode() {
    GameSceneNode = MM.ReadMem<uintptr_t>(pawn + m_pGameSceneNode);
    return GameSceneNode;
}

uintptr_t Entity::getBoneArray() {
    boneArray = MM.ReadMem<uintptr_t>(GameSceneNode + m_modelState + 0x80);
    return boneArray;
}

Vec3 Entity::getOrigin() {
    origin = MM.ReadMem<Vec3>(pawn + m_vOldOrigin);
    return origin;
}

std::string Entity::getName() {
    uintptr_t nameaddr = MM.ReadMem<uintptr_t>(controller + m_sSanitizedPlayerName);
    if (nameaddr) {
        char buf[MAX_PATH]{};
        MM.ReadBytes(nameaddr, MAX_PATH, buf);
        name = std::string(buf);
    }
    else {
        name = "NoName";
    }
    return name;
}

Vec3 Entity::getBonePos(int bone) {
    getGameSceneNode();
    getBoneArray();
    return MM.ReadMem<Vec3>(boneArray + bone * 32);
}

bool Game::isWarmUp() {
    return MM.ReadMem<bool>(rules + m_bWarmupPeriod);
}

bool Game::isMatch() {
    return MM.ReadMem<bool>(rules + m_bHasMatchStarted);
}

bool Game::isFreezeTime() {
    return MM.ReadMem<bool>(rules + m_bFreezePeriod);
}