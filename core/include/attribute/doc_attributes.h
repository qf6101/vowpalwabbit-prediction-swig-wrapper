//
// Created by qfeng on 17-5-23.
//

#ifndef PCTR_DOC_ATTRIBUTES_H
#define PCTR_DOC_ATTRIBUTES_H

#include <string>
#include <vector>
#include <utility>
using namespace std;

class doc_attributes {
public:
    string doc_id;
    string doc_type;
    vector<pair<string, float>> categories;
    vector<pair<string, float>> keywords;
    float hotness = -1;
    string position;
    long ptime = -1;
    float quality = -1;
    float vulgar = -1;
};


#endif //PCTR_DOC_ATTRIBUTES_H
