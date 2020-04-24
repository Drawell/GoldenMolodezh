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
	ICE_Useless		UMETA(DisplayName = "Useless"),
	ICE_Consumable 	UMETA(DisplayName = "Consumable"),
	ICE_Equipment 	UMETA(DisplayName = "Equipment")

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
		USphereComponent* Shpere;

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