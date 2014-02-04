#include "openglarea3.hpp"

#include <gtkmm/drawingarea.h>
#include <gdk/gdkx.h>

#include <memory>
#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <GL/glx.h>

namespace Evg {
  class GLXFBConfigDeleter {
  public:
    void operator () (GLXFBConfig *fbc) const {
      XFree (fbc);
    }
  };

  typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

  OpenGLArea3::OpenGLArea3 () {
    set_double_buffered (false);

    xdisplay = nullptr;
  }

  void OpenGLArea3::on_realize () {
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

  void OpenGLArea3::on_glx_init () {
    GLint glx_attrs[] = {
      GLX_X_RENDERABLE    , True,
      GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
      GLX_RENDER_TYPE     , GLX_RGBA_BIT,
      GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
      GLX_RGBA,
      GLX_RED_SIZE        , 8,
      GLX_GREEN_SIZE      , 8,
      GLX_BLUE_SIZE       , 8,
      GLX_ALPHA_SIZE      , 8,
      GLX_DEPTH_SIZE      , 24,
      GLX_STENCIL_SIZE    , 8,
      GLX_DOUBLEBUFFER    , False,
      GLX_SAMPLE_BUFFERS  , 1,
      GLX_SAMPLES         , 4,
      None,
    };

    xdisplay = GDK_WINDOW_XDISPLAY (Glib::unwrap (get_window ()));
    drawable = GDK_WINDOW_XID (Glib::unwrap (get_window ()));

    int fbcount;
    const std::unique_ptr<GLXFBConfig, Evg::GLXFBConfigDeleter> fbc (glXChooseFBConfig (xdisplay, DefaultScreen (xdisplay), glx_attrs, &fbcount));

    if (!fbc) {
      throw std::runtime_error ("failed glXChooseFBConfig.");
    }

    int gl3_attr[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
      GLX_CONTEXT_MINOR_VERSION_ARB, 3,
      GLX_CONTEXT_FLAGS_ARB,
      GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
      None
    };

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB ((const GLubyte *)"glXCreateContextAttribsARB");

    context = glXCreateContextAttribsARB (xdisplay, (fbc.get ())[0], nullptr, True, gl3_attr);

    if (!context) {
      throw std::runtime_error ("failed glXCreateContextAttribsARB.");
    }

    if (!glXMakeContextCurrent (xdisplay, drawable, drawable, context)) {
      throw std::runtime_error ("failed glXMakeContextCurrent.");
    }
  }

  void OpenGLArea3::on_gl_init () {
  }

  void OpenGLArea3::on_gl_draw () {
  }

  bool OpenGLArea3::on_configure_event (GdkEventConfigure *event) {
    if (xdisplay != nullptr && !glXMakeContextCurrent (xdisplay, drawable, drawable, context))
      return false;

    GLsizei width = get_allocated_width ();
    GLsizei height = get_allocated_height ();
    GLsizei size = std::min (width, height);

    glViewport ((width - size) / 2, (height - size) / 2, size, size);
    
    glXWaitX ();

    return true;
  }

  bool OpenGLArea3::on_draw (const Cairo::RefPtr<Cairo::Context> &cr) {
    if (xdisplay != nullptr && !glXMakeContextCurrent (xdisplay, drawable, drawable, context)) 
      return false;

    on_gl_draw ();

    glXWaitGL ();

    glXSwapBuffers (xdisplay, drawable);

    return true;
  }
}
