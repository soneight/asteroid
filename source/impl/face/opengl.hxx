#ifndef SON8_ASTEROID_OPENGL_HXX
#define SON8_ASTEROID_OPENGL_HXX
// NOTE: important that all `opengl` users of application use the same version
// \ so only include current file in other translation units for `opengl` part
#define SON8_OVERGLAD_INCLUDE_DEPRECATED
#include <son8/overglad/v3_3.hxx>
#include <string>

namespace app {
   namespace gl = son8::overglad;
   namespace glt = gl::types;

   enum class Vert_Shaders : unsigned {
      Default,
      Last_,
      Stars, // TODO
   };

   enum class Frag_Shaders: unsigned {
      Default,
      Last_,
      Stars, // TODO
   };

   void shader_source( glt::Shader shader, Vert_Shaders name );
   void shader_source( glt::Shader shader, Frag_Shaders name );

   void shader_compile_errors( glt::Shader shader );
   void program_link_errors( glt::Program program );

}

#endif//SON8_ASTEROID_OPENGL_HXX
