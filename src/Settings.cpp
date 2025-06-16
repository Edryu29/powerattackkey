#include <Settings.h>

int Settings::rightHandKey;
int Settings::leftHandKey;
int Settings::bothHandsKey;
int Settings::comboKey;

int Settings::rightHandKeyAlt1;
int Settings::leftHandKeyAlt1;
int Settings::bothHandsKeyAlt1;
int Settings::comboKeyAlt1;

int Settings::rightHandKeyAlt2;
int Settings::leftHandKeyAlt2;
int Settings::bothHandsKeyAlt2;
int Settings::comboKeyAlt2;

bool Settings::holdConsecutivePA;
bool Settings::holdConsecutiveLA;
float Settings::consecutiveAttacksDelay;
bool Settings::consecutiveDualAttacks;

bool Settings::requireStaminaPA;
int Settings::staminaCost1H;
int Settings::staminaCost2H;

void Settings::LoadSettings()
{
    constexpr auto path = L"Data/SKSE/Plugins/PowerAttackKey.ini";

    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(path);

    rightHandKey = std::stoi(ini.GetValue("Settings", "iRightHandKey", "45"));
    leftHandKey = std::stoi(ini.GetValue("Settings", "iLeftHandKey", "-1"));
    bothHandsKey = std::stoi(ini.GetValue("Settings", "iDualWieldKey", "-1"));
    comboKey = std::stoi(ini.GetValue("Settings", "iComboKey", "-1"));

    rightHandKeyAlt1 = std::stoi(ini.GetValue("Settings", "iRightHandKeyAlt1", "281"));
    leftHandKeyAlt1 = std::stoi(ini.GetValue("Settings", "iLeftHandKeyAlt1", "-1"));
    bothHandsKeyAlt1 = std::stoi(ini.GetValue("Settings", "iDualWieldKeyAlt1", "-1"));
    comboKeyAlt1 = std::stoi(ini.GetValue("Settings", "iComboKeyAlt1", "-1"));

    rightHandKeyAlt2 = std::stoi(ini.GetValue("Settings", "iRightHandKeyAlt2", "-1"));
    leftHandKeyAlt2 = std::stoi(ini.GetValue("Settings", "iLeftHandKeyAlt2", "-1"));
    bothHandsKeyAlt2 = std::stoi(ini.GetValue("Settings", "iDualWieldKeyAlt2", "-1"));
    comboKeyAlt2 = std::stoi(ini.GetValue("Settings", "iComboKeyAlt2", "-1"));

    holdConsecutivePA = std::stoi(ini.GetValue("Settings", "bConsecutivePowerAttacks", "0"));
    holdConsecutiveLA = std::stoi(ini.GetValue("Settings", "bConsecutiveLightAttacks", "0"));
    consecutiveAttacksDelay = std::stoi(ini.GetValue("Settings", "fConsecutiveAttacksDelay", "0.5"));
    consecutiveDualAttacks = std::stoi(ini.GetValue("Settings", "bConsecutiveDualAttacks", "0"));

    requireStaminaPA = std::stoi(ini.GetValue("Settings", "bPowerAttacksRequireStamina", "0"));
    staminaCost1H = std::stoi(ini.GetValue("Settings", "iStaminaCost1H", "15"));
    staminaCost2H = std::stoi(ini.GetValue("Settings", "iStaminaCost2H", "30"));

    (void)ini.SaveFile(path);
}