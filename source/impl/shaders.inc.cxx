#include <string_view>

namespace app { namespace {
   using namespace std::string_view_literals;
   using View = std::string_view;

   constexpr View Default_Shader = R"GLSL(
//!VERT
#version 330 core
layout (location=0) in vec2 appPos;
layout (location=1) in vec3 appColor;

out vec4 vertColor;

void main( ) {
   vertColor = vec4( appColor, 1.0 );
   gl_Position = vec4( appPos, 0.0, 1.0 );
}
//!FRAG
#version 330 core
in vec4 vertColor;
out vec4 fragColor;

void main( ) {
   fragColor = vertColor;
}

//!GLSL
   )GLSL"sv;

   constexpr View Stars_Shader = R"GLSL(
//!VERT
#version 330 core
layout (location=0) in vec3 appPos;

uniform mat4 uViewProj;

out float worldDistance;

void main( ) {
   gl_PointSize = 2.0;
   worldDistance = length( appPos );
   gl_Position = uViewProj * vec4( appPos, 1.0 );
}
//!FRAG
#version 330 core
out vec4 fragColor;

in float worldDistance;

void main( ) {
   vec3 color;
   if ( worldDistance < 3.0 ) {
      color = vec3( 0.6, 0.8, 1.0 );
   } else if ( worldDistance < 4.0 ) {
      color = vec3( 1.0, 0.9, 0.6 );
   } else {
      color = vec3( 0.8, 0.4, 0.3 );
   }
   fragColor = vec4( color, 1.0 );
}
//!GLSL
   )GLSL"sv;

}} // anonymous app namespace

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
