#ifndef SON8_ASTEROID_APP_CAMERA_HXX
#define SON8_ASTEROID_APP_CAMERA_HXX

#include "../alias.hxx"

namespace app {

   struct Camera {
      Col3f eye;
      Col3f aim;
      Col3f sky;
   };

}

#endif//SON8_ASTEROID_APP_CAMERA_HXX
