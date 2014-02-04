#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/main.h>

#include <glibmm.h>

#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GL/glx.h>

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "evg/openglarea3.hpp"
#include "evg/program.hpp"

class Cube {
private:
  GLuint program_id;
  static const GLfloat vertex_buffer_data[108];
  static const GLfloat color_buffer_data[108];

  GLuint vertex_buffer;
  GLuint color_buffer;

  GLuint vao_id;

  GLint position_id;
  GLint color_id;
  GLint matrix_id;
  
  static const GLfloat PI_4;

  glm::mat4 MVP;

public: 
  Cube (GLuint program_id) {
    this->program_id = program_id;

    glGenVertexArrays (1, &vao_id);
    glBindVertexArray (vao_id);

    position_id = glGetAttribLocation (this->program_id, "vertexPosition");
    color_id = glGetAttribLocation (this->program_id, "vertexColor");
    matrix_id = glGetUniformLocation (this->program_id, "MVP");

    glGenBuffers (1, &vertex_buffer);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData (GL_ARRAY_BUFFER, sizeof (vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers (1, &color_buffer);
    glBindBuffer (GL_ARRAY_BUFFER, color_buffer);
    glBufferData (GL_ARRAY_BUFFER, sizeof (color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray (position_id);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer (position_id, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray (color_id);
    glBindBuffer (GL_ARRAY_BUFFER, color_buffer);
    glVertexAttribPointer (color_id, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glm::mat4 Projection = glm::perspective (PI_4, 1.f, 0.1f, 10.f);
    glm::mat4 View = glm::lookAt (glm::vec3 (4.f, 3.f, 3.f),
				  glm::vec3 (0.f, 0.f, 0.f),
				  glm::vec3 (0.f, 1.f, 0.f));
    glm::mat4 Model = glm::mat4 (1.f);
    MVP = Projection * View * Model;
  }

  void update_MVP () {
    this->MVP = glm::rotate (this->MVP, this->PI_4 / 4.f, glm::vec3 (0.f, 1.f, 0.f));
  }

  ~Cube () {
    // Cleanup VBOs
    glDeleteBuffers (1, &vertex_buffer);
    glDeleteBuffers (1, &color_buffer);

    glDeleteVertexArrays (1, &vao_id);
  }

  void draw () {
    glUniformMatrix4fv (matrix_id, 1, GL_FALSE, glm::value_ptr (MVP));

    glBindVertexArray (vao_id);
    glDrawArrays (GL_TRIANGLES, 0, 36);
  }
};

const GLfloat Cube::PI_4 = std::atan (1.f);

const GLfloat Cube::vertex_buffer_data[] = {
  -1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
  
  1.0f, 1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f,

  1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f,-1.0f,
  1.0f,-1.0f,-1.0f,
  
  1.0f, 1.0f,-1.0f,
  1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,
  
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,
  
  1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f,-1.0f,

  -1.0f, 1.0f, 1.0f,
  -1.0f,-1.0f, 1.0f,
  1.0f,-1.0f, 1.0f,

  1.0f, 1.0f, 1.0f,
  1.0f,-1.0f,-1.0f,
  1.0f, 1.0f,-1.0f,

  1.0f,-1.0f,-1.0f,
  1.0f, 1.0f, 1.0f,
  1.0f,-1.0f, 1.0f,
  
  1.0f, 1.0f, 1.0f,
  1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f,
  
  1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,

  1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f,-1.0f, 1.0f,
};

const GLfloat Cube::color_buffer_data[] = {
  0.583f,  0.771f,  0.014f,
  0.609f,  0.115f,  0.436f,
  0.327f,  0.483f,  0.844f,
  0.822f,  0.569f,  0.201f,
  0.435f,  0.602f,  0.223f,
  0.310f,  0.747f,  0.185f,
  0.597f,  0.770f,  0.761f,
  0.559f,  0.436f,  0.730f,
  0.359f,  0.583f,  0.152f,
  0.483f,  0.596f,  0.789f,
  0.559f,  0.861f,  0.639f,
  0.195f,  0.548f,  0.859f,
  0.014f,  0.184f,  0.576f,
  0.771f,  0.328f,  0.970f,
  0.406f,  0.615f,  0.116f,
  0.676f,  0.977f,  0.133f,
  0.971f,  0.572f,  0.833f,
  0.140f,  0.616f,  0.489f,
  0.997f,  0.513f,  0.064f,
  0.945f,  0.719f,  0.592f,
  0.543f,  0.021f,  0.978f,
  0.279f,  0.317f,  0.505f,
  0.167f,  0.620f,  0.077f,
  0.347f,  0.857f,  0.137f,
  0.055f,  0.953f,  0.042f,
  0.714f,  0.505f,  0.345f,
  0.783f,  0.290f,  0.734f,
  0.722f,  0.645f,  0.174f,
  0.302f,  0.455f,  0.848f,
  0.225f,  0.587f,  0.040f,
  0.517f,  0.713f,  0.338f,
  0.053f,  0.959f,  0.120f,
  0.393f,  0.621f,  0.362f,
  0.673f,  0.211f,  0.457f,
  0.820f,  0.883f,  0.371f,
  0.982f,  0.099f,  0.879f,
};

class GLArea : public Evg::OpenGLArea3 {
public:
  GLArea () {
    set_size_request (300, 300);
  }

  ~GLArea () {
    delete program;
    delete cube;
  }

protected:
  void on_gl_init () {
    glClearColor (0.2f, 0.1f, 0.4f, 0.0f);

    std::cout << "on GL init" << std::endl;

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);

    program = new Evg::Program ("vertex.shader", "fragment.shader");

    cube = new Cube (program->get_id ());

    Glib::signal_timeout ().connect (sigc::mem_fun (*this, &GLArea::on_timeout), 100);
  }

  void on_gl_draw () {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->use ();

    cube->draw ();
  }

private:
  Evg::Program *program;
  Cube *cube;

  bool on_timeout () {
    cube->update_MVP ();

    queue_draw ();
    return true;
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
