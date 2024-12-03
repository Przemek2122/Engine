# Install script for directory: C:/Repos/Engine/ThirdParty/SDL_image/external/aom

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Repos/Engine/Engine/../output/install/x64-debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Tools/strawbery_perl/c/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aom" TYPE FILE FILES
    "C:/Repos/Engine/ThirdParty/SDL_image/external/aom/aom/aom.h"
    "C:/Repos/Engine/ThirdParty/SDL_image/external/aom/aom/aom_codec.h"
    "C:/Repos/Engine/ThirdParty/SDL_image/external/aom/aom/aom_frame_buffer.h"
    "C:/Repos/Engine/ThirdParty/SDL_image/external/aom/aom/aom_image.h"
    "C:/Repos/Engine/ThirdParty/SDL_image/external/aom/aom/aom_integer.h"
    "C:/Repos/Engine/ThirdParty/SDL_image/external/aom/aom/aom_decoder.h"
    "C:/Repos/Engine/ThirdParty/SDL_image/external/aom/aom/aomdx.h"
    "C:/Repos/Engine/ThirdParty/SDL_image/external/aom/aom/aomcx.h"
    "C:/Repos/Engine/ThirdParty/SDL_image/external/aom/aom/aom_encoder.h"
    "C:/Repos/Engine/ThirdParty/SDL_image/external/aom/aom/aom_external_partition.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Repos/Engine/output/build/x64-debug/SDL_image_build/external/aom/aom.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Repos/Engine/output/build/x64-debug/SDL_image_build/external/aom/aom.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Repos/Engine/output/build/x64-debug/bin/Debug/aom.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aom.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aom.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Tools/strawbery_perl/c/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aom.dll")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Repos/Engine/output/build/x64-debug/SDL_image_build/external/aom/aom_static.lib")
endif()

