#include <iostream>
#include <string>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::property_tree;

namespace {

// fragment from boost documentation. For test pupposses only
const string XML_TEXT = R"~(
<?xml version='1.0'?>
<!DOCTYPE rootElement>
<html>
    <head>
        <title>Chapter&#160;22.&#160;Boost.PropertyTree - 1.55.0</title>
    </head>
    <body bgcolor="white" text="black" link="#0000FF" vlink="#840084" alink="#0000FF">
        <div id="boost-common-heading-doc">
            <div class="heading-inner">
                <div class="heading-placard"></div>

                <h1 class="heading-title">
                    <a href="/">
                        <img src="/gfx/space.png" alt= "Boost C++ Libraries" class="heading-logo"></img>
                        <span class="heading-boost">Boost</span>
                        <span class="heading-cpplibraries">C++ Libraries</span>
                    </a>
                </h1>

                <p class="heading-quote">
                    <q>...one of the most highly regarded and expertly designed C++ library projects in the world.</q>
                    <span class="heading-attribution">&mdash;
                        <a href="http://www.gotw.ca/" class="external">Herb Sutter</a> and
                        <a href="http://en.wikipedia.org/wiki/Andrei_Alexandrescu" class="external">Andrei Alexandrescu</a>,
                        <a href="http://safari.awprofessional.com/?XmlId=0321113586" class="external">C++ Coding Standards</a>
                    </span>
                </p>
            </div>
        </div>
    </body>
</html>
)~";

}

int main() {
    stringstream stream;
    stream << XML_TEXT;

    try {
        ptree pt;
        read_xml(stream, pt);

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
        cout << endl;

    } catch (const std::exception& ex) {
        cout << "Error: " << ex.what() << endl;
    } catch (...) {
        cout << "FATAL!!!" << endl;
        return 1;
    }

    return 0;
}

