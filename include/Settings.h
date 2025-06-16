#pragma once

namespace Settings
{
    extern int rightHandKey;
    extern int leftHandKey;
    extern int bothHandsKey;
    extern int comboKey;
    
    extern int rightHandKeyAlt1;
    extern int leftHandKeyAlt1;
    extern int bothHandsKeyAlt1;
    extern int comboKeyAlt1;

    extern int rightHandKeyAlt2;
    extern int leftHandKeyAlt2;
    extern int bothHandsKeyAlt2;
    extern int comboKeyAlt2;

    extern bool holdConsecutivePA;
    extern bool holdConsecutiveLA;
    extern float consecutiveAttacksDelay;
    extern bool consecutiveDualAttacks;
    
    extern bool requireStaminaPA;
    extern int staminaCost1H;
    extern int staminaCost2H;

    void LoadSettings();
}