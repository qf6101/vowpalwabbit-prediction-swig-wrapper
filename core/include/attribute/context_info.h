//
// Created by qfeng on 17-5-23.
//

#ifndef PCTR_CONTEXT_INFO_H
#define PCTR_CONTEXT_INFO_H


#include <string>
#include "recent_doc.h"

using namespace std;

class context_info {
public:
    unsigned long time = 0;
    string platform;
    string network;
    string brand;
    string location0;
    string location1;
    string location2;
    vector<recent_doc> rdocs;
};


#endif //PCTR_CONTEXT_INFO_H
