#pragma once

/**
 * How should file be read?
 * https://wiki.libsdl.org/SDL3/SDL_IOFromFile
 */
enum class ENGINE_API EAssetReadType : uint8
{
	// "r": Open a file for reading. The file must exist.
	OpenForReading,
	// "w": Create an empty file for writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file.
	CreateEmptyFile,
	// "a": Append to a file. Writing operations append data at the end of the file. The file is created if it does not exist.
	AppendToFile,
	// "r+": Open a file for update both reading and writing. The file must exist.
	OpenFile,
	// "w+": Create an empty file for both reading and writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file.
	CreateEmptyFileForReadingAndWriting,
	// "a+": Open a file for reading and appending. All writing operations are performed at the end of the file, protecting the previous content to be overwritten. You can reposition (fseek, rewind) the internal pointer to anywhere in the file for reading, but writing operations will move it back to the end of file. The file is created if it does not exist.
	OpenFileForReadingAndAppending
};

/**
 * Globals for assets
 */
class ENGINE_API FAssetsGlobals
{
public:
	static std::string GetAssetReadType(EAssetReadType AssetReadType, bool bUseFileAsBinary = false);

};
