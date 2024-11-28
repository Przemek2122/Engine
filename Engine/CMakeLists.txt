cmake_minimum_required(VERSION 3.16)

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

# Set 34put directories
set_target_properties(Engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
)

# SDL build options
set(SDL_SHARED OFF CACHE BOOL "Disable shared libraries" FORCE)
set(SDL_STATIC ON CACHE BOOL "Enable static libraries" FORCE)
set(SDL_TEST OFF CACHE BOOL "Disable SDL test programs" FORCE)

# Create an option to switch between a system sdl library and a vendored SDL library
option(ENGINE_VENDORED "Use vendored libraries" ON)

# Add SDL3
if(EXISTS "${CMAKE_SOURCE_DIR}/../ThirdParty/SDL")
    message(STATUS "Adding SDL...")
    
    if(ENGINE_VENDORED)
        # This assumes you have added SDL as a submodule in vendored/SDL
        add_subdirectory("${CMAKE_SOURCE_DIR}/../ThirdParty/SDL" "${CMAKE_BINARY_DIR}/SDL_build")
        #find_package(SDL2 REQUIRED)
        
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
#[[
if(EXISTS "${CMAKE_SOURCE_DIR}/../ThirdParty/SDL_ttf")
    message(STATUS "Adding SDL_ttf...")
    
    if(ENGINE_VENDORED)
        target_link_libraries(Exe ${SDL3_LIBRARIES} ${SDL3_IMAGE_LIBRARIES})

        # This assumes you have added SDL as a submodule in vendored/SDL
        add_subdirectory("${CMAKE_SOURCE_DIR}/../ThirdParty/SDL_ttf" "${CMAKE_BINARY_DIR}/SDL_ttf_build")
    else()
        # Not tested
        find_package(SDL_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL_ttf-shared)
    endif()
else()
    message(FATAL_ERROR "SDL_ttf directory not found. Please check the path.")
    message(FATAL_ERROR "Missing directory could mean missing git submodules.")
endif()
]]

# Link SDL3 and SDL_image to your project
target_link_libraries(Engine PUBLIC SDL3::SDL3)
