cmake_minimum_required( VERSION 3.20...3.27 )
project( APPLICATION VERSION 0.0.0 LANGUAGES CXX )
set( SON8_APP ${PROJECT_NAME} )
# private configuration under source directory
add_subdirectory( source )
