// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#ifndef INVENTORY_H
//#define INVENTORY_H

#include "BaseItem.h"
#include "Engine.h"
#include "CoreMinimal.h"
#include "BaseChar.h"
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

	void Add(ABaseItem* Item)
	{
		Items.Add(Item);
	}

	ABaseItem* Get(int Index)
	{
		return Items[Index];
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


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSlotChangeEvent, int, SlotIndex);



UCLASS()
class GOLDEN_MOLODEZH_API AInventory : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInventory();

private:

	TArray<ABaseItem*> Slots;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetSize(int NewSize);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		int GetSize();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		bool AddItem(ABaseItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		ABaseItem* PutItem(ABaseItem* Item, int Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		bool IsSlotEmpty(int Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		ABaseItem* GetItem(int Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		int GetItemCount(int Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		ABaseItem* PopItem(int Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		ABaseItem* PopItems(int Index, int Amount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		int RemoveItem(int Index, int Amount);



	UFUNCTION(BlueprintCallable, Category = "Inventory")
		bool SwapSlots(int IndexFirst, int IndexSecond);




	UFUNCTION(BlueprintCallable, Category = "Inventory")
		bool UseItemAtIndex(int Index, ABaseChar* User);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		TArray<ABaseItem*>& GetSlots();

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FSlotChangeEvent OnSlotChanged;

	//DECLARE_EVENT_OneParam(AInventory, FSlotChangeEvent1, int)
	//FSlotChangeEvent1 OnSlotChanged1() { return ChangedEvent; }

	//UPROPERTY(BlueprintAssignable, Category = "Inventory")
		//FSlotChangeEvent1 ChangedEvent;


};

//#endif // !INVENTORY_H
