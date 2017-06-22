//
// Created by qfeng on 17-6-19.
//

#include "log/pctr_log.h"
#include <iostream>

std::string pctr_log::_core_logger_name = "pctr_core_logger";
std::string pctr_log::_core_logger_file_name = "pctr_logs/core/core_log";

bool pctr_log::create_loggers(const std::string& core_logger_name, const std::string& core_logger_file_name) {
    try {
        _core_logger_name = core_logger_name;
        _core_logger_file_name = core_logger_file_name;

        spdlog::set_async_mode(131072, spdlog::async_overflow_policy::discard_log_msg); // 13m buffer

        std::vector<spdlog::sink_ptr> sinks;
#ifdef _DEBUG
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
#endif
        sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt> //128m per file
                                (_core_logger_file_name, 1048576 * 128, 10));

        auto logger = std::make_shared<spdlog::logger>(_core_logger_name, begin(sinks), end(sinks));
        logger->set_level(spdlog::level::info);
        spdlog::register_logger(logger);

        logger->info("Successfully initialized spdlog logger.\n");
    } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Spdlog initialization failed: " << ex.what() << std::endl;
    }
};

bool pctr_log::release_loggers() {
    spdlog::drop_all();
}
