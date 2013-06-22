#include <map>
#include <iostream>
//#include <boost/filesystem/operations.hpp>
//#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/foreach.hpp>
#include <fstream>
#include <boost/range/algorithm.hpp>
#include <vector>
#include "parse_data.hpp"
#include <boost/filesystem.hpp>


using namespace std;
using namespace boost;


typedef PUGIXML_CHAR char_t;

std::ostream & operator<<(std::ostream & stream, const offer_t & offer)
{
    stream << "\"" << offer.off_id << "\"" << ", ";
    stream << "\"" << offer.name << "_small.jpg" "\"" << ", ";
    
    stream << "\"";
    if (device_iPhone == offer.type) stream << "iPhone";
    else if  (device_iPad == offer.type) stream << "iPad";
    else if  (device_all == offer.type) stream << "iOS";
    else assert(0);
    stream << "\"" << ", ";
//    stream << offer.payout << ", ";

//    stream << offer.itunes_id << ", ";
//    stream << "\"" << offer.full_name << "\"" << ", ";
    stream << "\"" << offer.contries_str << "\"";
    
    
    
    return stream;
}


const char* offers_to_ban[] = {
//    "Immortalis",
    "Casino Games",
    "Kabam Slots",
    "Poker",
};

bool parse_data(offer_t &offer, pugi::xml_node &node) {
    
    string _name(node.child("name").first_child().value());
    
    if (boost::algorithm::ifind_first(_name, "android")) {
        return false;
    }
    
    if (boost::algorithm::ifind_first(_name, "IDFA") || boost::algorithm::ifind_first(_name, "UDID") || boost::algorithm::ifind_first(_name, "MAC address")) {
        return false;
    }

    if (!(boost::algorithm::ifind_first(_name, "iphone")||boost::algorithm::ifind_first(_name, "ios")||boost::algorithm::ifind_first(_name, "ipad"))) {
        return false;
    }
    if(lexical_cast<float>(node.child("payout").first_child().value()) == 0.0) {
        return false;
    }
    if(strcmp(node.child("payout_type").first_child().value(), "cpa_percentage") == 0) {
        return false;
    }
    BOOST_FOREACH(const char* ban, offers_to_ban) {
        if (boost::algorithm::ifind_first(_name, ban)) {
            return false;
        }
    }

    
    for (pugi::xml_node sub_node = node.first_child(); sub_node; sub_node = sub_node.next_sibling()) {
//        std::cout << " " << sub_node.name() << '\t'  << sub_node.first_child().value() << std::endl;        
        string name(sub_node.name());
        if(boost::algorithm::ifind_first(name, "name")) {
            string _name(sub_node.first_child().value());
            if(ifind_first(_name, "iOS") && ifind_first(_name, "iPhone")) offer.type = device_iPhone;
            if(ifind_first(_name, "iOS") && ifind_first(_name, "iPad")) offer.type = device_iPad;
            else if(ifind_first(_name, "iOS")) offer.type = device_all;
            else if (ifind_first(_name, "iPhone") && ifind_first(_name, "iPad")) offer.type = device_all;
            else if (ifind_first(_name, "iPhone")) offer.type = device_iPhone;
            else if (ifind_first(_name, "iPad")) offer.type = device_iPad;
            else assert(0);
            offer.full_name = _name;            
            typedef vector< string > split_vector_type;
            split_vector_type SplitVec;
            split( SplitVec, _name, is_any_of("()") );
//            SplitVec[0].erase(boost::remove_if(SplitVec[0], boost::is_any_of(": ")), SplitVec[0].end());
            SplitVec[0].erase(boost::remove_if(SplitVec[0], !boost::is_alpha()), SplitVec[0].end());

            offer.name = SplitVec[0];
//            std::cout << "name" << '\t'  << sub_node.first_child().value() << std::endl;
        }
        else if (boost::algorithm::ifind_first(name, "preview_url")) {
            string itunes_url(sub_node.first_child().value());
//            std::cout << itunes_url << endl;
            typedef vector< string > split_vector_type;
            split_vector_type SplitVec;
            split( SplitVec, itunes_url, is_any_of("/") );
            itunes_url = SplitVec[SplitVec.size() - 1];
            split( SplitVec, itunes_url, is_any_of("?") );
            SplitVec[0].erase(boost::remove_if(SplitVec[0], !boost::is_digit()), SplitVec[0].end());
//            std::cout << SplitVec[0] << endl;
            try {
                offer.itunes_id = lexical_cast<int>(SplitVec[0]);

            } catch (...) {
                offer.itunes_id = -1;
            }

        }
        else if (boost::algorithm::ifind_first(name, "id")) {
            offer.off_id = lexical_cast<int>(sub_node.first_child().value());
        }

        
        else if (boost::algorithm::ifind_first(name, "tracking_url")) {
            offer.tracking_url = string(sub_node.first_child().value());
//            std::cout << "payout_type" << '\t'  << sub_node.first_child().value() << std::endl;
            
        }
        
        else if (boost::algorithm::ifind_first(name, "payout_type")) {
            //            std::cout << "payout_type" << '\t'  << sub_node.first_child().value() << std::endl;
            
        }
        else if (boost::algorithm::ifind_first(name, "payout")) {
            offer.payout = lexical_cast<float>(sub_node.first_child().value());

        }
        else if (boost::algorithm::ifind_first(name, "countries_short")) {
//            std::cout << "countries_short" << '\t'  << sub_node.first_child().value() << std::endl;
            offer.contries_str = string(sub_node.first_child().value());
        }
    }
//    cout << offer;
    return true;
}


//typedef std::pair<const char* , int> itune_id_t;
//itune_id_t offers_perform_great[] = {
//    itune_id_t( "Candy Crush Saga",  553834731 ),
//    itune_id_t( "Dragon City Mobile",  561941526 ),
//    itune_id_t( "Hay Day",  506627515 ),
//    itune_id_t( "The Hobbit",  558823262 ),
//    itune_id_t( "Pet Rescue Saga",  572821456 ),
//    itune_id_t( "Megapolis",  580765736 ),
//    itune_id_t( "Four Kingdoms",  585661281 ),
//    itune_id_t( "Marvel War",  536478373 ),
//};
const char* offers_perform_great[] = {
    "Candy Crush Saga",
    "Dragon City Mobile",
    "The Hobbit",
    "Marvel War",
    "Pet Rescue Saga",
    "Megapolis",
//    "Four Kingdoms",
    "Immortalis",
    "Hay Day",
    "Spellstorm",
};


struct ordering {
    bool operator ()(offer_t const& a, offer_t const& b) {
        if (a.itunes_id == b.itunes_id) return a.payout > b.payout;
        BOOST_FOREACH(const char* offer, offers_perform_great) {
            if(boost::algorithm::ifind_first(a.full_name, offer)) {
                if(boost::algorithm::ifind_first(b.full_name, offer)) return true;
                return true;

            }
            if(boost::algorithm::ifind_first(b.full_name, offer)) return false;

//            if(a.itunes_id == offer.second) return true;
//            if(b.itunes_id == offer.second) return false;

        }
//        if (553834731 == a.itunes_id) return true;
//        cout << " done " << endl;
        return a.payout > b.payout;
    }
};


void load_data(pugi::xml_node nodes) {
    
    map<int, offer_t> statistics_data;
    typedef map<int, offer_t>::value_type value_type;
    
    nodes = nodes.first_child();
    offer_t offer;
    for (; nodes; nodes = nodes.next_sibling()) {
        parse_data(offer, nodes);
        map<int, offer_t>::iterator find = statistics_data.find(offer.itunes_id);
        if (statistics_data.end() == find) {
            statistics_data[offer.itunes_id] = offer;
        }
        else {
            assert(find->first == offer.itunes_id);
            assert(boost::iequals(find->second.name, offer.name));
            assert(boost::iequals(find->second.name, offer.name));
            find->second.payout = max(find->second.payout, offer.payout);
            find->second.contries_str.append(",");
            find->second.contries_str.append(offer.contries_str);
            if(find->second.type != offer.type) find->second.type = device_all;
        }
    }
    
    
    vector<offer_t> offs;
    BOOST_FOREACH(value_type &value, statistics_data) {
        offs.push_back(value.second);
    }
    
    boost::sort(offs, ordering());
    
//    /Users/wuheshun/html/iu_affiliate/affiliate_img/all_image
    
    BOOST_FOREACH(offer_t &value, offs) {
        string file_name = value.name + "_small.jpg";
        file_name = "/Users/wuheshun/html/iu_affiliate/affiliate_img/all_image/" + file_name;
        value.image_exists = boost::filesystem::exists( file_name );
        if ( !boost::filesystem::exists( file_name ) ) {
            std::cout << value.name + "_small.jpg" << std::endl;
        }
    }


    
    BOOST_FOREACH(offer_t &value, offs) {
        if (!value.image_exists) continue;
        cout << "offers[" << "_index++" << "] = new Array(";
        cout << value;
        cout << ")" << endl;
    }
    
    std::ofstream f("name_info_text.txt");
    
    BOOST_FOREACH(value_type &value, statistics_data) {
        f << value.second.itunes_id << '\t' << value.second.name << "_small.jpg" << "     " << value.second.full_name << endl;
    }
}

void load_data2(pugi::xml_node nodes) {
    
    vector<offer_t> all_offers;
    typedef vector<offer_t>::value_type value_type;
    
    nodes = nodes.first_child();
    offer_t offer;
    for (; nodes; nodes = nodes.next_sibling()) {
        if (false == parse_data(offer, nodes)) continue;
        all_offers.push_back(offer);
    }
    
//    auto func1 = [](int i) { return i+4; };

//    auto compare = [](offer_t const& a, offer_t const& b) {return a.payout > b.payout;};
    
    boost::sort(all_offers, ordering());
    
    //    /Users/wuheshun/html/iu_affiliate/affiliate_img/all_image
    
    BOOST_FOREACH(offer_t &value, all_offers) {
        string file_name = value.name + "_small.jpg";
        file_name = "/Users/wuheshun/html/iu_affiliate/affiliate_img/all_image/" + file_name;
        value.image_exists = boost::filesystem::exists( file_name );
        if ( !boost::filesystem::exists( file_name ) ) {
            std::cout << value.name + "_small.jpg" << std::endl;
        }
    }
    
    
    
    BOOST_FOREACH(offer_t &value, all_offers) {
        if (!value.image_exists) continue;
        cout << "[" ;
        cout << value;
        cout << "]," << endl;
    }
    
//    std::ofstream f("name_info_text.txt");
//    
//    BOOST_FOREACH(value_type &value, statistics_data) {
//        f << value.second.itunes_id << '\t' << value.second.name << "_small.jpg" << "     " << value.second.full_name << endl;
//    }
}


