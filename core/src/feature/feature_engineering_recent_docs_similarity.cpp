#include "feature/feature_engineering_recent_docs_similarity.h"
#include "feature/vector_space_feature_helper.h"

optional<DenseVector> average_recent_keyword_word2vec_vector(
        const word2vec_model &w2v_model, const std::vector<recent_doc> &docs) {
    std::vector<DenseVector> vecs;
    for (auto x: docs) {
        if (auto vec = average_word2vec_vector(w2v_model, x.keywords)) {
            vecs.push_back(std::move(vec.value()));
        }
    }
    return average_vec(vecs);
}

optional<SparseVector> average_recent_keyword_sparse_vector(
        const std::vector<recent_doc> &docs, const int hash_size) {
    std::vector<SparseVector> vecs;
    for (auto x: docs) {
        if (auto vec = word_weight_to_sparse_vector(x.keywords, hash_size)) {
            vecs.push_back(std::move(vec.value()));
        }
    }
    return average_vec(vecs);
}

optional<SparseVector> average_recent_category_sparse_vector(
        const std::vector<recent_doc> &docs, const int hash_size) {
    std::vector<SparseVector> vecs;
    for (auto x: docs) {
        if (auto vec = word_weight_to_sparse_vector(x.categories, hash_size)) {
            vecs.push_back(std::move(vec.value()));
        }
    }
    return average_vec(vecs);
}

// #define AVERAGE_RECENT_ATTRIBUTE(name) \
//   float average_recent_##name(const std::vector<recent_doc>& docs) { \
//     float res = 0.0f; \
//       for(auto x: docs){ \
//         res += x.name; \
//     } \
//     res /= docs.size(); \
//     return res; \
//   }

// AVERAGE_RECENT_ATTRIBUTE(hotness);


void feature_engineering_recent_docs_similarity::fill_features(
        features &f, const user_profile &user, const context_info &cxt, const doc_attributes &doc) {
    const unsigned long TIME_LIMIT = 4102416000; //utc of 2100/01/01
    if (cxt.time <= 0 || cxt.time >= TIME_LIMIT) return;

    auto time = cxt.time;
    std::vector<recent_doc> one_min_docs, five_min_docs, ten_min_docs, long_time_docs;
    for (auto x: cxt.rdocs) {
        if (x.time <= 0 || x.time >= TIME_LIMIT) continue;
        auto time_diff = time - x.time;
        if (time_diff <= 60) one_min_docs.push_back(x);
        else if (time_diff <= 300) five_min_docs.push_back(x);
        else if (time_diff <= 600) ten_min_docs.push_back(x);
        else long_time_docs.push_back(x);
    }

    bool has_recent_docs = !(one_min_docs.empty() && five_min_docs.empty() && ten_min_docs.empty() &&
                             long_time_docs.empty());

    if (has_recent_docs) {
        auto doc_keyword_w2v_vec = average_word2vec_vector(w2v_model, doc.keywords);

        f._recent_doc_1min_keyword_w2v_similarity = optional_cosine_similarity(
                doc_keyword_w2v_vec,
                average_recent_keyword_word2vec_vector(w2v_model, one_min_docs)
        );
        f._recent_doc_5min_keyword_w2v_similarity = optional_cosine_similarity(
                doc_keyword_w2v_vec,
                average_recent_keyword_word2vec_vector(w2v_model, five_min_docs)
        );
        f._recent_doc_10min_keyword_w2v_similarity = optional_cosine_similarity(
                doc_keyword_w2v_vec,
                average_recent_keyword_word2vec_vector(w2v_model, ten_min_docs)
        );
        f._recent_doc_longtime_keyword_w2v_similarity = optional_cosine_similarity(
                doc_keyword_w2v_vec,
                average_recent_keyword_word2vec_vector(w2v_model, long_time_docs)
        );

        auto doc_keyword_sparse_vec = word_weight_to_sparse_vector(doc.keywords, hash_size);
        f._recent_doc_1min_keyword_cosine_similarity = optional_cosine_similarity(
                doc_keyword_sparse_vec,
                average_recent_keyword_sparse_vector(one_min_docs, hash_size)
        );
        f._recent_doc_5min_keyword_cosine_similarity = optional_cosine_similarity(
                doc_keyword_sparse_vec,
                average_recent_keyword_sparse_vector(five_min_docs, hash_size)
        );
        f._recent_doc_10min_keyword_cosine_similarity = optional_cosine_similarity(
                doc_keyword_sparse_vec,
                average_recent_keyword_sparse_vector(ten_min_docs, hash_size)
        );
        f._recent_doc_longtime_keyword_cosine_similarity = optional_cosine_similarity(
                doc_keyword_sparse_vec,
                average_recent_keyword_sparse_vector(long_time_docs, hash_size)
        );

        auto doc_category_sparse_vec = word_weight_to_sparse_vector(doc.categories, hash_size);
        f._recent_doc_1min_category_cosine_similarity = optional_cosine_similarity(
                doc_category_sparse_vec,
                average_recent_category_sparse_vector(one_min_docs, hash_size)
        );
        f._recent_doc_5min_category_cosine_similarity = optional_cosine_similarity(
                doc_category_sparse_vec,
                average_recent_category_sparse_vector(five_min_docs, hash_size)
        );
        f._recent_doc_10min_category_cosine_similarity = optional_cosine_similarity(
                doc_category_sparse_vec,
                average_recent_category_sparse_vector(ten_min_docs, hash_size)
        );
        f._recent_doc_longtime_category_cosine_similarity = optional_cosine_similarity(
                doc_category_sparse_vec,
                average_recent_category_sparse_vector(long_time_docs, hash_size)
        );
    }
}