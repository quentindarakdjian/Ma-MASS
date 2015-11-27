/*
 * File:   EP_IDFObject.h
 * Author: jake
 *
 * Created on September 18, 2013, 11:07 AM
 */

#ifndef EP_IDFOBJECT_H
#define	EP_IDFOBJECT_H
#include <string>
#include <vector>

class EP_IDFObject {
public:
    EP_IDFObject();
    EP_IDFObject(std::string idfLine);
    void add_item(std::string s);
    bool is(std::string s);
    std::string at(int i);
    void print();
    std::vector<std::string> getItems();
private:

    std::vector<std::string> items;

};

#endif	/* EP_IDFOBJECT_H */

