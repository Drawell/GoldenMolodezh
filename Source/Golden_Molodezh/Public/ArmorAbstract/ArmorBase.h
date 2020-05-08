// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EquippedMobInterface.h"

#include "CoreMinimal.h"
#include "InventorySystem/BaseItem.h"
#include "ArmorBase.generated.h"

/**
 * 
 */
UCLASS()
class GOLDEN_MOLODEZH_API AArmorBase : public ABaseItem
{
	GENERATED_BODY()

public:
	AArmorBase();

	virtual void Use(AActor* User) override;
	virtual void ThrowToWorld(FVector WorldOffset) override;

	virtual void ShowFront();
	virtual void ShowBack();
	virtual void ShowRight();
	virtual void ShowLeft();
	virtual void ShowSide(uint8 Side);

	UFUNCTION(BlueprintCallable, Category = Armor)
	virtual void ShowInWorld();

public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = State)
		TArray<UPrimitiveComponent*> RightComponents;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = State)
		TArray<UPrimitiveComponent*> LeftComponents;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = State)
		TArray<UPrimitiveComponent*> FrontComponents;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = State)
		TArray<UPrimitiveComponent*> BackComponents;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Armor)
	//	bool bNeedToRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Armor)
		FVector LockalOffset;


private:
	uint8 PresentSide;
	const uint8 LEFT_SIDE = 0;
	const uint8 RIGHT_SIDE = 1;
	const uint8 FRONT_SIDE = 2;
	const uint8 BACK_SIDE = 3;
	TArray<UPrimitiveComponent*>& GetComponentArray(uint8 Side);
	const FRotator WorldRotation = FRotator(0.f, 0.f, -70.f);


};
