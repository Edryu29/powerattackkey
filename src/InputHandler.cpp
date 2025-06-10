#include "InputHandler.h"
#include "Settings.h"

InputEventHandler* InputEventHandler::GetSingleton()
{
    static InputEventHandler instance;
    return &instance;
}

void InputEventHandler::PerformAction(RE::BGSAction* action, RE::Actor* player) {
	if (action && player) {
        std::unique_ptr<RE::TESActionData> data(RE::TESActionData::Create());
        data->source = RE::NiPointer<RE::TESObjectREFR>(player);
        data->action = action;
        typedef bool func_t(RE::TESActionData*);
        REL::Relocation<func_t> func{ RELOCATION_ID(40551, 41557) };
        if (func(data.get())) {
            logger::info("Action performed successfully");
        }
        else {
            logger::info("Action failed");
        }
	}
    else {
        logger::error("Action or player is null");
    }
}

RE::BSEventNotifyControl InputEventHandler::ProcessEvent(
    RE::InputEvent* const* a_event,
    RE::BSTEventSource<RE::InputEvent*>*)
{
    if (!a_event) {
        return RE::BSEventNotifyControl::kContinue;
    }

    if (const auto ui{ RE::UI::GetSingleton() }) {

        if (ui->GameIsPaused() || ui->IsApplicationMenuOpen() || ui->IsItemMenuOpen() || ui->IsModalMenuOpen()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (const auto control_map{ RE::ControlMap::GetSingleton() }; control_map->IsMovementControlsEnabled()) {
            const auto player = RE::PlayerCharacter::GetSingleton();
            if (player && player->Is3DLoaded()) {
                for (auto e{ *a_event }; e != nullptr; e = e->next) {
                    if (const auto btn_event{ e->AsButtonEvent() }) {
                        if (!btn_event->IsDown()) {
                            return RE::BSEventNotifyControl::kContinue;
                        }
                        const auto device{ btn_event->GetDevice() };
                        auto       keycode{ btn_event->GetIDCode() };

                        using enum RE::INPUT_DEVICE;
                        if (device != kKeyboard && device != kGamepad) {
                            return RE::BSEventNotifyControl::kContinue;
                        }
                        if (device == kGamepad) {
                            keycode = SKSE::InputMap::GamepadMaskToKeycode(keycode);
                        }
                        // logger::info("Key Pressed: {}", keycode);

                        if (keycode == Settings::rightHandKey && (Settings::comboKey<=-1 || comboActive)) {
                            logger::info("Right Hand Key Pressed");
                            PerformAction(rightHandAction, player);
                        } else if (keycode == Settings::leftHandKey && (Settings::comboKey<=-1 || comboActive)) {
                            logger::info("Left Hand Key Pressed");
                            PerformAction(leftHandAction, player);
                        } else if (keycode == Settings::bothHandsKey && (Settings::comboKey<=-1 || comboActive)) {
                            logger::info("Both Hands Key Pressed");
                            PerformAction(bothHandsAction, player);
                        }
                    }
                }
            }
        }
    }

    // if (event->GetEventType() == RE::INPUT_EVENT_TYPE::kButton){
        
    //     auto* buttonEvent = event->AsButtonEvent();
    //     auto dxScanCode = buttonEvent->GetIDCode();

    //     if (buttonEvent->device.get() == RE::INPUT_DEVICE::kMouse) {
    //         dxScanCode += 256;
    //     } else if (buttonEvent->device.get() == RE::INPUT_DEVICE::kGamepad) {
    //         dxScanCode = GamepadKeycode(dxScanCode);
    //     }

    //     if (buttonEvent && (buttonEvent->IsDown() || (holdContinuousPA && buttonEvent->IsHeld())) &&
    //         (dxScanCode == rightHandKey || dxScanCode == leftHandKey || dxScanCode == bothHandsKey)) {

    //         const auto gameUI = RE::UI::GetSingleton();
    //         const auto controlMap = RE::ControlMap::GetSingleton();
    //         const auto playerCharacter = RE::PlayerCharacter::GetSingleton();
    //         logger::info("Keycode... {}", dxScanCode);

    //         bool bAllowRotation = false;
    //         if (waitPowerAttack) {
    //             playerCharacter->GetGraphVariableBool("bAllowRotation", bAllowRotation);
    //             logger::info("Is powerattacking... {}", bAllowRotation);
    //         }

    //         if (gameUI && !gameUI->GameIsPaused() && controlMap && controlMap->IsFightingControlsEnabled() &&
    //             !bAllowRotation && playerCharacter) {
    //             const auto playerState = playerCharacter->AsActorState();

    //             if (playerState && !playerCharacter->IsInKillMove() &&
    //                 playerState->GetWeaponState() == RE::WEAPON_STATE::kDrawn &&
    //                 playerState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
    //                 playerState->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal &&
    //                 playerState->GetFlyState() == RE::FLY_STATE::kNone) {

    //                 if (dxScanCode == rightHandKey && (comboActive || comboKey<=-1 )) {
    //                     RunConsoleCommand(rightHand);
    //                 } else if (dxScanCode == leftHandKey && (comboActive || comboKey <= -1)) {
    //                     RunConsoleCommand(leftHand);
    //                 } else if (dxScanCode == bothHandsKey && (comboActive || comboKey <= -1)) {
    //                     RunConsoleCommand(bothHands);
    //                 }
    //             }
    //         }
    //     }
    //     if (dxScanCode == comboKey) {
    //         if (buttonEvent->IsHeld()) comboActive = true;
    //         if (buttonEvent->IsUp()) comboActive = false;
    //     }
    // }

    return RE::BSEventNotifyControl::kContinue;
}