// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Test/Samples/TestDelegate.h"

#if ENGINE_TESTS_ALLOW_ANY
void FTestDelegate::DoTest()
{
	TestFirstDelegate();

	TestSecondDelegate();

	TestThirdDelegate();

	TestFirstSafeDelegate();
}

char* FTestDelegate::GetTestName() const
{
	return TEXT_CHAR("FTestDelegate");
}

void FTestDelegate::TestFirstDelegate()
{
	{
		// Declaring type is optional - Could be 'auto'
		FFunctorLambda<void>* Lambda = new FFunctorLambda<void>(
			[]
			{
				LOG_DEBUG("Test#1 lambda called");
			});

		OnFirstDelegate.BindLambda(Lambda);
	}

	{
		auto Lambda2 = []
			{
				LOG_DEBUG("Test#2 lambda inline called");
			};

		OnFirstDelegate.BindLambda(Lambda2);
	}

	{
		OnFirstDelegate.BindLambda(new FFunctorLambda<void>(
			[]
			{
				LOG_DEBUG("Test#3 lambda inline called");
			}));

		OnFirstDelegate.BindLambda(
			[]
			{
				LOG_DEBUG("Test#4 lambda inline 2 called");
			});

		// Finally call all bound delegate functions.
		OnFirstDelegate.Execute();
	}
}

void FTestDelegate::TestSecondDelegate()
{
	OnSecondDelegate.BindLambda( [](int Test)
		{
			LOG_DEBUG("Test#4 lambda inline 2 called with int: " << Test);
		});

	OnSecondDelegate.Execute(234);
}

void FTestDelegate::TestThirdDelegate()
{
	OnThirdDelegate.BindLambda( [](int Test, float Test2)
		{
			LOG_DEBUG("Test#4 lambda inline 2 called with int: " << Test << " and float: " << Test2);
		});

	OnThirdDelegate.Execute(545, 5.5f);
}

void FTestDelegate::TestFirstSafeDelegate()
{
	auto LambdaInline = [](int Test, float Test2)
		{
			LOG_DEBUG("Test#5 lambda inline 2 called with int: " << Test << " and float: " << Test2);
		};

	OnFirstSafeDelegate.BindLambda(LambdaInline);

	FFunctorLambda<void, int, float> FunctorLambda = [](int Test, float Test2)
		{
			LOG_DEBUG("Test#5 lambda not-inline 2 called with int: " << Test << " and float: " << Test2);
		};

	OnFirstSafeDelegate.BindLambda(FunctorLambda);
	OnFirstSafeDelegate.UnBindLambda(FunctorLambda);
	OnFirstSafeDelegate.UnBindLambda(LambdaInline);

	OnFirstSafeDelegate.BindObject(this, &FTestDelegate::TestFirstSafeDelegateObjectCall);
	OnFirstSafeDelegate.UnBindObject(this, &FTestDelegate::TestFirstSafeDelegateObjectCall);

	OnFirstSafeDelegate.Execute(545, 5.5f);

	//OnFirstSafeDelegate.UnBindAll();
}

void FTestDelegate::TestFirstSafeDelegateObjectCall(int SomeInt, float SomeFloat)
{
	LOG_DEBUG("Test#5 object call with int: " << SomeInt << " and float: " << SomeFloat);
}
#endif
