// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "Engine.h"

#include "CoreMinimal.h"
#include "WeaponAbstract/WeaponBase.h"
#include "FollowingWeapon.generated.h"

/**
 *
 */
UCLASS()
class GOLDEN_MOLODEZH_API AFollowingWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	AFollowingWeapon();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponFollowing)
		USphereComponent* RotationPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponFollowing)
		UPhysicsConstraintComponent* WeaponConstraint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponFollowing)
		float DistanceMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponFollowing)
		float DistanceMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponFollowing)
		float LengthwiseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponFollowing)
		float CrossSpeed;

protected:
	FVector Destination;
	FVector PrevLocation;
	bool bNeedToMove = false;
	bool bCanDamage = true;
	int StartZ = 16;

public:
	virtual void StartMove(float X, float Y) override;
	virtual void ContinueMove(float ToX, float ToY) override;
	virtual void StopMove(float X, float Y) override;

protected:
	virtual void Tick(float DeltaTime) override;	
	virtual void PostInitProperties() override;
};
