#ifndef SON8_ASTEROID_OPENGL_HXX
#define SON8_ASTEROID_OPENGL_HXX
// app
#include "app/macros.hxx"
#include "alias.hxx"
#include "math.hxx"
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

   constexpr auto Uniform_Not_Found = -1;

   enum class VertShaders : unsigned {
      Default,
      Stars,
      Ship,
      Last_,
   };

   enum class FragShaders: unsigned {
      Default,
      Stars,
      Ship,
      Last_,
   };

   void shader_source( glt::Shader shader, VertShaders name );
   void shader_source( glt::Shader shader, FragShaders name );

   void shader_compile_errors( glt::Shader shader );
   void program_link_errors( glt::Program program );

   class Stars {
      static constexpr GLuint Position_Location = 0u;
      static constexpr GLint Vertex_Size = 3;
      GLint uViewProj_{ Uniform_Not_Found };
      glt::VertexArray indexVertex_;
      glt::BufferArray indexBuffer_;
      glt::Shader vert_, frag_;
      glt::Program prog_;
      GrowStars stars_;
      static constexpr GLuint Star_Bytes = sizeof( stars_[0] );
      void generate_Points( unsigned count );
   public:
      Stars( unsigned count );
      void draw( Col4x4f const &matViewProj );
   };

   class Ship {
      // implementation shader constants
      static constexpr auto Location_Pos = 0u;
      static constexpr auto Location_Normal = 1u;
      static constexpr auto Location_Color = 2u;
      struct Vertex {
         Col3f pos;
         Col3f normal;
         Col3f color;
      };
      using GrowVertex = std::vector< Vertex >;
      using Facet = std::array< Vertex, 3 >;
      glt::VertexArray indexVertex_;
      glt::BufferArray indexBuffer_;
      glt::Shader vert_, frag_;
      glt::Program prog_;
      GLint uViewProj_{ Uniform_Not_Found };
      GLint uModel_{ Uniform_Not_Found };
      GrowVertex ship_;
      Size vertexCount_;
      auto make_Facet( Ref< Col3f > a, Ref< Col3f > b, Ref< Col3f > c, Ref< Col3f > color ) -> Facet;
      auto generate_Mesh(  ) -> GrowVertex;
   public:
      // interface flight constants
      APP_DATA Rot_Speed    = math::PI_f / 4.f;  // ~45 degrees/sec rotation
      APP_DATA Thrust_Accel = 16.f;  // Acceleration (m/s^2)
      APP_DATA Max_Speed    = 64.f;  // Top boost speed (m/s)
      APP_DATA Brake_Damp   = 8.0f;  // Active braking factor
      APP_DATA Drag_Damp    = 0.2f;  // Passive vacuum drift drag
      // constructors
      Ship( );
      // methods
      void draw( Ref< Col4x4f > matViewProj, Ref< Col4x4f > matModel );
   };

} // namespace app

#endif//SON8_ASTEROID_OPENGL_HXX
