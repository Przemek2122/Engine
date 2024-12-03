// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"
#include "Assets/Parser.h"

FParserLine::FParserLine(FParserLine&& InOther) noexcept
	: Texts(std::move(InOther.Texts))
{
}

FParserLine::FParserLine(const FParserLine& InOther)
	: Texts(InOther.Texts)
{
}

FParserLine::FParserLine(CArray<FParserText> InTexts)
	: Texts(std::move(InTexts))
{
}

void FParserLine::operator=(const FParserLine& InOther)
{
	Texts = InOther.Texts;
}

FParser::FParser(const CArray<char>& InSeparatorCharArray, const CArray<char>& InCommentCharArray, const CArray<char>& InIgnoredCharArray)
	: SeparatorCharArray(InSeparatorCharArray)
	, CommentCharArray(InCommentCharArray)
	, IgnoredCharArray(InIgnoredCharArray)
{
	if (SeparatorCharArray.Size() == 0)
	{
		// This is crucial
		LOG_ERROR("FParser has no separator characters.");
	}

	if (CommentCharArray.Size() == 0)
	{
		// This is nice to have
		LOG_INFO("FParser has no comment characters.");
	}

	if (IgnoredCharArray.Size() == 0)
	{
		// This is not crucial
		LOG_INFO("FParser has no ignored characters.");
	}

	NewLineChar = FFileSystem::GetPlatformEndLine();
}

CArray<std::string> FParser::SimpleParseLineIntoStrings(const std::string& Line)
{
	CArray<std::string> ParsedStrings;

	std::string CurrentWord;

	for (const char& Char : Line)
	{
		if (IsComment(Char))
		{
			// Comment means we stop interpreting as comment goes to the end of the line
			break;
		}
		
		if (IsSeparator(Char))
		{
			ParsedStrings.Push(CurrentWord);

			CurrentWord.clear();
		}
		else if (!IsIgnored(Char))
		{
			CurrentWord += Char;
		}
	}

	if (CurrentWord.length() > 0)
	{
		ParsedStrings.Push(CurrentWord);
	}

	return ParsedStrings;
}

std::string FParser::SimpleParseStringsIntoLine(const CArray<std::string>& Strings)
{
	std::string ParsedLine;

	const int StringsCount = Strings.Size();
	const int LastStringIndex = StringsCount - 1;

	for (int i = 0; i < StringsCount; i++)
	{
		const bool bIsLast = (i == LastStringIndex);

		const std::string& String = Strings[i];

		if (bIsLast)
		{
			ParsedLine += String;
		}
		else
		{
			ParsedLine += String + SeparatorCharArray[0];
		}
	}

	return ParsedLine;
}

FParserLine FParser::AdvancedParseStringIntoLines(const std::string& Line)
{
	std::string CurrentWord;

	FParserLine ParserLine;

	EParserTextType ParserType = EParserTextType::Word;

	for (const char& CurrentChar : Line)
	{
		if (IsComment(CurrentChar) || ParserType == EParserTextType::Comment)
		{
			if (ParserType != EParserTextType::Comment)
			{
				ParserType = EParserTextType::Comment;

				// We got comment to end of the line - always
				if (!CurrentWord.empty())
				{
					ParserLine.Texts.Push(FParserText(CurrentWord, ParserType));

					CurrentWord.clear();
				}
			}

			CurrentWord += CurrentChar;
		}
		else if (IsIgnored(CurrentChar))
		{
			if (!CurrentWord.empty())
			{
				ParserLine.Texts.Push(FParserText(CurrentWord, ParserType));

				CurrentWord.clear();
			}

			ParserLine.Texts.Push(FParserText(TEXT_ADV(CurrentChar), EParserTextType::Ignored));
		}
		else
		{
			CurrentWord += CurrentChar;
		}
	}

	ParserLine.Texts.Push(FParserText(CurrentWord, ParserType));

	return std::move(ParserLine);
}

std::string FParser::AdvancedParseLinesIntoString(const CArray<FParserLine>& Lines)
{
	std::string Comment, Separator, Ignored;

	SetDefaultParseProperties(Comment, Separator, Ignored);

	std::string OutParsedString;

	for (ContainerInt CurrentLineIndex = 0; CurrentLineIndex < Lines.Size(); CurrentLineIndex++)
	{
		const FParserLine& ParserLine = Lines[CurrentLineIndex];

		std::string CurrentLine;

		for (ContainerInt ParserTextIndex = 0; ParserTextIndex < ParserLine.Texts.Size(); ParserTextIndex++)
		{
			const FParserText& FParserText = ParserLine.Texts[ParserTextIndex];

			std::string CurrentWord;

			switch (FParserText.Type)
			{
				case EParserTextType::Word:
				{
					// Do not add separator before first value
					if (ParserTextIndex == 0)
					{
						CurrentWord += FParserText.Text;
					}
					else
					{
						CurrentWord += SeparatorCharArray[0] + FParserText.Text;
					}

					break;
				}

				case EParserTextType::Comment:
				{
					CurrentWord += CommentCharArray[0] + FParserText.Text;

					break;
				}

				case EParserTextType::Ignored:
				{
					CurrentWord += IgnoredCharArray[0] + FParserText.Text;

					break;
				}

				case Unknown:
				{
					LOG_ERROR("FParser found unkown type of line.");

					break;
				}

				default:
				{
					LOG_ERROR("FParser found default type of line.");
				}
			}

			CurrentLine += CurrentWord;
		}

		if (CurrentLineIndex != Lines.Size() - 1)
		{
			OutParsedString += CurrentLine + NewLineChar;
		}
		else
		{
			// Do not add new line at the end (it is not needed for the last line
			OutParsedString += CurrentLine;
		}
	}

	return OutParsedString;
}

CArray<std::string> FParser::SplitString(const std::string& InString, const CArray<char>& InSeparatorCharArray)
{
	CArray<std::string> SubStrings;

	std::string CurrentSubString;

	for (char Char : InString)
	{
		if (InSeparatorCharArray.Contains(Char))
		{
			SubStrings.Push(CurrentSubString);

			CurrentSubString = "";
		}
		else
		{
			CurrentSubString += Char;
		}
	}

	return SubStrings;
}

void FParser::SetDefaultParseProperties(std::string& Comment, std::string& Separator, std::string& Ignored)
{
	if (SeparatorCharArray.Size() > 0)
	{
		Separator = SeparatorCharArray[0];
	}
	else if (FParserDefaults::DefaultSeparatorCharArray.Size() > 0)
	{
		Separator = FParserDefaults::DefaultSeparatorCharArray[0];
	}
	else
	{
		LOG_ERROR("FParser has no separator characters.");
	}

	if (CommentCharArray.Size() > 0)
	{
		Comment = CommentCharArray[0];
	}
	else if (FParserDefaults::DefaultCommentCharArray.Size() > 0)
	{
		Comment = FParserDefaults::DefaultCommentCharArray[0];
	}
	else
	{
		LOG_ERROR("FParser has no comment characters.");
	}

	if (IgnoredCharArray.Size() > 0)
	{
		Ignored = IgnoredCharArray[0];
	}
	else if (FParserDefaults::DefaultIgnoredCharArray.Size() > 0)
	{
		Ignored = FParserDefaults::DefaultIgnoredCharArray[0];
	}
	else
	{
		LOG_ERROR("FParser has no ignored characters.");
	}
}

bool FParser::AreCharsEqual(const char A, const char B)
{
	return (A == B);
}

bool FParser::IsComment(const char Any)
{
	bool bIsMatchFound = false;

	for (const char CommentChar : CommentCharArray)
	{
		if (AreCharsEqual(CommentChar, Any))
		{
			bIsMatchFound = true;

			break;
		}
	}

	return bIsMatchFound;
}

bool FParser::IsSeparator(const char Any)
{
	bool bIsMatchFound = false;

	for (const char& SeparatorChar : SeparatorCharArray)
	{
		if (AreCharsEqual(SeparatorChar, Any))
		{
			bIsMatchFound = true;

			break;
		}
	}

	return bIsMatchFound;
}

bool FParser::IsIgnored(char Any)
{
	bool bIsMatchFound = false;

	for (const char& IgnoredChar : IgnoredCharArray)
	{
		if (AreCharsEqual(IgnoredChar, Any))
		{
			bIsMatchFound = true;

			break;
		}
	}

	return bIsMatchFound;
}
