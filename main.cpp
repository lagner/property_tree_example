#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::property_tree;

extern std::string XML_TEXT;

boost::property_tree::ptree create_tree();
boost::property_tree::ptree parse_tree(istream& xml_stream);


int main() {
    try {
        // 1) serialize data
        create_tree();

        // 2) parse xml data
        std::stringstream stream;
        stream << XML_TEXT;
        parse_tree(stream);

    } catch (const std::exception& ex) {
        cout << "Error: " << ex.what() << endl;
    } catch (...) {
        cout << "FATAL!!!" << endl;
        return 1;
    }
    return 0;
}

boost::property_tree::ptree parse_tree(istream& xml_stream) {
    ptree pt;
    read_xml(xml_stream, pt);

    // get string value. If value wasn't found ptree_bad_path exception will be thrown
    string title = pt.get<std::string>("html.head.title");
    cout << "title: " << title << endl;

    // iterate through all "body" arrtibutes.
    // get_child returns child ptree or ptree_bad_path ex will be thrown
    ptree child_tree = pt.get_child("html.body.<xmlattr>");
    cout << "<body> attrbutes: " << endl;
    for (ptree::value_type& node: child_tree) {
        string attr_name = node.first;
        string attr_value = node.second.data();
        cout << "\t" << attr_name << " \t- " << attr_value << endl;
    }
    cout << endl;

    // try to get q tag
    boost::optional<ptree&> q_node = pt.get_child_optional("html.body.div.div.p.q");
    if (q_node) {
        cout << "q tag was found: " << q_node.get().data() << endl;
    } else {
        cout << "q tag wasn't found" << endl;
    }
    return pt;
}

boost::property_tree::ptree create_tree() {
    ptree pt;

    // add simple values
    pt.put("name", "Ivan");
    pt.put("last_name", "Ivanov");
    pt.put("age", 33);

    // add sub trees
    {
        ptree card;
        card.put("number", "0000 1111 2222 3333");
        card.put("expires", "01/11");
        card.put("cvc2_cvv2_cid", 111);

        pt.put_child("card", card);
    }

    // add collection data
    {
        ptree childs_array;

        vector<string> names = { "Misha", "Masha" };

        int i = 5;
        for (auto& name : names) {
            ptree child_tree;
            child_tree.put("name", name);
            child_tree.put("age", i++);

            // if you want to get json array as a result, you should use
            // an empty string as a key. But in this case xml output wouldn't look nice
            childs_array.push_back(std::make_pair("", child_tree));
        }
        pt.add_child("children", childs_array);
    }


    cout << "write json ----------------------------------------" << endl;
    json_parser::write_json(cout, pt, true);

    cout << "write xml -----------------------------------------" << endl;
    // note the key of chiled obj
    xml_writer_settings<char> settings('\t', 1);
    xml_parser::write_xml(cout, pt, settings);
    cout << "---------------------------------------------------" << endl;

    return pt;
}
