// Created by https://www.linkedin.com/in/przemek2122/ 2024

#pragma once

/** Structure for exectution by Worker threads */
struct FAsyncWorkStructure
{
	std::shared_ptr<FDelegateSafe<>> DelegateToRunAsync;
	std::shared_ptr<FDelegateSafe<>> AsyncCallback;
};

/** Structure for exectution when async work finishes */
struct FMainThreadCallbackStructure
{
	FMainThreadCallbackStructure()
	{
	}

	FMainThreadCallbackStructure(const std::shared_ptr<FDelegateSafe<>>& InAsyncCallback)
		: AsyncCallback(InAsyncCallback)
	{
	}

	std::shared_ptr<FDelegateSafe<>> AsyncCallback;
};