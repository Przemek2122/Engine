cmake_minimum_required (VERSION 3.1.1...4.1.1)

# Basic config
# Create an option to switch between a system sdl library and a vendored SDL library
option(ENGINE_VENDORED "Use vendored libraries" ON)
option(ENGINE_BUILD_AS_LIBRARY "ON - build as library, OFF - build as executable" ON)
option(ENGINE_COPY_ASSETS "Copy asssets to target" ON)
option(ENGINE_COPY_ENGINE_MAPS "Should copy asssets to target?" ON)

# Do this app use any video features?
option(ENGINE_USING_VIDEO "Is this app using any video features? (like SDL Video)" ON)
option(ENGINE_USING_AUDIO "Is this app using any audio features? (liek SDL Audio)" ON)

# Crow and asio are for HTTP, 
# Note: using sockets from Crow is higly NOT recomended
option(USE_LIBRARY_ASIO "Should we use asio library? (Crow requires asio)" ON)
option(USE_LIBRARY_CROW "Should we use crow library?" ON)

# Socket library support
option(USE_LIBRARY_BOOST_BEAST "Should we use boost beast library?" OFF)
option(USE_LIBRARY_UWEBSOCKETS "Should we use uWebSockets library?" ON)

# SOCI is for MySQL (db connection)
option(USE_LIBRARY_SOCI "Should we use SOCI library for db connecions?" ON)
option(SOCI_ENABLE_MYSQL "Enable MySQL backend" ON)
set(MYSQL_CONNECTOR_PATH "C:/Program Files/MySQL/MySQL Connector C++ 9.5" CACHE PATH "MYSQL Connector path")
option(SOCI_ENABLE_POSTGRESQL "Enable PostgreSQL backend" OFF)
option(SOCI_ENABLE_SQLITE3 "Enable SQLite3 backend" OFF)
option(SOCI_ENABLE_ORACLE "Enable Oracle backend" OFF)
option(SOCI_ENABLE_ODBC "Enable ODBC backend" OFF)
option(SOCI_ENABLE_FIREBIRD "Enable Firebird backend" OFF)
option(SOCI_ENABLE_DB2 "Enable DB2 backend" OFF)

set(ENGINE_VERSION "0.1")

set(ENGINE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Sources")
set(ENGINE_SOURCE_PRIVATE_DIR "${ENGINE_SOURCE_DIR}/Private")
set(ENGINE_SOURCE_PUBLIC_DIR "${ENGINE_SOURCE_DIR}/Public")
set(ENGINE_SOURCE_THIRD_PARTY_DIR "${ENGINE_SOURCE_DIR}/ThirdParty")

# Set the C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Hopefully temporary hack after upgrading CMAKE VERSION
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Set assets dir name for global access
set(ASSETS_DIR_NAME "Assets")
set_property(GLOBAL PROPERTY ASSETS_DIR_NAME ${ASSETS_DIR_NAME})

# Find VCPKG OR INSTALL FOR THIS BUILD ONLY (must be BEFORE project())
if(NOT DEFINED VCPKG_INSTALLED_TRIPLET_DIR AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	if(WIN32)
		execute_process(
			COMMAND cmd /c bootstrap-vcpkg.bat
			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/vcpkg
			RESULT_VARIABLE result
			OUTPUT_VARIABLE output
			ERROR_VARIABLE error
		)
	elseif(APPLE)
		execute_process(
			COMMAND bash bootstrap-vcpkg.sh
			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/vcpkg
			RESULT_VARIABLE result
			OUTPUT_VARIABLE output
			ERROR_VARIABLE error
		)
	elseif(UNIX)
		execute_process(
			COMMAND bash bootstrap-vcpkg.sh
			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/vcpkg
			RESULT_VARIABLE result
			OUTPUT_VARIABLE output
			ERROR_VARIABLE error
		)
	endif()

	if(VCPKG_EXE)
		# Get directory from vcpkg.exe path
		get_filename_component(VCPKG_DIR "${VCPKG_EXE}" DIRECTORY)
		set(VCPKG_TOOLCHAIN CACHE FILEPATH "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake")

		if(EXISTS "${VCPKG_TOOLCHAIN}")
			set(CMAKE_TOOLCHAIN_FILE "${VCPKG_TOOLCHAIN}" CACHE FILEPATH "Vcpkg toolchain")
			set(VCPKG_ROOT_DIR "${VCPKG_DIR}" CACHE PATH "Vcpkg root")
			message(STATUS "Auto-detected vcpkg: ${VCPKG_DIR}")
		endif()
	else()
		message(WARNING "VCPKG not found in PATH")
		message(STATUS "Will install VCPKG for this build")

		execute_process(COMMAND git clone https://github.com/microsoft/vcpkg.git)

		if(WIN32)
			execute_process(
				COMMAND cmd /c bootstrap-vcpkg.bat
				WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/vcpkg
				RESULT_VARIABLE result
				OUTPUT_VARIABLE output
				ERROR_VARIABLE error
			)
		elseif(APPLE)
			execute_process(
				COMMAND bash bootstrap-vcpkg.sh
				WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/vcpkg
				RESULT_VARIABLE result
				OUTPUT_VARIABLE output
				ERROR_VARIABLE error
			)
		elseif(UNIX)
			execute_process(
				COMMAND bash bootstrap-vcpkg.sh
				WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/vcpkg
				RESULT_VARIABLE result
				OUTPUT_VARIABLE output
				ERROR_VARIABLE error
			)
		endif()

		if(result)
			message(WARNING "Were running ${PROJECT_BINARY_DIR}/vcpkg/bootstrap-vcpkg script")
			message(WARNING "vcpkg bootstrap failed: ${result}\n${output}\n${error}")
			message(FATAL_ERROR "vcpkg bootstrap failed! (Log above)")
		else()
			set(VCPKG_ROOT_DIR "${PROJECT_BINARY_DIR}/vcpkg" CACHE INTERNAL "Path to vcpkg")
			set(VCPKG_INSTALLED_DIR "${VCPKG_ROOT_DIR}/installed" CACHE INTERNAL "Path to vcpkg/installed")

			message(STATUS "VCPKG installed for this build successfully.")
			message(WARNING "Global VCPKG would speed up the build.")
		endif()
	endif()
endif()

if(EXISTS "${VCPKG_TOOLCHAIN}")
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_TOOLCHAIN}")
elseif(EXISTS "${VCPKG_ROOT_DIR}")
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT_DIR}/scripts/buildsystems/vcpkg.cmake")
else()
	message(FATAL_ERROR "Missing VCPKG!")
endif()

# Set triplet for VCPKG (BEFORE project())
if(NOT DEFINED VCPKG_TARGET_TRIPLET)
    if(WIN32)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")
        else()
            set(VCPKG_TARGET_TRIPLET "x86-windows" CACHE STRING "")
        endif()
    else()
        # Linux/Mac
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "")
        else()
            set(VCPKG_TARGET_TRIPLET "x86-linux" CACHE STRING "")
        endif()
    endif()
endif()

# Set location for VCPKG content
if(NOT DEFINED VCPKG_INSTALLED_TRIPLET_DIR AND VCPKG_ROOT_DIR AND VCPKG_TARGET_TRIPLET)
    set(VCPKG_INSTALLED_TRIPLET_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}" CACHE INTERNAL "vcpkg/installed/triplet")
    message(STATUS "VCPKG_INSTALLED_TRIPLET_DIR: ${VCPKG_INSTALLED_TRIPLET_DIR}")
endif()

if(DEFINED VCPKG_ROOT_DIR)
    link_directories("${VCPKG_INSTALLED_TRIPLET_DIR}/lib")
endif()

function(install_package PACKAGE_NAME)
    message(STATUS "Installing ${PACKAGE_NAME} via vcpkg...")
    
    if(WIN32)
        set(VCPKG_EXEC "${VCPKG_ROOT_DIR}/vcpkg.exe")
    else()
        set(VCPKG_EXEC "${VCPKG_ROOT_DIR}/vcpkg")
    endif()
    
    execute_process(
        COMMAND ${VCPKG_EXEC} install --recurse ${PACKAGE_NAME}
        RESULT_VARIABLE VCPKG_RESULT
    )
    
    if(VCPKG_RESULT EQUAL 0)
        message(STATUS "${PACKAGE_NAME} installed successfully")
    else()
        message(FATAL_ERROR "Failed to install ${PACKAGE_NAME}")
    endif()
endfunction()

message(STATUS "VCPKG_TARGET_TRIPLET: '${VCPKG_TARGET_TRIPLET}'")
message(STATUS "Installing required vcpkg packages:")

if (DEFINED VCPKG_ROOT_DIR)
	# List of vcpkg packages to install
	install_package(OpenSSL)
	
	if(WIN32 AND VCPKG_INSTALLED_TRIPLET_DIR AND SOCI_ENABLE_MYSQL)
		install_package(libmysql)
	endif()
	
	if (NOT ENGINE_VENDORED)
		install_package(sdl3)
		if (ENGINE_USING_VIDEO)
			install_package(sdl3-image)
			install_package(sdl3-image[jpeg, png])
			install_package(sdl3-ttf)
		endif()
	endif()
	
	if (USE_LIBRARY_BOOST_BEAST)
		install_package(boost)
		install_package(boost-beast)
	endif()		

	if (USE_LIBRARY_UWEBSOCKETS)
		install_package(uSockets)
		install_package(libuv)
		install_package(uwebsockets[ssl,zlib])
	endif()
endif()

# Create project
project(Engine VERSION ${ENGINE_VERSION} LANGUAGES CXX)

if (ENGINE_USING_VIDEO)
	target_compile_definitions(Engine PUBLIC
		ENGINE_USING_VIDEO=1
	)
endif()

if (ENGINE_USING_AUDIO)
	target_compile_definitions(Engine PUBLIC
		ENGINE_USING_AUDIO=1
	)
endif()



if (APPLE OR EMSCRIPTEN)
	# Disable shared builds on platforms where it does not make sense to use them
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
endif()

if(MSVC)
    if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
        # parallelize each target, unless Ninja is the generator
		add_definitions(/MP)
    endif()
endif()

# on Web targets, we need CMake to generate a HTML webpage. 
if(EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
endif()

# Enable solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Collect all files recursively and set public accesss
file(GLOB_RECURSE ENGINE_SOURCE_FILES_PRIVATE_INTERNAL "${ENGINE_SOURCE_PRIVATE_DIR}/*.cpp")
set_property(GLOBAL PROPERTY ENGINE_SOURCE_FILES_PRIVATE ${ENGINE_SOURCE_FILES_PRIVATE_INTERNAL})

file(GLOB_RECURSE ENGINE_SOURCE_FILES_PUBLIC_INTERNAL "${ENGINE_SOURCE_PUBLIC_DIR}/*.h")
set_property(GLOBAL PROPERTY ENGINE_SOURCE_FILES_PUBLIC ${ENGINE_SOURCE_FILES_PUBLIC_INTERNAL})

file(GLOB_RECURSE ENGINE_SOURCE_FILES_THIRDPARTY_INTERNAL "${ENGINE_SOURCE_THIRD_PARTY_DIR}/*.h")
set_property(GLOBAL PROPERTY ENGINE_SOURCE_FILES_THIRDPARTY ${ENGINE_SOURCE_FILES_THIRDPARTY_INTERNAL})

if (ENGINE_BUILD_AS_LIBRARY)
    add_compile_definitions(ENGINE_BUILD_AS_LIBRARY=1)

    add_library(Engine SHARED 
        ${ENGINE_SOURCE_FILES_PRIVATE_INTERNAL}
        ${ENGINE_SOURCE_FILES_PUBLIC_INTERNAL}
        ${ENGINE_SOURCE_FILES_THIRDPARTY_INTERNAL}
    )
else()
    add_compile_definitions(ENGINE_BUILD_AS_LIBRARY=0)

    add_executable(Engine "${ENGINE_SOURCE_PRIVATE_DIR}/Core/Engine.cpp")
endif()

# Get all runtime dependencies (DLLs/SO files) from Engine target
if(WIN32)
    set(RUNTIME_LIB_EXT ".dll")
elseif(APPLE)
    set(RUNTIME_LIB_EXT ".dylib")
else()
    set(RUNTIME_LIB_EXT ".so")
endif()

# Set global property
set_property(GLOBAL PROPERTY RUNTIME_LIB_EXT ${RUNTIME_LIB_EXT})

# Precompiled header
target_precompile_headers(Engine PUBLIC "${ENGINE_SOURCE_PUBLIC_DIR}/Core/CoreEngine.h")

# Add directories with code
target_include_directories(Engine
    PUBLIC ${ENGINE_SOURCE_THIRD_PARTY_DIR}

    PUBLIC ${ENGINE_SOURCE_PRIVATE_DIR}
    PUBLIC ${ENGINE_SOURCE_PRIVATE_DIR}/Core

    PUBLIC ${ENGINE_SOURCE_PUBLIC_DIR}
    PUBLIC ${ENGINE_SOURCE_PUBLIC_DIR}/Core
)

# Add the files to your target
target_sources(Engine
    PUBLIC ${ENGINE_SOURCE_FILES_PUBLIC_INTERNAL}
    PRIVATE ${ENGINE_SOURCE_FILES_PRIVATE_INTERNAL}
    PUBLIC ${ENGINE_SOURCE_FILES_THIRDPARTY_INTERNAL}
)

# Source dirs
source_group(TREE ${ENGINE_SOURCE_DIR} PREFIX "Sources" FILES ${ENGINE_SOURCE_FILES_PRIVATE_INTERNAL})
source_group(TREE ${ENGINE_SOURCE_DIR} PREFIX "Sources" FILES ${ENGINE_SOURCE_FILES_PUBLIC_INTERNAL})
source_group(TREE ${ENGINE_SOURCE_DIR} PREFIX "Sources" FILES ${ENGINE_SOURCE_FILES_THIRDPARTY_INTERNAL})

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

    set(CMAKE_ASM_COMPILER  "nasm")

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

set_property(GLOBAL PROPERTY ASSETS_ENGINE_TARGET_PATH ${ASSETS_ENGINE_TARGET_PATH})

# Get assets
file(GLOB ASSET_FILES_TO_COPY "${ASSETS_ENGINE_SOURCE_PATH}/*")

# Print assets dirs
message(STATUS "COPY ENGINE ASSETS: '${ASSETS_ENGINE_SOURCE_PATH}', DESTINATION: '${ASSETS_ENGINE_TARGET_PATH}'")

# Attempt to copy files with a fallback for permission issues
foreach(file ${ASSET_FILES_TO_COPY})
    get_filename_component(filename ${file} NAME)
    if (NOT filename MATCHES "^\\..*" AND EXISTS ${file})
        if (ENGINE_COPY_ENGINE_MAPS OR (NOT filename MATCHES "[\/\\]Maps[\/\\]"))
            file(COPY ${file} DESTINATION ${ASSETS_ENGINE_TARGET_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE)
        endif()
    else()
        message(WARNING "Could not access file: ${file} (skipping)")
    endif()
endforeach()

set(ENGINE_LIB_DIRECTORY_PATH "${CMAKE_BINARY_DIR}/lib/${ENGINE_CONFIGURATION}")

message(STATUS "Adding prebuilt libraries...")

message(STATUS "==== Debug CMake paths ====")
message(STATUS "CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "VCPKG_ROOT_DIR = ${VCPKG_ROOT_DIR}")
message(STATUS "VCPKG_TARGET_TRIPLET = ${VCPKG_TARGET_TRIPLET}")
message(STATUS "VCPKG_INSTALLED_DIR = ${VCPKG_INSTALLED_DIR}")
message(STATUS "VCPKG_INSTALLED_TRIPLET_DIR = ${VCPKG_INSTALLED_TRIPLET_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "==== End Debug ====")

# Help CMake find OpenSSL from vcpkg
set(OPENSSL_ROOT_DIR "${VCPKG_INSTALLED_TRIPLET_DIR}")

# Try to find OpenSSL
find_package(OpenSSL MODULE REQUIRED)
target_link_libraries(Engine PUBLIC OpenSSL::Crypto OpenSSL::SSL)

if(NOT OpenSSL_FOUND)
	message(FATAL_ERROR "OpenSSL missing! Check package, probably missing")
endif()

# Boost beast
if (USE_LIBRARY_BOOST_BEAST)
    find_path(BOOST_INCLUDE_DIR boost/beast.hpp)
    target_include_directories(Engine PRIVATE ${BOOST_INCLUDE_DIR})
	
	target_compile_definitions(Engine PUBLIC USE_LIBRARY_BOOST_BEAST=1)
else()
	target_compile_definitions(Engine PUBLIC USE_LIBRARY_BOOST_BEAST=0)
endif()

# uWebSockets
if(USE_LIBRARY_UWEBSOCKETS)
    find_path(UWEBSOCKETS_INCLUDE_DIR uwebsockets/App.h)

	find_library(ZLIB_LIB_DEBUG NAMES zlibd zlib z 
		PATHS "${VCPKG_INSTALLED_TRIPLET_DIR}/debug/lib" NO_DEFAULT_PATH
	)
	find_library(ZLIB_LIB_RELEASE NAMES zlib z 
		PATHS "${VCPKG_INSTALLED_TRIPLET_DIR}/lib" NO_DEFAULT_PATH
	)
	
	if(NOT ZLIB_LIB_DEBUG OR NOT ZLIB_LIB_RELEASE)
		message(FATAL_ERROR "zlib lib not found!")
	endif()
	
	find_library(LIBUV_LIB_DEBUG NAMES uv_a uv 
		PATHS "${VCPKG_INSTALLED_TRIPLET_DIR}/debug/lib" NO_DEFAULT_PATH
	)
	find_library(LIBUV_LIB_RELEASE NAMES uv_a uv 
		PATHS "${VCPKG_INSTALLED_TRIPLET_DIR}/lib" NO_DEFAULT_PATH
	)
	
	if(NOT LIBUV_LIB_DEBUG OR NOT LIBUV_LIB_RELEASE)
		message(FATAL_ERROR "uv lib not found!")
	endif()
	
	target_link_libraries(Engine PUBLIC 
		$<$<CONFIG:Debug>:${LIBUV_LIB_DEBUG}>
		$<$<CONFIG:Release>:${LIBUV_LIB_RELEASE}>
		$<$<CONFIG:Debug>:${ZLIB_LIB_DEBUG}>
		$<$<CONFIG:Release>:${ZLIB_LIB_RELEASE}>
	)
    
    find_library(USOCKETS_LIB_DEBUG NAMES uSockets 
		PATHS "${VCPKG_INSTALLED_TRIPLET_DIR}/debug/lib" NO_DEFAULT_PATH
	)
    find_library(USOCKETS_LIB_RELEASE NAMES uSockets 
		PATHS "${VCPKG_INSTALLED_TRIPLET_DIR}/lib" NO_DEFAULT_PATH
	)
	
	if(NOT USOCKETS_LIB_DEBUG OR NOT USOCKETS_LIB_RELEASE)
		message(STATUS "VCPKG_INSTALLED_TRIPLET_DIR: ${VCPKG_INSTALLED_TRIPLET_DIR}")
		message(STATUS "Looking in: ${VCPKG_INSTALLED_TRIPLET_DIR}/lib")
		message(STATUS "Looking in: ${VCPKG_INSTALLED_TRIPLET_DIR}/debug/lib")
		message(FATAL_ERROR "uSockets lib not found!")
	endif()
    
    if(UWEBSOCKETS_INCLUDE_DIR AND EXISTS ${UWEBSOCKETS_INCLUDE_DIR})
		target_include_directories(Engine PRIVATE ${UWEBSOCKETS_INCLUDE_DIR})
	endif()

	target_link_libraries(Engine PUBLIC
		${USOCKETS_LIB_DEBUG}
		${USOCKETS_LIB_RELEASE}
	)

	#[[
	target_link_libraries(Engine PUBLIC
		$<$<CONFIG:Debug>:${USOCKETS_LIB_DEBUG}>
		$<$<CONFIG:Release>:${USOCKETS_LIB_RELEASE}>
	)
	]]
		
	target_compile_definitions(Engine PUBLIC USE_LIBRARY_UWEBSOCKETS=1)
else()
	target_compile_definitions(Engine PUBLIC USE_LIBRARY_UWEBSOCKETS=0)
endif()

# Add SDL3
message(STATUS "Adding SDL...")

if(ENGINE_VENDORED)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL")
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			message("Building in Debug mode - disable compiler optimizations")
			
			# Disable all CPU optimizations:
			set(SDL_ASSEMBLY OFF CACHE BOOL "" FORCE)
			set(SDL_SSE OFF CACHE BOOL "" FORCE)
			set(SDL_AVX OFF CACHE BOOL "" FORCE)
			
			set(WEBP_ENABLE_SIMD OFF CACHE BOOL "" FORCE)
		endif()
		
        set(SDL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL")
        set(SDL_BUILD_PATH "${CMAKE_BINARY_DIR}/SDL_build")

		set(SDL_TESTS OFF CACHE BOOL "" FORCE)

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
    
if (ENGINE_USING_VIDEO)
	# Add SDL_ttf
	message(STATUS "Adding SDL_ttf...")

	if(ENGINE_VENDORED)
		if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL_ttf")
			#option(SDLTTF_INSTALL "Enable SDL3_ttf install target" ON)
			#set(SDLTTF_INSTALL ON CACHE BOOL "Enable SDL3_ttf install target" FORCE)
			#set(SDLTTF_SHARED ON CACHE BOOL "Build SDL3_ttf as a shared library" FORCE)

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
endif()
    
if (ENGINE_USING_AUDIO)
	# Add SDL_mixer
	message(STATUS "Adding SDL_mixer...")

	if(ENGINE_VENDORED)
		if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/SDL_mixer")
			#option(SDLMIXER_INSTALL "Enable SDL3_mixer install target" ON)
			#set(SDLMIXER_INSTALL ON CACHE BOOL "Enable SDL3_mixer install target" FORCE)
			#set(SDLMIXER_SHARED ON CACHE BOOL "Build SDL3_mixer as a shared library" FORCE)

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
endif()

if (ENGINE_USING_VIDEO)
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
			#set(SDLIMAGE_SHARED ON CACHE BOOL "Build SDL3_image as a shared library" FORCE)

			#option(SDLIMAGE_INSTALL "Enable SDL3_image install target" ON)
			#set(SDLIMAGE_INSTALL ON CACHE BOOL "Enable SDL3_image install target" FORCE)

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
endif()

if(USE_LIBRARY_ASIO)
	message(STATUS "Adding ASIO...")
	
	set(ASIO_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/asio")
	set(ASIO_INCLUDE_DIR "${ASIO_BASE_DIR}/asio/include")
	
	if(EXISTS ASIO_INCLUDE_DIR)
		target_include_directories(Engine PUBLIC ${ASIO_INCLUDE_DIR})
		target_compile_definitions(Engine PUBLIC ASIO_STANDALONE)

		target_compile_definitions(Engine PUBLIC USE_LIBRARY_ASIO=1)
	else()
		target_compile_definitions(Engine PUBLIC USE_LIBRARY_ASIO=0)
	endif()
else()
	target_compile_definitions(Engine PUBLIC USE_LIBRARY_ASIO=FALSE)
endif()
	
if(USE_LIBRARY_CROW)
	message(STATUS "Adding CROW...")
	
	target_compile_definitions(Engine PUBLIC USE_LIBRARY_CROW=1)
	target_compile_definitions(Engine PUBLIC CROW_ENABLE_SSL)
	
	if(ENGINE_VENDORED)
		set(CROW_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/Crow")
		set(CROW_BUILD_PATH "${CMAKE_BINARY_DIR}/crow_build")
			
		if(EXISTS "${CROW_PATH}")
			add_subdirectory(${CROW_PATH} ${CROW_BUILD_PATH})

			target_link_directories(Engine PUBLIC ${CROW_BUILD_PATH})

			# After build copy includes
			set(INCLUDES_CROW_DIR ${CROW_PATH}/include/crow)
			set(INCLUDES_CROW_HEADER ${CROW_PATH}/include/crow.h)
			set(DESTINATION_DIR ${ENGINE_SOURCE_THIRD_PARTY_DIR})
			
			file(COPY ${INCLUDES_CROW_DIR} DESTINATION ${DESTINATION_DIR})
			file(COPY ${INCLUDES_CROW_HEADER} DESTINATION ${DESTINATION_DIR})
		else()
			message(FATAL_ERROR "Crow directory not found. Please check the path.")
			message(FATAL_ERROR "Missing directory could mean missing git submodules.")
		endif()
	else()
		# Not tested
		find_package(Crow REQUIRED)
	endif()
else()
	target_compile_definitions(Engine PUBLIC USE_LIBRARY_CROW=0)
endif()
	
if(USE_LIBRARY_SOCI)
	message(STATUS "Adding SOCI with selected backends...")
	
	target_compile_definitions(Engine PUBLIC USE_LIBRARY_SOCI=TRUE)
	
	if(ENGINE_VENDORED)
		set(SOCI_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/soci")
		set(SOCI_BUILD_PATH "${CMAKE_BINARY_DIR}/soci_build")
		
		if(EXISTS "${SOCI_PATH}")
			set(SOCI_TESTS OFF CACHE BOOL "Disable SOCI tests")
			set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
			set(SOCI_STATIC OFF CACHE BOOL "" FORCE)
			
			set(SOCI_BACKEND_LIBS soci_core)
			
			# MySQL
			if(SOCI_ENABLE_MYSQL)
				if(WIN32 AND VCPKG_INSTALLED_TRIPLET_DIR)
					# Windows - use vcpkg
					set(MySQL_INCLUDE_DIRS "${VCPKG_INSTALLED_TRIPLET_DIR}/include/mysql")
					set(MySQL_LIBRARIES "${VCPKG_INSTALLED_TRIPLET_DIR}/lib/libmysql.lib")
					
					set(MYSQL_INCLUDE_DIR "${MySQL_INCLUDE_DIRS}" CACHE PATH "")
					set(MYSQL_LIBRARY "${MySQL_LIBRARIES}" CACHE FILEPATH "")
				else() # Linux (detect vcpkg OR system):
					find_path(VCPKG_MYSQL_INCLUDE mysql.h PATHS "${VCPKG_INSTALLED_TRIPLET_DIR}/include/mysql" NO_DEFAULT_PATH)
					
					if(VCPKG_MYSQL_INCLUDE)
						set(MySQL_INCLUDE_DIRS "${VCPKG_INSTALLED_TRIPLET_DIR}/include/mysql")
						set(MySQL_LIBRARIES "${VCPKG_INSTALLED_TRIPLET_DIR}/lib/libmysqlclient${CMAKE_SHARED_LIBRARY_SUFFIX}")
					else()
						find_package(PkgConfig REQUIRED)
						pkg_check_modules(MYSQL REQUIRED mysqlclient)  # mariadb-devel provides
						set(MySQL_INCLUDE_DIRS "${MYSQL_INCLUDE_DIRS}")
						set(MySQL_LIBRARIES "${MYSQL_LIBRARIES}")
					endif()
					
					set(MYSQL_INCLUDE_DIR "${MySQL_INCLUDE_DIRS}" CACHE PATH "")
					set(MYSQL_LIBRARY "${MySQL_LIBRARIES}" CACHE FILEPATH "")
				endif()
								
				# Check if MySQL was found
				if(MySQL_INCLUDE_DIRS AND MySQL_LIBRARIES)
					set(SOCI_MYSQL ON CACHE BOOL "Enable MySQL backend")
					target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_MYSQL=TRUE)
					target_include_directories(Engine PRIVATE ${MySQL_INCLUDE_DIRS})
					target_link_libraries(Engine PRIVATE ${MySQL_LIBRARIES})
					list(APPEND SOCI_BACKEND_LIBS soci_mysql)
					message(STATUS "  - MySQL backend enabled")
					message(STATUS "    Include:  ${MySQL_INCLUDE_DIRS}")
					message(STATUS "    Library: ${MySQL_LIBRARIES}")
				else()
					message(WARNING "MySQL requested but not found!")
					set(SOCI_MYSQL OFF CACHE BOOL "Disable MySQL backend")
					target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_MYSQL=FALSE)
				endif()
			else()
				set(SOCI_MYSQL OFF CACHE BOOL "Disable MySQL backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_MYSQL=FALSE)
				message(STATUS "  - MySQL backend disabled")
			endif()


			# PostgreSQL
			if(SOCI_ENABLE_POSTGRESQL)
				set(SOCI_POSTGRESQL ON CACHE BOOL "Enable PostgreSQL backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_POSTGRESQL=TRUE)
				list(APPEND SOCI_BACKEND_LIBS soci_postgresql)
				message(STATUS "  - PostgreSQL backend enabled")
			else()
				set(SOCI_POSTGRESQL OFF CACHE BOOL "Disable PostgreSQL backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_POSTGRESQL=FALSE)
				message(STATUS "  - PostgreSQL backend disabled")
			endif()

			# SQLite3
			if(SOCI_ENABLE_SQLITE3)
				set(SOCI_SQLITE3 ON CACHE BOOL "Enable SQLite3 backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_SQLITE3=TRUE)
				list(APPEND SOCI_BACKEND_LIBS soci_sqlite3)
				message(STATUS "  - SQLite3 backend enabled")
			else()
				set(SOCI_SQLITE3 OFF CACHE BOOL "Disable SQLite3 backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_SQLITE3=FALSE)
				message(STATUS "  - SQLite3 backend disabled")
			endif()

			# Oracle
			if(SOCI_ENABLE_ORACLE)
				set(SOCI_ORACLE ON CACHE BOOL "Enable Oracle backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_ORACLE=TRUE)
				list(APPEND SOCI_BACKEND_LIBS soci_oracle)
				message(STATUS "  - Oracle backend enabled")
			else()
				set(SOCI_ORACLE OFF CACHE BOOL "Disable Oracle backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_ORACLE=FALSE)
				message(STATUS "  - Oracle backend disabled")
			endif()

			# ODBC (generic - can connect to many databases)
			if(SOCI_ENABLE_ODBC)
				set(SOCI_ODBC ON CACHE BOOL "Enable ODBC backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_ODBC=TRUE)
				list(APPEND SOCI_BACKEND_LIBS soci_odbc)
				message(STATUS "  - ODBC backend enabled")
			else()
				set(SOCI_ODBC OFF CACHE BOOL "Disable ODBC backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_ODBC=FALSE)
				message(STATUS "  - ODBC backend disabled")
			endif()

			# Firebird
			if(SOCI_ENABLE_FIREBIRD)
				set(SOCI_FIREBIRD ON CACHE BOOL "Enable Firebird backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_FIREBIRD=TRUE)
				list(APPEND SOCI_BACKEND_LIBS soci_firebird)
				message(STATUS "  - Firebird backend enabled")
			else()
				set(SOCI_FIREBIRD OFF CACHE BOOL "Disable Firebird backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_FIREBIRD=FALSE)
				message(STATUS "  - Firebird backend disabled")
			endif()

			# DB2
			if(SOCI_ENABLE_DB2)
				set(SOCI_DB2 ON CACHE BOOL "Enable DB2 backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_DB2=TRUE)
				list(APPEND SOCI_BACKEND_LIBS soci_db2)
				message(STATUS "  - DB2 backend enabled")
			else()
				set(SOCI_DB2 OFF CACHE BOOL "Disable DB2 backend")
				target_compile_definitions(Engine PUBLIC SOCI_SUPPORT_DB2=FALSE)
				message(STATUS "  - DB2 backend disabled")
			endif()
			
			add_subdirectory(${SOCI_PATH} ${SOCI_BUILD_PATH})
			
			target_link_libraries(Engine PUBLIC ${SOCI_BACKEND_LIBS})
			
			target_include_directories(Engine PUBLIC 
				${SOCI_PATH}/include
				${SOCI_BUILD_PATH}/include
			)
		else()
			message(FATAL_ERROR "SOCI directory not found at ${SOCI_PATH}")
		endif()
	else()
		find_package(SOCI REQUIRED)
		target_include_directories(Engine PUBLIC ${SOCI_INCLUDE_DIRS})
		target_link_libraries(Engine PUBLIC ${SOCI_LIBRARIES})
	endif()
else()
	target_compile_definitions(Engine PUBLIC USE_LIBRARY_SOCI=FALSE)
endif()

message(STATUS "Adding nlohmann/json...")

if(ENGINE_VENDORED)
	set(JSON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/json")
	set(JSON_BUILD_PATH "${CMAKE_BINARY_DIR}/json_build")
		
	if(EXISTS "${JSON_PATH}")
		# nlohmann/json is header-only, so we don't need add_subdirectory for building
		# Just include the directory for headers
		target_include_directories(Engine PUBLIC ${JSON_PATH}/include)
		
		# Copy includes to destination
		set(INCLUDES_JSON_DIR ${JSON_PATH}/include/nlohmann)
		set(INCLUDES_JSON_HEADER ${JSON_PATH}/single_include/nlohmann/json.hpp)
		set(DESTINATION_DIR ${ENGINE_SOURCE_THIRD_PARTY_DIR})
		
		# Copy the nlohmann directory structure
		file(COPY ${INCLUDES_JSON_DIR} DESTINATION ${DESTINATION_DIR})
		
		# Optionally copy the single header version for easier inclusion
		if(EXISTS ${INCLUDES_JSON_HEADER})
			file(COPY ${INCLUDES_JSON_HEADER} DESTINATION ${DESTINATION_DIR}/nlohmann/)
		endif()
		
	else()
		message(FATAL_ERROR "nlohmann/json directory not found. Please check the path.")
		message(FATAL_ERROR "Missing directory could mean missing git submodules.")
	endif()
else()
	# Use find_package for system-installed version
	find_package(nlohmann_json REQUIRED)
	target_link_libraries(Engine PUBLIC nlohmann_json::nlohmann_json)
endif()

# Add argon
set(ARGON2_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/argon2")
if(EXISTS "${ARGON2_PATH}")
	add_subdirectory(${ARGON2_PATH} ${CMAKE_BINARY_DIR}/argon2_build)
	target_link_libraries(Engine PUBLIC argon2)
	
	# Copy headers
	set(DESTINATION_DIR ${ENGINE_SOURCE_THIRD_PARTY_DIR})
	file(COPY ${ARGON2_PATH}/include/ DESTINATION ${DESTINATION_DIR}/argon2)
	
else()
	message(FATAL_ERROR "Argon2 directory not found at ${ARGON2_PATH}")
endif()

if(ANDROID OR (UNIX AND NOT APPLE))
	# Link shared on android
	target_link_libraries(Engine PUBLIC
		SDL3_ttf-shared
		SDL3_mixer-shared
		SDL3_image-shared
	)
else()
	# Link
	target_link_libraries(Engine PUBLIC
		SDL3_ttf
		SDL3_mixer
		SDL3_image
	)
endif()

if(ANDROID OR (UNIX AND NOT APPLE))
	target_link_libraries(Engine PUBLIC z)
endif()

target_link_libraries(Engine PUBLIC
	SDL3::SDL3
)

#==============================================================================
# FUNCTIONS
#==============================================================================

function(set_property_if_not_alias target_name folder_name)
    get_property(aliased_target TARGET "${target_name}" PROPERTY ALIASED_TARGET)
    if("${aliased_target}" STREQUAL "")
        # Not an alias - set property on this target
        set_property(TARGET ${target_name} PROPERTY FOLDER ${folder_name})
    else()
        # Is an alias - set property on the real target
        set_property(TARGET ${aliased_target} PROPERTY FOLDER ${folder_name})
    endif()
endfunction()

# Target grouping
function(group_targets folder_name)
    foreach(target ${ARGN})
        if(TARGET ${target})
            set_property_if_not_alias(${target} ${folder_name})
        endif()
    endforeach()
endfunction()

# Function to group any uncategorized targets
function(group_remaining_targets folder_name)
    get_property(_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
    foreach(_target ${_targets})
        if(TARGET ${_target})
            get_property(_folder TARGET ${_target} PROPERTY FOLDER)
            # If no folder is set, put it in "Other"
            if(NOT _folder)
				set_property_if_not_alias(${_target} ${folder_name})
            endif()
        endif()
    endforeach()
endfunction()

# Copies files to destination, skipping hidden files and invalid paths
function(CopyAssetFiles FilesList DestinationPath)
    foreach(File ${FilesList})
        get_filename_component(FileName ${File} NAME)
        
        # Skip hidden files (starting with .) and non-existent files
        if (NOT FileName MATCHES "^\\..*" AND EXISTS ${File})
            file(COPY ${File} 
                 DESTINATION ${DestinationPath} 
                 FILE_PERMISSIONS OWNER_READ OWNER_WRITE)
        else()
            message(WARNING "Could not access file: ${File} (skipping)")
        endif()
    endforeach()
endfunction()

# Copies assets to target output directory after build
# (Only then we know if it's debug/release)
function(CopyAssetsPostBuild TargetName SourcePath)
    set(DestSubfolder "Assets")
    if(ARGC GREATER 2)
        set(DestSubfolder ${ARGV2})
    endif()
    
    add_custom_command(TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
            "${SourcePath}" 
            "$<TARGET_FILE_DIR:${TargetName}>/${DestSubfolder}"
        COMMENT "Copying assets..."
    )
endfunction()

# Sample
# copy_library_files(your_target LIBRARIES crypto-3-x64 ssl-3-x64)
function(copy_library_files target)
	set(options "")
	set(oneValueArgs "")
	set(multiValueArgs LIBRARIES)
	cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	foreach(lib_name ${ARG_LIBRARIES})
		if(WIN32)
			set(lib_file "${VCPKG_INSTALLED_TRIPLET_DIR}/bin/${lib_name}.dll")
		elseif(APPLE)
			set(lib_file "${VCPKG_INSTALLED_TRIPLET_DIR}/lib/lib${lib_name}.dylib")
		elseif(UNIX)
			set(lib_file "${VCPKG_INSTALLED_TRIPLET_DIR}/lib/lib${lib_name}.so")
		endif()

		if(EXISTS "${lib_file}")
			add_custom_command(TARGET ${target} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different "${lib_file}" $<TARGET_FILE_DIR:${target}>
			)
		else()
			message(WARNING "Library not found: ${lib_file}")
		endif()
	endforeach()
endfunction()

# More generic copy function
#s
# Sample
# copy_libs(your_target
#	WIN_LIBS libcrypto-3-x64.dll libssl-3-x64.dll
#	UNIX_LIBS libcrypto.so.3 libssl.so.3
#	APPLE_LIBS libcrypto.3.dylib libssl.3.dylib
#)
function(copy_libs target)
	set(options "")
	set(oneValueArgs "")
	set(multiValueArgs WIN_LIBS UNIX_LIBS APPLE_LIBS)
	cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if(WIN32)
		set(libs ${ARG_WIN_LIBS})
		set(lib_dir "${VCPKG_INSTALLED_TRIPLET_DIR}/bin")
	elseif(APPLE)
		set(libs ${ARG_APPLE_LIBS})
		set(lib_dir "${VCPKG_INSTALLED_TRIPLET_DIR}/lib")
	elseif(UNIX)
		set(libs ${ARG_UNIX_LIBS})
		set(lib_dir "${VCPKG_INSTALLED_TRIPLET_DIR}/lib")
	endif()

	foreach(lib ${libs})
		add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${lib_dir}/${lib}" $<TARGET_FILE_DIR:${target}>
	)
	endforeach()
endfunction()

#==============================================================================
# TESTS
#==============================================================================

set(TESTS_ENABLED FALSE)

# Decide if we want tests (for now only windows x64)
if(WIN32 AND NOT CMAKE_GENERATOR_PLATFORM STREQUAL "ARM64")
    if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64" OR (NOT DEFINED CMAKE_GENERATOR_PLATFORM AND CMAKE_SIZEOF_VOID_P EQUAL 8))
		message(STATUS "Get google tests")
		
        set(TESTS_ENABLED TRUE)

		# Enable testing
		enable_testing()

		include(FetchContent)
		FetchContent_Declare(
			googletest
			GIT_REPOSITORY https://github.com/google/googletest.git
			GIT_TAG v1.15.2  # Use the latest stable release
		)

		set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
		FetchContent_MakeAvailable(googletest)
		
		message(STATUS "Adding engine google tests")
		
		# Create test executable
		add_executable(
			EngineTests

			# Sources for Test
			SourcesTest/MainTestFile.cpp
		)

		set_target_properties(EngineTests PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY            "${CMAKE_BINARY_DIR}/bin/Debug/EngineTest"
			RUNTIME_OUTPUT_DIRECTORY_DEBUG      "${CMAKE_BINARY_DIR}/bin/Debug/EngineTest"
			RUNTIME_OUTPUT_DIRECTORY_RELEASE    "${CMAKE_BINARY_DIR}/bin/Release/EngineTest"
		)

		# Link test libraries
		target_link_libraries(EngineTests
			PUBLIC Engine
			PUBLIC GTest::gtest_main
		)
		
		 # Add the files to your target
		target_sources(EngineTests
			PUBLIC      ${ENGINE_SOURCE_FILES_PUBLIC}
			PRIVATE     ${ENGINE_SOURCE_FILES_PRIVATE}
		)

		# Include directories
		target_include_directories(EngineTests PRIVATE
			# Project source files (for testing)
			PRIVATE     ${ENGINE_SOURCE_DIR}  
			PUBLIC      ${ENGINE_SOURCE_PUBLIC_DIR}
			PRIVATE     ${ENGINE_SOURCE_PRIVATE_DIR} 
			PRIVATE     ${ENGINE_SOURCE_THIRD_PARTY_DIR} 
		)
		
		# Skip project run as we run tests instead, a bit hacky but simple
		target_compile_definitions(EngineTests PRIVATE
			FLAGS_TESTING_PROJECT
		)

		# Discover and register tests
		include(GoogleTest)

		# Copy Engine library to output directory
		# Must be done after linking!
		add_custom_command(TARGET EngineTests POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				$<TARGET_FILE:Engine>
				$<TARGET_FILE_DIR:EngineTests>
			COMMENT "Copying Engine libraries to output directory"
		)
		
		message(STATUS "Added google tests!")
    endif()
endif()

function(copy_runtime_engine_dlls target)
	if (NOT UNIX AND VCPKG_ROOT_DIR AND VCPKG_INSTALLED_TRIPLET_DIR)
		copy_library_files(${target} LIBRARIES libcrypto-3-x64 libssl-3-x64)

		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3-shared> $<TARGET_FILE_DIR:${target}>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3_ttf-shared> $<TARGET_FILE_DIR:${target}>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3_mixer-shared> $<TARGET_FILE_DIR:${target}>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3_image-shared> $<TARGET_FILE_DIR:${target}>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Engine> $<TARGET_FILE_DIR:${target}>
		)

		if (USE_LIBRARY_SOCI)
			add_custom_command(TARGET ${target} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:soci_core> $<TARGET_FILE_DIR:${target}>
			)

			if (SOCI_ENABLE_MYSQL)
				add_custom_command(TARGET ${target} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:soci_mysql> $<TARGET_FILE_DIR:${target}>
				)

				copy_library_files(${target} LIBRARIES libmysql lz4 zlib1 zstd)

			endif()

			if (USE_LIBRARY_UWEBSOCKETS)
				if(CMAKE_BUILD_TYPE STREQUAL "Debug")
					copy_libs(${target}
						WIN_LIBS zlibd1.dll uv.dll
						UNIX_LIBS libz.so.1 libuv.so.1
						APPLE_LIBS libz.1.dylib libuv.1.dylib
					)
				else()
					copy_libs(${target}
						WIN_LIBS zlib1.dll uv.dll
						UNIX_LIBS libz.so.1 libuv.so.1
						APPLE_LIBS libz.1.dylib libuv.1.dylib
					)
				endif()
			endif()
		endif()
	endif()
endfunction()

# Target for build all (To use from config script)
if(TESTS_ENABLED)
	add_custom_target(BuildAllEngine ALL
		DEPENDS Engine SDL3::SDL3 EngineTests # SDL3_image SDL3_mixer SDL3_ttf
	)
	
    # Use it to copy engine libraries
    copy_runtime_engine_dlls(EngineTests)

    # Special test dlls copy
    add_custom_command(TARGET EngineTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest> $<TARGET_FILE_DIR:EngineTests>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest_main> $<TARGET_FILE_DIR:EngineTests>
    )

    # Discover after DLLs are copied
    add_test(NAME EngineTests COMMAND EngineTests)

	group_targets("Test" EngineTests)

    message(STATUS "Discovered gtests")
else()	
	add_custom_target(BuildAllEngine ALL
		DEPENDS Engine SDL3::SDL3 # SDL3_image SDL3_mixer SDL3_ttf
	)
endif()

set_property(TARGET Engine PROPERTY FOLDER "Core")
group_targets("Core" Engine)
group_targets("ThirdParty/SDL3" SDL3::SDL3 SDL3-shared SDL3_mixer-shared SDL3_ttf-shared)
group_targets("ThirdParty/gtest" GTest::gmock GTest::gmock_main GTest::gtest GTest::gtest_main)
group_targets("ThirdParty/encryption" argon2 OpenSSL::Crypto OpenSSL::SSL)
group_targets("CustomCommands" BuildAllEngine)

if(USE_LIBRARY_ASIO)
	if(USE_LIBRARY_CROW)
		# Link
		target_link_libraries(Engine PUBLIC
			Crow::Crow
		)
		
		group_targets("ThirdParty/Network" Crow::Crow)
	endif()
endif()

