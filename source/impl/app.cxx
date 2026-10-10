#include "face/app.hxx"
#include "face/data.hxx"
#include "face/opengl.hxx"
#include <array>
#include <iostream>
#include <string_view>

namespace app {
   namespace gle = gl::enums;
// implementation
// -- opengl
namespace {
   using namespace std::string_view_literals;
   using Shaders = std::array< std::string_view, 2 >;

   constexpr auto vertShaders = Shaders{{
      // Default
      R"GLSL(#version 330 core
layout (location=0) in vec2 appPos;
layout (location=1) in vec3 appColor;

out vec4 vertColor;

void main( ) {
   vertColor = vec4( appColor, 1.0 );
   gl_Position = vec4( appPos, 0.0, 1.0 );
}
      )GLSL"sv,
      // Stars
      R"GLSL(#version 330 core
layout (location=0) in vec3 appPos;

out float worldDistance;

void main( ) {
   gl_PointSize = 2.0;
   worldDistance = length( appPos );
   gl_Position = vec4( appPos, 1.0 );
}
      )GLSL"sv,
   }};

   constexpr auto fragShaders = Shaders{{
      // Default
      R"GLSL(#version 330 core
in vec4 vertColor;
out vec4 fragColor;

void main( ) {
   fragColor = vertColor;
}
      )GLSL"sv,
      // Stars
      R"GLSL(#version 330 core
in vec4 vertColor;
out vec4 fragColor;

in float worldDistance;

void main( ) {
   vec3 color;
   if ( worldDistance < 0.35 ) {
      color = vec3( 0.6, 0.8, 1.0 );
   } else if ( worldDistance < 0.7 ) {
      color = vec3( 1.0, 0.9, 0.6 );
   } else {
      color = vec3( 0.8, 0.4, 0.3 );
   }
   fragColor = vec4( color, 1.0 );
}
      )GLSL"sv
   }};

   static constexpr auto Info_Log_Size = 1024u;

   void upload_source( glt::Shader shader, std::string_view view ) {
      auto data = view.data( );
      auto size = static_cast< GLint >( view.size( ));
      gl::shader_source( shader, 1, &data, &size );
   }

   static constexpr bool ProgramError = false;
   static constexpr bool ShaderError = true;

   template< bool isShaderError >
   bool gl_check_is_errors( GLuint index ) {
      static constexpr std::string_view type = ( isShaderError ) ?  "shader compilation"sv : "program linking"sv;
      int success;
      char infoLog[Info_Log_Size];

      if constexpr ( isShaderError ) { gl::get_shader( index, GL_COMPILE_STATUS, &success ); }
      else                           { gl::get_program( index, GL_LINK_STATUS, &success ); }

      if ( success ) return false;

      if constexpr ( isShaderError ) { gl::get_shader_info_log( index, Info_Log_Size, nullptr, infoLog ); }
      else                           { gl::get_program_info_log( index, Info_Log_Size, nullptr, infoLog ); }

      std::cerr << type << " error info: " << infoLog << New_Line;

      return true;
   }

} // anonymous namespace

} // namespace app

void app::shader_source( glt::Shader shader, Vert_Shaders name ) {
   upload_source( shader, vertShaders[static_cast< unsigned >( name )] );
}
void app::shader_source( glt::Shader shader, Frag_Shaders name ) {
   upload_source( shader, fragShaders[static_cast< unsigned >( name )] );
}

void app::shader_compile_errors( glt::Shader shader ) {
   gl_check_is_errors< ShaderError >( shader );
}

void app::program_link_errors( glt::Program program ) {
   gl_check_is_errors< ProgramError >( program );
}

app::Stars::Stars( unsigned count ) {
   stars = generate_stars( count );
   vert = gl::shader( gle::Shader::Vertex );
   frag = gl::shader( gle::Shader::Fragment );
   shader_source( vert, Vert_Shaders::Stars );
   shader_source( frag, Frag_Shaders::Stars );
   gl::compile( vert );
   shader_compile_errors( vert );
   gl::compile( frag );
   shader_compile_errors( frag );
   prog = gl::program( );
   gl::attach( prog, vert );
   gl::attach( prog, frag );
   gl::link( prog );
   gl::free( vert );
   gl::free( frag );
   program_link_errors( prog );
   gl::gen( indexVertex );
   gl::bind( indexVertex );
   gl::gen( indexBuffer );
   gl::bind( indexBuffer );
   gl::buffer_data( indexBuffer.type( )
      , stars.size( ) * Star_Bytes
      , stars.data( )
      , GL_STATIC_DRAW );
   gl::vertex_attrib_pointer( Position_Location
      , Vertex_Size
      , GL_FLOAT
      , GL_FALSE
      , Star_Bytes
      , (void*)0 );
   gl::enable_vertex_attrib_array( Position_Location );

}

void app::Stars::bind( ) {
   gl::bind( prog );
   gl::bind( indexVertex );

}

void app::Stars::draw( ) {
   gl::draw_arrays( GL_POINTS, Zero_S, stars.size( ));
}

void app::Stars::free( ) {

}

// -- stars
auto app::generate_stars( unsigned count ) -> GrowStars {
   if ( Max_Stars < count ) { count = Zero_U; }
   APP_ASSERT_MSG( count, "app: no stars to generate" );
   using Dist = std::uniform_real_distribution< float >;
   std::mt19937 rng{ 8 };
   Dist distAngle{ Zero_F, Math_PIf };
   Dist distCos{-1.f,+1.f };
   Dist distRadius{-1.f,+1.f };

   GrowStars stars;
   stars.reserve( count );

   while ( count-- ) {
      float theta = distAngle( rng );
      float phi = std::acos( distCos( rng ));
      float radius = distRadius( rng );
      stars.push_back( Col3f{
         radius * std::sin( phi ) * std::cos( theta ),
         radius * std::sin( phi ) * std::sin( theta ),
         radius * std::cos( phi )
      });
   }

   return stars;
}

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
