// Created by Przemys³aw Wiewióra 2020

#pragma once
#include <string>

class FObject
{
public:
	FObject();
	virtual ~FObject();

	/** Will return string like class <FClassName> like class FObject */
	_NODISCARD std::string GetCppClassName() const;

	/** Will return <FClassName> like FObject */
	_NODISCARD std::string GetCppClassNameWithoutClass() const;
	
};
