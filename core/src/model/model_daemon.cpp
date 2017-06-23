//
// Created by qfeng on 17-5-27.
//

#include "model/model_daemon.h"
#include <sys/stat.h>
#include <boost/algorithm/string.hpp>
#include <log/pctr_log.h>
#include <boost/filesystem/operations.hpp>
#include "model/linear_model.h"

model_daemon::model_daemon(const string &model_path, const string &model_type, const string &w2v_model_path,
                           uint32_t update_interval) {
    _model_path = model_path;
    _model_type = model_type;
    _w2v_model_path = w2v_model_path;
    _update_interval = update_interval;
    _logger = spdlog::get(pctr_log::_core_logger_name);
    _logger->info(
            "Initialize model daemon with model path={}, model type={}, w2v model path={} and update interval={}.",
            model_path, model_type, w2v_model_path, update_interval);
}

model_daemon::~model_daemon() {
    _logger->info("Leave model daemon.");
}

bool model_daemon::start() {
    if (auto new_model = load_model()) {
        // load model
        _model.swap(new_model);
        new_model.reset();
        is_model_file_modified();
        _logger->info("Finish loading core model for model daemon.");
        // initialize feature hash
        _hash.reset(new feature_hash(_model->weight_size()));
        _logger->info("Finish initializing feature hash for model daemon.");
        // load word2vec model
        if (auto w2v_model = word2vec_model::load_model(_w2v_model_path)) {
            _w2v_model = std::move(w2v_model);
            _logger->info("Finish loading word2vec model for model daemon.");
        } else {
            _model.reset();
            _hash.release();
            _logger->info("Fail to start model daemon due to word2vec model.");
            return false;
        }
        // start daemon thread
        boost::function0<void> f = boost::bind(&model_daemon::update_model, this);
        _daemon_thread.reset(new boost::thread(f));
        _logger->info("Finish initializing daemon thread for model daemon.");
        _logger->info("Successfully start model daemon.");
        return true;
    } else {
        _logger->error("Fail to start model daemon due to core model.");
        return false;
    }
}

void model_daemon::stop() {
    if (_daemon_thread) {
        _daemon_thread->interrupt();
        _daemon_thread->join();
        _logger->info("Finish stop daemon thread for model daemon.");
    }
    _logger->info("Successfully stop model daemon.");
}

void model_daemon::update_model() {
    bool continue_loop = true;
    while (continue_loop) {
        try {
            boost::this_thread::sleep(boost::posix_time::seconds(_update_interval));
            if (is_model_file_modified()) {
                if (auto new_model = load_model()) {
                    // replace model
                    _model.swap(new_model);
                    new_model.reset();
                    _logger->info("Finish resetting core model for model daemon.");
                    // resize feature hash
                    if (_hash->size() != _model->weight_size()) {
                        auto old_size = _hash->size();
                        _hash->resize(_model->weight_size());
                        _logger->info("Finish resetting feature size from {} to {}", old_size, _model->weight_size());
                    }
                    _logger->info("Successfully update model for model daemon.");
                }
            }
        } catch (boost::thread_interrupted &ex) {
            continue_loop = false;
            _logger->info("Interrupt the update model thread for model daemon.");
        } catch (exception &ex) {
            _logger->error("Fail to update model for model daemon.");
        }
    }
}

bool model_daemon::is_model_file_modified() {
    if(!boost::filesystem::exists(_model_path)) return false;

    struct stat result;
    if (stat(_model_path.c_str(), &result) == 0) {
        timespec &mod_time = result.st_mtim;
        if (mod_time.tv_sec != _last_mod_time.tv_sec) {
            auto old_time = _last_mod_time;
            _last_mod_time = mod_time;
            _logger->info("Detected model file was modified from {} to {} in model daemon.",
                          old_time.tv_sec, mod_time.tv_sec);
            return true;
        }
        _logger->debug("Detected model file unchanged. Current model file was modified at {}.", _last_mod_time.tv_sec);
    }
    return false;
}

shared_ptr<model> model_daemon::load_model() {
    if (_model_type == "linear_model") {
        auto output_linear_model = linear_model::load_model(_model_path);
        _logger->info("Factory loaded linear model for model daemon.");
        return output_linear_model;
    } else {
        _logger->info("Wrong model type configured: {} for model daemon.", _model_type);
        return nullptr;
    }
}

float model_daemon::logistic_predict(const Eigen::SparseVector<float>& sample) {
    if (_model) return _model->logistic_predict(sample);
    else return 0;
}