// Created by Przemys³aw Wiewióra 2020

#pragma once

// GEngine
#include "Misc/Util.h"

#include "Assets/Assets/AssetBase.h"
#include "Assets/AssetsManager.h"

#include "Containers/ContainerBase.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/UnorderedMap.h"
#include "Containers/Deque.h"
#include "Containers/QueueSafe.h"

#include "Types/Vector2D.h"
#include "Types/ColorRGBA.h"
#include "Types/Functors/FunctorBase.h"

#include "Core/Object.h"
#include "Core/Engine.h"
#include "Core/EngineRender.h"
#include "Core/EngineMain.h"

#include "Core/Memory/AutoDeletePointer.h"
#include "Core/Memory/AutoUIMenu.h"

#include "Core/ECS/EntityManager.h"

#include "Renderer/Window.h"
#include "Renderer/Renderer.h"

#include "Renderer/Widgets/Widget.h"
#include "Renderer/Widgets/WidgetsManagementInterface.h"
#include "Renderer/Widgets/WidgetManager.h"
#include "Renderer/Widgets/WidgetInteraction.h"
