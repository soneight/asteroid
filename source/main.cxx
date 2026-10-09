#include "impl/face/app.hxx"
#include "impl/face/opengl.hxx"
// son8
#include <son8/matfourd/print.hxx>
#include <son8/windowed.hxx>
// std
#include <iostream>
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

   Cfg windowConfig{
      Cfg::Width{ 960u },
      Cfg::Height{ 540u },
      Cfg::Version{ wnd::OpenGL::Vx0303CE }
   };

   wnd::Window window{ windowConfig };

   std::thread drawThread( [&window] {
      if ( window.is_error( window.bind_opengl( ))) { throw std::runtime_error{ "draw thread could not bound opengl" }; }
      // TODO: add here opengl one-time data initialization before starting rendering
      DrawData triangleData;
      triangleData.vertShader = gl::shader( gle::Shader::Vertex );
      triangleData.fragShader = gl::shader( gle::Shader::Fragment );
      shader_source( triangleData.vertShader, Vert_Shaders::Default );
      shader_source( triangleData.fragShader, Frag_Shaders::Default );
      gl::compile( triangleData.vertShader );
      shader_compile_errors( triangleData.vertShader );
      gl::compile( triangleData.fragShader );
      shader_compile_errors( triangleData.fragShader );
      triangleData.program = gl::program( );
      gl::attach( triangleData.program, triangleData.vertShader );
      gl::attach( triangleData.program, triangleData.fragShader );
      gl::link( triangleData.program );
      gl::free( triangleData.vertShader );
      gl::free( triangleData.fragShader );
      program_link_errors( triangleData.program );
      gl::gen( triangleData.vertexArray );
      gl::gen( triangleData.bufferArray );
      gl::bind( triangleData.vertexArray );
      gl::bind( triangleData.bufferArray );
      gl::buffer_data( triangleData.bufferArray.type( )
         , triangleData.buffer_size( )
         , triangleData.array.data( )
         , GL_STATIC_DRAW );
      gl::vertex_attrib_pointer( DrawData::Vertex_Location
         , DrawData::Vertex_Size
         , GL_FLOAT
         , GL_FALSE
         , sizeof( DrawData::Vertex )
         , (void*)offsetof( DrawData::Vertex, xy ));
      gl::enable_vertex_attrib_array( DrawData::Vertex_Location );
      gl::vertex_attrib_pointer( DrawData::Color_Location
         , DrawData::Color_Size
         , GL_FLOAT
         , GL_FALSE
         , sizeof( DrawData::Vertex )
         , (void*)offsetof( DrawData::Vertex, rgb ));
      gl::enable_vertex_attrib_array( DrawData::Color_Location );
      // NOTE: right now `free_opengl` is necessary as `run_swap` expects not bound opengl
      window.free_opengl( );
      window.run_swap( [&window,&triangleData]{
         gl::clear_color( .125f, 0 );
         gl::clear( gle::Clearbit::Color );
         gl::bind( triangleData.program );
         gl::bind( triangleData.vertexArray );
         gl::draw_arrays( GL_TRIANGLES, 0, triangleData.array.size( ));
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
