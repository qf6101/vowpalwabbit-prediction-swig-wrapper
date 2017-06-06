package io.github.qf6101.pctrswigtest;

import io.github.qf6101.pctrswig.recent_doc;
import io.github.qf6101.pctrswig.kv_pair;
import io.github.qf6101.pctrswig.pair_vector;
import io.github.qf6101.pctrswig.rectdoc_vector;
import io.github.qf6101.pctrswig.user_profile;
import io.github.qf6101.pctrswig.context_info;
import io.github.qf6101.pctrswig.doc_attributes;
import io.github.qf6101.pctrswig.linear_predictor;

class PctrSwigTest {
    public static void main(String[] args) {
        linear_predictor predictor = new linear_predictor("", 30);
        user_profile user = new user_profile();
        user.setUser_id("abc");
        user.setGender("male");

        rectdoc_vector rdoc_vec = new rectdoc_vector();
        recent_doc rdoc = new recent_doc();
        rdoc.setDoc_id("res");
        rdoc_vec.add(rdoc);
        context_info cxt = new context_info();
        cxt.setTime(123456);
        cxt.setPlatform("ios");
        cxt.setNetwork("4g");
        cxt.setRdocs(rdoc_vec);

        doc_attributes doc = new doc_attributes();
        doc.setDoc_id("efg");
        doc.setDoc_type("C_");
        doc.setHotness(0.8f);
        pair_vector categories = new pair_vector();
        categories.add(new kv_pair("娱乐", 0.8f));
        categories.add(new kv_pair("体育", 0.2f));
        doc.setCategories(categories);
        float score = predictor.test(user, cxt, doc);
        System.out.println("score: " + score);
    }
}