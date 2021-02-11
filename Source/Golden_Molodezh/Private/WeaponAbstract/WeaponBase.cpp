// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
	:Super()
{
	Category = EItemCategoryEnum::ICE_WeaponBothHand;
	Amount = 1;
}

void AWeaponBase::StartMove(float X, float Y)
{	
	StartX = X;
	StartY = Y;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	bIsMoving = true;
}

void AWeaponBase::ContinueMove(float ToX, float ToY)
{

}

void AWeaponBase::StopMove(float X, float Y)
{
	bIsMoving = false;
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
