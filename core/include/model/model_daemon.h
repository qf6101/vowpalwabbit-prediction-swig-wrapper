//
// Created by qfeng on 17-5-27.
//

#ifndef PCTR_LINEAR_MODE_DAEMON_H
#define PCTR_LINEAR_MODE_DAEMON_H

#include "model.h"
#include <string>
#include <boost/thread/thread.hpp>
#include <Eigen/Sparse>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

using namespace boost::log::trivial;
using namespace std;

class model_daemon {
public:
    model* _model = NULL;
    string _model_path;
    string _model_type;
    uint32_t _update_interval;
    timespec _last_mod_time;
    boost::thread* _daemon_thread;
    boost::log::sources::severity_logger<severity_level> _logger;

    model_daemon(string model_path, string model_type, uint32_t update_interval = 30);

    virtual ~model_daemon();

    bool start();

    void stop();

    void update_model();

    bool is_model_file_modified();

    bool load_model(model** output_model);

    float logistic_predict(Eigen::SparseVector<float>& sample);
};


#endif //PCTR_LINEAR_MODE_DAEMON_H
