#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "program.hpp"

namespace Evg {		
  Program::Program () {
    this->id = 0;
    this->linked = false;

    this->create ();
  }

  Program::Program (const std::string &vertex_path, const std::string &fragment_path) : Program () {
    Evg::Shader vertex_shader (Evg::ShaderType::Vertex, vertex_path);
    Evg::Shader fragment_shader (Evg::ShaderType::Fragment, fragment_path);

    this->attach_shader (vertex_shader);
    this->attach_shader (fragment_shader);
    
    this->link ();
  }

  GLuint Program::get_id () const {
    return this->id;
  }

  bool Program::create () {
    this->id = glCreateProgram ();
    return (this->id != 0);
  }

  bool Program::attach_shader (const Evg::Shader &shader) {
    if (!shader.is_loaded ())
      return false;

    glAttachShader (this->id, shader.get_id ());
    return true;
  }
			
  bool Program::is_linked () const {
    return this->linked;
  }

  bool Program::is_created () const {
    return (this->id != 0);
  }

  bool Program::link () {
    glLinkProgram (this->id);

    GLint status = GL_FALSE;
    glGetProgramiv (this->id, GL_LINK_STATUS, &status);
    this->linked = (status == GL_TRUE);

    if (!this->linked) {
      GLsizei info_log_length;
      glGetShaderiv (this->id, GL_INFO_LOG_LENGTH, &info_log_length);

      std::vector<char> error_message (info_log_length);
      glGetShaderInfoLog(this->id, info_log_length, nullptr, error_message.data ());
      std::cout << "program info log: '" << error_message.data () << "'" << std::endl;
    }

    return this->linked;
}

  void Program::use () const {
    if (this->linked);
      glUseProgram (this->id);
  }

  void Program::remove () {
    if (!this->linked)
      return;

    this->linked = false;
    glDeleteProgram (this->id);
    this->id = 0;
  }

  Program::~Program () {
    this->remove ();
  }
}


