// Created by https://www.linkedin.com/in/przemek2122/ 2025

#pragma once

#include "CoreMinimal.h"
#include <cstring>

/** Generate field Id, used by SERIALIZE_FIELD, does not need to be used manually */
#define FIELD_ID(Name) FSerializer::HashFieldName(#Name)

/**
 * Universal macros - work for both single values and vectors
 * @Note: Variable names when loading and saving must be the same!
 * @Note: Variables needs to be loaded in the same order as saved (Save var A, Save var B will be Load var A, Load var B)
 */
#define SERIALIZE_FIELD(Data, Field) FSerializer::WriteField(Data, FIELD_ID(Field), Field)
#define DESERIALIZE_FIELD(Data, Offset, Field) FSerializer::ReadField(Data, Offset, FIELD_ID(Field), Field)

/** Helper functions for serializer */
class FSerializerHelper
{
public:
    // Helper: Write string to buffer
    static void WriteString(std::vector<char>& Data, const std::string& Str)
    {
        uint32 Length = static_cast<uint32>(Str.size());
        // Write length
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&Length),
            reinterpret_cast<const char*>(&Length) + sizeof(Length));
        // Write string data
        Data.insert(Data.end(), Str.begin(), Str.end());
    }

    // Helper: Read string from buffer
    static bool ReadString(const std::vector<char>& Data, size_t& Offset, std::string& OutStr)
    {
        if (Offset + sizeof(uint32) > Data.size())
            return false;

        // Read length
        uint32 Length;
        std::memcpy(&Length, Data.data() + Offset, sizeof(Length));
        Offset += sizeof(Length);

        if (Offset + Length > Data.size())
            return false;

        // Read string data
        OutStr.resize(Length);
        std::memcpy(&OutStr[0], Data.data() + Offset, Length);
        Offset += Length;

        return true;
    }

    // Helper: Write any value (POD or string)
    template<typename T>
    static void WriteValue(std::vector<char>& Data, const T& Value)
    {
        if constexpr (std::is_same_v<T, std::string>)
        {
            WriteString(Data, Value);
        }
        else if constexpr (std::is_trivially_copyable_v<T>)
        {
            // POD type - direct binary write
            Data.insert(Data.end(),
                reinterpret_cast<const char*>(&Value),
                reinterpret_cast<const char*>(&Value) + sizeof(T));
        }
        else
        {
            static_assert(std::is_same_v<T, std::string>, "Type must be POD or std::string");
        }
    }

    // Helper: Read any value (POD or string)
    template<typename T>
    static bool ReadValue(const std::vector<char>& Data, size_t& Offset, T& OutValue)
    {
        if constexpr (std::is_same_v<T, std::string>)
        {
            return ReadString(Data, Offset, OutValue);
        }
        else if constexpr (std::is_trivially_copyable_v<T>)
        {
            // POD type - direct binary read
            if (Offset + sizeof(T) > Data.size())
                return false;

            std::memcpy(&OutValue, Data.data() + Offset, sizeof(T));
            Offset += sizeof(T);
            return true;
        }
        else
        {
            static_assert(std::is_same_v<T, std::string>, "Type must be POD or std::string");
            return false;
        }
    }

    // Helper: Calculate size of value
    template<typename T>
    static uint32 GetValueSize(const T& Value)
    {
        if constexpr (std::is_same_v<T, std::string>)
        {
            return sizeof(uint32) + static_cast<uint32>(Value.size());
        }
        else
        {
            return sizeof(T);
        }
    }

};

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

    // Write single value field (supports POD and string)
    template<typename T>
    static void WriteFieldSingle(std::vector<char>& Data, uint32 FieldId, const T& Value)
    {
        // Compile-time check: must be POD or string
        static_assert(
            std::is_trivially_copyable_v<T> || std::is_same_v<T, std::string>,
            "Type must be POD (trivially copyable) or std::string"
            );

        // Write field ID
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&FieldId),
            reinterpret_cast<const char*>(&FieldId) + sizeof(FieldId));

        // Write data size
        uint32 Size = FSerializerHelper::GetValueSize(Value);
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&Size),
            reinterpret_cast<const char*>(&Size) + sizeof(Size));

        // Write actual data using helper
        FSerializerHelper::WriteValue(Data, Value);
    }

    // Read single value field (supports POD and string)
    template<typename T>
    static bool ReadFieldSingle(const std::vector<char>& Data, size_t& Offset, uint32 FieldId, T& OutValue)
    {
        // Compile-time check: must be POD or string
        static_assert(
            std::is_trivially_copyable_v<T> || std::is_same_v<T, std::string>,
            "Type must be POD (trivially copyable) or std::string"
            );

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
                // Use helper to read value
                return FSerializerHelper::ReadValue(Data, Offset, OutValue);
            }

            // Skip this field
            Offset += Size;
        }

        return false; // Not found
    }

    // Write vector field (supports POD and string)
    template<typename T>
    void WriteFieldVector(std::vector<char>& Data, uint32 FieldId, const std::vector<T>& Values)
    {
        // Compile-time check: must be POD or string
        static_assert(
            std::is_trivially_copyable_v<T> || std::is_same_v<T, std::string>,
            "Type must be POD (trivially copyable) or std::string"
            );

        // Write field ID
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&FieldId),
            reinterpret_cast<const char*>(&FieldId) + sizeof(FieldId));

        // Calculate total size (count + all elements)
        uint32 Count = static_cast<uint32>(Values.size());
        uint32 TotalSize = sizeof(Count);

        for (const auto& Value : Values)
        {
            TotalSize += GetValueSize(Value);
        }

        // Write total size
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&TotalSize),
            reinterpret_cast<const char*>(&TotalSize) + sizeof(TotalSize));

        // Write count
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&Count),
            reinterpret_cast<const char*>(&Count) + sizeof(Count));

        // Write all elements using helper
        for (const auto& Value : Values)
        {
            FSerializerHelper::WriteValue(Data, Value);
        }
    }

    // Read vector field (supports POD and string)
    template<typename T>
    static bool ReadFieldVector(const std::vector<char>& Data, size_t& Offset, uint32 FieldId, std::vector<T>& OutValues)
    {
        // Compile-time check: must be POD or string
        static_assert(
            std::is_trivially_copyable_v<T> || std::is_same_v<T, std::string>,
            "Type must be POD (trivially copyable) or std::string"
            );

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

                // Read elements using helper
                OutValues.resize(Count);
                for (uint32 i = 0; i < Count; ++i)
                {
                    if (!FSerializerHelper::ReadValue(Data, Offset, OutValues[i]))
                        return false;
                }

                return true;
            }

            // Skip this field
            Offset += Size;
        }

        return false;
    }

    // Write unordered_map to binary data buffer
    template<typename K, typename V>
    static void WriteFieldUnorderedMap(std::vector<char>& Data, uint32 FieldId, const std::unordered_map<K, V>& Map)
    {
        // Compile-time checks for both K and V
        static_assert(
            std::is_trivially_copyable_v<K> || std::is_same_v<K, std::string>,
            "Map key type must be POD (trivially copyable) or std::string"
            );
        static_assert(
            std::is_trivially_copyable_v<V> || std::is_same_v<V, std::string>,
            "Map value type must be POD (trivially copyable) or std::string"
            );

        // Write field ID
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&FieldId),
            reinterpret_cast<const char*>(&FieldId) + sizeof(FieldId));

        // Calculate total size
        uint32 Count = static_cast<uint32>(Map.size());
        uint32 TotalSize = sizeof(Count);

        for (const auto& [Key, Value] : Map)
        {
            TotalSize += FSerializerHelper::GetValueSize(Key);
            TotalSize += FSerializerHelper::GetValueSize(Value);
        }

        // Write total size
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&TotalSize),
            reinterpret_cast<const char*>(&TotalSize) + sizeof(TotalSize));

        // Write count
        Data.insert(Data.end(),
            reinterpret_cast<const char*>(&Count),
            reinterpret_cast<const char*>(&Count) + sizeof(Count));

        // Write all key-value pairs
        for (const auto& [Key, Value] : Map)
        {
            FSerializerHelper::WriteValue(Data, Key);
            FSerializerHelper::WriteValue(Data, Value);
        }
    }

    // Read unordered_map from binary data buffer
    template<typename K, typename V>
    static bool ReadFieldUnorderedMap(const std::vector<char>& Data, size_t& Offset, uint32 FieldId, std::unordered_map<K, V>& OutMap)
    {
        // Compile-time checks for both K and V
        static_assert(
            std::is_trivially_copyable_v<K> || std::is_same_v<K, std::string>,
            "Map key type must be POD (trivially copyable) or std::string"
            );
        static_assert(
            std::is_trivially_copyable_v<V> || std::is_same_v<V, std::string>,
            "Map value type must be POD (trivially copyable) or std::string"
            );

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

            if (CurrentId == FieldId)
            {
                // Read count
                uint32 Count;
                std::memcpy(&Count, Data.data() + Offset, sizeof(Count));
                Offset += sizeof(Count);

                OutMap.clear();
                OutMap.reserve(Count);

                // Read all key-value pairs
                for (uint32 i = 0; i < Count; ++i)
                {
                    K Key;
                    V Value;

                    if (!FSerializerHelper::ReadValue(Data, Offset, Key))
                        return false;

                    if (!FSerializerHelper::ReadValue(Data, Offset, Value))
                        return false;

                    OutMap[Key] = Value;
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
    template<typename K, typename V>
    static void WriteField(std::vector<char>& Data, uint32 FieldId, const std::unordered_map<K, V>& Values)
    {
        WriteFieldUnorderedMap(Data, FieldId, Values);
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
    template<typename K, typename V>
    static bool ReadField(const std::vector<char>& Data, size_t& Offset, uint32 FieldId, std::unordered_map<K, V>& OutMap)
    {
        return ReadFieldUnorderedMap(Data, Offset, FieldId, OutMap);
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
