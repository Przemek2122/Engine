// Created by Przemys�aw Wiewi�ra 2020-2025

#include "CoreEngine.h"
#include "Misc/FileSystem.h"

namespace fs = std::filesystem;

bool FFileSystem::Directory::Exists(const std::string& InPath)
{
#if PLATFORM_ANDROID
    FAssetsManager* AssetsManager = FGlobalDefines::GEngine->GetAssetsManager();
    std::shared_ptr<FIterateDirectoryData> IterateDirectoryData = AssetsManager->GetAndroidIterateDirectoryData();

    bool bExists = IterateDirectoryData->IsValid();
    CArray<std::string> PathSplitArray = FStringHelpers::SplitString(InPath, FFileSystem::GetPlatformSlash());

    // Check every part of path to make sure it exist
    for (std::string& SplitPart : PathSplitArray)
    {
        IterateDirectoryData = GetDirectoryByPath(IterateDirectoryData, SplitPart);

        // Check if returned value was correct
        if (IterateDirectoryData == nullptr || !IterateDirectoryData->IsValid())
        {
            bExists = false;

            break;
        }
    }

    return bExists;
#else
    return (std::filesystem::is_directory(InPath));
#endif
}

bool FFileSystem::Directory::Create(const std::string& InPath)
{
#if PLATFORM_ANDROID
    /** Android internal memory can not be modified, external storage needs to be used */
    LOG_ERROR("FFileSystem::Directory::Create is missing");
#endif

	return fs::create_directory(InPath);
}

bool FFileSystem::Directory::Delete(const std::string& InPath, const bool bRecursive)
{
#if PLATFORM_ANDROID
    /** Android internal memory can not be modified, external storage needs to be used */
    ENSURE_VALID(false);
#endif

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
#if PLATFORM_ANDROID
    /** Android internal memory can not be modified, external storage needs to be used */
    LOG_ERROR("FFileSystem::File::Create is missing");
#endif

	const std::ofstream File(InPath.c_str());

	return File.good();
}

bool FFileSystem::File::Delete(const std::string& InPath)
{
#if PLATFORM_ANDROID
    /** Android internal memory can not be modified, external storage needs to be used */
    ENSURE_VALID(false);
#endif

	return fs::remove(InPath);
}

void FFileSystem::File::Clear(const std::string& InPath)
{
#if PLATFORM_ANDROID
    /** Android internal memory can not be modified, external storage needs to be used */
    ENSURE_VALID(false);
#endif

    std::ifstream File;

    File.open(InPath.c_str(), std::ifstream::out | std::ifstream::trunc);
    if (!File.is_open() || File.fail())
    {
        File.close();

        LOG_ERROR("Failed to clear file.");
    }

    File.close();
}

bool FFileSystem::File::ReadLine(SDL_IOStream *IOStream, std::string& CurrentLine)
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

CArray<std::string> FFileSystem::GetDirectories(const std::string& Path)
{
    CArray<std::string> Directories;

    // @TODO Test if std filesystem is worth keeping
#if PLATFORM_DESKTOP
    for (const std::filesystem::directory_entry& Entry : fs::directory_iterator(Path))
    {
	    if (Entry.is_directory())
	    {
		    std::filesystem::path PathEntry = Entry.path();

            if (PathEntry.has_filename())
            {
                PathEntry = PathEntry.filename();
            }

            Directories.Push(PathEntry.string());
	    }
    }
#else
    const std::shared_ptr<FIterateDirectoryData> IterateDirectoryData = FGlobalDefines::GEngine->GetAssetsManager()->GetAndroidIterateDirectoryData();
    CArray<std::string> DirectoriesArray = FStringHelpers::SplitString(Path, FFileSystem::GetPlatformSlash());
    int32 DirectoriesArrayDepth = 0;
    int32 Index = INDEX_NONE;

    std::shared_ptr<FIterateDirectoryData> CurrentIterateDirectoryData = IterateDirectoryData;
	while (CurrentIterateDirectoryData->Directories.FindIndexByPredicate([&](const std::shared_ptr<FIterateDirectoryData>& Data)
        {
            return (Data->Path == DirectoriesArray[DirectoriesArrayDepth]);
        }, Index))
	{
		CurrentIterateDirectoryData = CurrentIterateDirectoryData->Directories[Index];
	}

    if (CurrentIterateDirectoryData != nullptr)
    {
        for (const std::shared_ptr<FIterateDirectoryData>& Data : CurrentIterateDirectoryData->Directories)
        {
            Directories.Push(Data->Path);
        }
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

#if PLATFORM_ANDROID
CArray<std::string> FFileSystem::GetAllAssetsFromManifest()
{
	static const std::string ManifestPath = "asset_manifest.txt";

	CArray<std::string> Files;

	// Open the manifest file
    std::string OpenForReading = FAssetsGlobals::GetAssetReadType(EAssetReadMethod::OpenForReading);
	SDL_IOStream* ManifestIO = SDL_IOFromFile(ManifestPath.c_str(), OpenForReading.c_str());
	if (ManifestIO != nullptr)
	{
        // Get the size of the manifest file
        Sint64 ReturnedSize = SDL_GetIOSize(ManifestIO);
        if (ReturnedSize > 0) 
        {
            // Read the entire manifest into memory
            std::vector<char> ManifestBuffer(static_cast<size_t>(ReturnedSize) + 1);
            const Sint64 LoadedSize = SDL_ReadIO(ManifestIO, ManifestBuffer.data(), ReturnedSize);
            if (LoadedSize == ReturnedSize)
            {
                ManifestBuffer[ReturnedSize] = '\0';

                // Parse the file paths
                std::string content(ManifestBuffer.data());
                size_t pos = 0;
                std::string line;

                while ((pos = content.find('\n')) != std::string::npos || !content.empty())
                {
                    if (pos != std::string::npos)
                    {
                        line = content.substr(0, pos);
                        content.erase(0, pos + 1);
                    }
                    else
                    {
                        line = content;
                        content.clear();
                    }

                    if (!line.empty()) {
                        Files.Push(line);
                    }
                }
            }
            else
            {
                LOG_ERROR("Failed to read manifest file");
            }
        }
        else
        {
			LOG_ERROR("Failed to get size of asset manifest: " << SDL_GetError());
        }
	}
	else
	{
		LOG_ERROR("Failed to open asset manifest: " << SDL_GetError());
	}

	return Files;
}

std::shared_ptr<FIterateDirectoryData> FFileSystem::GetAllAssetsFromManifestGrouped(const CArray<std::string>& RawData)
{
    std::shared_ptr<FIterateDirectoryData> OutData = std::make_shared<FIterateDirectoryData>("");

    for (const std::string& CurrentPath : RawData)
    {
		// Check if line is not empty
		if (!CurrentPath.empty())
		{
			CArray<std::string> PathStringParts = FStringHelpers::SplitString(CurrentPath, FFileSystem::GetPlatformSlash());

            IterativelyAddDirectoriesWithTheirData(OutData, PathStringParts, 0);
		}
        else
        {
			LOG_WARN("Empty line in manifest file.");
        }
    }

    return OutData;
}

void FFileSystem::IterativelyAddDirectoriesWithTheirData(std::shared_ptr<FIterateDirectoryData> InData, CArray<std::string>& PathStringParts, int32 SubStrDirectoryIndex)
{
    if (!PathStringParts.IsEmpty())
    {
        // Is last element
		if (PathStringParts.IsLastIndex(SubStrDirectoryIndex))
		{
			const std::string& Value = PathStringParts[PathStringParts.GetLastIndex()];
			const bool bIsFile = FStringHelpers::ContainsChar(Value, '.');
            if (bIsFile)
            {
				InData->Files.Push(Value);
			}
			else
			{
				InData->Directories.Push(std::make_shared<FIterateDirectoryData>(Value));
            }
		}
        else
        {
	        int32 Index = INDEX_NONE;

	        const bool bContainsPath = InData->Directories.FindIndexByPredicate([&](const std::shared_ptr<FIterateDirectoryData>& IterateDirectoryData) -> bool
	        {
	            return (IterateDirectoryData->Path == PathStringParts[SubStrDirectoryIndex]);
	        }, Index);

	        if (bContainsPath)
	        {
	            // Get path
	            std::shared_ptr<FIterateDirectoryData> FoundData = InData->Directories[Index];

				IterativelyAddDirectoriesWithTheirData(FoundData, PathStringParts, ++SubStrDirectoryIndex);
	        }
	        else
	        {
	            // Add new path
	            std::shared_ptr<FIterateDirectoryData> NewData = std::make_shared<FIterateDirectoryData>(PathStringParts[SubStrDirectoryIndex]);

	            IterativelyAddDirectoriesWithTheirData(NewData, PathStringParts, ++SubStrDirectoryIndex);

	            InData->Directories.Push(NewData);
	        }
        }
    }
}
std::shared_ptr<FIterateDirectoryData> FFileSystem::GetDirectoryByPath(std::shared_ptr<FIterateDirectoryData>& DirectoryEntry, const std::string& SplitPart)
{
    std::shared_ptr<FIterateDirectoryData> Out;

    for (std::shared_ptr<FIterateDirectoryData>& SubDirectoryEntry : DirectoryEntry->Directories)
    {
        if (SubDirectoryEntry->Path == SplitPart)
        {
            Out = SubDirectoryEntry;

            break;
        }
    }

    return Out;
}
#endif

void FFileSystem::GetFilesFromDirectoryRecursive(CArray<std::string>& Container, const std::filesystem::directory_entry& Entry, const bool bRecursive)
{
#if PLATFORM_ANDROID
    /**
     * Android implementation is very easy to do,
     * just use manifest file to iterate directories
     */
    ENSURE_VALID(false);
#else
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
#endif
}
