set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
# deps
# -- son8
# ---- `matfourd`
if( NOT TARGET son8__matfourd )
   include( FetchContent )
   message( STATUS "${SON8_APP}: FetchContent `soneight/matfourd`" )
   fetchcontent_declare(
      son8__matfourd
      GIT_REPOSITORY https://github.com/soneight/matfourd.git
      GIT_TAG        83f979910bfe4c1cc4c815031cb74b0599438b0a # v0.0.1
   )
   fetchcontent_makeavailable( son8__matfourd )
endif( )
message( STATUS "${SON8_APP}: target `son8__matfourd` found" )
# ---- `windowed`
if( NOT TARGET son8__windowed )
   include( FetchContent )
   message( STATUS "${SON8_APP}: FetchContent `soneight/windowed`" )
   fetchcontent_declare(
      son8__windowed
      GIT_REPOSITORY https://github.com/soneight/windowed.git
      GIT_TAG        8bedc378ec08500981f1d27dd302c4fce0c5f9d9 # v0.6.1
   )
   fetchcontent_makeavailable( son8__windowed )
endif( )
message( STATUS "${SON8_APP}: target `son8__windowed` found" )
# app
# -- face
set( SON8_APP_FACE ${SON8_APP}_FACE )
add_library( ${SON8_APP_FACE} INTERFACE )
target_compile_options( ${SON8_APP_FACE} INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/utf-8 /permissive- /Zc:__cplusplus> )
# -- impl
set( SON8_APP_IMPL ${SON8_APP}_IMPL )
add_library( ${SON8_APP_IMPL} STATIC )
target_sources( ${SON8_APP_IMPL} PRIVATE impl/app.cxx )
target_link_libraries( ${SON8_APP_IMPL} PUBLIC ${SON8_APP_FACE} son8::matfourd )
# -- main
add_executable( ${SON8_APP} )
target_sources( ${SON8_APP} PRIVATE main.cxx )
target_link_libraries( ${SON8_APP} PRIVATE ${SON8_APP_IMPL} son8::windowed glad::son8 )

# GNU Affero General Public License v3.0 or later
# NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
# SPDX-License-Identifier: AGPL-3.0-or-later
# app: `asteroid` C++17 Shatter Cosmic Rocks Game
# Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
