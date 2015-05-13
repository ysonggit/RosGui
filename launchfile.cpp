#include "launchfile.h"

using namespace std;


void LaunchFile::updateParam(ptree & param , LaunchParam lp){
    param.add("<xmlattr>.type", lp.ptype);
    param.add("<xmlattr>.name", lp.name);
    param.add("<xmlattr>.value", lp.value);
}

void LaunchFile::updateNode(ptree & node,  LaunchNode ln){
     node.add("<xmlattr>.type", ln.ntype);
     node.add("<xmlattr>.pkg", ln.pkg);
     node.add("<xmlattr>.name", ln.name);
     node.add("<xmlattr>.output", ln.output);
     //node.add("<xmlattr>.required", ln.required);
     //node.add("<xmlattr>.launch_prefix", ln.launch_prefix);
     for(LaunchParam lp : ln.params){
         ptree & param = node.add("param", "");
         updateParam(param, lp);
     }
}

void LaunchFile::create(vector<LaunchNode> lnodes, vector<LaunchParam> lparams){
    for (int i=0; i<lparams.size(); i++){
        ptree & param = launch.add("launch.param", "");
        updateParam(param, lparams[i]);
    }
    for(int i=0; i<lnodes.size(); i++){
        ptree & node = launch.add("launch.node", "");
        updateNode(node, lnodes[i]);
    }
}

void LaunchFile::write(const char * dir){
    string fullpath = string(dir)+filename;
    write_xml(fullpath, launch, std::locale(), xml_writer_settings<char>(' ', 4));
}
