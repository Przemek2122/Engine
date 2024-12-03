#include "CoreEngine.h"
#include "Renderer/Map/MapEditor.h"
#include "Renderer/Map/Map.h"

FMapEditor::FMapEditor(FMap* InMap)
	: Map(InMap)
	, SelectedTileIndex(0)
{
}

void FMapEditor::Initialize()
{
}

void FMapEditor::DeInitialize()
{
}

void FMapEditor::SelectTileIndex(int Index)
{
	SelectedTileIndex = Index;
}

void FMapEditor::OnMapClicked(const FVector2D<int>& Location) const
{
	if (Map != nullptr)
	{
		Map->ChangeTileAtLocation(Location, SelectedTileIndex);
	}
	else
	{
		LOG_ERROR("Map is nullptr");
	}
}

void FMapEditor::SaveMap() const
{
	if (Map != nullptr)
	{
		Map->Save();
	}
	else
	{
		LOG_ERROR("Map is nullptr");
	}
}
