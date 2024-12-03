// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "WidgetEnums.h"
#include "WidgetStructs.h"

#define WIDGET_MAX_DEPTH 1024

/**
 * Class responsible for managing widgets. \n
 * It will be on WidgetManager class as well as on each widget as we can add widget to each of those.
 */
class IWidgetManagementInterface
{
	friend FWidget;
	
public:
	IWidgetManagementInterface();
	virtual ~IWidgetManagementInterface();

	/** @returns size, this is used to move widgets by default to relative location. */
	_NODISCARD virtual FVector2D<int> GetWidgetManagerOffset() const = 0;

	/** @returns Size of this interface. */
	_NODISCARD virtual FVector2D<int> GetWidgetManagerSize() const = 0;

	/** Get owner or nullptr if there is none */
	_NODISCARD virtual IWidgetManagementInterface* GetParent() const = 0;

	/** Calculate numer of parents. */
	_NODISCARD virtual int32 GetParentsNumber() const = 0;

	/** True if has owner */
	_NODISCARD virtual bool HasParent() const = 0;

	virtual bool NeedsWidgetRebuild() { return bIsWidgetRequestingRebuild; }

	/**
	 * Ticking widgets works different than render. \n
	 * First child widgets are ticked than parent.
	 */
	virtual void TickWidgets();
	
	/**
	 * Rendering of widgets. \n
	 * First parent is rendered than all its child and it's children ...
	 */
	virtual void RenderWidgets();

	/** Marks widget as requiring rebuild. */
	virtual void RequestWidgetRebuild();

	/** Marks widget as after finished rebuild. Rebuild will not be done. */
	virtual void MarkAsWidgetRebuild();

	/** Used to determine widget size */
	virtual void GenerateWidgetGeometry(FWidgetGeometry& InWidgetGeometry);

	/** Will be called before calling GenerateWidgetGeometry to determine widget size but only if requesting it using bIsWidgetRequestingRebuild */
	virtual void RebuildWidget();

	/**
	 * Add child by moving from other interface. \n
	 * We do not have RemoveChild because this AddChild handles removing previous parent.
	 */
	virtual bool AddChild(FWidget* InWidget);
	
	/** Create new widget from template, auto-managed. */
	template<class TWidgetTemplate>
	INLINE_DEBUGABLE TWidgetTemplate* CreateWidget(const char* InWidgetName, const int32 InWidgetOrder = WIDGET_DEFINES_DEFAULT_ORDER)
	{
		return CreateWidget<TWidgetTemplate>(std::string(InWidgetName), InWidgetOrder);
	}

	/** Create new widget from template, auto-managed. */
	template<class TWidgetTemplate>
	INLINE_DEBUGABLE TWidgetTemplate* CreateWidget(std::string InWidgetName = "", int32 InWidgetOrder = WIDGET_DEFINES_DEFAULT_ORDER)
	{
		if (InWidgetName == "")
		{
			InWidgetName = GetUniqueNameFor<TWidgetTemplate>();
		}

		if (ManagedWidgetsMap.ContainsKey(InWidgetName))
		{
			LOG_WARN("Widget with this name already exists! Duplicate: " << InWidgetName);

			InWidgetName = GetUniqueNameFor<TWidgetTemplate>();
		}

		if (InWidgetOrder == WIDGET_DEFINES_DEFAULT_ORDER)
		{
			InWidgetOrder = GetParentsNumber() + 1;
		}
		
		TWidgetTemplate* CreatedWidget = new TWidgetTemplate(this, InWidgetName, InWidgetOrder);

		OnChildWidgetCreated(CreatedWidget);

		return CreatedWidget;
	}

	/** Best to call Destroy widget. @returns true on success. */
	bool DestroyChildWidget(FWidget* Widget);
	/** Slower overload which destroys by name. @returns true on success. */
	bool DestroyChildWidgetByName(const std::string& InWidgetName);

	/**
	 * Deletes all children immediate, call last if this is in some binding.
	 * Using widget after this call is forbidden and may result in a crash
	 */
	void ClearChildren();

	/** @returns number of children managed by this object */
	ContainerInt GetChildrenCount() const;

	/** @returns widget by name SLOW */
	_NODISCARD FWidget* GetWidgetByName(const std::string& InWidgetName);
	/** @returns widget by name SLOW - This implementation does auto deduction of type. */
	template<typename FWidgetAuto>
	_NODISCARD FWidgetAuto GetWidgetByName(const std::string& InWidgetName)
	{
		return dynamic_cast<FWidgetAuto>(GetWidgetByName(InWidgetName));
	}
	
	/** @returns true if widget with this name exists SLOW */
	bool HasWidget(const std::string& InWidgetName);
	/** @returns true if widget exists SLOW */
	bool HasWidget(FWidget* InWidget);

	/** Called before any initialization */
	virtual void RegisterWidget(FWidget* Widget);
	/** Called after widget initialization */
	virtual void RegisterWidgetPostInit(FWidget* Widget);
	virtual void UnRegisterWidget(FWidget* Widget);
	
	/** @returns Window which created this manager */
	_NODISCARD virtual FWindow* GetOwnerWindow() const = 0;

	/** Called when window */
	virtual void OnWindowChanged() = 0;

	_NODISCARD const CArray<FWidget*>& GetManagedWidgets() const { return ManagedWidgets; }

	template<class TWidgetTemplate>
	std::string GetUniqueNameFor()
	{
		++LastWidgetNumber;

		const std::string ClassName = typeid(TWidgetTemplate).name();
		const std::string Id = std::to_string(LastWidgetNumber);

		return ClassName + ("_" + Id);
	}

	virtual void OnChildWidgetCreated(FWidget* NewWidget);
	virtual void OnChildWidgetDestroyed(FWidget* NewWidget);

	virtual void OnChildSizeChanged();

	/** @See OnAnyChildChangedDelegate */
	virtual void OnAnyChildChanged();

	/**
	 * Called when any widget is changed (created or destroyed).
	 * Performance heavy, will be triggered when any child has changed as well
	 */
	FDelegateSafe<void> OnAnyChildChangedDelegate;

	/** Called when any child widget needs rebuild */
	FDelegateSafe<void> OnAnyChildRequestedRebuild;

	/** Called when widget order is changed */
	FDelegateSafe<void, FWidget*> OnWidgetOrderChanged;

	/** Last widget number - using when generating unique names for widgets */
	int32 LastWidgetNumber;

protected:
	/** Called by wiget when order is changed. */
	void ChangeWidgetOrder(FWidget* InWidget);

protected:
	/** Has all widgets. First renders last, last first.... */
	CArray<FWidget*> ManagedWidgets;
	
	/** Maps string to widget. */
	CMap<std::string, FWidget*> ManagedWidgetsMap;

private:
	/** True if widget needs update, important because whole chain up to parent needs to update */
	bool bIsWidgetRequestingRebuild;

};
