#pragma once

namespace Settings
{
    extern int rightHandKey;
    extern int leftHandKey;
    extern int bothHandsKey;
    extern int comboKey;
    
    extern int rightHandKeyAlt;
    extern int leftHandKeyAlt;
    extern int bothHandsKeyAlt;
    extern int comboKeyAlt;

    extern bool waitPowerAttack;
    extern bool holdContinuousPA;
    extern bool holdContinuousLA;
    extern bool requireStaminaPA;
    extern int amountStaminaPA;

    void LoadSettings();
}