// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Object.h"

FObject::FObject()
{
}

FObject::~FObject()
{
}

std::string FObject::GetCppClassName() const
{
	return typeid(this).name();
}

std::string FObject::GetCppClassNameWithoutClass() const
{
	std::string ClassName = GetCppClassName();

	// Remove "class "
	ClassName = ClassName.replace(0, 5, "");

	return ClassName;
}
