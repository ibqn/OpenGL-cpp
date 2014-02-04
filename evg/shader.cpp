#include <string>
#include <iostream>

#include <giomm/file.h>
#include <giomm/datainputstream.h>

#include "shader.hpp"

#include <GL/glew.h>

namespace Evg {
  Shader::Shader (Evg::ShaderType type) {
    this->loaded = false;
    this->id = 0;
    this->type = (GLenum)type;
  }

  Shader::Shader (Evg::ShaderType type, const std::string &path) : Shader (type) {
    load_from_file (path);
  }

  Shader::~Shader () {
    this->remove ();
  }
		
  bool Shader::load_from_string (const std::string &shader) {
    this->id = glCreateShader (this->type);

    char const *shader_source = shader.c_str ();
    glShaderSource (id, 1, &shader_source, nullptr);
    glCompileShader (id);

    GLint status = GL_FALSE;
    glGetShaderiv (id, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
      GLsizei info_log_length;
      glGetShaderiv (this->id, GL_INFO_LOG_LENGTH, &info_log_length);
    
      std::vector<char> error_message (info_log_length);
      glGetShaderInfoLog(this->id, info_log_length, nullptr, error_message.data ());
      std::cout << "shader info log: '" << error_message.data () << "'" << std::endl;

      return false;
    }

    this->type = type;
    this->loaded = true;

    return true;
  }

  bool Shader::load_from_file (const std::string &path) {
    Glib::RefPtr<Gio::File> file = Gio::File::create_for_path (path);
			
    if (!file->query_exists ()) {
      std::cout << "File '" << file->get_path () << "' doesn't exist." << std::endl;
      return false;
    }

    Glib::RefPtr<Gio::DataInputStream> dis = Gio::DataInputStream::create (file->read ());
    std::stringstream ss;
    std::string line;
  
    while (dis->read_line (line)) {
      ss << line << std::endl;
    }
    
    return this->load_from_string (ss.str ());
  }

  bool Shader::is_loaded () const {
    return this->loaded;
  }

  GLuint Shader::get_id () const {
    return this->id;
  }

  void Shader::remove () {
    if (!this->loaded) return;
    this->loaded = false;
    glDeleteShader (this->id);
  }
}

