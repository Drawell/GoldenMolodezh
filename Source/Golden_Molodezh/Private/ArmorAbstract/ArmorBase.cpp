// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorBase.h"

AArmorBase::AArmorBase() :
	Super()
{
	Category = EItemCategoryEnum::ICE_ArmorChest;
	MaxStackSize = 1;
	Amount = 1;
	PresentSide = 255;
	ShowInWorld();
}

void AArmorBase::Use(AActor* User)
{
	if (User == nullptr)
		return;

	IEquippedMobInterface* user = Cast<IEquippedMobInterface>(User);
	if (user == nullptr)
		return;

	if (user->CanEquip_Implementation(this))
		user->Equip_Implementation(this);

}

void AArmorBase::ThrowToWorld(FVector WorldOffset)
{
	Super::ThrowToWorld(WorldOffset);
	ShowInWorld();
}

void AArmorBase::ShowFront()
{
	if (PresentSide == FRONT_SIDE)
		return;

	TArray<UPrimitiveComponent*> PastComponents = GetComponentArray(PresentSide);
	PresentSide = FRONT_SIDE;

	for (auto comp : PastComponents)
		comp->SetHiddenInGame(true);

	for (auto comp : FrontComponents)
		comp->SetHiddenInGame(false);

}

void AArmorBase::ShowBack()
{
	if (PresentSide == BACK_SIDE)
		return;

	TArray<UPrimitiveComponent*> PastComponents = GetComponentArray(PresentSide);
	PresentSide = BACK_SIDE;

	for (auto comp : PastComponents)
		comp->SetHiddenInGame(true);

	for (auto comp : BackComponents)
		comp->SetHiddenInGame(false);
}

void AArmorBase::ShowRight()
{
	if (PresentSide == RIGHT_SIDE)
		return;

	TArray<UPrimitiveComponent*> PastComponents = GetComponentArray(PresentSide);
	PresentSide = RIGHT_SIDE;

	for (auto comp : PastComponents)
		comp->SetHiddenInGame(true);

	for (auto comp : RightComponents)
		comp->SetHiddenInGame(false);

}

void AArmorBase::ShowLeft()
{
	if (PresentSide == LEFT_SIDE)
		return;

	TArray<UPrimitiveComponent*> PastComponents = GetComponentArray(PresentSide);
	PresentSide = LEFT_SIDE;

	for (auto comp : PastComponents)
		comp->SetHiddenInGame(true);

	for (auto comp : LeftComponents)
		comp->SetHiddenInGame(false);
}

void AArmorBase::ShowSide(uint8 Side)
{
	if (Side == LEFT_SIDE) ShowLeft();
	else if (Side == RIGHT_SIDE) ShowRight();
	else if (Side == BACK_SIDE) ShowBack();
	else ShowFront();
}



void AArmorBase::ShowInWorld()
{
	for (auto comp : LeftComponents)
		comp->SetHiddenInGame(true);

	for (auto comp : RightComponents)
		comp->SetHiddenInGame(true);

	for (auto comp : BackComponents)
		comp->SetHiddenInGame(true);

	ShowFront();
	if (Category != EItemCategoryEnum::ICE_ArmorAmulet
		&& Category != EItemCategoryEnum::ICE_ArmorNecklace)
		SetActorRelativeRotation(WorldRotation);
}

TArray<UPrimitiveComponent*>& AArmorBase::GetComponentArray(uint8 Side)
{
	if (Side == LEFT_SIDE) return LeftComponents;
	else if (Side == RIGHT_SIDE) return RightComponents;
	else if (Side == BACK_SIDE) return BackComponents;
	else return FrontComponents;
}
