cmake_minimum_required(VERSION 2.8)
project (ImgPopper)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

# include headers
include_directories(include)

# include source files
file(GLOB SOURCES "src/*.cpp" "include/*.hpp")

add_executable(ImgPopper ${SOURCES})

find_package(PkgConfig)

# POPPLER
pkg_check_modules(POPPLER_GLIB REQUIRED poppler-glib)
include_directories(${POPPLER_GLIB_INCLUDE_DIR})
target_link_libraries(ImgPopper ${POPPLER_GLIB_LIBRARIES})

# CAIRO
pkg_check_modules(CAIRO REQUIRED cairo)
include_directories(${CAIRO_INCLUDE_DIRS})
link_directories(${CAIRO_LIBRARY_DIRS})
target_link_libraries(ImgPopper ${CAIRO_LIBRARIES} glib-2.0)

# ZZIP
pkg_check_modules(LIBARCHIVE REQUIRED libarchive)
#message("${LIBARCHIVE_INCLUDE_DIRS}")
#message("${LIBARCHIVE_LIBRARIES}")
include_directories(${LIBARCHIVE_INCLUDE_DIRS})
#link_directories(${LIBARCHIVE_LIBRARIES})
target_link_libraries(ImgPopper ${LIBARCHIVE_LIBRARIES})
