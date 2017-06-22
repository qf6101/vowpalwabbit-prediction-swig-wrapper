//
// Created by qfeng on 17-6-19.
//

#ifndef PCTR_PCTR_LOG_H
#define PCTR_PCTR_LOG_H

#include <string>
#include <spdlog/spdlog.h>

class pctr_log {
public:
    static std::string _core_logger_name;
    static std::string _core_logger_file_name;
    static bool create_loggers(const std::string& core_logger_name, const std::string& core_logger_file_name);
    static bool release_loggers();
};

#endif //PCTR_PCTR_LOG_H
