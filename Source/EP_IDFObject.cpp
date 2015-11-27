/*
 * File:   EP_IDFObject.cpp
 * Author: jake
 *
 * Created on September 18, 2013, 11:07 AM
 */
#include <ostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <iostream>
#include "EP_IDFObject.h"

EP_IDFObject::EP_IDFObject() {
}

EP_IDFObject::EP_IDFObject(std::string idfLine) {
    boost::split(items, idfLine, boost::is_any_of(","));
}

std::vector<std::string> EP_IDFObject::getItems() {
    return items;
}

void EP_IDFObject::add_item(std::string s) {
    items.push_back(s);
}

bool EP_IDFObject::is(std::string s) {
    if (items.size() < 1) {
        return false;
    }
    return (items.at(0) == s);

}

std::string EP_IDFObject::at(int i) {
    return items.at(i);
}

void EP_IDFObject::print() {

    for(std::string o: items) {
        std::cout << o << ",";
    }
    std::cout << std::endl;
}
