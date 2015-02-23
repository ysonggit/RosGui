#include "rosgui.h"
using namespace std;

const char * RosGui::lat_path = "src/controller/lattice/";
const char * RosGui::lau_path = "src/controller/launch/";

RosGui::RosGui()
    : roscore_btn("Start Roscore"),
      rviz_btn("Start Rviz"),
      clean_btn("Rosclean"),
      make_btn("CatkinMake"),
      launch_btn("Roslaunch"),
      rosrun_btn("Rosrun"),
      quit_btn("Quit"),
      m_VBox(Gtk::ORIENTATION_VERTICAL),
      m_EBox(Gtk::ORIENTATION_VERTICAL),
      m_RBox(Gtk::ORIENTATION_VERTICAL),
      m_Button_Buffer1("Show Lattice Files"),
      m_Button_Buffer2("Show Launch Files"),
      m_Button_Buffer3("Show Log Files"),
      m_Button_File("Choose File"),
      m_Frame_Generator("Launch File Generator"),
      m_VBox_Main(Gtk::ORIENTATION_VERTICAL, 5),
      m_VBox_Generator(Gtk::ORIENTATION_VERTICAL),
      m_VBox_Countdown(Gtk::ORIENTATION_VERTICAL),
    m_VBox_Robotsnum(Gtk::ORIENTATION_VERTICAL),
    m_VBox_Robotsposes(Gtk::ORIENTATION_VERTICAL),
    m_VBox_Lattice(Gtk::ORIENTATION_VERTICAL),
    m_Label_Countdown("Countdown Seconds: "),
    m_Label_Robotsnum("Robots Number: "),
    m_Label_Lattice("Lattice Graph: "),
    m_Label_Robotsposes("Initial Poses: "),
    m_adjustment_countdown( Gtk::Adjustment::create(1.0, 0.0, 60.0, 1.0, 1.0, 0.0) ),
    m_adjustment_robotsnum( Gtk::Adjustment::create(2.0, 1.0, 200.0, 1.0, 1.0, 0.0) ),
    m_SpinButton_Countdown(m_adjustment_countdown),
    m_SpinButton_Robotsnum(m_adjustment_robotsnum),
    m_Frame_Environment("Environment Dimension"),
    m_VBox_Environment(Gtk::ORIENTATION_VERTICAL),
    m_VBox_MinX(Gtk::ORIENTATION_VERTICAL),
    m_VBox_MinY(Gtk::ORIENTATION_VERTICAL),
    m_VBox_MaxX(Gtk::ORIENTATION_VERTICAL),
    m_VBox_MaxY(Gtk::ORIENTATION_VERTICAL),
    m_Label_MinX("Min X: "),
    m_Label_MinY("Min Y: "),
    m_Label_MaxX("Max X: "),
    m_Label_MaxY("Max Y: "),
    m_adjustment_minx(Gtk::Adjustment::create(-100.0, -1000.0, 1000.0, 10.0, 1.0, 0.0) ),
    m_adjustment_miny(Gtk::Adjustment::create(-100.0, -1000.0, 1000.0, 10.0, 1.0, 0.0) ),
    m_SpinButton_MinX(m_adjustment_minx),
    m_SpinButton_MinY(m_adjustment_miny),
    m_adjustment_maxx(Gtk::Adjustment::create(100.0, -1000.0, 1000.0, 10.0, 1.0, 0.0) ),
    m_adjustment_maxy(Gtk::Adjustment::create(100.0, -1000.0, 1000.0, 10.0, 1.0, 0.0) ),
    m_SpinButton_MaxX(m_adjustment_maxx),
    m_SpinButton_MaxY(m_adjustment_maxy),
    m_CheckButton_Savepose("Save Robots Poses"),
    m_CheckButton_Setpose("Manually Set Initial Poses"),
    m_Button_Enter("Enter Poses"),
    m_Button_Generate("Generate Launch File"),
    m_VBox_Panel(Gtk::ORIENTATION_VERTICAL)
{
    set_title("ROS GUI");
    lattice_files = read_dir(lat_path);
    robotsnum = 2;
    countdown = 1;
    savepose = false;
    setpose = false;
    minx = -100;
    miny = -100;
    maxx = 100;
    maxy = 100;
    pose_id = 1;
    ptrPoses = & all_poses;
    // Sets the border width of the window.
    set_border_width(10);
    set_size_request(300, 800);
    add(m_Table);
    //add(m_VBox_Panel);
    // get home dir
    working_dir = getWorkingDir();
    set_box_utilities();
    set_textview_buffer();
    set_box_roslaunch_rosrun();
    set_frame_generator();
    // The final step is to display this newly created widget...
    show_all_children();
}

RosGui::~RosGui(){}

void RosGui::on_roscore_btn_clicked(){
    // printf ("checking if processor is available...");
    // if (system(null)) puts ("ok");
    // else exit (exit_failure);
    printf ("Call roscore service ... \n");
    int i = system("gnome-terminal -x sh -c \"roscore; bash\"");
    //printf ("The value returned was: %d.\n",i);
}

void RosGui::on_rviz_btn_clicked(){
    printf ("Rosrun rviz ... \n");
    int i = system("gnome-terminal -x sh -c \"rosrun rviz rviz; bash\"");
}

void RosGui::on_quit_btn_clicked(){
    hide();
}

void RosGui::on_clean_btn_clicked(){
    printf ("Rosclean ... \n");
    int i = system("rm -rf ~/.ros/log/*");
}

void RosGui::on_make_btn_clicked(){
    printf ("catkin_make ... \n");
    int i = system("catkin_make");
}

std::string RosGui::getWorkingDir(){
    // show working directory
    char the_path[256];
    getcwd(the_path, 255);
    strcat(the_path, "/");
    printf("%s\n", the_path);
    return string(the_path);
}

void RosGui::set_box_utilities(){
    // This packs the button into the Window (a container).
    /*
      0          1          2
      0+----------+----------+
      |          |          |
      1+----------+----------+
      |          |          |
      2+----------+----------+
      void Gtk::Table::attach(Gtk::Widget& child,
      guint left_attach, guint right_attach,
      guint top_attach, guint bottom_attach,
      guint xoptions = Gtk::FILL | Gtk::EXPAND,
      guint yoptions = Gtk::FILL | Gtk::EXPAND,
      guint xpadding = 0, guint ypadding = 0);
    */
    m_Table.attach(m_BBox, 0,2, 0, 1);
    //m_VBox_Panel.pack_start(m_BBox);
    m_BBox.pack_start(utility_Box, Gtk::PACK_SHRINK);
    utility_Box.pack_start(roscore_btn);
    utility_Box.pack_start(rviz_btn);
    utility_Box.pack_start(clean_btn);
    utility_Box.pack_start(make_btn);
    utility_Box.pack_start(quit_btn);
 
    // When the button receives the "clicked" signal, it will call the
    // on_button_clicked() method defined below.
    roscore_btn.signal_clicked().connect(sigc::mem_fun(*this,
            &RosGui::on_roscore_btn_clicked));
    
    rviz_btn.signal_clicked().connect(sigc::mem_fun(*this,
            &RosGui::on_rviz_btn_clicked));

    quit_btn.signal_clicked().connect(sigc::mem_fun(*this,
            &RosGui::on_quit_btn_clicked));

    clean_btn.signal_clicked().connect(sigc::mem_fun(*this,
            &RosGui::on_clean_btn_clicked));

    make_btn.signal_clicked().connect(sigc::mem_fun(*this,
            &RosGui::on_make_btn_clicked));
}
void RosGui::set_box_roslaunch_rosrun(){
    // add box containing roslaunch button and lg entry
    m_Table.attach(m_EBox,0,2,1,2);
    //m_VBox_Panel.pack_start(m_EBox);
    m_EBox.pack_start(launch_Box, Gtk::PACK_SHRINK);
    launch_Box.pack_start(launch_btn, Gtk::PACK_SHRINK);
    launch_Box.pack_start(launch_entry, Gtk::PACK_SHRINK);
    launch_Box.pack_start(lg_entry, Gtk::PACK_SHRINK);
    launch_btn.signal_clicked().connect(sigc::mem_fun(*this,
            &RosGui::on_launch_btn_clicked));
    launch_entry.set_text("controller");
    lg_entry.set_text("");
    // Configure the Completion
    m_refEntryCompletion = Gtk::EntryCompletion::create();
    m_refCompletionModel = Gtk::ListStore::create(m_completionRecord);
    m_refEntryCompletion->set_model(m_refCompletionModel);
    lg_entry.set_completion(m_refEntryCompletion);
    m_refEntryCompletion->set_text_column (m_completionRecord.col_text);
    m_refEntryCompletion->set_minimum_key_length(1);
    m_refEntryCompletion->set_popup_completion(true);
    populateCompletion();
    // add box containing rosrun button, rospack entry and rosnode entry
    // m_Table.attach(m_RBox,0,2,2,3);
    // m_RBox.pack_start(run_Box, Gtk::PACK_SHRINK);
    // run_Box.pack_start(rosrun_btn, Gtk::PACK_SHRINK);
    // run_Box.pack_start(rospack_entry, Gtk::PACK_SHRINK);
    // run_Box.pack_start(rosnode_entry, Gtk::PACK_SHRINK);
    // rosrun_btn.signal_clicked().connect(sigc::mem_fun(*this,
    //         &RosGui::on_rosrun_btn_clicked));
    // //rospack_entry.set_max_length(100);
    // rospack_entry.set_text("rospackage name");
    // //rosnode_entry.set_max_length(100);
    // rosnode_entry.set_text("rosnode name");
 
}

void RosGui::set_textview_buffer(){ 
    m_Table.attach(m_VBox,0,2,3,10);
    // m_VBox_Panel.pack_start(m_VBox);
    //Add the TreeView, inside a ScrolledWindow, with the button underneath:
    m_ScrolledWindow.add(m_TextView);

    //Only show the scrollbars when they are necessary:
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_VBox.pack_start(m_ScrolledWindow);

    //Add buttons:
    m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);
    m_ButtonBox.pack_start(m_Button_Buffer1);
    m_ButtonBox.pack_start(m_Button_Buffer2);
    m_ButtonBox.pack_start(m_Button_Buffer3);
   
    //m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
    //Connect signals:
    m_Button_Buffer1.signal_clicked().connect(sigc::mem_fun(*this,
            &RosGui::on_button_buffer1) );
    m_Button_Buffer2.signal_clicked().connect(sigc::mem_fun(*this,
            &RosGui::on_button_buffer2) );
    m_Button_Buffer3.signal_clicked().connect(sigc::mem_fun(*this,
            &RosGui::on_button_buffer3) );

    m_ButtonBox.pack_start(m_Button_File);
    m_Button_File.signal_clicked().connect(sigc::mem_fun(*this,
            &RosGui::on_button_file_clicked) );
}

void RosGui::on_launch_btn_clicked(){
    printf ("Roslaunch ... \n");
    string launchfile = string("roslaunch ")
        + string(launch_entry.get_text()) + string(" ")
        + string(lg_entry.get_text());
    cout<<launchfile<<endl;
    const int n = launchfile.length()+1;
    char cmd[n];
    strncpy(cmd, launchfile.c_str(), n-1);
    cmd[n-1] = 0;
    int o = system(cmd);
}

void RosGui::on_rosrun_btn_clicked(){
    string run_cmd = string("rosrun ")
        + string(rospack_entry.get_text()) + string(" ")
        + string(rosnode_entry.get_text());
    const int n = run_cmd.length()+1;
    char cmd[n];
    strncpy(cmd, run_cmd.c_str(), n-1);
    cmd[n-1] = 0;
    int i = system(cmd);
}

vector<string> RosGui:: read_dir(const char * path){
    DIR* d = opendir(path);
    vector<string> files;
    if(d == NULL)  {
        printf("Invalid path!\n");
        return files;
    }
    for(struct dirent *de = NULL; (de = readdir(d)) != NULL; ){
        if(*de->d_name != '.'){ // do not add file name starting of .
            files.push_back(string(de->d_name));
        }
    }
    closedir(d);
    return files;
}
void RosGui::list_dir(const char *name, int level, std::string & files){
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(name))){
        return;
    }
    if (!(entry = readdir(dir))){
        return;
    }

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
            //printf("%*s[%s]\n", level*2, "", entry->d_name);
            for(int i=0; i<level*2; i++){
                files += string(" ");
            }
            files += string(entry->d_name) + string("\n");
            list_dir(path, level + 1, files);
        }
        else{
            //printf("%*s- %s\n", level*2, "", entry->d_name);
            for(int i=0; i<level*2; i++){
                files += string(" ");
            }
            files += string("- ") + string(entry->d_name) + string("\n");
        }
    } while (entry = readdir(dir));
    closedir(dir);
}


void RosGui::on_button_buffer1(){
    m_refTextBuffer1 = Gtk::TextBuffer::create();
    const char * lat_type = "Lattice Files\n";
    //std::string lat_files = read_dir(lat_path, lat_type);
    std::string lat_files(lat_type);
    list_dir(lat_path, 0, lat_files);
    const Glib::ustring lat(lat_files);
    m_refTextBuffer1->set_text(lat);
    m_TextView.set_buffer(m_refTextBuffer1);
}

void RosGui::on_button_buffer2(){
    m_refTextBuffer2 = Gtk::TextBuffer::create();
    const char * lau_type = "Launch Files\n";
    std::string lau_files(lau_type);
    list_dir(lau_path, 0, lau_files);
    const Glib::ustring lau(lau_files);
    m_refTextBuffer2->set_text(lau);
    m_TextView.set_buffer(m_refTextBuffer2);
}

void RosGui::on_button_buffer3(){
    m_refTextBuffer3 = Gtk::TextBuffer::create();
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    const char * logdir = "/.ros/log/";
    std::string logpath = string(homedir) + string(logdir);
    const char * log_type = "Ros Log Files\n";
    std::string log_files(log_type);
    list_dir(logpath.c_str(), 0, log_files);
    const Glib::ustring log(log_files);
    m_refTextBuffer3->set_text(log);
    m_TextView.set_buffer(m_refTextBuffer3);
}

void RosGui::on_button_file_clicked(){
    Gtk::FileChooserDialog dialog("Please choose a file",
        Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:

    Glib::RefPtr<Gtk::FileFilter> filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog.add_filter(filter_text);

    Glib::RefPtr<Gtk::FileFilter> filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog.add_filter(filter_cpp);

    Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch(result){
        case(Gtk::RESPONSE_OK):{
            std::cout << "Open clicked." << std::endl;

            //Notice that this is a std::string, not a Glib::ustring.
            std::string filename = dialog.get_filename();
            std::cout << "File selected: " <<  filename << std::endl;
            string openfile = string("gedit ") + filename;
            const int n = openfile.length()+1;
            char cmd[n];
            strncpy(cmd, openfile.c_str(), n-1);
            cmd[n-1] = 0;
            int o = system(cmd);
            break;
        }
        case(Gtk::RESPONSE_CANCEL):{
            std::cout << "Cancel clicked." << std::endl;
            break;
        }
        default:{
            std::cout << "Unexpected button clicked." << std::endl;
            break;
        }
    }
}

void RosGui::on_checkbutton_savepose(){
    savepose = m_CheckButton_Savepose.get_active();
}

void RosGui::on_checkbutton_setpose(){
    setpose = m_CheckButton_Setpose.get_active();
}

void RosGui::on_spinbutton_robotsnum_changed(){
    robotsnum = m_SpinButton_Robotsnum.get_value_as_int();
}

void RosGui::on_spinbutton_countdown_changed(){
    countdown = m_SpinButton_Countdown.get_value_as_int();
}

void RosGui::on_spinbutton_minx_changed(){
    minx = m_SpinButton_MinX.get_value();
}

void RosGui::on_spinbutton_miny_changed(){
    miny = m_SpinButton_MinY.get_value();
}

void RosGui::on_spinbutton_maxx_changed(){
    maxx = m_SpinButton_MaxX.get_value();
}

void RosGui::on_spinbutton_maxy_changed(){
    maxy = m_SpinButton_MaxY.get_value();
}

string RosGui::bool_to_string(bool b){
    return b? string("true") : string("false");
}

string RosGui::double_to_string(double n){
    stringstream ss;
    ss<<n;
    return ss.str();
}

void RosGui::on_button_generate(){
    string lat_type;
    on_combo_robotsposes_changed();
    on_combo_lattice_changed();
    int dot_pos = lattice_name.length()-1;
    for(int i=0; i<lattice_name.length(); i++){
        if(lattice_name[i]=='.') {dot_pos = i;}
        if(i<dot_pos) lat_type.push_back(lattice_name[i]);
    }
    stringstream ss;
    ss<<robotsnum;
    string fname = string("test_")+lat_type+string("_")+ss.str()+string("bots.launch");
    LaunchFile lf(fname);
    // create parameters
    std::vector<LaunchParam> lparams;
    LaunchParam lp_countdown = LaunchParam("count_down", "double", double_to_string(countdown));
    lparams.push_back(lp_countdown);
    LaunchParam lp_sensing = LaunchParam("sensing_range", "double", "50");
    lparams.push_back(lp_sensing);
    LaunchParam lp_max_vel = LaunchParam("max_linear_vel", "double", "1");
    lparams.push_back(lp_max_vel);
    LaunchParam lp_save_pose = LaunchParam("save_pose", "bool", bool_to_string(savepose));
    lparams.push_back(lp_save_pose);
    string lg_val = string("$(find controller)lattice/") + string(lattice_name);
    LaunchParam lp_lg = LaunchParam("latticegraph", "str", lg_val);
    lparams.push_back(lp_lg);
    lparams.push_back(LaunchParam("robots_num", "int", ss.str()));
    std::vector<LaunchNode> lnodes;
    // create simulator node
    LaunchNode ln_simu;
    ln_simu.ntype="simulator";
    ln_simu.pkg = "simulator";
    ln_simu.name = "simu";
    ln_simu.params.push_back(LaunchParam("env_x1", "double", double_to_string(minx)));
    ln_simu.params.push_back(LaunchParam("env_y1", "double", double_to_string(miny)));
    ln_simu.params.push_back(LaunchParam("env_x2", "double", double_to_string(maxx)));
    ln_simu.params.push_back(LaunchParam("env_y2", "double", double_to_string(maxy)));
    ln_simu.params.push_back(LaunchParam("set_pose", "bool", bool_to_string(setpose)));
    on_combo_robotsposes_changed();
    if(pose_id==2){
        // manually enter pose
        for(int i=0; i<robotsnum;i++){
            stringstream ss_i;
            ss_i << i+1;
            string x = string("X")+ss_i.str();
            string y = string("Y")+ss_i.str();
            string theta = string("Theta")+ss_i.str();
            ln_simu.params.push_back(LaunchParam(x, "double", all_poses[i].x));
            ln_simu.params.push_back(LaunchParam(y, "double", all_poses[i].y));
            ln_simu.params.push_back(LaunchParam(theta, "double", all_poses[i].theta));
        }
    }
    if(pose_id == 3){
        // load poses
    }
    lnodes.push_back(ln_simu);
    // create nodes of controllers
    for(int i=0; i<robotsnum; i++){
        LaunchNode ln;
        ln.ntype="controller";
        ln.pkg="controller";
        stringstream ss;
        ss<<i;
        ln.name=string("robot")+ss.str();
        ln.params.push_back(LaunchParam("robotid", "int", ss.str()));
        lnodes.push_back(ln);
    }
    
    lf.create(lnodes, lparams);
    //const char * lau_path = "src/controller/launch/";
    lf.write(lau_path);
    cout<<"Generated launch file in "<<lau_path<<endl;
    // for(int i=0; i<robotsnum; i++){
    //     cout<<all_poses[i].x<<" "<<all_poses[i].y<<" "<<all_poses[i].theta<<endl;
    // }
}

void RosGui::set_combo_robotsposes(){
    //Create the Tree model:
    //m_refTreeModel = Gtk::TreeStore::create(m_Columns);
    m_refTreeModel = Gtk::ListStore::create(m_Columns_Robotsposes);
    m_Combo_Robotsposes.set_model(m_refTreeModel);

    //Set the ComboBox's Tree Model:
    Gtk::TreeModel::Row row = *(m_refTreeModel->append());
    row[m_Columns_Robotsposes.m_col_id] = 1;
    row[m_Columns_Robotsposes.m_col_name] = "Generate Poses Randomly";
    m_Combo_Robotsposes.set_active(row);

    row = *(m_refTreeModel->append());
    row[m_Columns_Robotsposes.m_col_id] = 2;
    row[m_Columns_Robotsposes.m_col_name] = "Manually Assign Poses";

    row = *(m_refTreeModel->append());
    row[m_Columns_Robotsposes.m_col_id] = 3;
    row[m_Columns_Robotsposes.m_col_name] = "Load Saved Poses";

    //m_Combo_Robotsposes.pack_start(m_Columns.m_col_id);
    m_Combo_Robotsposes.pack_start(m_Columns_Robotsposes.m_col_name);
}

void RosGui::on_combo_robotsposes_changed(){
    Gtk::TreeModel::iterator iter = m_Combo_Robotsposes.get_active();
    if(iter){
        Gtk::TreeModel::Row row = *iter;
        if(row){
            //Get the data for the selected row, using our knowledge of the tree
            //model:
            // int
            pose_id = row[m_Columns_Robotsposes.m_col_id];
            Glib::ustring name = row[m_Columns_Robotsposes.m_col_name];
            //std::cout << " ID=" << id << ", name=" << name << std::endl;
        }
    }else{
        std::cout << "invalid iter" << std::endl;
    }
}
void RosGui::set_combo_lattice(){
    m_refTreeModel_Lattice = Gtk::ListStore::create(m_Columns_Lattice);
    m_Combo_Lattice.set_model(m_refTreeModel_Lattice);

    //Set the ComboBox's Tree Model:
    Gtk::TreeModel::Row row = *(m_refTreeModel_Lattice->append());
    if (lattice_files.size()>0) {
        row[m_Columns_Lattice.m_col_name]  = Glib::ustring(lattice_files[0]);
    }
    m_Combo_Lattice.set_active(row);
    for(int i=1; i< lattice_files.size(); i++){
        row = *(m_refTreeModel_Lattice->append());
        row[m_Columns_Lattice.m_col_name]  = Glib::ustring(lattice_files[i]);
    }
    //m_Combo_Robotsposes.pack_start(m_Columns.m_col_id);
    m_Combo_Lattice.pack_start(m_Columns_Lattice.m_col_name);
}

void RosGui::on_combo_lattice_changed(){
    Gtk::TreeModel::iterator iter = m_Combo_Lattice.get_active();
    if(iter){
        Gtk::TreeModel::Row row = *iter;
        if(row){
            //Get the data for the selected row, using our knowledge of the tree
            //model:
            lattice_name = row[m_Columns_Lattice.m_col_name];
            //std::cout << name << std::endl;
        }
    }else{
        std::cout << "invalid iter" << std::endl;
    }
}

void RosGui::set_frame_generator(){
    ///////////////////////////////////////////////////
    // m_VBox_Panel.pack_start(m_VBox_Main);
    m_VBox_Main.set_border_width(10);
    m_Table.attach(m_VBox_Main,0,2,11,16);

    //Generator:
    m_VBox_Main.pack_start(m_Frame_Generator);

    m_VBox_Generator.set_border_width(5);
    m_Frame_Generator.add(m_VBox_Generator);
    /*-------------------- two vertical boxes in a horizontal box --------------------*/
    m_VBox_Generator.pack_start(m_HBox_Generator, Gtk::PACK_EXPAND_WIDGET, 5);
    m_HBox_Generator.pack_start(m_VBox_Countdown, Gtk::PACK_EXPAND_WIDGET, 5);

    m_Label_Countdown.set_alignment(Gtk::ALIGN_START);
    m_VBox_Countdown.pack_start(m_Label_Countdown);

    m_VBox_Countdown.pack_start(m_SpinButton_Countdown);
    m_adjustment_countdown->signal_value_changed().connect( sigc::mem_fun(*this,
            &RosGui::on_spinbutton_countdown_changed) );
    
    m_HBox_Generator.pack_start(m_VBox_Robotsnum, Gtk::PACK_EXPAND_WIDGET, 5);
    m_Label_Robotsnum.set_alignment(Gtk::ALIGN_START);
    m_VBox_Robotsnum.pack_start(m_Label_Robotsnum);
    
    m_adjustment_robotsnum->signal_value_changed().connect( sigc::mem_fun(*this,
            &RosGui::on_spinbutton_robotsnum_changed) );

    m_VBox_Robotsnum.pack_start(m_SpinButton_Robotsnum);

    /*-------------------- Four vertical boxes in a horizontal box --------------------*/
    m_VBox_Generator.pack_start(m_Frame_Environment);
    m_Frame_Environment.add(m_VBox_Environment);
    m_VBox_Environment.pack_start(m_HBox_Generator1, Gtk::PACK_EXPAND_WIDGET, 5);
    
    m_HBox_Generator1.pack_start(m_VBox_MinX, Gtk::PACK_EXPAND_WIDGET, 5);
    m_Label_MinX.set_alignment(Gtk::ALIGN_START);
    m_VBox_MinX.pack_start(m_Label_MinX);
    m_adjustment_minx->signal_value_changed().connect( sigc::mem_fun(*this,
            &RosGui::on_spinbutton_minx_changed) );
    m_VBox_MinX.pack_start(m_SpinButton_MinX);
        
    m_HBox_Generator1.pack_start(m_VBox_MinY, Gtk::PACK_EXPAND_WIDGET, 5);
    m_Label_MinY.set_alignment(Gtk::ALIGN_START);
    m_VBox_MinY.pack_start(m_Label_MinY);  
    m_adjustment_miny->signal_value_changed().connect( sigc::mem_fun(*this,
            &RosGui::on_spinbutton_miny_changed) );
    m_VBox_MinY.pack_start(m_SpinButton_MinY);
    
    m_HBox_Generator1.pack_start(m_VBox_MaxX, Gtk::PACK_EXPAND_WIDGET, 5);
    m_Label_MaxX.set_alignment(Gtk::ALIGN_START);
    m_VBox_MaxX.pack_start(m_Label_MaxX);
    m_adjustment_maxx->signal_value_changed().connect( sigc::mem_fun(*this,
            &RosGui::on_spinbutton_maxx_changed) );
    m_VBox_MaxX.pack_start(m_SpinButton_MaxX);
    
    m_HBox_Generator1.pack_start(m_VBox_MaxY, Gtk::PACK_EXPAND_WIDGET, 5);
    m_Label_MaxY.set_alignment(Gtk::ALIGN_START);
    m_VBox_MaxY.pack_start(m_Label_MaxY);
    m_adjustment_maxy->signal_value_changed().connect( sigc::mem_fun(*this,
            &RosGui::on_spinbutton_maxy_changed) );
    m_VBox_MaxY.pack_start(m_SpinButton_MaxY);
    /*-------------------- two combo windows in a horizontal box -------------------*/
    m_VBox_Generator.pack_start(m_HBox_Generator2, Gtk::PACK_EXPAND_WIDGET, 5);

    m_HBox_Generator2.pack_start(m_VBox_Robotsposes, Gtk::PACK_EXPAND_WIDGET, 5);
    m_Label_Robotsposes.set_alignment(Gtk::ALIGN_START);
    m_VBox_Robotsposes.pack_start(m_Label_Robotsposes);
  
    m_HBox_Generator2.pack_start(m_VBox_Lattice, Gtk::PACK_EXPAND_WIDGET, 5);
    m_Label_Lattice.set_alignment(Gtk::ALIGN_START);
    m_VBox_Lattice.pack_start(m_Label_Lattice);
  
    set_combo_robotsposes();
    //Connect signal handler:
    m_Combo_Robotsposes.signal_changed().connect( sigc::mem_fun(*this,
            &RosGui::on_combo_robotsposes_changed) );
    m_VBox_Robotsposes.pack_start(m_Combo_Robotsposes);
    
    set_combo_lattice();
    //Connect signal handler:
    m_Combo_Lattice.signal_changed().connect( sigc::mem_fun(*this,
            &RosGui::on_combo_lattice_changed) );
    m_VBox_Lattice.pack_start(m_Combo_Lattice);
    /*----------------------- two check buttons ------------------------*/
    //CheckButtons:
    m_VBox_Generator.pack_start(m_CheckButton_Savepose);
    m_CheckButton_Savepose.set_active(false);
    m_CheckButton_Savepose.signal_clicked().connect( sigc::mem_fun(*this,
            &RosGui::on_checkbutton_savepose) );

    m_VBox_Generator.pack_start(m_CheckButton_Setpose);
    m_CheckButton_Setpose.set_active(false);
    m_CheckButton_Setpose.signal_clicked().connect( sigc::mem_fun(*this,
            &RosGui::on_checkbutton_setpose) );

    //Buttons:
    m_VBox_Generator.pack_start (m_HBox_Buttons, Gtk::PACK_SHRINK, 5);
    m_Button_Enter.signal_clicked().connect( sigc::mem_fun(*this,
            &RosGui::on_button_enter) );
    m_HBox_Buttons.pack_start(m_Button_Enter, Gtk::PACK_EXPAND_WIDGET, 5);
    
    m_HBox_Buttons.pack_start(m_Button_Generate);
    m_Button_Generate.signal_clicked().connect( sigc::mem_fun(*this,
            &RosGui::on_button_generate) );
}


void RosGui::on_button_enter(){
    ptrWindowEnter = make_shared<PopWindow>(robotsnum, ptrPoses);
    ptrWindowEnter->set_title("Enter Poses");
    ptrWindowEnter->show();
}

void RosGui::populateCompletion(){
    vector<string> lau_files = read_dir(lau_path);
    Gtk::TreeModel::Row row;
    for(string s : lau_files){
        row =*(m_refCompletionModel->append());
        row[m_completionRecord.col_text] = s;
    }
 
}