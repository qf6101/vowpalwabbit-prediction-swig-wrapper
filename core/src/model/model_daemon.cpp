//
// Created by qfeng on 17-5-27.
//

#include "model/model_daemon.h"
#include "config/configuration.h"
#include <sys/stat.h>
#include <boost/algorithm/string.hpp>
#include <limits>
#include <boost/log/expressions.hpp>
#include "model/linear_model.h"

model_daemon::model_daemon(string model_path, string model_type, uint32_t update_interval) {
    _model_path = model_path;
    _model_type = model_type;
    _update_interval = update_interval;
}

model_daemon::~model_daemon() {
    if (_model != NULL) delete _model;
}

bool model_daemon::start() {
    model *new_model = NULL;
    if (load_model(&new_model)) {
        _model = new_model;
        is_model_file_modified();
        boost::function0<void> f = boost::bind(&model_daemon::update_model, this);
        _daemon_thread = new boost::thread(f);
        return true;
    } else {
        BOOST_LOG_SEV(_logger, fatal) << "Fail to load model when initializing!";
        return false;
        float logistic_predict(Eigen::SparseVector<float> &sample);
    }
}

void model_daemon::stop() {
    if (_daemon_thread != NULL) {
        _daemon_thread->interrupt();
        _daemon_thread->join();
    }
}

void model_daemon::update_model() {
    bool continue_loop = true;
    while (continue_loop) {
        try {
            boost::this_thread::sleep(boost::posix_time::seconds(_update_interval));
            if (is_model_file_modified()) {
                model *new_model = NULL;
                if (load_model(&new_model)) {
                    model *old_model = _model;
                    _model = new_model;
                    delete old_model;
                    BOOST_LOG_SEV(_logger, info) << "Successfully updated the model!";
                }
            }
        } catch (boost::thread_interrupted &ex) {
            continue_loop = false;
            BOOST_LOG_SEV(_logger, info) << "Interrupt the update model thread!";
        } catch (exception &ex) {
            BOOST_LOG_SEV(_logger, error) << "Fail to update model: " << ex.what();
        }
    }
}

bool model_daemon::is_model_file_modified() {
    struct stat result;
    if (stat(_model_path.c_str(), &result) == 0) {
        timespec &mod_time = result.st_mtim;
        if (mod_time.tv_sec != _last_mod_time.tv_sec) {
            _last_mod_time = mod_time;
            return true;
        }
    }
    return false;
}

bool model_daemon::load_model(model **output_model) {
    if (_model_type == "linear_model") {
        linear_model *output_linear_model = NULL;
        bool load_succ = linear_model::load_model(_model_path, &output_linear_model);
        if (load_succ) {
            *output_model = output_linear_model;
            return true;
        } else if (output_linear_model != NULL) {
            delete output_linear_model;
            BOOST_LOG_SEV(_logger, error) << "Fail to load model for model daemon: "
                                          << _model_path
                                          << ", "
                                          << _model_type;
        }
    } else {
        BOOST_LOG_SEV(_logger, error) << "Wrong model type configured: " << _model_type;
    }
    return false;
}

float model_daemon::logistic_predict(Eigen::SparseVector<float> &sample) {
    if (_model != NULL) return _model->logistic_predict(sample);
    else return std::numeric_limits<float>::quiet_NaN();
}