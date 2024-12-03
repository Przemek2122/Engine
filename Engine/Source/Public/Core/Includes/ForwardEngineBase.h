// Created by Przemys³aw Wiewióra 2020 - 2022

#pragma once

// GEngine base classes forward declarations

class FEngineMain;
class FEngine;
class FEngineRender;
class FFileSystem;

class FMutex;
class FMutexScopeLock;

class FAssetsManager;
class FAssetBase;

class FRenderer;
class FWindow;

class FWidget;
class FTextWidget;
class FImageWidget;
class FBorderWidget;
class FHorizontalBoxWidget;
class FVerticalBoxWidget;

class FWidgetManager;
class IWidgetManagementInterface;
class IWidgetInteraction;

class FThread;
class FThreadData;
class FThreadsManager;
class FRenderThread;
class FMapManager;
class FEventHandler;
class FEngineManager;

class FTimer;

class FEntityManager;
class EEntity;
class UBaseComponent;
class UParentComponent;
class UComponent;

template<class TType = float>
class FVector2D;
class FColorRGBA;
template<typename TReturnType, typename... TInParams>
class FFunctorBase;
#include "Types/Delegate/Delegate.h"
#include "Types/Delegate/DelegateSafe.h"

/** Begin of containers */
template<typename TType, typename TSizeType>
class CContainerBase;
template<typename TType, typename TSizeType>
class CArray;
template<typename TKey, typename TValue, typename TSizeType>
class CMap;
template<typename TKey, typename TValue, typename TSizeType>
class CUnorderedMap;
template<typename TType, typename TSizeType>
class CDeque;
template<typename TType, typename TSizeType>
class CQueueSafe;
/** End of containers */

/** Begin of memory */
template<typename TTypeToStore>
class FAutoDeletePointer;
template<typename TTypeToStore>
class FAutoUIMenu;
#include "Memory/Class.h"
/** End of memory */
