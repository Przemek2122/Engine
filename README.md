# Engine

## Implemented partialy for gaming, partialy for applications.
Libraries in:
 - SDL3 (can be disabled)
 - SSL, zlib
 - SOCI & MySQL support (can be disabled)
 - HTTP using CrowCPP (can be disabled)
 - Sockets using uWebSockets
 [.. more]

## Functionality:
 - Thread management
 - App loop
 - ECS
 [.. more]

# Setup

## Required (linux packages list):

### Tested with fedora, debian has some issues even on latest version
 - Install cmake 4.1 at least from 'https://cmake.org/download/'
 - (dnf install) git wget nano make gcc-c++
 - anytime but after git download: 'git submodule update --init --recursive'

### Latest cmake install:
 - https://cmake.org/download/
 - 'mkdir /var/tools && cd /var/tools'
 - 'wget https://github.com/Kitware/CMake/releases/download/v4.2.1/cmake-4.2.1-linux-x86_64.sh'
 - Install cmake 'chmod +x cmake-4.2.1-linux-x86_64.sh && ./cmake-4.2.1-linux-x86_64.sh && rm '
 - 'export PATH="/var/tools/cmake-4.2.1-linux-x86_64/bin:$PATH"'
 - Verify 'cmake --version'
 <img width="569" height="39" alt="image" src="https://github.com/user-attachments/assets/b3184f42-11da-40c9-8cd5-8634e5295d73" />

## Optional: 
#### (vcpkg will be downloaded and installed by cmake with it's packages, but can be installed already to install it outside build dir to speedup in case where build dir needs to be deleted)
 - vcpkg, 
 - Some compiler + IDE

