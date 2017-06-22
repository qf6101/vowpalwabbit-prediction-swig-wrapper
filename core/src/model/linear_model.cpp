//
// Created by qfeng on 17-5-25.
//

#include "model/linear_model.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <log/pctr_log.h>

linear_model::linear_model(uint64_t weight_size) {
    _weights.resize(weight_size);
    _logger = spdlog::get(pctr_log::_core_logger_name);
    _logger->info("Initialize linear model with weight_size={}.", weight_size);
}

linear_model::~linear_model() {
    _weights.setZero();
    _logger->info("Leave linear model.");
}

uint64_t linear_model::weight_size() {
    return _weights.size();
}

float linear_model::get(uint64_t index) {
    return _weights.coeff(index);
}

void linear_model::update_weight(uint64_t index, float value) {
    if (value != 0) _weights.coeffRef(index) = value;
}

float linear_model::margin(const Eigen::SparseVector<float> &sample) {
    return _weights.dot(sample);
}

float linear_model::logistic_predict(const Eigen::SparseVector<float> &sample) {
    return 1.0 / (1.0 + exp(-margin(sample)));
}

shared_ptr<linear_model> linear_model::load_model(const string &model_path) {
    auto logger = spdlog::get(pctr_log::_core_logger_name);
    try {
        ifstream model_file;
        model_file.open(model_path);
        if (model_file.is_open()) {
            shared_ptr<linear_model> output_model;
            string line;
            bool start_reading_weights = false;
            while (!model_file.eof()) {
                getline(model_file, line);
                if (start_reading_weights && line.length() > 0) {
                    vector<string> splits;
                    boost::algorithm::split(splits, line, boost::algorithm::is_any_of(":"));
                    uint64_t index = boost::lexical_cast<uint64_t>(splits[0]);
                    float value = boost::lexical_cast<float>(splits[1]);
                    output_model->update_weight(index, value);
                } else if (boost::algorithm::starts_with(line, "bits")) {
                    vector<string> splits;
                    boost::algorithm::split(splits, line, boost::algorithm::is_any_of(":"));
                    uint64_t bits = boost::lexical_cast<uint64_t>(splits[1]);
                    output_model = make_shared<linear_model>(1 << bits);
                } else if (boost::algorithm::starts_with(line, ":")) {
                    float bias = boost::lexical_cast<float>(line.substr(1));
                    output_model->update_weight(0, bias);
                    start_reading_weights = true;
                }
            }
            if (model_file.is_open()) model_file.close();
            if (output_model) {
                logger->info("Natively loaded linear model from {}.", model_path);
                return output_model;
            } else logger->error("Fail to read model file when natively loading linear model from {}.", model_path);
        } else logger->error("Fail to open model file when natively loading linear model from {}.", model_path);
    } catch (exception &ex) {
        logger->error("Fail to natively load linear model from {}, detail: {}.", model_path, ex.what());
    }
    return nullptr;
}