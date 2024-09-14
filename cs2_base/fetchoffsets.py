from bs4 import BeautifulSoup, GuessedAtParserWarning
import warnings
warnings.simplefilter(action='ignore', category=GuessedAtParserWarning)

import requests
import json

offsets_url = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/offsets.json"
client_url = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/client_dll.json"
#offsets_url = "https://raw.githubusercontent.com/ch4zx1/a2x-dumper-backup/main/output/offsets.json"
#client_url = "https://raw.githubusercontent.com/ch4zx1/a2x-dumper-backup/main/output/client_dll.json"


offsets = ["dwEntityList", "dwGameRules", "dwLocalPlayerController", "dwLocalPlayerPawn", "dwSensitivity", "dwSensitivity_sensitivity", "dwViewAngles", "dwViewMatrix"]
client = ["m_iHealth", "m_iTeamNum", "m_pGameSceneNode", "m_hPlayerPawn", "m_iPawnHealth", "v_angle", "m_vecLastClipCameraPos", "m_vLastSlopeCheckPos", "m_modelState"]
targets = ["dwEntityList", "dwGameRules", "dwLocalPlayerController", "dwLocalPlayerPawn",
           "dwSensitivity", "dwSensitivity_sensitivity", "dwViewAngles", "dwViewMatrix",
           "m_iHealth", "m_iTeamNum", "m_pGameSceneNode", "m_hPlayerPawn", "m_iPawnHealth",
            "v_angle", "m_vecLastClipCameraPos", "m_vLastSlopeCheckPos", "m_modelState",
            "m_vOldOrigin", "m_bWarmupPeriod", "m_bHasMatchStarted", "m_bFreezePeriod",
            "m_sSanitizedPlayerName", "m_flFOVSensitivityAdjust", "m_aimPunchCache",
            "m_aimPunchAngle", "m_iShotsFired", "m_bIsScoped"]

def get_tuple_list(url_offsets, url_client, list_offsets):
    response = requests.get(url_offsets)
    response2 = requests.get(url_client)
    soup1= BeautifulSoup(response.text, "html")
    soup2 = BeautifulSoup(response2.text, "html")
    if not response.ok or not response2.ok:
        return False
    d1 = json.loads(str(soup1.body.p.get_text()))["client.dll"]
    d2 = json.loads(str(soup2.body.p.get_text()))["client.dll"]["classes"]
    ret = list()
    temp = list(d1.items())
    for k in d2.keys():
        temp += list(d2[k]['fields'].items())
    for t in temp:
        if t[0] in list_offsets:
            ret.append(t)
    
    return ret

def write_defines(define_list):
    with open("offsets.h", mode="w+") as f:
        f.write("#pragma once\n")
        for name, value in define_list:
            f.write(f"#define {name} {value}\n")

def main():
    goodstuff = get_tuple_list(offsets_url, client_url, targets)
    if len(goodstuff) == len(targets):
        write_defines(goodstuff)
    else:
        print("Couldnt fetch, check repo")

if __name__ == "__main__":
    main()

