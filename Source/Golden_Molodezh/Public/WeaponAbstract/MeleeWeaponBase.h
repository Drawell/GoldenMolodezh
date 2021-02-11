// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "WeaponAbstract/FollowingWeapon.h"
#include "MeleeWeaponBase.generated.h"

/**
 *
 */
UCLASS()
class GOLDEN_MOLODEZH_API AMeleeWeaponBase : public AFollowingWeapon
{
	GENERATED_BODY()

public:
	AMeleeWeaponBase();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponMelee)
		USphereComponent* HardPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponMelee)
		USphereComponent* DamagePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponMelee)
		float DamageRate;


	FTimerHandle DamageRateTimerHandler;


public:

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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
