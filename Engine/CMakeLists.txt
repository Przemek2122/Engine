cmake_minimum_required(VERSION 3.8)

# Config
set(TOOL_NASM_PATH "C:/Tools/nasm/nasm.exe")

# Create project
project(Engine LANGUAGES CXX)

# Add directories with code
include_directories(${CMAKE_SOURCE_DIR}/Source/Public)
include_directories(${CMAKE_SOURCE_DIR}/Source/Private)
include_directories(${CMAKE_SOURCE_DIR}/Source/ThirdParty)

# Add your main executable
add_executable(Engine Source/Private/Engine.cpp)

# Force CMake to Use NASM
#enable_language(ASM_NASM)
set(CMAKE_ASM_MASM_COMPILER "nasm")
set(CMAKE_ASM_NASM_COMPILER ${TOOL_NASM_PATH})
#set(CMAKE_ASM_NASM_FLAGS "<correct flags>")

# Platform flags
if(WIN32)
    message(STATUS "Configuring for Windows...")
    
    target_compile_definitions(Engine PUBLIC PLATFORM_WINDOWS)
    
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CMAKE_ASM_NASM_OBJECT_FORMAT "win64") # 64-bit Windows
    else()
        set(CMAKE_ASM_NASM_OBJECT_FORMAT "win32") # 32-bit Windows
    endif()
elseif(APPLE)
    message(STATUS "Configuring for macOS...")

    target_compile_definitions(Engine PUBLIC PLATFORM_MAC)

    set(CMAKE_ASM_NASM_OBJECT_FORMAT "macho64") # macOS (assumes 64-bit)
elseif(UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux...")

    target_compile_definitions(Engine PUBLIC PLATFORM_UNIX)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf64") # 64-bit Linux
    else()
        set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf32") # 32-bit Linux
    endif()
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

set(CMAKE_ASM_NASM_COMPILE_OBJECT "<command> -o <OBJECT> <SOURCE>")
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT>")
set(CMAKE_ASM_NASM_LINK_EXECUTABLE "ld <FLAGS> <CMAKE_ASM_NASM_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")

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
    else()
        # Not tested
        find_package(SDL_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL_ttf-shared)
    endif()
else()
    message(FATAL_ERROR "SDL_ttf directory not found. Please check the path.")
    message(FATAL_ERROR "Missing directory could mean missing git submodules.")
endif()

# Add SDL_image
if(EXISTS "${CMAKE_SOURCE_DIR}/../ThirdParty/SDL_image")
    message(STATUS "Adding SDL_image...")
    
    if(ENGINE_VENDORED)
        option(SDLIMAGE_INSTALL "Enable SDL3_image install target" ON)
        set(SDLIMAGE_INSTALL ON CACHE BOOL "Enable SDL3_image install target" FORCE)

        set(SDL_IMAGE_PATH "${CMAKE_SOURCE_DIR}/../ThirdParty/SDL_image")
        set(SDL_IMAGE_BUILD_PATH "${CMAKE_BINARY_DIR}/SDL_image_build")

        add_subdirectory(${SDL_IMAGE_PATH} ${SDL_IMAGE_BUILD_PATH})

        target_link_directories(Engine PRIVATE ${SDL_IMAGE_BUILD_PATH})
    else()
        # Not tested
        find_package(SDL_image REQUIRED CONFIG REQUIRED COMPONENTS SDL_image-shared)
    endif()
else()
    message(FATAL_ERROR "SDL_image directory not found. Please check the path.")
    message(FATAL_ERROR "Missing directory could mean missing git submodules.")
endif()

# Add SDL_mixer
if(EXISTS "${CMAKE_SOURCE_DIR}/../ThirdParty/SDL_mixer")
    message(STATUS "Adding SDL_mixer...")
    
    if(ENGINE_VENDORED)
        option(SDLMIXER_INSTALL "Enable SDL3_mixer install target" ON)
        set(SDLMIXER_INSTALL ON CACHE BOOL "Enable SDL3_mixer install target" FORCE)

        set(SDL_MIXER_PATH "${CMAKE_SOURCE_DIR}/../ThirdParty/SDL_mixer")
        set(SDL_MIXER_BUILD_PATH "${CMAKE_BINARY_DIR}/SDL_mixer_build")

        add_subdirectory(${SDL_MIXER_PATH} ${SDL_MIXER_BUILD_PATH})

        target_link_directories(Engine PRIVATE ${SDL_MIXER_BUILD_PATH})
    else()
        # Not tested
        find_package(SDL_mixer REQUIRED CONFIG REQUIRED COMPONENTS SDL_mixer-shared)
    endif()
else()
    message(FATAL_ERROR "SDL_mixer directory not found. Please check the path.")
    message(FATAL_ERROR "Missing directory could mean missing git submodules.")
endif()

# Link SDL3 to target
target_link_libraries(Engine PUBLIC SDL3::SDL3)

# Link SDL_ttf to target
target_link_libraries(Engine PUBLIC SDL3_ttf)

# Link SDL_mixer to target
target_link_libraries(Engine PUBLIC SDL_image)

# Link SDL3_image to target
target_link_libraries(Engine PUBLIC SDL3_image)
