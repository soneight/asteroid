#include "impl/face/app.hxx"
// son8
#include <glad/son8.hxx>
#include <son8/matfourd/print.hxx>
#include <son8/windowed.hxx>
// std
#include <iostream>
#include <thread>

namespace wnd = son8::windowed;

int main( [[maybe_unused]] int argc, [[maybe_unused]] char *argv[] ) {
   APP_ASSERT_MSG( argc == 1, "argc must contain one argument" );

   using namespace app;

   using Cfg = wnd::Config;

   Cfg windowConfig{
      Cfg::Width{ 960u },
      Cfg::Height{ 540u },
      Cfg::Version{ wnd::OpenGL::Vx0303CE }
   };

   wnd::Window window{ windowConfig };

   std::thread drawThread( [&window] {
      if ( window.is_error( window.bind_opengl( ))) { throw std::runtime_error{ "draw thread could not bound opengl" }; }
      // TODO: add here opengl one-time data initialization before starting rendering

      // NOTE: right now `free_opengl` is necessary as `run_swap` expects not bound opengl
      window.free_opengl( );
      window.run_swap( [&window]{
         glClearColor( .125f, .125f, .125f, 0 );
         glClear( GL_COLOR_BUFFER_BIT );
      });
   });

   window.run_poll( []{

   });

   drawThread.join( );

}

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
