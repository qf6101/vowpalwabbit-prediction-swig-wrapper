//
// Created by qfeng on 17-6-2.
//

#ifndef PCTR_VW_INT_FEATURE_H
#define PCTR_VW_INT_FEATURE_H

#include <string>
#include <boost/optional.hpp>

using namespace std;
using boost::optional;

class vw_feature {
protected:
    virtual optional<tuple<string, string, float>> dumps() = 0;
    virtual ~vw_feature() {};
};


#endif //PCTR_VW_INT_FEATURE_H
