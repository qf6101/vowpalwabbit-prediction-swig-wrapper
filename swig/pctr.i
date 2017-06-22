%module pctr_swig

%include "std_pair.i"
%include "std_string.i"
%include "std_vector.i"

%template(kv_pair) std::pair<std::string, float>;
%template(pair_vector) std::vector<std::pair<std::string, float>>;

%{
#include "log/pctr_log.h"
#include "attribute/recent_doc.h"
#include "attribute/user_profile.h"
#include "attribute/context_info.h"
#include "attribute/doc_attributes.h"
#include "prediction/linear_predictor.h"
%}

%include "attribute/recent_doc.h"
%template(rectdoc_vector) std::vector<recent_doc>;

%include "log/pctr_log.h"
%include "attribute/user_profile.h"
%include "attribute/context_info.h"
%include "attribute/doc_attributes.h"
%include "prediction/linear_predictor.h"

%pragma(java) jniclasscode=%{
    static {
        System.loadLibrary("pctr_swig");
    }
%}