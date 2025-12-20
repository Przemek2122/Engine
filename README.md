# Engine

## Required (linux packages list):
### (tested with fedora, debian has some issues even on latest version)
 - cmake, git, "Development Tools",  
 - after downloading: 'git submodule update --init --recursive'
## Optional: 
#### (vcpkg will be downloaded and installed by cmake with it's packages, but can be installed already to install it outside build dir to speedup in case where build dir needs to be deleted)
 - vcpkg, Some compiler + IDE

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
