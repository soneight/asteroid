#ifndef SON8_APP_HXX
#define SON8_APP_HXX

#include <cassert>

#ifndef APP_DEBUG
#define APP_ASSERT( check ) ( (void)0 )
#define APP_ASSERT_MSG( check, msg ) ( (void)0 )
#else
#define APP_ASSERT( check ) assert( check )
#define APP_ASSERT_MSG( check, msg ) assert( check and msg )
#endif//APP_DEBUG

namespace app {

} // namespace app

#endif//SON8_APP_HXX
