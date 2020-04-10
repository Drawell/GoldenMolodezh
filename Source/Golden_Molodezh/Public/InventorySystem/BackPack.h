// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#ifndef BACK_PACK_H
//#define BACK_PACK_H


#include "Inventory.h"
#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BackPack.generated.h"

UCLASS()
class GOLDEN_MOLODEZH_API ABackPack : public AInventory
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABackPack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

//#endif // !BACK_PACK_H