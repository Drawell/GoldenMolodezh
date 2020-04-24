// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponBase.h"

AMeleeWeaponBase::AMeleeWeaponBase()
{
	RotationPart = CreateDefaultSubobject<USphereComponent>(TEXT("RotationPart"));
	RotationPart->SetCollisionProfileName(TEXT("NoCollision"));
	RotationPart->SetSphereRadius(1);
	RotationPart->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	HardPart = CreateDefaultSubobject<USphereComponent>(TEXT("HardPart"));
	HardPart->SetCollisionProfileName(TEXT("HardBody"));
	HardPart->SetSphereRadius(16);
	HardPart->AttachToComponent(RotationPart, FAttachmentTransformRules::KeepRelativeTransform);

	DamagePart = CreateDefaultSubobject<USphereComponent>(TEXT("DamagePart"));
	DamagePart->SetCollisionProfileName(TEXT("OverlapHitBox"));
	DamagePart->SetSphereRadius(16);
	DamagePart->AttachToComponent(RotationPart, FAttachmentTransformRules::KeepRelativeTransform);
	DamagePart->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::BeginOverlapDamagePart);
	Amount = 1;

	DistanceMax = 150;
	DistanceMin = 40;
	MovementSpeed = 15;
	DamageRate = 1;
	PrimaryActorTick.bCanEverTick = true;

}

void AMeleeWeaponBase::StartUse(float X, float Y)
{

	//X *= KoefFromScreenToLocal;
	//Y *= KoefFromScreenToLocal;
	FVector start(X, Y, 0);
	start.Normalize();
	start *= DistanceMin;
	start.Z = StartZ;

	SetActorRelativeLocation(start);
	Super::StartUse(X, Y);
}

void AMeleeWeaponBase::ContinueUse(float X, float Y)
{
	if (!bIsUsing)
		return;

	Destination.X = X;// KoefFromScreenToLocal* X;
	Destination.Y = Y;// KoefFromScreenToLocal* Y;

	if (FMath::IsNearlyZero((Destination - RootComponent->GetRelativeLocation()).Size()))
		bNeedToMove = false;
	else
		bNeedToMove = true;


	//this->SetActorRelativeLocation(NewLocation);
}

void AMeleeWeaponBase::StopUse(float X, float Y)
{
	Super::StopUse(X, Y);
	bNeedToMove = false;	
}

void AMeleeWeaponBase::BeginOverlapDamagePart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bPickedUp || !bIsUsing || !bCanDamage || !OtherActor )
		return;

	FHitResult HitResult;
	UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, GetActorLocation(), HitResult, nullptr, this, nullptr);
	bCanDamage = false;
	GetWorld()->GetTimerManager().SetTimer(DamageRateTimerHandler, this, &AMeleeWeaponBase::OnCanDamage, 1.f / DamageRate, false);
}

void AMeleeWeaponBase::OnCanDamage()
{
	bCanDamage = true;
}


void AMeleeWeaponBase::Tick(float DeltaTime)
{
	if (!bIsUsing || !bNeedToMove)
		return;

	FVector move = Destination - RootComponent->GetRelativeLocation();
	move.Z = 0;
	move *= (DeltaTime * MovementSpeed);
	AddActorLocalOffset(move);

	//Place in moving scope
	FVector2D newLocation2(RootComponent->GetRelativeLocation());
	int distance = newLocation2.Size();
	distance = FMath::Clamp(distance, DistanceMin, DistanceMax);
	newLocation2.Normalize();
	newLocation2 *= distance;
	RootComponent->SetRelativeLocation(FVector(newLocation2, StartZ));

	float newYaw = FMath::RadiansToDegrees(FMath::Atan2(newLocation2.Y, newLocation2.X));
	RotationPart->SetRelativeRotation(FRotator(		
		RotationPart->GetRelativeRotation().Pitch,
		newYaw + 90,
		RotationPart->GetRelativeRotation().Roll));
	//Calculate New Lockation

}

void AMeleeWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(DamageRateTimerHandler);
}


