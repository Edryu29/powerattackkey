#include <spdlog/sinks/basic_file_sink.h>
#include <SimpleIni.h>

namespace logger = SKSE::log;

std::string rightHand = "player.pa ActionRightPowerAttack";
std::string leftHand = "player.pa ActionLeftPowerAttack";
std::string bothHands = "player.pa ActionDualPowerAttack";

bool waitPowerAttack = false;
bool holdContinuousPA = false;

int rightHandKey = -1;
int leftHandKey = -1;
int bothHandsKey = -1;

int comboKey = -1;
bool comboActive = false;

void SetupLog() {
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
    spdlog::set_default_logger(std::move(loggerPtr));
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);
}

void LoadSettings() {
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

void RunConsoleCommand(std::string a_command) {
    const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
    const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
    if (script) {
        const auto selectedRef = RE::Console::GetSelectedRef();
        script->SetCommand(a_command);
        script->CompileAndRun(selectedRef.get());
        delete script;
    }
}


uint32_t GamepadKeycode(uint32_t dxScanCode) { 
    int dxGamepadKeycode = -1;
    RE::BSWin32GamepadDevice::Key gamepadKey = static_cast<RE::BSWin32GamepadDevice::Key>(dxScanCode);
    switch (gamepadKey) {
        case RE::BSWin32GamepadDevice::Key::kUp:
            dxGamepadKeycode = 266;
            break;
        case RE::BSWin32GamepadDevice::Key::kDown:
            dxGamepadKeycode = 267;
            break;
        case RE::BSWin32GamepadDevice::Key::kLeft:
            dxGamepadKeycode = 268;
            break;
        case RE::BSWin32GamepadDevice::Key::kRight:
            dxGamepadKeycode = 269;
            break;
        case RE::BSWin32GamepadDevice::Key::kStart:
            dxGamepadKeycode = 270;
            break;
        case RE::BSWin32GamepadDevice::Key::kBack:
            dxGamepadKeycode = 271;
            break;
        case RE::BSWin32GamepadDevice::Key::kLeftThumb:
            dxGamepadKeycode = 272;
            break;
        case RE::BSWin32GamepadDevice::Key::kRightThumb:
            dxGamepadKeycode = 273;
            break;
        case RE::BSWin32GamepadDevice::Key::kLeftShoulder:
            dxGamepadKeycode = 274;
            break;
        case RE::BSWin32GamepadDevice::Key::kRightShoulder:
            dxGamepadKeycode = 275;
            break;
        case RE::BSWin32GamepadDevice::Key::kA:
            dxGamepadKeycode = 276;
            break;
        case RE::BSWin32GamepadDevice::Key::kB:
            dxGamepadKeycode = 277;
            break;
        case RE::BSWin32GamepadDevice::Key::kX:
            dxGamepadKeycode = 278;
            break;
        case RE::BSWin32GamepadDevice::Key::kY:
            dxGamepadKeycode = 279;
            break;
        case RE::BSWin32GamepadDevice::Key::kLeftTrigger:
            dxGamepadKeycode = 280;
            break;
        case RE::BSWin32GamepadDevice::Key::kRightTrigger:
            dxGamepadKeycode = 281;
            break;
        default:
            dxGamepadKeycode = static_cast<uint32_t>(-1);
            break;
    }
    return dxGamepadKeycode;
}

class InputEventSink : public RE::BSTEventSink<RE::InputEvent*> {
    InputEventSink() = default;
    InputEventSink(const InputEventSink&) = delete;
    InputEventSink(InputEventSink&&) = delete;
    InputEventSink& operator=(const InputEventSink&) = delete;
    InputEventSink& operator=(InputEventSink&&) = delete;

public:
    static InputEventSink* GetSingleton() {
        static InputEventSink singleton;
        return &singleton;
    }

    RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* eventPtr, RE::BSTEventSource<RE::InputEvent*>*) {
        if (!eventPtr) return RE::BSEventNotifyControl::kContinue;

        auto* event = *eventPtr;
        if (!event) return RE::BSEventNotifyControl::kContinue;

        if (event->GetEventType() == RE::INPUT_EVENT_TYPE::kButton){
            auto* buttonEvent = event->AsButtonEvent();
            auto dxScanCode = buttonEvent->GetIDCode();

            if (buttonEvent->device.get() == RE::INPUT_DEVICE::kMouse) {
                dxScanCode += 256;
            } else if (buttonEvent->device.get() == RE::INPUT_DEVICE::kGamepad) {
                dxScanCode = GamepadKeycode(dxScanCode);
            }

            if (buttonEvent && (buttonEvent->IsDown() || (holdContinuousPA && buttonEvent->IsHeld())) &&
                (dxScanCode == rightHandKey || dxScanCode == leftHandKey || dxScanCode == bothHandsKey)) {

                const auto gameUI = RE::UI::GetSingleton();
                const auto controlMap = RE::ControlMap::GetSingleton();
                const auto playerCharacter = RE::PlayerCharacter::GetSingleton();
                //logger::info("Keycode... {}", dxScanCode);

                bool bAllowRotation = false;
                if (waitPowerAttack) {
                    playerCharacter->GetGraphVariableBool("bAllowRotation", bAllowRotation);
                    // logger::info("Is powerattacking... {}", bAllowRotation);
                }

                if (gameUI && !gameUI->GameIsPaused() && controlMap && controlMap->IsFightingControlsEnabled() &&
                    !bAllowRotation && playerCharacter) {
                    const auto playerState = playerCharacter->AsActorState();

                    if (playerState && !playerCharacter->IsInKillMove() &&
                        playerState->GetWeaponState() == RE::WEAPON_STATE::kDrawn &&
                        playerState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
                        playerState->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal &&
                        playerState->GetFlyState() == RE::FLY_STATE::kNone) {

                        if (dxScanCode == rightHandKey && (comboActive || comboKey<=-1 )) {
                            RunConsoleCommand(rightHand);
                        } else if (dxScanCode == leftHandKey && (comboActive || comboKey <= -1)) {
                            RunConsoleCommand(leftHand);
                        } else if (dxScanCode == bothHandsKey && (comboActive || comboKey <= -1)) {
                            RunConsoleCommand(bothHands);
                        }
                    }
                }
            }
            if (dxScanCode == comboKey) {
                if (buttonEvent->IsHeld()) comboActive = true;
                if (buttonEvent->IsUp()) comboActive = false;
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
};

void OnMessage(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kInputLoaded)
        RE::BSInputDeviceManager::GetSingleton()->AddEventSink(InputEventSink::GetSingleton());
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);

    SetupLog();
    logger::info("Setup log...");

    LoadSettings();
    logger::info("Settings loaded...");

    SKSE::GetMessagingInterface()->RegisterListener(OnMessage);

    return true;
}