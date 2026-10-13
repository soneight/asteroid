#ifndef SON8_ASTEROID_ALIAS_HXX
#define SON8_ASTEROID_ALIAS_HXX
// son8
#include <son8/matfourd/mat.hxx>
#include <son8/matfourd/vec.hxx>
// std
#include <cstdint>
#include <chrono>
#include <vector>

namespace app {
   namespace m4d = son8::matfourd;

   using Unt2 = std::uint32_t;

   using Col2f = m4d::Col2< float >;
   using Col3f = m4d::Col3< float >;
   using Col4x4f = m4d::Col4x4< float >;

   using GrowStars = std::vector< Col3f >;

   using Clock = std::chrono::steady_clock;
}

#endif//SON8_ASTEROID_ALIAS_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
