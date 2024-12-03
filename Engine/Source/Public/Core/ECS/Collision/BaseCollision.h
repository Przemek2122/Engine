// Created by Przemys³aw Wiewióra 2024

#pragma once

struct FCollisionTile;
class UCollisionComponent;
class FCollisionManager;

enum class ECollisionType
{
	Circle,
	Square,
	Other
};

class FCollisionBase
{
	friend FCollisionManager;

public:
	FCollisionBase(UCollisionComponent* InCollisionComponent);
	virtual ~FCollisionBase() = default;

	/** @returns radius for basic distance check. */
	virtual int GetBaseExtentRadius();

	ECollisionType GetCollisionType() const;

	UCollisionComponent* GetCollisionComponent() const { return CollisionComponent; }

	const CArray<FCollisionTile*>& GetCurrentlyLocatedTiles() const { return CurrentlyLocatedTiles; }

protected:
	ECollisionType CollisionType;

	CArray<FCollisionBase*> OtherCollidersCurrentlyColliding;

	CArray<FCollisionTile*> CurrentlyLocatedTiles;

	UCollisionComponent* CollisionComponent;

};
