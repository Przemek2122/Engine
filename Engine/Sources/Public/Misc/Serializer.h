// Created by Przemys³aw Wiewióra 2025

#pragma once

#include "CoreMinimal.h"

/** Generate field Id, used by SERIALIZE_FIELD, does not need to be used manually */
#define FIELD_ID(Name) FSerializer::HashFieldName(#Name)

/**
 * Universal macros - work for both single values and vectors
 * @Note: Variable names when loading and saving must be the same!
 * @Note: Variables needs to be loaded in the same order as saved (Save var A, Save var B will be Load var A, Load var B)
 */
#define SERIALIZE_FIELD(Data, Field) FSerializer::WriteField(Data, FIELD_ID(Field), Field)
#define DESERIALIZE_FIELD(Data, Offset, Field) FSerializer::ReadField(Data, Offset, FIELD_ID(Field), Field)

/** Utility class for saving */
class FSerializer
{
public:
    /**
     * Structure for representing object bytes
     *
     * @note: Best performance
     * @note BUT:
     * @note in case where we have any field changed (added or removed), data will be corrupted
     * @note in case where we have data changed but same object size, unpredicted behaviour will happen
     */
    template<typename TObject>
    struct FObjectAsBytes
    {
        /** Input converted into bytes */
        FObjectAsBytes(const TObject& Object)
	        : ObjectBytes(reinterpret_cast<char*>(&Object))
			, ObjectSize(sizeof(TObject))
        {
        }

        /** Output converted from bytes with size check */
        TObject* GetObject()
        {
            // Security check: verify stored size matches current template type size
            if (ObjectSize != sizeof(TObject))
            {
                // Size mismatch - object corrupted or wrong type used
                return nullptr;
            }

            return reinterpret_cast<TObject*>(ObjectBytes);
        }

        char* ObjectBytes;
        uint64 ObjectSize;
    };

    template<typename T>
    static void WriteFieldSingle(std::vector<char>& Data, uint32 FieldId, const T& Value)
    {
        // Write field ID
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&FieldId),
            reinterpret_cast<const char*>(&FieldId) + sizeof(FieldId));

        // Write data size
        uint32 Size = sizeof(T);
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&Size),
            reinterpret_cast<const char*>(&Size) + sizeof(Size));

        // Write actual data
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&Value),
            reinterpret_cast<const char*>(&Value) + sizeof(Value));
    }

    template<typename T>
    static bool ReadFieldSingle(const std::vector<char>& Data, size_t& Offset, uint32 FieldId, T& OutValue)
    {
        while (Offset < Data.size())
        {
            // Read field ID
            uint32 CurrentId;
            std::memcpy(&CurrentId, Data.data() + Offset, sizeof(CurrentId));
            Offset += sizeof(CurrentId);

            // Read size
            uint32 Size;
            std::memcpy(&Size, Data.data() + Offset, sizeof(Size));
            Offset += sizeof(Size);

            // Check if this is the field we want
            if (CurrentId == FieldId)
            {
                std::memcpy(&OutValue, Data.data() + Offset, sizeof(T));
                Offset += Size;
                return true; // Found it
            }

            // Skip this field
            Offset += Size;
        }
        return false; // Not found
    }

    template<typename T>
    void WriteFieldVector(std::vector<char>& Data, uint32 FieldId, const std::vector<T>& Values)
    {
        // Write field ID
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&FieldId),
            reinterpret_cast<const char*>(&FieldId) + sizeof(FieldId));

        // Calculate total size (count + all elements)
        uint32 Count = static_cast<uint32>(Values.size());
        uint32 TotalSize = sizeof(Count) + (Count * sizeof(T));

        // Write total size
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&TotalSize),
            reinterpret_cast<const char*>(&TotalSize) + sizeof(TotalSize));

        // Write count
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&Count),
            reinterpret_cast<const char*>(&Count) + sizeof(Count));

        // Write all elements
        for (const auto& Value : Values)
        {
            Data.insert(Data.end(),
                reinterpret_cast<const char*>(&Value),
                reinterpret_cast<const char*>(&Value) + sizeof(T));
        }
    }

    template<typename T>
    static bool ReadFieldVector(const std::vector<char>& Data, size_t& Offset, uint32 FieldId, std::vector<T>& OutValues)
    {
        while (Offset < Data.size())
        {
            // Read field ID
            uint32 CurrentId;
            std::memcpy(&CurrentId, Data.data() + Offset, sizeof(CurrentId));
            Offset += sizeof(CurrentId);

            // Read total size
            uint32 Size;
            std::memcpy(&Size, Data.data() + Offset, sizeof(Size));
            Offset += sizeof(Size);

            // Check if this is our field
            if (CurrentId == FieldId)
            {
                // Read count
                uint32 Count;
                std::memcpy(&Count, Data.data() + Offset, sizeof(Count));
                Offset += sizeof(Count);

                // Read elements
                OutValues.resize(Count);
                for (uint32 i = 0; i < Count; ++i)
                {
                    std::memcpy(&OutValues[i], Data.data() + Offset, sizeof(T));
                    Offset += sizeof(T);
                }
                return true;
            }

            // Skip this field
            Offset += Size;
        }
        return false;
    }

    // Hash string at compile time to generate field ID
    static constexpr uint32 HashFieldName(const char* Str)
    {
        uint32 Hash = 5381;
        while (*Str)
        {
            Hash = ((Hash << 5) + Hash) + static_cast<unsigned char>(*Str++);
        }
        return Hash;
    }

	// Overloaded WriteField - automatically picks correct version
    template<typename T>
    static void WriteField(std::vector<char>& Data, uint32 FieldId, const T& Value)
    {
        WriteFieldSingle(Data, FieldId, Value);
    }
    template<typename T>
    static void WriteField(std::vector<char>& Data, uint32 FieldId, const std::vector<T>& Values)
    {
        WriteFieldVector(Data, FieldId, Values);
    }
    // More types supported for serialization can be added here

    // Overloaded ReadField - automatically picks correct version
    template<typename T>
    static bool ReadField(const std::vector<char>& Data, size_t& Offset, uint32 FieldId, T& OutValue)
    {
        return ReadFieldSingle(Data, Offset, FieldId, OutValue);
    }
    template<typename T>
    static bool ReadField(const std::vector<char>& Data, size_t& Offset, uint32 FieldId, std::vector<T>& OutValues)
    {
        return ReadFieldVector(Data, Offset, FieldId, OutValues);
    }
    // More types supported for de-serialization can be added here

    /**
	 * Save serialized data to file with verification
	 * @param Filename - Path to the file where data will be saved
	 * @param Data - Serialized data to write to file (must not be empty)
	 * @param SavingMode - File opening mode (default: binary mode)
	 * @return true if save succeeded, false if failed (empty data, cannot open file, or write error)
	 */
    static bool Save(const std::string& Filename, const std::vector<char>& Data, std::ios::openmode SavingMode = std::ios::binary)
    {
        // Verify data is not empty
        if (Data.empty())
        {
            return false;
        }

        std::ofstream File(Filename, SavingMode);
        if (!File)
        {
            return false;
        }

        // Write magic number for file format verification
        constexpr uint32 MagicNumber = 0x53455246; // "FSER" in hex
        File.write(reinterpret_cast<const char*>(&MagicNumber), sizeof(MagicNumber));

        // Write data size
        uint64 DataSize = Data.size();
        File.write(reinterpret_cast<const char*>(&DataSize), sizeof(DataSize));

        // Write actual data
        File.write(Data.data(), Data.size());

        // Verify write succeeded
        return File.good();
    }

    /**
     * Load serialized data from file with verification
     * @param Filename - Path to the file to read from
     * @param Data - Output vector that will contain loaded data (resized automatically)
     * @param SavingMode - File opening mode (default: binary mode)
     * @return true if load succeeded, false if failed (file not found, wrong format, corrupted, or read error)
     */
    static bool Load(const std::string& Filename, std::vector<char>& Data, std::ios::openmode SavingMode = std::ios::binary)
    {
        std::ifstream File(Filename, SavingMode);
        if (!File)
        {
            return false;
        }

        // Verify magic number
        constexpr uint32 MagicNumber = 0x53455246; // "FSER" in hex
        uint32 ReadMagic;
        File.read(reinterpret_cast<char*>(&ReadMagic), sizeof(ReadMagic));
        if (!File.good() || ReadMagic != MagicNumber)
        {
            return false; // Invalid file format
        }

        // Read data size
        uint64 DataSize;
        File.read(reinterpret_cast<char*>(&DataSize), sizeof(DataSize));
        if (!File.good() || DataSize == 0)
        {
            return false; // Invalid or corrupted file
        }

        // Verify file has enough data
        File.seekg(0, std::ios::end);
        size_t FileSize = File.tellg();
        if (FileSize < sizeof(MagicNumber) + sizeof(DataSize) + DataSize)
        {
            return false; // File truncated or corrupted
        }
        File.seekg(sizeof(MagicNumber) + sizeof(DataSize), std::ios::beg);

        // Read actual data
        Data.resize(DataSize);
        File.read(Data.data(), DataSize);

        // Verify read succeeded and we got all data
        return File.good() && File.gcount() == static_cast<std::streamsize>(DataSize);
    }
};
