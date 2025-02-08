cmake_minimum_required(VERSION 3.8)

# Create project
project(Engine LANGUAGES CXX)

# Config
project(Engine VERSION 0.0.1)

# Create an option to switch between a system sdl library and a vendored SDL library
option(ENGINE_VENDORED "Use vendored libraries" ON)
option(ENGINE_BUILD_AS_LIBRARY "ON - build as library, OFF - build as executable" ON)
option(ENGINE_COPY_ASSETS "Copy asssets to target" ON)

set(ENGINE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(ENGINE_SOURCE_PRIVATE_DIR "${ENGINE_SOURCE_DIR}/Private")
set(ENGINE_SOURCE_PUBLIC_DIR "${ENGINE_SOURCE_DIR}/Public")
set(ENGINE_SOURCE_THIRD_PARTY_DIR "${ENGINE_SOURCE_DIR}/ThirdParty")

# Set assets dir name for global access
set(ASSETS_DIR_NAME "Assets")
set_property(GLOBAL PROPERTY ASSETS_DIR_NAME ${ASSETS_DIR_NAME})

# Set the C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all files recursively and set public accesss
file(GLOB_RECURSE ENGINE_SOURCE_FILES_PRIVATE_INTERNAL "${ENGINE_SOURCE_PRIVATE_DIR}/*.cpp")
set_property(GLOBAL PROPERTY ENGINE_SOURCE_FILES_PRIVATE ${ENGINE_SOURCE_FILES_PRIVATE_INTERNAL})

file(GLOB_RECURSE ENGINE_SOURCE_FILES_PUBLIC_INTERNAL "${ENGINE_SOURCE_PUBLIC_DIR}/*.h")
set_property(GLOBAL PROPERTY ENGINE_SOURCE_FILES_PUBLIC ${ENGINE_SOURCE_FILES_PUBLIC_INTERNAL})

file(GLOB_RECURSE ENGINE_SOURCE_FILES_THIRDPARTY_INTERNAL "${ENGINE_SOURCE_THIRD_PARTY_DIR}/*.h")
set_property(GLOBAL PROPERTY ENGINE_SOURCE_FILES_THIRDPARTY ${ENGINE_SOURCE_FILES_THIRDPARTY_INTERNAL})

# Add your main executable
if (ENGINE_BUILD_AS_LIBRARY)
    add_library(Engine STATIC 
        ${ENGINE_SOURCE_FILES_PRIVATE_INTERNAL}
        ${ENGINE_SOURCE_FILES_PUBLIC_INTERNAL}
        ${ENGINE_SOURCE_FILES_THIRDPARTY_INTERNAL}
    )
else()
    add_executable(Engine "${ENGINE_SOURCE_PRIVATE_DIR}/Core/Engine.cpp")
endif()

# Precompiled header
target_precompile_headers(Engine PUBLIC "${ENGINE_SOURCE_PUBLIC_DIR}/Core/CoreEngine.h")

# Add directories with code
target_include_directories(Engine
    PUBLIC ${ENGINE_SOURCE_THIRD_PARTY_DIR}
    PUBLIC ${ENGINE_SOURCE_THIRD_PARTY_DIR}/libuv

    PUBLIC ${ENGINE_SOURCE_PRIVATE_DIR}
    PUBLIC ${ENGINE_SOURCE_PRIVATE_DIR}/Core

    PUBLIC ${ENGINE_SOURCE_PUBLIC_DIR}
    PUBLIC ${ENGINE_SOURCE_PUBLIC_DIR}/Core
)

# Platform flags
if(WIN32)
    message(STATUS "Configuring for Windows...")
    
    target_compile_definitions(Engine PUBLIC PLATFORM_WINDOWS)
    
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CMAKE_ASM_MASM_FLAGS "/nologo /Dx64")
        set(CMAKE_ASM_MASM_COMPILER "ml64")
        enable_language(ASM_MASM)
    else()
        set(CMAKE_ASM_MASM_FLAGS "/nologo")
        set(CMAKE_ASM_MASM_COMPILER "ml")
        enable_language(ASM_MASM)
    endif()

    set(CMAKE_ASM_COMPILE_OBJECT "${CMAKE_ASM_COMPILER} /c /Fo<OBJECT> <SOURCE>")
elseif(APPLE)
    message(STATUS "Configuring for macOS...")

    target_compile_definitions(Engine PUBLIC PLATFORM_MAC)
    
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CMAKE_PLATFORM_FLAGS "macho64")
    else()
        set(CMAKE_PLATFORM_FLAGS "macho32")
    endif()

    set(CMAKE_ASM_COMPILER  "nasm") # Could also be YASM or other probably

    # NASM config
    set(CMAKE_NASM_COMPILER ${CMAKE_ASSEMBLY_COMPILER})
    set(CMAKE_ASM_NASM_COMPILE_OBJECT "${CMAKE_NASM_COMPILER} -f ${CMAKE_PLATFORM_FLAGS} -o <OBJECT> <SOURCE>")
elseif(UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux...")

    target_compile_definitions(Engine PUBLIC PLATFORM_UNIX)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CMAKE_PLATFORM_FLAGS "elf64")
    else()
        set(CMAKE_PLATFORM_FLAGS "elf32")
    endif()

    set(CMAKE_ASM_COMPILER  "nasm") # Could also be YASM or other probably

    # NASM config
    set(CMAKE_NASM_COMPILER ${CMAKE_ASSEMBLY_COMPILER})
    set(CMAKE_ASM_NASM_COMPILE_OBJECT "${CMAKE_NASM_COMPILER} -f ${CMAKE_PLATFORM_FLAGS} -o <OBJECT> <SOURCE>")
else()
    # Add CMAKE_ASM_NASM_FLAGS from link below
    # See https://www.nasm.us/doc/nasmdoc2.html
    set(CMAKE_PLATFORM_FLAGS "PLATFORM_NOT_SET_CHECK_CONFIG")

    message(FATAL_ERROR "Unsupported platform!")
endif()

# Set debug method
if(MSVC AND NOT CMAKE_ASM_NASM_COMPILER)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT EditAndContinue)

    # Enable Hot Reload for MSVC compilers if supported
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
else()
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT ProgramDatabase)
endif()

# Debug/Production flag
target_compile_definitions(Engine
    PUBLIC
    # If building Debug, define DEBUG; otherwise define PRODUCTION
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:RelWithDebInfo>:PRODUCTION>
    $<$<CONFIG:Release>:PRODUCTION>
    $<$<CONFIG:MinSizeRel>:PRODUCTION>
)

# SDL build options
set(SDL_SHARED ON CACHE BOOL "shared libraries" FORCE)
set(SDL_STATIC ON CACHE BOOL "static libraries" FORCE)
set(SDL_TEST OFF CACHE BOOL "SDL test programs" FORCE)

set(ENGINE_BIN_DIRECTORY_PATH "${CMAKE_BINARY_DIR}/bin")

set(EXECUTABLE_OUTPUT_PATH ${ENGINE_BIN_DIRECTORY_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ENGINE_BIN_DIRECTORY_PATH})

set(ASSETS_ENGINE_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_DIR_NAME}")
set(ASSETS_ENGINE_TARGET_PATH "${ENGINE_BIN_DIRECTORY_PATH}/${ASSETS_DIR_NAME}")

# Get assets
file(GLOB ASSET_FILES_TO_COPY "${ASSETS_ENGINE_SOURCE_PATH}/*")

# Print assets dirs
message(STATUS "COPY ENGINE ASSETS: '${ASSETS_ENGINE_SOURCE_PATH}', DESTINATION: '${ASSETS_ENGINE_TARGET_PATH}'")

# Attempt to copy files with a fallback for permission issues
foreach(file ${ASSET_FILES_TO_COPY})
    get_filename_component(filename ${file} NAME)
    if (NOT filename MATCHES "^\\..*" AND EXISTS ${file})
        file(COPY ${file} DESTINATION ${ASSETS_ENGINE_TARGET_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE)
    else()
        message(WARNING "Could not access file: ${file} (skipping)")
    endif()
endforeach()

set(ENGINE_LIB_DIRECTORY_PATH "${CMAKE_BINARY_DIR}/lib/${ENGINE_CONFIGURATION}")

# Add SDL3
message(STATUS "Adding SDL...")
    
if(ENGINE_VENDORED)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL")
        set(SDL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL")
        set(SDL_BUILD_PATH "${CMAKE_BINARY_DIR}/SDL_build")

        # This assumes you have added SDL as a submodule in vendored/SDL
        add_subdirectory(${SDL_PATH} ${SDL_BUILD_PATH})
        
        # After build copy includes
        set(INCLUDES_DIR ${SDL_PATH}/include/SDL3)
        set(DESTINATION_DIR ${ENGINE_SOURCE_THIRD_PARTY_DIR})
        
        file(COPY ${INCLUDES_DIR} DESTINATION ${DESTINATION_DIR})
    else()
        message(FATAL_ERROR "SDL directory not found. Please check the path.")
        message(FATAL_ERROR "Missing directory could mean missing git submodules.")
    endif()
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED)
endif()

# Add SDL_ttf
message(STATUS "Adding SDL_ttf...")
    
if(ENGINE_VENDORED)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL_ttf")
        option(SDLTTF_INSTALL "Enable SDL3_ttf install target" ON)
        set(SDLTTF_INSTALL ON CACHE BOOL "Enable SDL3_ttf install target" FORCE)

        set(SDL_TTF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL_ttf")
        set(SDL_TTF_BUILD_PATH "${CMAKE_BINARY_DIR}/SDL_ttf_build")

        add_subdirectory(${SDL_TTF_PATH} ${SDL_TTF_BUILD_PATH})

        target_link_directories(Engine PUBLIC ${SDL_TTF_BUILD_PATH})

        # After build copy includes
        set(INCLUDES_DIR ${SDL_TTF_PATH}/include/SDL3_ttf)
        set(DESTINATION_DIR ${ENGINE_SOURCE_THIRD_PARTY_DIR})
        
        file(COPY ${INCLUDES_DIR} DESTINATION ${DESTINATION_DIR})
    else()
        message(FATAL_ERROR "SDL_ttf directory not found. Please check the path.")
        message(FATAL_ERROR "Missing directory could mean missing git submodules.")
    endif()
else()
    # Not tested
    find_package(SDL3_ttf REQUIRED)
endif()

# Add SDL_mixer
message(STATUS "Adding SDL_mixer...")
    
if(ENGINE_VENDORED)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL_mixer")
        option(SDLMIXER_INSTALL "Enable SDL3_mixer install target" ON)
        set(SDLMIXER_INSTALL ON CACHE BOOL "Enable SDL3_mixer install target" FORCE)

        set(SDL_MIXER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL_mixer")
        set(SDL_MIXER_BUILD_PATH "${CMAKE_BINARY_DIR}/SDL_mixer_build")

        set(WAVPACK_ENABLE_ASM OFF CACHE BOOL "Disable assembly optimizations" FORCE)

        add_subdirectory(${SDL_MIXER_PATH} ${SDL_MIXER_BUILD_PATH})

        target_link_directories(Engine PUBLIC ${SDL_MIXER_BUILD_PATH})
        
        # After build copy includes
        set(INCLUDES_DIR ${SDL_MIXER_PATH}/include/SDL3_mixer)
        set(DESTINATION_DIR ${ENGINE_SOURCE_THIRD_PARTY_DIR})
        
        file(COPY ${INCLUDES_DIR} DESTINATION ${DESTINATION_DIR})
    else()
        message(FATAL_ERROR "SDL_mixer directory not found. Please check the path.")
        message(FATAL_ERROR "Missing directory could mean missing git submodules.")
    endif()
else()
    # Not tested
    find_package(SDL3_mixer REQUIRED)
endif()

# Add SDL_image
message(STATUS "Adding SDL_image...")

if(ENGINE_VENDORED)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL_image")
        # Disable library which causes compilation issue on windows
        set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
        # Disable optional SDL_Image
        set(SDLIMAGE_TESTS OFF CACHE BOOL "" FORCE)
        set(SDLIMAGE_TESTS_INSTALL OFF CACHE BOOL "" FORCE)
        set(SDLIMAGE_SAMPLES OFF CACHE BOOL "" FORCE)

        option(SDLIMAGE_INSTALL "Enable SDL3_image install target" ON)
        set(SDLIMAGE_INSTALL ON CACHE BOOL "Enable SDL3_image install target" FORCE)

        set(SDL_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL_image")
        set(SDL_IMAGE_BUILD_PATH "${CMAKE_BINARY_DIR}/SDL_image_build")

        add_subdirectory(${SDL_IMAGE_PATH} ${SDL_IMAGE_BUILD_PATH})

        target_link_directories(Engine PUBLIC ${SDL_IMAGE_BUILD_PATH})
        
        # After build copy includes
        set(INCLUDES_DIR ${SDL_IMAGE_PATH}/include/SDL3_image)
        set(DESTINATION_DIR ${ENGINE_SOURCE_THIRD_PARTY_DIR})
        
        file(COPY ${INCLUDES_DIR} DESTINATION ${DESTINATION_DIR})
    else()
        message(FATAL_ERROR "SDL_image directory not found. Please check the path.")
        message(FATAL_ERROR "Missing directory could mean missing git submodules.")
    endif()
else()
    # Not tested
    find_package(SDL3_image REQUIRED)
endif()

# Add libuv
message(STATUS "Adding libuv...")

if(ENGINE_VENDORED)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/libuv")
        set(LIBUV_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/libuv")
        set(LIBUV_BUILD_PATH "${CMAKE_BINARY_DIR}/libuv_build")
    
        add_subdirectory(${LIBUV_PATH} ${LIBUV_BUILD_PATH})

        target_link_directories(Engine PUBLIC ${LIBUV_BUILD_PATH})

        # After build copy includes
        set(INCLUDES_LIBVW_DIR ${LIBUV_PATH}/include/uv)
        set(INCLUDES_LIBUV_H ${LIBUV_PATH}/include/uv.h)
        set(DESTINATION_DIR ${ENGINE_SOURCE_THIRD_PARTY_DIR}/libuv)
        
        file(COPY ${INCLUDES_LIBVW_DIR} DESTINATION ${DESTINATION_DIR})
        file(COPY ${INCLUDES_LIBUV_H} DESTINATION ${DESTINATION_DIR})
    else()
        message(FATAL_ERROR "SDL_image directory not found. Please check the path.")
        message(FATAL_ERROR "Missing directory could mean missing git submodules.")
    endif()
else()
    # Not tested
    find_package(uv REQUIRED)
endif()

# Add uvw
message(STATUS "Adding uvw...")

if(ENGINE_VENDORED)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/uvw")
        set(UVW_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/uvw")
        set(UVW_BUILD_PATH "${CMAKE_BINARY_DIR}/uvw_build")
    
        add_subdirectory(${UVW_PATH} ${UVW_BUILD_PATH})

        target_link_directories(Engine PUBLIC ${UVW_BUILD_PATH})

        # After build copy includes
        set(INCLUDES_UVW_DIR ${UVW_PATH}/src/uvw)
        set(INCLUDES_UVW_HEADER ${UVW_PATH}/src/uvw.hpp)
        set(DESTINATION_DIR ${ENGINE_SOURCE_THIRD_PARTY_DIR})
        
        file(COPY ${INCLUDES_UVW_DIR} DESTINATION ${DESTINATION_DIR})
        file(COPY ${INCLUDES_UVW_HEADER} DESTINATION ${DESTINATION_DIR})
    else()
        message(FATAL_ERROR "SDL_image directory not found. Please check the path.")
        message(FATAL_ERROR "Missing directory could mean missing git submodules.")
    endif()
else()
    # Not tested
    find_package(uvw REQUIRED)
endif()

# Link SDL3 to target
target_link_libraries(Engine PUBLIC SDL3::SDL3)

# Link SDL_ttf to target
target_link_libraries(Engine PUBLIC SDL3_ttf)

# Link SDL_mixer to target
target_link_libraries(Engine PUBLIC SDL3_mixer)

# Link SDL3_image to target
target_link_libraries(Engine PUBLIC SDL3_image)

# Link uv to target
target_link_libraries(Engine PUBLIC uv)

# Link uvw to target
target_link_libraries(Engine PUBLIC uvw)
