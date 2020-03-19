// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseItem.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"


USTRUCT()
struct FSlot
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		TArray<ABaseItem*> Items;
		
	int Num()
	{
		return Items.Num();
	}

	void Add(ABaseItem* item)
	{
		Items.Add(item);
	}

	ABaseItem* Get(int index)
	{
		return Items[index];
	}

	ABaseItem* Last()
	{
		return Items.Last();
	}

	ABaseItem* Pop()
	{
		return Items.Pop(true);
	}
	   
	FSlot()
	{
	}
};

UCLASS()
class GOLDEN_MOLODEZH_API AInventory : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInventory();

private:

	//Contains slots. in one slot can be few items.
	//UPROPERTY(VisibleAnyWhere, Category = Inventory)
		TArray<FSlot> Slots;

	//UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Inventory)
	//TArray<uint32> ItemsInSlots;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetSize(int NewSize);

	UFUNCTION(BlueprintCallable)
		int GetSize();

	UFUNCTION(BlueprintCallable)
		bool AddItem(ABaseItem* item);

	UFUNCTION(BlueprintCallable)
		bool IsSlotEmpty(int index);

	UFUNCTION(BlueprintCallable)
		ABaseItem* GetItem(int index);

	UFUNCTION(BlueprintCallable)
		int GetItemCount(int index);

	UFUNCTION(BlueprintCallable)
		ABaseItem* PopItem(int index);
};
