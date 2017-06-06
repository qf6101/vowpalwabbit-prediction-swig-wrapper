//
// Created by qfeng on 17-6-2.
//

#ifndef PCTR_RECENT_DOC_H
#define PCTR_RECENT_DOC_H

#include <string>
#include <vector>
#include <utility>
using namespace std;

class recent_doc {
public:
    string doc_id;
    long time;
    vector<pair<string, float>> categories;
    vector<pair<string, float>> keywords;
};

#endif //PCTR_RECENT_DOC_H
