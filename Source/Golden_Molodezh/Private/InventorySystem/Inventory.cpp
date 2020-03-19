// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInventory::SetSize(int NewSize)
{
	Slots.SetNum(NewSize);
	//for (int i = 0; i < Slots.Num; i++)
	//	Slots[i] = TArray<ABaseItem*>();

}

int AInventory::GetSize()
{
	return Slots.Num();
}

bool AInventory::AddItem(ABaseItem* item)
{
	int32 firstEmptySlotIndex = -1;
	for (int i = 0; i < Slots.Num(); i++)	
	{
		FSlot slot = Slots[i];
		if (slot.Num() == 0) // found empty		
			if (item->MaxStackSize <= 1) // can be only one in slot
			{
				slot.Add(item);
				return true;
			}
			else
			{
				firstEmptySlotIndex = i;
			}

		// same type and slot is not full
		if (item->GetClass() == slot.Get(0)->GetClass() && slot.Num() <= item->MaxStackSize)
		{
			slot.Add(item);
			return true;
		}
	}

	//not empty stack was not found
	//but was found empty stack
	if (firstEmptySlotIndex >= 0)
	{
		Slots[firstEmptySlotIndex].Add(item);
		return true;
	}

	return false;
}

bool AInventory::IsSlotEmpty(int index)
{
	if (index >= Slots.Num())
		return false;
	return Slots.Num() == 0;
}

ABaseItem * AInventory::GetItem(int index)
{
	if (index >= Slots.Num() || Slots[index].Num() == 0)
		return nullptr;

	return Slots[index].Last();
}

int AInventory::GetItemCount(int index)
{
	if (index >= Slots.Num())
		return 0;

	return Slots[index].Num();
}

ABaseItem * AInventory::PopItem(int index)
{
	if (index >= Slots.Num() || Slots[index].Num() == 0)
		return nullptr;

	return Slots[index].Pop();
}
