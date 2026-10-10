#include "impl/face/app.hxx"
#include "impl/face/opengl.hxx"
// son8
#include <son8/matfourd/print.hxx>
#include <son8/windowed.hxx>
// std
#include <iostream>
#include <thread>
#include <iomanip>

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

   // for ( auto star : stars ) { std::cout << star.pos << New_Line; }

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
      Stars stars{ 100 };
      // NOTE: right now `free_opengl` is necessary as `run_swap` expects not bound opengl
      window.free_opengl( );
      window.run_swap( [&window,&stars]{
         gl::clear_color( .125f, 0 );
         gl::clear( gle::Clearbit::Color );
         stars.bind( );
         stars.draw( );
      });
   });

   window.run_poll( []{

   });

   drawThread.join( );

}

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
