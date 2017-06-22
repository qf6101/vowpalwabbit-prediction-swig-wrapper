//
// Created by qfeng on 17-5-27.
//

#ifndef PCTR_LINEAR_MODE_DAEMON_H
#define PCTR_LINEAR_MODE_DAEMON_H

#include "model.h"
#include "word2vec_model.h"
#include <string>
#include <boost/thread/thread.hpp>
#include <Eigen/Sparse>
#include <feature/feature_hash.h>
#include <spdlog/logger.h>

using namespace std;

class model_daemon {
public:
    // spdlog logger
    shared_ptr<spdlog::logger> _logger;
    // predict model
    shared_ptr<model> _model;
    string _model_path;
    string _model_type;
    // feature hash
    unique_ptr<feature_hash> _hash;
    // word2vec model
    unique_ptr<word2vec_model> _w2v_model;
    string _w2v_model_path;
    // daemon thread
    unique_ptr<boost::thread> _daemon_thread;
    // configuration
    uint32_t _update_interval;
    timespec _last_mod_time;

    model_daemon(const string& model_path, const string& model_type, const string& w2v_model_path,
                 uint32_t update_interval);

    virtual ~model_daemon();

    bool start();

    void stop();

    void update_model();

    bool is_model_file_modified();

    shared_ptr<model> load_model();

    float logistic_predict(const Eigen::SparseVector<float>& sample);
};


#endif //PCTR_LINEAR_MODE_DAEMON_H
