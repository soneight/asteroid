#include "impl/face/app.hxx"
#include "impl/face/opengl.hxx"
#include "impl/face/math.hxx"
// son8
#include <son8/matfourd/print.hxx>
#include <son8/windowed.hxx>
// std
#include <thread>

namespace wnd = son8::windowed;
namespace gl = son8::overglad;
namespace gle = gl::enums;
namespace glt = gl::types;

struct DrawData {
   static constexpr GLuint Vertex_Location = 0;
   static constexpr GLint Vertex_Size = 2;
   static constexpr GLuint Color_Location = 1;
   static constexpr GLint Color_Size = 3;
   struct Vertex {
      app::Col2f xy;
      app::Col3f rgb;
   };
   glt::VertexArray vertexArray;
   glt::BufferArray bufferArray;
   std::array< Vertex, 3 > array{{
      {{-.8f,-.8f }, { 1.f, 0.f, 0.f }},
      {{+.8f,-.8f }, { 0.f, 1.f, 0.f }},
      {{+.0f,+.8f }, { 0.f, 0.f, 1.f }},
   }};
   glt::Shader vertShader, fragShader;
   glt::Program program;

   auto buffer_size( ) const { return array.size( ) * sizeof( Vertex ); }
};

int main( [[maybe_unused]] int argc, [[maybe_unused]] char *argv[] ) {
   APP_ASSERT_MSG( argc == 1, "app: additional arguments are not supported" );

   using namespace app;
   using Cfg = wnd::Config;

   static constexpr auto Window_W = 960u;
   static constexpr auto Window_H = 540u;
   static constexpr auto Aspect_F = 1.f * Window_W / Window_H;

   Cfg windowConfig{
      Cfg::Width{ 960u },
      Cfg::Height{ 540u },
      Cfg::Version{ wnd::OpenGL::Vx0303CE }
   };

   wnd::Window window{ windowConfig };

   std::thread drawThread( [&window] {
      if ( window.is_error( window.bind_opengl( ))) { throw std::runtime_error{ "draw thread could not bound opengl" }; }
      // TODO: add here opengl one-time data initialization before starting rendering
      gl::enable( GL_PROGRAM_POINT_SIZE );
      gl::enable( gle::Capability::Depth_Test );
      Stars stars{ 256 };
      // NOTE: right now `free_opengl` is necessary as `run_swap` expects not bound opengl
      window.free_opengl( );
      float time = 0.f;
      auto prevTime = Clock::now( );
      window.run_swap( [&]{
         auto currTime = Clock::now( );
         std::chrono::duration< float > const elapsed = currTime - prevTime;
         prevTime = currTime;
         time += elapsed.count( ); // (or elapsed delta time)

         // TODO: make camera class later when basic app input structure would be ready
         Col3f const eye{ 0.f, 0.f, 0.f };
         float const yaw   = time * 0.25f;
         float const pitch = std::sin( time * 0.15f ) * 0.4f;
         Col3f const forward{
            std::sin( yaw ) * std::cos( pitch ),
            std::sin( pitch ),
            -std::cos( yaw ) * std::cos( pitch )
         };
         Col3f const target = eye + forward;
         Col3f const up{ 0.f, 1.f, 0.f };

         Col4x4f const view = look_at( eye, target, up );
         Col4x4f const proj = perspective( 1.047f, Aspect_F, 1.f, 2048.f );

         Col4x4f const viewProj = proj * view;
         gl::clear_color( );
         gl::clear( gle::Clearbit::Color | gle::Clearbit::Depth );
         // NOTE: stars do not need translation as they draw as points without depth test
         stars.draw( proj * rotation_only( view ));
      }); // window run_swap
   }); // draw thread

   window.run_poll( []{

   });

   drawThread.join( );

} // main()

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
