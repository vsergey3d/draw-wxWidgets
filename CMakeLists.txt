cmake_minimum_required(VERSION 3.3)

set(PROJECT wxdraw)
project(${PROJECT} CXX)

if(WIN32)
    set(PLATFORM WIN32)
endif()

add_definitions(-D_UNICODE -DGLEW_STATIC)
if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic -Wnon-virtual-dtor -Werror")
    set(LIB_TYPE mingw_lib)
elseif(MSVC)
    set(LIB_TYPE msvc_lib)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI /MTd")
endif()

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_DIR ${ROOT_DIR}/src)
set(LIB_DIR ${ROOT_DIR}/lib)

set(SOURCE_FILES
        ${SRC_DIR}/canvas.cpp
        ${SRC_DIR}/frame.cpp)

set(wxWidgets_ROOT_DIR ${LIB_DIR}/wxWidgets)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(wxWidgets_CONFIGURATION mswud)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(wxWidgets_CONFIGURATION mswu)
endif()
find_package(wxWidgets COMPONENTS gl core base REQUIRED)
if (wxWidgets_FOUND)
    INCLUDE(${wxWidgets_USE_FILE})
endif()

find_library(OPENGL opengl)

if(MINGW)
set(LIB_EXT .a)
set(LIB_PREFIX lib)
elseif(MSVC)
set(LIB_EXT .lib)
set(LIB_PREFIX )
endif()
add_library(draw STATIC IMPORTED)
add_library(agg STATIC IMPORTED)
add_library(freetype2 STATIC IMPORTED)
add_library(glew STATIC IMPORTED)
set_target_properties(draw PROPERTIES
        IMPORTED_LOCATION_DEBUG ${LIB_DIR}/draw/lib/${LIB_TYPE}/${LIB_PREFIX}drawd${LIB_EXT}
        IMPORTED_LOCATION ${LIB_DIR}/draw/lib/${LIB_TYPE}/${LIB_PREFIX}draw${LIB_EXT})
set_target_properties(agg PROPERTIES
        IMPORTED_LOCATION_DEBUG ${LIB_DIR}/agg/lib/${LIB_TYPE}/${LIB_PREFIX}aggd${LIB_EXT}
        IMPORTED_LOCATION ${LIB_DIR}/agg/lib/${LIB_TYPE}/${LIB_PREFIX}agg${LIB_EXT})
set_target_properties(freetype2 PROPERTIES
        IMPORTED_LOCATION_DEBUG ${LIB_DIR}/freetype2/lib/${LIB_TYPE}/${LIB_PREFIX}freetyped${LIB_EXT}
        IMPORTED_LOCATION ${LIB_DIR}/freetype2/lib/${LIB_TYPE}/${LIB_PREFIX}freetype${LIB_EXT})
set_target_properties(glew PROPERTIES
        IMPORTED_LOCATION_DEBUG ${LIB_DIR}/glew/lib/${LIB_TYPE}/${LIB_PREFIX}glew32d${LIB_EXT}
        IMPORTED_LOCATION ${LIB_DIR}/glew/lib/${LIB_TYPE}/${LIB_PREFIX}glew32${LIB_EXT})
set(LIBS ${OPENGL_gl_LIBRARY} draw agg freetype2 glew ${wxWidgets_LIBRARIES})

link_directories(${LIB_DIR}/draw/lib/${LIB_TYPE}
        ${LIB_DIR}/agg/lib/${LIB_TYPE}
        ${LIB_DIR}/freetype2/lib/${LIB_TYPE}
        ${LIB_DIR}/glew/lib/${LIB_TYPE})

set(INCLUDE_DIR 
        ${LIB_DIR}/freetype2/include
        ${LIB_DIR}/glew/include
        ${LIB_DIR}/agg/include
        ${LIB_DIR}/draw/include
)
include_directories(${SRC_DIR})
include_directories(SYSTEM ${INCLUDE_DIR} {$OPENGL_INCLUDE_DIR} ${wxWidgets_INCLUDE_DIRS})

add_executable(${PROJECT} ${PLATFORM} ${SOURCE_FILES})
target_link_libraries(${PROJECT} ${LIBS})

set_target_properties(${PROJECT} PROPERTIES DEBUG_POSTFIX d)
set_target_properties(${PROJECT} PROPERTIES FOLDER "runtime")

set_target_properties(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/bin)
set_target_properties(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ROOT_DIR}/bin)
set_target_properties(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ROOT_DIR}/bin)
