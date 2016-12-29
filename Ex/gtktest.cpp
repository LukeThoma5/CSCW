#include <gtkmm.h>

int main(int argc, char *argv[])
{
  auto app =
    Gtk::Application::create(argc, argv,
      "org.gtkmm.examples.base");

  Gtk::Window window;

  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("guitest.glade");

  window.set_default_size(200, 200);

  return app->run(builder);
}