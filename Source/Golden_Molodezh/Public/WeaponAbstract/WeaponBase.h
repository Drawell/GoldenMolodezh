// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EquippedMobInterface.h"

#include "CoreMinimal.h"
#include "InventorySystem/BaseItem.h"
#include "WeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class GOLDEN_MOLODEZH_API AWeaponBase : public ABaseItem
{
	GENERATED_BODY()

public:
    AWeaponBase();

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
        bool bIsMoving;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
        float DamageAmount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
        AActor* User;


public:
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void StartMove(float X, float Y);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void ContinueMove(float ToX, float ToY);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void StopMove(float X, float Y);

    virtual void Use(AActor* User) override;

protected:
    int StartX;
    int StartY;
    

};
