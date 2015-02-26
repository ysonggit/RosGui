#ifndef ROSGUI_H
#define ROSGUI_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/table.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>
#include <gtkmm.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <glibmm/ustring.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstring>
#include <pwd.h>
#include <cstdio>
#include "launchfile.h"
#include "popwindow.h"
#include <memory>
#include "pose.h"
#include <map>

class CompletionRecord : public Gtk::TreeModel::ColumnRecord{
public:
    CompletionRecord(){
        add(col_text);
    }
    Gtk::TreeModelColumn<Glib::ustring> col_text;
};

//Tree model columns:
class ModelColumns : public Gtk::TreeModel::ColumnRecord{
public:
    ModelColumns(){ add(m_col_id); add(m_col_name); }
    Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
};


class RosGui : public Gtk::Window{
public:
    RosGui();
    virtual ~RosGui();

protected:
    static const char * lat_path;
    static const char * lau_path;
    std::string working_dir;
    std::string getWorkingDir();
    //Signal handlers:
    void on_roscore_btn_clicked();
    void on_rviz_btn_clicked();
    void on_clean_btn_clicked();
    void on_quit_btn_clicked();
    void on_make_btn_clicked();
    void on_launch_btn_clicked();
    void on_rosrun_btn_clicked();
    
    //Member widgets:
    Gtk::Table m_Table;
    Gtk::Box m_BBox;
    Gtk::ButtonBox utility_Box;
    Gtk::Button roscore_btn, rviz_btn, clean_btn,
        make_btn, launch_btn, rosrun_btn;
    Gtk::Button quit_btn;
    Gtk::Entry launch_entry, lg_entry, rospack_entry, rosnode_entry;
    Gtk::Box m_EBox, m_RBox;
    Gtk::ButtonBox launch_Box, run_Box;

    void set_box_utilities();
    void set_box_roslaunch_rosrun();
    // textview and textbuffer
    void on_button_buffer1();
    void on_button_buffer2();
    void on_button_buffer3();
    Gtk::Box m_VBox;
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::TextView m_TextView;

    std::vector<std::string> lattice_files;
    std::vector<std::string> read_dir(const char * path);
    void list_dir(const char * path, int level, std::string & files);
    Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1, m_refTextBuffer2, m_refTextBuffer3;
    Gtk::ButtonBox m_ButtonBox;
    Gtk::Button m_Button_Buffer1, m_Button_Buffer2, m_Button_Buffer3;
    // folder dialog
    void on_button_file_clicked();
    Gtk::Button m_Button_File;
    void set_textview_buffer();
   
    // frame
    int robotsnum;
    int countdown;
    int timeout;
    int pose_id; // id of selecting methods to initialize poses
    Glib::ustring lattice_name;
    bool savepose, setpose;
    std::string bool_to_string(bool );
    std::string double_to_string(double );
    double minx, miny, maxx, maxy;
    void on_checkbutton_savepose();
    void on_checkbutton_setpose();
    void on_spinbutton_robotsnum_changed();
    void on_spinbutton_countdown_changed();
    void on_button_generate();
    void on_spinbutton_minx_changed();
    void on_spinbutton_miny_changed();
    void on_spinbutton_maxx_changed();
    void on_spinbutton_maxy_changed();
    //Child widgets:
    Gtk::Frame m_Frame_Generator, m_Frame_Environment;
    Gtk::Box m_HBox_Generator,  m_HBox_Generator1,
        m_HBox_Generator2, m_HBox_Buttons;
    Gtk::Box m_VBox_Main, m_VBox_Generator, m_VBox_Environment, m_VBox_Countdown,
        m_VBox_Robotsnum, m_VBox_Lattice, m_VBox_Robotsposes,
        m_VBox_MinX, m_VBox_MinY, m_VBox_MaxX, m_VBox_MaxY;
    Gtk::Label m_Label_Countdown, m_Label_Robotsnum,
        m_Label_ShowCountdown, m_Label_Lattice, m_Label_Robotsposes,
        m_Label_MinX, m_Label_MinY, m_Label_MaxX, m_Label_MaxY;
    Glib::RefPtr<Gtk::Adjustment> m_adjustment_countdown, m_adjustment_robotsnum,
        m_adjustment_minx, m_adjustment_miny,
        m_adjustment_maxx, m_adjustment_maxy;
    Gtk::SpinButton m_SpinButton_Countdown, m_SpinButton_Robotsnum,
        m_SpinButton_MinX, m_SpinButton_MinY,
        m_SpinButton_MaxX, m_SpinButton_MaxY;
    Gtk::CheckButton m_CheckButton_Savepose, m_CheckButton_Setpose;
    Gtk::Button m_Button_Enter, m_Button_Generate;

    void set_combo_robotsposes();
    void set_combo_lattice();
    //Signal handlers:
    void on_combo_robotsposes_changed();
    void on_combo_lattice_changed();

    void set_frame_generator();
    ModelColumns m_Columns_Robotsposes, m_Columns_Lattice;

    //Child widgets:
    Gtk::ComboBox m_Combo_Lattice, m_Combo_Robotsposes;
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel_Lattice;

    // popup window for entering poses
    shared_ptr<PopWindow>  ptrWindowEnter;
    void on_button_enter();
    std::map<int, Pose>  all_poses;
    std::map<int, Pose> * ptrPoses;
    Gtk::Box m_VBox_Panel;

    // entry completion
    //Gtk::Entry m_Entry;
    CompletionRecord m_completionRecord;
    Glib::RefPtr<Gtk::EntryCompletion> m_refEntryCompletion;
    Glib::RefPtr<Gtk::ListStore> m_refCompletionModel;
    void populateCompletion();
};


#endif // ROSGUI_H
