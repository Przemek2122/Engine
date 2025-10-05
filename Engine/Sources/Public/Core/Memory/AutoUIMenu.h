// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "Renderer/Widgets/UIMenu.h"

/**
 * \brief Class for FUIMenu auto cleanup.
 * \tparam TTypeToStore must be of type FUIMenu
 */
template<typename TTypeToStore>
class FAutoUIMenu : public FAutoDeletePointer<TTypeToStore>
{
public:
	FAutoUIMenu()
		: FAutoDeletePointer<TTypeToStore>()
	{
		static_assert(std::is_base_of<FUIMenu, TTypeToStore>(), "TTypeToStore must be derived from FUserUI!");
	}

	/** Normal constructor - Create object to store */
	template<typename ...TInParams>
	explicit FAutoUIMenu(TInParams... Params)
		: FAutoDeletePointer<TTypeToStore>(Params ...)
	{
		static_assert(std::is_base_of<FUIMenu, TTypeToStore>(), "TTypeToStore must be derived from FUserUI!");
	}

	/** Normal constructor - Pass in object to store */
	explicit FAutoUIMenu(TTypeToStore* Params)
		: FAutoDeletePointer<TTypeToStore>(Params)
	{
		static_assert(std::is_base_of<FUIMenu, TTypeToStore>(), "TTypeToStore must be derived from FUserUI!");
	}

	/** Copy constructor - Makes previous FAutoDeletePointer not usable anymore. */
	explicit FAutoUIMenu(FAutoUIMenu& OtherAutoUIMenu) noexcept
		: FAutoDeletePointer<TTypeToStore>(OtherAutoUIMenu)
	{
		static_assert(std::is_base_of<FUIMenu, TTypeToStore>(), "TTypeToStore must be derived from FUserUI!");
	}

	/** Move constructor - Makes previous FAutoDeletePointer not usable anymore. */
	FAutoUIMenu(FAutoUIMenu&& OtherAutoUIMenu) noexcept
		: FAutoDeletePointer<TTypeToStore>(OtherAutoUIMenu)
	{
		static_assert(std::is_base_of<FUIMenu, TTypeToStore>(), "TTypeToStore must be derived from FUserUI!");
	}

	FAutoUIMenu& operator=(FAutoUIMenu&& OtherAutoUIMenu) noexcept
	{
		static_assert(std::is_base_of<FUIMenu, TTypeToStore>(), "TTypeToStore must be derived from FUserUI!");

		// We can not move class if there is already a class.
		ENSURE_VALID(this->StoredObject == nullptr);

		this->StoredObject = OtherAutoUIMenu.StoredObject;

		OtherAutoUIMenu.MarkAsMoved();

		this->bHasAnyObject = (this->StoredObject != nullptr);

		return *this;
	}

	~FAutoUIMenu() override
	{
		if (this->bHasAnyObject)
		{
			FUIMenu* UIMenu = dynamic_cast<FUIMenu*>(this->StoredObject);
			if (UIMenu)
			{
				UIMenu->DeInitializePublic();
			}
			else
			{
				LOG_ERROR("StoredObject is not of type FUIMenu!");
			}
		}
	}
};
