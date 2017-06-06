//
// Created by qfeng on 17-5-25.
//

#include "model/linear_model.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>

linear_model::linear_model(uint64_t weight_size) {
    _weights.resize(weight_size);
}

linear_model::~linear_model() {
    _weights.setZero();
}

uint64_t linear_model::weight_size() {
    return _weights.size();
}

float linear_model::get(uint64_t index) {
    return _weights.coeff(index);
}

void linear_model::update_weight(uint64_t index, float value) {
    if(value != 0) _weights.coeffRef(index) = value;
}

float linear_model::margin(Eigen::SparseVector<float>& sample) {
    return _weights.dot(sample);
}

float linear_model::logistic_predict(Eigen::SparseVector<float>& sample) {
    return 1.0 / (1.0 + exp(-margin(sample)));
}

bool linear_model::load_model(string model_path, linear_model **output_model) {
    try {
        ifstream model_file;
        model_file.open(model_path);
        if (!model_file.is_open()) return false;
        string line;
        bool start_reading_weights = false;
        while (!model_file.eof()) {
            getline(model_file, line);
            if (start_reading_weights && line.length() > 0) {
                vector<string> splits;
                boost::algorithm::split(splits, line, boost::algorithm::is_any_of(":"));
                uint64_t index = boost::lexical_cast<uint64_t>(splits[0]);
                float value = boost::lexical_cast<float>(splits[1]);
                (*output_model)->update_weight(index, value);
            } else if (boost::algorithm::starts_with(line, "bits")) {
                vector<string> splits;
                boost::algorithm::split(splits, line, boost::algorithm::is_any_of(":"));
                uint64_t bits = boost::lexical_cast<uint64_t>(splits[1]);
                *output_model = new linear_model(1 << bits);
            } else if (boost::algorithm::starts_with(line, ":")) {
                float bias = boost::lexical_cast<float>(line.substr(1));
                (*output_model)->update_weight(0, bias);
                start_reading_weights = true;
            }
        }
        model_file.close();
        return true;
    } catch (exception &ex) {
        BOOST_LOG_TRIVIAL(error) << "Fail to load model: " << ex.what();
        return false;
    }
}