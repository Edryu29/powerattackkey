#include <InputHandler.h>
#include <Settings.h>

void MessageHandler(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kDataLoaded){
        RE::BSInputDeviceManager::GetSingleton()->AddEventSink(InputEventHandler::GetSingleton());
    }
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

    Settings::LoadSettings();
    logger::info("Settings loaded...");

    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);

    return true;
}