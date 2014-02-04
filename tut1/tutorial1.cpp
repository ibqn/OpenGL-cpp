#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/main.h>

#include <GL/glew.h>
#include <GL/glx.h>

#include "evg/openglarea.hpp"

class GLArea : public Evg::OpenGLArea {
public:
  GLArea () {
    set_size_request (300, 300);
  }

protected:
  void on_gl_draw () {
    glClear (GL_COLOR_BUFFER_BIT);

    glBegin (GL_TRIANGLES);
    glIndexi (0);
    glColor3f (1.0f, 0.0f, 0.0f);
    glVertex2i (0, 1);
    glIndexi (0);
    glColor3f (0.0f, 1.0f, 0.0f);
    glVertex2i (-1, -1);
    glIndexi (0);
    glColor3f (0.0f, 0.0f, 1.0f);
    glVertex2i (1, -1);
    glEnd ();
  }
};

class GLWindow : public Gtk::Window {
public:
  GLWindow () {
    vbox = new Gtk::VBox;
    button = new Gtk::Button ("quit");
    drawing_area = new GLArea;

    vbox->pack_start (*drawing_area, true, true);
    vbox->pack_start (*button, false, false);

    button->signal_clicked ().connect ([] () { Gtk::Main::quit (); });

    add (*vbox);
  }

private:
  Gtk::Button *button;
  Gtk::VBox *vbox;
  GLArea *drawing_area;
};

int main () {
  Gtk::Main kit;

  GLWindow window;
  window.show_all ();

  kit.run (window);
}
