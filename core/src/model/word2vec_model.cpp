#include "model/word2vec_model.h"
#include <fstream>
#include <log/pctr_log.h>


using std::string;

word2vec_model::word2vec_model(EmbeddingMap&& embeddings) {
    _embeddings = embeddings;
    _logger = spdlog::get(pctr_log::_core_logger_name);
    _logger->info("Initialize word2vec model with embeddings size={}", _embeddings.size());
};

std::unique_ptr<word2vec_model> word2vec_model::load_model(string model_path) {
    auto _logger = spdlog::get(pctr_log::_core_logger_name);
    try {
        std::ifstream model_file;
        word2vec_model::EmbeddingMap embeddings;
        model_file.exceptions(std::ios_base::failbit | std::ios_base::badbit);
        model_file.open(model_path);
        int n_word, n_dim;
        model_file >> n_word >> n_dim;
        for (int i = 0; i < n_word; i++) {
            string word;
            model_file >> word;
            DenseVector vec(n_dim);
            for (int j = 0; j < n_dim; j++) {
                model_file >> vec[j];
            }
            embeddings.emplace(word, vec);
        }
        if (model_file.is_open()) model_file.close();
        _logger->info("Natively loaded word2vec model from {}.", model_path);
        return std::unique_ptr<word2vec_model>(new word2vec_model(std::move(embeddings)));
    } catch (std::exception &ex) {
        _logger->error("Fail to natively load word2vec model from {}, detail: {}", model_path, ex.what());
        return nullptr;
    }
}