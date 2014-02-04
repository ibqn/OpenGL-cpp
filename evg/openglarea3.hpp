#ifndef _EVG_OPEN_GL_AREA_
#define _EVG_OPEN_GL_AREA_

#include <gtkmm/drawingarea.h>

#include <GL/glew.h>
#include <GL/glx.h>

namespace Evg {
  class OpenGLArea3 : public Gtk::DrawingArea {
  public:
    OpenGLArea3 ();

  protected:
    virtual void on_realize ();
    virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &);
    virtual bool on_configure_event (GdkEventConfigure *event);

    virtual void on_gl_init ();
    virtual void on_gl_draw ();

  private:
    void on_glx_init ();

    Display *xdisplay;
    GLXDrawable drawable;
    GLXContext context;
  };
}

#endif
