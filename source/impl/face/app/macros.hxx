#ifndef SON8_ASTEROID_APP_MACROS_HXX
#define SON8_ASTEROID_APP_MACROS_HXX

#ifndef APP_DEBUG
#define APP_ASSERT( check ) ( (void)0 )
#define APP_ASSERT_MSG( check, msg ) ( (void)0 )
#else
#define APP_ASSERT( check ) assert( check )
#define APP_ASSERT_MSG( check, msg ) assert( check and msg )
#endif//APP_DEBUG

#define APP_DATA static constexpr auto
#define APP_EXPR constexpr auto

#endif//SON8_ASTEROID_APP_MACROS_HXX
