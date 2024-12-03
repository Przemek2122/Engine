// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "Misc/Math.h"

/* Two dimensional vector. */
template<class TType>
class FVector2D
{
public:
	FVector2D() : X(TType(0)), Y(TType(0)) {}

	/** Initialize X and Y with In */
	FVector2D(const TType In) 
		: X(In), Y(In)
	{
	}
	
	FVector2D(const TType InX, const TType InY) 
		: X(InX), Y(InY)
	{
	}

	template<typename TOldType>
	FVector2D(FVector2D<TOldType> OldVector) 
	{ 
		X = static_cast<TType>(OldVector.X); 
		Y = static_cast<TType>(OldVector.Y); 
	}

	template<typename TOldType>
	FVector2D& operator=(const FVector2D<TOldType> &OldVector) 
	{ 
		X = static_cast<TType>(OldVector.X); 
		Y = static_cast<TType>(OldVector.Y); 
		return *this; 
	}

	float Magnitude() const
	{
		return static_cast<float>(std::sqrt((X * X) + (Y * Y)));
	}

	// Function to get normalized vector 
	FVector2D<float> Normalize() const
	{
		FVector2D<float> NormalizedVector;

		float Len = Magnitude();
		if (Len > 0.0f)
		{
			NormalizedVector = FVector2D<float>(X / Len, Y / Len);
		}
		else
		{
			NormalizedVector = FVector2D<float>(0.f, 0.f);
		}

		return std::move(NormalizedVector);
	}

	static float Dot(const FVector2D<float> A, const FVector2D<float> B)
	{
		return ((A.X * B.X) + (A.Y * B.Y));
	}

	// Negate
	
	FVector2D operator-()
	{
		return FVector2D(-X, -Y);
	}
	FVector2D operator-() const
	{
		return FVector2D(-X, -Y);
	}

	static FVector2D Zero()
	{
		static FVector2D ZeroVector(0, 0);

		return ZeroVector;
	}

	// Basic calculations
	
	friend FVector2D operator+(const FVector2D<TType>& A, const FVector2D<TType>& B) 
	{ 
		return FVector2D<TType>(A.X + B.X, A.Y + B.Y); 
	}
	friend FVector2D operator-(const FVector2D<TType>& A, const FVector2D<TType>& B) 
	{
		return FVector2D<TType>(A.X - B.X, A.Y - B.Y); 
	}
	friend FVector2D operator*(const FVector2D<TType>& A, const FVector2D<TType>& B) 
	{ 
		return FVector2D<TType>(A.X * B.X, A.Y * B.Y); 
	}
	friend FVector2D operator/(const FVector2D<TType>& A, const FVector2D<TType>& B) 
	{ 
		return FVector2D<TType>(A.X / B.X, A.Y / B.Y); 
	}

	template <typename TOtherType>
	friend FVector2D operator+(const FVector2D<TOtherType>& L, const FVector2D<TType>& R) 
	{ 
		return FVector2D<TType>(static_cast<TType>(L.X) + R.X, static_cast<TType>(L.Y) + R.Y);
	}
	template <typename TOtherType>
	friend FVector2D operator-(const FVector2D<TOtherType>& L, const FVector2D<TType>& R) 
	{ 
		return FVector2D<TType>(static_cast<TType>(L.X) - R.X, static_cast<TType>(L.Y) - R.Y);
	}
	template <typename TOtherType>
	friend FVector2D operator*(const FVector2D<TOtherType>& L, const FVector2D<TType>& R) 
	{ 
		return FVector2D<TType>(static_cast<TType>(L.X) * R.X, static_cast<TType>(L.Y) * R.Y);
	}
	template <typename TOtherType>
	friend FVector2D operator/(const FVector2D<TOtherType>& L, const FVector2D<TType>& R) 
	{ 
		return FVector2D<TType>(static_cast<TType>(L.X) / R.X, static_cast<TType>(L.Y)/ R.Y);
	}

	FVector2D& operator+=(const FVector2D& V)
	{
		X += V.X; Y += V.Y; return *this;
	}
	FVector2D& operator-=(const FVector2D& V)
	{
		X -= V.X; Y -= V.Y; return *this;
	}
	FVector2D& operator*=(const FVector2D& V)
	{
		X *= V.X; Y *= V.Y; return *this;
	}
	FVector2D& operator/=(const FVector2D& V)
	{
		X /= V.X; Y /= V.Y; return *this;
	}


	FVector2D& operator*=(const TType& V)
	{
		X *= V; Y *= V; return *this;
	}
	FVector2D& operator/=(const TType& V)
	{
		X /= V; Y /= V; return *this;
	}

	// Comparison
	
	friend bool operator==(const FVector2D& L, const FVector2D& R)
	{
		return (L.X == R.X) && (L.Y == R.Y);
	}
	friend bool operator!=(const FVector2D& L, const FVector2D& R)
	{
		return (L.X != R.X) || (L.Y != R.Y);
	}
	friend bool operator<(const FVector2D& L, const FVector2D& R)
	{
		return (L.X < R.X) || (L.Y < R.Y);
	}
	friend bool operator<=(const FVector2D& L, const FVector2D& R)
	{
		return (L.X <= R.X) && (L.Y <= R.Y);
	}
	friend bool operator>(const FVector2D& L, const FVector2D& R)
	{
		return (L.X > R.X) || (L.Y > R.Y);;
	}
	friend bool operator>=(const FVector2D& L, const FVector2D& R)
	{
		return (L.X >= R.X) && (L.Y >= R.Y);;
	}

	/** Calculate distance from this vector to 'OtherVector' */
	TType DistanceTo(const FVector2D& OtherVector) const
	{
		const float DiffXSquared = static_cast<float>(FMath::Power(OtherVector.X - X));
		const float DiffYSquared = static_cast<float>(FMath::Power(OtherVector.Y - Y));

		return static_cast<TType>(FMath::Sqrt(DiffXSquared + DiffYSquared));
	}

	/** Return copy of rotated vector */
	FVector2D<TType> Rotate(const int32 AngleInDegrees)
	{
		const double AngleInRadians = FMath::DegreesToRadians(AngleInDegrees);

		return RotateRadians(AngleInRadians);
	}

	/**
	 * Return copy of rotated vector
	 * without conversion of Degrees to radians
	 */
	FVector2D<TType> RotateRadians(const double AngleInRadians)
	{
		FVector2D<TType> RetVector;

		RetVector.X = X * FMath::Cos(AngleInRadians) - Y * FMath::Sin(AngleInRadians);
		RetVector.Y = X * FMath::Sin(AngleInRadians) + Y * FMath::Cos(AngleInRadians);

		return RetVector;
	}


	/**
	 * Function that calculates point which is returned,
	 * which is at a distance 'InDistance' from point 'To' in the direction from point 'From' to point 'To'
	 */
	static FVector2D<TType> FindEscapingPoint(const FVector2D<TType>& From, const FVector2D<TType>& To, const float InDistance)
	{
		FVector2D<TType> OutVector;

		const double DistanceFromTo = From.DistanceTo(To);

		const double UnitVecX = (To.X - From.X) / DistanceFromTo;
		const double UnitVecY = (To.Y - From.Y) / DistanceFromTo;

		OutVector.X = To.X + InDistance * UnitVecX;
		OutVector.Y = To.Y + InDistance * UnitVecY;

		return OutVector;
	}

	//FVector2D operator*(const TType& S, const FVector2D<TType>& V) { return FVector2D<TType>(V) *= S; }
	//FVector2D operator*(const FVector2D<TType>& V, const TType& S) { return FVector2D<TType>(V) *= S; }
	//FVector2D operator/(const TType& S, const FVector2D<TType>& V) { return FVector2D<TType>(V) /= S; }
	//FVector2D operator/(const FVector2D<TType>& V, const TType& S) { return FVector2D<TType>(V) /= S; }

	//template<class TType> TType DotProduct(const Vector2d<TType>&, const Vector2d<TType>&);
	//template<class TType> TType CrossProduct(const Vector2d<TType>&, const Vector2d<TType>&);

	////Returns the length of the vector from the origin.
	//template<class TType> TType EuclideanNorm(const Vector2d<TType>&);

	//// Return the unit vector of the input
	//template<class TType> Vector2d<TType> Normal(const Vector2d<TType>&);

	////Return a vector perpendicular to the left.
	//template<class TType> Vector2d<TType> Perpendicular(const Vector2d<TType>&);

	////Return true if two line segments intersect.
	//template<class TType> bool Intersect(const Vector2d<TType>&, const Vector2d<TType>&, const Vector2d<TType>&, const Vector2d<TType>&);

	////Return the point where two lines intersect.
	//template<class TType> Vector2d<TType> GetIntersect(const Vector2d<TType>&, const Vector2d<TType>&, const Vector2d<TType>&, const Vector2d<TType>&)

	// Conversion
	operator FVector2D<int>() const
	{
		return { static_cast<int>(X), static_cast<int>(Y) };
	}

	operator SDL_Point() const
	{
		SDL_Point Point;
		Point.x = static_cast<int>(X);
		Point.y = static_cast<int>(Y);

		return std::move(Point);
	}

	// @TODO Fix warning
	// C4172 returning address of local variable or temporary
	operator SDL_Point*() const
	{
		SDL_Point Point;
		Point.x = static_cast<int>(X);
		Point.y = static_cast<int>(Y);

		return &Point;
	}

	operator SDL_FPoint() const
	{
		SDL_FPoint Point;
		Point.x = static_cast<float>(X);
		Point.y = static_cast<float>(Y);

		return std::move(Point);
	}

	// @TODO Fix warning
	// C4172
	operator SDL_FPoint*() const
	{
		SDL_FPoint Point;
		Point.x = static_cast<float>(X);
		Point.y = static_cast<float>(Y);

		return &Point;
	}

	friend std::ostream& operator<<(std::ostream& InputStream, const FVector2D& Vector)
	{
		InputStream << Vector.ToString();

		return InputStream;
	}

	operator std::string() const 
	{ 
		return ToString();
	}

	std::string ToString() const
	{
		return "(" + std::to_string(X) + ", " + std::to_string(Y) + ")";
	}

public:
	TType X;
	TType Y;
};
