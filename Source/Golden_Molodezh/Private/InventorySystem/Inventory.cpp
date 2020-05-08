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

bool AInventory::AddItem(ABaseItem* Item)
{
	if (Item == nullptr)
		return false;

	int32 firstEmptySlotIndex = -1;
	for (int i = 0; i < Slots.Num(); i++)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("1"));
		if (Slots[i] == nullptr) { // found empty
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Empty slot"));

			if (Item->MaxStackSize <= 1) // can be only one in slot
			{
				Slots[i] = Item;
				Item->PickUp();
				Item->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				if (OnSlotChanged.IsBound()) OnSlotChanged.Broadcast(i);

				return true;
			}
			else if (firstEmptySlotIndex == -1)
			{

				firstEmptySlotIndex = i;
			}
		}
		// same type and slot is not full
		else if (Item->GetClass() == Slots[i]->GetClass() && Slots[i]->Amount < Item->MaxStackSize)
		{
			int amountToAdd = FMath::Min(Slots[i]->MaxStackSize - Slots[i]->Amount, Item->Amount);
			Slots[i]->Amount += amountToAdd;
			Item->Amount -= amountToAdd;	// reduce Item amount
			if (OnSlotChanged.IsBound()) OnSlotChanged.Broadcast(i);
			if (Item->Amount == 0) { // and if it turns 0 destroy it, otherwise continue search slot for rest
				Item->Destroy();
				return true;
			}
		}
	}

	//not empty stack was not found
	//but was found empty stack
	if (firstEmptySlotIndex >= 0)
	{
		Slots[firstEmptySlotIndex] = Item;
		Item->PickUp();
		Item->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		if (OnSlotChanged.IsBound()) OnSlotChanged.Broadcast(firstEmptySlotIndex);
		return true;
	}

	return false;
}

ABaseItem* AInventory::PutItem(ABaseItem* Item, int Index)
{
	if (!Slots.IsValidIndex(Index))
		return nullptr;

	//like pop item
	if (Item == nullptr)
		return PopItem(Index);

	ABaseItem* itemToReturn = nullptr;

	//Just put in slot
	if (Slots[Index] == nullptr)
	{
		Item->PickUp();
		Item->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		Slots[Index] = Item;
	}
	//Same class and slot in inventory not full
	else if (Item->GetClass() == Slots[Index]->GetClass()
		&& Slots[Index]->Amount < Slots[Index]->MaxStackSize)
	{
		//Can add all items
		if (Slots[Index]->MaxStackSize - Slots[Index]->Amount >= Item->Amount)
		{
			Slots[Index]->Amount += Item->Amount;
			Item->Destroy();
		}
		else { //Add what can and return rest
			int itemToAdd = Slots[Index]->MaxStackSize - Slots[Index]->Amount;
			Item->Amount -= itemToAdd;
			Slots[Index]->Amount = Slots[Index]->MaxStackSize;
			itemToReturn = Item;
		}
	}
	else { //different classes or slot in inventry full. Swap Items
		itemToReturn = Slots[Index];
		Item->PickUp();
		Item->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		Slots[Index] = Item;
	}

	if (OnSlotChanged.IsBound()) OnSlotChanged.Broadcast(Index);
	return itemToReturn;
}

bool AInventory::IsSlotEmpty(int Index)
{
	if (!Slots.IsValidIndex(Index))
		return false;

	return Slots[Index] == nullptr;
}

ABaseItem* AInventory::GetItem(int Index)
{
	if (!Slots.IsValidIndex(Index))
		return nullptr;

	return Slots[Index];
}

int AInventory::GetItemCount(int Index)
{
	if (!Slots.IsValidIndex(Index))
		return 0;

	return Slots[Index]->Amount;
}

ABaseItem* AInventory::PopItem(int Index)
{
	if (!Slots.IsValidIndex(Index) || Slots[Index] == nullptr)
		return nullptr;

	ABaseItem* Item = Slots[Index];
	Slots[Index] = nullptr;
	if (OnSlotChanged.IsBound()) OnSlotChanged.Broadcast(Index);
	return Item;
}

ABaseItem* AInventory::PopItems(int Index, int Amount)
{
	if (!Slots.IsValidIndex(Index) || Slots[Index] == nullptr)
		return nullptr;

	if (Amount >= Slots[Index]->Amount)
		return PopItem(Index);

	FActorSpawnParameters Parameters;
	Parameters.Template = Slots[Index];
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABaseItem* newItem = GetWorld()->SpawnActor<ABaseItem>(Slots[Index]->GetClass(), Parameters);
	if (newItem == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Create NULL"));
		return nullptr;

	}
	newItem->SetOwner(GetOwner());
	newItem->Amount = Amount;
	Slots[Index]->Amount -= Amount;
	if (OnSlotChanged.IsBound()) OnSlotChanged.Broadcast(Index);

	return newItem;

	//ABaseItem* Item = Slots[Index];
	//ABaseItem* newItem = GetWorld()->SpawnActor(Item->GetClass(), Item->GetActorTransform);
	//Item->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);

	//return Item;
}

int AInventory::RemoveItem(int Index, int Amount)
{
	if (!Slots.IsValidIndex(Index) || Slots[Index] == nullptr)
		return 0;

	int amountToReturn = 0;
	if (Slots[Index]->Amount <= Amount) {
		amountToReturn = Slots[Index]->Amount;
		Slots[Index]->Destroy();
		Slots[Index] = nullptr;
	}
	else {
		Slots[Index]->Amount -= Amount;
	}

	if (OnSlotChanged.IsBound()) OnSlotChanged.Broadcast(Index);
	return amountToReturn;
}

bool AInventory::SwapSlots(int IndexFirst, int IndexSecond)
{
	if (!Slots.IsValidIndex(IndexFirst) || !Slots.IsValidIndex(IndexSecond))
		return false;

	ABaseItem* tmp = Slots[IndexFirst];
	Slots[IndexFirst] = Slots[IndexSecond];
	Slots[IndexSecond] = tmp;

	return true;
}


bool AInventory::UseItemAtIndex(int Index, AActor* User)
{
	if (!Slots.IsValidIndex(Index))
		return false;

	Slots[Index]->Use(User);
	return true;
}

TArray<ABaseItem*>& AInventory::GetSlots()
{
	return Slots;
}



