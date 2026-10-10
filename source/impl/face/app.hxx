#ifndef SON8_ASTEROID_APP_HXX
#define SON8_ASTEROID_APP_HXX

#include "alias.hxx"
// son8
// std
#include <cassert>
#include <random>
#include <vector>

#ifndef APP_DEBUG
#define APP_ASSERT( check ) ( (void)0 )
#define APP_ASSERT_MSG( check, msg ) ( (void)0 )
#else
#define APP_ASSERT( check ) assert( check )
#define APP_ASSERT_MSG( check, msg ) assert( check and msg )
#endif//APP_DEBUG

namespace app {

   constexpr char New_Line = '\n';
   constexpr auto Zero_S = 0;
   constexpr auto Zero_U = 0u;
   constexpr auto Zero_F = 0.f;
   constexpr auto Math_PIf = 3.14159265f;
   constexpr auto Math_PId = 3.141592653589793;
   constexpr auto Max_Stars = 1u << 16u;

} // namespace app

#endif//SON8_ASTEROID_APP_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
