// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once
#include <string>

class ENGINE_API FObject
{
public:
	FObject();
	virtual ~FObject();

	/** Will return string like class <FClassName> like class FObject */
	NO_DISCARD std::string GetCppClassName() const;

	/** Will return <FClassName> like FObject */
	NO_DISCARD std::string GetCppClassNameWithoutClass() const;
	
};
