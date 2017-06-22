//
// Created by qfeng on 17-6-6.
//

#include "feature/feature_engineering_time.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <log/pctr_log.h>

using namespace boost::posix_time;
using namespace boost::gregorian;


void feature_engineering_time::fill_features(features &f,
                                             const user_profile &user,
                                             const context_info &cxt,
                                             const doc_attributes &doc) {
    auto logger = spdlog::get(pctr_log::_core_logger_name);
    const unsigned long TIME_LIMIT = 4102416000; //utc of 2100/01/01
    typedef boost::date_time::c_local_adjustor<ptime> local_adj;

    if (cxt.time > 0 && cxt.time < TIME_LIMIT) {
        try {
            // initialization
            f._time = cxt.time;
            ptime t = from_time_t(f._time.get());
            t = local_adj::utc_to_local(t);
            // weekday
            f._weekday = t.date().day_of_week();
            if (f._weekday.get() == 0) f._weekday = 7;
            // workday
            f._is_workday = f._weekday.get() < 6;
            // hours
            f._hour = t.time_of_day().hours();
            f._hour_desc = get_time_intraday_period(f._hour.get());
        } catch (exception &ex) {
            logger->debug("Fail to parse context time features={}", cxt.time);
        }

    }
    if (doc.ptime > 0 && doc.ptime < TIME_LIMIT) {
        try {
            // initialization
            ptime t = from_time_t(doc.ptime);
            t = local_adj::utc_to_local(t);
            // hours
            f._publish_hour = t.time_of_day().hours();
            f._publish_hour_desc = get_time_intraday_period(f._publish_hour.get());
        } catch (exception &ex) {
            logger->debug("Fail to parse document time features={}", doc.ptime);
        }
    }
    if (cxt.time > 0 && cxt.time < TIME_LIMIT && doc.ptime > 0 && doc.ptime < TIME_LIMIT) {
        ptime view_time = from_time_t(cxt.time);
        view_time = local_adj::utc_to_local(view_time);
        ptime doc_time = from_time_t(doc.ptime);
        doc_time = local_adj::utc_to_local(doc_time);

        f._view_and_doc_in_same_day = view_time.date().year() == doc_time.date().year() &&
                                      view_time.date().day_of_year() == doc_time.date().day_of_year();

        auto delta = cxt.time - doc.ptime;

        f._view_and_doc_delay_days = 1.0f * delta / (24 * 60 * 60);

        if (delta < -60 * 60.0)
            f._view_and_doc_delay_desc = 6; // view time is an hour before doc time
        else if (delta <= 60 * 60.0)
            f._view_and_doc_delay_desc = 0; // in an hour
        else if (delta <= 24 * 60 * 60)
            f._view_and_doc_delay_desc = 1; // in a day
        else if (delta <= 2 * 24 * 60 * 60)
            f._view_and_doc_delay_desc = 2; // in two days
        else if (delta <= 3 * 24 * 60 * 60)
            f._view_and_doc_delay_desc = 3; // in three days
        else if (delta <= 5 * 24 * 60 * 60)
            f._view_and_doc_delay_desc = 4; // in a week
        else
            f._view_and_doc_delay_desc = 5; // after a week
    }
}

short feature_engineering_time::get_time_intraday_period(short hour) {
    if (hour <= 6)
        return 0; //midnight
    else if (hour <= 8)
        return 1; //early morning
    else if (hour <= 12)
        return 2; //morning
    else if (hour <= 17) //afternoon
        return 3;
    else if (hour <= 21) //night
        return 4;
    else //deep night
        return 5;
}