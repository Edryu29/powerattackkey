#include "InputHandler.h"
#include "Settings.h"

InputEventHandler* InputEventHandler::GetSingleton()
{
    static InputEventHandler instance;
    return &instance;
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
 
                        const auto device{ btn_event->GetDevice() };
                        auto keycode{ btn_event->GetIDCode() };

                        using enum RE::INPUT_DEVICE;
                        if (device != kKeyboard && device != kGamepad) {
                            return RE::BSEventNotifyControl::kContinue;
                        }
                        if (device == kGamepad) {
                            keycode = SKSE::InputMap::GamepadMaskToKeycode(keycode);
                        }
                        // logger::info("Key Pressed: {}", keycode);

                        if (Settings::comboKey>0 && keycode == Settings::comboKey) {
                            if (btn_event->IsHeld()) comboActive = true;
                            if (btn_event->IsUp()) comboActive = false;
                            // logger::info("Combo Key Pressed");
                        }

                        if (!(btn_event->IsDown() || (btn_event->IsHeld() && Settings::holdContinuousPA))) {
                            return RE::BSEventNotifyControl::kContinue;
                        }

                        if (keycode == Settings::rightHandKey || keycode == Settings::leftHandKey || keycode == Settings::bothHandsKey) {
                            bool bAllowRotation = false;
                            if (Settings::waitPowerAttack) {
                                player->GetGraphVariableBool("bAllowRotation", bAllowRotation);
                                if (bAllowRotation) {
                                    // logger::info("Player is powerattacking");
                                    return RE::BSEventNotifyControl::kContinue;
                                }
                            }

                            const auto playerState = player->AsActorState();
                            if (!(!player->IsInKillMove() ||  playerState->GetWeaponState() == RE::WEAPON_STATE::kDrawn ||
                                playerState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal ||
                                playerState->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal ||
                                playerState->GetFlyState() == RE::FLY_STATE::kNone)){
                                // logger::info("Player cannot attack currently, ignoring input");
                                return RE::BSEventNotifyControl::kContinue;
                            }

                            if (keycode == Settings::rightHandKey && (Settings::comboKey<=0 || comboActive)) {
                                // logger::info("Right Hand Key Pressed");
                                PerformAction(rightHandAction, player);
                            } else if (keycode == Settings::leftHandKey && (Settings::comboKey<=0 || comboActive)) {
                                // logger::info("Left Hand Key Pressed");
                                PerformAction(leftHandAction, player);
                            } else if (keycode == Settings::bothHandsKey && (Settings::comboKey<=0 || comboActive)) {
                                // logger::info("Both Hands Key Pressed");
                                PerformAction(bothHandsAction, player);
                            }
                        }
                    }
                }
            }
        }
    }

    return RE::BSEventNotifyControl::kContinue;
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