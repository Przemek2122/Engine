# Install script for directory: C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Repos/Engine/output/build/x64-debug/SDL_mixer_build/external/ogg/ogg.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Repos/Engine/output/build/x64-debug/bin/Debug/ogg-0.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ogg-0.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ogg-0.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Tools/strawbery_perl/c/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ogg-0.dll")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Repos/Engine/output/build/x64-debug/SDL_mixer_build/external/ogg/ogg.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/SDL3_mixer/html" TYPE FILE FILES
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/framing.html"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/index.html"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/oggstream.html"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/ogg-multiplex.html"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/fish_xiph_org.png"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/multiplex1.png"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/packets.png"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/pages.png"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/stream.png"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/vorbisword2.png"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/white-ogg.png"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/white-xifish.png"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/rfc3533.txt"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/rfc5334.txt"
    "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/skeleton.html"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/SDL3_mixer/html" TYPE DIRECTORY FILES "C:/Repos/Engine/ThirdParty/SDL_mixer/external/ogg/doc/libogg")
endif()

