#pragma once

class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*>
{
    public:
        static InputEventHandler* GetSingleton();
        RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event,RE::BSTEventSource<RE::InputEvent*>*) override;
        static void PerformAction(RE::BGSAction* action, RE::Actor* a);
        void GetAttackKeys();

    private:
        RE::BGSAction* PARightHandAction = RE::TESForm::LookupByID(0x13383)->As<RE::BGSAction>();
        RE::BGSAction* PALeftHandAction = RE::TESForm::LookupByID(0x2E2F6)->As<RE::BGSAction>();
        RE::BGSAction* PABothHandsAction = RE::TESForm::LookupByID(0x2E2F7)->As<RE::BGSAction>();
        RE::BGSAction* LARightHandAction = RE::TESForm::LookupByID(0x13005)->As<RE::BGSAction>();
        RE::BGSAction* LALeftHandAction = RE::TESForm::LookupByID(0x13005)->As<RE::BGSAction>();
        RE::BGSAction* LABothHandAction = RE::TESForm::LookupByID(0x50C96)->As<RE::BGSAction>();

        bool comboActive = false;
        bool comboActiveAlt = false;

        bool IsRightHandKey(RE::INPUT_DEVICE device, std::uint32_t key) const;
        bool IsLeftHandKey(RE::INPUT_DEVICE device, std::uint32_t key) const;

        float vanillaPADelay = 0.3000000119f;
        float RightHandLAHeldTime = 0.0f;
        bool RightHandLAWait = false;
        float LeftHandLAHeldTime = 0.0f;
        bool LeftHandLAWait = false;
        float BothHandsLAHeldTime = 0.0f;
        bool BothHandsLAWait = false;


        std::uint32_t rightHandKeyKeyboard = 255;
        std::uint32_t rightHandKeyMouse = 255;
        std::uint32_t rightHandKeyGamepad = 255;

        std::uint32_t leftHandKeyKeyboard = 255;
        std::uint32_t leftHandKeyMouse = 255;
        std::uint32_t leftHandKeyGamepad = 255;
};