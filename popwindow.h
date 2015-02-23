#ifndef POP_WINDOW_H
#define POP_WINDOW_H

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/table.h>
#include <math.h>
#include <memory>
#include <sstream>
#include <iostream>
#include <map>
#include "pose.h"

class PoseFrame : public Gtk::Window {
public:
    PoseFrame(std::string );
    virtual ~PoseFrame();

    std::string id;
    Gtk::Frame m_Frame_Pose;
    Gtk::Entry m_Entry_X, m_Entry_Y, m_Entry_Theta;
    /* Gtk::SpinButton m_SpinButton_X, m_SpinButton_Y, m_SpinButton_Theta; */
    /* Glib::RefPtr<Gtk::Adjustment> m_adjustment_x, m_adjustment_y, m_adjustment_theta; */
    /* Gtk::Box  m_VBox_Pose;//, m_HBox_Pose; */
    Gtk::ButtonBox m_HBox_Pose;
    Gtk::Label m_Label_X, m_Label_Y, m_Label_Theta;
    Pose get_pose();
};

class PopWindow : public Gtk::Window{
    public:
    PopWindow(int , std::map<int, Pose> *&);
        virtual ~PopWindow();
        std::map<int, Pose> * ptr_all_poses;
    protected:
        int robotsnum;
        std::map<int, std::shared_ptr<PoseFrame> > ptrPoseFrames;
        Gtk::Table m_Table;
        Gtk::Grid m_Grid;
        Gtk::Frame m_Frame_Main;
        Gtk::Box m_VBox_Main, m_VBox_Sub, m_VBox_Footer;
        Gtk::ButtonBox m_ButtonBox;
        Gtk::ScrolledWindow m_ScrolledWindow;
        Gtk::Button m_Button_Save, m_Button_Close;
        void on_button_close();
        void on_button_save();
};

#endif 