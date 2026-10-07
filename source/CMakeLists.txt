set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
# app
# -- face
set( SON8_APP_FACE ${SON8_APP}_FACE )
add_library( ${SON8_APP_FACE} INTERFACE )
target_compile_options( ${SON8_APP_FACE} INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/utf-8 /permissive- /Zc:__cplusplus> )
# -- impl
set( SON8_APP_IMPL ${SON8_APP}_IMPL )
add_library( ${SON8_APP_IMPL} STATIC )
target_sources( ${SON8_APP_IMPL} PRIVATE impl/app.cxx )
target_link_libraries( ${SON8_APP_IMPL} PUBLIC ${SON8_APP_FACE} )
# -- main
add_executable( ${SON8_APP} )
target_sources( ${SON8_APP} PRIVATE main.cxx )
target_link_libraries( ${SON8_APP} PRIVATE ${SON8_APP_IMPL} )
