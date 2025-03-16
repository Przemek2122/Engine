// Created by Przemys�aw Wiewi�ra 2020-2025

#include "CoreEngine.h"
#include "Misc/FileSystem.h"
#include "Assets/AssetsGlobals.h"

namespace fs = std::filesystem;

bool FFileSystem::Directory::Exists(const std::string& InPath)
{
    return (std::filesystem::is_directory(InPath));
}

bool FFileSystem::Directory::Create(const std::string& InPath)
{
	return fs::create_directory(InPath);
}

bool FFileSystem::Directory::Delete(const std::string& InPath, const bool bRecursive)
{
    if (bRecursive)
    {
        return fs::remove_all(InPath);
    }
    else
    {
        return fs::remove(InPath) != false;
    }
}

bool FFileSystem::File::Exists(const std::string& InPath)
{
#if PLATFORM_ANDROID
    const std::string AssetReadType = "r";

    SDL_IOStream *IOStream = SDL_IOFromFile(InPath.c_str(), AssetReadType.c_str());
    if (IOStream != nullptr)
    {
        SDL_CloseIO(IOStream);

        return true;
    }

    return false;
#else
    return (fs::exists(InPath));
#endif
}

bool FFileSystem::File::Create(const std::string& InPath)
{
	const std::ofstream File(InPath.c_str());

	return File.good();
}

bool FFileSystem::File::Delete(const std::string& InPath)
{
	return fs::remove(InPath);
}

void FFileSystem::File::Clear(const std::string& InPath)
{
    std::ifstream File;

    File.open(InPath.c_str(), std::ifstream::out | std::ifstream::trunc);
    if (!File.is_open() || File.fail())
    {
        File.close();

        LOG_ERROR("Failed to clear file.");
    }

    File.close();
}

bool FFileSystem::ReadLine(SDL_IOStream *IOStream, std::string& CurrentLine)
{
    CurrentLine = "";

    while (true)
    {
        // @TODO: In ideal scenario, we would like to read bigger chunks of data.
        char c;

        // Read exactly 1 byte
        size_t bytesRead = SDL_ReadIO(IOStream, &c, 1);

        if (bytesRead == 0)
        {
            // We have some data but no newline, so treat it as the final line
            break;
        }

        if (c == GetNewLineChar()) {
            // Stop reading at newline
            break;
        }

        // If needed, you can handle '\r' specially here (e.g., Windows "\r\n" line endings).
        CurrentLine.push_back(c);
    }


    return !CurrentLine.empty();
}

void FFileSystem::File::GetFileContentLineByLine(FDelegateSafe<void, const std::string&>& DelegateCalledForEachLine, const std::string& InPath)
{
    // Create a text string, which is used to output the text file
    std::string CurrentLine;

#if PLATFORM_ANDROID
    std::string OpenForReading = FAssetsGlobals::GetAssetReadType(EAssetReadMethod::OpenForReading);
    SDL_IOStream *IOStream = SDL_IOFromFile(InPath.c_str(), OpenForReading.c_str());
    if (IOStream != nullptr)
    {
        while (ReadLine(IOStream, CurrentLine))
        {
            DelegateCalledForEachLine.Execute(CurrentLine);
        }

        SDL_CloseIO(IOStream);
    }
#else
    // Read from the text file
    std::ifstream MyReadFile(InPath);

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(MyReadFile, CurrentLine))
    {
        DelegateCalledForEachLine.Execute(CurrentLine);
    }

    // Close the file
    MyReadFile.close();
#endif
}

void FFileSystem::File::AddFileContentLine(const std::string& Line, const std::string& InPath)
{
    std::ofstream MyFile(InPath);

    MyFile << Line;

    MyFile.close();
}

void FFileSystem::File::AddFileContentLines(const CArray<std::string>& Lines, const std::string& InPath)
{
    std::ofstream MyFile(InPath);

    for (const std::string& Line : Lines)
    {
        MyFile << Line;
    }

    MyFile.close();
}

bool FFileSystem::IsDirectory(const std::string& InPath)
{
	return fs::is_directory(InPath);
}

CArray<std::string> FFileSystem::GetFilesFromDirectory(const std::string& Path, const bool bRecursive)
{
    CArray<std::string> Files;

    for (const std::filesystem::directory_entry& Entry : fs::directory_iterator(Path))
    {
        GetFilesFromDirectoryRecursive(Files, Entry, bRecursive);
    }

    return Files;
}

CArray<std::string> FFileSystem::GetDirectories(const std::string& Path, const bool bCovertToJustFolders)
{
    CArray<std::string> Directories;

#if PLATFORM_DESKTOP
    for (const std::filesystem::directory_entry& Entry : fs::directory_iterator(Path))
    {
	    if (Entry.is_directory())
	    {
            auto PathEntry = Entry.path();

            if (bCovertToJustFolders)
            {
	            if (PathEntry.has_filename())
	            {
                    PathEntry = PathEntry.filename();
	            }
            }

            Directories.Push(PathEntry.string());
	    }
    }
#else
	auto ListDirectories = [](void* userdata, const char* dirname, const char* fname) -> SDL_EnumerationResult
	{
		CArray<std::string>* DirectoriesPtr = static_cast<CArray<std::string>*>(userdata);

        DirectoriesPtr->Push(fname);

        return SDL_ENUM_CONTINUE;
	};

    void* Data = &Directories;

    if (!SDL_EnumerateDirectory(Path.c_str(), ListDirectories, Data))
    {
        LOG_ERROR("Failed to enumerate directory: " <<  SDL_GetError());
    }
#endif

    return Directories;
}

char FFileSystem::GetPlatformSlash()
{
    static char* PlatformSlash =
#if _WIN32 || _WIN64
	TEXT_CHAR("\\");
#else
    TEXT_CHAR("/");
#endif

	return *PlatformSlash;
}

char FFileSystem::GetNewLineChar()
{
#if PLATFORM_ANDROID
    return '\r';
#else
    return '\n';
#endif
}

CArray<char> FFileSystem::GetNewLineChars()
{
    static CArray<char> NewlineChars = { '\r', '\n' };

    return NewlineChars;
}

std::string FFileSystem::GetBasePathCached()
{
    return FGlobalDefines::GEngine->GetLaunchFullPath();
}

void FFileSystem::GetFilesFromDirectoryRecursive(CArray<std::string>& Container, const std::filesystem::directory_entry& Entry, const bool bRecursive)
{
    if (Entry.is_directory() && bRecursive)
    {
        for (const std::filesystem::directory_entry& CurrentEntry : fs::directory_iterator(Entry.path()))
        {
            GetFilesFromDirectoryRecursive(Container, CurrentEntry, bRecursive);
        }
    }
    else if (!Entry.is_directory())
    {
        Container.Push(Entry.path().relative_path().string());
    }
}
