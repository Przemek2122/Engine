// See https://github.com/Przemek2122/GameEngine

#pragma once

#include "CoreMinimal.h"
#include "WidgetsPositionInterface.h"

enum class EInputState;
class FWidgetInputManager;
class FInteractionBaseWidget;

/** 
 * Widgets can be created only from within FWidgetManager which is inside window or inside other widgets.
 * It's mostly to ensure widget render in proper window.
 *
 * LifeCycle:
 * Widget is Created using IWidgetManagementInterface it inherits from. @See CreateWidget inside of this interface.
 * Widget is destroying in same interface using DestroyWidget or DestroyWidget on widget you would like to destroy.
 * @Note It's worth mentioning that after destroying it's kept for one frame to ensure proper destruction of all children.
 *
 * Widgets are build one frame after requesting rebuild to avoid loops when creating widgets
 */
class ENGINE_API FWidget : public FObject, public IWidgetPositionInterface
{
	friend IWidgetManagementInterface;

protected:
	/** If creating outside manager make sure to send proper IWidgetManagementInterface. Otherwise exception will be thrown in debug. */
	FWidget(IWidgetManagementInterface* InWidgetManagementInterface, std::string InWidgetName, const int32 InWidgetOrder = WIDGET_DEFINES_DEFAULT_ORDER);
	~FWidget() override;

	/** Advanced, use Tick() if possible instead. */
	virtual void ReceiveTick();

	/** Advanced, use Render() if possible instead. */
	virtual void ReceiveRender();

	/** Called right after construction\n Usage: Set default size, bind delegates etc...*/
	virtual void Init();

	/** Called before DestroyWidget */
	virtual void PreDeInit();

	/** Called right before destruction, @NOTE PreDeInit is called earlier. */
	virtual void DeInit();

	/** Called each frame.\n Should be used for code logic. */
	virtual void Tick();

	/** Called each frame.\n Should be used To draw data only. @Note: When using Renderer calls use bIsLocationRelative=false in each function to prevent UI Moving with map */
	virtual void Render();

	virtual void SetupInput(FEventHandler* EventHandler);
	virtual void ClearInput(FEventHandler* EventHandler);

	virtual void OnWidgetOrderChanged();
	virtual void OnWidgetVisibilityChanged();
	void OnChildSizeChanged() override;

	virtual void OnMouseMove(FVector2D<int> InMousePosition, EInputState InputState);
	virtual bool OnMouseLeftClick(FVector2D<int> InMousePosition, EInputState InputState);
	virtual bool OnMouseRightClick(FVector2D<int> InMousePosition, EInputState InputState);

	void UpdateSizeToFitChildren();

public:
	void DestroyWidget();
	void FinalizeDestroyWidget();
	/** Currently a hack - Do not use at all. */
	void DestroyWidgetImmediate();

	void SetWidgetVisibility(const EWidgetVisibility InWidgetVisibility);
	void SetWidgetInteraction(const EWidgetInteraction InWidgetInteraction);
	void SetWidgetOrder(const int InWidgetOrder);
	void SetShouldChangeSizeOnChildChange(const bool bInShouldChangeSizeOnChildChange);

	/** Begin IWidgetManagementInterface */
	NO_DISCARD virtual FVector2D<int> GetWidgetManagerOffset() const override;
	NO_DISCARD virtual FVector2D<int> GetWidgetManagerSize() const override;
	NO_DISCARD virtual bool HasParent() const override;
	NO_DISCARD virtual FWindow* GetOwnerWindow() const override;
	virtual void OnWindowChanged() override;
	/** End IWidgetManagementInterface */

	/** True if DestroyWidget() has been called already */
	bool IsPendingDelete() const { return bIsPendingDelete; }
	bool IsVisible() const;
	bool IsInteractive() const;
	virtual bool ShouldConsumeInput() const;
	
	/** Decides if Render() should be called, affects all children */
	NO_DISCARD virtual bool ShouldBeRendered() const;

	/** Name of this widget. Can be displayed or widget can be get using this variable. */
	NO_DISCARD std::string GetName() const override;
	
	NO_DISCARD FWindow* GetWindow() const;
	NO_DISCARD FRenderer* GetRenderer() const;
	static FEventHandler* GetEventHandler();
	NO_DISCARD EWidgetVisibility GetWidgetVisibility() const;
	NO_DISCARD std::string GetWidgetVisibilityAsString() const;
	NO_DISCARD EWidgetInteraction GetWidgetInteraction() const;
	NO_DISCARD std::string GetWidgetInteractionAsString() const;
	NO_DISCARD int32 GetWidgetOrder() const;

	int32 GetParentsNumber() const override;

	/** @returns parent IWidgetManagementInterface pointer */
	NO_DISCARD IWidgetManagementInterface* GetParent() const override;

	/** @returns first parent (top of tree) */
	NO_DISCARD IWidgetManagementInterface* GetParentRoot() const;

	virtual void ReceiveOnMouseMove(FVector2D<int> InMousePosition, EInputState InputState);
	virtual bool ReceiveOnMouseLeftClick(FVector2D<int> InMousePosition, EInputState InputState);
	virtual bool ReceiveOnMouseRightClick(FVector2D<int> InMousePosition, EInputState InputState);

#if WITH_WIDGET_DEBUGGER
	void SetIsWidgetBeingDebugged(const bool bNewValue);
#endif

protected:
	/** Will be called when this widget or any parent was hidden */
	void OnVisibilityChangedToHidden();

	/** True if WidgetManagementInterface decided to render this widget. */
	bool bWasRenderedThisFrame;

private:
	/** True if widget is initialized */
	bool bWasInitCalled;

	/** Name of this widget. Can be displayed or widget can be get using this variable. */
	std::string WidgetName;

	/** Order - Higher = render first, interact first */
	int32 WidgetOrder;

	/** Visibility state of widget */
	EWidgetVisibility WidgetVisibility;

	/** Widget interaction */
	EWidgetInteraction WidgetInteraction;

	/** Owner manager */
	IWidgetManagementInterface* WidgetManagementInterface;

	/** if true widget is going to be destroyed */
	bool bIsPendingDelete;

	/** Cached input manager for widgets */
	FWidgetInputManager* WidgetInputManager;

#if WITH_WIDGET_DEBUGGER
	bool bIsWidgetBeingDebugged;
#endif
	
};
