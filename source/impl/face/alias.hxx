#ifndef SON8_ASTEROID_ALIAS_HXX
#define SON8_ASTEROID_ALIAS_HXX
// son8
#include <son8/matfourd/mat.hxx>
#include <son8/matfourd/vec.hxx>
// std
#include <vector>

namespace app {
   namespace m4d = son8::matfourd;

   using Col2f = m4d::Col2< float >;
   using Col3f = m4d::Col3< float >;
   using Col4x4f = m4d::Col4x4< float >;

   using GrowStars = std::vector< Col3f >;
}

#endif//SON8_ASTEROID_ALIAS_HXX
