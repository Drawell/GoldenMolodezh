// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
	:Super()
{

}

void AWeaponBase::StartUse(float X, float Y)
{
	bIsUsing = true;
	StartX = X;
	StartY = Y;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AWeaponBase::ContinueUse(float X, float Y)
{

}

void AWeaponBase::StopUse(float X, float Y)
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	bIsUsing = false;

}