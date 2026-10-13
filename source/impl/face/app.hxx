#ifndef SON8_ASTEROID_APP_HXX
#define SON8_ASTEROID_APP_HXX

#include "alias.hxx"
// std
#include <atomic>
#include <cassert>

#ifndef APP_DEBUG
#define APP_ASSERT( check ) ( (void)0 )
#define APP_ASSERT_MSG( check, msg ) ( (void)0 )
#else
#define APP_ASSERT( check ) assert( check )
#define APP_ASSERT_MSG( check, msg ) assert( check and msg )
#endif//APP_DEBUG

#define APP_DATA static constexpr auto

namespace app {

   constexpr char New_Line = '\n';
   constexpr auto Zero_S = 0;
   constexpr auto Zero_U = 0u;
   constexpr auto Zero_F = 0.f;
   constexpr auto Math_PIf = 3.14159265f;
   constexpr auto Math_PId = 3.141592653589793;
   constexpr auto Max_Stars = 1u << 16u;

   constexpr auto Uniform_Not_Found = -1;

   struct Action {
      using InputMask = Unt2;
      APP_DATA Pitch_DN = InputMask{ 1u << 0u }; // W
      APP_DATA Pitch_UP = InputMask{ 1u << 1u }; // S
      APP_DATA Yaw_LT = InputMask{ 1u << 2u }; // A
      APP_DATA Yaw_RT = InputMask{ 1u << 3u }; // D
      APP_DATA Roll_CCW = InputMask{ 1u << 4 }; // Q
      APP_DATA Roll_CW = InputMask{ 1u << 5 }; // E
      APP_DATA Speed = InputMask{ 1u << 6 }; // Space
      APP_DATA Brake = InputMask{ 1u << 7 }; // Shift
   };

} // namespace app

namespace app::globals {
   inline std::atomic< Action::InputMask > inputMask{ };
}

#endif//SON8_ASTEROID_APP_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
