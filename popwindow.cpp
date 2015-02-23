#include "popwindow.h"
using namespace std;

PoseFrame::PoseFrame(string _id)
    : id(_id),
      m_Label_X("X: "),
      m_Label_Y("y: "),
      m_Label_Theta("theta: ")
{
    string frame_label = string("Robot ")+id+string(" :");
    m_Frame_Pose.set_label(frame_label);
    m_HBox_Pose.set_border_width(5);
    m_Frame_Pose.add(m_HBox_Pose);
    m_HBox_Pose.pack_start(m_Label_X);
    // m_adjustment_x->signal_value_changed().connect( sigc::mem_fun(*this,
    //         &PoseFrame::on_spinbutton_x_changed) );
    m_HBox_Pose.pack_start(m_Entry_X);
    // m_adjustment_y->signal_value_changed().connect( sigc::mem_fun(*this,
    //     &PoseFrame::on_spinbutton_y_changed) );
    m_HBox_Pose.pack_start(m_Label_Y);
    m_HBox_Pose.pack_start(m_Entry_Y);
    m_HBox_Pose.pack_start(m_Label_Theta);
    m_HBox_Pose.pack_start(m_Entry_Theta);
    // m_adjustment_theta->signal_value_changed().connect( sigc::mem_fun(*this,
    //     &PoseFrame::on_spinbutton_theta_changed) );

}

PoseFrame::~PoseFrame(){}

Pose PoseFrame::get_pose(){
    return Pose(m_Entry_X.get_text(), m_Entry_Y.get_text(), m_Entry_Theta.get_text());
}

PopWindow::PopWindow(int n, std::map<int, Pose> *& ptrPoses )
    : robotsnum(n),
      ptr_all_poses(ptrPoses),
      m_Button_Save("Save"),
      m_Button_Close("Close"),
      m_VBox_Main(Gtk::ORIENTATION_VERTICAL),
      m_VBox_Sub(Gtk::ORIENTATION_VERTICAL),
      m_VBox_Footer(Gtk::ORIENTATION_VERTICAL)
{
    set_default_size(300, 400);
    // Gtk::Label *label = Gtk::manage(new Gtk::Label("New Popup Window"));
    // add(*label);
    add(m_Table);
    m_Table.attach(m_VBox_Main, 0, 3, 0, 6);
  
    for(int i=0; i<robotsnum; i++){
        stringstream ss;
        ss<<i;
        ptrPoseFrames[i] = make_shared<PoseFrame>(ss.str());
        m_VBox_Sub.pack_start(ptrPoseFrames[i]->m_Frame_Pose);
    }
    m_ScrolledWindow.add(m_VBox_Sub);
    //Only show the scrollbars when they are necessary:
    m_ScrolledWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    m_VBox_Main.set_border_width(10);
    m_VBox_Main.pack_start(m_ScrolledWindow);

    m_VBox_Footer.set_border_width(20);
    m_VBox_Footer.pack_start(m_ButtonBox);
    m_Button_Save.signal_clicked().connect( sigc::mem_fun(*this,
             &PopWindow::on_button_save) );
    m_ButtonBox.pack_start(m_Button_Save, Gtk::PACK_EXPAND_WIDGET, 5);
    m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
             &PopWindow::on_button_close) );
    m_ButtonBox.pack_start(m_Button_Close, Gtk::PACK_EXPAND_WIDGET, 5);

    m_Table.attach(m_VBox_Footer, 1,2,7,8);
 
    show_all_children();
}

PopWindow::~PopWindow(){
    //dtor
}

void PopWindow::on_button_close(){
    hide();
}

void PopWindow::on_button_save(){
    for(int i=0; i<robotsnum; i++){
        if(ptrPoseFrames.find(i) != ptrPoseFrames.end()){
            (*ptr_all_poses)[i] = ptrPoseFrames[i]->get_pose();
        }else{
            (*ptr_all_poses)[i] = Pose("", "", "");
        }
    }
    // for(int i=0; i<robotsnum; i++){
    //     cout<<all_poses[i].x<<" "<<all_poses[i].y<<" "<<all_poses[i].theta<<endl;
    // }
}