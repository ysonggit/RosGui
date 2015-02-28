#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <stdlib.h>
#include <string>
#include <set>
#include <iostream>
#include <sstream>
#include <vector>

using boost::property_tree::ptree;
using boost::property_tree::write_xml;
using boost::property_tree::xml_writer_settings;
using namespace std;

struct LaunchParam{
    string name;
    string ptype;
    string value;
    LaunchParam(){}
    LaunchParam(std::string n, std::string t, std::string v) : name(n), ptype(t), value(v){}
};

struct LaunchNode{
    string ntype;
    string pkg;
    string name;
    string output;
    string required;
    string launch_prefix;
    vector<LaunchParam> params;
};

class LaunchFile{
public:
    LaunchFile(string fname) : filename(fname){
        launch.add("launch", "");
    }
    ptree launch;
    string filename;
    void updateNode(ptree & , LaunchNode );
    void updateParam(ptree & , LaunchParam );
    void create(vector<LaunchNode>, vector<LaunchParam> );
    void write(const char * dir);
};