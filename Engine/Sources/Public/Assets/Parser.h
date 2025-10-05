// See https://github.com/Przemek2122/GameEngine

#pragma once

#include "CoreMinimal.h"

/** Defaults for Parser. */
namespace FParserDefaults
{
	static const CArray DefaultSeparatorCharArray = { ',' };
	static const CArray DefaultCommentCharArray = { '#' };
	static const CArray DefaultIgnoredCharArray = { ' ' };
}

/** Enum for ParserLineType, it's used to determine what type of line is it. */
enum EParserTextType : Uint8
{
	/** This type will not have anything added before text */
	Unknown = 0,
	/** This means it's pure word and will have separator added */
	Word,
	/** This type will have comment added before text */
	Comment,
	/** This type will have ignored char added before text @see FParserDefaults::DefaultIgnoredCharArray for defaults */
	Ignored,
};

/** Text which is inside of the line, parsed by dividing one chars of InSeparatorCharArray of FParser */
struct FParserText
{
	FParserText() = default;

	FParserText(std::string InText, const EParserTextType InLineType)
		: Text(std::move(InText))
		, Type(InLineType)
	{
	}

	/** Text of the line */
	std::string Text;

	/** Type of the line */
	EParserTextType Type;
};

/** Struct for holding parsed line from file or lines to parse into file. */
struct FParserLine
{
	FParserLine() = default;

	/** Move constructor */
	FParserLine(FParserLine&& InOther) noexcept;

	/** Copy constructor */
	FParserLine(const FParserLine& InOther);

	FParserLine(CArray<FParserText> InTexts);

	void operator=(const FParserLine& InOther);

	CArray<FParserText> Texts;
};

/**
 * Class for parsing strings into words, it's meant to be used for parsing files and commands.
 * It's not meant to be used for parsing code, as it's not meant to be fast.
 */
class FParser
{
public:
	/**
	 * Constructor for Parser, it's required to call before anything else in this class as it sets basic properties.
	 * @param InSeparatorCharArray Defines which chars are meant to be words breaker
	 * @param InIgnoredCharArray Defines which chars are ignored like they were not present
	 * @param InCommentCharArray Defines which chars means comment and rest of the line after them are ignored.
	 */
	FParser(
		const CArray<char>& InSeparatorCharArray,
		const CArray<char>& InCommentCharArray = FParserDefaults::DefaultCommentCharArray,
		const CArray<char>& InIgnoredCharArray = FParserDefaults::DefaultIgnoredCharArray
	);

	/**
	 * God for line by line work. Bad for anything el
	 * Takes given string and splits it into words by Separators set in constructor
	 */
	CArray<std::string> SimpleParseLineIntoStrings(const std::string& Line);

	/**
	 * God for line by line work. Bad for anything else
	 * Reverse of SimpleParseLineIntoStrings, for saving purposes
	 */
	std::string SimpleParseStringsIntoLine(const CArray<std::string>& Strings);

	/**
	 * It's more complex and it returns comments
	 * Takes given string and splits it into lines by Separators set in constructor
	 */
	FParserLine AdvancedParseStringIntoLines(const std::string& Line);

	/**
	 * Reverse of AdvancedParseStringIntoLines, for saving purposes
	 * It's more complex and it adds comments
	 * Takes given lines and merges them into one string with separators
	 */
	std::string AdvancedParseLinesIntoString(const CArray<FParserLine>& Lines);

	/** Splits given string by given  */
	static CArray<std::string> SplitString(const std::string& InString, const CArray<char>& InSeparatorCharArray);

	/** Sets default properties depending on arrays */
	void SetDefaultParseProperties(std::string& Comment, std::string& Separator, std::string& Ignored);

	/** Utility to compare characters */
	inline static bool AreCharsEqual(char A, char B);

	bool IsComment(char Any);
	bool IsSeparator(char Any);
	bool IsIgnored(char Any);

protected:
	CArray<char> SeparatorCharArray;
	CArray<char> CommentCharArray;
	CArray<char> IgnoredCharArray;
	char NewLineChar;

};
