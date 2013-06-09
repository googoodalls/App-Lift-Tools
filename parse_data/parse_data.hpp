#ifndef _parse_data_h_
#define _parse_data_h_

#include <string>
#include <list>
#include "pugixml.hpp"

enum device_type
{
    device_null,
    device_iPhone,
    device_iPad,
    device_all,
};


struct offer_t{
    int off_id;
    std::string name;
    std::string full_name;
    int itunes_id;
    float payout;
    std::string tracking_url;
    std::list<std::string> contries;
    std::string contries_str;
    device_type type;
    bool image_exists;
};


void load_data(pugi::xml_node nodes);
bool parse_data(offer_t &offer, pugi::xml_node &node);
void load_data2(pugi::xml_node nodes);

#endif