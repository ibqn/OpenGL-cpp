#ifndef _EVG_SHADER_
#define _EVG_SHADER_

#include <string>

#include <GL/glew.h>

namespace Evg {
  enum class ShaderType : GLenum {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
  };
  
  class Shader {
  private:
    bool loaded;
    GLenum type;
    GLuint id;

  public: 
    Shader (Evg::ShaderType type);
    Shader (Evg::ShaderType type, const std::string &path);
    ~Shader ();

    bool load_from_string (const std::string &shader);
    bool load_from_file (const std::string &path);
    bool is_loaded () const;
    GLuint get_id () const;
    void remove ();
  };
}

#endif
