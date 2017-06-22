#include "feature/feature_engineering_user_doc_similarity.h"
#include "feature/vector_space_feature_helper.h"

void feature_engineering_user_doc_similarity::fill_features(
  features &f, const user_profile &user, const context_info &cxt, const doc_attributes &doc) {
    f._user_doc_keyword_w2v_similarity = optional_cosine_similarity(
      average_word2vec_vector(w2v_model, user.tagwords),
      average_word2vec_vector(w2v_model, doc.keywords)
    );
    f._user_doc_keyword_cosine_similarity = optional_cosine_similarity(
       word_weight_to_sparse_vector(user.tagwords, hash_size),
       word_weight_to_sparse_vector(doc.keywords, hash_size)
    );
    f._user_doc_category_jaccard_similarity = jaccard_similarity(
      user.category, doc.categories, jaccard_min_weight
    );
    f._user_doc_category_cosine_similarity = optional_cosine_similarity(
       word_weight_to_sparse_vector(user.category, hash_size),
       word_weight_to_sparse_vector(doc.categories, hash_size)
    );
}