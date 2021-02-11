// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowingWeapon.h"

AFollowingWeapon::AFollowingWeapon()
	:Super()
{
	RotationPart = CreateDefaultSubobject<USphereComponent>(TEXT("RotationPart"));
	RotationPart->SetCollisionProfileName(TEXT("WeaponHard"));
	RotationPart->SetSphereRadius(1);
	RotationPart->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	RotationPart->SetConstraintMode(EDOFMode::Type::Default);
	RotationPart->SetLinearDamping(5);
	RotationPart->SetEnableGravity(false);
	RotationPart->BodyInstance.bLockXRotation = true;
	RotationPart->BodyInstance.bLockYRotation = true;
	
	//RotationPart->Cons

	WeaponConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("WeaponConstraint"));
	WeaponConstraint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	WeaponConstraint->ComponentName1.ComponentName = RootComponent->GetFName();
	WeaponConstraint->ComponentName2.ComponentName = RotationPart->GetFName();
	WeaponConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 10);

	//WeaponConstraint->SetConstrainedComponents(RootComponent, FName(), RotationPart, FName());


	PrimaryActorTick.bCanEverTick = true;
	LengthwiseSpeed = 10.f;
	CrossSpeed = 10.f;
	DistanceMax = 100;
	DistanceMin = 40;
	Destination.Z = 0;
}

void AFollowingWeapon::StartMove(float X, float Y)
{

	//X *= KoefFromScreenToLocal;
	//Y *= KoefFromScreenToLocal;
	FVector start(X, Y, 0);
	start.Normalize();
	start *= DistanceMin;
	start += ActorToWorld().GetLocation();
	//start.Z = StartZ;

	//to avoid selfdamage
	bCanDamage = false;
	//GetWorld()->GetTimerManager().SetTimer(DamageRateTimerHandler, this, &AMeleeWeaponBase::OnCanDamage, 0.02f, false);
	//SetActorRelativeLocation(start, false, nullptr, ETeleportType::TeleportPhysics);
	

	Super::StartMove(X, Y);
	SetActorEnableCollision(true);
	RotationPart->SetSimulatePhysics(true);
	RotationPart->SetWorldLocation(start, false, nullptr, ETeleportType::TeleportPhysics);

	RotationPart->SetPhysicsLinearVelocity(FVector::ZeroVector);

	if (RotationPart->IsSimulatingPhysics())
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("TRUE"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("FALSE"));
}

void AFollowingWeapon::ContinueMove(float ToX, float ToY)
{
	if (!bIsMoving)
		return;

	Destination.X = ToX;
	Destination.Y = ToY;

	if (FMath::IsNearlyZero((Destination - RotationPart->GetRelativeLocation()).Size()))
		bNeedToMove = false;
	else
		bNeedToMove = true;
}

void AFollowingWeapon::StopMove(float X, float Y)
{
	Super::StopMove(X, Y);
	bNeedToMove = false;
	RotationPart->SetPhysicsLinearVelocity(FVector::ZeroVector);
	RotationPart->SetSimulatePhysics(false);
	RotationPart->SetRelativeLocation(FVector::ZeroVector, false, nullptr, ETeleportType::TeleportPhysics);
	RotationPart->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);	
	SetActorEnableCollision(false);
}

void AFollowingWeapon::Tick(float DeltaTime)
{
	if (!bIsMoving || !bNeedToMove)
		return;
	
	//when we enable phisycs relative location turn into world
	FVector relativeLocation = RotationPart->GetRelativeLocation() - this->ActorToWorld().GetLocation();
	
	FVector2D lengthwise = FVector2D(relativeLocation.X, relativeLocation.Y);
	lengthwise.Normalize();
	FVector2D cross = FVector2D(lengthwise.Y, -lengthwise.X);
	cross.Normalize();
	FVector2D direction = FVector2D(Destination - relativeLocation);

	// cross vector must directed in direction vectror
	if (FVector2D::DotProduct(direction, cross) < 0)
	{
		cross.Y = -cross.Y;
		cross.X = -cross.X;
	}

	lengthwise = lengthwise * (FVector2D::DotProduct(lengthwise, direction) * LengthwiseSpeed);
	cross = cross * (FVector2D::DotProduct(cross, direction) * CrossSpeed);
	FVector move = FVector(lengthwise + cross, 0);

	RotationPart->AddForce(move, NAME_None, true);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, move.ToString());

	float newYaw = FMath::RadiansToDegrees(FMath::Atan2(
		relativeLocation.Y, relativeLocation.X));

	RotationPart->SetWorldRotation(FRotator(
		RotationPart->GetRelativeRotation().Pitch,
		newYaw + 90,
		RotationPart->GetRelativeRotation().Roll));

}

void AFollowingWeapon::PostInitProperties()
{
	Super::PostInitProperties();
	WeaponConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, DistanceMax);
	WeaponConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, DistanceMax);
}