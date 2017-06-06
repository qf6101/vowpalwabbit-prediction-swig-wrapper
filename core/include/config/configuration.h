//
// Created by qfeng on 17-5-31.
//

#ifndef PCTR_CONFIGURATION_H
#define PCTR_CONFIGURATION_H

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>

static void init_log() {

    static const std::string COMMON_FMT("[%LineID%][%TimeStamp%][%Severity%]:  %Message%");

    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

    boost::log::add_console_log(
            std::cout,
            boost::log::keywords::format = COMMON_FMT,
            boost::log::keywords::auto_flush = true
    );

    boost::log::add_file_log(
            boost::log::keywords::target = "pctr_core_logs",
            boost::log::keywords::file_name = "pctr_core_%3N.log",
            boost::log::keywords::open_mode = std::ios_base::app | std::ios_base::out,
            boost::log::keywords::rotation_size = 200 * 1024 * 1024,
            boost::log::keywords::max_files = 5,
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
            boost::log::keywords::format = COMMON_FMT,
            boost::log::keywords::auto_flush = true
    );

    boost::log::add_common_attributes();

#ifdef _DEBUG
    boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::debug
    );
#else
    boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::info
    );
#endif

}

#endif //PCTR_CONFIGURATION_H
