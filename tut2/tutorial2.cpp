#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/main.h>

#include <iostream>

#include <GL/glew.h>
#include <GL/glx.h>

#include "evg/openglarea.hpp"
#include "evg/program.hpp"
#include "evg/shader.hpp"

class Triangle {
private:
  GLuint program_id;
  static const GLfloat vertex_buffer_data[9];
  static const GLfloat color_buffer_data[];
  GLuint vertex_array_id;
  GLuint vertex_buffer;

public: 
  Triangle (GLuint program_id) {
    this->program_id = program_id;

    glGenVertexArrays (1, &vertex_array_id);
    glBindVertexArray (vertex_array_id);

    glGenBuffers (1, &vertex_buffer);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData (GL_ARRAY_BUFFER, sizeof (vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
  }

  ~Triangle () {
    // Cleanup VBO
    glDeleteBuffers (1, &vertex_buffer);
    glDeleteVertexArrays (1, &vertex_array_id);
  }

  void draw () {
    GLint position_id = glGetAttribLocation (this->program_id, "vPosition");
    glEnableVertexAttribArray (position_id);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer (position_id, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    GLint color_id = glGetUniformLocation (this->program_id, "vColor");
    glUniform3fv (color_id, 1, color_buffer_data);

    glDrawArrays (GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray (position_id);
  }
};

const GLfloat Triangle::vertex_buffer_data[] = { 
  -1.f, -1.f, 0.f,
  1.f, -1.f, 0.f,
  0.f,  1.f, 0.f,
};

const GLfloat Triangle::color_buffer_data[] = { 0.6f, 0.7f, 0.2f };

class GLArea : public Evg::OpenGLArea {
public:
  GLArea () {
    set_size_request (300, 300);
  }
  
  ~GLArea () {
    delete program;
    delete triangle;
  }

protected:
  void on_gl_init () {
    glClearColor (0.0f, 0.0f, 0.4f, 0.0f);
    
    std::cout << "on GL init" << std::endl;
    
    program = new Evg::Program;
    
    Evg::Shader vertex_shader (Evg::ShaderType::Vertex, "vertex.shader");
    Evg::Shader fragment_shader (Evg::ShaderType::Fragment, "fragment.shader");

    program->attach_shader (vertex_shader);
    program->attach_shader (fragment_shader);
    program->link ();

    triangle = new Triangle (program->get_id ());
  }
  
  void on_gl_draw () {
    std::cout << "on draw" << std::endl;
    glClear (GL_COLOR_BUFFER_BIT);

    program->use ();

    triangle->draw ();
  }
  
private:
  Evg::Program *program;
  Triangle *triangle;
};

class GLWindow : public Gtk::Window {
public:
  GLWindow() {
    vbox = new Gtk::VBox;
    button = new Gtk::Button ("quit");
    drawing_area = new GLArea;

    vbox->pack_start (*drawing_area, true, true);
    vbox->pack_start (*button, false, false);

    button->signal_clicked ().connect ([] () { Gtk::Main::quit (); });

    add(*vbox);
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
