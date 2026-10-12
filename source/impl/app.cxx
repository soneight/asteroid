#include "face/app.hxx"
#include "face/data.hxx"
#include "face/opengl.hxx"
// std
#include <array>
#include <iostream>
#include <random>
#include <string_view>
#include <vector>
// NOTE: deliberately include source file for shaders
#include "shaders.inc.cxx"

namespace app {
   namespace gle = gl::enums;
// implementation
// -- opengl
namespace {
   using namespace std::string_view_literals;
   using Size = std::size_t;
   using View = std::string_view;
   using VertShadersArray = std::array< View, static_cast< Size >( VertShaders::Last_ )>;
   using FragShadersArray = std::array< View, static_cast< Size >( FragShaders::Last_ )>;

   template< gle::Shader Shader_Type >
   constexpr auto extract_shader_guard_helper( ) -> View {
      if constexpr ( Shader_Type == gle::Shader::Vertex ) return "//!VERT"sv;
      else if constexpr ( Shader_Type == gle::Shader::Fragment ) return "//!FRAG"sv;
      else return ""sv;
   }

   template< gle::Shader Shader_Type >
   constexpr auto extract_shader( View source ) -> View {
      View guard = extract_shader_guard_helper< Shader_Type >( );
      if ( guard.empty( )) return ""sv;

      Size guardPos = source.find( guard );
      if ( guardPos == View::npos ) return ""sv;
      Size versionPos = source.find( "#version"sv, guardPos );
      if ( versionPos == View::npos ) return ""sv;
      Size endPos = source.find( "//!"sv, versionPos );
      if ( endPos == View::npos ) return ""sv;
      return source.substr( versionPos, endPos - versionPos );
   }

#ifdef APP_INNER
   static_assert( extract_shader< gle::Shader::Vertex >( "//!VERT\n#version\nvoid main( )//!FRAG"sv )
      == "#version\nvoid main( )"sv  );
#endif

   constexpr auto Vert_Shaders = VertShadersArray{{
      extract_shader< gle::Shader::Vertex >( Default_Shader ),
      extract_shader< gle::Shader::Vertex >( Stars_Shader ),
   }};

   constexpr auto Frag_Shaders = FragShadersArray{{
      extract_shader< gle::Shader::Fragment >( Default_Shader ),
      extract_shader< gle::Shader::Fragment >( Stars_Shader ),
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

void app::shader_source( glt::Shader shader, VertShaders name ) {
   upload_source( shader, Vert_Shaders[static_cast< unsigned >( name )] );
}
void app::shader_source( glt::Shader shader, FragShaders name ) {
   upload_source( shader, Frag_Shaders[static_cast< unsigned >( name )] );
}

void app::shader_compile_errors( glt::Shader shader ) {
   gl_check_is_errors< ShaderError >( shader );
}

void app::program_link_errors( glt::Program program ) {
   gl_check_is_errors< ProgramError >( program );
}

app::Stars::Stars( unsigned count ) {
   stars_ = generate_stars( count );
   vert_ = gl::shader( gle::Shader::Vertex );
   frag_ = gl::shader( gle::Shader::Fragment );
   shader_source( vert_, VertShaders::Stars );
   shader_source( frag_, FragShaders::Stars );
   gl::compile( vert_ );
   shader_compile_errors( vert_ );
   gl::compile( frag_ );
   shader_compile_errors( frag_ );
   prog_ = gl::program( );
   gl::attach( prog_, vert_ );
   gl::attach( prog_, frag_ );
   gl::link( prog_ );
   gl::free( vert_ );
   gl::free( frag_ );
   program_link_errors( prog_ );
   uViewProj_ = gl::get_uniform_location( prog_, "uViewProj" );
   APP_ASSERT_MSG( uViewProj_ != Uniform_Not_Found, "app: Stars::Stars uViewProj uniform not found" );
   gl::gen( indexVertex_ );
   gl::bind( indexVertex_ );
   gl::gen( indexBuffer_ );
   gl::bind( indexBuffer_ );
   gl::buffer_data( indexBuffer_.type( )
      , stars_.size( ) * Star_Bytes
      , stars_.data( )
      , GL_STATIC_DRAW );
   gl::vertex_attrib_pointer( Position_Location
      , Vertex_Size
      , GL_FLOAT
      , GL_FALSE
      , Star_Bytes
      , (void*)0 );
   gl::enable_vertex_attrib_array( Position_Location );
}

void app::Stars::draw( Col4x4f const &matViewProj ) {
   gl::bind( prog_ );
   gl::bind( indexVertex_ );
   gl::uniform_matrix_4( uViewProj_, 1, GL_FALSE, matViewProj.data( ));
   gl::depth_mask( GL_FALSE );
   gl::draw_arrays( GL_POINTS, Zero_S, stars_.size( ));
   gl::depth_mask( GL_TRUE );
}

// -- stars
auto app::generate_stars( unsigned count ) -> GrowStars {
   if ( Max_Stars < count ) { count = Zero_U; }
   APP_ASSERT_MSG( count, "app: no stars to generate" );
   using Dist = std::uniform_real_distribution< float >;
   std::mt19937 rng{ 0 };
   Dist distAngle{ Zero_F, 2.f * Math_PIf };
   Dist distCos{-1.f,+1.f };
   Dist distRadius{2.f,5.f };

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
