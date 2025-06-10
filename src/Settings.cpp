#include <Settings.h>


int Settings::rightHandKey = -1;
int Settings::leftHandKey = -1;
int Settings::bothHandsKey = -1;
int Settings::comboKey = -1;

bool Settings::waitPowerAttack = false;
bool Settings::holdContinuousPA = false;

void Settings::LoadSettings()
{
    constexpr auto path = L"Data/SKSE/Plugins/PowerAttackKey.ini";

    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(path);

    rightHandKey = std::stoi(ini.GetValue("Settings", "Right Hand", "-1"));
    leftHandKey = std::stoi(ini.GetValue("Settings", "Left Hand", "-1"));
    bothHandsKey = std::stoi(ini.GetValue("Settings", "Dual Wield", "-1"));
    comboKey = std::stoi(ini.GetValue("Settings", "Combo Key", "-1"));

    waitPowerAttack = std::stoi(ini.GetValue("Settings", "Wait Power Attack", "0"));
    holdContinuousPA = std::stoi(ini.GetValue("Settings", "Hold Continuous Power Attack", "0"));

    (void)ini.SaveFile(path);
}