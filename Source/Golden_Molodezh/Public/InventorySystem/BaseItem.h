// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#ifndef BASE_ITEM_H
//#define BASE_ITEM_H

#include "Engine/Texture2D.h"
#include "Components/SphereComponent.h"
#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EItemCategoryEnum : uint8
{
	ICE_Useless = 0 UMETA(DisplayName = "Useless"),
	ICE_Consumable = 1 UMETA(DisplayName = "Consumable"),
	//ICE_Equipment  = 2  UMETA(DisplayName = "Equipment")

	//Weapon
	ICE_WeaponTwoHand = 10 UMETA(DisplayName = "Two Hand Weapon"),
	ICE_WeaponBothHand = 11 UMETA(DisplayName = "Both Hand Weapon"),
	ICE_WeaponOnlyLeftHand = 12 UMETA(DisplayName = "Only Left Hand Weapon"),
	ICE_WeaponOnlyRightHand = 13 UMETA(DisplayName = "Only Right Hand Weapon"),

	//Armor
	ICE_ArmorHelmet = 20 UMETA(DisplayName = "Helmet"),
	ICE_ArmorChest = 21 UMETA(DisplayName = "Chest"),
	ICE_ArmorBoot = 22 UMETA(DisplayName = "Boot"),
	ICE_ArmorNecklace = 23 UMETA(DisplayName = "Necklace"),
	ICE_ArmorAmulet = 24 UMETA(DisplayName = "Amulet"),


};

UCLASS(BlueprintType)
class GOLDEN_MOLODEZH_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseItem();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		USphereComponent* RootShpere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		EItemCategoryEnum Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		bool bCanBeUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FText UseText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		bool bPickedUp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void PickUp();

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void ThrowToWorld(FVector WorldOffset);

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void Use(AActor* User);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Item")
		void UseBluePrintImplemetnation(AActor* User);
};

//#endif // !BASE_ITEM_H