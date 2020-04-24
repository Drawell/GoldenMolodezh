// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobAbstract/ResourceComponent.h"
#include "HealthComponent.generated.h"

/**
 * 
 */
UCLASS()
class GOLDEN_MOLODEZH_API UHealthComponent : public UResourceComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
