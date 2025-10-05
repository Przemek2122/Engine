// Created by Przemys�aw Wiewi�ra 2020

#pragma once

#include "CoreMinimal.h"

struct FIterateDirectoryData
{
	FIterateDirectoryData(const std::string& InPath = "")
		: Path(InPath)
	{
	}

	bool IsValid() const { return (!Path.empty() || !Files.IsEmpty() || !Directories.IsEmpty()); }

	std::string Path;

	CArray<std::string> Files;
	CArray<std::shared_ptr<FIterateDirectoryData>> Directories;
};

class ENGINE_API FFileSystem
{
public:
	/** All directory related functions */
	class ENGINE_API Directory
	{
	public:
		static bool Exists(const std::string& InPath);
		static bool Create(const std::string& InPath);
		static bool Delete(const std::string& InPath, const bool bRecursive = false);
	};

	/** All file related functions */
	class ENGINE_API File
	{
	public:
		static bool Exists(const std::string& InPath);
		static bool Create(const std::string& InPath);

		/** Remove file from disk. */
		static bool Delete(const std::string& InPath);

		/** Remove all content. */
		static void Clear(const std::string& InPath);

		/** Calls delegate with each line separately */
		static void GetFileContentLineByLine(FDelegateSafe<void, const std::string&>& DelegateCalledForEachLine, const std::string& InPath);

		static void AddFileContentLine(const std::string& Line, const std::string& InPath);
		static void AddFileContentLines(const CArray<std::string>& Lines, const std::string& InPath);

	private:
		static bool ReadLine(SDL_IOStream* IOStream, std::string& CurrentLine);
	};

	/** Determine if file is directory */
	static bool IsDirectory(const std::string& InPath);

	/** @returns files from given directory, optionally recursive */
	static CArray<std::string> GetFilesFromDirectory(const std::string& Path, const bool bRecursive = false);

	/** @returns directory names from given location */
	static CArray<std::string> GetDirectories(const std::string& Path);
	
	/** @returns slash for current filesystem */
	static char GetPlatformSlash();

	/**
	 * @returns newline specific to platform. \n
	 * Prefer GetNewLineChars if possible
	 */
	static char GetNewLineChar();

	/** @returns newline char array */
	static CArray<char> GetNewLineChars();

	static std::string GetBasePathCached();

#if PLATFORM_ANDROID
	/** Generates android files array using manifest, depending on number of files may be slow */
	static CArray<std::string> GetAllAssetsFromManifest();
	static std::shared_ptr<FIterateDirectoryData> GetAllAssetsFromManifestGrouped(const CArray<std::string>& RawData);
	static void IterativelyAddDirectoriesWithTheirData(std::shared_ptr<FIterateDirectoryData> InData, CArray<std::string>& PathStringParts, int32 SubStrDirectoryIndex);
	static  std::shared_ptr<FIterateDirectoryData> GetDirectoryByPath(std::shared_ptr<FIterateDirectoryData>& DirectoryEntry, const std::string& SplitPart);
#endif

private:
	static void GetFilesFromDirectoryRecursive(CArray<std::string>& Container, const std::filesystem::directory_entry& Entry, const bool bRecursive);

};
