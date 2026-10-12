#ifndef SON8_ASTEROID_OPENGL_HXX
#define SON8_ASTEROID_OPENGL_HXX
// app
#include "alias.hxx"
// son8
// NOTE: important that all `opengl` users of application use the same version
// \ so only include current file in other translation units for `opengl` part
#define SON8_OVERGLAD_INCLUDE_DEPRECATED
#include <son8/overglad/v3_3.hxx>
// std
#include <string>

namespace app {
   namespace gl = son8::overglad;
   namespace glt = gl::types;

   enum class VertShaders : unsigned {
      Default,
      Stars,
      Last_,
   };

   enum class FragShaders: unsigned {
      Default,
      Stars,
      Last_,
   };

   void shader_source( glt::Shader shader, VertShaders name );
   void shader_source( glt::Shader shader, FragShaders name );

   void shader_compile_errors( glt::Shader shader );
   void program_link_errors( glt::Program program );

   class Stars {
      static constexpr GLuint Position_Location = 0u;
      static constexpr GLint Vertex_Size = 3;
      GLint uViewProj_{ -1 };
      glt::VertexArray indexVertex_;
      glt::BufferArray indexBuffer_;
      glt::Shader vert_, frag_;
      glt::Program prog_;
      GrowStars stars_;
      static constexpr GLuint Star_Bytes = sizeof( stars_[0] );
   public:
      Stars( unsigned count );
      void draw( Col4x4f const &matViewProj );
   };

} // namespace app

#endif//SON8_ASTEROID_OPENGL_HXX
