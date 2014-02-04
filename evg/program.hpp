#ifndef _EVG_PROGRAM_
#define _EVG_PROGRAM_

#include "shader.hpp"
#include "program.hpp"

#include <GL/glew.h>

namespace Evg {
  class Program {
  private:
    bool linked;
    GLuint id;

  public:	
    Program ();
    Program (const std::string &vertex_path, const std::string &fragment_path);
    ~Program ();

    GLuint get_id () const;
    bool create ();
    bool attach_shader (const Evg::Shader &shader);			
    bool is_linked () const;
    bool is_created () const;
    bool link ();
    void use () const;
    void remove ();
  };
}

#endif
