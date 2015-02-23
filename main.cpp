#include "rosgui.h"
#include <gtkmm.h>

int main(int argc, char *argv[])
{
  Glib::RefPtr<Gtk::Application> app =
    Gtk::Application::create(argc, argv,
      "org.gtkmm.examples.base");

  RosGui ros_gui;
  
  return app->run(ros_gui);
}