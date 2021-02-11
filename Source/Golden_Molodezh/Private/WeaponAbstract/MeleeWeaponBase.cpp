// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponBase.h"

AMeleeWeaponBase::AMeleeWeaponBase()
	:Super()
{
	HardPart = CreateDefaultSubobject<USphereComponent>(TEXT("HardPart"));
	HardPart->SetCollisionProfileName(TEXT("WeaponHard"));
	HardPart->SetSphereRadius(16);
	//HardPart->AttachToComponent(RotationPart, FAttachmentTransformRules::KeepRelativeTransform);

	DamagePart = CreateDefaultSubobject<USphereComponent>(TEXT("DamagePart"));
	DamagePart->SetCollisionProfileName(TEXT("OverlapHitBox"));
	DamagePart->SetSphereRadius(16);
	DamagePart->AttachToComponent(RotationPart, FAttachmentTransformRules::KeepRelativeTransform);
	//DamagePart->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::BeginOverlapDamagePart);
	


	DamageRate = 1;
}

void AMeleeWeaponBase::BeginOverlapDamagePart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bPickedUp || !bIsMoving || !bCanDamage || !OtherActor )
		return;

	FHitResult HitResult;
	UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, GetActorLocation(), HitResult, nullptr, this, nullptr);
	bCanDamage = false;
	//GetWorld()->GetTimerManager().SetTimer(DamageRateTimerHandler, this, &AMeleeWeaponBase::OnCanDamage, 0.01f, false);
}

void AMeleeWeaponBase::OnCanDamage()
{
	bCanDamage = true;
}

void AMeleeWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(DamageRateTimerHandler);
}


