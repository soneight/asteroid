#ifndef SON8_ASTEROID_APP_HXX
#define SON8_ASTEROID_APP_HXX

#include "app/macros.hxx"
#include "app/camera.hxx"
#include "alias.hxx"
// std
#include <atomic>
#include <cassert>

namespace app {

   APP_EXPR New_Line = '\n';
   APP_EXPR Zero_S = 0;
   APP_EXPR Zero_U = 0u;
   APP_EXPR Zero_F = 0.f;
   APP_EXPR Max_Stars = 1u << 16u;

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
