//
// Created by qfeng on 17-5-23.
//

#ifndef PCTR_DOC_ATTRIBUTES_H
#define PCTR_DOC_ATTRIBUTES_H

#include <string>
#include <vector>
#include <utility>
#include <limits>
using namespace std;

class doc_attributes {
public:
    string doc_id;
    string doc_type;
    vector<pair<string, float>> categories;
    vector<pair<string, float>> keywords;
    string position0;
    string position1;
    string position2;
    unsigned long ptime = 0;
    float hotness = -1;
    float quality = -1;
    float vulgar = -1;
};


#endif //PCTR_DOC_ATTRIBUTES_H
