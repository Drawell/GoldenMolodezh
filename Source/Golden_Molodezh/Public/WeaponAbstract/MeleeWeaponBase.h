// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "WeaponAbstract/WeaponBase.h"
#include "MeleeWeaponBase.generated.h"

/**
 *
 */
UCLASS()
class GOLDEN_MOLODEZH_API AMeleeWeaponBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	AMeleeWeaponBase();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponMelee)
		USphereComponent* RotationPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponMelee)
		USphereComponent* HardPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponMelee)
		USphereComponent* DamagePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponMelee)
		int DistanceMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponMelee)
		int DistanceMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponMelee)
		int MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponMelee)
		float DamageRate;


	FTimerHandle DamageRateTimerHandler;

protected:
	FVector Destination;
	FVector PrevLocation;
	//static const int KoefFromScreenToLocal = 400;
	bool bNeedToMove = false;
	bool bCanDamage = true;
	int StartZ = 16;

public:
	virtual void StartUse(float X, float Y) override;
	virtual void ContinueUse(float X, float Y) override;
	virtual void StopUse(float X, float Y) override;

	UFUNCTION()
		void BeginOverlapDamagePart(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnCanDamage();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
