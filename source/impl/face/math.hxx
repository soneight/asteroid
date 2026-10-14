#ifndef SON8_ASTEROID_MATH_HXX
#define SON8_ASTEROID_MATH_HXX

#include "alias.hxx"
#include "app/camera.hxx"
#include "app/macros.hxx"
// std
#include <cmath>

namespace app::math {

   APP_EXPR PI_f = 3.14159265f;
   APP_EXPR PI_d = 3.1415926535897932;

   inline auto length_squared( Col3f const &vec ) -> float { return vec * vec; }

   inline auto length( Col3f const &vec ) -> float { return std::sqrt( length_squared( vec )); }

   inline Col3f normalize( Col3f const &v ) {
      float const len = length( v );
      if ( len < 1e-6f ) return Col3f{ 0.f, 0.f, 0.f };
      return v * ( 1.f / len );
   }

   inline auto perspective( float fovRad, float aspect, float nearZ, float farZ ) {
      float const tanHalfFov = std::tan( fovRad * 0.5f );
      float const sX_ = 1.f / ( aspect * tanHalfFov );
      float const sY_ = 1.f / tanHalfFov;
      float const sZ_ = ( farZ + nearZ ) / ( nearZ - farZ );
      float const sW_ = ( 2.f * farZ * nearZ ) / ( nearZ - farZ );

      return Col4x4f{
         { sX_, 0.f, 0.f, 0.f },
         { 0.f, sY_, 0.f, 0.f },
         { 0.f, 0.f, sZ_,-1.f },
         { 0.f, 0.f, sW_, 0.f }
      };
   }

   inline auto look_at( Ref< Camera > camera ) {
      Col3f const zAxis = normalize( camera.eye - camera.aim );
      Col3f const xAxis = normalize( camera.sky ^ zAxis );
      Col3f const yAxis = zAxis ^ xAxis;

      return Col4x4f{
         { xAxis.x( ), yAxis.x( ), zAxis.x( ), 0.f },
         { xAxis.y( ), yAxis.y( ), zAxis.y( ), 0.f },
         { xAxis.z( ), yAxis.z( ), zAxis.z( ), 0.f },
         { -( xAxis * camera.eye ), -( yAxis * camera.eye ), -( zAxis * camera.eye ), 1.f }
      };
   }

   inline auto rotation_only( Ref< Col4x4f > mat ) {
      return Col4x4f{
         mat.v1( ),
         mat.v2( ),
         mat.v3( ),
         { 0.f, 0.f, 0.f, 1.f } // no translation
      };
   }

   inline auto vec4( Col3f vec3, float w ) {
      return Col4f{ vec3.x( ), vec3.y( ), vec3.z( ), w };
   }

} // namespace app

#endif//SON8_ASTEROID_MATH_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
