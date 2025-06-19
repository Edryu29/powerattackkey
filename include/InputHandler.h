#pragma once

class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*>
{
    public:
        static InputEventHandler* GetSingleton();
        RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event,RE::BSTEventSource<RE::InputEvent*>*) override;
        void GetAttackKeys();

    private:
        bool IsRightHandKey(const RE::INPUT_DEVICE device, const std::uint32_t key) const;
        bool IsLeftHandKey(const RE::INPUT_DEVICE device, const std::uint32_t key) const;
        bool PerformRightHandPA(RE::PlayerCharacter* player);
        bool PerformLeftHandPA(RE::PlayerCharacter* player);
        bool PerformBothHandsPA(RE::PlayerCharacter* player);

        static void PerformAction(RE::BGSAction* action, RE::Actor* a);
        static bool HasEquipedWeapon(const RE::PlayerCharacter* player, bool leftHand);
        static bool IsHandUnarmed(const RE::PlayerCharacter* player, bool leftHand);
        static bool HasEquippedTwoHandedWeapon(const RE::PlayerCharacter* player);
        static bool HasEnoughStamina(RE::PlayerCharacter* player, bool rightHand, bool leftHand);
        static void FlashHUDMeter(RE::ActorValue a_av);

        RE::BGSAction* PARightHandAction = RE::TESForm::LookupByID(0x13383)->As<RE::BGSAction>();
        RE::BGSAction* PALeftHandAction = RE::TESForm::LookupByID(0x2E2F6)->As<RE::BGSAction>();
        RE::BGSAction* PABothHandsAction = RE::TESForm::LookupByID(0x2E2F7)->As<RE::BGSAction>();
        RE::BGSAction* LARightHandAction = RE::TESForm::LookupByID(0x13005)->As<RE::BGSAction>();
        RE::BGSAction* LALeftHandAction = RE::TESForm::LookupByID(0x13004)->As<RE::BGSAction>();
        RE::BGSAction* LABothHandsAction = RE::TESForm::LookupByID(0x50C96)->As<RE::BGSAction>();

        std::uint32_t rightAttackKeyKeyboard = 255;
        std::uint32_t rightAttackKeyMouse = 255;
        std::uint32_t rightAttackKeyGamepad = 255;

        std::uint32_t leftAttackKeyKeyboard = 255;
        std::uint32_t leftAttackKeyMouse = 255;
        std::uint32_t leftAttackKeyGamepad = 255;

        bool comboActive = false;
        bool comboActiveAlt1 = false;
        bool comboActiveAlt2 = false;

        bool rightHandKeyPressed = false;
        bool leftHandKeyPressed = false;

        float rightAttackHeldTime = 0.0f;
        bool rightAttackWaiting = false;

        float leftAttackHeldTime = 0.0f;
        bool leftAttackWaiting = false;

        float powerAttackHeldTime = 0.0f;
        bool powerAttackWaiting = false;
        
};