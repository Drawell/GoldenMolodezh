// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLDEN_MOLODEZH_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceComponent();
public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Resource)
		float Amount;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Resource)
		float MaxAmount;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Resource)
		float RegenerationAmount;

public:
	UFUNCTION(BlueprintCallable, Category = "Resource")
		float GetResourceInNormal();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
