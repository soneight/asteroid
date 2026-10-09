#include "face/app.hxx"
#include "face/opengl.hxx"
#include <array>
#include <iostream>
#include <string_view>

namespace app {
// implementation
// -- opengl
namespace {
   using namespace std::string_view_literals;
   using Shaders = std::array< std::string_view, 1 >;

   constexpr auto vertShaders = Shaders{{
      R"GLSL(#version 330 core
layout (location=0) in vec2 appPos;
layout (location=1) in vec3 appColor;

out vec4 vertColor;

void main( ) {
   vertColor = vec4( appColor, 1.0 );
   gl_Position = vec4( appPos, 0.0, 1.0 );
}
      )GLSL"sv,
   }};

   constexpr auto fragShaders = Shaders{{
      R"GLSL(#version 330 core
in vec4 vertColor;
out vec4 fragColor;

void main( ) {
   fragColor = vertColor;
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

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
