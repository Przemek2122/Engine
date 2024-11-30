cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Create project
project(Engine LANGUAGES CXX)

# Add directories with code
include_directories(${CMAKE_SOURCE_DIR}/Source/Public)
include_directories(${CMAKE_SOURCE_DIR}/Source/Private)
include_directories(${CMAKE_SOURCE_DIR}/Source/ThirdParty)

# Add your main executable
add_executable(Engine Source/Private/Engine.cpp)

# Platform flags
if(WIN32)
    target_compile_definitions(Engine PUBLIC PLATFORM_WINDOWS)
elseif(UNIX)
    target_compile_definitions(Engine PUBLIC PLATFORM_UNIX)
endif()

# Debug/Production flag
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(Engine PUBLIC DEBUG)
else()
    target_compile_definitions(Engine PUBLIC PRODUCTION)
endif()

# SDL build options
set(SDL_SHARED ON CACHE BOOL "shared libraries" FORCE)
set(SDL_STATIC ON CACHE BOOL "static libraries" FORCE)
set(SDL_TEST OFF CACHE BOOL "SDL test programs" FORCE)

# Create an option to switch between a system sdl library and a vendored SDL library
option(ENGINE_VENDORED "Use vendored libraries" ON)

# Set directories
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(ENGINE_BIN_DIRECTORY_PATH "${CMAKE_BINARY_DIR}/bin/Debug")
else()
    set(ENGINE_BIN_DIRECTORY_PATH "${CMAKE_BINARY_DIR}/bin/Release")
endif()

set(EXECUTABLE_OUTPUT_PATH ${ENGINE_BIN_DIRECTORY_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ENGINE_BIN_DIRECTORY_PATH})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(ENGINE_LIB_DIRECTORY_PATH "${CMAKE_BINARY_DIR}/lib/Debug")
else()
    set(ENGINE_LIB_DIRECTORY_PATH "${CMAKE_BINARY_DIR}/lib/Release")
endif()

# Add SDL3
if(EXISTS "${CMAKE_SOURCE_DIR}/../ThirdParty/SDL")
    message(STATUS "Adding SDL...")
    
    if(ENGINE_VENDORED)
        set(SDL_PATH "${CMAKE_SOURCE_DIR}/../ThirdParty/SDL")
        set(SDL_BUILD_PATH "${CMAKE_BINARY_DIR}/SDL_build")

        # This assumes you have added SDL as a submodule in vendored/SDL
        add_subdirectory(${SDL_PATH} ${SDL_BUILD_PATH})

        # Copy dynamic libraries
        #[[
        if(WIN32)
            add_custom_command(TARGET Engine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDL_BUILD_PATH}/SDL3.dll
                ${ENGINE_BIN_DIRECTORY_PATH}/SDL3.dll)
        elseif(APPLE)
            add_custom_command(TARGET Engine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDL_BUILD_PATH}/libSDL3.dylib
                ${ENGINE_BIN_DIRECTORY_PATH}/libSDL3.dylib)
        elseif(UNIX)
            add_custom_command(TARGET Engine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDL_BUILD_PATH}/libSDL3.so
                ${ENGINE_BIN_DIRECTORY_PATH}/libSDL3.so)
        endif()
        ]]
    else()
        # 1. Look for a SDL3 package,
        # 2. look for the SDL3-shared component, and
        # 3. fail if the shared component cannot be found.
        find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
    endif()
else()
    message(FATAL_ERROR "SDL directory not found. Please check the path.")
    message(FATAL_ERROR "Missing directory could mean missing git submodules.")
endif()

# Add SDL_ttf
if(EXISTS "${CMAKE_SOURCE_DIR}/../ThirdParty/SDL_ttf")
    message(STATUS "Adding SDL_ttf...")
    
    if(ENGINE_VENDORED)
        option(SDLTTF_INSTALL "Enable SDL3_ttf install target" ON)
        set(SDLTTF_INSTALL ON CACHE BOOL "Enable SDL3_ttf install target" FORCE)

        set(SDL_TTF_PATH "${CMAKE_SOURCE_DIR}/../ThirdParty/SDL_ttf")
        set(SDL_TTF_BUILD_PATH "${CMAKE_BINARY_DIR}/SDL_ttf_build")

        add_subdirectory(${SDL_TTF_PATH} ${SDL_TTF_BUILD_PATH})

        target_link_directories(Engine PRIVATE ${SDL_TTF_BUILD_PATH})

        # Copy dynamic libraries
        #[[
        if(WIN32)
            add_custom_command(TARGET Engine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDL_TTF_BUILD_PATH}/SDL3_ttf.dll
                ${ENGINE_BIN_DIRECTORY_PATH}/SDL3_ttf.dll)
        elseif(APPLE)
            add_custom_command(TARGET Engine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDL_TTF_BUILD_PATH}/libSDL3_ttf.dylib
                ${ENGINE_BIN_DIRECTORY_PATH}/libSDL3_ttf.dylib)
        elseif(UNIX)
            add_custom_command(TARGET Engine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDL_TTF_BUILD_PATH}/libSDL3_ttf.so
                ${ENGINE_BIN_DIRECTORY_PATH}/libSDL3_ttf.so)
        endif()
        ]]
    else()
        # Not tested
        find_package(SDL_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL_ttf-shared)
    endif()
else()
    message(FATAL_ERROR "SDL_ttf directory not found. Please check the path.")
    message(FATAL_ERROR "Missing directory could mean missing git submodules.")
endif()

# Link SDL3 to target
target_link_libraries(Engine PUBLIC SDL3::SDL3)

# Link SDL_ttf to target
target_link_libraries(Engine PUBLIC SDL3_ttf)
