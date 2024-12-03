// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "AI/AITree.h"
#include "ECS/BaseComponent.h"

class FGameModeBase;
class FWindowAdvanced;
class FMap;
class FGameModeManager;
class FEntityManager;

/**
 * Generic entity class
 */
class EEntity : public FObject, public IComponentManagerInterface
{
public:
	EEntity(FEntityManager* InEntityManager);
	~EEntity() override = default;

	/** Called when starting or creating Entity */
	virtual void BeginPlay();

	/** Called right before destruction */
	virtual void EndPlay();

	/** Called every frame */
	virtual void Tick(float DeltaTime);

	/** Called every frame from engine code. */
	void ReceiveTick(float DeltaTime);

	/** Called every frame for rendering purposes */
	virtual void Render();

	/** Called every frame from engine code. */
	void ReceiveRender();

	/** Tells us if we are attached to other entity */
	bool IsAttached() const;
	/** Removes attachment */
	void ResetAttachment();
	/** Attach to other entity */
	void AttachToEntity(EEntity* InEntityToAttachTo);
	EEntity* GetEntityAttachment() const { return EntityAttachment; }
		
	virtual void SetRootComponent(UParentComponent* NewComponent);
	virtual UParentComponent* GetRootComponent() const;

	void SetLocation(const FVector2D<int32> NewLocation);
	void SetRotation(int32 Rotation);

	void SetRelativeLocation(const FVector2D<int32> NewLocation);
	void SetRelativeRotation(const int32 NewRotation);

	const FVector2D<int32>& GetRelativeLocation() const { return AttachmentRelativeLocation; }
	int32 GetRelativeRotation() const { return AttachmentRelativeRotation; }

	/** @Returns value of parent component (if present) */
	FVector2D<int32> GetLocation() const;

	/** @Returns value of parent component (if present) */
	int32 GetRotation() const;

	FEntityManager* GetEntityManagerOwner() const;
	FWindow* GetWindow() const;
	FWindowAdvanced* GetWindowAdvanced() const;
	FGameModeManager* GetGameModeManager() const;
	FGameModeBase* GetGameMode() const;
	FMap* GetCurrentMap() const;

	const CArray<std::shared_ptr<FAIMemorySet>>& GetAIMemorySetArray() const { return AIMemorySetArray; }

	/** Get AIMemorySet by class, it will iterate all existing MemorySets to find correct one */
	template<typename TAIMemorySetClass>
	std::shared_ptr<TAIMemorySetClass> GetAIMemorySetByClass()
	{
		ASSERT_IS_BASE_OF(FAIMemorySet, TAIMemorySetClass, "Class mismatch, GetAIMemorySetByClass requries class to inherit from FAIMemorySet.");

		std::shared_ptr<TAIMemorySetClass> RetAIMemorySetPtr;

		for (std::shared_ptr<FAIMemorySet>& AIMemorySetPtr : AIMemorySetArray)
		{
			if (dynamic_cast<TAIMemorySetClass*>(AIMemorySetPtr.get()) != nullptr)
			{
				RetAIMemorySetPtr = std::dynamic_pointer_cast<TAIMemorySetClass>(AIMemorySetPtr);
			}
		}

		return RetAIMemorySetPtr;
	}

protected:
	/** Register input to FEventHandler. Remember to unregister input in UnRegisterInput! */
	virtual void RegisterInput(FEventHandler* InputHandler);
	virtual void UnRegisterInput(FEventHandler* InputHandler);

	void RegisterInputInternal();
	void UnRegisterInputInternal();

	virtual void SetupAIActions();

	template<typename TAITreeClass>
	TAITreeClass* CreateAiTree()
	{
		ASSERT_IS_BASE_OF(FAITree, TAITreeClass, "Class mismatch, CreateAiTree requries class to inherit from FAITree.");

		std::shared_ptr<TAITreeClass> AiTreePtr = std::make_shared<TAITreeClass>(this);

		AiTreeArray.Push(AiTreePtr);

		return AiTreePtr.get();
	}

	/** Create new AIMemorySet */
	template<class TAIMemorySetClass>
	void CreateAIMemorySet()
	{
		ASSERT_IS_BASE_OF(FAIMemorySet, TAIMemorySetClass, "Class mismatch, CreateAIMemorySet requries class to inherit from FAIMemorySet.");

		// Create set without returning it as we want to keep it as shared_ptr (GetAIMemorySetByClass can be used to get it.)
		std::shared_ptr<TAIMemorySetClass> NewAIMemorySet = std::make_shared<TAIMemorySetClass>();

		AIMemorySetArray.Push(NewAIMemorySet);
	}

	/** Begin IComponentManagerInterface */
	void OnComponentCreated(const std::string& ComponentName, UBaseComponent* NewComponent) override;
	/** End IComponentManagerInterface */

	virtual void OnAttachedToEntity();
	virtual void OnDeAttachedFromEntity();

	virtual void OnAttachedComponentLocationChanged(const FTransform2DLocation& NewLocation);
	virtual void OnAttachedComponentRotationChanged(const FTransform2DRotation NewRotation);

private:
	/** Array of AI memory sets. */
	CArray<std::shared_ptr<FAIMemorySet>> AIMemorySetArray;

	/** Entity manager */
	FEntityManager* EntityManagerOwner;

	/** Root component. First component added or set by user */
	UParentComponent* DefaultRootComponent;

	/** Ai tree array */
	CArray<std::shared_ptr<FAITree>> AiTreeArray;

	/** Used to avoid double calling BeginPlay */
	bool bWasBeginPlayCalled;

	/** Entity which this entity is attached to */
	EEntity* EntityAttachment;

	/** EntityAttachment's root component cache */
	UParentComponent* EntityAttachmentRootComponent;

	/** Used to define relative location to attached actor */
	FVector2D<int32> AttachmentRelativeLocation;

	/** Used to define relative rotation to attached actor */
	int32 AttachmentRelativeRotation;

};
