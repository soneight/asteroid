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

   enum class Vert_Shaders : unsigned {
      Default,
      Stars, // TODO
      Last_,
   };

   enum class Frag_Shaders: unsigned {
      Default,
      Stars, // TODO
      Last_,
   };

   void shader_source( glt::Shader shader, Vert_Shaders name );
   void shader_source( glt::Shader shader, Frag_Shaders name );

   void shader_compile_errors( glt::Shader shader );
   void program_link_errors( glt::Program program );


   class Stars {
      static constexpr GLuint Position_Location = 0u;
      static constexpr GLint Vertex_Size = 3;

      glt::VertexArray indexVertex;
      glt::BufferArray indexBuffer;
      glt::Shader vert, frag;
      glt::Program prog;
      GrowStars stars;
      static constexpr GLuint Star_Bytes = sizeof( stars[0] );
   public:
      Stars( unsigned count );
      void bind( );
      void draw( );
      void free( );
   };
}

#endif//SON8_ASTEROID_OPENGL_HXX
