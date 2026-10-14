#include "impl/face/app.hxx"
#include "impl/face/opengl.hxx"
#include "impl/face/math.hxx"
// son8
#include <glfw/son8.hxx>
#include <son8/matfourd/print.hxx>
#include <son8/windowed.hxx>
// std
#include <thread>
#include <iostream>

namespace wnd = son8::windowed;
namespace gl = son8::overglad;
namespace gle = gl::enums;
namespace glt = gl::types;

struct DrawData {
   static constexpr GLuint Vertex_Location = 0;
   static constexpr GLint Vertex_Size = 2;
   static constexpr GLuint Color_Location = 1;
   static constexpr GLint Color_Size = 3;
   struct Vertex {
      app::Col2f xy;
      app::Col3f rgb;
   };
   glt::VertexArray vertexArray;
   glt::BufferArray bufferArray;
   std::array< Vertex, 3 > array{{
      {{-.8f,-.8f }, { 1.f, 0.f, 0.f }},
      {{+.8f,-.8f }, { 0.f, 1.f, 0.f }},
      {{+.0f,+.8f }, { 0.f, 0.f, 1.f }},
   }};
   glt::Shader vertShader, fragShader;
   glt::Program program;

   auto buffer_size( ) const { return array.size( ) * sizeof( Vertex ); }
};

int main( [[maybe_unused]] int argc, [[maybe_unused]] char *argv[] ) {
   APP_ASSERT_MSG( argc == 1, "app: additional arguments are not supported" );

   using namespace app;
   using Cfg = wnd::Config;

   static constexpr auto Window_W = 960u;
   static constexpr auto Window_H = 540u;
   static constexpr auto Aspect_F = 1.f * Window_W / Window_H;

   Cfg windowConfig{
      Cfg::Width{ Window_W },
      Cfg::Height{ Window_H },
      Cfg::Version{ wnd::OpenGL::Vx0303CE }
   };

   wnd::Window window{ windowConfig };

   static auto windowPtr = &window;

   glfwSetKeyCallback( window, []( GLFWwindow *w, int key, int scan, int action, int mods ){
      bool pressed = action == GLFW_PRESS;
      if ( key == GLFW_KEY_ESCAPE and pressed ) { windowPtr->close( ); return; }

      Action::InputMask inputBit;
      switch ( key ) {
      case GLFW_KEY_W: { inputBit = Action::Pitch_DN; break; }
      case GLFW_KEY_S: { inputBit = Action::Pitch_UP; break; }
      case GLFW_KEY_A: { inputBit = Action::Yaw_LT; break; }
      case GLFW_KEY_D: { inputBit = Action::Yaw_RT; break; }
      case GLFW_KEY_Q: { inputBit = Action::Roll_CCW; break; }
      case GLFW_KEY_E: { inputBit = Action::Roll_CW; break; }
      case GLFW_KEY_SPACE: { inputBit = Action::Speed; break; }
      case GLFW_KEY_LEFT_SHIFT: { inputBit = Action::Brake; break; }
         default: { return; }
      }

      switch ( action ) {
      case GLFW_PRESS: { globals::inputMask.fetch_or( inputBit, std::memory_order_relaxed ); } break;
      case GLFW_RELEASE: { globals::inputMask.fetch_and( ~inputBit, std::memory_order_relaxed ); } break;
         default: return;
      }
   });

   std::thread drawThread( [&window] {
      if ( window.is_error( window.bind_opengl( ))) { throw std::runtime_error{ "draw thread could not bound opengl" }; }

      gl::enable( GL_PROGRAM_POINT_SIZE );
      gl::enable( gle::Capability::Depth_Test );

      Stars stars{ 256 };
      Ship ship{ };

      window.free_opengl( );

      auto prevTime = Clock::now( );

      // --- SHIP PHYSICAL STATE ---
      Col3f shipPosition{ 0.f, 0.f, 0.f };
      Col3f shipVelocity{ 0.f, 0.f, 0.f };

      // 3D Basis Triad (Orientation)
      Col3f shipForward{  0.f, 0.f, -1.f };
      Col3f shipUp{       0.f, 1.f,  0.f };
      Col3f shipRight{    1.f, 0.f,  0.f };

      window.run_swap( [&]{
         auto currTime = Clock::now( );
         std::chrono::duration< float > const elapsed = currTime - prevTime;
         prevTime = currTime;
         float const dt = elapsed.count( );
         // ATTITUDE CONTROL
         float const rotAngle = Ship::Rot_Speed * dt;
         // process input
         Action::InputMask input = globals::inputMask.load( std::memory_order_relaxed );
         // -- pitch (move nose up or down)
         if ( input & Action::Pitch_UP ) {
            shipForward = math::normalize( shipForward + ( shipUp * rotAngle ));
            shipUp      = math::normalize( shipRight ^ shipForward );
         }
         if ( input & Action::Pitch_DN ) {
            shipForward = math::normalize( shipForward - ( shipUp * rotAngle ));
            shipUp      = math::normalize( shipRight ^ shipForward );
         }
         // -- yaw (move body left or right)
         if ( input & Action::Yaw_LT ) {
            shipForward = math::normalize( shipForward - ( shipRight * rotAngle ));
            shipRight   = math::normalize( shipForward ^ shipUp );
         }
         if ( input & Action::Yaw_RT ) {
            shipForward = math::normalize( shipForward + ( shipRight * rotAngle ));
            shipRight   = math::normalize( shipForward ^ shipUp );
         }
         // -- roll (move wing counter clock-wise or clock-wise)
         if ( input & Action::Roll_CCW ) {
            shipRight = math::normalize( shipRight + ( shipUp * rotAngle ));
            shipUp    = math::normalize( shipRight ^ shipForward );
         }
         if ( input & Action::Roll_CW ) {
            shipRight = math::normalize( shipRight - ( shipUp * rotAngle ));
            shipUp    = math::normalize( shipRight ^ shipForward );
         }
         // -- speed (accelerate or brake)
         if ( input & Action::Speed ) {
            // Push ship forward along its current nose direction
            shipVelocity += shipForward * ( Ship::Thrust_Accel * dt );

            // Clamp to top speed
            float const currentSpeed = math::length( shipVelocity );
            if ( currentSpeed > Ship::Max_Speed ) {
               shipVelocity = ( shipVelocity * ( 1.f / currentSpeed )) * Ship::Max_Speed;
            }
         }
         // TODO: right now not visible effect in empty space
         // \ active: reduce velocity quickly
         // \ passive: reduce velocity slowly
         if ( input & Action::Brake ) { shipVelocity -= shipVelocity * ( Ship::Brake_Damp * dt ); }
         else                         { shipVelocity -= shipVelocity * ( Ship::Drag_Damp * dt ); }
#ifdef APP_DEBUG
         std::cout << "speed: " << math::length( shipVelocity ) << ", velocity: " << shipVelocity << std::endl;
#endif
         // Integrate velocity into position
         shipPosition += shipVelocity * dt;
         // camera tracking ship
         float constexpr ChaseDist = 14.0f; // behind
         float constexpr ChaseHgt  =  4.0f; // above
         Camera camera{
            .eye = shipPosition - ( shipForward * ChaseDist ) + ( shipUp * ChaseHgt ),
            .aim = shipPosition + ( shipForward * 4.0f ),
            .sky = { shipUp.x( ), 1.0, shipUp.z( ) } //shipUp, // camera rolls with the ship
         };
         // view projection matrix
         Col4x4f const view = math::look_at( camera );
         Col4x4f const proj = math::perspective( 1.047f, Aspect_F, 1.f, 2048.f );
         Col4x4f const viewProj = proj * view;
         // ship model matrix
         Col4x4f const shipModel{
            math::vec4( shipRight   , 0.f ), // column 1: local X
            math::vec4( shipUp      , 0.f ), // column 2: local Y
            math::vec4(-shipForward , 0.f ), // column 3: local Z (facing -forward)
            math::vec4( shipPosition, 1.f )  // column 4: translation
         };
         // rendering
         gl::clear_color( );
         gl::clear( gle::Clearbit::Color | gle::Clearbit::Depth );
         // NOTE: depth mask disabled here
         stars.draw( proj * math::rotation_only( view ) );
         // player ship
         ship.draw( viewProj, shipModel );
      });
   });

   window.run_poll( []{

   });

   drawThread.join( );

} // main()

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `asteroid` C++17 Shatter Cosmic Rocks Game
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
