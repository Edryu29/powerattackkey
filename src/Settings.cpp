#include <Settings.h>

int Settings::rightHandKey;
int Settings::leftHandKey;
int Settings::bothHandsKey;
int Settings::comboKey;

int Settings::rightHandKeyAlt;
int Settings::leftHandKeyAlt;
int Settings::bothHandsKeyAlt;
int Settings::comboKeyAlt;

bool Settings::waitPowerAttack;
bool Settings::holdContinuousPA;
bool Settings::holdContinuousLA;
bool Settings::requireStaminaPA;
int Settings::amountStaminaPA;

void Settings::LoadSettings()
{
    constexpr auto path = L"Data/SKSE/Plugins/PowerAttackKey.ini";

    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(path);

    rightHandKey = std::stoi(ini.GetValue("Settings", "Right Hand", "45"));
    leftHandKey = std::stoi(ini.GetValue("Settings", "Left Hand", "-1"));
    bothHandsKey = std::stoi(ini.GetValue("Settings", "Dual Wield", "-1"));
    comboKey = std::stoi(ini.GetValue("Settings", "Combo Key", "-1"));

    rightHandKeyAlt = std::stoi(ini.GetValue("Settings", "Right Hand Alt", "281"));
    leftHandKeyAlt = std::stoi(ini.GetValue("Settings", "Left Hand Alt", "-1"));
    bothHandsKeyAlt = std::stoi(ini.GetValue("Settings", "Dual Wield Alt", "-1"));
    comboKeyAlt = std::stoi(ini.GetValue("Settings", "Combo Key Alt", "-1"));

    waitPowerAttack = std::stoi(ini.GetValue("Settings", "Wait Power Attack", "1"));
    holdContinuousPA = std::stoi(ini.GetValue("Settings", "Hold Continuous Power Attack", "0"));
    holdContinuousLA = std::stoi(ini.GetValue("Settings", "Hold Continuous Light Attacks", "0"));
    requireStaminaPA = std::stoi(ini.GetValue("Settings", "Power Attack Require Stamina", "0"));
    amountStaminaPA = std::stoi(ini.GetValue("Settings", "Power Attack Amount Stamina", "-1"));

    (void)ini.SaveFile(path);
}