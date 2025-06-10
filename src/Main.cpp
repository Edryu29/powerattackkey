#include <InputHandler.h>

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

void MessageHandler(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kInputLoaded){}
        RE::BSInputDeviceManager::GetSingleton()->AddEventSink(InputEventHandler::GetSingleton());
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

void InitializeLog() {
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

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    InitializeLog();
	logger::info("Game version : {}", skse->RuntimeVersion().string());

    SKSE::Init(skse);

    LoadSettings();
    logger::info("Settings loaded...");

    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);

    return true;
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