#include "CoreEngine.h"
#include "Test/Samples/TestClassType.h"

#if ENGINE_TESTS_ALLOW_ANY
void FTestClassType::DoTest()
{
	const FTemporaryTestClass* ClassInstance = StoredClass.Allocate();
	if (ClassInstance != nullptr)
	{
		LOG_DEBUG("TemporaryTestClass instance created from class name: " << StoredClass.GetClassName());
	}

	delete ClassInstance;
}

char* FTestClassType::GetTestName() const
{
	return TEXT_CHAR("FTestClassType");
}
#endif
