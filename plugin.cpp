#include <spdlog/sinks/basic_file_sink.h>
#include <SimpleIni.h>

namespace logger = SKSE::log;

std::string rightHand = "player.pa ActionRightPowerAttack";
std::string leftHand = "player.pa ActionLeftPowerAttack";
std::string bothHands = "player.pa ActionDualPowerAttack";

bool waitPowerAttack = false;

int rightHandKey = -1;
int leftHandKey = -1;
int bothHandsKey = -1;

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
    waitPowerAttack = std::stoi(ini.GetValue("Settings", "Wait Power Attack", "0"));

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
            if (dxScanCode == rightHandKey || dxScanCode == leftHandKey || dxScanCode == bothHandsKey) {
                const auto gameUI = RE::UI::GetSingleton();
                if (gameUI && !gameUI->GameIsPaused()) {
                    const auto controlMap = RE::ControlMap::GetSingleton();
                    const auto playerCharacter = RE::PlayerCharacter::GetSingleton();

                    bool bAllowRotation = false;
                    if (waitPowerAttack) {
                        playerCharacter->GetGraphVariableBool("bAllowRotation", bAllowRotation);
                        // logger::info("Is powerattacking... {}", bAllowRotation);
                    }

                    if (controlMap && !bAllowRotation && playerCharacter && controlMap->IsFightingControlsEnabled()) {
                        const auto playerState = playerCharacter->AsActorState();

                        if (playerState && !playerCharacter->IsInKillMove() &&
                            playerState->GetWeaponState() == RE::WEAPON_STATE::kDrawn &&
                            playerState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
                            playerState->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal &&
                            playerState->GetFlyState() == RE::FLY_STATE::kNone) {

                            if (dxScanCode == rightHandKey) {
                                // playerCharacter->NotifyAnimationGraph("attackPowerStartInPlace");
                                RunConsoleCommand(rightHand);
                            } else if (dxScanCode == leftHandKey) {
                                RunConsoleCommand(leftHand);
                            } else if (dxScanCode == bothHandsKey) {
                                RunConsoleCommand(bothHands);
                            }
                        }
                    }
                }
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