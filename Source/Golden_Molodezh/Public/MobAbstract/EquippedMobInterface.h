// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BaseItem.h"

#include "Engine.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquippedMobInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquippedMobInterface : public UInterface
{
	GENERATED_BODY()
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentSlotChangeEvent, FName, SlotName);
/**
 * 
 */
class GOLDEN_MOLODEZH_API IEquippedMobInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
		ABaseItem* Equip(ABaseItem* Equipment);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
		ABaseItem* EquipAtSlot(ABaseItem* Equipment, FName SlotName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
		bool CanEquip(ABaseItem* Equipment);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
		bool CanEquipAtSlot(ABaseItem* Equipment, FName SlotName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
		ABaseItem* GetItemAtSlot(FName SlotName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
		ABaseItem* PopItemAtSlot(FName SlotName);
	
};
