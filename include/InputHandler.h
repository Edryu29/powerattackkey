#pragma once

class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*>
{
    public:
        static InputEventHandler* GetSingleton();
        static void PerformAction(RE::BGSAction* action, RE::Actor* a);
        RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override;
    
    private:
        RE::BGSAction* rightHandAction = RE::TESForm::LookupByID(0x13383)->As<RE::BGSAction>();
        RE::BGSAction* leftHandAction = RE::TESForm::LookupByID(0x2E2F6)->As<RE::BGSAction>();
        RE::BGSAction* bothHandsAction = RE::TESForm::LookupByID(0x2E2F7)->As<RE::BGSAction>();

        bool comboActive = false;
};