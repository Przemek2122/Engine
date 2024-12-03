// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

class FFileSystem
{
public:
	/** All directory related functions */
	class Directory
	{
	public:
		static bool Exists(const std::string& InPath);
		static bool Create(const std::string& InPath);
		static bool Delete(const std::string& InPath, const bool bRecursive = false);
	};

	/** All file related functions */
	class File
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
	};

	/** Determine if file is directory */
	static bool IsDirectory(const std::string& InPath);

	/** @returns files from given directory, optionally recursive */
	static CArray<std::string> GetFilesFromDirectory(const std::string& Path, const bool bRecursive = false);

	/** @returns directory names from given location */
	static CArray<std::string> GetDirectories(const std::string& Path, const bool bCovertToJustFolders = false);
	
	/** @returns slash for current filesystem */
	static char GetPlatformSlash();

	static char GetPlatformEndLine();

private:
	static void GetFilesFromDirectoryRecursive(CArray<std::string>& Container, const std::filesystem::directory_entry& Entry, const bool bRecursive);

};
