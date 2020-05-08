// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
	:Super()
{
	Category = EItemCategoryEnum::ICE_WeaponBothHand;
}

void AWeaponBase::StartUse(float X, float Y)
{	
	StartX = X;
	StartY = Y;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	bIsUsing = true;
}

void AWeaponBase::ContinueUse(float X, float Y)
{

}

void AWeaponBase::StopUse(float X, float Y)
{
	bIsUsing = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

}

void AWeaponBase::Use(AActor* User)
{
	if (User == nullptr)
		return;

	IEquippedMobInterface* user = Cast<IEquippedMobInterface>(User);
	if (user == nullptr)
		return;

	if (user->CanEquip_Implementation(this))
		user->Equip_Implementation(this);

}
