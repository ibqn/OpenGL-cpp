#include "openglarea.hpp"

#include <gtkmm/drawingarea.h>
#include <gdk/gdkx.h>

#include <memory>
#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <GL/glx.h>

namespace Evg {
  class XVisualInfoDeleter {
  public:
    void operator () (XVisualInfo *visual) const {
      XFree (visual);
    }
  };

  OpenGLArea::OpenGLArea () {
    set_double_buffered (false);

    xdisplay = nullptr;
  }

  void OpenGLArea::on_realize () {
    Gtk::DrawingArea::on_realize ();

    on_glx_init ();

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit () != GLEW_OK) {
      throw std::runtime_error ("Failed to initialize GLEW");
    }

    std::cout << "Vendor: ........... " << glGetString (GL_VENDOR) << std::endl;
    std::cout << "Renderer: ......... " << glGetString (GL_RENDERER) << std::endl;
    std::cout << "Version: .......... " << glGetString (GL_VERSION) << std::endl;
    std::cout << "GLSL version: ..... " << glGetString (GL_SHADING_LANGUAGE_VERSION) << std::endl;

    on_gl_init ();
  }

  void OpenGLArea::on_glx_init () {
    GLint glx_attrs[] = {
      GLX_RGBA,
      GLX_RED_SIZE        , 8,
      GLX_GREEN_SIZE      , 8,
      GLX_BLUE_SIZE       , 8,
      GLX_ALPHA_SIZE      , 8,
      GLX_DEPTH_SIZE      , 24,
      GLX_STENCIL_SIZE    , 8,
      GLX_DOUBLEBUFFER    , False,
      None,
    };

    xdisplay = GDK_WINDOW_XDISPLAY (Glib::unwrap (get_window ()));
    drawable = GDK_WINDOW_XID (Glib::unwrap (get_window ()));
    const std::unique_ptr<XVisualInfo, Evg::XVisualInfoDeleter> visual_info (glXChooseVisual (xdisplay, DefaultScreen (xdisplay), glx_attrs));

    if (!visual_info) {
      throw std::runtime_error ("failed glXChooseVisual.");
    }

    context = glXCreateContext (xdisplay, visual_info.get (), nullptr, True);
    if (!context) {
      throw std::runtime_error ("failed glXCreateContext.");
    }

    if (!glXMakeCurrent (xdisplay, drawable, context)) {
      throw std::runtime_error ("failed glXMakeCurrent.");
    }
  }

  void OpenGLArea::on_gl_init () {
  }

  void OpenGLArea::on_gl_draw () {
  }

  bool OpenGLArea::on_configure_event (GdkEventConfigure *event) {
    if (xdisplay != nullptr && !glXMakeCurrent (xdisplay, drawable, context))
      return false;
    std::cout << "on configure" << std::endl;

    GLsizei width = get_allocated_width ();
    GLsizei height = get_allocated_height ();
    GLsizei size = std::min (width, height);

    glViewport ((width - size) / 2, (height - size) / 2, size, size);

    return true;
  }

  bool OpenGLArea::on_draw (const Cairo::RefPtr<Cairo::Context> &cr) {
    if (!glXMakeCurrent (xdisplay, drawable, context))
      return false;

    on_gl_draw ();
    //std::cout << "on draw" << std::endl;

    glXSwapBuffers (xdisplay, drawable);

    return true;
  }
}
