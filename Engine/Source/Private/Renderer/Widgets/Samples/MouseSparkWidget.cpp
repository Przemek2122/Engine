// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Renderer/Widgets/Samples/MouseSparkWidget.h"
#include "Input/EventHandler.h"

FSpark::FSpark()
	: Location(0)
	, Speed(0)
	, Angle(0)
{
}

FSpark::FSpark(const FVector2D<int>& InLocation, const float InSpeed, const int InRotationDegree)
	: Location(InLocation)
	, Speed(InSpeed)
	, Angle(InRotationDegree)
{
}

FMouseSparkWidget::FMouseSparkWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder)
	: FWidget(InWidgetManagementInterface, InWidgetName, InWidgetOrder)
	, MinMaxSpeedRange(0.04f, 0.3f)
	, MinMaxAngleRange(0, 359)
	, MaxNumOfPoints(256)
	, PointPerTick(4)
{
}

void FMouseSparkWidget::Init()
{
	FWidget::Init();

	SetDefaultAnchor(EAnchor::None);
}

void FMouseSparkWidget::Tick()
{
	Super::Tick();

	// Add sparks is focused
	if (GetParent()->GetOwnerWindow()->IsWindowFocused())
	{
		const FEventHandler* EventHandler = GEngine->GetEventHandler();

		const FVector2D<int> MouseLocation = EventHandler->GetMouseLocationCurrent();

		for (int i = 0; i < PointPerTick; i++)
		{
			Sparks.PushBack(FSpark(
				MouseLocation, 
				FUtil::GetRandomValueFloating<float>(MinMaxSpeedRange.X, MinMaxSpeedRange.Y), 
				FUtil::GetRandomValue<int>(MinMaxAngleRange.X, MinMaxAngleRange.Y)
			));
		}
	}
	// Deque one by one (This will execute only when mouse is not on window)
	else if (Sparks.Size())
	{
		Sparks.DequeFront();
	}

	// Limit sparks
	while (Sparks.Size() > MaxNumOfPoints)
	{
		Sparks.DequeFront();
	}

	Points.Clear();
	Points.SetNum(Sparks.Size());
		
	for (int i = 0; i < Sparks.Size(); i++)
	{
		const FSpark& Spark = Sparks[i];
			
		const FVector2D<float> LocationChange = FUtil::GetPointAngle<float>(Spark.Speed, static_cast<float>(Spark.Angle));
		
		Sparks[i].Location += LocationChange;

		Points[i] = Sparks[i].Location;
	}
}

void FMouseSparkWidget::Render()
{
	Super::Render();
	
	if (const FRenderer* Renderer = GetRenderer())
	{
		Renderer->DrawPointsAt(Points, FColorRGBA(0, 153, 154), false);
	}
}
