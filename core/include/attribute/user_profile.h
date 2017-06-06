//
// Created by qfeng on 17-5-23.
//

#ifndef PCTR_USER_PROFILE_H
#define PCTR_USER_PROFILE_H

#include <string>
#include <vector>
#include <utility>

using namespace std;


class user_profile {
public:
    string user_id;
    string gender;
    vector<pair<string, float>> category;
    vector<pair<string, float>> tagwords;
};


#endif //PCTR_USER_PROFILE_H
