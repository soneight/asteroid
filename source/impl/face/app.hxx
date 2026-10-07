#ifndef SON8_APP_HXX
#define SON8_APP_HXX

#include <cassert>
#include <son8/matfourd/mat.hxx>

#ifndef APP_DEBUG
#define APP_ASSERT( check ) ( (void)0 )
#define APP_ASSERT_MSG( check, msg ) ( (void)0 )
#else
#define APP_ASSERT( check ) assert( check )
#define APP_ASSERT_MSG( check, msg ) assert( check and msg )
#endif//APP_DEBUG

namespace app {
   namespace m4d = son8::matfourd;

   using Col4x4f = m4d::Col4x4< float >;
} // namespace app

#endif//SON8_APP_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
